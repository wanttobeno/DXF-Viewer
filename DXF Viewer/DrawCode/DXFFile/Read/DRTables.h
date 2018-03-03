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
|  DRTables.h                                                         |
\*-------------------------------------------------------------------*/

#ifndef DRTABLES_H
#define DRTABLES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Base\DXFBase.h"

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
DWORD dxfReadTableTypeName(
	HDXF	hDxf		// handle to the openning DXF file structure
);

DWORD dxfGetCurrentTableType( 
	HDXF	hDxf		// handle to the openning DXF file structure
);

DWORD dxfFindNextTableType(
	HDXF	hDxf		// handle to the openning DXF file structure
);

DWORD dxfReadTableType(
	HDXF	hDxf,		// handle to the openning DXF file structure
	LPVOID	pTableType	// pointer to table type structure
);

#endif // DRTABLES_H
