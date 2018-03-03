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
|  CadLib.h: The interface for CadIO.dll                              |
\*-------------------------------------------------------------------*/

#ifndef CADLIB_H
#define CADLIB_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DXFDef.h"
#include "DrawingDef.h"

// CDxfFileWrite class ---------------------------------------------
class CDxfFileWrite
{
public:
	// Initialization & UnInitialization
	CDxfFileWrite();
	~CDxfFileWrite();

	// Construction & Destruction
#ifdef __AFXWIN_H__
	BOOL Create( CString filename, bool overwrite=false );
#endif // __AFXWIN_H__
	
	BOOL Create( char* filename, bool overwrite=false );
	BOOL Close( );

	// Initialization
	BOOL SetCurrentColor( int Color );

	// Sections
	BOOL BeginSection( DWORD dwSection );
	BOOL EndSection( );

	// TABLES section
	BOOL BeginTableType( DWORD dwTableType );
	BOOL EndTableType( );
	BOOL AddLayer( char* Name, int Color, char* Linetype );
	BOOL SetCurrentLayer( char* Name, char* LineType=NULL );
	BOOL AddLinetype( PDXFLTYPE pLineType );
	BOOL SetCurrentLinetype( char* Name=NULL );
	BOOL AddTextStyle( PDXFSTYLE pTextStyle );
	BOOL SetCurrentTextStyle( char* Name=NULL );
	BOOL AddDimStyle( PDXFDIMSTYLE pDimStyle );
	BOOL SetCurrentDimStyle( LPCTSTR Name );

	// ENTITIES section
	BOOL Line( double x1, double y1, double x2, double y2 );
	BOOL Circle( double cx, double cy, double r );
	BOOL Solid( int PointsNum, PREALPOINT Points );
	BOOL Text( LPCTSTR Text, double x1, double y1, double x2, double y2, double Height, DWORD Justification, double Rotation=0, double WidthFactor=0, LPCTSTR StyleName=NULL );
	BOOL Text( LPCTSTR Text, double x, double y, double Height, double Rotation=0, double WidthFactor=0, LPCTSTR StyleName=NULL );
	BOOL DimLinear( double x1, double y1, double x2, double y2, double x3, double y3, double angle, LPCTSTR text=NULL );
	BOOL Arc( double cx, double cy, double r, double StartAngle, double EndAngle, double Thickness=0.0 );
	BOOL InsertBlock( LPCTSTR BlockName, double x, double y, double xScale=1, double yScale=1, double Rotation=0 );
	
	// BLOCKS section
	BOOL BlockBegin( LPCTSTR BlockName, LPCTSTR LayerName, double bx, double by, char flags=0);
	BOOL BlockEnd();

	BOOL WriteParameter( int GroupCode, LPCTSTR Value );
	BOOL WriteParameter( int GroupCode, double Value );
	BOOL WriteParameter( int GroupCode, int Value );

// Base Functions
protected:
	HINSTANCE hinstCadIO;
	HDXF m_hDxf;
	bool isLibraryLoaded;
	bool isFileOpen;

private:
	// DLL 类型的接口
#ifdef ZY_DLL
	BOOL (*dxfWriteParamString)( HDXF hDxf, int GroupCode, LPCTSTR value );
	BOOL (*dxfWriteParamInteger)( HDXF hDxf, int GroupCode, int value );
	BOOL (*dxfWriteParamDouble)( HDXF hDxf, int GroupCode, double value );
	HDXF (*dxfCreateFile)( LPCTSTR FileName );
	BOOL (*dxfCloseFile)( HDXF hDxf );
	BOOL (*dxfSetCurrentColor)( HDXF hDxf, int Color );
	BOOL (*dxfSectionBegin)( HDXF hDxf, DWORD dwSection );
	BOOL (*dxfSectionEnd)( HDXF hDxf );
	BOOL (*dxfTableTypeBegin)( HDXF hDxf, DWORD dwTableType );
	BOOL (*dxfTableTypeEnd)( HDXF hDxf );
	BOOL (*dxfAddLayer)( HDXF hDxf, char* Name, int Color, char* Linetype );
	BOOL (*dxfSetCurrentLayer)( HDXF hDxf, char* Name, char* LineType );
	BOOL (*dxfAddLinetype)( HDXF hDxf, PDXFLTYPE pLineType );
	BOOL (*dxfSetCurrentLinetype)( HDXF hDxf, char* Name );
	BOOL (*dxfAddTextStyle)( HDXF hDxf, PDXFSTYLE pTextStyle );
	BOOL (*dxfSetCurrentTextStyle)( HDXF hDxf, char* Name );
	BOOL (*dxfLine)( HDXF hDxf, double x1, double y1, double x2, double y2 );
	BOOL (*dxfCircle)( HDXF hDxf, double cx, double cy, double r );
	BOOL (*dxfSolid)( HDXF hDxf, REALPOINT& Point0, REALPOINT& Point1, REALPOINT& Point2, REALPOINT& Point3 );
	BOOL (*dxfText)( HDXF hDxf, LPCTSTR Text, double x1, double y1, double x2, double y2, double Height, double Rotation, DWORD Justification, double WidthFactor, LPCTSTR StyleName );
	BOOL (*dxfDimLinear)( HDXF hDxf, double x1, double y1, double x2, double y2, double x3, double y3, double angle, LPCTSTR text );
	BOOL (*dxfAddDimStyle)( HDXF hDxf, PDXFDIMSTYLE pDimStyle );
	BOOL (*dxfSetCurrentDimStyle)( HDXF hDxf, LPCTSTR Name );
	BOOL (*dxfArc)( HDXF hDxf, double cx, double cy, double r, double StartAngle, double EndAngle, double Thickness );
	BOOL (*dxfInsertBlock)( HDXF hDxf, LPCTSTR BlockName, double x, double y, double xScale, double yScale, double Rotation );
	BOOL (*dxfBlockBegin)( HDXF hDxf, LPCTSTR BlockName, LPCTSTR LayerName, double bx, double by, char flags );
	BOOL (*dxfBlockEnd)( HDXF hDxf );
	
#else

	
	BOOL dxfWriteParamString( HDXF hDxf, int GroupCode, LPCTSTR value );
	BOOL dxfWriteParamInteger( HDXF hDxf, int GroupCode, int value );
	BOOL dxfWriteParamDouble(HDXF hDxf, int GroupCode, double value );
	HDXF dxfCreateFile(LPCTSTR FileName );
	BOOL dxfCloseFile(HDXF hDxf );
	BOOL dxfSetCurrentColor(HDXF hDxf, int Color );
	BOOL dxfSectionBegin(HDXF hDxf, DWORD dwSection );
	BOOL dxfSectionEnd(HDXF hDxf );
	BOOL dxfTableTypeBegin(HDXF hDxf, DWORD dwTableType );
	BOOL dxfTableTypeEnd(HDXF hDxf );
	BOOL dxfAddLayer(HDXF hDxf, char* Name, int Color, char* Linetype );
	BOOL dxfSetCurrentLayer(HDXF hDxf, char* Name, char* LineType );
	BOOL dxfAddLinetype(HDXF hDxf, PDXFLTYPE pLineType );
	BOOL dxfSetCurrentLinetype(HDXF hDxf, char* Name );
	BOOL dxfAddTextStyle(HDXF hDxf, PDXFSTYLE pTextStyle );
	BOOL dxfSetCurrentTextStyle(HDXF hDxf, char* Name );
	BOOL dxfLine(HDXF hDxf, double x1, double y1, double x2, double y2 );
	BOOL dxfCircle(HDXF hDxf, double cx, double cy, double r );
	BOOL dxfSolid(HDXF hDxf, REALPOINT& Point0, REALPOINT& Point1, REALPOINT& Point2, REALPOINT& Point3 );
	BOOL dxfText(HDXF hDxf, LPCTSTR Text, double x1, double y1, double x2, double y2, double Height, double Rotation, DWORD Justification, double WidthFactor, LPCTSTR StyleName );
	BOOL dxfDimLinear(HDXF hDxf, double x1, double y1, double x2, double y2, double x3, double y3, double angle, LPCTSTR text );
	BOOL dxfAddDimStyle(HDXF hDxf, PDXFDIMSTYLE pDimStyle );
	BOOL dxfSetCurrentDimStyle(HDXF hDxf, LPCTSTR Name );
	BOOL dxfArc(HDXF hDxf, double cx, double cy, double r, double StartAngle, double EndAngle, double Thickness );
	BOOL dxfInsertBlock( HDXF hDxf, LPCTSTR BlockName, double x, double y, double xScale, double yScale, double Rotation );
	BOOL dxfBlockBegin(HDXF hDxf, LPCTSTR BlockName, LPCTSTR LayerName, double bx, double by, char flags );
	BOOL dxfBlockEnd(HDXF hDxf );
	
#endif // ZY_DLL


}; 

// CDrawing class --------------------------------------------------
class CDrawing
{
public:
	// Initialization & UnInitialization
	CDrawing();
	~CDrawing();

	BOOL isOpen();
	// Construction & Destruction
	BOOL Create( );
	BOOL Destroy( );

	// Drawing View
	BOOL InitView( int x, int y, int nWidth, int nHeight );
	BOOL Paint( HDC hdc );
	double GetZoomLevel( );
	BOOL   SetZoomLevel(double ZoomLevel);
	BOOL GetViewProperties( PVIEW pView );
	BOOL SetViewProperties( PVIEW pView );
	BOOL GetDrawingBorder( PREALRECT pRect );
	BOOL ZoomExtents( );

	// Configuration
	BOOL SetLayer( LPCTSTR Name );
	BOOL SetLineType( LPCTSTR Name );
	BOOL SetTextStyle( LPCTSTR Name );
	BOOL SetDimStyle( LPCTSTR Name );
	BOOL SetColor( short Color );
	BOOL SetThickness( double Thickness );
	double SetLineTypeScale( double LineTypeScale );

	// TABLES section
	OBJHANDLE AddTableType( DWORD dwTableType, LPVOID pTableType );
	OBJHANDLE AddLayer( PLAYER pLayer );
	OBJHANDLE AddLinetype( PLTYPE pLineType );
	OBJHANDLE AddTextStyle( PSTYLE pTextStyle );
	OBJHANDLE AddDimStyle( PDIMSTYLE pDimStyle );
//	BOOL ChangeTableType( DWORD dwTableType, OBJHANDLE TableTypeObjhandle, LPVOID pTableType );
	BOOL DeleteTableType( DWORD dwTableType, OBJHANDLE TableTypeObjhandle );
	BOOL DeleteLayer( OBJHANDLE LayerObjhandle );
	BOOL DeleteLinetype( OBJHANDLE LineTypeObjhandle );
	BOOL DeleteTextStyle( OBJHANDLE TextStyleObjhandle );
	BOOL DeleteDimStyle( OBJHANDLE DimStyleObjhandle );
	OBJHANDLE FindTableType( DWORD dwTableType, DWORD dwFindType, LPVOID pTableType );

	// BLOCKS section
	OBJHANDLE AddBlock( PBLOCKHEADER pBlockHeader, LPCTSTR strFileName=NULL );
//	BOOL ChangeBlock( OBJHANDLE BlockObjhandle, PBLOCKHEADER pBlockHeader );
	BOOL DeleteBlock( OBJHANDLE BlockObjhandle );
	OBJHANDLE FindBlock( DWORD dwFindType, PBLOCKHEADER pBlockHeader );
	BOOL SetActiveBlock( OBJHANDLE ohBlock );
	BOOL SetActiveBlock( LPCTSTR strBlockName );
	OBJHANDLE GetActiveBlock( );

	// ENTITIES section
	OBJHANDLE AddEntity( OBJHANDLE BlockObjhandle, PENTITYHEADER pEntityHeader, LPVOID pEntityData );
	BOOL ChangeEntity( PENTITYHEADER pEntityHeader, LPVOID pEntityData, LPCTSTR strBlockName=NULL );
	BOOL DeleteEntity( LPCTSTR strBlockName, OBJHANDLE EntityObjhandle );
	OBJHANDLE FindEntity( DWORD dwFindType, PENTITYHEADER pEntityHeader, LPVOID pEntityData, LPCTSTR strBlockName=NULL );
	OBJHANDLE Arc( double cx, double cy, double r, double StartAngle, double EndAngle );
	OBJHANDLE Circle( double cx, double cy, double r );
//	OBJHANDLE Ellipse( double cx, double cy, double ex, double ey, double Ratio, double StartParam=0.0, double EndParam=2*PI );
	OBJHANDLE DimLinear( double x1, double y1, double x2, double y2, double x3, double y3, double angle, LPCTSTR text=NULL );
	OBJHANDLE InsertBlock( OBJHANDLE BlockObjhandle, double x, double y, double xScale=1, double yScale=1, double Rotation=0 );
	OBJHANDLE InsertBlock( LPCTSTR BlockName, double x, double y, double xScale=1, double yScale=1, double Rotation=0 );
	OBJHANDLE Line( double x1, double y1, double x2, double y2 );
	OBJHANDLE Solid( REALPOINT &Point0, REALPOINT &Point1, REALPOINT &Point2, REALPOINT &Point3 );
	OBJHANDLE Text( LPCTSTR Text, double x1, double y1, double x2, double y2, double Height, short Justification, double Rotation=0, double WidthFactor=0 );
	OBJHANDLE Text( LPCTSTR Text, double x, double y, double Height, double Rotation=0, double WidthFactor=0 );
	OBJHANDLE PolyLine( PENTVERTEX pVertex, int nVertex, unsigned short Flag=0 );

	// Datafile data-exchange
	BOOL SaveDXFFile( LPCTSTR FileName, HWND hWndProgress=NULL );
	BOOL LoadDXFFile( LPCTSTR FileName, HWND hWndProgress=NULL );
	
protected:
	HINSTANCE		hinstCadIO;
	HDRAWING		m_hDrawing;
	bool			isLibraryLoaded;
	OBJHANDLE		CurrentLayerObjhandle;
	OBJHANDLE		CurrentLTypeObjhandle;
	OBJHANDLE		CurrentDimStyleObjhandle;
	OBJHANDLE		CurrentStyleObjhandle;
	short			CurrentColor;
	double			CurrentThickness;
	OBJHANDLE		ActiveBlockObjhandle;
	ENTITYHEADER	m_EntityHeader;

//protected:
//	OBJHANDLE InitEntityHeader();

private:
#ifdef ZY_DLL

	HDRAWING (*drwCreate)( );
	BOOL (*drwDestroy)( HDRAWING hDrawing );
	
	OBJHANDLE (*drwAddTableType)( HDRAWING hDrawing, DWORD dwTableType, LPVOID pTableType );
	BOOL (*drwDeleteTableType)( HDRAWING hDrawing, DWORD dwTableType, OBJHANDLE TableTypeObjhandle );
	OBJHANDLE (*drwFindTableType)( HDRAWING hDrawing, DWORD dwTableType, DWORD dwFindType, LPVOID pTableType );
	
	OBJHANDLE (*drwAddBlock)( HDRAWING hDrawing, PBLOCKHEADER pBlockHeader );
	BOOL (*drwDeleteBlock)( HDRAWING hDrawing, OBJHANDLE BlockObjhandle );
	OBJHANDLE (*drwFindBlock)( HDRAWING hDrawing, DWORD dwFindType, PBLOCKHEADER pBlockHeader );
	
	OBJHANDLE (*drwAddEntity)( HDRAWING hDrawing, OBJHANDLE BlockObjhandle, PENTITYHEADER pEntityHeader, LPVOID pEntityData );
	BOOL (*drwDeleteEntity)( HDRAWING hDrawing, LPCTSTR strBlockName, OBJHANDLE EntityHandle );
	BOOL (*drwChangeEntity)( HDRAWING hDrawing, LPCTSTR strBlockName, PENTITYHEADER pEntityHeader, LPVOID pEntityData );
	OBJHANDLE (*drwFindEntity)( HDRAWING hDrawing, LPCTSTR strBlockName, PENTITYHEADER pEntityHeader, LPVOID pEntityData, DWORD dwFindType );
	
	BOOL (*drwSaveDataToFile)( HDRAWING hDrawing, DWORD Reserved, LPCTSTR strFileName, HWND hWndProgress );
	BOOL (*drwLoadDataFromFile)( HDRAWING hDrawing, OBJHANDLE BlockObjhandle, DWORD Reserved, LPCTSTR strFileName, HWND hWndProgress );
	
	BOOL (*drwInitView)( HDRAWING hDrawing, int x, int y, int Width, int Height );
	BOOL (*drwPaint)( HDRAWING hDrawing, HDC hdc );
	BOOL (*drwGetViewProperties)( HDRAWING hDrawing, PVIEW pView );
	BOOL (*drwSetViewProperties)( HDRAWING hDrawing, PVIEW pView );
	BOOL (*drwGetDrawingBorder)( HDRAWING hDrawing, PREALRECT pRect);
	BOOL (*drwZoomExtents)( HDRAWING hDrawing );

#else
	HDRAWING drwCreate( );
	BOOL drwDestroy( HDRAWING hDrawing );
	
	OBJHANDLE drwAddTableType( HDRAWING hDrawing, DWORD dwTableType, LPVOID pTableType );
	BOOL drwDeleteTableType( HDRAWING hDrawing, DWORD dwTableType, OBJHANDLE TableTypeObjhandle );
	OBJHANDLE drwFindTableType( HDRAWING hDrawing, DWORD dwTableType, DWORD dwFindType, LPVOID pTableType );
	
	OBJHANDLE drwAddBlock( HDRAWING hDrawing, PBLOCKHEADER pBlockHeader );
	BOOL drwDeleteBlock( HDRAWING hDrawing, OBJHANDLE BlockObjhandle );
	OBJHANDLE drwFindBlock( HDRAWING hDrawing, DWORD dwFindType, PBLOCKHEADER pBlockHeader );
	
	OBJHANDLE drwAddEntity( HDRAWING hDrawing, OBJHANDLE BlockObjhandle, PENTITYHEADER pEntityHeader, LPVOID pEntityData );
	BOOL drwDeleteEntity( HDRAWING hDrawing, LPCTSTR strBlockName, OBJHANDLE EntityHandle );
	BOOL drwChangeEntity( HDRAWING hDrawing, LPCTSTR strBlockName, PENTITYHEADER pEntityHeader, LPVOID pEntityData );
	OBJHANDLE drwFindEntity( HDRAWING hDrawing, LPCTSTR strBlockName, PENTITYHEADER pEntityHeader, LPVOID pEntityData, DWORD dwFindType );
	
	BOOL drwSaveDataToFile( HDRAWING hDrawing, DWORD Reserved, LPCTSTR strFileName, HWND hWndProgress );
	BOOL drwLoadDataFromFile( HDRAWING hDrawing, OBJHANDLE BlockObjhandle, DWORD Reserved, LPCTSTR strFileName, HWND hWndProgress );
	
	BOOL drwInitView( HDRAWING hDrawing, int x, int y, int Width, int Height );
	BOOL drwPaint( HDRAWING hDrawing, HDC hdc );
	BOOL drwGetViewProperties( HDRAWING hDrawing, PVIEW pView );
	BOOL drwSetViewProperties( HDRAWING hDrawing, PVIEW pView );
	BOOL drwGetDrawingBorder( HDRAWING hDrawing, PREALRECT pRect);
	BOOL drwZoomExtents( HDRAWING hDrawing );

#endif // ZY_DLL

};

#endif // CADLIB_H
