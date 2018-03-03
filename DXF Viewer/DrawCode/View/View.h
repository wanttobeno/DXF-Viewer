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
|  View.h                                                             |
\*-------------------------------------------------------------------*/

#ifndef VIEW_H
#define VIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include "..\MemDrw.h"

// Definitions


//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
// Initialize Drawing
BOOL drwInitView( 
	HDRAWING	hDrawing,			// handle to the current DRAWING structure
	int			x,					// horizontal position of drawing window
	int			y,					// vertical position of drawing window
	int			Width,				// width of drawing window
	int			Height				// height of drawing window
);

BOOL drwPaint(
	HDRAWING	hDrawing,			// handle to the current DRAWING structure
	HDC			hdc					// handle to view window DC
);

// Functions for changing view properties
BOOL drwGetViewProperties(
	HDRAWING	hDrawing,			// handle to the current DRAWING structure
	PVIEW		pView				// pointer to view properties structure
);

BOOL drwSetViewProperties(
	HDRAWING	hDrawing,			// handle to the current DRAWING structure
	PVIEW		pView				// pointer to view properties structure
);

// Zoom Functions
BOOL drwGetDrawingBorder( 
	HDRAWING	hDrawing,			// handle to the current DRAWING structure
	PREALRECT	pRect				// pointer to drawing boundary rect
);

BOOL drwZoomExtents( 
	HDRAWING	hDrawing			// handle to the current DRAWING structure
);

#endif // VIEW_H
