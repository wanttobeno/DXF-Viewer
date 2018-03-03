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
|  DREntities.h                                                       |
\*-------------------------------------------------------------------*/

#ifndef DRENTITIES_H
#define DRENTITIES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Base\DXFBase.h"

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
BOOL dxfReadEntityData( 
	HDXF				hDxf,			// handle to the openning DXF file structure
	PDXFENTITYHEADER	pEntityHeader,	// pointer to entity data header
	LPVOID				pEntityData		// pointer to entity data structure to read
);

BOOL dxfReadEntityData_Direct(
	PDXF				pDxf,			// pointer to the openning DXF file structure
	PDXFENTITYHEADER	pEntityHeader,	// pointer to entity data header
	LPVOID				pEntityData		// pointer to entity data structure to read
);

#endif // DRENTITIES_H
