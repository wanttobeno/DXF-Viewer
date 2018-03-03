/*-------------------------------------------------------------------*\
|  CadLib Version 2.1                                                 |
|  Written by Omid Shahabi <omid_shahabi@hotmail.com>                 |
|  Copyright ?2002-2004                                              |
|  Pars Technology Development Co.                                    |
|                                                                     |
|  This software is distributed on an "AS IS" basis, WITHOUT WARRANTY |
|  OF ANY KIND, either express or implied.                            |
|                                                                     |
|                                                                     |
|  CadLib.cpp: The interface for CadIO.dll                            |
\*-------------------------------------------------------------------*/

#include "stdafx.h"
#include "CadLib.h"

// ¾²Ì¬¿âÄ£Ê½ 
#ifndef  ZY_DLL

#include "DrawCode/DXFFile/Base/DXFBase.h"
#include "DrawCode/DXFFile/Read/DXFRead.h"
#include "DrawCode/DXFFile/Write/DXFWrite.h"
#include "DrawCode/MemDrw.h"  //drwCreate
#include "DrawCode/DrawingDef.h"
#include "DrawCode/DrwIO.h"
// View
#include "DrawCode/View/View.h"


#endif	// ZY_DLL



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef ZY_DLL
#define ISLIBRARYLOADED();         \
	if(!isLibraryLoaded)           \
	{                              \
		/* Library is not loaded*/ \
		return FALSE;              \
	}                              
#else
	#define ISLIBRARYLOADED();  \
	isLibraryLoaded = TRUE;
#endif // ZY_DLL

/********************************************************************
 * CDXFFileWrite Class Implementation                               *
 ********************************************************************/

// Initialization & UnInitialization -------------------------------
CDxfFileWrite::CDxfFileWrite()
{
	bool result;

	isLibraryLoaded = true;

	// load CadIO.dll
	hinstCadIO = LoadLibrary( "CadIO.dll" );
	if( !hinstCadIO )
	{
		// Cannot load CadIO.dll
		isLibraryLoaded = false;
        return;
	}

	result = true;
#ifdef ZY_DLL
	
	result &= ((dxfWriteParamString = (BOOL(*)(HDXF, int, LPCTSTR))
		GetProcAddress( hinstCadIO, "dxfWriteParamString" )) != NULL);
	result &= ((dxfWriteParamInteger = (BOOL(*)(HDXF, int, int))
		GetProcAddress( hinstCadIO, "dxfWriteParamInteger" )) != NULL);
	result &= ((dxfWriteParamDouble = (BOOL(*)(HDXF, int, double))
		GetProcAddress( hinstCadIO, "dxfWriteParamDouble" )) != NULL);
	
	result &= ((dxfCreateFile = (HDXF(*)(LPCTSTR))
		GetProcAddress( hinstCadIO, "dxfCreateFile" )) != NULL);
	result &= ((dxfCloseFile = (BOOL(*)(HDXF))
		GetProcAddress( hinstCadIO, "dxfCloseFile" )) != NULL);
	
	result &= ((dxfSetCurrentColor = (BOOL(*)(HDXF, int))
		GetProcAddress( hinstCadIO, "dxfSetCurrentColor" )) != NULL);
	
	result &= ((dxfSectionBegin = (BOOL(*)(HDXF hDxf, DWORD dwSection))
		GetProcAddress( hinstCadIO, "dxfSectionBegin" )) != NULL);
	result &= ((dxfSectionEnd = (BOOL(_cdecl*)(HDXF hDxf))
		GetProcAddress( hinstCadIO, "dxfSectionEnd" )) != NULL);
	
	result &= ((dxfTableTypeBegin = (BOOL(_cdecl*)(HDXF hDxf, DWORD dwTableType))
		GetProcAddress( hinstCadIO, "dxfTableTypeBegin" )) != NULL);
	result &= ((dxfTableTypeEnd = (BOOL(_cdecl*)(HDXF hDxf))
		GetProcAddress( hinstCadIO, "dxfTableTypeEnd" )) != NULL);
	result &= ((dxfAddLayer = (BOOL(_cdecl*)( HDXF hDxf, char* Name, int Color, char* Linetype ))
		GetProcAddress( hinstCadIO, "dxfAddLayer" )) != NULL);
	result &= ((dxfSetCurrentLayer = (BOOL(_cdecl*)(HDXF hDxf, char* Name, char* LineType))
		GetProcAddress( hinstCadIO, "dxfSetCurrentLayer" )) != NULL);
	result &= ((dxfAddLinetype = (BOOL(_cdecl*)(HDXF hDxf, PDXFLTYPE pLineType))
		GetProcAddress( hinstCadIO, "dxfAddLinetype" )) != NULL);
	result &= ((dxfSetCurrentLinetype = (BOOL(_cdecl*)(HDXF hDxf, char* Name))
		GetProcAddress( hinstCadIO, "dxfSetCurrentLinetype" )) != NULL);
	result &= ((dxfAddTextStyle = (BOOL(_cdecl*)(HDXF hDxf, PDXFSTYLE pTextStyle))
		GetProcAddress( hinstCadIO, "dxfAddTextStyle" )) != NULL);
	result &= ((dxfSetCurrentTextStyle = (BOOL(_cdecl*)(HDXF hDxf, char* Name))
		GetProcAddress( hinstCadIO, "dxfSetCurrentTextStyle" )) != NULL);
	result &= ((dxfAddDimStyle = (BOOL(_cdecl*)(HDXF, PDXFDIMSTYLE))
		GetProcAddress( hinstCadIO, "dxfAddDimStyle" )) != NULL);
	result &= ((dxfSetCurrentDimStyle = (BOOL(_cdecl*)(HDXF, LPCTSTR)) 
		GetProcAddress( hinstCadIO, "dxfSetCurrentDimStyle" )) != NULL);
	
	result &= ((dxfLine = (BOOL(_cdecl*)(HDXF hDxf, double x1, double y1, double x2, double y2))
		GetProcAddress( hinstCadIO, "dxfLine" )) != NULL);
	result &= ((dxfCircle = (BOOL(_cdecl*)(HDXF hDxf, double cx, double cy, double r))
		GetProcAddress( hinstCadIO, "dxfCircle" )) != NULL);
	result &= ((dxfSolid = (BOOL(_cdecl*)(HDXF hDxf, REALPOINT& Point0, REALPOINT& Point1, REALPOINT& Point2, REALPOINT& Point3))
		GetProcAddress( hinstCadIO, "dxfSolid" )) != NULL);
	result &= ((dxfText = (BOOL(_cdecl*)( HDXF hDxf, LPCTSTR Text, double x1, double y1, double x2, double y2, double Height, double Rotation, DWORD Justification, double WidthFactor, LPCTSTR StyleName ))
		GetProcAddress( hinstCadIO, "dxfText" )) != NULL);
	result &= ((dxfDimLinear = (BOOL(_cdecl*)( HDXF, double, double, double, double, double, double, double, LPCTSTR))
		GetProcAddress( hinstCadIO, "dxfDimLinear" )) != NULL);
	result &= ((dxfArc = (BOOL(_cdecl*)( HDXF, double, double, double, double, double, double ))
		GetProcAddress( hinstCadIO, "dxfArc" )) != NULL);
	result &= ((dxfInsertBlock = (BOOL(_cdecl*)( HDXF hDxf, LPCTSTR BlockName, double x, double y, double xScale, double yScale, double Rotation ))
		GetProcAddress( hinstCadIO, "dxfInsertBlock" )) != NULL);
	
	result &= ((dxfBlockBegin = (BOOL(_cdecl*)( HDXF hDxf, LPCTSTR BlockName, LPCTSTR LayerName, double bx, double by, char flags ))
		GetProcAddress( hinstCadIO, "dxfBlockBegin" )) != NULL);
	result &= ((dxfBlockEnd = (BOOL(_cdecl*)( HDXF hDxf ))
		GetProcAddress( hinstCadIO, "dxfBlockEnd" )) != NULL);

#endif // ZY_DLL

	if(!result)
	{
		// Cannot load all functions from library
		isLibraryLoaded = false;
		return;
	}

	isFileOpen = false;
}

CDxfFileWrite::~CDxfFileWrite()
{
	// Close file, if it is opened
	if(isFileOpen)
		Close();
#ifdef  ZY_DLL
	// Unload CadIO.dll
	if(isLibraryLoaded)
		FreeLibrary ( hinstCadIO );
#endif // ZY_DLL
	isLibraryLoaded = false;
}

// Construction & Destruction -------------------------------------
#ifdef __AFXWIN_H__
BOOL CDxfFileWrite::Create( CString filename, bool overwrite )
{
	return Create( filename.GetBuffer(512), overwrite );
}
#endif // #ifdef __AFXWIN_H__

BOOL CDxfFileWrite::Create( char* filename, bool overwrite )
{
	ISLIBRARYLOADED();
	
	if( (m_hDxf=dxfCreateFile( filename ))==NULL )
	{
		// Cannot create new dxf file
		return FALSE;
	}

	isFileOpen = true;

	return TRUE;
}

BOOL CDxfFileWrite::Close( )
{
	ISLIBRARYLOADED();

	if(dxfCloseFile( m_hDxf ))
	{
		isFileOpen = false;
		return TRUE;
	}
	else
		return FALSE;
}

// Initialization -------------------------------------------------
BOOL CDxfFileWrite::SetCurrentColor( int Color )
{
	ISLIBRARYLOADED();
	return dxfSetCurrentColor( m_hDxf, Color );
}

// Sections --------------------------------------------------------
BOOL CDxfFileWrite::BeginSection( DWORD dwSection )
{
	ISLIBRARYLOADED();
	return dxfSectionBegin( m_hDxf, dwSection );
}

BOOL CDxfFileWrite::EndSection( )
{
	ISLIBRARYLOADED();
	return dxfSectionEnd( m_hDxf );
}

// TABLES section --------------------------------------------------
BOOL CDxfFileWrite::BeginTableType( DWORD dwTableType )
{
	ISLIBRARYLOADED();
	return dxfTableTypeBegin( m_hDxf, dwTableType );
}

BOOL CDxfFileWrite::EndTableType( )
{
	ISLIBRARYLOADED();
	return dxfTableTypeEnd( m_hDxf );
}

BOOL CDxfFileWrite::AddLayer( char* Name, int Color, char* Linetype )
{
	ISLIBRARYLOADED();
	return dxfAddLayer( m_hDxf, Name, Color, Linetype );
}

BOOL CDxfFileWrite::SetCurrentLayer( char* Name, char* LineType )
{
	ISLIBRARYLOADED();
	return dxfSetCurrentLayer( m_hDxf, Name, LineType );
}

BOOL CDxfFileWrite::AddLinetype( PDXFLTYPE pLineType )
{
	ISLIBRARYLOADED();
	return dxfAddLinetype( m_hDxf, pLineType );
}

BOOL CDxfFileWrite::SetCurrentLinetype( char* Name )
{
	ISLIBRARYLOADED();
	return dxfSetCurrentLinetype( m_hDxf, Name );
}

BOOL CDxfFileWrite::AddTextStyle( PDXFSTYLE pTextStyle )
{
	ISLIBRARYLOADED();
	return dxfAddTextStyle( m_hDxf, pTextStyle );
}

BOOL CDxfFileWrite::SetCurrentTextStyle( char* Name )
{
	ISLIBRARYLOADED();
	return dxfSetCurrentTextStyle( m_hDxf, Name );
}

BOOL CDxfFileWrite::AddDimStyle( PDXFDIMSTYLE pDimStyle )
{
	ISLIBRARYLOADED();
	return dxfAddDimStyle( m_hDxf, pDimStyle );
}

BOOL CDxfFileWrite::SetCurrentDimStyle( LPCTSTR Name )
{
	ISLIBRARYLOADED();
	return dxfSetCurrentDimStyle( m_hDxf, Name );
}

// ENTITIES section ------------------------------------------------
BOOL CDxfFileWrite::Line( double x1, double y1, double x2, double y2 )
{
	ISLIBRARYLOADED();
	return dxfLine( m_hDxf, x1, y1, x2, y2 );
}

BOOL CDxfFileWrite::Circle( double cx, double cy, double r )
{
	ISLIBRARYLOADED();
	return dxfCircle( m_hDxf, cx, cy, r );
}

BOOL CDxfFileWrite::Solid( int PointsNum, PREALPOINT Points )
{
	ISLIBRARYLOADED();
	if(PointsNum==3)
		return dxfSolid( m_hDxf, Points[0], Points[1], Points[2], Points[2] );
	else if(PointsNum==4)
		return dxfSolid( m_hDxf, Points[0], Points[1], Points[2], Points[3] );
	else
		return FALSE;
}

BOOL CDxfFileWrite::Text( LPCTSTR Text, double x1, double y1, double x2, double y2, double Height, DWORD Justification, double Rotation, double WidthFactor, LPCTSTR StyleName )
{
	ISLIBRARYLOADED();
	return dxfText( m_hDxf, Text, x1, y1, x2, y2, Height, Rotation, Justification, WidthFactor, StyleName );
}

BOOL CDxfFileWrite::Text( LPCTSTR Text, double x, double y, double Height, double Rotation, double WidthFactor, LPCTSTR StyleName )
{
	ISLIBRARYLOADED();
	return dxfText( m_hDxf, Text, x, y, x, y, Height, Rotation, TJ_LEFT, WidthFactor, StyleName );
}

BOOL CDxfFileWrite::DimLinear( double x1, double y1, double x2, double y2, double x3, double y3, double angle, LPCTSTR text )
{
	ISLIBRARYLOADED();
	return dxfDimLinear( m_hDxf, x1, y1, x2, y2, x3, y3, angle, text );
}

BOOL CDxfFileWrite::Arc( double cx, double cy, double r, double StartAngle, double EndAngle, double Thickness )
{
	ISLIBRARYLOADED();
	return dxfArc( m_hDxf, cx, cy, r, StartAngle, EndAngle, Thickness );
}

BOOL CDxfFileWrite::InsertBlock( LPCTSTR BlockName, double x, double y, double xScale, double yScale, double Rotation )
{
	ISLIBRARYLOADED();
	return dxfInsertBlock( m_hDxf, BlockName, x, y, xScale, yScale, Rotation );
}
	
// BLOCKS section --------------------------------------------------
BOOL CDxfFileWrite::BlockBegin( LPCTSTR BlockName, LPCTSTR LayerName, double bx, double by, char flags )
{
	ISLIBRARYLOADED();
	return dxfBlockBegin( m_hDxf, BlockName, LayerName, bx, by, flags );
}

BOOL CDxfFileWrite::BlockEnd()
{
	ISLIBRARYLOADED();
	return dxfBlockEnd( m_hDxf );
}

// Base Functions --------------------------------------------------
BOOL CDxfFileWrite::WriteParameter( int GroupCode, LPCTSTR Value )
{
	ISLIBRARYLOADED();
	return dxfWriteParamString( m_hDxf, GroupCode, Value );
}

BOOL CDxfFileWrite::WriteParameter( int GroupCode, double Value )
{
	ISLIBRARYLOADED();
	return dxfWriteParamDouble( m_hDxf, GroupCode, Value );
}

BOOL CDxfFileWrite::WriteParameter( int GroupCode, int Value )
{
	ISLIBRARYLOADED();
	return dxfWriteParamInteger( m_hDxf, GroupCode, Value );
}


/********************************************************************
 * CDrawing Class Implementation                                    *
 ********************************************************************/

// Initialization & UnInitialization -------------------------------
CDrawing::CDrawing()
{
	bool result;

	m_hDrawing = NULL;
	isLibraryLoaded = true;
#ifdef ZY_DLL
	// load CadIO.dll
	hinstCadIO = LoadLibrary( "CadIO.dll" );
	if( !hinstCadIO )
	{
		// Cannot load CadIO.dll
		isLibraryLoaded = false;
        return;
	}

	result = true;

	// Construction & Destruction
	result &= ((drwCreate = (HDRAWING(*)())
		GetProcAddress( hinstCadIO, "drwCreate" )) != NULL);
	result &= ((drwDestroy = (BOOL(*)(HDRAWING))
		GetProcAddress( hinstCadIO, "drwDestroy" )) != NULL);
	
	// TABLES
	result &= ((drwAddTableType = (OBJHANDLE(*)(HDRAWING, DWORD, LPVOID))
		GetProcAddress( hinstCadIO, "drwAddTableType" )) != NULL);
	result &= ((drwDeleteTableType = (BOOL(*)(HDRAWING, DWORD, OBJHANDLE))
		GetProcAddress( hinstCadIO, "drwDeleteTableType" )) != NULL);
	result &= ((drwFindTableType = (OBJHANDLE(*)(HDRAWING, DWORD, DWORD, LPVOID))
		GetProcAddress( hinstCadIO, "drwFindTableType" )) != NULL);
	// BLOCKS
	result &= ((drwAddBlock = (OBJHANDLE(*)(HDRAWING, PBLOCKHEADER))
		GetProcAddress( hinstCadIO, "drwAddBlock" )) != NULL);
	result &= ((drwDeleteBlock = (BOOL(*)(HDRAWING, OBJHANDLE))
		GetProcAddress( hinstCadIO, "drwDeleteBlock" )) != NULL);
	result &= ((drwFindBlock = (OBJHANDLE(*)(HDRAWING, DWORD, PBLOCKHEADER))
		GetProcAddress( hinstCadIO, "drwFindBlock" )) != NULL);
	// ENTITIES
	result &= ((drwAddEntity = (OBJHANDLE(*)(HDRAWING, OBJHANDLE, PENTITYHEADER, LPVOID))
		GetProcAddress( hinstCadIO, "drwAddEntity" )) != NULL);
	result &= ((drwDeleteEntity = (BOOL(*)(HDRAWING, LPCTSTR, OBJHANDLE))
		GetProcAddress( hinstCadIO, "drwDeleteEntity" )) != NULL);
	result &= ((drwChangeEntity = (BOOL(*)(HDRAWING, LPCTSTR, PENTITYHEADER, LPVOID))
		GetProcAddress( hinstCadIO, "drwChangeEntity" )) != NULL);
	result &= ((drwFindEntity = (OBJHANDLE(*)(HDRAWING, LPCTSTR, PENTITYHEADER, LPVOID, DWORD))
		GetProcAddress( hinstCadIO, "drwFindEntity" )) != NULL);
	// Datafile dataexchange
	result &= ((drwSaveDataToFile = (BOOL(*)(HDRAWING, DWORD, LPCTSTR, HWND))
		GetProcAddress( hinstCadIO, "drwSaveDataToFile" )) != NULL);
	result &= ((drwLoadDataFromFile = (BOOL(*)(HDRAWING, OBJHANDLE, DWORD, LPCTSTR, HWND))
		GetProcAddress( hinstCadIO, "drwLoadDataFromFile" )) != NULL);
	// Drawing Window View
	result &= ((drwInitView = (BOOL(*)(HDRAWING, int, int, int, int))
		GetProcAddress( hinstCadIO, "drwInitView" )) != NULL);
	result &= ((drwPaint = (BOOL(*)(HDRAWING, HDC))
		GetProcAddress( hinstCadIO, "drwPaint" )) != NULL);
	result &= ((drwGetViewProperties = (BOOL(*)(HDRAWING, PVIEW))
		GetProcAddress( hinstCadIO, "drwGetViewProperties" )) != NULL);
	result &= ((drwSetViewProperties = (BOOL(*)(HDRAWING, PVIEW))
		GetProcAddress( hinstCadIO, "drwSetViewProperties" )) != NULL);
	result &= ((drwGetDrawingBorder = (BOOL(*)(HDRAWING, PREALRECT))
		GetProcAddress( hinstCadIO, "drwGetDrawingBorder" )) != NULL);
	result &= ((drwZoomExtents = (BOOL(*)(HDRAWING))
		GetProcAddress( hinstCadIO, "drwZoomExtents" )) != NULL);

	if(!result)
	{
		// Cannot load all functions from library
		isLibraryLoaded = false;
		return;
	}
#endif // ZY_DLL

}

CDrawing::~CDrawing()
{
	if(m_hDrawing!=NULL)
		Destroy();
	// Unload CadIO.dll
	if(isLibraryLoaded)
		FreeLibrary ( hinstCadIO );
	isLibraryLoaded = false;
}

BOOL CDrawing::isOpen()
{
	if((!isLibraryLoaded) || (m_hDrawing==NULL))
		return FALSE;
	else
		return TRUE;
}

// Construction & Destruction -------------------------------------
BOOL CDrawing::Create( )
{
	ISLIBRARYLOADED();
	if(m_hDrawing!=NULL)
	{
		// Drawing is already created
		return FALSE;
	}

	if((m_hDrawing = drwCreate())!=NULL)
	{
		m_EntityHeader.LayerObjhandle = CurrentLayerObjhandle = 0;// Layer 0
		m_EntityHeader.LTypeObjhandle = CurrentLTypeObjhandle = 0;// ByLayer
		m_EntityHeader.Color = CurrentColor = 256;				  // ByLayer
		m_EntityHeader.Thickness = CurrentThickness = 0;
		m_EntityHeader.LineTypeScale = 1.0;
		CurrentDimStyleObjhandle = 0;
		CurrentStyleObjhandle = 0;
		ActiveBlockObjhandle = 0;	// Entities section
	}

	return (m_hDrawing!=NULL);
}

BOOL CDrawing::Destroy( )
{
	BOOL result;
	ISLIBRARYLOADED();
	result = drwDestroy(m_hDrawing);
	if(result)
		m_hDrawing = NULL;
	return result;
}

// Drawing View
BOOL CDrawing::InitView( int x, int y, int nWidth, int nHeight )
{
	ISLIBRARYLOADED();
	if(m_hDrawing!=NULL)
		return drwInitView(m_hDrawing, x, y, nWidth, nHeight);
	else
		return NULL;
}

BOOL CDrawing::Paint( HDC hdc )
{
	ISLIBRARYLOADED();
	return drwPaint(m_hDrawing, hdc );
}

double CDrawing::GetZoomLevel( )
{
	ISLIBRARYLOADED();

	VIEW drwview;
	if(drwGetViewProperties(m_hDrawing, &drwview))
		return drwview.ZoomLevel;
	else
		return 0;
}

BOOL CDrawing::SetZoomLevel(double ZoomLevel)
{
	ISLIBRARYLOADED();

	VIEW drwview;
	if(drwGetViewProperties(m_hDrawing, &drwview))
	{
		double cx;
		double cy;
		cx = drwview.ViewLeft + ((drwview.WindowRight-drwview.WindowLeft)/2)*(1/(drwview.PPU*drwview.ZoomLevel));
		cy = drwview.ViewBottom + ((drwview.WindowBottom-drwview.WindowTop)/2)*(1/(drwview.PPU*drwview.ZoomLevel));

		drwview.ZoomLevel = ZoomLevel;
		drwview.ViewLeft = cx - ((drwview.WindowRight-drwview.WindowLeft)/2)*(1/(drwview.PPU*drwview.ZoomLevel));
		drwview.ViewBottom = cy - ((drwview.WindowBottom-drwview.WindowTop)/2)*(1/(drwview.PPU*drwview.ZoomLevel));
		if(drwSetViewProperties(m_hDrawing, &drwview))
			return TRUE;
	}
	return FALSE;
}

BOOL CDrawing::GetViewProperties( PVIEW pView )
{
	ISLIBRARYLOADED();
	return drwGetViewProperties(m_hDrawing, pView);
}

BOOL CDrawing::SetViewProperties( PVIEW pView )
{
	ISLIBRARYLOADED();
	return drwSetViewProperties(m_hDrawing, pView);
}

BOOL CDrawing::GetDrawingBorder( PREALRECT pRect )
{
	ISLIBRARYLOADED();
	return drwGetDrawingBorder(m_hDrawing, pRect);
}

BOOL CDrawing::ZoomExtents( )
{
	ISLIBRARYLOADED();
	return drwZoomExtents(m_hDrawing);
}

// Configuration ---------------------------------------------------
BOOL CDrawing::SetLayer( LPCTSTR Name )
{
	LAYER Layer;
	strcpy(Layer.Name, Name);
	if(drwFindTableType(m_hDrawing, TAB_LAYER, FIND_BYNAME, &Layer)>0)
	{
		CurrentLayerObjhandle = Layer.Objhandle;
		m_EntityHeader.LayerObjhandle = CurrentLayerObjhandle;
		SetLineType(NULL); // Set linetype to default
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CDrawing::SetLineType( LPCTSTR Name )
{
	LTYPE LType;

	// Check for default linetype
	if(Name==NULL)
	{
		CurrentLTypeObjhandle = 0;
		m_EntityHeader.LTypeObjhandle = 0;
		return TRUE;
	}

	strcpy(LType.Name, Name);
	if(drwFindTableType(m_hDrawing, TAB_LTYPE, FIND_BYNAME, &LType)>0)
	{
		CurrentLTypeObjhandle = LType.Objhandle;
		m_EntityHeader.LTypeObjhandle = CurrentLTypeObjhandle;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CDrawing::SetTextStyle( LPCTSTR Name )
{
	STYLE Style;
	strcpy(Style.Name, Name);
	if(drwFindTableType(m_hDrawing, TAB_STYLE, FIND_BYNAME, &Style)>0)
	{
		CurrentStyleObjhandle = Style.Objhandle;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CDrawing::SetDimStyle( LPCTSTR Name )
{
	DIMSTYLE DimStyle;
	strcpy(DimStyle.Name, Name);
	if(drwFindTableType(m_hDrawing, TAB_DIMSTYLE, FIND_BYNAME, &DimStyle)>0)
	{
		CurrentDimStyleObjhandle = DimStyle.Objhandle;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CDrawing::SetColor( short Color )
{
	CurrentColor = Color;
	m_EntityHeader.Color = CurrentColor;
	return TRUE;
}

BOOL CDrawing::SetThickness( double Thickness )
{
	CurrentThickness = Thickness;
	m_EntityHeader.Thickness = CurrentThickness;
	return TRUE;
}

double CDrawing::SetLineTypeScale( double LineTypeScale )
{
	double prevLineTypeScale = m_EntityHeader.LineTypeScale;
	m_EntityHeader.LineTypeScale = LineTypeScale;
	return prevLineTypeScale;
}

// TABLES ----------------------------------------------------------
OBJHANDLE CDrawing::AddTableType( DWORD dwTableType, LPVOID pTableType )
{
	ISLIBRARYLOADED();
	return drwAddTableType(m_hDrawing, dwTableType, pTableType);
}

OBJHANDLE CDrawing::AddLayer( PLAYER pLayer )
{
	return AddTableType(TAB_LAYER, pLayer);
}

OBJHANDLE CDrawing::AddLinetype( PLTYPE pLineType )
{
	return AddTableType(TAB_LTYPE, pLineType);
}

OBJHANDLE CDrawing::AddTextStyle( PSTYLE pTextStyle )
{
	return AddTableType(TAB_STYLE, pTextStyle);
}

OBJHANDLE CDrawing::AddDimStyle( PDIMSTYLE pDimStyle )
{
	return AddTableType(TAB_DIMSTYLE, pDimStyle);
}

BOOL CDrawing::DeleteTableType( DWORD dwTableType, OBJHANDLE TableTypeObjhandle )
{
	ISLIBRARYLOADED();
	return drwDeleteTableType(m_hDrawing, dwTableType, TableTypeObjhandle);
}

BOOL CDrawing::DeleteLayer( OBJHANDLE LayerObjhandle )
{
	return DeleteTableType(TAB_LAYER, LayerObjhandle);
}

BOOL CDrawing::DeleteLinetype( OBJHANDLE LineTypeObjhandle )
{
	return DeleteTableType(TAB_LTYPE, LineTypeObjhandle);
}

BOOL CDrawing::DeleteTextStyle( OBJHANDLE TextStyleObjhandle )
{
	return DeleteTableType(TAB_STYLE, TextStyleObjhandle);
}

BOOL CDrawing::DeleteDimStyle( OBJHANDLE DimStyleObjhandle )
{
	return DeleteTableType(TAB_DIMSTYLE, DimStyleObjhandle);
}

OBJHANDLE CDrawing::FindTableType( DWORD dwTableType, DWORD dwFindType, LPVOID pTableType )
{
	ISLIBRARYLOADED();
	return drwFindTableType(m_hDrawing, dwTableType, dwFindType, pTableType);
}

// BLOCKS ----------------------------------------------------------
OBJHANDLE CDrawing::AddBlock( PBLOCKHEADER pBlockHeader, LPCTSTR strFileName )
{
	OBJHANDLE	result;
	ISLIBRARYLOADED();
	result = drwAddBlock(m_hDrawing, pBlockHeader);
	if((strFileName!=NULL) && result)
		drwLoadDataFromFile(m_hDrawing, result, 0, strFileName, NULL);
	return result;
}

BOOL CDrawing::DeleteBlock( OBJHANDLE BlockObjhandle )
{
	ISLIBRARYLOADED();
	return drwDeleteBlock(m_hDrawing, BlockObjhandle);
}

OBJHANDLE CDrawing::FindBlock( DWORD dwFindType, PBLOCKHEADER pBlockHeader )
{
	ISLIBRARYLOADED();
	return drwFindBlock(m_hDrawing, dwFindType, pBlockHeader );
}

BOOL CDrawing::SetActiveBlock( OBJHANDLE ohBlock )
{
	ISLIBRARYLOADED();

	if(ohBlock==NULL)
		ActiveBlockObjhandle = NULL; // Entities section is active now
	else
	{
		BLOCKHEADER BlockHeader;
		OBJHANDLE	ohBlockHeader;
		BlockHeader.Objhandle = ohBlock;
		ohBlockHeader = drwFindBlock(m_hDrawing, FIND_BYHANDLE, &BlockHeader);
		if(ohBlockHeader==0) // Block Not Found
			return FALSE;
		else
			ActiveBlockObjhandle = ohBlockHeader;
	}

	return TRUE;
}

BOOL CDrawing::SetActiveBlock( LPCTSTR strBlockName )
{
	ISLIBRARYLOADED();

	if(strBlockName==NULL)
		ActiveBlockObjhandle = NULL; // Entities section is active now
	else
	{
		BLOCKHEADER BlockHeader;
		OBJHANDLE	ohBlockHeader;
		strcpy(BlockHeader.Name, strBlockName);
		ohBlockHeader = drwFindBlock(m_hDrawing, FIND_BYNAME, &BlockHeader);
		if(ohBlockHeader==0) // Block Not Found
			return FALSE;
		else
			ActiveBlockObjhandle = ohBlockHeader;
	}

	return TRUE;
}

OBJHANDLE CDrawing::GetActiveBlock( )
{
	ISLIBRARYLOADED();
	return ActiveBlockObjhandle;
}

// ENTITIES --------------------------------------------------------
OBJHANDLE CDrawing::AddEntity( OBJHANDLE BlockObjhandle, PENTITYHEADER pEntityHeader, LPVOID pEntityData )
{
	ISLIBRARYLOADED();
	return drwAddEntity(m_hDrawing, BlockObjhandle, pEntityHeader, pEntityData);
}

BOOL CDrawing::DeleteEntity( LPCTSTR strBlockName, OBJHANDLE EntityObjhandle )
{
	ISLIBRARYLOADED();
	return drwDeleteEntity(m_hDrawing, strBlockName, EntityObjhandle);
}

BOOL CDrawing::ChangeEntity( PENTITYHEADER pEntityHeader, LPVOID pEntityData, LPCTSTR strBlockName )
{
	ISLIBRARYLOADED();
	return drwChangeEntity(m_hDrawing, strBlockName, pEntityHeader, pEntityData);
}

OBJHANDLE CDrawing::FindEntity( DWORD dwFindType, PENTITYHEADER pEntityHeader, LPVOID pEntityData, LPCTSTR strBlockName )
{
	ISLIBRARYLOADED();
	return drwFindEntity(m_hDrawing, strBlockName, pEntityHeader, pEntityData, dwFindType);
}

OBJHANDLE CDrawing::Arc( double cx, double cy, double r, double StartAngle, double EndAngle )
{
	ENTARC arc;
	arc.Point0.x = cx;
	arc.Point0.y = cy;
	arc.Radius = r;
	arc.StartAngle = StartAngle;
	arc.EndAngle = EndAngle;
	m_EntityHeader.EntityType = ENT_ARC;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &arc);
}

OBJHANDLE CDrawing::Circle( double cx, double cy, double r )
{
	ENTCIRCLE circle;
	circle.Point0.x = cx;
	circle.Point0.y = cy;
	circle.Radius = r;
	m_EntityHeader.EntityType = ENT_CIRCLE;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &circle);
}

/*OBJHANDLE CDrawing::Ellipse( double cx, double cy, double ex, double ey, double Ratio, double StartParam, double EndParam )
{
	ENTELLIPSE ellipse;
	ellipse.CenterPoint.x = cx;
	ellipse.CenterPoint.y = cy;
	ellipse.MajorAxisEndPoint.x = ex;
	ellipse.MajorAxisEndPoint.y = ey;
	ellipse.MinorToMajorRatio = Ratio;
	ellipse.StartParam = StartParam;
	ellipse.EndParam = EndParam;
	m_EntityHeader.EntityType = ENT_ELLIPSE;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &ellipse);
}
*/
OBJHANDLE CDrawing::DimLinear( double x1, double y1, double x2, double y2, double x3, double y3, double angle, LPCTSTR text )
{
	ENTDIMENSION dimension;
	ZeroMemory(&dimension, sizeof(ENTDIMENSION));
	dimension.DefPoint3.x = x1;
	dimension.DefPoint3.y = y1;
	dimension.DefPoint4.x = x2;
	dimension.DefPoint4.y = y2;
	dimension.DimLineDefPoint.x = x3;
	dimension.DimLineDefPoint.y = y3;
	dimension.DimRotationAngle = angle;
	strcpy(dimension.DimText, text);
	dimension.DimStyleObjhandle = CurrentDimStyleObjhandle;
	m_EntityHeader.EntityType = ENT_DIMENSION;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &dimension);
}

OBJHANDLE CDrawing::InsertBlock( OBJHANDLE BlockObjhandle, double x, double y, double xScale, double yScale, double Rotation )
{
	ENTINSERT insert;
	insert.BlockHeaderObjhandle = BlockObjhandle;
	insert.Point0.x = x;
	insert.Point0.y = y;
	insert.XScale = xScale;
	insert.YScale = yScale;
	insert.RotationAngle = Rotation;
	m_EntityHeader.EntityType = ENT_INSERT;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &insert);
}

OBJHANDLE CDrawing::InsertBlock( LPCTSTR BlockName, double x, double y, double xScale, double yScale, double Rotation )
{
	BLOCKHEADER BlockHeader;
	OBJHANDLE BlockHandle;
	strcpy(BlockHeader.Name, BlockName);
	BlockHandle = drwFindBlock(m_hDrawing, FIND_BYNAME, &BlockHeader);
	if(BlockHandle==0) // Block Not Found
		return 0;
	else
		return InsertBlock( BlockHandle, x, y, xScale, yScale, Rotation );
}


OBJHANDLE CDrawing::Line( double x1, double y1, double x2, double y2 )
{
	ENTLINE line;
	line.Point0.x = x1;
	line.Point0.y = y1;
	line.Point1.x = x2;
	line.Point1.y = y2;
	m_EntityHeader.EntityType = ENT_LINE;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &line);
}

OBJHANDLE CDrawing::Solid( REALPOINT &Point0, REALPOINT &Point1, REALPOINT &Point2, REALPOINT &Point3 )
{
	ENTSOLID solid;
	solid.Point0 = Point0;
	solid.Point1 = Point1;
	solid.Point2 = Point2;
	solid.Point3 = Point3;
	m_EntityHeader.EntityType = ENT_SOLID;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &solid);
}

OBJHANDLE CDrawing::Text( LPCTSTR Text, double x1, double y1, double x2, double y2, double Height, short Justification, double Rotation, double WidthFactor )
{
	ENTTEXT text;
	text.Point0.x = x1;
	text.Point0.y = y1;
	strcpy(text.strText, Text);
	text.TextData.Height = Height;
	text.TextData.RotationAngle = Rotation;
	text.TextData.WidthFactor = WidthFactor;
//	double		Oblique;
//	char		GenerationFlag;
	text.TextData.Justification = Justification;
	text.TextData.SecondAlignmentPoint.x = x2;
	text.TextData.SecondAlignmentPoint.y = y2;
	text.TextData.TextStyleObjhandle = CurrentStyleObjhandle;
	m_EntityHeader.EntityType = ENT_TEXT;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &text);
}

OBJHANDLE CDrawing::Text( LPCTSTR Text, double x, double y, double Height, double Rotation, double WidthFactor )
{
	return this->Text(Text, x, y, x, y, Height, TJ_LEFT, Rotation, WidthFactor);
}

OBJHANDLE CDrawing::PolyLine( PENTVERTEX pVertex, int nVertex, unsigned short Flag )
{
	ENTPOLYLINE polyline;
	polyline.pVertex = pVertex;
	polyline.nVertex = nVertex;
	polyline.Flag = Flag;
	m_EntityHeader.EntityType = ENT_POLYLINE;
	return AddEntity(ActiveBlockObjhandle, &m_EntityHeader, &polyline);
}

// Datafile data-exchange ------------------------------------------
BOOL CDrawing::SaveDXFFile( LPCTSTR FileName, HWND hWndProgress )
{
	ISLIBRARYLOADED();
	return drwSaveDataToFile(m_hDrawing, 0, FileName, hWndProgress);
}

BOOL CDrawing::LoadDXFFile( LPCTSTR FileName, HWND hWndProgress )
{
	ISLIBRARYLOADED();
	return drwLoadDataFromFile(m_hDrawing, NULL, 0, FileName, hWndProgress);
}

