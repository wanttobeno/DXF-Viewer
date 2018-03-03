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
|  DXFDef.h                                                           |
\*-------------------------------------------------------------------*/

#ifndef DXFDEF_H
#define DXFDEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
// Handle to DXF
DECLARE_HANDLE(HDXF);

// Section Definitions
#define	SEC_NOTSET		0x0000
#define	SEC_HEADER		0x0001
#define	SEC_CLASSES		0x0002
#define	SEC_TABLES		0x0004
#define	SEC_BLOCKS		0x0008
#define	SEC_ENTITIES	0x0010
#define	SEC_OBJECTS		0x0020
#define	SEC_UNKNOWN		0x0040

// Table Type Definitions
#define	TAB_NOTSET		0x0000
#define	TAB_APPID		0x0001
#define	TAB_BLOCKRECORD	0x0002
#define	TAB_DIMSTYLE	0x0004
#define	TAB_LAYER		0x0008
#define	TAB_LTYPE		0x0010
#define	TAB_STYLE		0x0020
#define	TAB_UCS			0x0040
#define	TAB_VIEW		0x0080
#define	TAB_VPORT		0x0100
#define	TAB_UNKNOWN		0x0200

// Text Justification Types
#define	TJ_LEFT			0x0000
#define	TJ_CENTER		0x0001
#define	TJ_RIGHT		0x0002
#define	TJ_ALIGNED		0x0003
#define	TJ_MIDDLE		0x0004
#define	TJ_FIT			0x0005
#define	TJ_BOTTOMLEFT	0x0100
#define	TJ_BOTTOMCENTER	0x0101
#define	TJ_BOTTOMRIGHT	0x0102
#define	TJ_MIDDLELEFT	0x0200
#define	TJ_MIDDLECENTER	0x0201
#define	TJ_MIDDLERIGHT	0x0202
#define	TJ_TOPLEFT		0x0300
#define	TJ_TOPCENTER	0x0301
#define	TJ_TOPRIGHT		0x0302

// Structures
//  Base ----------
typedef struct tag_REALPOINT
{
	double	x;
	double	y;
	double	z;
} REALPOINT, *PREALPOINT;

typedef struct tag_REALRECT
{
	double	top;
	double	left;
	double	bottom;
	double	right;
} REALRECT, *PREALRECT;

//  TABLES --------
typedef struct tag_DXFLAYER
{
	char	Name[512];					// Layer Name
	int		StandardFlags;				// Standard flags
	int		Color;						// Layer Color
	char	LineType[512];				// Line Type
} DXFLAYER, *PDXFLAYER;

typedef struct tag_DXFLTYPE
{
	char	Name[512];					// Line Type Name
	int		StandardFlags;				// Standard flags
	char	DescriptiveText[512];		// Descriptive Text
	int		ElementsNumber;				// Line Elements Number
	double	TotalPatternLength;			// Total Pattern Length
	double	Elements[30];				// Line Elements
} DXFLTYPE, *PDXFLTYPE;

typedef struct tag_DXFSTYLE
{
	char	Name[512];					// Style Name
	char	PrimaryFontFilename[512];	// Primary Font Filename
	int		StandardFlags;				// Standard flag values -> 4=Vertical text
	int		TextGenerationFlags;		// Text generation flags -> 2=Text is backward  4=Text is upside down
	double	FixedTextHeight;			// Fixed text height
	double	WidthFactor;				// Width Factor
	double	Height;						// Height
	double	ObliqueAngle;				// Oblique angle
} DXFSTYLE, *PDXFSTYLE;

typedef struct tag_DXFDIMSTYLE
{
	char	Name[512];					// DimStyle Name
	int		StandardFlags;				// Standard flag values
	int		DIMCLRD;					// Dimension line & Arrow heads color
	double	DIMDLE;						// Dimension line size after Extensionline
	int		DIMCLRE;					// Extension line color
	double	DIMEXE;						// Extension line size after Dimline
	double	DIMEXO;						// Offset from origin
	char	DIMBLK1[512];				// 1st Arrow head
	char	DIMBLK2[512];				// 2nd Arrow head
	double	DIMASZ;						// Arrow size
	char	DIMTXSTY[512];				// Text style
	DWORD	DIMTXSTYObjhandle;			// Text style Object Handle
	int		DIMCLRT;					// Text color
	double	DIMTXT;						// Text height
	int		DIMTAD;						// Vertical Text Placement
	double	DIMGAP;						// Offset from dimension line
} DXFDIMSTYLE, *PDXFDIMSTYLE;

//  BLOCKS --------
typedef struct tag_DXFBLOCKHEADER
{
	char		Name[512];				// Block name
	char		Flags;					// Block-type flags
	REALPOINT	BasePoint;				// Base point
	char		LayerName[512];			// Layer Name
} DXFBLOCKHEADER, *PDXFBLOCKHEADER;

//  ENTITIES ------
typedef struct tag_DXFENTITYHEADER
{
	unsigned short	EntityType;			// Entity type
	char		LayerName[512];			// Used Layer's Name
	char		LTypeName[512];			// Used LineType's Name
	short		Color;					// Color (0=BYBLOCK, 256=BYLAYER, negative=layer is turned off)
	double		Thickness;				// Thickness (default=0)
	double		LineTypeScale;			// Linetype scale (default=1.0)
	double		ExtrusionDirection[3];	// Extrusion direction. (default = 0, 0, 1)
	short		LineWeight;				// Lineweight enum value (*2000*)
} DXFENTITYHEADER, *PDXFENTITYHEADER;

typedef struct tag_DXFENTLINE {
	REALPOINT	Point0;
	REALPOINT	Point1;
} DXFENTLINE, *PDXFENTLINE;

typedef struct tag_DXFENTARC {
	REALPOINT	Point0;
	double		Radius;
	double		StartAngle;
	double		EndAngle;
} DXFENTARC, *PDXFENTARC;

typedef struct tag_DXFENTCIRCLE {
	REALPOINT	Point0;
	double		Radius;
} DXFENTCIRCLE, *PDXFENTCIRCLE;

typedef struct tag_DXFENTDIMENSION {
	REALPOINT	DefPoint3;
	REALPOINT	DefPoint4;
	double		DimRotationAngle;
	REALPOINT	DimLineDefPoint;
	char		BlockName[16];
	char		DimStyleName[512];
	char		DimText[1024];
} DXFENTDIMENSION, *PDXFENTDIMENSION;

typedef struct tag_DXFENTINSERT {
	REALPOINT	Point0;
	double		XScale;
	double		YScale;
	double		ZScale;
	double		RotationAngle;
	char		BlockHeaderName[512];
} DXFENTINSERT, *PDXFENTINSERT;

typedef struct tag_DXFENTPOINT {
	REALPOINT	Point0;
} DXFENTPOINT, *PDXFENTPOINT;

typedef struct tag_DXFENTSOLID {
	REALPOINT	Point0;
	REALPOINT	Point1;
	REALPOINT	Point2;
	REALPOINT	Point3;
} DXFENTSOLID, *PDXFENTSOLID;

typedef struct tag_DXFTEXTDATA {
	double		Height;
	double		RotationAngle;
	double		WidthFactor;
	double		Oblique;
	char		GenerationFlag;
	short		Justification;
	REALPOINT	SecondAlignmentPoint;
	char		TextStyleName[512];
} _DXFTEXTDATA,*_PDXFTEXTDATA;

typedef struct tag_DXFENTTEXT {
	REALPOINT		Point0;
	_DXFTEXTDATA	TextData;
	char			strText[2048];
} DXFENTTEXT, *PDXFENTTEXT;

typedef struct tag_DXFENTVERTEX {
  REALPOINT			Point;
  unsigned short	Flag;
  double			StartWidth;
  double			EndWidth;
  double			Bulge;
  double			TangentDir;
} DXFENTVERTEX, *PDXFENTVERTEX;

// this code is implemented by tran duy dung 20.10.2003
typedef struct tag_DXFENTPOLYLINE {
	PDXFENTVERTEX	pVertex;
	int				nVertex;
	unsigned short	Flag;
} DXFENTPOLYLINE, *PDXFENTPOLYLINE;

typedef struct tag_DXFENTELLIPSE {
  REALPOINT		CenterPoint;
  REALPOINT		MajorAxisEndPoint;
  double		MinorToMajorRatio;
  double		StartParam;
  double		EndParam;
} DXFENTELLIPSE, *PDXFENTELLIPSE;

// Arrow Heads --------
struct ARROWHEAD
{
	LPCTSTR	ArrowHeadName;
	int		BLKNum;
	LPCTSTR	Command;
	double	Param1;
	double	Param2;
	double	Param3;
	double	Param4;
	double	Param5;
	double	Param6;
	double	Param7;
	double	Param8;
};

// Arrow Heads Data
static ARROWHEAD ArrowHeads[] =
{
	{"ClosedFilled", 1, "SOLID", 0, 0, -1, -0.1667, -1, 0.1667, -1, 0.1667},
	{"ClosedFilled", 2, "SOLID", 0, 0, -1, -0.1667, -1, 0.1667, -1, 0.1667},
	{"Oblique", 1, "LINE", -0.5, -0.5, 0.5, 0.5, 0, 0, 0, 0},
	{"Oblique", 2, "LINE", -0.5, -0.5, 0.5, 0.5, 0, 0, 0, 0},
};

static int ArrowHeadsNum = sizeof(ArrowHeads) / sizeof(ARROWHEAD);


#endif // DXFDEF_H
