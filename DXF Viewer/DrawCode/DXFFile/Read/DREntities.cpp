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
|  DREntities.cpp                                                     |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DREntities.h"
#include "..\Base\SimpleList.h"


int		GCode;
char	strValue[2048];

/*-------------------------------------------------------------------*
 |  ReadLineData                                                     |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTLINE pLine = pointer to Line structure                |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadLineData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTLINE pLine )
{
	ZeroMemory(pLine, sizeof(DXFENTLINE));

	pEntityHeader->EntityType = ENT_LINE;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 10:	// Start point X
			pLine->Point0.x = atof(strValue);
			break;
		case 20:	// Start point Y
			pLine->Point0.y = atof(strValue);
			break;
		case 30:	// Start point Z
			pLine->Point0.z = atof(strValue);
			break;
		case 11:	// End point X
			pLine->Point1.x = atof(strValue);
			break;
		case 21:	// End point Y
			pLine->Point1.y = atof(strValue);
			break;
		case 31:	// End point Z
			pLine->Point1.z = atof(strValue);
			break;
 		}

		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}

	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadPointData                                                    |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTPOINT pPoint = pointer to Point structure             |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadPointData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTPOINT pPoint )
{
	ZeroMemory(pPoint, sizeof(DXFENTPOINT));

	pEntityHeader->EntityType = ENT_POINT;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 10:	// Point location X
			pPoint->Point0.x = atof(strValue);
			break;
		case 20:	// Point location Y
			pPoint->Point0.y = atof(strValue);
			break;
		case 30:	// Point location Z
			pPoint->Point0.z = atof(strValue);
			break;
 		}

		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadCircleData                                                   |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTCIRCLE pPoint = pointer to Circle structure           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadCircleData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTCIRCLE pCircle )
{
	ZeroMemory(pCircle, sizeof(DXFENTCIRCLE));

	pEntityHeader->EntityType = ENT_CIRCLE;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 10:	// Center point X
			pCircle->Point0.x = atof(strValue);
			break;
		case 20:	// Center point Y
			pCircle->Point0.y = atof(strValue);
			break;
		case 30:	// Center point Z
			pCircle->Point0.z = atof(strValue);
			break;
		case 40:	// Radius
			pCircle->Radius = atof(strValue);
			break;
 		}
	
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadEllipseData                                                  |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTCIRCLE pPoint = pointer to Circle structure           |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadEllipseData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTELLIPSE pEllipse )
{
	ZeroMemory(pEllipse, sizeof(DXFENTELLIPSE));

	pEntityHeader->EntityType = ENT_ELLIPSE;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 10:	// Center point X
			pEllipse->CenterPoint.x = atof(strValue);
			break;
		case 20:	// Center point Y
			pEllipse->CenterPoint.y = atof(strValue);
			break;
		case 30:	// Center point Z
			pEllipse->CenterPoint.z = atof(strValue);
			break;
		case 11:	// Endpoint of major axis X
			pEllipse->MajorAxisEndPoint.x = atof(strValue);
			break;
		case 21:	// Endpoint of major axis Y
			pEllipse->MajorAxisEndPoint.y = atof(strValue);
			break;
		case 31:	// Endpoint of major axis Z
			pEllipse->MajorAxisEndPoint.z = atof(strValue);
			break;
		case 40:	// Ratio of minor axis to major axis
			pEllipse->MinorToMajorRatio = atof(strValue);
			break;
		case 41:	// Start parameter (this value is 0.0 for a full ellipse)
			pEllipse->StartParam = atof(strValue);
			break;
		case 42:	// End parameter (this value is 2pi for a full ellipse)
			pEllipse->EndParam = atof(strValue);
			break;
 		}
	
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadTextData                                                     |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTTEXT pText = pointer to Text structure                |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadTextData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTTEXT pText )
{
	ZeroMemory(pText, sizeof(DXFENTTEXT));

	pEntityHeader->EntityType = ENT_TEXT;
	pText->TextData.Justification = 0;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 7:		// Text Style Name
			strcpy(pText->TextData.TextStyleName, strValue);
			break;
		case 10:	// First alignment point (in OCS) X
			pText->Point0.x = atof(strValue);
			break;
		case 20:	// First alignment point (in OCS) Y
			pText->Point0.y = atof(strValue);
			break;
		case 30:	// First alignment point (in OCS) Z
			pText->Point0.z = atof(strValue);
			break;
		case 72:	// Horizontal text justification type (default = 0)
			pText->TextData.Justification = (pText->TextData.Justification & 0xff00) | (char)atoi(strValue);
			break;
		case 73:	// Vertical text justification type (default = 0)
			pText->TextData.Justification = (pText->TextData.Justification & 0x00ff) | (char)atoi(strValue) << 8;
			break;
		case 11:	// Second alignment point (in OCS) X
			pText->TextData.SecondAlignmentPoint.x = atof(strValue);
			break;
		case 21:	// Second alignment point (in OCS) Y
			pText->TextData.SecondAlignmentPoint.y = atof(strValue);
			break;
		case 31:	// Second alignment point (in OCS) Z
			pText->TextData.SecondAlignmentPoint.z = atof(strValue);
			break;
		case 40:	// Text Height
			pText->TextData.Height = atof(strValue);
			break;
		case 41:	// Width Factor
			pText->TextData.WidthFactor = atof(strValue);
			break;
		case 50:	// Text Rotation Angle
			pText->TextData.RotationAngle = atof(strValue);
			break;
		case 1:		// Default value (the string itself)
			strcpy(pText->strText, strValue);
			break;
 		}

		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadArcData                                                      |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTARC pArc = pointer to Arc structure                   |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadArcData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTARC pArc )
{
	ZeroMemory(pArc, sizeof(DXFENTARC));

	pEntityHeader->EntityType = ENT_ARC;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 10:	// Center point X
			pArc->Point0.x = atof(strValue);
			break;
		case 20:	// Center point Y
			pArc->Point0.y = atof(strValue);
			break;
		case 30:	// Center point Z
			pArc->Point0.z = atof(strValue);
			break;
		case 40:	// Radius
			pArc->Radius = atof(strValue);
			break;
		case 50:	// Start angle
			pArc->StartAngle = atof(strValue);
			break;
		case 51:	// End angle
			pArc->EndAngle = atof(strValue);
			break;
 		}

		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadSolidData                                                    |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTSOLID pSolid = pointer to Solid structure             |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadSolidData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTSOLID pSolid )
{
	ZeroMemory(pSolid, sizeof(DXFENTSOLID));

	pEntityHeader->EntityType = ENT_SOLID;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 10:	// First corner X
			pSolid->Point0.x = atof(strValue);
			break;
		case 20:	// First corner Y
			pSolid->Point0.y = atof(strValue);
			break;
		case 30:	// First corner Z
			pSolid->Point0.z = atof(strValue);
			break;
		case 11:	// Second corner X
			pSolid->Point1.x = atof(strValue);
			break;
		case 21:	// Second corner Y
			pSolid->Point1.y = atof(strValue);
			break;
		case 31:	// Second corner Z
			pSolid->Point1.z = atof(strValue);
			break;
		case 12:	// Third corner X
			pSolid->Point2.x = atof(strValue);
			break;
		case 22:	// Third corner Y
			pSolid->Point2.y = atof(strValue);
			break;
		case 32:	// Third corner Z
			pSolid->Point2.z = atof(strValue);
			break;
		case 13:	// Fourth corner X
			pSolid->Point3.x = atof(strValue);
			break;
		case 23:	// Fourth corner Y
			pSolid->Point3.y = atof(strValue);
			break;
		case 33:	// Fourth corner Z
			pSolid->Point3.z = atof(strValue);
			break;
 		}

		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadInsertData                                                   |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTINSERT pInsert = pointer to Insert structure          |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadInsertData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTINSERT pInsert )
{
	ZeroMemory(pInsert, sizeof(DXFENTINSERT));

	pEntityHeader->EntityType = ENT_INSERT;
	pInsert->XScale = 1;
	pInsert->YScale = 1;
	pInsert->ZScale = 1;
	pInsert->RotationAngle = 0;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 2:		// Block Name
			strcpy(pInsert->BlockHeaderName, strValue);
			break;
		case 10:	// Insertion point X
			pInsert->Point0.x = atof(strValue);
			break;
		case 20:	// Insertion point Y
			pInsert->Point0.y = atof(strValue);
			break;
		case 30:	// Insertion point Z
			pInsert->Point0.z = atof(strValue);
			break;
		case 41:	// X scale factor (optional; default = 1)
			pInsert->XScale = atof(strValue);
			break;
		case 42:	// Y scale factor (optional; default = 1)
			pInsert->YScale = atof(strValue);
			break;
		case 43:	// Z scale factor (optional; default = 1)
			pInsert->ZScale = atof(strValue);
			break;
		case 50:	// Rotation angle (optional; default = 0)
			pInsert->RotationAngle = atof(strValue);
			break;
 		}

		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	return TRUE;
}

//*************************************************************************
// This code is implemented by Tran duy Dung 2003/10/21
/*-------------------------------------------------------------------*
 |  ReadPolyLineData                                                 |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTLINE pPolyLine = pointer to PolyLine structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadPolyLineData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTPOLYLINE pPolyLine )
{
	ZeroMemory(pPolyLine, sizeof(DXFENTPOLYLINE));

	pEntityHeader->EntityType = ENT_POLYLINE;
	pPolyLine->Flag = 0;

	ReadParamFromDxfFile(pDxf, GCode, strValue);	
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;	
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 70:	// Polyline flag (bit-coded); default is 0
			pPolyLine->Flag = atoi(strValue);
			break;	
 		}
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}

	// Reading Vertex Data
	SimpleList<DXFENTVERTEX> vertices;
	DXFENTVERTEX Vertex;
	while((GCode==0) && (strcmp(strValue,"VERTEX")==0))
	{
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);	
		while(GCode!=0){
			switch(GCode)
			{
				case 10:	// Start point X
					Vertex.Point.x = atof(strValue);
					break;		
				case 20:	// Start point X
					Vertex.Point.y = atof(strValue);
					break;		
				case 30:	// Start point X
					Vertex.Point.z = atof(strValue);
					break;		
				case 42:	// Bulge (optional; default is 0)
					Vertex.Bulge = atof(strValue);
					break;		
 			}
			dxfStorePos(pDxf);
			ReadParamFromDxfFile(pDxf, GCode, strValue);
		}
		vertices.add(Vertex);		
	}

	pPolyLine->nVertex = vertices.getSize();
	pPolyLine->pVertex = new DXFENTVERTEX[pPolyLine->nVertex];
	int i=0;
	vertices.start();
	while (vertices.next()) {
		pPolyLine->pVertex[i] = vertices.get();		
		i++;
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadDimensionData                                                |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      PDXFENTDIMENSION pDimension = pointer to Dimension structure |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadDimensionData( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, PDXFENTDIMENSION pDimension )
{
	ZeroMemory(pDimension, sizeof(DXFENTDIMENSION));

	pEntityHeader->EntityType = ENT_DIMENSION;

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 8:		// Layer Name
			strcpy(pEntityHeader->LayerName, strValue);
			break;
		case 62:	// Color
			pEntityHeader->Color = atoi(strValue);
			break;
		case 6:		// Line Type
			strcpy(pEntityHeader->LTypeName, strValue);
			break;
		case 39:	// Thickness
			pEntityHeader->Thickness = atof(strValue);
			break;	
		case 48:	// Linetype scale
			pEntityHeader->LineTypeScale = atof(strValue);
			break;
		case 10:	// Definition point (in WCS) X
			pDimension->DimLineDefPoint.x = atof(strValue);
			break;
		case 20:	// Definition point (in WCS) Y
			pDimension->DimLineDefPoint.y = atof(strValue);
			break;
		case 30:	// Definition point (in WCS) Z
			pDimension->DimLineDefPoint.z = atof(strValue);
			break;
		case 13:	// Definition point for linear and angular dimensions (in WCS) X
			pDimension->DefPoint3.x = atof(strValue);
			break;
		case 23:	// Definition point for linear and angular dimensions (in WCS) Y
			pDimension->DefPoint3.y = atof(strValue);
			break;
		case 33:	// Definition point for linear and angular dimensions (in WCS) Z
			pDimension->DefPoint3.z = atof(strValue);
			break;
		case 14:	// Definition point for linear and angular dimensions (in WCS) X
			pDimension->DefPoint4.x = atof(strValue);
			break;
		case 24:	// Definition point for linear and angular dimensions (in WCS) Y
			pDimension->DefPoint4.y = atof(strValue);
			break;
		case 34:	// Definition point for linear and angular dimensions (in WCS) Z
			pDimension->DefPoint4.z = atof(strValue);
			break;
		case 53:	// The rotation angle of the dimension text away from its default orientation (the direction of the dimension line)
			pDimension->DimRotationAngle = atof(strValue);
			break;
		case 3:		// Dimension style name
			strcpy(pDimension->DimStyleName, strValue);
			break;
		case 1:		// Dimension text explicitly entered by the user. Optional; default is the measurement. If null or "<>", the dimension measurement is drawn as the text, if " " (one blank space), the text is suppressed.
			strcpy(pDimension->DimText, strValue);
			break;
		case 2:		// Name of the block that contains the entities that make up the dimension picture
			strcpy(pDimension->BlockName, strValue);
			break;
 		}

		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  SetEntityHeaderDefaultValues                                     |
 |  Inputs:                                                          |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |  Output: Nothing!                                                 |
 *-------------------------------------------------------------------*/
void SetEntityHeaderDefaultValues( PDXFENTITYHEADER pEntityHeader )
{
	ZeroMemory(pEntityHeader, sizeof(DXFENTITYHEADER));
	pEntityHeader->Color = 256;					// Color (Default=BYLAYER)
	pEntityHeader->Thickness = 0;				// Thickness (default=0)
	pEntityHeader->LineTypeScale = 1.0;			// Linetype scale (default=1.0)
	pEntityHeader->ExtrusionDirection[0] = 0;	// Extrusion direction. (default = 0, 0, 1)
	pEntityHeader->ExtrusionDirection[1] = 0;
	pEntityHeader->ExtrusionDirection[2] = 1;
}

/*-------------------------------------------------------------------*
 |  dxfReadEntityData_Direct                                         |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      LPVOID pEntityData = pointer to entity data structure to read|
 |  Output: TRUE if new entity is loaded                             |
 *-------------------------------------------------------------------*/
BOOL dxfReadEntityData_Direct( PDXF pDxf, PDXFENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	BOOL	result;

	// Check if current section is ENTITIES or BLOCKS
	if(pDxf->Read.CurrentSection!=SEC_ENTITIES)
	{
		if((pDxf->Read.CurrentSection!=SEC_BLOCKS) || (!pDxf->Read.isBlockOpen))
		{
			// Can not read entity data
			return FALSE;
		}
	}

	dxfStorePos(pDxf);
	ReadParamFromDxfFile(pDxf, GCode, strValue);
	if(GCode!=0)
	{
		// Can not read entity data
		dxfRestorePos(pDxf);
		return FALSE;
	}

	SetEntityHeaderDefaultValues(pEntityHeader);	// Set Entities Header to Default Values
	result = FALSE;

	do {
		if(strcmp(strValue, "LINE")==0)
		{
			ReadLineData(pDxf, pEntityHeader, (PDXFENTLINE)pEntityData);
			result = TRUE;
			break;
		}
		else if(strcmp(strValue, "POINT")==0)
		{
			ReadPointData(pDxf, pEntityHeader, (PDXFENTPOINT)pEntityData);
			result = TRUE;
			break;
		}
		else if(strcmp(strValue, "CIRCLE")==0)
		{
			ReadCircleData(pDxf, pEntityHeader, (PDXFENTCIRCLE)pEntityData);
			result = TRUE;
			break;
		}
		else if(strcmp(strValue, "ELLIPSE")==0)
		{
			ReadEllipseData(pDxf, pEntityHeader, (PDXFENTELLIPSE)pEntityData);
			result = TRUE;
			break;
		}
		else if(strcmp(strValue, "TEXT")==0)
		{
			ReadTextData(pDxf, pEntityHeader, (PDXFENTTEXT)pEntityData);
			result = TRUE;
			break;
		}
		else if(strcmp(strValue, "ARC")==0)
		{
			ReadArcData(pDxf, pEntityHeader, (PDXFENTARC)pEntityData);
			result = TRUE;
			break;
		}
		else if(strcmp(strValue, "SOLID")==0)
		{
			ReadSolidData(pDxf, pEntityHeader, (PDXFENTSOLID)pEntityData);
			result = TRUE;
			break;
		}
		else if(strcmp(strValue, "INSERT")==0)
		{
			ReadInsertData(pDxf, pEntityHeader, (PDXFENTINSERT)pEntityData);
			result = TRUE;
			break;
		}
		//This code is implemented by tran duy dung
		//*******************************************************************
		else if(strcmp(strValue, "POLYLINE")==0){
			ReadPolyLineData(pDxf, pEntityHeader, (PDXFENTPOLYLINE)pEntityData);
			result = TRUE;
			break;
		}
		//*******************************************************************
		else if(strcmp(strValue, "DIMENSION")==0)
		{
			ReadDimensionData(pDxf, pEntityHeader, (PDXFENTDIMENSION)pEntityData);
			result = TRUE;
			break;
		}
		else
		{
			// Unknown entity
			// Looking for next entity
			do {
				ReadParamFromDxfFile(pDxf, GCode, strValue);
			} while(GCode!=0);
		}
	} while((strcmp(strValue, "ENDBLK")!=0) && (strcmp(strValue, "ENDSEC")!=0) && (pDxf->Read.CurrentPos<pDxf->Read.FileSize));

	if((pDxf->Read.CurrentSection==SEC_BLOCKS) && (pDxf->Read.isBlockOpen))
	{
		if((GCode==0) && (strcmp(strValue, "ENDBLK")==0))
		{
			pDxf->Read.isBlockOpen = FALSE;

			do{
				dxfStorePos(pDxf);
				ReadParamFromDxfFile(pDxf, GCode, strValue);
			} while(GCode!=0);

			if(strcmp(strValue, "ENDSEC")==0)
				pDxf->Read.CurrentSection = SEC_NOTSET; // Blocks section has been finished
			else // Reached to the next block
				dxfRestorePos(pDxf);
		}
		else
			dxfRestorePos(pDxf);
	}
	else
	{
		if((GCode==0) && (strcmp(strValue, "ENDSEC")==0))
			pDxf->Read.CurrentSection = SEC_NOTSET; // Entities section has been finished
		else
			dxfRestorePos(pDxf);
	}

	return result;
}

/*-------------------------------------------------------------------*
 |  dxfReadEntityData                                                |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      PDXFENTITYHEADER pEntityHeader =                             |
 |                  pointer to entity data header                    |
 |      LPVOID pEntityData = pointer to entity data structure to read|
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfReadEntityData( HDXF hDxf, PDXFENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	if(!dxfReadEntityData_Direct(pDxf, pEntityHeader, pEntityData))
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}
