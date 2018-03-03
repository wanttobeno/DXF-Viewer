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
|  DRTables.cpp                                                       |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "DRTables.h"

/********************************************************************
 * TABLES Section                                                   *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  ReadTableTypeName                                                |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |  Output: TableType Code else TAB_NOTSET                           |
 *-------------------------------------------------------------------*/
DWORD ReadTableTypeName( PDXF pDxf )
{
	int		GCode;
	char	strValue[2048];

	ReadParamFromDxfFile(pDxf, GCode, strValue);
	if(strcmp(strValue, "LAYER")==0)
		pDxf->Read.CurrentTableType = TAB_LAYER;
	else if (strcmp(strValue, "LTYPE")==0)
		pDxf->Read.CurrentTableType = TAB_LTYPE;
	else if (strcmp(strValue, "STYLE")==0)
		pDxf->Read.CurrentTableType = TAB_STYLE;
	else if (strcmp(strValue, "DIMSTYLE")==0)
		pDxf->Read.CurrentTableType = TAB_DIMSTYLE;
	else
		pDxf->Read.CurrentTableType = TAB_UNKNOWN;

	return pDxf->Read.CurrentTableType;
}

/*-------------------------------------------------------------------*
 |  dxfReadTableTypeName                                             |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: TableType Code else TAB_NOTSET                           |
 *-------------------------------------------------------------------*/
DWORD dxfReadTableTypeName( HDXF hDxf )
{
	PDXF	pDxf;
	int		GCode;
	char	strValue[2048];
	DWORD	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Read.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return FALSE;
	}

	dxfStorePos(pDxf);

	// Read TableType Name
	dxfReadParam(hDxf, GCode, strValue);
	if((GCode==0) && (strcmp(strValue, "TABLE")==0))
		ReadTableTypeName(pDxf);
	else
		dxfRestorePos(pDxf); // Restore Old Position

	result = pDxf->Read.CurrentTableType;

	// UnInitilize hDxf -----------------
	UnInitilizePDXF(hDxf);

	return result;
}

/*-------------------------------------------------------------------*
 |  dxfGetCurrentTableType                                           |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: current table type code if everything is ok              |
 *-------------------------------------------------------------------*/
DWORD dxfGetCurrentTableType( HDXF hDxf )
{
	PDXF	pDxf;
	DWORD	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Read.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return FALSE;
	}

	result = pDxf->Read.CurrentTableType;

	// UnInitilize hDxf -----------------
	UnInitilizePDXF(hDxf);

	return result;
}

/*-------------------------------------------------------------------*
 |  dxfFindNextTableType                                             |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |  Output: TableType Code else TAB_NOTSET                           |
 *-------------------------------------------------------------------*/
DWORD dxfFindNextTableType( HDXF hDxf )
{
	PDXF	pDxf;
	DWORD	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Read.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return FALSE;
	}

	// Find TableType Name
	if(FindParamFromDxfFile(pDxf, 0, "TABLE"))
	{
		// Read TableType Name
		ReadTableTypeName(pDxf);
		result = pDxf->Read.CurrentTableType;
	}
	else
		result = TAB_NOTSET;

	// UnInitilize hDxf -----------------
	UnInitilizePDXF(hDxf);

	return result;
}

/*-------------------------------------------------------------------*
 |  ReadDimStyleData                                                 |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFDIMSTYLE pDimStyle = pointer to DimStyle structure       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadDimStyleData( PDXF pDxf, PDXFDIMSTYLE pDimStyle )
{
	int		GCode;
	char	strValue[2048];

	ZeroMemory(pDimStyle, sizeof(DXFDIMSTYLE));
	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 2:		// DimStyle Name
			strcpy(pDimStyle->Name, strValue);
			break;
		case 70:	// Standard flag values
			pDimStyle->StandardFlags = char(atoi(strValue));
			break;
		case 176:	// Dimension line & Arrow heads color
			pDimStyle->DIMCLRD = atoi(strValue);
			break;
		case 46:	// Dimension line size after Extensionline
			pDimStyle->DIMDLE = atof(strValue);
			break;
		case 177:	// Extension line color
			pDimStyle->DIMCLRE = atoi(strValue);
			break;
		case 44:	// Extension line size after Dimline
			pDimStyle->DIMEXE = atof(strValue);
			break;
		case 42:	// Offset from origin
			pDimStyle->DIMEXO = atof(strValue);
			break;
		case 6:		// 1st Arrow head
			strcpy(pDimStyle->DIMBLK1, strValue);
			break;
		case 7:		// 2nd Arrow head
			strcpy(pDimStyle->DIMBLK2, strValue);
			break;
		case 41:	// Arrow size
			pDimStyle->DIMASZ = atof(strValue);
			break;
		case 340:	// Text style handle
			pDimStyle->DIMTXSTYObjhandle = atoi(strValue);
			break;
		case 178:	// Text color
			pDimStyle->DIMCLRT = atoi(strValue);
			break;
		case 140:	// Text height
			pDimStyle->DIMTXT = atof(strValue);
			break;
		case 77:	// Vertical Text Placement
			pDimStyle->DIMTAD = atoi(strValue);
			break;
		case 147:	// Offset from dimension line
			pDimStyle->DIMGAP = atof(strValue);
			break;
		}
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	dxfRestorePos(pDxf);
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadLayerData                                                    |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFLAYER pLayer = pointer to Layer structure                |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadLayerData( PDXF pDxf, PDXFLAYER pLayer )
{
	int		GCode;
	char	strValue[2048];

	ZeroMemory(pLayer, sizeof(DXFLAYER));
	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 2:		// Layer Name
			strcpy(pLayer->Name, strValue);
			break;
		case 70:	// Standard flags
			pLayer->StandardFlags = char(atoi(strValue));
			break;
		case 62:	// Layer Color
			pLayer->Color = atoi(strValue);
			break;
		case 6:		// Linetype
			strcpy(pLayer->LineType, strValue);
			break;
 		}
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	dxfRestorePos(pDxf);
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadLTypeData                                                    |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFLTYPE pLType = pointer to LType structure                |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadLTypeData( PDXF pDxf, PDXFLTYPE pLType )
{
	int		GCode;
	char	strValue[2048];
	int		i;

	i = 0;
	ZeroMemory(pLType, sizeof(DXFLTYPE));
	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 2:		// Linetype Name
			strcpy(pLType->Name, strValue);
			break;
		case 70:	// Standard flags
			pLType->StandardFlags = char(atoi(strValue));
			break;
		case 3:		// Descriptive text for linetype
			strcpy(pLType->DescriptiveText, strValue);
			break;
		case 73:	// Number of linetype elements
			pLType->ElementsNumber = atoi(strValue);
			break;
		case 40:	// Total pattern length
			pLType->TotalPatternLength = atof(strValue);
			break;
		case 49:	// Dash, dot or space length
			pLType->Elements[i] = atof(strValue);
			i++;
			break;
		}
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}
	
	dxfRestorePos(pDxf);
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  ReadStyleData                                                    |
 |  Inputs:                                                          |
 |      PDXF pDxf = pointer to the openning DXF file structure       |
 |      PDXFSTYLE pStyle = pointer to Style structure                |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL ReadStyleData( PDXF pDxf, PDXFSTYLE pStyle )
{
	int		GCode;
	char	strValue[2048];

	ZeroMemory(pStyle, sizeof(DXFSTYLE));
	ReadParamFromDxfFile(pDxf, GCode, strValue);
	while(GCode!=0)
	{
		switch(GCode)
		{
		case 2:		// Style Name
			strcpy(pStyle->Name, strValue);
			break;
		case 3:		// Primary Font Filename
			strcpy(pStyle->PrimaryFontFilename, strValue);
			break;
		case 70:	// Standard flags
			pStyle->StandardFlags = char(atoi(strValue));
			break;
		case 71:	// Text generation flags -> 2=Text is backward  4=Text is upside down
			pStyle->TextGenerationFlags = atoi(strValue);
			break;
		case 40:	// Fixed text height
			pStyle->FixedTextHeight = atof(strValue);
			break;
		case 41:	// Width Factor
			pStyle->WidthFactor = atof(strValue);
			break;
		case 42:	// Height
			pStyle->Height = atof(strValue);
			break;
		case 50:	// Oblique angle
			pStyle->ObliqueAngle = atof(strValue);
			break;
		}
		dxfStorePos(pDxf);
		ReadParamFromDxfFile(pDxf, GCode, strValue);
	}

	dxfRestorePos(pDxf);
	return TRUE;
}

/*-------------------------------------------------------------------*
 |  dxfReadTableType                                                 |
 |  Inputs:                                                          |
 |      HDXF hDxf = handle to the openning DXF file structure        |
 |      LPVOID pTableType = pointer to table type structure          |
 |  Output: TableType Code else TAB_NOTSET                           |
 *-------------------------------------------------------------------*/
DWORD dxfReadTableType( HDXF hDxf, LPVOID pTableType )
{
	PDXF	pDxf;
	int		GCode;
	char	strValue[2048];
	DWORD	result;

	// Initialize pDxf ------------------
	if((pDxf = InitilizePDXF(hDxf))==NULL)
		return FALSE;

	// Check if current section is TABLES
	if(pDxf->Read.CurrentSection!=SEC_TABLES)
	{
		// Current section is not TABLES
		GlobalUnlock(hDxf);
		return TAB_NOTSET;
	}

	// Looking for next table type if CurrentTableType==TAB_NOTSET
	if(pDxf->Read.CurrentTableType==TAB_NOTSET)
	{
		if(dxfFindNextTableType(hDxf)==TAB_NOTSET)
		{
			pDxf->Read.CurrentSection = SEC_NOTSET;
			GlobalUnlock(hDxf);
			return TAB_NOTSET;
		}
	}

	// Read current table type data
	result = TAB_NOTSET;
	switch(pDxf->Read.CurrentTableType)
	{
	case TAB_DIMSTYLE:
		if(!FindParamFromDxfFile(pDxf, 0, "DIMSTYLE"))
			break;
		else
		{
			if(ReadDimStyleData(pDxf, (PDXFDIMSTYLE)pTableType))
				result = TAB_DIMSTYLE;
		}
		break;

	case TAB_LAYER:
		if(!FindParamFromDxfFile(pDxf, 0, "LAYER"))
			break;
		else
		{
			if(ReadLayerData(pDxf, (PDXFLAYER)pTableType))
				result = TAB_LAYER;
		}
		break;

	case TAB_LTYPE:
		if(!FindParamFromDxfFile(pDxf, 0, "LTYPE"))
			break;
		else
		{
			if(ReadLTypeData(pDxf, (PDXFLTYPE)pTableType))
				result = TAB_LTYPE;
		}
		break;

	case TAB_STYLE:
		if(!dxfFindParam( hDxf, 0, "STYLE" ))
			break;
		else
		{
			if(ReadStyleData(pDxf, (PDXFSTYLE)pTableType))
				result = TAB_STYLE;
		}
		break;
	}

	dxfStorePos(pDxf);
	dxfReadParam(hDxf, GCode, strValue);

	if((GCode==0) && (strcmp(strValue, "ENDTAB")==0))
	{
		pDxf->Read.CurrentTableType = TAB_NOTSET;

//		while(dxfReadTableTypeName(hDxf)==TAB_NOTSET)
//		{
			dxfStorePos(pDxf);
			dxfReadParam(hDxf, GCode, strValue);
			if((GCode==0) && (strcmp(strValue, "ENDSEC")==0))
			{
				pDxf->Read.CurrentSection = SEC_NOTSET; // Tables section has been finished
//				break;
			}
			else
				dxfRestorePos(pDxf);
//		}
	}
	else
		dxfRestorePos(pDxf);

	// UnInitilize hDxf -----------------
	UnInitilizePDXF(hDxf);

	return result;
}

