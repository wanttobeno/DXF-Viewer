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
|  DXFWrite.h                                                         |
\*-------------------------------------------------------------------*/

#ifndef DXFWRITE_H
#define DXFWRITE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\Base\DXFBase.h"
#include "DWTables.h"
#include "DWEntities.h"
#include "DWBlocks.h"

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
// *** Construction & Destruction ***
HDXF dxfCreateFile( 
	LPCTSTR	FileName	// pointer to DXF file name
);

BOOL dxfCloseFile( 
	HDXF	hDxf		// handle to the openning DXF file structure
);

// *** Initialization ***
BOOL dxfSetCurrentColor(
	HDXF	hDxf,		// handle to the openning DXF file structure
	int		Color		// Color number to set (0=ByBlock, 256=ByLayer, Negative=Layer is turned off)
);

// *** Sections ***
BOOL dxfSectionBegin( 
	HDXF	hDxf,		// handle to the openning DXF file structure
	DWORD	dwSection	// ID of the section to be opened
);

BOOL dxfSectionEnd( 
	HDXF	hDxf		// handle to the openning DXF file structure
);


#endif // DXFWRITE_H
