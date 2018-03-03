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
|  DWEntities.h                                                       |
\*-------------------------------------------------------------------*/

#ifndef DWENTITIES_H
#define DWENTITIES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Base\DXFBase.h"

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
// *** ENTITIES Section ***
BOOL dxfLine( 
	HDXF	hDxf,		// handle to the openning DXF file structure
	double	x1,			// x position of start point
	double	y1,			// y position of start point
	double	x2,			// x position of end point
	double	y2			// y position of end point
);

BOOL dxfCircle(
	HDXF	hDxf,		// handle to the openning DXF file structure
	double	cx,			// x position of center point
	double	cy,			// y position of center point
	double	r			// radius
);

BOOL dxfEllipse(
	HDXF	hDxf,		// handle to the openning DXF file structure
	double	cx,			// x position of center point
	double	cy,			// y position of center point
	double	ex,			// x position of endpoint of major axis
	double	ey,			// y position of endpoint of major axis
	double	Ratio,		// Ratio of minor axis to major axis
	double	StartParam,	// Start parameter (this value is 0.0 for a full ellipse)
	double	EndParam	// End parameter (this value is 2pi for a full ellipse)
);

BOOL dxfPolyLine(
	HDXF			hDxf,	// handle to the openning DXF file structure 
	PDXFENTVERTEX	pVertex,// pointer of pline vertices
	int				nVertex,// number of vertices in Polyline
	unsigned short	Flag	// Polyline flag (bit-coded)
);

BOOL dxfSolid( 
	HDXF		hDxf,		// handle to the openning DXF file structure
	REALPOINT&	Point0,		// point 0
	REALPOINT&	Point1,		// point 1
	REALPOINT&	Point2,		// point 2
	REALPOINT&	Point3		// point 3
);

BOOL dxfText( 
	HDXF	hDxf,			// handle to the openning DXF file structure
	LPCTSTR	Text,			// text to be shown
	double	x1,				// x of first alignment point
	double	y1,				// y of first alignment point
	double	x2,				// x of second alignment point (by default must be same as x1)
	double	y2,				// y of second alignment point (by default must be same as y1)
	double	Height,			// height of text
	double	Rotation,		// rotation angle of text
	DWORD	Justification,	// horizontal & vertical text justification
	double	WidthFactor,	// Relative X scale factor--width (default = 1.0)
	LPCTSTR	StyleName		// text style name, NULL = current style
);

BOOL dxfPoint( 
	HDXF	hDxf,		// handle to the openning DXF file structure
	double	x,			// x position of point
	double	y,			// y position of point
	double	thickness	// Thickness (optional; default = 0)
);

BOOL dxfDimLinear(
	HDXF	hDxf,		// handle to the openning DXF file structure
	double	x1,			// x position of start point
	double	y1,			// y position of start point
	double	x2,			// x position of end point
	double	y2,			// y position of end point
	double	x3,			// x position of dimension line
	double	y3,			// y position of dimension line
	double	angle,		// dim-line's rotation angle
	LPCTSTR	text		// overwrite text (dafault is NULL)
);

BOOL dxfArc(
	HDXF	hDxf,		// handle to the openning DXF file structure
	double	cx,			// x position of center point
	double	cy,			// y position of center point
	double	r,			// radius of arc
	double	StartAngle,	// start angle of arc
	double	EndAngle,	// end angle of arc
	double	Thickness	// Thickness (optional; default = 0)
);

BOOL dxfInsertBlock(
	HDXF	hDxf,		// handle to the openning DXF file structure
	LPCTSTR	BlockName,	// block name to be inserted
	double	x,			// x position of insertion point
	double	y,			// y position of insertion point
	double	xScale,		// x scale factor (optional; default = 1)
	double	yScale,		// y scale factor (optional; default = 1)
	double	Rotation	// rotation angle (optional; default = 0)
);


#endif // DWENTITIES_H
