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
|  DWEntities.cpp                                                     |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DWEntities.h"
#include "DWTables.h"


BOOL dimDrawArrowHead( HDXF hDxf, double cx, double cy, double Angle, double ArrowSize, LPCTSTR ArrowHeadName, int BLKNum );

/********************************************************************
 * ENTITIES Section                                                 *
 ********************************************************************/


//*************************************************************************
// This code is implemented by Tran duy Dung 2003/10/20             
/*----------------------------------------------------------------------*
 |  dxfPolyLine                                                         |
 |  Inputs:                                                             |
 |      HDXF hDxf = handle to the openning DXF file structure           |
 |      PDXFENTVERTEX pVertex = pointer of pline vertices               |
 |      int nVertex = number of vertices in Polyline                    |
 |      unsigned short Flag = POLYLINE_CLOSE if Polyline is close       |
 |                          = 0(zero) if Polyline is open               |
 |  Output: TRUE if everything is ok                                    |
 *----------------------------------------------------------------------*/
BOOL dxfPolyLine( HDXF hDxf, PDXFENTVERTEX pVertex, int nVertex, unsigned short Flag )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;
	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "POLYLINE");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);
	// Polyline flag (bit-coded)	
	result &= dxfWriteParamInteger(hDxf, 70, Flag);
	
	// Vertices are about to follow
	result &= dxfWriteParamString(hDxf, 66, " 1");
	// "dummy" point; the X and Y values are always 0,
	// and the Z value is the polyline's elevation (in OCS when 2D, WCS when 3D)
	result &= dxfWriteParamString(hDxf, 10, "0.0");
	result &= dxfWriteParamString(hDxf, 20, "0.0");
//	result &= dxfWriteParamString(hDxf, 30, "0.0");

	// VERTEX
	for(int ipoint=0; ipoint<nVertex; ipoint++){
		result &= dxfWriteParamString(hDxf, 0, "VERTEX");
		// Layer Name
		result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
		// x
		result &= dxfWriteParamDouble(hDxf, 10, pVertex[ipoint].Point.x);
		// y
		result &= dxfWriteParamDouble(hDxf, 20, pVertex[ipoint].Point.y);
		// z
//		result &= dxfWriteParamDouble(hDxf, 30, 0);
	}	

	//End Polyline
	result &= dxfWriteParamString(hDxf, 0, "SEQEND");
	result &= dxfWriteParamString(hDxf, 8, "0");

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfLine                                                          |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      double x1 = x position of start point                        |
 |      double y1 = y position of start point                        |
 |      double x2 = x position of end point                          |
 |      double y2 = y position of end point                          |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfLine( HDXF hDxf, double x1, double y1, double x2, double y2 )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "LINE");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);
	// x1
	result &= dxfWriteParamDouble(hDxf, 10, x1);
	// y1
	result &= dxfWriteParamDouble(hDxf, 20, y1);
	// x2
	result &= dxfWriteParamDouble(hDxf, 11, x2);
	// y2
	result &= dxfWriteParamDouble(hDxf, 21, y2);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfCircle                                                        |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      double cx = x position of center point                       |
 |      double cy = y position of center point                       |
 |      double r = radius                                            |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfCircle( HDXF hDxf, double cx, double cy, double r )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "CIRCLE");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);
	// cx
	result &= dxfWriteParamDouble(hDxf, 10, cx);
	// cy
	result &= dxfWriteParamDouble(hDxf, 20, cy);
	// radius
	result &= dxfWriteParamDouble(hDxf, 40, r);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfEllipse                                                       |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      double cx = x position of center point                       |
 |      double cy = y position of center point                       |
 |      double ex = x position of endpoint of major axis             |
 |      double ey = y position of endpoint of major axis             |
 |      double Ratio = Ratio of minor axis to major axis             |
 |      double StartParam = Start parameter                          |
 |      double EndParam = End parameter                              |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfEllipse( HDXF hDxf, double cx, double cy, double ex, double ey, double Ratio, double StartParam, double EndParam )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "ELLIPSE");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);

	// cx
	result &= dxfWriteParamDouble(hDxf, 10, cx);
	// cy
	result &= dxfWriteParamDouble(hDxf, 20, cy);
	// x of endpoint of major axis
	result &= dxfWriteParamDouble(hDxf, 11, ex);
	// y of endpoint of major axis
	result &= dxfWriteParamDouble(hDxf, 21, ey);
	// Ratio of minor axis to major axis
	result &= dxfWriteParamDouble(hDxf, 40, Ratio);
	// Start parameter (this value is 0.0 for a full ellipse)
	result &= dxfWriteParamDouble(hDxf, 41, StartParam);
	// End parameter (this value is 2pi for a full ellipse)
	result &= dxfWriteParamDouble(hDxf, 42, EndParam);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfSolid                                                         |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      REALPOINT& Point0 = point 0                                  |
 |      REALPOINT& Point1 = point 1                                  |
 |      REALPOINT& Point2 = point 2                                  |
 |      REALPOINT& Point3 = point 3                                  |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfSolid( HDXF hDxf, REALPOINT &Point0, REALPOINT &Point1, REALPOINT &Point2, REALPOINT &Point3 )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "SOLID");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);

	// x & y & z
	// 0
	result &= dxfWriteParamDouble(hDxf, 10, Point0.x);
	result &= dxfWriteParamDouble(hDxf, 20, Point0.y);
	result &= dxfWriteParamDouble(hDxf, 30, 0);
	// 1
	result &= dxfWriteParamDouble(hDxf, 11, Point1.x);
	result &= dxfWriteParamDouble(hDxf, 21, Point1.y);
	result &= dxfWriteParamDouble(hDxf, 31, 0);
	// 2
	result &= dxfWriteParamDouble(hDxf, 12, Point2.x);
	result &= dxfWriteParamDouble(hDxf, 22, Point2.y);
	result &= dxfWriteParamDouble(hDxf, 32, 0);
	// 3
	result &= dxfWriteParamDouble(hDxf, 13, Point3.x);
	result &= dxfWriteParamDouble(hDxf, 23, Point3.y);
	result &= dxfWriteParamDouble(hDxf, 33, 0);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfText                                                          |
 |  Inputs:                                                          |
 |    HDXF hDxf = handle to the openning DXF file structure          |
 |    LPCTSTR Text = text to be shown                                |
 |    double x1 = x of first alignment point                         |
 |    double y1 = y of first alignment point                         |
 |    double x2 = x of second alignment point (by default must be x1)|
 |    double y2 = y of second alignment point (by default must be y1)|
 |    double Height = height of text                                 |
 |    double Rotation = rotation angle of text                       |
 |    DWORD Justification = horizontal & vertical text justification |
 |    double WidthFactor = Relative X scale factor--width (def = 1.0)|
 |    LPCTSTR StyleName = text style name, NULL = current style      |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfText( HDXF hDxf, LPCTSTR Text, double x1, double y1, double x2, double y2, double Height, double Rotation, DWORD Justification, double WidthFactor, LPCTSTR StyleName )
{
	PDXF	pDxf;
	BOOL	result;

	if(Text==NULL)
	{
		// Text should not be null
		return FALSE;
	}

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "TEXT");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);
	// Text Style Name
	if( StyleName )
		result &= dxfWriteParamString(hDxf, 7, StyleName);
	else if( strlen(pDxf->Write.CurrentTextStyle)>0 )
		result &= dxfWriteParamString(hDxf, 7, pDxf->Write.CurrentTextStyle);

	// First alignment point (in OCS)
	result &= dxfWriteParamDouble(hDxf, 10, x1);
	result &= dxfWriteParamDouble(hDxf, 20, y1);

	// Text justification type
	if(Justification!=TJ_LEFT)
	{
		result &= dxfWriteParamInteger(hDxf, 72, (Justification & 0xff));
		result &= dxfWriteParamInteger(hDxf, 73, (Justification >> 8) & 0xff);
		// Second alignment point (in OCS)
		result &= dxfWriteParamDouble(hDxf, 11, x2);
		result &= dxfWriteParamDouble(hDxf, 21, y2);
	}

	// Text Height
	result &= dxfWriteParamDouble(hDxf, 40, Height);
	// Width Factor
	if(WidthFactor!=0)
		result &= dxfWriteParamDouble(hDxf, 41, WidthFactor);
	// Text Rotation Angle
	result &= dxfWriteParamDouble(hDxf, 50, Rotation);
	// The text itself
	result &= dxfWriteParamString(hDxf, 1, Text);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfPoint                                                         |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      double x = x position of point                               |
 |      double y = y position of point                               |
 |      double thickness = Thickness (optional; default = 0)         |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfPoint( HDXF hDxf, double x, double y, double thickness )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "POINT");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);
	// x
	result &= dxfWriteParamDouble(hDxf, 10, x);
	// y
	result &= dxfWriteParamDouble(hDxf, 20, y);
	// Thickness
	if(thickness!=0)
		result &= dxfWriteParamDouble(hDxf, 39, thickness);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfDimLinear                                                     |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      double x1 = x position of start point                        |
 |      double y1 = y position of start point                        |
 |      double x2 = x position of end point                          |
 |      double y2 = y position of end point                          |
 |      double x3 = x position of dimension line                     |
 |      double y3 = y position of dimension line                     |
 |      double angle = dim-line's rotation angle                     |
 |      LPCTSTR text = overwrite text (dafault is NULL)              |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfDimLinear( HDXF hDxf, double x1, double y1, double x2, double y2, double x3, double y3, double angle, LPCTSTR text )
{
	PDXF			pDxf;
	DXF				oldDxf;
	BOOL			result;
	PDXFDIMSTYLE	pDimStyle;
	double			TextWidth;
	double			a, a1, a2, a3;
	double			sina1, cosa1;
	double			sina2, cosa2;
	REALPOINT		A1, A2, A3;
	REALPOINT		B1, B2, B3;
	REALPOINT		C1, C2, C3;
	double			pi = 3.1415926535;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Search for dimstyle data
	pDimStyle = NULL;
	for(UINT i=0; i<pDxf->Write.DimStylesNumber; i++)
	{
		PDXFDIMSTYLE pFixDimStyle;
		pFixDimStyle = (PDXFDIMSTYLE)((DWORD)pDxf + sizeof(DXF) + sizeof(DXFDIMSTYLE)*i);
		if(strcmp(pFixDimStyle->Name, pDxf->Write.CurrentDimStyle)==0)
		{
			pDimStyle = pFixDimStyle;
			break;
		}
	}

	if(pDimStyle==NULL)
	{
		// No current dim style is available
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Save dxf's old config data
	memcpy(&oldDxf, pDxf, sizeof(DXF));

	// Writting dimension data to the dxf file ---------------------
	result = TRUE;

	a = tan( angle*pi/180 );
	if(a!=0)
	{
		A1.x = (y1 - y3 + x1/a + a*x3) / (a + 1/a);
		A1.y = a * (A1.x - x3) + y3;
		B1.x = (y2 - y3 + x2/a + a*x3) / (a + 1/a);
		B1.y = a * (B1.x - x3) + y3;

		a1 = (A1.y - y1) / (A1.x - x1);
		sina1 = (A1.x>x1? 1 : -1) * sin( atan(a1) );
		cosa1 = (A1.x>x1? 1 : -1) * cos( atan(a1) );
		A2.x = A1.x + cosa1*pDimStyle->DIMEXE;
		A2.y = A1.y + sina1*pDimStyle->DIMEXE;
		A3.x = x1 + cosa1*pDimStyle->DIMEXO;
		A3.y = y1 + sina1*pDimStyle->DIMEXO;

		a2 = (B1.y - y2) / (B1.x - x2);
		sina2 = (B1.x>x2? 1 : -1) * sin( atan(a2) );
		cosa2 = (B1.x>x2? 1 : -1) * cos( atan(a2) );
		B2.x = B1.x + cosa2*pDimStyle->DIMEXE;
		B2.y = B1.y + sina2*pDimStyle->DIMEXE;
		B3.x = x2 + cosa2*pDimStyle->DIMEXO;
		B3.y = y2 + sina2*pDimStyle->DIMEXO;
	}
	else
	{
		A1.x = x1;
		A1.y = y3;
		B1.x = x2;
		B1.y = y3;

		A2.x = A1.x;
		A2.y = A1.y + _copysign(pDimStyle->DIMEXE, A1.y-y1);
		A3.x = x1;
		A3.y = y1 + _copysign(pDimStyle->DIMEXO, A1.y-y1);

		B2.x = B1.x;
		B2.y = B1.y + _copysign(pDimStyle->DIMEXE, B1.y-y2);
		B3.x = x2;
		B3.y = y2 + _copysign(pDimStyle->DIMEXO, B1.y-y2);
	}

	C1.x = (A1.x + B1.x) / 2;
	C1.y = (A1.y + B1.y) / 2;

	// Begin & End Points ----
	dxfPoint(hDxf, x1, y1, 0);
	dxfPoint(hDxf, x2, y2, 0);

	// Dimension Line ----
	pDxf->Write.CurrentColor = pDimStyle->DIMCLRD;

	switch(pDimStyle->DIMTAD)
	{
	case 0: // Centered
		// At this time the program cannot calculate the exact width of the text
		TextWidth = (strlen(text) * pDimStyle->DIMTXT * 0.3) + pDimStyle->DIMGAP*2;
		C2.x = C1.x + _copysign( cos(angle*pi/180), A1.x-B1.x)*TextWidth;
		C2.y = C1.y + _copysign( sin(angle*pi/180), A1.x-B1.x)*TextWidth;
		C3.x = C1.x + _copysign( cos(angle*pi/180), B1.x-A1.x)*TextWidth;
		C3.y = C1.y + _copysign( sin(angle*pi/180), B1.x-A1.x)*TextWidth;

		result &= dxfLine(hDxf, A1.x, A1.y, C2.x, C2.y);
		result &= dxfLine(hDxf, C3.x, C3.y, B1.x, B1.y);

		// Text
		pDxf->Write.CurrentColor = pDimStyle->DIMCLRT;
		result &= dxfText(hDxf, text, C1.x, C1.y, C1.x, C1.y, pDimStyle->DIMTXT, angle, TJ_MIDDLE, 0, pDimStyle->DIMTXSTY);
		break;
	case 1: // Above
		result &= dxfLine(hDxf, A1.x, A1.y, B1.x, B1.y);
		// Text
		pDxf->Write.CurrentColor = pDimStyle->DIMCLRT;
		REALPOINT C11;
		C11.x = C1.x + cos((angle+90)*pi/180)*(pDimStyle->DIMGAP+pDimStyle->DIMTXT/2);
		C11.y = C1.y + sin((angle+90)*pi/180)*(pDimStyle->DIMGAP+pDimStyle->DIMTXT/2);
		result &= dxfText(hDxf, text, C11.x, C11.y, C11.x, C11.y, pDimStyle->DIMTXT, angle, TJ_MIDDLE, 0, pDimStyle->DIMTXSTY);
		break;
	}

	// Arrow Heads ----
	pDxf->Write.CurrentColor = pDimStyle->DIMCLRD;

	a3 = (B1.y - A1.y) / (B1.x - A1.x);

	if(B1.x>A1.x)
	{
		// 1st Arrow Head
		result &= dimDrawArrowHead( hDxf, A1.x, A1.y, pi+atan(a3), pDimStyle->DIMASZ, pDimStyle->DIMBLK1, 1 );
		// 2nd Arrow Head
		result &= dimDrawArrowHead( hDxf, B1.x, B1.y, atan(a3), pDimStyle->DIMASZ, pDimStyle->DIMBLK2, 2 );
	}
	else if(B1.x<A1.x)
	{
		// 1st Arrow Head
		result &= dimDrawArrowHead( hDxf, A1.x, A1.y, atan(a3), pDimStyle->DIMASZ, pDimStyle->DIMBLK1, 1 );
		// 2nd Arrow Head
		result &= dimDrawArrowHead( hDxf, B1.x, B1.y, pi+atan(a3), pDimStyle->DIMASZ, pDimStyle->DIMBLK2, 2 );
	}
	else // (B1.x==A1.x)
	{
		// 1st Arrow Head
		result &= dimDrawArrowHead( hDxf, A1.x, A1.y, (B1.y>A1.y?0:1)*pi+atan(a3), pDimStyle->DIMASZ, pDimStyle->DIMBLK1, 1 );
		// 2nd Arrow Head
		result &= dimDrawArrowHead( hDxf, B1.x, B1.y, (B1.y>A1.y?1:0)*pi+atan(a3), pDimStyle->DIMASZ, pDimStyle->DIMBLK2, 2 );
	}

	// Extension Lines ----
	pDxf->Write.CurrentColor = pDimStyle->DIMCLRE;
	// 1st Extension Line
	dxfLine(hDxf, A2.x, A2.y, A3.x, A3.y);
	// 2nd Extension Line
	dxfLine(hDxf, B2.x, B2.y, B3.x, B3.y);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// Restore dxf's old config data
	memcpy(pDxf, &oldDxf, sizeof(DXF));

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dimDrawArrowHead                                                 |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      double cx = x position of center point of arrowhead          |
 |      double cy = y position of center point of arrowhead          |
 |      double Angle = angle of arrowhead                            |
 |      double ArrowSize = arrow size                                |
 |      LPCTSTR ArrowHeadName = name of arrow head to draw           |
 |      int BLKNum = number of arrow head (can be 1 or 2)            |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dimDrawArrowHead( HDXF hDxf, double cx, double cy, double Angle, double ArrowSize, LPCTSTR ArrowHeadName, int BLKNum )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	pDxf = InitilizePDXF(hDxf);

	// Write arrow head data to the dxf file
	result = TRUE;

	for(int i=0; i<ArrowHeadsNum; i++)
	{
		if((strcmp(ArrowHeadName, ArrowHeads[i].ArrowHeadName)==0)
			&& (BLKNum==ArrowHeads[i].BLKNum))
		{
			if(strcmp(ArrowHeads[i].Command, "LINE")==0)
			{
				result &= dxfLine(hDxf,
					(cx + ((cos(Angle)*ArrowHeads[i].Param1-sin(Angle)*ArrowHeads[i].Param2)*ArrowSize)),
					(cy + ((sin(Angle)*ArrowHeads[i].Param1+cos(Angle)*ArrowHeads[i].Param2)*ArrowSize)),
					(cx + ((cos(Angle)*ArrowHeads[i].Param3-sin(Angle)*ArrowHeads[i].Param4)*ArrowSize)),
					(cy + ((sin(Angle)*ArrowHeads[i].Param3+cos(Angle)*ArrowHeads[i].Param4)*ArrowSize)));
			}
			else if(strcmp(ArrowHeads[i].Command, "SOLID")==0)
			{
				REALPOINT points[4];
				points[0].x = cx + ((cos(Angle)*ArrowHeads[i].Param1-sin(Angle)*ArrowHeads[i].Param2)*ArrowSize);
				points[0].y = cy + ((sin(Angle)*ArrowHeads[i].Param1+cos(Angle)*ArrowHeads[i].Param2)*ArrowSize);
				points[1].x = cx + ((cos(Angle)*ArrowHeads[i].Param3-sin(Angle)*ArrowHeads[i].Param4)*ArrowSize);
				points[1].y = cy + ((sin(Angle)*ArrowHeads[i].Param3+cos(Angle)*ArrowHeads[i].Param4)*ArrowSize);
				points[2].x = cx + ((cos(Angle)*ArrowHeads[i].Param5-sin(Angle)*ArrowHeads[i].Param6)*ArrowSize);
				points[2].y = cy + ((sin(Angle)*ArrowHeads[i].Param5+cos(Angle)*ArrowHeads[i].Param6)*ArrowSize);
				points[3].x = cx + ((cos(Angle)*ArrowHeads[i].Param7-sin(Angle)*ArrowHeads[i].Param8)*ArrowSize);
				points[3].y = cy + ((sin(Angle)*ArrowHeads[i].Param7+cos(Angle)*ArrowHeads[i].Param8)*ArrowSize);
				result &= dxfSolid(hDxf, points[0], points[1], points[2], points[3]);
			}
		}
	}

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfArc                                                           |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      double cx = x position of center point                       |
 |      double cy = y position of center point                       |
 |      double r = radius of arc                                     |
 |      double StartAngle = start angle of arc                       |
 |      double EndAngle = end angle of arc                           |
 |      double Thickness = Thickness (optional; default = 0)         |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfArc(
	HDXF	hDxf,		// handle to the openning DXF file structure
	double	cx,			// x position of center point
	double	cy,			// y position of center point
	double	r,			// radius of arc
	double	StartAngle,	// start angle of arc
	double	EndAngle,	// end angle of arc
	double	Thickness	// Thickness (optional; default = 0)
	)
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "ARC");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);
	// Thickness
	if(Thickness>0)
		result &= dxfWriteParamDouble(hDxf, 39, Thickness);

	// cx
	result &= dxfWriteParamDouble(hDxf, 10, cx);
	// cy
	result &= dxfWriteParamDouble(hDxf, 20, cy);
	// radius
	result &= dxfWriteParamDouble(hDxf, 40, r);
	// start angle
	result &= dxfWriteParamDouble(hDxf, 50, StartAngle);
	// end angle
	result &= dxfWriteParamDouble(hDxf, 51, EndAngle);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}


/*-------------------------------------------------------------------*
 |  dxfInsertBlock                                                   |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      LPCTSTR BlockName = block name to be inserted                |
 |      double x = x position of insertion point                     |
 |      double y = y position of insertion point                     |
 |      double xScale = x scale factor (optional; default = 1)       |
 |      double yScale = y scale factor (optional; default = 1)       |
 |      double Rotation = rotation angle (optional; default = 0)     |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfInsertBlock( HDXF hDxf, LPCTSTR BlockName, double x, double y, double xScale, double yScale, double Rotation )
{
	PDXF	pDxf;
	BOOL	result;

	if(BlockName==NULL)
	{
		// BlockName should not be null
		return FALSE;
	}

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is ENTITIES or a new Block
	if(!(pDxf->Write.CurrentSection & SEC_ENTITIES))
	{
		// Current section is not ENTITIES or not a new Block begin
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;

	// Entity type
	result &= dxfWriteParamString(hDxf, 0, "INSERT");
	// Layer Name
	result &= dxfWriteParamString(hDxf, 8, pDxf->Write.CurrentLayer);
	// Line Color
	if((pDxf->Write.CurrentColor>0) && (pDxf->Write.CurrentColor<256))
		result &= dxfWriteParamInteger(hDxf, 62, pDxf->Write.CurrentColor);
	// Line Type
	if(strlen(pDxf->Write.CurrentLine)>0)
		result &= dxfWriteParamString(hDxf, 6, pDxf->Write.CurrentLine);

	// Block Name
	result &= dxfWriteParamString(hDxf, 2, BlockName);
	// Insertion point
	result &= dxfWriteParamDouble(hDxf, 10, x);
	result &= dxfWriteParamDouble(hDxf, 20, y);
	// Scale factors
	result &= dxfWriteParamDouble(hDxf, 41, xScale);
	result &= dxfWriteParamDouble(hDxf, 42, yScale);
	// Rotation angle
	result &= dxfWriteParamDouble(hDxf, 50, Rotation);

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}
