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
|  DimView.h                                                          |
\*-------------------------------------------------------------------*/

#ifndef DIMVIEW_H
#define DIMVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MemDrw.h"

// Definitions


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
BOOL GetDimRect(
	PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	PENTDIMENSION	pDim,			// pointer to dimension data structure
	REALRECT*		pRect			// pointer to entity boundary rectangle
);

BOOL DimLinear(
    PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	OBJHANDLE		BlockObjhandle,	// handle of block that entity must be added in it. (NULL = Entity Section)
	PENTITYHEADER	pDimHeader,		// pointer to dim data header
	PENTDIMENSION	pDimension,		// pointer to dim data structure
	PDIMSTYLE		pDimStyle		// pointer of dim-style structure
);

void PlotDimension(
    PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	HDC				hdc,			// handle of DC for drawing
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	PENTDIMENSION	pDimension,		// pointer to dimension data
	double			x,				// x position of insertion point (default = 0)
	double			y,				// y position of insertion point (default = 0)
	double			xScale,			// x scale factor (default = 1)
	double			yScale,			// y scale factor (default = 1)
	double			Rotation		// rotation angle (default = 0)
);

#endif // DIMVIEW_H
