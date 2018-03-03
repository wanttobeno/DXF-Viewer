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
|  DXFWrite.cpp                                                       |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DXFWrite.h"


/********************************************************************
 * Construction & Destruction Functions                             *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  dxfCreateFile                                                    |
 |  Inputs:                                                          |
 |      LPCTSTR FileName = pointer to DXF file name                  |
 |  Output: Handle to new created and opened file                    |
 *-------------------------------------------------------------------*/
HDXF dxfCreateFile( LPCTSTR FileName )
{
	HDXF	hDxf;
	FILE*	pStream;
	PDXF	pDxf;

	if( (pStream = fopen( FileName, "wt" )) == NULL )
	{
		// Cannot create new file
		return NULL;
	}

	hDxf = (HDXF)GlobalAlloc(GHND, 65536);
	if(hDxf==NULL)
	{
		// GlobalAlloc error
		return NULL;
	}

	pDxf = (PDXF)GlobalLock(hDxf);
	if(pDxf==NULL)
	{
		// GlobalLock error
		GlobalFree(hDxf);
		return NULL;
	}
	pDxf->pStream = pStream;
	pDxf->Size = sizeof(DXF);
	pDxf->isWriteMode = TRUE;

	strcpy( pDxf->Write.CurrentLayer, "0" );
	strcpy( pDxf->Write.CurrentLine, "" ); // BYLAYER
	strcpy( pDxf->Write.CurrentTextStyle, "" );// STANDARD
	strcpy( pDxf->Write.CurrentDimStyle, "ClosedFilled");
	pDxf->Write.CurrentColor = 256; // ByLayer

	pDxf->Write.DoneSections = SEC_NOTSET;
	pDxf->Write.CurrentSection = SEC_NOTSET;
	pDxf->Write.DoneTableTypes = TAB_NOTSET;
	pDxf->Write.CurrentTableType = TAB_NOTSET;
	pDxf->Write.DimStylesNumber = 0;

	GlobalUnlock(hDxf);

	return hDxf;
}

/*-------------------------------------------------------------------*
 |  dxfCloseFile                                                     |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfCloseFile( HDXF hDxf )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	if(pDxf->isWriteMode)
	{
		// Check for opened sections --------
		if(pDxf->Write.CurrentSection!=SEC_NOTSET)
		{
			// There is an opened section (close it first)
			GlobalUnlock(hDxf);
			return FALSE;
		}

		dxfWriteParamString(hDxf, 0, "EOF");
	}

	// Closing DXF File -----------------
	if(fclose(pDxf->pStream)!=0)
	{
		// error in closing dxf file
		GlobalUnlock(hDxf);
		GlobalFree(hDxf);
		return FALSE;
	}
	
	// Unlock hDxf ----------------------
	DWORD ii;
	if((ii=GlobalUnlock(hDxf))>0/* && (GetLastError()!=NO_ERROR)*/)
	{
		// memory error
		return FALSE;
	}

	if(GlobalFree(hDxf)!=NULL)
	{
		// error in GlobalFree function
		return FALSE;
	}

	return TRUE;
}

/********************************************************************
 * Initialization                                                   *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  dxfSetCurrentColor                                               |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      int Color = Color number to set                              |
 |            (0=ByBlock, 256=ByLayer, Negative=Layer is turned off) |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfSetCurrentColor( HDXF hDxf, int Color )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	pDxf->Write.CurrentColor = Color;

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/********************************************************************
 * Sections Functions                                               *
 ********************************************************************/

 /*-------------------------------------------------------------------*
  |  dxfSectionBegin                                                  |
  |  Inputs:                                                          |
  |      HDXF hDxf = handle to the openning DXF file structure        |
  |      DWORD dwSection = ID of the section to be opened             |
  |  Output: TRUE if everything is ok                                 |
  *-------------------------------------------------------------------*/
BOOL dxfSectionBegin( HDXF hDxf, DWORD dwSection )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Process Section ------------------
	if(pDxf->Write.CurrentSection!=SEC_NOTSET)
	{
		// Last section is not closed
		GlobalUnlock(hDxf);
		return FALSE;
	}
	else
	{
		if(dwSection==SEC_HEADER)
		{
			if((pDxf->Write.DoneSections&SEC_HEADER)!=0)
			{
				// Section HEADER is already done
				GlobalUnlock(hDxf);
				return FALSE;
			}

			pDxf->Write.CurrentSection = SEC_HEADER;
			if(!dxfWriteParamString(hDxf, 0, "SECTION"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
			if(!dxfWriteParamString(hDxf, 2, "HEADER"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
		}
		else if(dwSection==SEC_TABLES)
		{
			if((pDxf->Write.DoneSections&SEC_TABLES)!=0)
			{
				// Section TABLES is already done
				GlobalUnlock(hDxf);
				return FALSE;
			}

			pDxf->Write.CurrentSection = SEC_TABLES;
			if(!dxfWriteParamString(hDxf, 0, "SECTION"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
			if(!dxfWriteParamString(hDxf, 2, "TABLES"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
		}
		else if(dwSection==SEC_ENTITIES)
		{
			if((pDxf->Write.DoneSections&SEC_ENTITIES)!=0)
			{
				// Section ENTITIES is already done
				GlobalUnlock(hDxf);
				return FALSE;
			}

			pDxf->Write.CurrentSection = SEC_ENTITIES;
			if(!dxfWriteParamString(hDxf, 0, "SECTION"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
			if(!dxfWriteParamString(hDxf, 2, "ENTITIES"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
		}
		else if(dwSection==SEC_BLOCKS)
		{
			if((pDxf->Write.DoneSections&SEC_BLOCKS)!=0)
			{
				// Section BLOCKS is already done
				GlobalUnlock(hDxf);
				return FALSE;
			}

			pDxf->Write.CurrentSection = SEC_BLOCKS;
			if(!dxfWriteParamString(hDxf, 0, "SECTION"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
			if(!dxfWriteParamString(hDxf, 2, "BLOCKS"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
		}
		else
		{
			// Section Not Allowed
			GlobalUnlock(hDxf);
			return FALSE;
		}
	}

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

 /*-------------------------------------------------------------------*
  |  dxfSectionEnd                                                    |
  |  Inputs:                                                          |
  |      HDXF hDxf = handle to the openning DXF file structure        |
  |  Output: TRUE if everything is ok                                 |
  *-------------------------------------------------------------------*/
BOOL dxfSectionEnd( HDXF hDxf )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if there is any opened section
	if(pDxf->Write.CurrentSection==SEC_NOTSET)
	{
		// No section is opened
		GlobalUnlock(hDxf);
		return FALSE;
	}

	if(!dxfWriteParamString(hDxf, 0, "ENDSEC"))
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// ----------------------------------
	pDxf->Write.DoneSections |= pDxf->Write.CurrentSection;
	pDxf->Write.CurrentSection = SEC_NOTSET;

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}


