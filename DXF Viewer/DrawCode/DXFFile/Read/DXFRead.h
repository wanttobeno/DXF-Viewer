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
|  DXFRead.h                                                          |
\*-------------------------------------------------------------------*/

#ifndef DXFREAD_H
#define DXFREAD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Base\DXFBase.h"
#include "DRTables.h"
#include "DREntities.h"
#include "DRBlocks.h"

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
// *** Construction & Destruction ***
HDXF dxfOpenFile( 
	LPCTSTR	FileName	// pointer to DXF file name
);

// *** Sections ***
DWORD dxfFindNextSection(
	HDXF	hDxf		// handle to the openning DXF file structure
);

DWORD dxfGetCurrentSection(
	HDXF	hDxf		// handle to the openning DXF file structure
);


#endif // DXFREAD_H
