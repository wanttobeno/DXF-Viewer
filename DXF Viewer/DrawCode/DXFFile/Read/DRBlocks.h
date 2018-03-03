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
|  DRBlocks.h                                                         |
\*-------------------------------------------------------------------*/

#ifndef DRBLOCKS_H
#define DRBLOCKS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Base\DXFBase.h"

//---------------------------------------------------------------------------
// Function declarations
//---------------------------------------------------------------------------
BOOL dxfReadBlockHeader( 
	HDXF			hDxf,		// handle to the openning DXF file structure
	PDXFBLOCKHEADER	pBlockHeader// pointer to blockheader structure
);

BOOL dxfFindBlockHeader(
	HDXF			hDxf,		// handle to the openning DXF file structure
	PDXFBLOCKHEADER	pBlockHeader// pointer to blockheader structure
);

BOOL dxfGetBlockOpen(
	HDXF			hDxf		// handle to the openning DXF file structure
);

#endif // DRBLOCKS_H
