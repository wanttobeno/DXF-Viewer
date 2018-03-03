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
|  MemDrw.h                                                           |
\*-------------------------------------------------------------------*/

#ifndef MEMDRW_H
#define MEMDRW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DXFDef.h"
#include "DrawingDef.h"

// Definitions
#define		MAX_DIMSTYLES	64
#define		MAX_LAYERS		128
#define		MAX_LTYPES		128
#define		MAX_STYLES		128

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

// Tables ***************************
typedef struct tag_TABLES
{
	LAYER		Layer[MAX_LAYERS];
	LTYPE		LType[MAX_LTYPES];
	STYLE		Style[MAX_STYLES];
	DIMSTYLE	DimStyle[MAX_DIMSTYLES];

	UINT		NumLayers;
	UINT		NumLTypes;
	UINT		NumStyles;
	UINT		NumDimStyles;
	
	UINT		CurrentLayer;
	UINT		CurrentLType;
	UINT		CurrentStyle;
	UINT		CurrentDimStyle;

//	OBJHANDLE	CurrentLayerHandle;		// Current Layer for drawing
//	OBJHANDLE	CurrentTextStyleHandle;	// Current TextStyle for drawing
//	OBJHANDLE	CurrentDimStyleHandle;	// Current DimStyle for drawing
//	OBJHANDLE	CurrentLineTypeHandle;	// Current LineType for drawing
//	short		CurrentColor;			// Current Color for drawing
//	short		CurrentLineWeight;		// Current LineWeight for drawing
} TABLES, *PTABLES;

// Drawing Structure ****************
typedef struct tag_DRAWING
{
	TABLES		Tables;
	ENTITIES	Entities;

	BLOCKHEADER	Blocks[256];
	DWORD		BlocksNumber;
	DWORD		CurrentBlock;

	OBJHANDLE	LastObjhandle;
	DWORD		LastDimBlkNum;

	VIEW		View;
} DRAWING, *PDRAWING;

//---------------------------------------------------------------------------
// Function declarations
//---------------------------------------------------------------------------
PDRAWING InitilizePDRAWING(HDRAWING hDrawing);
BOOL UnInitilizePDRAWING(HDRAWING hDrawing);

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
// Construction & Destruction ******
HDRAWING drwCreate();

BOOL drwDestroy(
	HDRAWING	hDrawing			// handle to the drawing to be destroyed
);

// TABLES ******
OBJHANDLE drwAddTableType(
	HDRAWING	hDrawing,			// handle to the current DRAWING structure
	DWORD		dwTableType,		// table type to be added
	LPVOID		pTableType			// pointer to table type structure to add
);

BOOL drwDeleteTableType(
	HDRAWING	hDrawing,			// handle to the current DRAWING structure
	DWORD		dwTableType,		// table type to be removed
	OBJHANDLE	TableTypeObjhandle	// handle of table type to remove
);

OBJHANDLE drwFindTableType_Direct(
	PDRAWING	pDrawing,			// pointer to the current DRAWING structure
	DWORD		dwTableType,		// table type to get data ( TAB_NOTSET = search in all table types )
	DWORD		dwFindType,			// find type
	LPVOID		pTableType			// pointer to table type structure
);

OBJHANDLE drwFindTableType(
	HDRAWING	hDrawing,			// handle to the current DRAWING structure
	DWORD		dwTableType,		// table type to get data ( TAB_NOTSET = search in all table types )
	DWORD		dwFindType,			// find type
	LPVOID		pTableType			// pointer to table type structure
);

// BLOCKS ******
OBJHANDLE drwAddBlock_Direct(
	PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	PBLOCKHEADER	pBlockHeader	// pointer to blockheader structure
);

OBJHANDLE drwAddBlock(
	HDRAWING		hDrawing,		// handle to the current DRAWING structure
    PBLOCKHEADER	pBlockHeader	// pointer to blockheader structure
);

BOOL drwDeleteBlock(
	HDRAWING		hDrawing,		// handle to the current DRAWING structure
	OBJHANDLE		BlockObjhandle	// handle of block to Delete
);

OBJHANDLE drwFindBlock_Direct(
	PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	DWORD			dwFindType,		// find type
	PBLOCKHEADER	pBlockHeader	// pointer to block header data
);

OBJHANDLE drwFindBlock(
	HDRAWING		hDrawing,		// handle to the current DRAWING structure
	DWORD			dwFindType,		// find type
	PBLOCKHEADER	pBlockHeader	// pointer to block header data
);

int drwGetBlocksNum(
	HDRAWING		hDrawing		// handle to the current DRAWING structure
);

// ENTITIES ******
OBJHANDLE drwAddEntity_Direct(
	PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	OBJHANDLE		BlockObjhandle,	// handle of block that entity must be added in it. (NULL = Entity Section)
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	LPVOID			pEntityData		// pointer to entity data structure to add
);

OBJHANDLE drwAddEntity(
	HDRAWING		hDrawing,		// handle to the current DRAWING structure
	OBJHANDLE		BlockObjhandle,	// handle of block that entity must be added in it. (NULL = Entity Section)
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	LPVOID			pEntityData		// pointer to entity data structure to add
);

BOOL drwDeleteEntity(
	HDRAWING		hDrawing,		// handle to the current DRAWING structure
	LPCTSTR			strBlockName,	// block name that entity must be deleted in it. (NULL=Entity Section)
	OBJHANDLE		EntityHandle	// handle of entity to be deleted
);

OBJHANDLE drwFindEntity_Direct(
	PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	LPCTSTR			strBlockName,	// block name that entity must be found in it. (NULL=Entity Section)
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	LPVOID			pEntityData,	// pointer to entity data structure
	DWORD			dwFindType,		// find type
	DWORD*			pdwEntityPos=NULL// position of found entity
);

OBJHANDLE drwFindEntity(
	HDRAWING		hDrawing,		// handle to the current DRAWING structure
	LPCTSTR			strBlockName,	// block name that entity must be found in it. (NULL=Entity Section)
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	LPVOID			pEntityData,	// pointer to entity data structure
	DWORD			dwFindType		// find type
);

BOOL drwChangeEntity_Direct(
	PDRAWING		pDrawing,		// pointer to the current DRAWING structure
	LPCTSTR			strBlockName,	// block name that entity must be changed in it. (NULL=Entity Section)
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	LPVOID			pEntityData		// pointer to entity data structure
);

BOOL drwChangeEntity(
	HDRAWING		hDrawing,		// handle to the current DRAWING structure
	LPCTSTR			strBlockName,	// block name that entity must be changed in it. (NULL=Entity Section)
	PENTITYHEADER	pEntityHeader,	// pointer to entity data header
	LPVOID			pEntityData		// pointer to entity data structure
);



#endif // MEMDRW_H
