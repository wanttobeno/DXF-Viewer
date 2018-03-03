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
|  DWBlocks.cpp                                                       |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DWBlocks.h"


/*-------------------------------------------------------------------*
 |  dxfBlockBegin                                                    |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      LPCTSTR	BlockName = block name                               |
 |      LPCTSTR LayerName = layer name                               |
 |      double bx = x position of base point                         |
 |      double by = y position of base point                         |
 |      char flags = flags                                           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfBlockBegin(	HDXF hDxf, LPCTSTR BlockName, LPCTSTR LayerName, double bx, double by, char flags )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is BLOCKS
	if(!(pDxf->Write.CurrentSection & SEC_BLOCKS))
	{
		// Current section is not BLOCKS
		UnInitilizePDXF(hDxf);
		return FALSE;
	}

	// Process new block ----------------
	if(pDxf->Write.CurrentSection & SEC_ENTITIES)
	{
		// Last block is not closed
		UnInitilizePDXF(hDxf);
		return FALSE;
	}
	else
		pDxf->Write.CurrentSection |= SEC_ENTITIES;

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "BLOCK");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, LayerName);
	// Block Name
	result &= dxfWriteParamString(hDxf, 2, BlockName);
	// Block Flag
	result &= dxfWriteParamInteger(hDxf, 70, flags);
	// Base point
	result &= dxfWriteParamDouble(hDxf, 10, bx);
	result &= dxfWriteParamDouble(hDxf, 20, by);
	// Block Name
	result &= dxfWriteParamString(hDxf, 3, BlockName);

	if(!result)
	{
		UnInitilizePDXF(hDxf);
		return FALSE;
	}

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfBlockEnd                                                      |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfBlockEnd( HDXF hDxf )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is BLOCKS
	if(!(pDxf->Write.CurrentSection & SEC_BLOCKS))
	{
		// Current section is not BLOCKS
		UnInitilizePDXF(hDxf);
		return FALSE;
	}

	// Check if new block is began
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// There is not any new block began
		UnInitilizePDXF(hDxf);
		return FALSE;
	}
	else
		pDxf->Write.CurrentSection = SEC_BLOCKS;

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "ENDBLK");

	if(!result)
	{
		UnInitilizePDXF(hDxf);
		return FALSE;
	}

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}
