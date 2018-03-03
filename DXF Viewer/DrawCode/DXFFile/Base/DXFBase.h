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
|  DXFBase.h                                                          |
\*-------------------------------------------------------------------*/

#ifndef DXFBASE_H
#define DXFBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\DXFDef.h"
#include "..\..\DrawingDef.h"

// Definitions
#define	READBUFFERSIZE	32768

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------
typedef struct tag_DXFREAD
{
	long	FileSize;
	long	StoredStreamPos;
	DWORD	CurrentSection;
	DWORD	CurrentTableType;
	BOOL	isBlockOpen;

	char	ReadBuffer[READBUFFERSIZE];
	long	BufferDataSize;
	long	BufferPos;
	long	CurrentPos;
} DXFREAD, *PDXFREAD;

typedef struct tag_DXFWRITE
{
	char	CurrentLayer[32];
	char	CurrentLine[32];
	char	CurrentTextStyle[32];
	char	CurrentDimStyle[32];
	int		CurrentColor;
	DWORD	DoneSections;
	DWORD	CurrentSection;
	DWORD	DoneTableTypes;
	DWORD	CurrentTableType;
	UINT	DimStylesNumber;
} DXFWRITE, *PDXFWRITE;

typedef struct tag_DXF
{
	DWORD		Size;
	FILE*		pStream;
	DXFWRITE	Write;
	DXFREAD		Read;
	BOOL		isWriteMode;
} DXF, *PDXF;

//---------------------------------------------------------------------------
// Function declarations
//---------------------------------------------------------------------------
PDXF InitilizePDXF(HDXF hDxf);
BOOL UnInitilizePDXF(HDXF hDxf);

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
// *** DXF Writer ***
BOOL dxfWriteParamString( 
	HDXF	hDxf,		// handle to the openning DXF file structure
	int		GroupCode,	// group code
	LPCTSTR	value		// pointer to null terminated string to put in DXF
);

BOOL dxfWriteParamInteger( 
	HDXF	hDxf,		// handle to the openning DXF file structure
	int		GroupCode,	// group code
	int		value		// an integer value to put in dxf file
);

BOOL dxfWriteParamDouble( 
	HDXF	hDxf,		// handle to the openning DXF file structure
	int		GroupCode,	// group code
	double	value		// a real value to put in DXF file
);

// *** DXF Read ***
BOOL ReadParamFromDxfFile(
	PDXF	pDxf,		// pointer to the openning DXF file structure
	int&	GroupCode,	// group code
	LPTSTR valuestr		// pointer to null terminated string to put data in it
);

BOOL dxfReadParam(
	HDXF	hDxf,		// handle to the openning DXF file structure
	int&	GroupCode,	// group code
	LPTSTR	valuestr	// pointer to null terminated string to put data in it
);

BOOL FindParamFromDxfFile(
	PDXF	pDxf,		// pointer to the openning DXF file structure
	int		GroupCode,	// group code
	LPCTSTR	valuestr	// pointer to null terminated string to find
);

BOOL dxfFindParam(
	HDXF	hDxf,		// handle to the openning DXF file structure
	int		GroupCode,	// group code
	LPCTSTR	valuestr	// pointer to null terminated string to find
);

BOOL dxfStorePos(
	PDXF pDxf			// pointer to the openning DXF file structure
);

BOOL dxfRestorePos(
	PDXF pDxf			// pointer to the openning DXF file structure
);

#endif // DXFBASE_H
