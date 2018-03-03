/*-------------------------------------------------------------------*\
|  CadLib Version 2.1                                                 |
|  Written by Omid Shahabi <omid_shahabi@hotmail.com>                 |
|  Copyright © 2002-2004                                              |
|  Pars Technology Development Co.                                    |
|                                                                     |
|  This software is distributed on an "AS IS" basis, WITHOUT WARRANTY |
|  OF ANY KIND, either express or implied.                            |
|                                                                     |
|                                                                     |
|  DXFBase.cpp                                                        |
\*-------------------------------------------------------------------*/

///#include "stdafx.h"
#include "DXFBase.h"

/********************************************************************
 * HDXF -> PDXF Functions                                           *
 ********************************************************************/

PDXF InitilizePDXF(HDXF hDxf)
{
	PDXF pDxf;

	if( hDxf == NULL )
	{
		// No DXF file is open
		return NULL;
	}

	pDxf = (PDXF)GlobalLock(hDxf);
	if(pDxf==NULL)
	{
		// GlobalLock error
		return NULL;
	}

	return pDxf;
}

BOOL UnInitilizePDXF(HDXF hDxf)
{
	// Unlock hDxf ----------------------
	GlobalUnlock(hDxf);
/*	if(!GlobalUnlock(hDxf) && (GetLastError()!=NO_ERROR))
	{
		// memory error
		return FALSE;
	}*/

	return TRUE;
}


/********************************************************************
 * DXF Writer Functions                                             *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  dxfWriteParamString                                              |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      int GroupCode = group code                                   |
 |      char* value = pointer to null terminated string to put in DXF|
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfWriteParamString( HDXF hDxf, int GroupCode, LPCTSTR value )
{
	PDXF	pDxf;

	if( hDxf == NULL )
	{
		// No DXF file is open
		return FALSE;
	}

	pDxf = (PDXF)GlobalLock(hDxf);
	if(pDxf==NULL)
	{
		// GlobalLock error
		return FALSE;
	}

	if(fprintf( pDxf->pStream, "%d\n%s\n", GroupCode, value )<=0)
	{
		// file write error
		return FALSE;
	}

	// Unlock hDxf ----------------------
	GlobalUnlock(hDxf);
/*	if(!GlobalUnlock(hDxf) && (GetLastError()!=NO_ERROR))
	{
		// memory error
		return FALSE;
	}*/

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  dxfWriteParamInteger                                             |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      int GroupCode = group code                                   |
 |      int value = an integer value to put in dxf file              |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfWriteParamInteger( HDXF hDxf, int GroupCode, int value )
{
	PDXF	pDxf;

	if( hDxf == NULL )
	{
		// No DXF file is open
		return FALSE;
	}

	pDxf = (PDXF)GlobalLock(hDxf);
	if(pDxf==NULL)
	{
		// GlobalLock error
		return FALSE;
	}

	if(fprintf( pDxf->pStream, "%d\n%d\n", GroupCode, value )<=0)
	{
		// file write error
		return FALSE;
	}

	// Unlock hDxf ----------------------
	GlobalUnlock(hDxf);
/*	if(!GlobalUnlock(hDxf) && (GetLastError()!=NO_ERROR))
	{
		// memory error
		return FALSE;
	}*/

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  dxfWriteParamDouble                                              |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      int GroupCode = group code                                   |
 |      double value = a real value to put in DXF file               |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfWriteParamDouble( HDXF hDxf, int GroupCode, double value )
{
	PDXF	pDxf;

	if( hDxf == NULL )
	{
		// No DXF file is open
		return FALSE;
	}

	pDxf = (PDXF)GlobalLock(hDxf);
	if(pDxf==NULL)
	{
		// GlobalLock error
		return FALSE;
	}

	if(fprintf( pDxf->pStream, "%d\n%f\n", GroupCode, value )<=0)
	{
		// file write error
		return FALSE;
	}

	// Unlock hDxf ----------------------
	GlobalUnlock(hDxf);
/*	if(!GlobalUnlock(hDxf) && (GetLastError()!=NO_ERROR))
	{
		// memory error
		return FALSE;
	}*/

	return TRUE;
}

/********************************************************************
 * DXF Read Functions                                               *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  ReadLine                                                         |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      LPTSTR valuestr = pointer to null terminated string          |
 |                       to put data in it                           |
 |  Output: Nothing                                                  |
 *-------------------------------------------------------------------*/
void ReadLine( PDXF pDxf, LPTSTR valuestr )
{
	char ch;
	int  i = 0;

/*	if(ch = fgetc(pDxf->pStream)==0x0a)
		ch = fgetc(pDxf->pStream);
	while(!feof(pDxf->pStream))
	{
		if(ch==0x0a)
		{
			valuestr[i]=0;
			break;
		}
		valuestr[i++] = ch;
		ch = fgetc(pDxf->pStream);
	}*/

	while(pDxf->Read.CurrentPos<pDxf->Read.FileSize)
	{
		ch = pDxf->Read.ReadBuffer[pDxf->Read.CurrentPos-pDxf->Read.BufferPos];
		if(ch==0x0d)
		{
			valuestr[i]=0;
			break;
		}

		valuestr[i++] = ch;

		pDxf->Read.CurrentPos++;
		if((pDxf->Read.CurrentPos-pDxf->Read.BufferPos)>=pDxf->Read.BufferDataSize)
		{
			pDxf->Read.BufferPos += pDxf->Read.BufferDataSize;
			pDxf->Read.BufferDataSize = fread(pDxf->Read.ReadBuffer, 1, READBUFFERSIZE, pDxf->pStream);
		}
	}

	pDxf->Read.CurrentPos++;
	if((pDxf->Read.CurrentPos-pDxf->Read.BufferPos)>=pDxf->Read.BufferDataSize)
	{
		pDxf->Read.BufferPos += pDxf->Read.BufferDataSize;
		pDxf->Read.BufferDataSize = fread(pDxf->Read.ReadBuffer, 1, READBUFFERSIZE, pDxf->pStream);
	}

	ch = pDxf->Read.ReadBuffer[pDxf->Read.CurrentPos-pDxf->Read.BufferPos];
	if(ch==0x0a)
	{
		pDxf->Read.CurrentPos++;
		if((pDxf->Read.CurrentPos-pDxf->Read.BufferPos)>=pDxf->Read.BufferDataSize)
		{
			pDxf->Read.BufferPos += pDxf->Read.BufferDataSize;
			pDxf->Read.BufferDataSize = fread(pDxf->Read.ReadBuffer, 1, READBUFFERSIZE, pDxf->pStream);
		}
	}
}

/*-------------------------------------------------------------------*
 |  ReadParamFromDxfFile                                             |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      int& GroupCode = group code                                  |
 |      LPTSTR valuestr = pointer to null terminated string          |
 |                       to put data in it                           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadParamFromDxfFile( PDXF pDxf, int& GroupCode, LPTSTR valuestr )
{
/*	if(fscanf(pDxf->pStream, "%d", &GroupCode)<=0)
	{
		// file read error
		return FALSE;
	}*/

	char strGroupCode[32];
	ReadLine(pDxf, strGroupCode);
	GroupCode = atoi(strGroupCode);
	ReadLine(pDxf, valuestr);

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  dxfReadParam                                                     |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      int& GroupCode = group code                                  |
 |      LPTSTR valuestr = pointer to null terminated string          |
 |                       to put data in it                           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfReadParam( HDXF hDxf, int& GroupCode, LPTSTR valuestr )
{
	PDXF	pDxf;

	if( hDxf == NULL )
	{
		// No DXF file is open
		return FALSE;
	}

	pDxf = (PDXF)GlobalLock(hDxf);
	if(pDxf==NULL)
	{
		// GlobalLock error
		return FALSE;
	}

/*	if(fscanf(pDxf->pStream, "%d", &GroupCode)<=0)
	{
		// file read error
		return FALSE;
	}

	ReadLine(pDxf, valuestr);
*/
	char strGroupCode[32];
	ReadLine(pDxf, strGroupCode);
	GroupCode = atoi(strGroupCode);
	ReadLine(pDxf, valuestr);

	// Unlock hDxf ----------------------
	GlobalUnlock(hDxf);

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  FindParamFromDxfFile                                             |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      int GroupCode = group code                                   |
 |      LPCTSTR valuestr = pointer to null terminated string to find |
 |  Output: TRUE if found else FALSE                                 |
 *-------------------------------------------------------------------*/
BOOL FindParamFromDxfFile( PDXF pDxf, int GroupCode, LPCTSTR valuestr )
{
	int		groupcode;
	char	value[2048];
	BOOL	found;

	found = FALSE;
	dxfStorePos(pDxf);
	while(/*!feof(pDxf->pStream)*/pDxf->Read.CurrentPos<pDxf->Read.FileSize)
	{
/*		if(fscanf(pDxf->pStream, "%d", &groupcode)<=0)
		{
			// file read error
			break;
		}

		ReadLine(pDxf, value);*/

		char strGroupCode[32];
		ReadLine(pDxf, strGroupCode);
		groupcode = atoi(strGroupCode);
		ReadLine(pDxf, value);

		if((groupcode==GroupCode) && (strcmp(valuestr, value)==0))
		{
			found = TRUE;
			break;
		}
	}

	if(!found)
		dxfRestorePos(pDxf);

	return found;
}

/*-------------------------------------------------------------------*
 |  dxfFindParam                                                     |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      int GroupCode = group code                                   |
 |      LPCTSTR valuestr = pointer to null terminated string to find |
 |  Output: TRUE if found else FALSE                                 |
 *-------------------------------------------------------------------*/
BOOL dxfFindParam( HDXF hDxf, int GroupCode, LPCTSTR valuestr )
{
	PDXF	pDxf;
	int		groupcode;
	char	value[2048];
	BOOL	found;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	found = FALSE;
	dxfStorePos(pDxf);
	while(/*!feof(pDxf->pStream)*/pDxf->Read.CurrentPos<pDxf->Read.FileSize)
	{
/*		if(fscanf(pDxf->pStream, "%d", &groupcode)<=0)
		{
			// file read error
			GlobalUnlock(hDxf);
			return FALSE;
		}

		ReadLine(pDxf, value);*/

		char strGroupCode[32];
		ReadLine(pDxf, strGroupCode);
		groupcode = atoi(strGroupCode);
		ReadLine(pDxf, value);

		if((groupcode==GroupCode) && (strcmp(valuestr, value)==0))
		{
			found = TRUE;
			break;
		}
	}

	if(!found)
		dxfRestorePos(pDxf);

	// UnInitilize hDxf -----------------
	UnInitilizePDXF(hDxf);

	return found;
}

/*-------------------------------------------------------------------*
 |  dxfStorePos                                                      |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |  Output: TRUE if found else FALSE                                 |
 *-------------------------------------------------------------------*/
BOOL dxfStorePos( PDXF pDxf )
{
	if(pDxf==NULL)
		return FALSE;

	pDxf->Read.StoredStreamPos = pDxf->Read.CurrentPos/*ftell(pDxf->pStream)*/;

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  dxfRestorePos                                                    |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |  Output: TRUE if found else FALSE                                 |
 *-------------------------------------------------------------------*/
BOOL dxfRestorePos( PDXF pDxf )
{
	if(pDxf==NULL)
		return FALSE;

	pDxf->Read.CurrentPos = pDxf->Read.StoredStreamPos;
	if(((pDxf->Read.CurrentPos-pDxf->Read.BufferPos)<0) || ((pDxf->Read.CurrentPos-pDxf->Read.BufferPos)>=pDxf->Read.BufferDataSize))
	{
		fseek(pDxf->pStream, pDxf->Read.StoredStreamPos, SEEK_SET);
		pDxf->Read.BufferDataSize = fread(pDxf->Read.ReadBuffer, 1, READBUFFERSIZE, pDxf->pStream);
		pDxf->Read.BufferPos = pDxf->Read.StoredStreamPos;
	}

	return TRUE;
}
