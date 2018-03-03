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
|  View.cpp                                                           |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "View.h"
#include "Colors.h"
#include "TextView.h"
#include "DimView.h"

/********************************************************************
 * Functions for calculating entities boundary rectangle            *
 ********************************************************************/

BOOL GetEntityRect(PDRAWING pDrawing, PENTITYHEADER pEntityHeader, LPVOID pEntityData, REALRECT* pRect);

/*-------------------------------------------------------------------*
 |  GetLineRect                                                      |
 |  Inputs:                                                          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTLINE pLine = pointer to line data structure              |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetLineRect(PENTITYHEADER pEntityHeader, PENTLINE pLine, REALRECT* pRect)
{
	pRect->top = __max(pLine->Point0.y, pLine->Point1.y);
	pRect->left = __min(pLine->Point0.x, pLine->Point1.x);
	pRect->bottom = __min(pLine->Point0.y, pLine->Point1.y);
	pRect->right = __max(pLine->Point0.x, pLine->Point1.x);
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  GetPointRect                                                     |
 |  Inputs:                                                          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTPOINT pPoint = pointer to point data structure           |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetPointRect(PENTITYHEADER pEntityHeader, PENTPOINT pPoint, REALRECT* pRect)
{
	pRect->top = pPoint->Point0.y;
	pRect->left = pPoint->Point0.x;
	pRect->bottom = pPoint->Point0.y;
	pRect->right = pPoint->Point0.x;
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  GetCircleRect                                                    |
 |  Inputs:                                                          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTCIRCLE pCircle = pointer to circle data structure        |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetCircleRect(PENTITYHEADER pEntityHeader, PENTCIRCLE pCircle, REALRECT* pRect)
{
	pRect->top = pCircle->Point0.y + pCircle->Radius;
	pRect->left = pCircle->Point0.x - pCircle->Radius;
	pRect->bottom = pCircle->Point0.y - pCircle->Radius;
	pRect->right = pCircle->Point0.x + pCircle->Radius;
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  GetSolidRect                                                     |
 |  Inputs:                                                          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTSOLID pSolid = pointer to solid data structure           |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetSolidRect(PENTITYHEADER pEntityHeader, PENTSOLID pSolid, REALRECT* pRect)
{
	pRect->top = __max(pSolid->Point0.y, pSolid->Point1.y);
	pRect->top = __max(pRect->top, pSolid->Point2.y);
	pRect->top = __max(pRect->top, pSolid->Point3.y);
	pRect->left = __min(pSolid->Point0.x, pSolid->Point1.x);
	pRect->left = __min(pRect->left, pSolid->Point2.x);
	pRect->left = __min(pRect->left, pSolid->Point3.x);
	pRect->bottom = __min(pSolid->Point0.y, pSolid->Point1.y);
	pRect->bottom = __min(pRect->bottom, pSolid->Point2.y);
	pRect->bottom = __min(pRect->bottom, pSolid->Point3.y);
	pRect->right = __max(pSolid->Point0.x, pSolid->Point1.x);
	pRect->right = __max(pRect->right, pSolid->Point2.x);
	pRect->right = __max(pRect->right, pSolid->Point3.x);
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  GetInsertBlockRect                                               |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTINSERT pInsert = pointer to insert data structure        |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetInsertBlockRect(PDRAWING pDrawing, PENTITYHEADER pEntityHeader, PENTINSERT pInsert, REALRECT* pRect)
{
	BLOCKHEADER		BlockHeader;
	ENTITYHEADER	BlockEntityHeader;
	char			BlockEntityData[4096];

	pRect->left = DBL_MAX;
	pRect->top = DBL_MIN;
	pRect->right = DBL_MIN;
	pRect->bottom = DBL_MAX;

	BlockHeader.Objhandle = pInsert->BlockHeaderObjhandle;
	if(drwFindBlock_Direct(pDrawing, FIND_BYHANDLE, &BlockHeader)>0)
	{
		if(drwFindEntity_Direct(pDrawing, BlockHeader.Name, &BlockEntityHeader, &BlockEntityData, FIND_FIRST)>0)
		{
			do{
				REALRECT	Rect;
				if(GetEntityRect(pDrawing, &BlockEntityHeader, BlockEntityData, &Rect))
				{
					pRect->top = __max(Rect.top*pInsert->YScale+pInsert->Point0.y, pRect->top);
					pRect->left = __min(Rect.left*pInsert->XScale+pInsert->Point0.x, pRect->left);
					pRect->bottom = __min(Rect.bottom*pInsert->YScale+pInsert->Point0.y, pRect->bottom);
					pRect->right = __max(Rect.right*pInsert->XScale+pInsert->Point0.x, pRect->right);
				}
			} while(drwFindEntity_Direct(pDrawing, BlockHeader.Name, &BlockEntityHeader, &BlockEntityData, FIND_NEXT)>0);
		}
	}

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  GetPolylineRect                                                  |
 |  Inputs:                                                          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTPOLYLINE pPolyline = pointer to polyline data structure  |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetPolylineRect(PENTITYHEADER pEntityHeader, PENTPOLYLINE pPolyline, REALRECT* pRect)
{
	pRect->top = pPolyline->pVertex[0].Point.y;
	pRect->left = pPolyline->pVertex[0].Point.x;
	pRect->bottom = pPolyline->pVertex[0].Point.y;
	pRect->right = pPolyline->pVertex[0].Point.x;

	for(int i=1; i<pPolyline->nVertex; i++)
	{
		pRect->top = __max(pRect->top, pPolyline->pVertex[i].Point.y);
		pRect->left = __min(pRect->left, pPolyline->pVertex[i].Point.x);
		pRect->bottom = __min(pRect->bottom, pPolyline->pVertex[i].Point.y);
		pRect->right = __max(pRect->right, pPolyline->pVertex[i].Point.x);
	}

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  GetEntityRect                                                    |
 |  Descriptions:                                                    |
 |      Calculates entity boundary rectangle                         |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure        |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetEntityRect(PDRAWING pDrawing, PENTITYHEADER pEntityHeader, LPVOID pEntityData, REALRECT* pRect)
{
	BOOL	result;

	switch( pEntityHeader->EntityType )
	{
	case ENT_LINE:
		result = GetLineRect(pEntityHeader, (PENTLINE)pEntityData, pRect);
		break;

	case ENT_POINT:
		result = GetPointRect(pEntityHeader, (PENTPOINT)pEntityData, pRect);
		break;

	case ENT_CIRCLE:
		result = GetCircleRect(pEntityHeader, (PENTCIRCLE)pEntityData, pRect);
		break;

//	case ENT_ELLIPSE:
//		break;

//	case ENT_TEXT:
//		break;

//	case ENT_ARC:
//		break;

	case ENT_SOLID:
		result = GetSolidRect(pEntityHeader, (PENTSOLID)pEntityData, pRect);
		break;

	case ENT_INSERT:
		result = GetInsertBlockRect(pDrawing, pEntityHeader, (PENTINSERT)pEntityData, pRect);
		break;

	case ENT_POLYLINE:
		result = GetPolylineRect(pEntityHeader, (PENTPOLYLINE)pEntityData, pRect);
		break;

	case ENT_DIMENSION:
		result = GetDimRect(pDrawing, pEntityHeader, (PENTDIMENSION)pEntityData, pRect);
		break;

	default: // Entity type is not accepted
		result = FALSE;
	}

	return result;
}

/********************************************************************
 * Functions for plot the drawing on DC                             *
 ********************************************************************/

// Units to pixel conversions
#define	U2PX(units)	pDrawing->View.WindowLeft + (int)((-pDrawing->View.ViewLeft + x+(units)*xScale) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define	U2PY(units)	pDrawing->View.WindowBottom - (int)((-pDrawing->View.ViewBottom + y+(units)*yScale) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define	U2PXROT(xUnits,yUnits)	pDrawing->View.WindowLeft + (int)((-pDrawing->View.ViewLeft + x+((xUnits)*xScale*cos(Rotation*PI/180)-(yUnits)*yScale*sin(Rotation*PI/180))*1) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define	U2PYROT(xUnits,yUnits)	pDrawing->View.WindowBottom - (int)((-pDrawing->View.ViewBottom + y+((xUnits)*xScale*sin(Rotation*PI/180)+(yUnits)*yScale*cos(Rotation*PI/180))*1) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define PI	3.1415926535

/*-------------------------------------------------------------------*
 |  DrawLine                                                         |
 |  Descriptions:                                                    |
 |      Draws a line in view area                                    |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      HDC hdc= handle of DC for drawing                            |
 |      int x1 = x1 of line                                          |
 |      int y1 = y1 of line                                          |
 |      int x2 = x2 of line                                          |
 |      int y2 = y2 of line                                          |
 |  Output: Nothing                                                  |
 *-------------------------------------------------------------------*/
void DrawLine(PDRAWING pDrawing, HDC hdc, COLORREF rgbcolor, int x1, int y1, int x2, int y2)
{
	double m;

	if(abs(x2-x1)>abs(y2-y1))
	{
		if(x1>x2)
		{
			int dummy;
			dummy = x1; x1 = x2; x2 = dummy;
			dummy = y1; y1 = y2; y2 = dummy;
		}
//		if((y2>pDrawing->View.WindowTop) && (y1<pDrawing->View.WindowBottom))
		{
			m = (double)(y2-y1)/(x2-x1);
			for(int i=__max(x1,pDrawing->View.WindowLeft); i<=__min(x2,pDrawing->View.WindowRight); i++)
			{
				int j = int(m*(i-x1))+y1;
				if((j>=pDrawing->View.WindowTop) && (j<=pDrawing->View.WindowBottom))
				  SetPixel( hdc, i, j, rgbcolor );
			}
		}
	}
	else
	{
		if(y1>y2)
		{
			int dummy;
			dummy = x1; x1 = x2; x2 = dummy;
			dummy = y1; y1 = y2; y2 = dummy;
		}
//		if((x2>pDrawing->View.WindowLeft) && (x1<pDrawing->View.WindowRight))
		{
			m = (double)(x2-x1)/(y2-y1);
			for(int j=__max(y1,pDrawing->View.WindowTop); j<=__min(y2,pDrawing->View.WindowBottom); j++)
			{
				int i = int(m*(j-y1))+x1;
				if((i>=pDrawing->View.WindowLeft) && (i<=pDrawing->View.WindowRight))
				  SetPixel( hdc, i, j, rgbcolor );
			}
		}
	}

}


/*-------------------------------------------------------------------*
 |  DrawDashLine                                                     |
 |  Descriptions:                                                    |
 |      Draws a line in view area                                    |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      HDC hdc= handle of DC for drawing                            |
 |      int x1 = x1 of line                                          |
 |      int y1 = y1 of line                                          |
 |      int x2 = x2 of line                                          |
 |      int y2 = y2 of line                                          |
 |      double PatternStartPoint                                     |
 |      double x = x position of insertion point (default = 0)       |
 |      double y = y position of insertion point (default = 0)       |
 |      double xScale = x scale factor (default = 1)                 |
 |      double yScale = y scale factor (default = 1)                 |
 |  Output: Nothing                                                  |
 *-------------------------------------------------------------------*/
void DrawDashLine(PDRAWING pDrawing, HDC hdc, LTYPE* pLType, double x1, double y1, double x2, double y2, COLORREF rgbcolor, double Scale, double PatternStartPoint, double x, double y, double xScale, double yScale, double Rotation)
{
	double x11, y11;
	double x12, y12;
	double m;
	double cosalpha;
	double l1, l2=0;
	double xend, yend;
	double MoveDirection = 1;
	int ElementNum;

	if(fabs(x2-x1)>fabs(y2-y1))
	{
		if(x1>x2)
			MoveDirection = -1;

		m = (y2-y1)/(x2-x1);
		cosalpha = 1 / sqrt(1+m*m);

		x11 = x1;//__max(x1,pDrawing->View.ViewLeft);
		y11 = y1 + m*(x11-x1);
		l1 = sqrt((x1-x11)*(x1-x11)+(y1-y11)*(y1-y11));
		l2 = fmod(PatternStartPoint + l1, pLType->PatternLength*Scale); 
		for(ElementNum=0; ElementNum<pLType->ElementsNumber; ElementNum++)
		{
			if(l2<fabs(pLType->Elements[ElementNum]*Scale))
				break;
			l2 = l2 - fabs(pLType->Elements[ElementNum]*Scale);
		}

		x12 = x11 + MoveDirection * (fabs(pLType->Elements[ElementNum]*Scale)-l2)*(cosalpha);
		y12 = y11 + (x12-x11)*(m);

		xend = x2;//__min(x2,pDrawing->View.ViewRight);
		while(MoveDirection*x12<MoveDirection*xend)
		{
			if(pLType->Elements[ElementNum]>=0)
//				DrawLine( pDrawing, hdc, rgbcolor, U2PX(x11), U2PY(y11), U2PX(x12), U2PY(y12) );
				DrawLine( pDrawing, hdc, rgbcolor, U2PXROT(x11,y11), U2PYROT(x11,y11), U2PXROT(x12,y12), U2PYROT(x12,y12) );
	
			ElementNum++;
			if(ElementNum>=pLType->ElementsNumber)
				ElementNum=0;

			x11 = x12;
			y11 = y12;
			x12 = x11 + MoveDirection * fabs(pLType->Elements[ElementNum]*Scale)*(cosalpha);
			y12 = y11 + (x12-x11)*(m);
		}
		if(pLType->Elements[ElementNum]>=0)
//			DrawLine( pDrawing, hdc, rgbcolor, U2PX(x11), U2PY(y11), U2PX(x2), U2PY(y2) );
			DrawLine( pDrawing, hdc, rgbcolor, U2PXROT(x11,y11), U2PYROT(x11,y11), U2PXROT(x2,y2), U2PYROT(x2,y2) );
	//*****************************************************************************************
	}
	else
	{
		if(y1>y2)
			MoveDirection = -1;

		m = (x2-x1)/(y2-y1);
		cosalpha = 1 / sqrt(1+m*m);

		y11 = y1;//__max(y1,pDrawing->View.WindowBottom);
		x11 = x1 + m*(y11-y1);
		l1 = sqrt((x1-x11)*(x1-x11)+(y1-y11)*(y1-y11));
		l2 = fmod(PatternStartPoint + l1, pLType->PatternLength*Scale); 
		for(ElementNum=0; ElementNum<pLType->ElementsNumber; ElementNum++)
		{
			if(l2<fabs(pLType->Elements[ElementNum]*Scale))
				break;
			l2 = l2 - fabs(pLType->Elements[ElementNum]*Scale);
		}

		y12 = y11 + MoveDirection * (fabs(pLType->Elements[ElementNum]*Scale)-l2)*(cosalpha);
		x12 = x11 + (y12-y11)*(m);

		yend = y2;//__min(y2,pDrawing->View.ViewTop);
		while(MoveDirection*y12<MoveDirection*yend)
		{
			if(pLType->Elements[ElementNum]>=0)
//				DrawLine( pDrawing, hdc, rgbcolor, U2PX(x11), U2PY(y11), U2PX(x12), U2PY(y12) );
				DrawLine( pDrawing, hdc, rgbcolor, U2PXROT(x11,y11), U2PYROT(x11,y11), U2PXROT(x12,y12), U2PYROT(x12,y12) );
	
			ElementNum++;
			if(ElementNum>=pLType->ElementsNumber)
				ElementNum=0;

			y11 = y12;
			x11 = x12;
			y12 = y11 + MoveDirection * fabs(pLType->Elements[ElementNum]*Scale)*(cosalpha);
			x12 = x11 + (y12-y11)*(m);
		}
		if(pLType->Elements[ElementNum]>=0)
//			DrawLine( pDrawing, hdc, rgbcolor, U2PX(x11), U2PY(y11), U2PX(x2), U2PY(y2) );
			DrawLine( pDrawing, hdc, rgbcolor, U2PXROT(x11,y11), U2PYROT(x11,y11), U2PXROT(x2,y2), U2PYROT(x2,y2) );
	}
}


/*-------------------------------------------------------------------*
 |  PlotEntity                                                       |
 |  Descriptions:                                                    |
 |      Plots an entity into given Drawing Context                   |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      HDC hdc= handle of DC for drawing                            |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      LPVOID pEntityData = pointer to entity data structure        |
 |      double x = x position of insertion point (default = 0)       |
 |      double y = y position of insertion point (default = 0)       |
 |      double xScale = x scale factor (default = 1)                 |
 |      double yScale = y scale factor (default = 1)                 |
 |      double Rotation = rotation angle (default = 0)               |
 |  Output: Nothing                                                  |
 *-------------------------------------------------------------------*/
void PlotEntity(PDRAWING pDrawing, HDC hdc, PENTITYHEADER pEntityHeader, LPVOID pEntityData, double x=0, double y=0, double xScale=1, double yScale=1, double Rotation=0)
{
	LAYER		Layer;
	LTYPE		LType;
	COLORREF	rgbcolor;
	short		color;
	HPEN		pen, oldpen;
	HBRUSH		brush, oldbrush;
	int			i;
	BOOL		isLTypeAvailable = FALSE;

	// Exit and do not plot, if this object is deleted
	if(pEntityHeader->Deleted)
		return;

	if(pEntityHeader->EntityType!=ENT_TEXT)
	{
		// Initialize LayerName & LineTypeName & Color
		color = pEntityHeader->Color;

		Layer.Objhandle = pEntityHeader->LayerObjhandle;
		if(drwFindTableType_Direct(pDrawing, TAB_LAYER, FIND_BYHANDLE, &Layer)>0)
		{
			if((pEntityHeader->Color==COLOR_BYBLOCK) || (pEntityHeader->Color==COLOR_BYLAYER))
				color = Layer.Color;
			else if (Layer.Color<0)
			{
				// Layer is off
				return;
			}
		}
		else if(color>=COLOR_BYLAYER)
			color = 0;

		rgbcolor = CadColor[color];

		pen = CreatePen(PS_SOLID, 0, rgbcolor);
		oldpen = (HPEN)SelectObject(hdc, pen);
	
		LType.Objhandle = pEntityHeader->LTypeObjhandle;
		isLTypeAvailable = drwFindTableType_Direct(pDrawing, TAB_LTYPE, FIND_BYHANDLE, &LType);
	}

	// Plot entities data on DC
	switch(pEntityHeader->EntityType)
	{
	case ENT_LINE:
/*		if( !(((x1*xScale+x<pDrawing->View.ViewLeft) && (x2*xScale+x<pDrawing->View.ViewLeft)) || 
			((x1*xScale+x>pDrawing->View.ViewRight) && (x2*xScale+x>pDrawing->View.ViewRight)) || 
			((y1*yScale+y<pDrawing->View.ViewBottom) && (y2*yScale+y<pDrawing->View.ViewBottom)) || 
			((y1*yScale+y>pDrawing->View.ViewTop) && (y2*yScale+y>pDrawing->View.ViewTop))) )
*/		{
			double x1 = ((PENTLINE)pEntityData)->Point0.x;
			double y1 = ((PENTLINE)pEntityData)->Point0.y;
			double x2 = ((PENTLINE)pEntityData)->Point1.x;
			double y2 = ((PENTLINE)pEntityData)->Point1.y;

			if((LType.PatternLength==0) || (!isLTypeAvailable))
			{ // Solid Line
				DrawLine(pDrawing, hdc, rgbcolor, U2PXROT(x1,y1), U2PYROT(x1,y1), U2PXROT(x2,y2), U2PYROT(x2,y2));
			}
			else
			{ // Line has Pattern
				double scale = pEntityHeader->LineTypeScale;

				double linelen = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
				double PatternStartPoint = fabs(LType.Elements[0]*scale)/2;
				double LinePatternStartPoint = fmod(linelen,LType.PatternLength*scale)/2;

				if((linelen<LType.PatternLength*scale) || (LType.PatternLength*scale*pDrawing->View.PPU*pDrawing->View.ZoomLevel<5))
				{
					DrawLine(pDrawing, hdc, rgbcolor, U2PXROT(x1,y1), U2PYROT(x1,y1), U2PXROT(x2,y2), U2PYROT(x2,y2));
				}
				else
				{
					double m, cosalpha;
					double x11, y11;
					double x21, y21;

					if(fabs(x2-x1)>fabs(y2-y1))
					{
						if(x1>x2)
						{
							double dummy;
							dummy = x1; x1 = x2; x2 = dummy;
							dummy = y1; y1 = y2; y2 = dummy;
						}
						m = (y2-y1)/(x2-x1);
						cosalpha = 1 / sqrt(1+m*m);
						x11 = x1 + LinePatternStartPoint*cosalpha;
						y11 = y1 + (x11-x1)*m;
						x21 = x2 - LinePatternStartPoint*cosalpha;
						y21 = y2 + (x21-x2)*m;
					}
					else
					{
						if(y1>y2)
						{
							double dummy;
							dummy = x1; x1 = x2; x2 = dummy;
							dummy = y1; y1 = y2; y2 = dummy;
						}
						m = (x2-x1)/(y2-y1);
						cosalpha = 1 / sqrt(1+m*m);
						y11 = y1 + LinePatternStartPoint*cosalpha;
						x11 = x1 + (y11-y1)*m;
						y21 = y2 - LinePatternStartPoint*cosalpha;
						x21 = x2 + (y21-y2)*m;
					}

					DrawLine( pDrawing, hdc, rgbcolor, U2PXROT(x1,y1), U2PYROT(x1,y1), U2PXROT(x11,y11), U2PYROT(x11,y11) );
					DrawDashLine(pDrawing, hdc, &LType, x11, y11, x21, y21, rgbcolor, scale, PatternStartPoint, x, y, xScale, yScale, Rotation);
					DrawLine( pDrawing, hdc, rgbcolor, U2PXROT(x21,y21), U2PYROT(x21,y21), U2PXROT(x2,y2), U2PYROT(x2,y2) );
				}
			}
		}
		break;

	case ENT_POINT:
		SetPixel(
			hdc,
			U2PX(((PENTPOINT)pEntityData)->Point0.x),
			U2PY(((PENTPOINT)pEntityData)->Point0.y),
			rgbcolor
		);
		break;

	case ENT_CIRCLE:
		double x1,y1,x2,y2,xc,yc,r;
		x1 = ((PENTCIRCLE)pEntityData)->Point0.x-((PENTCIRCLE)pEntityData)->Radius;
		y1 = ((PENTCIRCLE)pEntityData)->Point0.y-((PENTCIRCLE)pEntityData)->Radius;
		x2 = ((PENTCIRCLE)pEntityData)->Point0.x+((PENTCIRCLE)pEntityData)->Radius;
		y2 = ((PENTCIRCLE)pEntityData)->Point0.y+((PENTCIRCLE)pEntityData)->Radius;
		xc = ((PENTCIRCLE)pEntityData)->Point0.x;
		yc = ((PENTCIRCLE)pEntityData)->Point0.y;
		r = ((PENTCIRCLE)pEntityData)->Radius;
		
		if((LType.PatternLength==0) || (!isLTypeAvailable))
		{ // Solid Line Circle
			Ellipse(
			  hdc,
			  U2PXROT(x1,y1),
			  U2PYROT(x1,y1),
			  U2PXROT(x2,y2),
			  U2PYROT(x2,y2)
			);
		}
		else
		{ // Circle has Pattern
			double scale = pEntityHeader->LineTypeScale;

			double primeter = 2*PI*r*xScale;

			if((primeter<LType.PatternLength*scale) || (LType.PatternLength*scale*pDrawing->View.PPU*pDrawing->View.ZoomLevel<5))
			{
				Ellipse(
				  hdc,
				  U2PXROT(x1,y1),
				  U2PYROT(x1,y1),
				  U2PXROT(x2,y2),
				  U2PYROT(x2,y2)
				);
			}
			else
			{
				double n, alpha;
				double x11, y11;
				double x21, y21;
				double PatternStartPoint=0;
				double len = 0;

				scale = primeter/ceil(primeter/(LType.PatternLength*scale))/LType.PatternLength;
				n = ceil((primeter*pDrawing->View.ZoomLevel*pDrawing->View.PPU) / 5);
				if(n<8)	n = 8;
				if(n>100)
					n=100;
				alpha = 2*PI/n;

				x21 = xc + r;
				y21 = yc;
				for(int i=1; i<=n; i++)
				{
					x11 = x21;
					y11 = y21;
					x21 = xc + r*cos(alpha*double(i));
					y21 = yc + r*sin(alpha*double(i));
					len = sqrt((x11-x21)*(x11-x21)+(y11-y21)*(y11-y21));
					DrawDashLine(pDrawing, hdc, &LType, x11, y11, x21, y21, rgbcolor, scale, PatternStartPoint, x, y, xScale, yScale, Rotation);
					PatternStartPoint = fmod(PatternStartPoint+len,LType.PatternLength*scale);
				}
			}
		}
		
		break;

	case ENT_TEXT:
		PlotText(
			pDrawing,
			hdc, 
			pEntityHeader,
			(PENTTEXT)pEntityData,
			x, y,
			xScale, yScale,
			Rotation
		);
		break;

	case ENT_ARC:
		Arc(
			hdc,
			U2PX(((PENTARC)pEntityData)->Point0.x-((PENTARC)pEntityData)->Radius),
			U2PY(((PENTARC)pEntityData)->Point0.y-((PENTARC)pEntityData)->Radius),
			U2PX(((PENTARC)pEntityData)->Point0.x+((PENTARC)pEntityData)->Radius),
			U2PY(((PENTARC)pEntityData)->Point0.y+((PENTARC)pEntityData)->Radius),
			U2PX(((PENTARC)pEntityData)->Point0.x+((PENTARC)pEntityData)->Radius * cos(((PENTARC)pEntityData)->StartAngle*PI/180)),
			U2PY(((PENTARC)pEntityData)->Point0.y+((PENTARC)pEntityData)->Radius * sin(((PENTARC)pEntityData)->StartAngle*PI/180)),
			U2PX(((PENTARC)pEntityData)->Point0.x+((PENTARC)pEntityData)->Radius * cos(((PENTARC)pEntityData)->EndAngle*PI/180)),
			U2PY(((PENTARC)pEntityData)->Point0.y+((PENTARC)pEntityData)->Radius * sin(((PENTARC)pEntityData)->EndAngle*PI/180))
		);
		break;

	case ENT_SOLID:
		POINT	p[4];

		if(Rotation==0)
		{
			p[0].x = U2PX(((PENTSOLID)pEntityData)->Point0.x);
			p[0].y = U2PY(((PENTSOLID)pEntityData)->Point0.y);
			p[1].x = U2PX(((PENTSOLID)pEntityData)->Point1.x);
			p[1].y = U2PY(((PENTSOLID)pEntityData)->Point1.y);
			p[3].x = U2PX(((PENTSOLID)pEntityData)->Point2.x);
			p[3].y = U2PY(((PENTSOLID)pEntityData)->Point2.y);
			p[2].x = U2PX(((PENTSOLID)pEntityData)->Point3.x);
			p[2].y = U2PY(((PENTSOLID)pEntityData)->Point3.y);
		}
		else
		{
			p[0].x = U2PXROT(((PENTSOLID)pEntityData)->Point0.x, ((PENTSOLID)pEntityData)->Point0.y);
			p[0].y = U2PYROT(((PENTSOLID)pEntityData)->Point0.x, ((PENTSOLID)pEntityData)->Point0.y);
			p[1].x = U2PXROT(((PENTSOLID)pEntityData)->Point1.x, ((PENTSOLID)pEntityData)->Point1.y);
			p[1].y = U2PYROT(((PENTSOLID)pEntityData)->Point1.x, ((PENTSOLID)pEntityData)->Point1.y);
			p[3].x = U2PXROT(((PENTSOLID)pEntityData)->Point2.x, ((PENTSOLID)pEntityData)->Point2.y);
			p[3].y = U2PYROT(((PENTSOLID)pEntityData)->Point2.x, ((PENTSOLID)pEntityData)->Point2.y);
			p[2].x = U2PXROT(((PENTSOLID)pEntityData)->Point3.x, ((PENTSOLID)pEntityData)->Point3.y);
			p[2].y = U2PYROT(((PENTSOLID)pEntityData)->Point3.x, ((PENTSOLID)pEntityData)->Point3.y);
		}
		
		brush = CreateSolidBrush(rgbcolor);
		oldbrush = (HBRUSH)SelectObject(hdc, brush);
		Polygon(hdc, p, 4);
		SelectObject(hdc, oldbrush);
		DeleteObject(brush);
		break;

	case ENT_INSERT:
		BLOCKHEADER		BlockHeader;
		ENTITYHEADER	BlockEntityHeader;
		char			BlockEntityData[4096];
		BlockHeader.Objhandle = ((PENTINSERT)pEntityData)->BlockHeaderObjhandle;
		if(drwFindBlock_Direct(pDrawing, FIND_BYHANDLE, &BlockHeader)>0)
		{
//			LAYER	BlockLayer;
//			Layer.Objhandle = BlockHeader.LayerObjhandle;
//			drwFindTableType(hDrawing, TAB_LAYER, FIND_BYHANDLE, &Layer);

			if(drwFindEntity_Direct(pDrawing, BlockHeader.Name, &BlockEntityHeader, &BlockEntityData, FIND_FIRST)>0)
			{
				do{
					if(BlockEntityHeader.Color==0)
						BlockEntityHeader.Color = color;

					PlotEntity(
						pDrawing,
						hdc,
						&BlockEntityHeader,
						&BlockEntityData,
						((PENTINSERT)pEntityData)->Point0.x*xScale+x,
						((PENTINSERT)pEntityData)->Point0.y*yScale+y,
						((PENTINSERT)pEntityData)->XScale*xScale,
						((PENTINSERT)pEntityData)->YScale*yScale,
						((PENTINSERT)pEntityData)->RotationAngle+Rotation
					);
				} while(drwFindEntity_Direct(pDrawing, BlockHeader.Name, &BlockEntityHeader, &BlockEntityData, FIND_NEXT)>0);
			}
		}
		break;

	case ENT_POLYLINE:
		MoveToEx(
			hdc,
			U2PX(((PENTPOLYLINE)pEntityData)->pVertex[0].Point.x),
			U2PY(((PENTPOLYLINE)pEntityData)->pVertex[0].Point.y),
			NULL
		);
		for(i=1; i<((PENTPOLYLINE)pEntityData)->nVertex; i++)
		{
			if(int(((PENTPOLYLINE)pEntityData)->pVertex[i].Bulge*100)==0)
				LineTo(
					hdc,
					U2PX(((PENTPOLYLINE)pEntityData)->pVertex[i].Point.x),
					U2PY(((PENTPOLYLINE)pEntityData)->pVertex[i].Point.y)
				);
			else
			{ // Bulge is greater than zero
				double a1, a2, a3, a4;
				double x1, y1;
				double x2, y2;
				double x3, y3;
				double x4, y4;
				double xm, ym;
				double xc, yc, r;
				double bulge = ((PENTPOLYLINE)pEntityData)->pVertex[i].Bulge;

				x1 = ((PENTPOLYLINE)pEntityData)->pVertex[i].Point.x;
				x2 = ((PENTPOLYLINE)pEntityData)->pVertex[i-1].Point.x;
				y1 = ((PENTPOLYLINE)pEntityData)->pVertex[i].Point.y;
				y2 = ((PENTPOLYLINE)pEntityData)->pVertex[i-1].Point.y;
				a1 = (y1-y2) / (x1-x2);
				a2 = a1-bulge*2;
				a3 = -1/a2;
				a4 = -1/a1;
				
				xm = (x1+x2)/2;
				ym = (y1+y2)/2;

				xc = (ym-y1+a3*x1-a4*xm)/(a3-a4);
				yc = a3*(xc-x1)+y1;
				r = sqrt((x1-xc)*(x1-xc)+(y1-yc)*(y1-yc));

				x3 = xc-r;
				y3 = yc-r;
				x4 = xc+r;
				y4 = yc+r;
				
				Arc(
					hdc,
					U2PXROT(x3,y3),
					U2PYROT(x3,y3),
					U2PXROT(x4,y4),
					U2PYROT(x4,y4),
					U2PXROT(x1,y1),
					U2PYROT(x1,y1),
					U2PXROT(x2,y2),
					U2PYROT(x2,y2)
				);
/*				Ellipse(
				  hdc,
				  U2PXROT(x3,y3),
				  U2PYROT(x3,y3),
				  U2PXROT(x4,y4),
				  U2PYROT(x4,y4)
				);*/
			}
		}
		if(((PENTPOLYLINE)pEntityData)->Flag & POLYLINE_CLOSED)
			LineTo(
				hdc,
				U2PX(((PENTPOLYLINE)pEntityData)->pVertex[0].Point.x),
				U2PY(((PENTPOLYLINE)pEntityData)->pVertex[0].Point.y)
			);
		break;

	case ENT_DIMENSION:
		BLOCKHEADER		DimBlockHeader;
		ENTITYHEADER	DimBlockEntityHeader;
		char			DimBlockEntityData[4096];
		DimBlockHeader.Objhandle = ((PENTDIMENSION)pEntityData)->BlockheaderObjhandle;
		if(drwFindBlock_Direct(pDrawing, FIND_BYHANDLE, &DimBlockHeader)>0)
		{
//			LAYER	BlockLayer;
//			Layer.Objhandle = BlockHeader.LayerObjhandle;
//			drwFindTableType(hDrawing, TAB_LAYER, FIND_BYHANDLE, &Layer);

			if(drwFindEntity_Direct(pDrawing, DimBlockHeader.Name, &DimBlockEntityHeader, &DimBlockEntityData, FIND_FIRST)>0)
			{
				do{
					if(DimBlockEntityHeader.Color==0)
						DimBlockEntityHeader.Color = color;

					PlotEntity(
						pDrawing,
						hdc,
						&DimBlockEntityHeader,
						&DimBlockEntityData,
						0+x,
						0+y,
						1*xScale,
						1*yScale,
						0+Rotation
					);
				} while(drwFindEntity_Direct(pDrawing, DimBlockHeader.Name, &DimBlockEntityHeader, &DimBlockEntityData, FIND_NEXT)>0);
			}
		}
		break;
	}

	if(pEntityHeader->EntityType!=ENT_TEXT)
	{
		SelectObject(hdc, oldpen);
		DeleteObject(pen);
	}
}


/*-------------------------------------------------------------------*
 |  drwPlot                                                          |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      HDC hdc= handle of DC for drawing                            |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwPlot( HDRAWING hDrawing, HDC hdc )
{
	PDRAWING		pDrawing;
	ENTITYHEADER	EntityHeader;
	char			EntityData[4096];

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	if(pDrawing->View.Viewable)
	{
		// Entities Section ----------------------------------
		if(drwFindEntity_Direct(pDrawing, NULL, &EntityHeader, &EntityData, FIND_FIRST)>0)
		{
			do{
				PlotEntity(pDrawing, hdc, &EntityHeader, &EntityData);
			} while(drwFindEntity_Direct(pDrawing, NULL, &EntityHeader, &EntityData, FIND_NEXT)>0);
		}
	}

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}

/*-------------------------------------------------------------------*
 |  drwInitView                                                      |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      int x = horizontal position of drawing window                |
 |      int y = vertical position of drawing window                  |
 |      int Width = width of drawing window                          |
 |      int Height = height of drawing window                        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwInitView( HDRAWING hDrawing, int x, int y, int Width, int Height )
{
	PDRAWING		pDrawing;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return NULL;

	pDrawing->View.ZoomLevel = 1;
	pDrawing->View.PPU = 20;

	pDrawing->View.WindowLeft = x;
	pDrawing->View.WindowTop = y;
	pDrawing->View.WindowRight = x + Width;
	pDrawing->View.WindowBottom = y + Height;
	pDrawing->View.ViewLeft = 0;
	pDrawing->View.ViewBottom = 0;

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}

/*-------------------------------------------------------------------*
 |  drwPaint                                                         |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |	    HDC hdc	= handle to view window DC                           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwPaint( HDRAWING hDrawing, HDC hdc )
{
	PDRAWING	pDrawing;
	HRGN		ViewRgn;
	BOOL		result;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	Rectangle(
		hdc,
		pDrawing->View.WindowLeft,
		pDrawing->View.WindowTop,
		pDrawing->View.WindowRight,
		pDrawing->View.WindowBottom
	);

	ViewRgn = CreateRectRgn(
					pDrawing->View.WindowLeft,
					pDrawing->View.WindowTop,
					pDrawing->View.WindowRight,
					pDrawing->View.WindowBottom
				);
	SelectObject(hdc, ViewRgn);

	SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
	result = drwPlot(hDrawing, hdc);

	DeleteObject(ViewRgn);

	// UnInitilize pDrawing -----------------
	UnInitilizePDRAWING(hDrawing);

	return result;
}

/********************************************************************
 * Functions for changing view properties                           *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  drwGetViewProperties                                             |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      PVIEW pView = pointer to view properties structure           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwGetViewProperties( HDRAWING hDrawing, PVIEW pView )
{
	PDRAWING		pDrawing;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	memcpy(pView, &pDrawing->View, sizeof(VIEW));

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}

/*-------------------------------------------------------------------*
 |  drwSetViewProperties                                             |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      PVIEW pView = pointer to view properties structure           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwSetViewProperties( HDRAWING hDrawing, PVIEW pView )
{
	PDRAWING		pDrawing;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return FALSE;

	memcpy(&pDrawing->View, pView, sizeof(VIEW));

	pDrawing->View.ViewTop = pDrawing->View.ViewBottom + (pDrawing->View.WindowBottom-pDrawing->View.WindowTop)/pDrawing->View.ZoomLevel/pDrawing->View.PPU;
	pDrawing->View.ViewRight = pDrawing->View.ViewLeft + (pDrawing->View.WindowRight-pDrawing->View.WindowLeft)/pDrawing->View.ZoomLevel/pDrawing->View.PPU;

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}

/********************************************************************
 * Zoom Functions                                                   *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  drwGetDrawingBorder                                              |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |      PREALRECT pRect = pointer to drawing boundary rect           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwGetDrawingBorder( HDRAWING hDrawing, PREALRECT pRect )
{
	PDRAWING		pDrawing;
	ENTITYHEADER	EntityHeader;
	char			EntityData[4096];
	REALRECT		Rect;
	double			x1 = DBL_MAX;
	double			y1 = DBL_MIN;
	double			x2 = DBL_MIN;
	double			y2 = DBL_MAX;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return NULL;

//	if(pDrawing->View.Viewable)
	{
		// Entities Section ----------------------------------
		if(drwFindEntity_Direct(pDrawing, NULL, &EntityHeader, &EntityData, FIND_FIRST)>0)
		{
			do{
				if(GetEntityRect(pDrawing, &EntityHeader, EntityData, &Rect))
				{
					y1 = __max(Rect.top, y1);
					x1 = __min(Rect.left, x1);
					y2 = __min(Rect.bottom, y2);
					x2 = __max(Rect.right, x2);
				}
			} while(drwFindEntity_Direct(pDrawing, NULL, &EntityHeader, &EntityData, FIND_NEXT)>0);
		}
	}
	
	pRect->left = x1;
	pRect->right = x2;
	pRect->bottom = y2;
	pRect->top = y1;

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}

/*-------------------------------------------------------------------*
 |  drwZoomExtents                                                   |
 |  Inputs:                                                          |
 |      HDRAWING hDrawing = handle to the current DRAWING structure  |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL drwZoomExtents( HDRAWING hDrawing )
{
	PDRAWING		pDrawing;
	REALRECT		rect;
	double			ZoomLevelX, ZoomLevelY;

	// Initialize pDrawing ------------------
	if((pDrawing = InitilizePDRAWING(hDrawing))==NULL)
		return NULL;

	drwGetDrawingBorder(hDrawing, &rect);

	// Change Drawing View Properties
	ZoomLevelX = (pDrawing->View.WindowBottom-pDrawing->View.WindowTop-20)/((rect.top-rect.bottom)*pDrawing->View.PPU);
	ZoomLevelY = (pDrawing->View.WindowRight-pDrawing->View.WindowLeft-20)/((rect.right-rect.left)*pDrawing->View.PPU);
	pDrawing->View.ZoomLevel = (ZoomLevelX < ZoomLevelY ? ZoomLevelX : ZoomLevelY);

	pDrawing->View.ViewBottom = rect.bottom - ((pDrawing->View.WindowBottom-pDrawing->View.WindowTop)/(pDrawing->View.ZoomLevel*pDrawing->View.PPU) - (rect.top-rect.bottom))/2;
	pDrawing->View.ViewLeft = rect.left - ((pDrawing->View.WindowRight-pDrawing->View.WindowLeft)/(pDrawing->View.ZoomLevel*pDrawing->View.PPU) - (rect.right-rect.left))/2;

	pDrawing->View.ViewTop = pDrawing->View.ViewBottom + (pDrawing->View.WindowBottom-pDrawing->View.WindowTop)/pDrawing->View.ZoomLevel/pDrawing->View.PPU;
	pDrawing->View.ViewRight = pDrawing->View.ViewLeft + (pDrawing->View.WindowRight-pDrawing->View.WindowLeft)/pDrawing->View.ZoomLevel/pDrawing->View.PPU;

	// UnInitilize pDrawing -----------------
	return UnInitilizePDRAWING(hDrawing);
}
