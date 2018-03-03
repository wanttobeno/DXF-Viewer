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
|  DXFRead.cpp                                                        |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DXFRead.h"


/********************************************************************
 * Construction & Destruction Functions                             *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  dxfOpenFile                                                      |
 |  Inputs:                                                          |
 |      LPCTSTR FileName = pointer to DXF file name                  |
 |  Output: Handle to new created and opened file                    |
 *-------------------------------------------------------------------*/
HDXF dxfOpenFile( LPCTSTR FileName )
{
	HDXF	hDxf;
	FILE*	pStream;
	PDXF	pDxf;

	if( (pStream = fopen( FileName, "rb" )) == NULL )
	{
		// Cannot create new file
		return NULL;
	}

	hDxf = (HDXF)GlobalAlloc(GHND, sizeof(DXF));
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
	pDxf->isWriteMode = FALSE;

	pDxf->Read.CurrentSection = SEC_NOTSET;
	pDxf->Read.CurrentTableType = TAB_NOTSET;
	pDxf->Read.isBlockOpen = FALSE;
	pDxf->Read.StoredStreamPos = 0;

    fseek(pStream, 0, SEEK_END);
	pDxf->Read.FileSize = ftell(pStream);
	rewind(pStream);

	pDxf->Read.CurrentPos = 0;
	pDxf->Read.BufferPos = 0;
	pDxf->Read.BufferDataSize = fread(pDxf->Read.ReadBuffer, 1, READBUFFERSIZE, pDxf->pStream);

	GlobalUnlock(hDxf);

	return hDxf;
}

/********************************************************************
 * Sections Functions                                               *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  dxfFindNextSection                                               |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: section code if everything is ok else SEC_NOTSET         |
 *-------------------------------------------------------------------*/
DWORD dxfFindNextSection( HDXF hDxf )
{
	PDXF	pDxf;
	int		GCode;
	char	SectionName[32];

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Set CurrentSection to SEC_NOTSET
	pDxf->Read.CurrentSection = SEC_NOTSET;

	// Search for Section
	if(!dxfFindParam( hDxf, 0, "SECTION" ))
	{
		// Invalid Dxf file format
		GlobalUnlock(hDxf);
		return SEC_NOTSET;
	}

	if(/*!feof(pDxf->pStream)*/pDxf->Read.CurrentPos<pDxf->Read.FileSize)
	{
		dxfReadParam(hDxf, GCode, SectionName);
		if(strcmp(SectionName, "HEADER")==0)
			pDxf->Read.CurrentSection = SEC_HEADER;
		else if(strcmp(SectionName, "TABLES")==0)
			pDxf->Read.CurrentSection = SEC_TABLES;
		else if(strcmp(SectionName, "BLOCKS")==0)
			pDxf->Read.CurrentSection = SEC_BLOCKS;
		else if(strcmp(SectionName, "ENTITIES")==0)
			pDxf->Read.CurrentSection = SEC_ENTITIES;
		else
			pDxf->Read.CurrentSection = SEC_UNKNOWN;
	}

	// UnInitilize hDxf -----------------
	UnInitilizePDXF(hDxf);

	return pDxf->Read.CurrentSection;
}

/*-------------------------------------------------------------------*
 |  dxfGetCurrentSection                                             |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: current section code if everything is ok                 |
 *-------------------------------------------------------------------*/
DWORD dxfGetCurrentSection( HDXF hDxf )
{
	PDXF	pDxf;
	DWORD	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	result = pDxf->Read.CurrentSection;
	
	// UnInitilize hDxf -----------------
	UnInitilizePDXF(hDxf);

	return result;
}

