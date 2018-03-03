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
|  DrwIO.h                                                            |
\*-------------------------------------------------------------------*/

#ifndef DRWIO_H
#define DRWIO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemDrw.h"

//---------------------------------------------------------------------------
// Exported Functions
//---------------------------------------------------------------------------
BOOL drwSaveDataToFile(
	HDRAWING	hDrawing,		// handle to the current DRAWING structure
	DWORD		Reserved,		// reserved, must be 0
	LPCTSTR		strFileName,	// output file name (Can not be NULL)
	HWND		hWndProgress	// handle of progress window
);

BOOL drwLoadDataFromFile(
	HDRAWING	hDrawing,		// handle to the current DRAWING structure
	OBJHANDLE	BlockObjhandle,	// handle of block that entities must be put in it. (NULL = Entity Section)
	DWORD		Reserved,		// reserved, must be 0
	LPCTSTR		strFileName,	// input file name (Can not be NULL)
	HWND		hWndProgress	// handle of progress window
);


#endif // DRWIO_H
