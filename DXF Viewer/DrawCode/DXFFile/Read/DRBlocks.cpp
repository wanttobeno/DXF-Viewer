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
|  DRBlocks.cpp                                                       |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DRBlocks.h"

/*-------------------------------------------------------------------*
 |  ReadBlockHeader                                                  |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFBLOCKHEADER pBlockHeader =                               |
 |                           pointer to blockheader structure        |
 |  Output: Nothing!                                                 |
 *-------------------------------------------------------------------*/
void ReadBlockHeader( PDXF pDxf, PDXFBLOCKHEADER pBlockHeader )
{
	int		GCode;
	char	strValue[2048];

	dxfStorePos(pDxf);
	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pBlockHeader->LayerName, strValue);
			break;
		case 2:		// Block Name
			strcpy(pBlockHeader->Name, strValue);
			break;
		case 70:	// Block-type flags
			pBlockHeader->Flags = char(atoi(strValue));
		case 10:	// Base point X
			pBlockHeader->BasePoint.x = atoi(strValue);
			break;
		case 20:	// Base point Y
			pBlockHeader->BasePoint.y = atoi(strValue);
			break;
		case 30:	// Base point Z
			pBlockHeader->BasePoint.z = atoi(strValue);
			break;
		case 3:		// Block Name
			strcpy(pBlockHeader->Name, strValue);
			break;
		}
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}

	if((GCode==0) && (strcmp(strValue, "ENDBLK")==0))
	{
		pDxf->Read.isBlockOpen = FALSE;

		do{
			dxfStorePos(pDxf);
			ReadParamFromDxfFile(pDxf, GCode, strValue);
		} while(GCode!=0);

		if(strcmp(strValue, "ENDSEC")==0)
			pDxf->Read.CurrentSection = SEC_NOTSET; // Blocks section has been finished
		else // Reached to next block
			dxfRestorePos(pDxf);
	}
	else
	{
		dxfRestorePos(pDxf);
		pDxf->Read.isBlockOpen = TRUE;
	}
}

/*-------------------------------------------------------------------*
 |  dxfReadBlockHeader                                               |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      PDXFBLOCKHEADER pBlockHeader =                               |
 |                           pointer to blockheader structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfReadBlockHeader( HDXF hDxf, PDXFBLOCKHEADER pBlockHeader )
{
	PDXF	pDxf;
	int		GCode;
	char	strValue[2048];

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is BLOCKS
	if(pDxf->Read.CurrentSection!=SEC_BLOCKS)
	{
		// Current section is not BLOCKS
		GlobalUnlock(hDxf);
		return FALSE;
	}

	dxfStorePos(pDxf);
	ReadParamFromDxfFile(pDxf, GCode, strValue);
	if((GCode==0) && (strcmp(strValue, "BLOCK")==0))
		ReadBlockHeader(pDxf, pBlockHeader);
	else
	{
		// Can not read block header
		dxfRestorePos(pDxf);
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfFindBlockHeader                                               |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      PDXFBLOCKHEADER pBlockHeader =                               |
 |                           pointer to blockheader structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfFindBlockHeader( HDXF hDxf, PDXFBLOCKHEADER pBlockHeader )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is BLOCKS
	if(pDxf->Read.CurrentSection!=SEC_BLOCKS)
	{
		// Current section is not BLOCKS
		GlobalUnlock(hDxf);
		return FALSE;
	}

	dxfStorePos(pDxf);
	if(FindParamFromDxfFile(pDxf, 0, "BLOCK"))
		ReadBlockHeader(pDxf, pBlockHeader);
	else
	{
		// Can not read block header
		dxfRestorePos(pDxf);
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfGetBlockOpen                                                  |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: TRUE if block is open.                                   |
 *-------------------------------------------------------------------*/
BOOL dxfGetBlockOpen( HDXF hDxf )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	result = pDxf->Read.isBlockOpen;

	// UnInitilize hDxf -----------------
	UnInitilizePDXF(hDxf);

	return result;
}
