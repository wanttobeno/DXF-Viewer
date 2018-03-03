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
|  DWTables.h                                                         |
\*-------------------------------------------------------------------*/

#ifndef DWTABLES_H
#define DWTABLES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Base\DXFBase.h"

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
// *** TABLES Section ***
BOOL dxfTableTypeBegin( 
	HDXF	hDxf,			// handle to the openning DXF file structure
	DWORD	dwTableType		// ID of table type to begin
);

BOOL dxfTableTypeEnd( 
	HDXF	hDxf			// handle to the openning DXF file structure
);

//   Layers
BOOL dxfAddLayer( 
	HDXF	hDxf,			// handle to the openning DXF file structure
	char*	Name,			// layer name to be added
	int		Color,			// current layer's color number
	char*	Linetype		// current layer's linetype name
);

BOOL dxfSetCurrentLayer( 
	HDXF	hDxf,			// handle to the openning DXF file structure
	char*	Name,			// layer name to be set as current
	char*	LineType		// current layer's line type
);

//   Line Type
BOOL dxfAddLinetype( 
	HDXF	hDxf,			// handle to the openning DXF file structure
	PDXFLTYPE pLineType		// pointer to linetype structure
);

BOOL dxfSetCurrentLinetype( 
	HDXF	hDxf,			// handle to the openning DXF file structure
	char*	Name			// line type name to be set as current
);

//   Text Style
BOOL dxfAddTextStyle( 
	HDXF	hDxf,			// handle to the openning DXF file structure
	PDXFSTYLE pTextStyle	// pointer to textstyle structure
);

BOOL dxfSetCurrentTextStyle( 
	HDXF	hDxf,			// handle to the openning DXF file structure
	char*	Name			// textstyle name to be set as current
);

// Dim Style
BOOL dxfAddDimStyle(
	HDXF		hDxf,		// handle to the openning DXF file structure
	PDXFDIMSTYLE pDimStyle	// pointer to the dimstyle structure
);

BOOL dxfSetCurrentDimStyle(
	HDXF	hDxf,			// handle to the openning DXF file structure
	LPCTSTR	Name			// pointer of dimstyle name to be set as current
);


#endif // DWTABLES_H
