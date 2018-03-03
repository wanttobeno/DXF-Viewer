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
|  DWBlocks.h                                                         |
\*-------------------------------------------------------------------*/

#ifndef DWBLOCKS_H
#define DWBLOCKS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Base\DXFBase.h"

//---------------------------------------------------------------------------
// Function declarations
//---------------------------------------------------------------------------
// *** BLOCKS Section ***
BOOL dxfBlockBegin(
	HDXF	hDxf,		// handle to the openning DXF file structure
	LPCTSTR	BlockName,	// block name
	LPCTSTR	LayerName,	// layer name
	double	bx,			// x position of base point
	double	by,			// y position of base point
	char	flags		// flags
);

BOOL dxfBlockEnd(
	HDXF	hDxf		// handle to the openning DXF file structure
);

#endif // DWBLOCKS_H
