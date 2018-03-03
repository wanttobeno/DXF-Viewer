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
|  TextView.h                                                         |
\*-------------------------------------------------------------------*/

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

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
void PlotText(
    PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	HDC				hdc,			// handle of DC for drawing
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	PENTTEXT		pText,			// pointer to text data
	double			x,				// x position of insertion point (default = 0)
	double			y,				// y position of insertion point (default = 0)
	double			xScale,			// x scale factor (default = 1)
	double			yScale,			// y scale factor (default = 1)
	double			Rotation		// rotation angle (default = 0)
);

#endif // TEXTVIEW_H
