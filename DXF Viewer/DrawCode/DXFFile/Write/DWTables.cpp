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
|  DWTables.cpp                                                       |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DWTables.h"


/********************************************************************
 * TABLES Section                                                   *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  dxfTableTypeBegin                                                |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      DWORD dwTableType = ID of table type to begin                |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfTableTypeBegin( HDXF hDxf, DWORD dwTableType )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Write.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Process Table Type ---------------
	if(pDxf->Write.CurrentTableType!=TAB_NOTSET)
	{
		// Last table type is not closed
		GlobalUnlock(hDxf);
		return FALSE;
	}

	if((pDxf->Write.DoneTableTypes & dwTableType)!=0)
	{
		// Table type is already done
		GlobalUnlock(hDxf);
		return FALSE;
	}
	else
	{
		if(dwTableType==TAB_LAYER)
		{
			pDxf->Write.CurrentTableType = TAB_LAYER;
			if(!dxfWriteParamString(hDxf, 0, "TABLE"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
			if(!dxfWriteParamString(hDxf, 2, "LAYER"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
		}
		else if(dwTableType==TAB_LTYPE)
		{
			pDxf->Write.CurrentTableType = TAB_LTYPE;
			if(!dxfWriteParamString(hDxf, 0, "TABLE"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
			if(!dxfWriteParamString(hDxf, 2, "LTYPE"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
		}
		else if(dwTableType==TAB_STYLE)
		{
			pDxf->Write.CurrentTableType = TAB_STYLE;
			if(!dxfWriteParamString(hDxf, 0, "TABLE"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
			if(!dxfWriteParamString(hDxf, 2, "STYLE"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
		}
		else if(dwTableType==TAB_DIMSTYLE)
		{
			pDxf->Write.CurrentTableType = TAB_DIMSTYLE;
			if(!dxfWriteParamString(hDxf, 0, "TABLE"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
			if(!dxfWriteParamString(hDxf, 2, "DIMSTYLE"))
			{
				GlobalUnlock(hDxf);
				return FALSE;
			}
		}
		else
		{
			// Table type Not Allowed
			GlobalUnlock(hDxf);
			return FALSE;
		}
	}

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfTableTypeEnd                                                  |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfTableTypeEnd( HDXF hDxf )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check is there any table type not closed
	if(pDxf->Write.CurrentTableType==TAB_NOTSET)
	{
		// No table type is open
		GlobalUnlock(hDxf);
		return FALSE;
	}

	if(!dxfWriteParamString(hDxf, 0, "ENDTAB"))
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// ----------------------------------
	pDxf->Write.DoneTableTypes |= pDxf->Write.CurrentTableType;
	pDxf->Write.CurrentTableType = TAB_NOTSET;

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

// LAYER /////////////////

/*-------------------------------------------------------------------*
 |  dxfAddLayer                                                      |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      char* Name = layer name to be added                          |
 |      int Color = current layer's color number                     |
 |      char* Linetype = current layer's linetype name               |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfAddLayer( HDXF hDxf, char* Name, int Color, char* Linetype )
{
	PDXF	pDxf;
	BOOL	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Write.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Check if current type is LAYER
	if(pDxf->Write.CurrentTableType!=TAB_LAYER)
	{
		// Current table type is not LAYER
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;
	result &= dxfWriteParamString(hDxf, 0, "LAYER"); // Entity type
	result &= dxfWriteParamString(hDxf, 2, Name);	 // Layer Name
	result &= dxfWriteParamInteger(hDxf, 70, 0);	 // Standard flags
	result &= dxfWriteParamInteger(hDxf, 62, Color); // Layer Color
	result &= dxfWriteParamString(hDxf, 6, Linetype);// Linetype
	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfSetCurrentLayer                                               |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      char* Name = layer name to be set as current                 |
 |      char* LineType = current layer's line type                   |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfSetCurrentLayer( HDXF hDxf, char* Name, char* LineType )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Change current layer & current line
	if(Name!=NULL)
		strcpy( pDxf->Write.CurrentLayer, Name );
	else
	{
		// Layer's name cannot be NULL
		GlobalUnlock(hDxf);
		return FALSE;
	}
	if(LineType!=NULL)
		strcpy( pDxf->Write.CurrentLine, LineType );
	else
		strcpy( pDxf->Write.CurrentLine, "" );

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

// LTYPE /////////////////

/*-------------------------------------------------------------------*
 |  dxfAddLinetype                                                   |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      PDXFLTYPE pLineType = pointer to linetype structure          |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfAddLinetype( HDXF hDxf, PDXFLTYPE pLineType )
{
	PDXF	pDxf;
	BOOL	result;

	if(pLineType==NULL)
	{
		// pLineType shall not be NULL
		return FALSE;
	}

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Write.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Check if current type is LTYPE
	if(pDxf->Write.CurrentTableType!=TAB_LTYPE)
	{
		// Current table type is not LTYPE
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;
	result &= dxfWriteParamString(hDxf, 0, "LTYPE");					// Entity type
	result &= dxfWriteParamString(hDxf, 2, pLineType->Name);			// Linetype Name
	result &= dxfWriteParamInteger(hDxf, 70, pLineType->StandardFlags); // Standard flags
	result &= dxfWriteParamString(hDxf, 3, pLineType->DescriptiveText); // Descriptive text for linetype
	result &= dxfWriteParamInteger(hDxf, 72, 65);						// Alignment code
	result &= dxfWriteParamInteger(hDxf, 73, pLineType->ElementsNumber);// Number of linetype elements
	result &= dxfWriteParamDouble(hDxf, 40, pLineType->TotalPatternLength);// Total pattern length
	for(int i=0; i<pLineType->ElementsNumber; i++)
		result &= dxfWriteParamDouble(hDxf, 49, pLineType->Elements[i]);// Dash, dot or space length

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfSetCurrentLinetype                                            |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      char* Name = line type name to be set as current             |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfSetCurrentLinetype( HDXF hDxf, char* Name )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Change current line type
	strcpy( pDxf->Write.CurrentLine, Name );

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

// STYLE /////////////////

/*-------------------------------------------------------------------*
 |  dxfAddTextStyle                                                  |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      PDXFSTYLE pTextStyle = pointer to textstyle structure        |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfAddTextStyle( HDXF hDxf, PDXFSTYLE pTextStyle )
{
	PDXF	pDxf;
	BOOL	result;

	if(pTextStyle==NULL)
	{
		// pTextStyle shall not be NULL
		return FALSE;
	}

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Write.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Check if current type is STYLE
	if(pDxf->Write.CurrentTableType!=TAB_STYLE)
	{
		// Current table type is not STYLE
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Write data to output file
	result = TRUE;
	result &= dxfWriteParamString(hDxf, 0, "STYLE");						// Entity type
	result &= dxfWriteParamString(hDxf, 2, pTextStyle->Name);				// Style Name
	result &= dxfWriteParamString(hDxf, 3, pTextStyle->PrimaryFontFilename);// Primary Font Filename
	result &= dxfWriteParamInteger(hDxf, 70, pTextStyle->StandardFlags);	// Standard flag values -> 4=Vertical text
	result &= dxfWriteParamInteger(hDxf, 71, pTextStyle->TextGenerationFlags);// Text generation flags -> 2=Text is backward  4=Text is upside down
	result &= dxfWriteParamDouble(hDxf, 40, pTextStyle->FixedTextHeight);	// Fixed text height
	result &= dxfWriteParamDouble(hDxf, 41, pTextStyle->WidthFactor);		// Width Factor
	result &= dxfWriteParamDouble(hDxf, 42, pTextStyle->Height);			// Height
	result &= dxfWriteParamDouble(hDxf, 50, pTextStyle->ObliqueAngle);		// Oblique angle

	if(!result)
	{
		GlobalUnlock(hDxf);
		return FALSE;
	}
	
	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfSetCurrentTextStyle                                           |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      char* Name = textstyle name to be set as current             |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfSetCurrentTextStyle( HDXF hDxf, char* Name )
{
	PDXF	pDxf;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Change current text style type
	strcpy( pDxf->Write.CurrentTextStyle, Name );

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

// DIMSTYLE //////////////
/*-------------------------------------------------------------------*
 |  WriteDimStyleData                                                |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFDIMSTYLE pDimStyle = pointer to the dimstyle structure   |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL WriteDimStyleData( PDXF pDxf, PDXFDIMSTYLE pDimStyle )
{
	BOOL	result;

	result = TRUE;
/*	result &= dxfWriteParamString(hDxf, 0, "DIMSTYLE");						// DimStyle Name
	result &= dxfWriteParamString(hDxf, 2, pDimStyle->Name);				// Standard flag values
	result &= dxfWriteParamInteger(hDxf, 176, pDimStyle->DIMCLRD);			// Dimension line & Arrow heads color
	result &= dxfWriteParamDouble(hDxf, 46, pDimStyle->DIMDLE);				// Dimension line size after Extensionline
	result &= dxfWriteParamInteger(hDxf, 177, pDimStyle->DIMCLRE);			// Extension line color
	result &= dxfWriteParamDouble(hDxf, 44, pDimStyle->DIMEXE);				// Extension line size after Dimline
	result &= dxfWriteParamDouble(hDxf, 42, pDimStyle->DIMEXO);				// Offset from origin
	result &= dxfWriteParamString(hDxf, 6, pDimStyle->DIMBLK1);				// 1st Arrow head
	result &= dxfWriteParamString(hDxf, 7, pDimStyle->DIMBLK2);				// 2nd Arrow head
	result &= dxfWriteParamDouble(hDxf, 41, pDimStyle->DIMASZ);				// Arrow size
	result &= dxfWriteParamInteger(hDxf, 340, pDimStyle->DIMTXSTYObjhandle);// Text style Object Handle
	result &= dxfWriteParamInteger(hDxf, 178, pDimStyle->DIMCLRT);			// Text color
	result &= dxfWriteParamDouble(hDxf, 140, pDimStyle->DIMTXT);			// Text height
	result &= dxfWriteParamInteger(hDxf, 77, pDimStyle->DIMTAD);			// Vertical Text Placement
	result &= dxfWriteParamDouble(hDxf, 147, pDimStyle->DIMGAP);			// Offset from dimension line
*/
	return result;
}

/*-------------------------------------------------------------------*
 |  dxfAddDimStyle                                                   |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      PDXFDIMSTYLE pDimStyle = pointer to the dimstyle structure   |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfAddDimStyle( HDXF hDxf, PDXFDIMSTYLE pDimStyle )
{
	PDXF		pDxf;
	DWORD		CurrentSize;
	DWORD		NeededSize;

	if(pDimStyle==NULL)
	{
		// pDimStyle shall not be NULL
		return FALSE;
	}

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Write.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Check if current type is DIMSTYLE
	if(pDxf->Write.CurrentTableType!=TAB_DIMSTYLE)
	{
		// Current table type is not DIMSTYLE
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Save data to momery for further use
	CurrentSize = GlobalSize(hDxf);
	NeededSize = pDxf->Size + sizeof(DXFDIMSTYLE);
	if(NeededSize>CurrentSize)
	{
		GlobalUnlock(hDxf);
		GlobalReAlloc(hDxf, CurrentSize+65535, GHND);
		pDxf = (PDXF)GlobalLock(hDxf);
		CurrentSize = GlobalSize(hDxf);
		if(NeededSize>CurrentSize)
		{
			// Low memory error
			GlobalUnlock(hDxf);
			return FALSE;
		}
	}

	CopyMemory((LPVOID)((DWORD)pDxf+pDxf->Size), pDimStyle, sizeof(DXFDIMSTYLE));

	pDxf->Size += sizeof(DXFDIMSTYLE);
	pDxf->Write.DimStylesNumber++;

	// Write data to output file
	if(!WriteDimStyleData(pDxf, pDimStyle))
	{
		// Writing DimStyle data error
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

/*-------------------------------------------------------------------*
 |  dxfSetCurrentDimStyle                                            |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      LPCTSTR Name = pointer of dimstyle name to be set as current |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL dxfSetCurrentDimStyle( HDXF hDxf, LPCTSTR Name )
{
	PDXF			pDxf;
	PDXFDIMSTYLE	pNewDimStyle;

	if(Name==NULL)
	{
		// Name shall not be NULL
		return FALSE;
	}

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Search for dimstyle name
	pNewDimStyle = NULL;

	for(UINT i=0; i<pDxf->Write.DimStylesNumber; i++)
	{
		PDXFDIMSTYLE pFixDimStyle;
		pFixDimStyle = (PDXFDIMSTYLE)((DWORD)pDxf + sizeof(DXF) + sizeof(DXFDIMSTYLE)*i);
		if(strcmp(pFixDimStyle->Name, Name)==0)
		{
			pNewDimStyle = pFixDimStyle;
			break;
		}
	}

	if(pNewDimStyle==NULL)
	{
		// The dimstyle is not available
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Change current dim style
	strcpy( pDxf->Write.CurrentDimStyle, Name );

	// UnInitilize hDxf -----------------
	return UnInitilizePDXF(hDxf);
}

