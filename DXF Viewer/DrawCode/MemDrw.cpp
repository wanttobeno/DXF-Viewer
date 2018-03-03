/*-------------------------------------------------------------------*\
|  CadLib Version 2.1                                                 |
|  Written by Omid Shahabi <omid_shahabi@hotmail.com>                 |
|  Copyright ?2002-2004                                              |
|  Pars Technology Development Co.                                    |
|                                                                     |
|  This software is distributed on an "AS IS" basis, WITHOUT WARRANTY |
|  OF ANY KIND, either express or implied.                            |
|                                                                     |
|                                                                     |
|  MemDrw.cpp                                                         |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "MemDrw.h"
#include "View\DimView.h"

/********************************************************************
 * HDRAWING -> PDRAWING Functions                                   *
 ********************************************************************/

PDRAWING InitilizePDRAWING(HDRAWING hDrawing)
{
	PDRAWING pDrawing;

	if( hDrawing == NULL )
	{
		// hDrawing shall not be null
		return NULL;
	}

	pDrawing = (PDRAWING)GlobalLock(hDrawing);
	if(pDrawing==NULL)
	{
		// GlobalLock error
		return NULL;
	}

	return pDrawing;
}

BOOL UnInitilizePDRAWING(HDRAWING hDrawing)
{
	// Unlock hDrawing -------------------
	GlobalUnlock(hDrawing);
/*	DWORD ii;
	if(!GlobalUnlock(hDrawing) && ((ii=GetLastError())!=NO_ERROR))
	{
		// memory error
		return FALSE;
	}*/

	return TRUE;
}

/********************************************************************
 * Construction & Destruction Functions                             *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  drwCreate                                                        |
 |  Inputs:                                                          |
 |      Nothing!                                                     |
 |  Output: Handle to new created drawing ( NULL=Error )             |
 *-------------------------------------------------------------------*/
HDRAWING drwCreate( )
{
	HDRAWING	hDrawing;
	PDRAWING	pDrawing;

	hDrawing = (HDRAWING)GlobalAlloc(GHND, sizeof(DRAWING));
	if(hDrawing==NULL)
	{
		// GlobalAlloc error
		return NULL;
	}

	pDrawing = (PDRAWING)GlobalLock(hDrawing);
	if(pDrawing==NULL)
	{
		// GlobalLock error
		GlobalFree(hDrawing);
		return NULL;
	}
	
	ZeroMemory(pDrawing, sizeof(DRAWING));

//	pDrawing->Tables.CurrentLayerHandle = 0;	// Layer 0
//	pDrawing->Tables.CurrentTextStyleHandle = 0;// Standard
//	pDrawing->Tables.CurrentDimStyleHandle = 0;	// Default
//	pDrawing->Tables.CurrentLineTypeHandle = 0;	// ByLayer
//	pDrawing->Tables.CurrentColor = 256;		// ByLayer
//	pDrawing->Tables.CurrentLineWeight = 0;		// ByLayer

	pDrawing->Tables.NumLayers = 0;
	pDrawing->Tables.NumLTypes = 0;
	pDrawing->Tables.NumStyles = 0;
	pDrawing->Tables.NumDimStyles = 0;

	pDrawing->Tables.CurrentLayer = 0;
	pDrawing->Tables.CurrentLType = 0;
	pDrawing->Tables.CurrentStyle = 0;
	pDrawing->Tables.CurrentDimStyle = 0;

	pDrawing->BlocksNumber = 0;
	pDrawing->CurrentBlock = 0;

	pDrawing->LastObjhandle = 15;
	pDrawing->LastDimBlkNum = 0;

	// Initilize Entities Section
	pDrawing->Entities.hEntities = (HDRAWING)GlobalAlloc(GHND, 65536);
	if(pDrawing->Entities.hEntities==NULL)
	{
		// GlobalAlloc error
		GlobalUnlock(hDrawing);
		GlobalFree(hDrawing);
		return NULL;
	}

	pDrawing->Entities.TotalSize = GlobalSize(pDrawing->Entities.hEntities);
	pDrawing->Entities.FreePos = 0;
	pDrawing->Entities.LastEntityPos = 0;
	pDrawing->Entities.CurrentEntityPos = 0;
	pDrawing->Entities.EntitiesNumber = 0;

	pDrawing->View.Viewable = TRUE;

	GlobalUnlock(hDrawing);

	return hDrawing;
}

/*-------------------------------------------------------------------*
 |  drwDestroy                                                       |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the drawing to be destroyed    |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwDestroy( HDRAWING hDrawing )
{
	PDRAWING		pDrawing;
	ENTITYHEADER	EntityHeader;
	char			EntityData[4096];
	BLOCKHEADER		BlockHeader;
	PENTPOLYLINE	pPLine;

	// Initialize pDrawing --------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	pDrawing->View.Viewable = FALSE;

	// Free all allocated memory by polylines before destroy entire drawing
	// Entities Section
	if(drwFindEntity_Direct(pDrawing, NULL, &EntityHeader, &EntityData, FIND_FIRST)>0)
	{
		do{
			if(EntityHeader.EntityType==ENT_POLYLINE)
			{
				pPLine = ((PENTPOLYLINE)&EntityData);
				delete ((PENTPOLYLINE)&EntityData)->pVertex;
			}
		} while(drwFindEntity_Direct(pDrawing, NULL, &EntityHeader, &EntityData, FIND_NEXT)>0);
	}
	// Blocks Section
	if(drwFindBlock_Direct(pDrawing, FIND_FIRST, &BlockHeader)>0)
	{
		do{
			if(drwFindEntity_Direct(pDrawing, BlockHeader.Name, &EntityHeader, &EntityData, FIND_FIRST)>0)
			{
				do{
					if(EntityHeader.EntityType==ENT_POLYLINE)
						delete ((PENTPOLYLINE)&EntityData)->pVertex;
				} while(drwFindEntity_Direct(pDrawing, BlockHeader.Name, &EntityHeader, &EntityData, FIND_NEXT)>0);
			}
		} while(drwFindBlock_Direct(pDrawing, FIND_NEXT, &BlockHeader)>0);
	}


	// Free Entities Memory
	GlobalFree(pDrawing->Entities.hEntities);

	// Free Blocks Memory
	for(unsigned int i=0; i<pDrawing->BlocksNumber; i++)
		GlobalFree(pDrawing->Blocks[i].Entities.hEntities);

	// Unlock hDrawing ------------------
	if(GlobalUnlock(hDrawing)>0)
	{
		// memory error
		return FALSE;
	}

	if(GlobalFree(hDrawing)!=NULL)
	{
		// error in GlobalFree function
		return FALSE;
	}

	return TRUE;
}


/********************************************************************
 * TABLES Section                                                   *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  drwFindTableType_Direct                                          |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      DWORD dwTableType = table type to get data                   |
 |                        ( TAB_NOTSET = search in all table types ) |
 |      DWORD dwFindType = find type                                 |
 |      LPVOID pTableType = pointer to table type structure          |
 |  Output: Handle of found table type (0=Error or Not found)        |
 *-------------------------------------------------------------------*/
OBJHANDLE drwFindTableType_Direct( PDRAWING pDrawing, DWORD dwTableType, DWORD dwFindType, LPVOID pTableType )
{
	OBJHANDLE	result=0;

	if(dwFindType & FIND_FIRST)
	{
		switch(dwTableType)
		{
		case TAB_DIMSTYLE:
			if(pDrawing->Tables.NumDimStyles<=0)
				break;
			pDrawing->Tables.CurrentDimStyle = 0;
			CopyMemory(pTableType, &pDrawing->Tables.DimStyle[pDrawing->Tables.CurrentDimStyle], sizeof(DIMSTYLE));
			result = pDrawing->Tables.DimStyle[pDrawing->Tables.CurrentDimStyle].Objhandle;
			break;
		case TAB_LAYER:
			if(pDrawing->Tables.NumLayers<=0)
				break;
			pDrawing->Tables.CurrentLayer = 0;
			CopyMemory(pTableType, &pDrawing->Tables.Layer[pDrawing->Tables.CurrentLayer], sizeof(LAYER));
			result = pDrawing->Tables.Layer[pDrawing->Tables.CurrentLayer].Objhandle;
			break;
		case TAB_LTYPE:
			if(pDrawing->Tables.NumLTypes<=0)
				break;
			pDrawing->Tables.CurrentLType = 0;
			CopyMemory(pTableType, &pDrawing->Tables.LType[pDrawing->Tables.CurrentLType], sizeof(LTYPE));
			result = pDrawing->Tables.LType[pDrawing->Tables.CurrentLType].Objhandle;
			break;
		case TAB_STYLE:
			if(pDrawing->Tables.NumStyles<=0)
				break;
			pDrawing->Tables.CurrentStyle = 0;
			CopyMemory(pTableType, &pDrawing->Tables.Style[pDrawing->Tables.CurrentStyle], sizeof(STYLE));
			result = pDrawing->Tables.Style[pDrawing->Tables.CurrentStyle].Objhandle;
			break;
		}
	}
	else if(dwFindType & FIND_LAST)
	{
		switch(dwTableType)
		{
		case TAB_DIMSTYLE:
			if(pDrawing->Tables.NumDimStyles<=0)
				break;
			pDrawing->Tables.CurrentDimStyle = pDrawing->Tables.NumDimStyles - 1;
			CopyMemory(pTableType, &pDrawing->Tables.DimStyle[pDrawing->Tables.CurrentDimStyle], sizeof(DIMSTYLE));
			result = pDrawing->Tables.DimStyle[pDrawing->Tables.CurrentDimStyle].Objhandle;
			break;
		case TAB_LAYER:
			if(pDrawing->Tables.NumLayers<=0)
				break;
			pDrawing->Tables.CurrentLayer = pDrawing->Tables.NumLayers - 1;
			CopyMemory(pTableType, &pDrawing->Tables.Layer[pDrawing->Tables.CurrentLayer], sizeof(LAYER));
			result = pDrawing->Tables.Layer[pDrawing->Tables.CurrentLayer].Objhandle;
			break;
		case TAB_LTYPE:
			if(pDrawing->Tables.NumLTypes<=0)
				break;
			pDrawing->Tables.CurrentLType = pDrawing->Tables.NumLTypes - 1;
			CopyMemory(pTableType, &pDrawing->Tables.LType[pDrawing->Tables.CurrentLType], sizeof(LTYPE));
			result = pDrawing->Tables.LType[pDrawing->Tables.CurrentLType].Objhandle;
			break;
		case TAB_STYLE:
			if(pDrawing->Tables.NumStyles<=0)
				break;
			pDrawing->Tables.CurrentStyle = pDrawing->Tables.NumStyles - 1;
			CopyMemory(pTableType, &pDrawing->Tables.Style[pDrawing->Tables.CurrentStyle], sizeof(STYLE));
			result = pDrawing->Tables.Style[pDrawing->Tables.CurrentStyle].Objhandle;
			break;
		}
	}
	else if(dwFindType & FIND_PREVIOUS)
	{
		switch(dwTableType)
		{
		case TAB_DIMSTYLE:
			if(pDrawing->Tables.CurrentDimStyle<=0)
				break;
			pDrawing->Tables.CurrentDimStyle--;
			CopyMemory(pTableType, &pDrawing->Tables.DimStyle[pDrawing->Tables.CurrentDimStyle], sizeof(DIMSTYLE));
			result = pDrawing->Tables.DimStyle[pDrawing->Tables.CurrentDimStyle].Objhandle;
			break;
		case TAB_LAYER:
			if(pDrawing->Tables.CurrentLayer<=0)
				break;
			pDrawing->Tables.CurrentLayer--;
			CopyMemory(pTableType, &pDrawing->Tables.Layer[pDrawing->Tables.CurrentLayer], sizeof(LAYER));
			result = pDrawing->Tables.Layer[pDrawing->Tables.CurrentLayer].Objhandle;
			break;
		case TAB_LTYPE:
			if(pDrawing->Tables.CurrentLType<=0)
				break;
			pDrawing->Tables.CurrentLType--;
			CopyMemory(pTableType, &pDrawing->Tables.LType[pDrawing->Tables.CurrentLType], sizeof(LTYPE));
			result = pDrawing->Tables.LType[pDrawing->Tables.CurrentLType].Objhandle;
			break;
		case TAB_STYLE:
			if(pDrawing->Tables.CurrentStyle<=0)
				break;
			pDrawing->Tables.CurrentStyle--;
			CopyMemory(pTableType, &pDrawing->Tables.Style[pDrawing->Tables.CurrentStyle], sizeof(STYLE));
			result = pDrawing->Tables.Style[pDrawing->Tables.CurrentStyle].Objhandle;
			break;
		}
	}
	else if(dwFindType & FIND_NEXT)
	{
		switch(dwTableType)
		{
		case TAB_DIMSTYLE:
			if(pDrawing->Tables.CurrentDimStyle>=pDrawing->Tables.NumDimStyles-1)
				break;
			pDrawing->Tables.CurrentDimStyle++;
			CopyMemory(pTableType, &pDrawing->Tables.DimStyle[pDrawing->Tables.CurrentDimStyle], sizeof(DIMSTYLE));
			result = pDrawing->Tables.DimStyle[pDrawing->Tables.CurrentDimStyle].Objhandle;
			break;
		case TAB_LAYER:
			if(pDrawing->Tables.CurrentLayer>=pDrawing->Tables.NumLayers-1)
				break;
			pDrawing->Tables.CurrentLayer++;
			CopyMemory(pTableType, &pDrawing->Tables.Layer[pDrawing->Tables.CurrentLayer], sizeof(LAYER));
			result = pDrawing->Tables.Layer[pDrawing->Tables.CurrentLayer].Objhandle;
			break;
		case TAB_LTYPE:
			if(pDrawing->Tables.CurrentLType>=pDrawing->Tables.NumLTypes-1)
				break;
			pDrawing->Tables.CurrentLType++;
			CopyMemory(pTableType, &pDrawing->Tables.LType[pDrawing->Tables.CurrentLType], sizeof(LTYPE));
			result = pDrawing->Tables.LType[pDrawing->Tables.CurrentLType].Objhandle;
			break;
		case TAB_STYLE:
			if(pDrawing->Tables.CurrentStyle>=pDrawing->Tables.NumStyles-1)
				break;
			pDrawing->Tables.CurrentStyle++;
			CopyMemory(pTableType, &pDrawing->Tables.Style[pDrawing->Tables.CurrentStyle], sizeof(STYLE));
			result = pDrawing->Tables.Style[pDrawing->Tables.CurrentStyle].Objhandle;
			break;
		}
	}
	else if(dwFindType & (FIND_BYHANDLE | FIND_BYNAME))
	{
		if((dwTableType==TAB_NOTSET) || (dwTableType==TAB_DIMSTYLE))
			for(UINT i=0; i<pDrawing->Tables.NumDimStyles; i++)
				if( ((pDrawing->Tables.DimStyle[i].Objhandle==((PDIMSTYLE)pTableType)->Objhandle) && (dwFindType & FIND_BYHANDLE))
				  ||((strcmp(pDrawing->Tables.DimStyle[i].Name,((PDIMSTYLE)pTableType)->Name)==0) && (dwFindType & FIND_BYNAME)) )
				{
					dwTableType = TAB_DIMSTYLE;
					CopyMemory(pTableType, &pDrawing->Tables.DimStyle[i], sizeof(DIMSTYLE));
					result = pDrawing->Tables.DimStyle[i].Objhandle;
				}

		if((dwTableType==TAB_NOTSET) || (dwTableType==TAB_LAYER))
			for(UINT i=0; i<pDrawing->Tables.NumLayers; i++)
				if( ((pDrawing->Tables.Layer[i].Objhandle==((PLAYER)pTableType)->Objhandle) && (dwFindType & FIND_BYHANDLE))
				  ||((strcmp(pDrawing->Tables.Layer[i].Name,((PLAYER)pTableType)->Name)==0) && (dwFindType & FIND_BYNAME)) )
				{
					dwTableType = TAB_LAYER;
					CopyMemory(pTableType, &pDrawing->Tables.Layer[i], sizeof(LAYER));
					result = pDrawing->Tables.Layer[i].Objhandle;
				}

		if((dwTableType==TAB_NOTSET) || (dwTableType==TAB_LTYPE))
			for(UINT i=0; i<pDrawing->Tables.NumLTypes; i++)
				if( ((pDrawing->Tables.LType[i].Objhandle==((PLTYPE)pTableType)->Objhandle) && (dwFindType & FIND_BYHANDLE))
				  ||((strcmp(pDrawing->Tables.LType[i].Name,((PLTYPE)pTableType)->Name)==0) && (dwFindType & FIND_BYNAME)) )
				{
					dwTableType = TAB_LTYPE;
					CopyMemory(pTableType, &pDrawing->Tables.LType[i], sizeof(LTYPE));
					result = pDrawing->Tables.LType[i].Objhandle;
				}

		if((dwTableType==TAB_NOTSET) || (dwTableType==TAB_STYLE))
			for(UINT i=0; i<pDrawing->Tables.NumStyles; i++)
				if( ((pDrawing->Tables.Style[i].Objhandle==((PSTYLE)pTableType)->Objhandle) && (dwFindType & FIND_BYHANDLE))
				  ||((strcmp(pDrawing->Tables.Style[i].Name,((PSTYLE)pTableType)->Name)==0) && (dwFindType & FIND_BYNAME)))
				{
					dwTableType = TAB_STYLE;
					CopyMemory(pTableType, &pDrawing->Tables.Style[i], sizeof(STYLE));
					result = pDrawing->Tables.Style[i].Objhandle;
				}
	}

	return result;
}

/*-------------------------------------------------------------------*
 |  drwFindTableType                                                 |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      DWORD dwTableType = table type to get data                   |
 |                        ( TAB_NOTSET = search in all table types ) |
 |      DWORD dwFindType = find type                                 |
 |      LPVOID pTableType = pointer to table type structure          |
 |  Output: Handle of found table type (0=Error or Not found)        |
 *-------------------------------------------------------------------*/
OBJHANDLE drwFindTableType( HDRAWING hDrawing, DWORD dwTableType, DWORD dwFindType, LPVOID pTableType )
{
	PDRAWING	pDrawing;
	OBJHANDLE	result;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return 0;

	result = drwFindTableType_Direct(pDrawing, dwTableType, dwFindType, pTableType);

	// UnInitilize pDrawing -----------------
	UnInitilizePDRAWING(hDrawing);

	return result;
}

/*-------------------------------------------------------------------*
 |  drwAddTableType                                                  |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      DWORD dwTableType = table type to be added                   |
 |      LPVOID pTableType = pointer to table type structure to add   |
 |  Output: Handle of new table type (0=Error)                       |
 *-------------------------------------------------------------------*/
OBJHANDLE drwAddTableType( HDRAWING hDrawing, DWORD dwTableType, LPVOID pTableType )
{
	PDRAWING	pDrawing;
	int			CurrentTType;
	OBJHANDLE	NewTableTypeObjhandle;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return 0;

	// Check for duplication
	if(drwFindTableType_Direct(pDrawing, dwTableType, FIND_BYNAME, pTableType)!=0)
	{
		// TableType already exists
		GlobalUnlock(hDrawing);
		return 0;
	}

	NewTableTypeObjhandle = pDrawing->LastObjhandle + 1;

	switch( dwTableType )
	{
	case TAB_DIMSTYLE:
		if(pDrawing->Tables.NumDimStyles>=MAX_DIMSTYLES)
		{
			// Cannot add more DimStyles
			GlobalUnlock(hDrawing);
			return 0;
		}

		STYLE		Style;
		Style.Objhandle = ((PDIMSTYLE)pTableType)->dimtxstyObjhandle;
		if(drwFindTableType(hDrawing, TAB_STYLE, FIND_BYHANDLE, &Style)<=0)
		{
			// Cannot find textstyle used for this dimstyle
			GlobalUnlock(hDrawing);
			return 0;
		}

		CurrentTType = pDrawing->Tables.NumDimStyles;
		CopyMemory(&pDrawing->Tables.DimStyle[CurrentTType], pTableType, sizeof(DIMSTYLE));
		pDrawing->Tables.DimStyle[CurrentTType].Objhandle = NewTableTypeObjhandle;
		pDrawing->Tables.NumDimStyles++;
		break;

	case TAB_LAYER:
		if(pDrawing->Tables.NumLayers>=MAX_LAYERS)
		{
			// Cannot add more Layers
			GlobalUnlock(hDrawing);
			return 0;
		}

		LTYPE	LType;
		LType.Objhandle = ((PLAYER)pTableType)->LineTypeObjhandle;
		if(drwFindTableType(hDrawing, TAB_LTYPE, FIND_BYHANDLE, &LType)<=0)
		{
			// Cannot find linetype used for this layer
			GlobalUnlock(hDrawing);
			return 0;
		}

		CurrentTType = pDrawing->Tables.NumLayers;
		CopyMemory(&pDrawing->Tables.Layer[CurrentTType], pTableType, sizeof(LAYER));
		pDrawing->Tables.Layer[CurrentTType].Objhandle = NewTableTypeObjhandle;
		pDrawing->Tables.NumLayers++;
		break;

	case TAB_LTYPE:
		if(pDrawing->Tables.NumLTypes>=MAX_LTYPES)
		{
			// Cannot add more LineTypes
			GlobalUnlock(hDrawing);
			return 0;
		}

		CurrentTType = pDrawing->Tables.NumLTypes;
		CopyMemory(&pDrawing->Tables.LType[CurrentTType], pTableType, sizeof(LTYPE));
		pDrawing->Tables.LType[CurrentTType].Objhandle = NewTableTypeObjhandle;
		pDrawing->Tables.NumLTypes++;
		break;

	case TAB_STYLE:
		if(pDrawing->Tables.NumStyles>=MAX_STYLES)
		{
			// Cannot add more TextStyles
			GlobalUnlock(hDrawing);
			return 0;
		}

		CurrentTType = pDrawing->Tables.NumStyles;
		CopyMemory(&pDrawing->Tables.Style[CurrentTType], pTableType, sizeof(STYLE));
		pDrawing->Tables.Style[CurrentTType].Objhandle = NewTableTypeObjhandle;
		pDrawing->Tables.NumStyles++;
		break;

	default:
		// Unknown Table Type
		GlobalUnlock(hDrawing);
		return 0;
	}

	pDrawing->LastObjhandle++;

	// UnInitilize pDrawing -----------------
	UnInitilizePDRAWING(hDrawing);

	return NewTableTypeObjhandle;
}

/*-------------------------------------------------------------------*
 |  drwDeleteTableType                                               |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      DWORD dwTableType = table type to be removed                 |
 |      OBJHANDLE TableTypeObjhandle = handle of table type to remove|
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwDeleteTableType( HDRAWING hDrawing, DWORD dwTableType, OBJHANDLE TableTypeObjhandle )
{
	PDRAWING	pDrawing;
	BOOL		result;
	UINT		i, j;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	result = FALSE;
	switch( dwTableType )
	{
	case TAB_DIMSTYLE:
		for(i=0; i<pDrawing->Tables.NumDimStyles; i++)
			if(pDrawing->Tables.DimStyle[i].Objhandle == TableTypeObjhandle)
			{
				for(j=pDrawing->Tables.NumDimStyles-1; j>i; j--)
					pDrawing->Tables.DimStyle[j] = pDrawing->Tables.DimStyle[j-1];
				result = TRUE;
			}
		pDrawing->Tables.NumDimStyles--;
		break;

	case TAB_LAYER:
		for(i=0; i<pDrawing->Tables.NumLayers; i++)
			if(pDrawing->Tables.Layer[i].Objhandle == TableTypeObjhandle)
			{
				for(j=pDrawing->Tables.NumLayers-1; j>i; j--)
					pDrawing->Tables.Layer[j] = pDrawing->Tables.Layer[j-1];
				result = TRUE;
			}
		pDrawing->Tables.NumLayers--;
		break;

	case TAB_LTYPE:
		for(i=0; i<pDrawing->Tables.NumLTypes; i++)
			if(pDrawing->Tables.LType[i].Objhandle == TableTypeObjhandle)
			{
				for(j=pDrawing->Tables.NumLTypes-1; j>i; j--)
					pDrawing->Tables.LType[j] = pDrawing->Tables.LType[j-1];
				result = TRUE;
			}
		pDrawing->Tables.NumLTypes--;
		break;

	case TAB_STYLE:
		for(i=0; i<pDrawing->Tables.NumStyles; i++)
			if(pDrawing->Tables.Style[i].Objhandle == TableTypeObjhandle)
			{
				for(j=pDrawing->Tables.NumStyles-1; j>i; j--)
					pDrawing->Tables.Style[j] = pDrawing->Tables.Style[j-1];
				result = TRUE;
			}
		pDrawing->Tables.NumStyles--;
		break;

	default: // Table Type is not accepted
		GlobalUnlock(hDrawing);
		return FALSE;
	}

	if(!result)
	{
		// Table type is not found
		GlobalUnlock(hDrawing);
		return FALSE;
	}

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}


/********************************************************************
 * BLOCKS Section                                                   *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  drwFindBlock_Direct                                              |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      DWORD dwFindType = find type                                 |
 |      PBLOCKHEADER pBlockHeader = pointer to block header data     |
 |  Output: Handle of found block (0=Error or Not found)             |
 *-------------------------------------------------------------------*/
OBJHANDLE drwFindBlock_Direct( PDRAWING pDrawing, DWORD dwFindType, PBLOCKHEADER pBlockHeader )
{
	OBJHANDLE		result;
	bool			isBlockFound;
	unsigned int	i;

	if(pDrawing->BlocksNumber<=0)
	{
		// There is no block to find
		return 0;
	}

	isBlockFound = false;
	switch(dwFindType)
	{
	case FIND_FIRST:
		pDrawing->CurrentBlock = 0;
		isBlockFound = true;
		break;
	case FIND_LAST:
		pDrawing->CurrentBlock = pDrawing->BlocksNumber - 1;
		isBlockFound = true;
		break;
	case FIND_PREVIOUS:
		if(pDrawing->CurrentBlock<=0)
			break;
		pDrawing->CurrentBlock--;
		isBlockFound = true;
		break;
	case FIND_NEXT:
		if(pDrawing->CurrentBlock>=pDrawing->BlocksNumber-1)
			break;
		pDrawing->CurrentBlock++;
		isBlockFound = true;
		break;
	case FIND_BYHANDLE:
		for(i=0; i<pDrawing->BlocksNumber; i++)
			if( pDrawing->Blocks[i].Objhandle==pBlockHeader->Objhandle )
			{
				pDrawing->CurrentBlock = i;
				isBlockFound = true;
				break;
			}
		break;
	case FIND_BYNAME:
		for(i=0; i<pDrawing->BlocksNumber; i++)
			if( strcmp(pDrawing->Blocks[i].Name, pBlockHeader->Name)==0 )
			{
				pDrawing->CurrentBlock = i;
				isBlockFound = true;
				break;
			}
		break;
	}

	if(isBlockFound)
	{
		CopyMemory(pBlockHeader, &pDrawing->Blocks[pDrawing->CurrentBlock], sizeof(BLOCKHEADER));
		result = pDrawing->Blocks[pDrawing->CurrentBlock].Objhandle;
	}
	else
		result = 0;

	return result;
}

/*-------------------------------------------------------------------*
 |  drwFindBlock                                                     |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      DWORD dwFindType = find type                                 |
 |      PBLOCKHEADER pBlockHeader = pointer to block header data     |
 |  Output: Handle of found block (0=Error or Not found)             |
 *-------------------------------------------------------------------*/
OBJHANDLE drwFindBlock( HDRAWING hDrawing, DWORD dwFindType, PBLOCKHEADER pBlockHeader )
{
	PDRAWING		pDrawing;
	OBJHANDLE		result;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	if((result=drwFindBlock_Direct(pDrawing, dwFindType, pBlockHeader))==0)
	{
		GlobalUnlock(hDrawing);
		return 0;
	}

	// UnInitilize pDrawing -----------------
	UnInitilizePDRAWING(hDrawing);

	return result;
}

/*-------------------------------------------------------------------*
 |  drwGetBlocksNum                                                  |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |  Output: number of blocks in current drawing (lower than 0=Error) |
 *-------------------------------------------------------------------*/
int drwGetBlocksNum( HDRAWING hDrawing )
{
	PDRAWING	pDrawing;
	int			result;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return -1;

	result = pDrawing->BlocksNumber;

	// UnInitilize pDrawing -----------------
	UnInitilizePDRAWING(hDrawing);

	return result;
}

/*-------------------------------------------------------------------*
 |  FindBlockPosByHandle                                             |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current drawing structure |
 |      OBJHANDLE BlockObjhandle = handle of block to find           |
 |  Output: Position of block in Blocks list. (-1=block not found)   |
 *-------------------------------------------------------------------*/
int FindBlockPosByHandle( PDRAWING pDrawing, OBJHANDLE BlockObjhandle )
{
	for(unsigned int i=0; i<pDrawing->BlocksNumber; i++)
		if(pDrawing->Blocks[i].Objhandle==BlockObjhandle)
			return i;
	
	return -1;
}

/*-------------------------------------------------------------------*
 |  FindBlockPosByName                                               |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current drawing structure |
 |      LPCTSTR strBlockName = block name to be found                |
 |  Output: Position of block in Blocks list. (-1=block not found)   |
 *-------------------------------------------------------------------*/
int FindBlockPosByName( PDRAWING pDrawing, LPCTSTR strBlockName )
{
	for(unsigned int i=0; i<pDrawing->BlocksNumber; i++)
		if( strcmp(pDrawing->Blocks[i].Name, strBlockName)==0 )
			return i;
	
	return -1;
}

/*-------------------------------------------------------------------*
 |  drwAddBlock_Direct                                               |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      PBLOCKHEADER pBlockHeader = pointer to blockheader structure |
 |  Output: Handle of the new block (0=Error)                        |
 *-------------------------------------------------------------------*/
OBJHANDLE drwAddBlock_Direct( PDRAWING pDrawing, PBLOCKHEADER pBlockHeader )
{
	DWORD		BlockNumber;
	OBJHANDLE	NewBlockObjhandle;

	// Check for current blocks number
	if(pDrawing->BlocksNumber>=256)
	{
		// No more blocks can be added
		return 0;
	}

	if( FindBlockPosByName(pDrawing, pBlockHeader->Name)>=0 )
	{
		// The block already exists
		return 0;
	}

	// Initilize new block
	NewBlockObjhandle = pDrawing->LastObjhandle + 1;
	BlockNumber = pDrawing->BlocksNumber;
	CopyMemory(&pDrawing->Blocks[BlockNumber], pBlockHeader, sizeof(BLOCKHEADER));
	pDrawing->Blocks[BlockNumber].Objhandle = NewBlockObjhandle;

	// Initilize Entities Section of new block
	pDrawing->Blocks[BlockNumber].Entities.hEntities = (HDRAWING)GlobalAlloc(GHND, 16384);
	if(pDrawing->Blocks[BlockNumber].Entities.hEntities==NULL)
	{
		// GlobalAlloc error
		return 0;
	}

	pDrawing->Blocks[BlockNumber].Entities.TotalSize = GlobalSize(pDrawing->Blocks[BlockNumber].Entities.hEntities);
	pDrawing->Blocks[BlockNumber].Entities.FreePos = 0;
	pDrawing->Blocks[BlockNumber].Entities.LastEntityPos = 0;
	pDrawing->Blocks[BlockNumber].Entities.CurrentEntityPos = 0;
	pDrawing->Blocks[BlockNumber].Entities.EntitiesNumber = 0;

	pDrawing->LastObjhandle++;
	pDrawing->BlocksNumber++;

	return NewBlockObjhandle;
}

/*-------------------------------------------------------------------*
 |  drwAddBlock                                                      |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      PBLOCKHEADER pBlockHeader = pointer to blockheader structure |
 |  Output: Handle of the new block (0=Error)                        |
 *-------------------------------------------------------------------*/
OBJHANDLE drwAddBlock( HDRAWING hDrawing, PBLOCKHEADER pBlockHeader )
{
	PDRAWING	pDrawing;
	OBJHANDLE	result;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return 0;
	
	result = drwAddBlock_Direct(pDrawing, pBlockHeader);
	if(result==0)
	{
		GlobalUnlock(hDrawing);
		return 0;
	}

	// UnInitilize pDrawing -----------------
	UnInitilizePDRAWING(hDrawing);

	return result;
}

/*-------------------------------------------------------------------*
 |  drwDeleteBlock                                                   |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      OBJHANDLE BlockObjhandle = handle of block to Delete         |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwDeleteBlock( HDRAWING hDrawing, OBJHANDLE BlockObjhandle )
{
	PDRAWING	pDrawing;
	DWORD		BlockNumber;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;
	
	BlockNumber = FindBlockPosByHandle( pDrawing, BlockObjhandle );
	if(BlockNumber<0)
	{
		// The block cannot be found
		GlobalUnlock(hDrawing);
		return FALSE;
	}

	// Free entites memory related to this block
	if(GlobalFree(pDrawing->Blocks[BlockNumber].Entities.hEntities)!=NULL)
	{
		// error in GlobalFree function
		return FALSE;
	}

	pDrawing->BlocksNumber--;

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}


/********************************************************************
 * ENTITIES Section                                                 *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  AddToEntitiesList                                                |
 |  Inputs:                                                          |
 |      PENTITIES pEntities = pointer of ENTITIES structure          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure to add |
 |      WORD EntitySize = size of entity data to be added            |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL AddToEntitiesList(PENTITIES pEntities, PENTITYHEADER pEntityHeader, LPVOID pEntityData, WORD EntitySize)
{
	LPVOID		pEntitiesData;

	// Initialize pEntitiesData ----------
	pEntitiesData = GlobalLock(pEntities->hEntities);
	if(pEntitiesData==NULL)
	{
		// GlobalLock error
		return FALSE;
	}

	// Check if we need more memory space for entities data
	if((pEntities->FreePos + sizeof(ENTITYHEADER) + EntitySize) >= pEntities->TotalSize)
	{
		// Unlock hDrawing -------------------
		GlobalUnlock(pEntities->hEntities);
/*		if(!GlobalUnlock(pEntities->hEntities) && (GetLastError()!=NO_ERROR))
		{
			// memory error
			return FALSE;
		}*/
		
		pEntities->hEntities = GlobalReAlloc(
				pEntities->hEntities,
				pEntities->TotalSize + 65536,
				GHND );

		pEntities->TotalSize = GlobalSize( pEntities->hEntities );
		if((pEntities->FreePos + sizeof(ENTITYHEADER) + EntitySize) >= pEntities->TotalSize)
		{
			// Memory limitation error
			return FALSE;
		}

		pEntitiesData = GlobalLock(pEntities->hEntities);
		if(pEntitiesData==NULL)
		{
			// GlobalLock error
			return NULL;
		}
	}
	
	// Init EntityHeader for previous entity position
	pEntityHeader->PreviousEntityPos = pEntities->LastEntityPos;
	pEntityHeader->NextEntityPos = pEntities->FreePos + sizeof(ENTITYHEADER) + EntitySize;

	// Copy entity data to memory
	CopyMemory((LPVOID)((DWORD)pEntitiesData+pEntities->FreePos), pEntityHeader, sizeof(ENTITYHEADER));
	CopyMemory((LPVOID)((DWORD)pEntitiesData+pEntities->FreePos+sizeof(ENTITYHEADER)), (LPVOID)pEntityData, EntitySize);

	// Update pointers
	pEntities->LastEntityPos = pEntities->FreePos;
	pEntities->FreePos += sizeof(ENTITYHEADER) + EntitySize;

	// Unlock hDrawing -------------------
	GlobalUnlock(pEntities->hEntities);
/*	if(!GlobalUnlock(pEntities->hEntities) && (GetLastError()!=NO_ERROR))
	{
		// memory error
		return FALSE;
	}*/

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  AddEntity                                                        |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      PENTITIES pEntities = pointer of ENTITIES structure          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure to add |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL AddEntity( PDRAWING pDrawing, PENTITIES pEntities, PENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	BOOL result;
	OBJHANDLE		BlockObjhandle1;

	switch( pEntityHeader->EntityType )
	{
	case ENT_LINE:
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTLINE));
		break;

	case ENT_POINT:
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTPOINT));
		break;

	case ENT_CIRCLE:
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTCIRCLE));
		break;

	case ENT_ELLIPSE:
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTELLIPSE));
		break;

	case ENT_TEXT:
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTTEXT));
		break;

	case ENT_ARC:
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTARC));
		break;

	case ENT_SOLID:
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTSOLID));
		break;

	case ENT_INSERT:
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTINSERT));
		break;

	case ENT_POLYLINE:
		PENTVERTEX pVertex;
		pVertex = new ENTVERTEX[((PENTPOLYLINE)pEntityData)->nVertex];
		memcpy(pVertex, ((PENTPOLYLINE)pEntityData)->pVertex, sizeof(ENTVERTEX)*((PENTPOLYLINE)pEntityData)->nVertex);
		((PENTPOLYLINE)pEntityData)->pVertex = pVertex;
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTPOLYLINE));
		break;

	case ENT_DIMENSION:
		BlockObjhandle1 = ((PENTDIMENSION)pEntityData)->BlockheaderObjhandle;

		if(BlockObjhandle1==NULL)
		{
			DIMSTYLE		DimStyle;
			BLOCKHEADER		BlockHeader;
			OBJHANDLE		BlockObjhandle;
			char			buf[32];
			// Looking for dim style
			DimStyle.Objhandle = PENTDIMENSION(pEntityData)->DimStyleObjhandle;
			if(drwFindTableType_Direct(pDrawing, TAB_DIMSTYLE, FIND_BYHANDLE, &DimStyle)>0)
			{
				// Create a new Dimension Block
				ZeroMemory(&BlockHeader, sizeof(BLOCKHEADER));
				strcpy(BlockHeader.Name, "*D");
				strcat(BlockHeader.Name, ltoa(++pDrawing->LastDimBlkNum, buf, 10));
				BlockHeader.Flags = 1;
				BlockHeader.BasePoint.x = 0;
				BlockHeader.BasePoint.y = 0;
				BlockHeader.BasePoint.z = 0;
				BlockHeader.LayerObjhandle = pEntityHeader->LayerObjhandle;
				BlockObjhandle = drwAddBlock_Direct(pDrawing, &BlockHeader);

				if(BlockObjhandle!=NULL)
				{
					DimLinear( pDrawing, BlockObjhandle, pEntityHeader, PENTDIMENSION(pEntityData), &DimStyle );
					PENTDIMENSION(pEntityData)->BlockheaderObjhandle = BlockObjhandle;
				}
			}

		}
		result = AddToEntitiesList(pEntities, pEntityHeader, pEntityData, sizeof(ENTDIMENSION));
		break;

	default: // Entity is not accepted
		result = FALSE;
	}

	if(!result)
		return FALSE;
	else
		pEntities->EntitiesNumber++;

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  drwAddEntity_Direct                                              |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      OBJHANDLE BlockObjhandle = handle of block that entity must  |
 |                        be added in it. (NULL = Entity Section)    |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure to add |
 |  Output: Handle of the new entity (0=Error)                       |
 *-------------------------------------------------------------------*/
OBJHANDLE drwAddEntity_Direct( PDRAWING pDrawing, OBJHANDLE BlockObjhandle, PENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	PENTITIES		pEntities;
//	ENTITYHEADER	EntityHeader;
	OBJHANDLE		NewEntityObjhandle;

	if(BlockObjhandle!=NULL)
	{ // Search for block name
		DWORD i = FindBlockPosByHandle( pDrawing, BlockObjhandle );
		if(i<0)
		{
			// The block cannot be found
			return 0;
		}
		else
			pEntities = &pDrawing->Blocks[i].Entities;
	}
	else
		pEntities = &pDrawing->Entities;

	// Initialize EntityHeader
	NewEntityObjhandle = pDrawing->LastObjhandle + 1;
	pEntityHeader->Objhandle = NewEntityObjhandle;
	pEntityHeader->Deleted = FALSE;
//	EntityHeader.EntityType = usEntityType;
//	EntityHeader.LayerObjhandle = pDrawing->Tables.CurrentLayerHandle;
//	EntityHeader.LTypeObjhandle = pDrawing->Tables.CurrentLineTypeHandle;
//	EntityHeader.Color = pDrawing->Tables.CurrentColor;

	// Add Entity to list
	if( !AddEntity(pDrawing, pEntities, pEntityHeader, pEntityData) )
	{
		return 0;
	}

	pDrawing->LastObjhandle++;

	return NewEntityObjhandle;
}

/*-------------------------------------------------------------------*
 |  drwAddEntity                                                     |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      OBJHANDLE BlockObjhandle = handle of block that entity must  |
 |                        be added in it. (NULL = Entity Section)    |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure to add |
 |  Output: Handle of the new entity (0=Error)                       |
 *-------------------------------------------------------------------*/
OBJHANDLE drwAddEntity( HDRAWING hDrawing, OBJHANDLE BlockObjhandle, PENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	PDRAWING		pDrawing;
	OBJHANDLE		NewEntityObjhandle;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return 0;

	NewEntityObjhandle = drwAddEntity_Direct(pDrawing, BlockObjhandle, pEntityHeader, pEntityData);
	if(NewEntityObjhandle==0)
	{
		GlobalUnlock(hDrawing);
		return 0;
	}

	// UnInitilize pDrawing -----------------
	UnInitilizePDRAWING(hDrawing);

	return NewEntityObjhandle;
}

/*-------------------------------------------------------------------*
 |  drwDeleteEntity                                                  |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      LPCTSTR strBlockName = block name that entity must be changed|
 |                             in it. (NULL = Entities Section)      |
 |      OBJHANDLE EntityHandle = handle of entity to be deleted      |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwDeleteEntity( HDRAWING hDrawing, LPCTSTR strBlockName, OBJHANDLE EntityHandle )
{
	PDRAWING		pDrawing;
	ENTITYHEADER	EntityHeader;
	char			EntityData[4096];

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return 0;

	// Find the entity and mark it as "deleted"
	EntityHeader.Objhandle = EntityHandle;
	if(drwFindEntity_Direct(pDrawing, strBlockName, &EntityHeader, EntityData, FIND_BYHANDLE)==0)
	{
		GlobalUnlock(hDrawing);
		return FALSE;
	}

	EntityHeader.Deleted = TRUE;
	if(drwChangeEntity_Direct(pDrawing, strBlockName, &EntityHeader, &EntityData)==FALSE)
	{
		GlobalUnlock(hDrawing);
		return FALSE;
	}

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}

/*-------------------------------------------------------------------*
 |  GetCurrentEntityData                                             |
 |  Inputs:                                                          |
 |      PENTITIES pEntities = pointer of ENTITIES structure          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure        |
 |  Output: current entity data size (0 = error occured)             |
 *-------------------------------------------------------------------*/
DWORD GetCurrentEntityData( PENTITIES pEntities, PENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	LPVOID	pEntitiesData;
	DWORD	EntityDataSize;

	// Initialize pEntitiesData ----------
	pEntitiesData = GlobalLock(pEntities->hEntities);
	if(pEntitiesData==NULL)
	{
		// GlobalLock error
		return 0;
	}

	CopyMemory(pEntityHeader, (LPVOID)((DWORD)pEntitiesData+pEntities->CurrentEntityPos), sizeof(ENTITYHEADER));

	EntityDataSize = pEntityHeader->NextEntityPos - (pEntities->CurrentEntityPos + sizeof(ENTITYHEADER));
	CopyMemory(pEntityData, (LPVOID)((DWORD)pEntitiesData+pEntities->CurrentEntityPos+sizeof(ENTITYHEADER)), EntityDataSize);

	return EntityDataSize;
}

/*-------------------------------------------------------------------*
 |  SetEntityData                                                    |
 |  Inputs:                                                          |
 |      PENTITIES pEntities = pointer of ENTITIES structure          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure        |
 |      DWORD dwEntityPos = position of entity to set data on it     |
 |  Output: current entity data size (0 = error occured)             |
 *-------------------------------------------------------------------*/
DWORD SetEntityData( PENTITIES pEntities, PENTITYHEADER pEntityHeader, LPVOID pEntityData, DWORD dwEntityPos )
{
	LPVOID	pEntitiesData;
	DWORD	EntityDataSize;

	// Initialize pEntitiesData ----------
	pEntitiesData = GlobalLock(pEntities->hEntities);
	if(pEntitiesData==NULL)
	{
		// GlobalLock error
		return 0;
	}

	CopyMemory((LPVOID)((DWORD)pEntitiesData+dwEntityPos), pEntityHeader, sizeof(ENTITYHEADER));

	EntityDataSize = pEntityHeader->NextEntityPos - (dwEntityPos + sizeof(ENTITYHEADER));
	CopyMemory((LPVOID)((DWORD)pEntitiesData+dwEntityPos+sizeof(ENTITYHEADER)), pEntityData, EntityDataSize);

	return EntityDataSize;
}

/*-------------------------------------------------------------------*
 |  drwFindEntity_Direct                                             |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      LPCTSTR strBlockName = block name that entity must be found  |
 |                             in it. (NULL = Entity Section)        |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure        |
 |      DWORD dwFindType = find type                                 |
 |      DWORD* pdwEntityPos = position of found entity               |
 |  Output: Handle of found entity (0=Error or Not found)            |
 *-------------------------------------------------------------------*/
OBJHANDLE drwFindEntity_Direct( PDRAWING pDrawing, LPCTSTR strBlockName, PENTITYHEADER pEntityHeader, LPVOID pEntityData, DWORD dwFindType, DWORD* pdwEntityPos )
{
	PENTITIES		pEntities;
	OBJHANDLE		result;

	if(pEntityHeader==NULL)
	{
		// pEntityHeader can not be NULL
		return 0;
	}

	if(pEntityData==NULL)
	{
		// pEntityData can not be NULL
		return 0;
	}

	if(strBlockName!=NULL)
	{ // Search for block name
		DWORD i = FindBlockPosByName( pDrawing, strBlockName );
		if(i<0)
		{
			// The block cannot be found
			return 0;
		}
		else
			pEntities = &pDrawing->Blocks[i].Entities;
	}
	else
		pEntities = &pDrawing->Entities;

	// Check number of entities
	if(pEntities->EntitiesNumber<1)
	{
		// There is no entity to find
		return 0;
	}

	result = 0;

	if(dwFindType & FIND_BYHANDLE)
	{
		ENTITYHEADER tempEntityHeader;
		char tempEntityData[4096];
		DWORD dwEntitySize;

		pEntities->CurrentEntityPos = 0;
		while(pEntities->CurrentEntityPos<pEntities->FreePos)
		{
			dwEntitySize = GetCurrentEntityData(pEntities, &tempEntityHeader, tempEntityData);
			if(pdwEntityPos!=NULL)
				*pdwEntityPos = pEntities->CurrentEntityPos;

			pEntities->CurrentEntityPos = tempEntityHeader.NextEntityPos;
			if(pEntityHeader->Objhandle==tempEntityHeader.Objhandle)
			{
				CopyMemory(pEntityHeader, &tempEntityHeader, sizeof(ENTITYHEADER));
				CopyMemory(pEntityData, tempEntityData, dwEntitySize);
				result = pEntityHeader->Objhandle;
				break;
			}
		}
	}
	else
	{
		if(dwFindType & FIND_FIRST)
		{
			pEntities->CurrentEntityPos = 0;
			GetCurrentEntityData(pEntities, pEntityHeader, pEntityData);
			if(pdwEntityPos!=NULL)
				*pdwEntityPos = pEntities->CurrentEntityPos;

			pEntities->CurrentEntityPos = pEntityHeader->NextEntityPos;
			result = pEntityHeader->Objhandle;
		}
		else if(dwFindType & FIND_LAST)
		{
			pEntities->CurrentEntityPos = pEntities->LastEntityPos;
			GetCurrentEntityData(pEntities, pEntityHeader, pEntityData);
			if(pdwEntityPos!=NULL)
				*pdwEntityPos = pEntities->CurrentEntityPos;

			pEntities->CurrentEntityPos = pEntities->FreePos;
			result = pEntityHeader->Objhandle;
		}
		else if(dwFindType & FIND_PREVIOUS)
		{
	//		if(pEntities->CurrentEntityPos<=0)
	//			break;
	//		GetCurrentEntityData(pEntities, pEntityHeader, pEntityData);
	//		if(pdwEntityPos!=NULL)
	//			*pdwEntityPos = pEntities->CurrentEntityPos;

	//		pEntities->CurrentEntityPos = pEntityHeader->PreviousEntityPos;
	//		GetCurrentEntityData(pEntities, pEntityHeader, pEntityData);
	//		result = pEntityHeader->Objhandle;
		}
		else if(dwFindType & FIND_NEXT)
		{
			if(pEntities->CurrentEntityPos<pEntities->FreePos)
			{
				GetCurrentEntityData(pEntities, pEntityHeader, pEntityData);
				if(pdwEntityPos!=NULL)
					*pdwEntityPos = pEntities->CurrentEntityPos;

				pEntities->CurrentEntityPos = pEntityHeader->NextEntityPos;
				result = pEntityHeader->Objhandle;
			}
		}
	}

	return result;
}

/*-------------------------------------------------------------------*
 |  drwFindEntity                                                    |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      LPCTSTR strBlockName = block name that entity must be found  |
 |                             in it. (NULL = Entities Section)      |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure        |
 |      DWORD dwFindType = find type                                 |
 |  Output: Handle of found entity (0=Error or Not found)            |
 *-------------------------------------------------------------------*/
OBJHANDLE drwFindEntity( HDRAWING hDrawing, LPCTSTR strBlockName, PENTITYHEADER pEntityHeader, LPVOID pEntityData, DWORD dwFindType )
{
	PDRAWING		pDrawing;
	OBJHANDLE		result;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	if((result=drwFindEntity_Direct(pDrawing, strBlockName, pEntityHeader, pEntityData, dwFindType))==0)
	{
		GlobalUnlock(hDrawing);
		return 0;
	}

	// UnInitilize pDrawing -----------------
	UnInitilizePDRAWING(hDrawing);

	return result;
}

/*-------------------------------------------------------------------*
 |  drwChangeEntity_Direct                                           |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      LPCTSTR strBlockName = block name that entity must be changed|
 |                             in it. (NULL = Entities Section)      |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwChangeEntity_Direct( PDRAWING pDrawing, LPCTSTR strBlockName, PENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	PENTITIES		pEntities;
	ENTITYHEADER	tempEntityHeader;
	char			tempEntityData[4096];
	DWORD			dwEntityPos;

	if(pEntityHeader==NULL)
	{
		// pEntityHeader can not be NULL
		return FALSE;
	}

	if(pEntityData==NULL)
	{
		// pEntityData can not be NULL
		return FALSE;
	}

	// Looking for entity if found then change it
	memcpy(&tempEntityHeader, pEntityHeader, sizeof(ENTITYHEADER));
	if(drwFindEntity_Direct(pDrawing, strBlockName, &tempEntityHeader, tempEntityData, FIND_BYHANDLE, &dwEntityPos)==0)
	{
		// Entity object handle is not found
		return FALSE;
	}
	if(tempEntityHeader.EntityType!=pEntityHeader->EntityType)
	{
		// Entity type is not matched with memory object
		return FALSE;
	}

	pEntityHeader->PreviousEntityPos = tempEntityHeader.PreviousEntityPos;
	pEntityHeader->NextEntityPos = tempEntityHeader.NextEntityPos;

	if(strBlockName!=NULL)
	{ // Search for block name
		DWORD i = FindBlockPosByName( pDrawing, strBlockName );
		if(i<0)
		{
			// The block cannot be found
			return FALSE;
		}
		else
			pEntities = &pDrawing->Blocks[i].Entities;
	}
	else
		pEntities = &pDrawing->Entities;

	// Change entity with new given data
	SetEntityData( pEntities, pEntityHeader, pEntityData, dwEntityPos );

	return TRUE;
}


/*-------------------------------------------------------------------*
 |  drwChangeEntity                                                  |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      LPCTSTR strBlockName = block name that entity must be changed|
 |                             in it. (NULL = Entities Section)      |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwChangeEntity( HDRAWING hDrawing, LPCTSTR strBlockName, PENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	PDRAWING		pDrawing;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	if(drwChangeEntity_Direct(pDrawing, strBlockName, pEntityHeader, pEntityData)==FALSE)
	{
		GlobalUnlock(hDrawing);
		return FALSE;
	}
	
	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}

