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
|  DimView.cpp                                                        |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DimView.h"
#include "Colors.h"
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
/********************************************************************
 * Function for calculating dim boundary rectangle                  *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  GetDimRect                                                       |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTDIMENSION pDim = pointer to dimension data structure     |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetDimRect(PDRAWING pDrawing, PENTITYHEADER pEntityHeader, PENTDIMENSION pDim, REALRECT* pRect)
{
	DIMSTYLE	DimStyle;

	pRect->top = __max(pDim->DefPoint3.y, pDim->DefPoint4.y);
	pRect->top = __max(pRect->top, pDim->DimLineDefPoint.y);
	pRect->left = __min(pDim->DefPoint3.x, pDim->DefPoint4.x);
	pRect->left = __min(pRect->left, pDim->DimLineDefPoint.x);
	pRect->bottom = __min(pDim->DefPoint3.y, pDim->DefPoint4.y);
	pRect->bottom = __min(pRect->bottom, pDim->DimLineDefPoint.y);
	pRect->right = __max(pDim->DefPoint3.x, pDim->DefPoint4.x);
	pRect->right = __max(pRect->right, pDim->DimLineDefPoint.x);

	DimStyle.Objhandle = pDim->DimStyleObjhandle;
	if(drwFindTableType_Direct(pDrawing, TAB_DIMSTYLE, FIND_BYHANDLE, &DimStyle)>0)
	{
		pRect->top += DimStyle.dimtxt + DimStyle.dimgap;
		pRect->left -= DimStyle.dimtxt + DimStyle.dimgap;
		pRect->bottom -= DimStyle.dimtxt + DimStyle.dimgap;
		pRect->right += DimStyle.dimtxt + DimStyle.dimgap;
	}

	return TRUE;
}


/********************************************************************
 * Functions for plot the Dimension                                 *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  dimDrawArrowHead                                                 |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      OBJHANDLE BlockObjhandle = handle of block that entity must  |
 |                        be added in it. (NULL = Entity Section)    |
 |      PENTITYHEADER pHeader = pointer to entity data header        |
 |      double cx = x position of center point of arrowhead          |
 |      double cy = y position of center point of arrowhead          |
 |      double Angle = angle of arrowhead                            |
 |      double ArrowSize = arrow size                                |
 |      LPCTSTR ArrowHeadName = name of arrow head to draw           |
 |      int BLKNum = number of arrow head (can be 1 or 2)            |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dimDrawArrowHead( PDRAWING pDrawing, OBJHANDLE BlockObjhandle, PENTITYHEADER pHeader, double cx, double cy, double Angle, double ArrowSize, LPCTSTR ArrowHeadName, int BLKNum )
{
	ENTLINE		Line;
	ENTSOLID	Solid;

	// Write arrow head data to the Block-refrence

	for(int i=0; i<ArrowHeadsNum; i++)
	{
		if((strcmp(ArrowHeadName, ArrowHeads[i].ArrowHeadName)==0)
			&& (BLKNum==ArrowHeads[i].BLKNum))
		{
			if(strcmp(ArrowHeads[i].Command, "LINE")==0)
			{
				pHeader->EntityType = ENT_LINE;
				Line.Point0.x = cx + ((cos(Angle)*ArrowHeads[i].Param1-sin(Angle)*ArrowHeads[i].Param2)*ArrowSize);
				Line.Point0.y = cy + ((sin(Angle)*ArrowHeads[i].Param1+cos(Angle)*ArrowHeads[i].Param2)*ArrowSize);
				Line.Point1.x = cx + ((cos(Angle)*ArrowHeads[i].Param3-sin(Angle)*ArrowHeads[i].Param4)*ArrowSize);
				Line.Point1.y = cy + ((sin(Angle)*ArrowHeads[i].Param3+cos(Angle)*ArrowHeads[i].Param4)*ArrowSize);
				drwAddEntity_Direct(pDrawing, BlockObjhandle, pHeader, &Line);
			}
			else if(strcmp(ArrowHeads[i].Command, "SOLID")==0)
			{
				pHeader->EntityType = ENT_SOLID;
				Solid.Point0.x = cx + ((cos(Angle)*ArrowHeads[i].Param1-sin(Angle)*ArrowHeads[i].Param2)*ArrowSize);
				Solid.Point0.y = cy + ((sin(Angle)*ArrowHeads[i].Param1+cos(Angle)*ArrowHeads[i].Param2)*ArrowSize);
				Solid.Point1.x = cx + ((cos(Angle)*ArrowHeads[i].Param3-sin(Angle)*ArrowHeads[i].Param4)*ArrowSize);
				Solid.Point1.y = cy + ((sin(Angle)*ArrowHeads[i].Param3+cos(Angle)*ArrowHeads[i].Param4)*ArrowSize);
				Solid.Point2.x = cx + ((cos(Angle)*ArrowHeads[i].Param5-sin(Angle)*ArrowHeads[i].Param6)*ArrowSize);
				Solid.Point2.y = cy + ((sin(Angle)*ArrowHeads[i].Param5+cos(Angle)*ArrowHeads[i].Param6)*ArrowSize);
				Solid.Point3.x = cx + ((cos(Angle)*ArrowHeads[i].Param7-sin(Angle)*ArrowHeads[i].Param8)*ArrowSize);
				Solid.Point3.y = cy + ((sin(Angle)*ArrowHeads[i].Param7+cos(Angle)*ArrowHeads[i].Param8)*ArrowSize);
				drwAddEntity_Direct(pDrawing, BlockObjhandle, pHeader, &Solid);
			}
		}
	}

	return TRUE;
}


/*-------------------------------------------------------------------*
 |  DimLinear                                                        |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      OBJHANDLE BlockObjhandle = handle of block that entity must  |
 |                        be added in it. (NULL = Entity Section)    |
 |      PENTITYHEADER pDimHeader = pointer to dim data header        |
 |      PENTDIMENSION pDimension = pointer to dim data structure     |
 |      PDIMSTYLE pDimStyle = pointer of dim-style structure         |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL DimLinear( PDRAWING pDrawing, OBJHANDLE BlockObjhandle, PENTITYHEADER pDimHeader, PENTDIMENSION pDimension, PDIMSTYLE pDimStyle )
{
	BOOL			result;
	double			TextWidth;
	double			a, a1, a2, a3;
	double			sina1, cosa1;
	double			sina2, cosa2;
	REALPOINT		A1, A2, A3;
	REALPOINT		B1, B2, B3;
	REALPOINT		C1, C2, C3;
	double			pi = 3.1415926535;

	double			x1 = pDimension->DefPoint3.x;			// x position of start point
	double			y1 = pDimension->DefPoint3.y;			// y position of start point
	double			x2 = pDimension->DefPoint4.x;			// x position of end point
	double			y2 = pDimension->DefPoint4.y;			// y position of end point
	double			x3 = pDimension->DimLineDefPoint.x;		// x position of dimension line
	double			y3 = pDimension->DimLineDefPoint.y;		// y position of dimension line
	double			angle = pDimension->DimRotationAngle;	// dim-line's rotation angle
	LPCTSTR			text = pDimension->DimText;				// dim-line's text (dafault is NULL)

	ENTITYHEADER	EntityHeader;
	ENTPOINT		Point;
	ENTLINE			Line;
	ENTTEXT			Text;

	memcpy(&EntityHeader, pDimHeader, sizeof(ENTITYHEADER));
	// Writting dimension data to the drawing  ---------------------
	result = TRUE;

	a = tan( angle*pi/180 );
	if(a!=0)
	{
		A1.x = (y1 - y3 + x1/a + a*x3) / (a + 1/a);
		A1.y = a * (A1.x - x3) + y3;
		B1.x = (y2 - y3 + x2/a + a*x3) / (a + 1/a);
		B1.y = a * (B1.x - x3) + y3;

		a1 = (A1.y - y1) / (A1.x - x1);
		sina1 = (A1.x>x1 ? 1 : -1) * sin( atan(a1) );
		cosa1 = (A1.x>x1 ? 1 : -1) * cos( atan(a1) );
		A2.x = A1.x + cosa1*pDimStyle->dimexe;
		A2.y = A1.y + sina1*pDimStyle->dimexe;
		A3.x = x1 + cosa1*pDimStyle->dimexo;
		A3.y = y1 + sina1*pDimStyle->dimexo;

		a2 = (B1.y - y2) / (B1.x - x2);
		sina2 = (B1.x>x2 ? 1 : -1) * sin( atan(a2) );
		cosa2 = (B1.x>x2 ? 1 : -1) * cos( atan(a2) );
		B2.x = B1.x + cosa2*pDimStyle->dimexe;
		B2.y = B1.y + sina2*pDimStyle->dimexe;
		B3.x = x2 + cosa2*pDimStyle->dimexo;
		B3.y = y2 + sina2*pDimStyle->dimexo;
	}
	else
	{
		A1.x = x1;
		A1.y = y3;
		B1.x = x2;
		B1.y = y3;

		A2.x = A1.x;
		A2.y = A1.y + _copysign(pDimStyle->dimexe, A1.y-y1);
		A3.x = x1;
		A3.y = y1 + _copysign(pDimStyle->dimexo, A1.y-y1);

		B2.x = B1.x;
		B2.y = B1.y + _copysign(pDimStyle->dimexe, B1.y-y2);
		B3.x = x2;
		B3.y = y2 + _copysign(pDimStyle->dimexo, B1.y-y2);
	}

	C1.x = (A1.x + B1.x) / 2;
	C1.y = (A1.y + B1.y) / 2;

	// Begin & End Points ----
	EntityHeader.EntityType = ENT_POINT;
	Point.Point0.x = x1; Point.Point0.y = y1;
	drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Point);
	Point.Point0.x = x2; Point.Point0.y = y2;
	drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Point);

	// Dimension Line ----
	EntityHeader.Color = pDimStyle->dimclrd;

	switch(pDimStyle->dimtad)
	{
	case 0: // Centered
		// At this time the program cannot calculate the exact width of the text
		TextWidth = (strlen(text) * pDimStyle->dimtxt * 0.3) + pDimStyle->dimgap*2;
		C2.x = C1.x + _copysign( cos(angle*pi/180), A1.x-B1.x)*TextWidth;
		C2.y = C1.y + _copysign( sin(angle*pi/180), A1.x-B1.x)*TextWidth;
		C3.x = C1.x + _copysign( cos(angle*pi/180), B1.x-A1.x)*TextWidth;
		C3.y = C1.y + _copysign( sin(angle*pi/180), B1.x-A1.x)*TextWidth;

		EntityHeader.EntityType = ENT_LINE;
		Line.Point0.x = A1.x; Line.Point0.y = A1.y;
		Line.Point1.x = C2.x; Line.Point1.y = C2.y;
		drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Line);
		Line.Point0.x = C3.x; Line.Point0.y = C3.y;
		Line.Point1.x = B1.x; Line.Point1.y = B1.y;
		drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Line);

		// Text
		EntityHeader.Color = pDimStyle->dimclrt;

		EntityHeader.EntityType = ENT_TEXT;
		Text.Point0.x = C1.x; Text.Point0.y = C1.y;
		Text.TextData.SecondAlignmentPoint.x = C1.x; Text.TextData.SecondAlignmentPoint.y = C1.y;
		Text.TextData.Height = pDimStyle->dimtxt;
		Text.TextData.RotationAngle = angle;
		Text.TextData.Justification = TJ_MIDDLE;
		Text.TextData.WidthFactor = 0;
		Text.TextData.TextStyleObjhandle = pDimStyle->dimtxstyObjhandle;
		strcpy(Text.strText, pDimension->DimText);
		drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Text);
		break;
	case 1: // Above
		EntityHeader.EntityType = ENT_LINE;
		Line.Point0.x = A1.x; Line.Point0.y = A1.y;
		Line.Point1.x = B1.x; Line.Point1.y = B1.y;
		drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Line);
		// Text
		EntityHeader.Color = pDimStyle->dimclrt;

		EntityHeader.EntityType = ENT_TEXT;
		Text.Point0.x = C1.x + cos((angle+90)*pi/180)*(pDimStyle->dimgap+pDimStyle->dimtxt/2);
		Text.Point0.y = C1.y + sin((angle+90)*pi/180)*(pDimStyle->dimgap+pDimStyle->dimtxt/2);
		Text.TextData.SecondAlignmentPoint.x = Text.Point0.x;
		Text.TextData.SecondAlignmentPoint.y = Text.Point0.y;
		Text.TextData.Height = pDimStyle->dimtxt;
		Text.TextData.RotationAngle = angle;
		Text.TextData.Justification = TJ_MIDDLE;
		Text.TextData.WidthFactor = 0;
		Text.TextData.TextStyleObjhandle = pDimStyle->dimtxstyObjhandle;
		strcpy(Text.strText, pDimension->DimText);
		drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Text);
		break;
	}

	// Arrow Heads ----
	EntityHeader.Color = pDimStyle->dimclrd;

	a3 = (B1.y - A1.y) / (B1.x - A1.x);

	if(B1.x>A1.x)
	{
		// 1st Arrow Head
		result &= dimDrawArrowHead( pDrawing, BlockObjhandle, &EntityHeader, A1.x, A1.y, pi+atan(a3), pDimStyle->dimasz, pDimStyle->dimblk1, 1 );
		// 2nd Arrow Head
		result &= dimDrawArrowHead( pDrawing, BlockObjhandle, &EntityHeader, B1.x, B1.y, atan(a3), pDimStyle->dimasz, pDimStyle->dimblk2, 2 );
	}
	else if(B1.x<A1.x)
	{
		// 1st Arrow Head
		result &= dimDrawArrowHead( pDrawing, BlockObjhandle, &EntityHeader, A1.x, A1.y, atan(a3), pDimStyle->dimasz, pDimStyle->dimblk1, 1 );
		// 2nd Arrow Head
		result &= dimDrawArrowHead( pDrawing, BlockObjhandle, &EntityHeader, B1.x, B1.y, pi+atan(a3), pDimStyle->dimasz, pDimStyle->dimblk2, 2 );
	}
	else // (B1.x==A1.x)
	{
		// 1st Arrow Head
		result &= dimDrawArrowHead( pDrawing, BlockObjhandle, &EntityHeader, A1.x, A1.y, (B1.y>A1.y?0:1)*pi+atan(a3), pDimStyle->dimasz, pDimStyle->dimblk1, 1 );
		// 2nd Arrow Head
		result &= dimDrawArrowHead( pDrawing, BlockObjhandle, &EntityHeader, B1.x, B1.y, (B1.y>A1.y?1:0)*pi+atan(a3), pDimStyle->dimasz, pDimStyle->dimblk2, 2 );
	}

	// Extension Lines ----
	EntityHeader.Color = pDimStyle->dimclre;
	// 1st Extension Line
	EntityHeader.EntityType = ENT_LINE;
	Line.Point0.x = A2.x; Line.Point0.y = A2.y;
	Line.Point1.x = A3.x; Line.Point1.y = A3.y;
	drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Line);
	// 2nd Extension Line
	EntityHeader.EntityType = ENT_LINE;
	Line.Point0.x = B2.x; Line.Point0.y = B2.y;
	Line.Point1.x = B3.x; Line.Point1.y = B3.y;
	drwAddEntity_Direct(pDrawing, BlockObjhandle, &EntityHeader, &Line);

	return result;
}


// Units to pixel conversions
#define	U2PX(units)	pDrawing->View.WindowLeft + (int)((-pDrawing->View.ViewLeft + x+units*xScale) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define	U2PY(units)	pDrawing->View.WindowBottom - (int)((-pDrawing->View.ViewBottom + y+units*yScale) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define	U2PXROT(xUnits,yUnits)	pDrawing->View.WindowLeft + (int)((-pDrawing->View.ViewLeft + x+(xUnits*xScale*cos(Rotation*PI/180)-yUnits*yScale*sin(Rotation*PI/180))*1) * pDrawing->View.PPU pDrawing->View.ZoomLevel)
#define	U2PYROT(xUnits,yUnits)	pDrawing->View.WindowBottom - (int)((-pDrawing->View.ViewBottom + y+(xUnits*xScale*sin(Rotation*PI/180)+yUnits*yScale*cos(Rotation*PI/180))*1) * pDrawing->View.PPU pDrawing->View.ZoomLevel)
#define PI	3.1415926535

/*-------------------------------------------------------------------*
 |  isDimInViewableArea                                              |
 |  Inputs:                                                          |
 |      PVIEW pView = pointer to the current Drawing View structure  |
 |      RECT *pDimRect = pointer to dimension boundary rectangle     |
 |  Output:                                                          |
 |      TRUE if text can be shown in viewable area                   |
 *-------------------------------------------------------------------*/
BOOL isDimInViewableArea(PVIEW pView, RECT *pDimRect)
{
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  PlotDimension                                                    |
 |  Descriptions:                                                    |
 |      Plots a dimension into given Drawing Context                 |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      HDC hdc= handle of DC for drawing                            |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTDIMENSION pDimension = pointer to dimension data         |
 |      double xOffset = x position of insertion point (default = 0) |
 |      double yOffset = y position of insertion point (default = 0) |
 |      double xScale = x scale factor (default = 1)                 |
 |      double yScale = y scale factor (default = 1)                 |
 |      double Rotation = rotation angle (default = 0)               |
 |  Output: Nothing                                                  |
 *-------------------------------------------------------------------*/
void PlotDimension(PDRAWING pDrawing, HDC hdc, PENTITYHEADER pEntityHeader, PENTDIMENSION pDimension, double x=0, double y=0, double xScale=1, double yScale=1, double Rotation=0)
{
	LAYER		Layer;
//	LTYPE		LType;
//	STYLE		Style;
	COLORREF	rgbcolor;
	short		color;
	HPEN		pen, oldpen;

	// Initialize LayerName & LineTypeName & Color
	color = pEntityHeader->Color;

	Layer.Objhandle = pEntityHeader->LayerObjhandle;
	if(drwFindTableType_Direct(pDrawing, TAB_LAYER, FIND_BYHANDLE, &Layer)>0)
	{
		if((pEntityHeader->Color==COLOR_BYBLOCK) || (pEntityHeader->Color==COLOR_BYLAYER))
			color = Layer.Color;
		else if (Layer.Color<0)
		{
			return;
		}
	}
	else if(color>=COLOR_BYLAYER)
		color = 0;

	rgbcolor = CadColor[color];

	pen = CreatePen(PS_SOLID, 0, rgbcolor);
	oldpen = (HPEN)SelectObject(hdc, pen);

//	LType.Objhandle = pEntityHeader->LTypeObjhandle;
//	drwFindTableType(hDrawing, TAB_LTYPE, FIND_BYHANDLE, &LType);

	// Plot the Dimension

	SelectObject(hdc, oldpen);
	DeleteObject(pen);
}


