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
|  TextView.cpp                                                       |
\*-------------------------------------------------------------------*/

//#include "stdafx.h"
#include "TextView.h"
#include "Colors.h"

/********************************************************************
 * Function for calculating text boundary rectangle                 *
 ********************************************************************/

/*-------------------------------------------------------------------*
 |  GetTextRect                                                      |
 |  Inputs:                                                          |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTTEXT pText = pointer to text data structure              |
 |      REALRECT* pRect = pointer to entity boundary rectangle       |
 |  Output: TRUE if everything is ok                                 |
 *-------------------------------------------------------------------*/
BOOL GetTextRect(PENTITYHEADER pEntityHeader, PENTTEXT pText, REALRECT* pRect)
{
	return TRUE;
}

/********************************************************************
 * Functions for plot the Text                                      *
 ********************************************************************/

// Units to pixel conversions
#define	U2PX(units)	pDrawing->View.WindowLeft + (int)((-pDrawing->View.ViewLeft + x+(units)*xScale) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define	U2PY(units)	pDrawing->View.WindowBottom - (int)((-pDrawing->View.ViewBottom + y+(units)*yScale) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define	U2PXROT(xUnits,yUnits)	pDrawing->View.WindowLeft + (int)((-pDrawing->View.ViewLeft + x+((xUnits)*xScale*cos(Rotation*PI/180)-(yUnits)*yScale*sin(Rotation*PI/180))*1) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define	U2PYROT(xUnits,yUnits)	pDrawing->View.WindowBottom - (int)((-pDrawing->View.ViewBottom + y+((xUnits)*xScale*sin(Rotation*PI/180)+(yUnits)*yScale*cos(Rotation*PI/180))*1) * pDrawing->View.PPU * pDrawing->View.ZoomLevel)
#define PI	3.1415926535

/*-------------------------------------------------------------------*
 |  isTextInViewableArea                                             |
 |  Inputs:                                                          |
 |      PVIEW pView = pointer to the current Drawing View structure  |
 |      RECT *pTextRect = pointer to text boundary rectangle         |
 |  Output:                                                          |
 |      TRUE if text can be shown in viewable area                   |
 *-------------------------------------------------------------------*/
BOOL isTextInViewableArea(PVIEW pView, RECT *pTextRect)
{
	if((pTextRect->top>=pView->WindowTop) && (pTextRect->top<=pView->WindowBottom) &&
	   (pTextRect->left>=pView->WindowLeft) && (pTextRect->left<=pView->WindowRight))
	   return TRUE;
	if((pTextRect->bottom>=pView->WindowTop) && (pTextRect->bottom<=pView->WindowBottom) &&
	   (pTextRect->right>=pView->WindowLeft) && (pTextRect->right<=pView->WindowRight))
	   return TRUE;

	return FALSE;
}


/*-------------------------------------------------------------------*
 |  DecodeText                                                       |
 |  Descriptions:                                                    |
 |      Converts DXF text to windows compatible text to be shown     |
 |  Inputs:                                                          |
 |      LPCTSTR TextIn = pointer to the current Text to be decoded   |
 |      LPTSTR TextOut = pointer to the decoded output Text          |
 |  Output:                                                          |
 |      length of the decoded output Text                            |
 *-------------------------------------------------------------------*/
UINT DecodeText(LPCTSTR TextIn, LPTSTR TextOut)
{
	UINT i, l, j;
	l=strlen(TextIn);
	j=0;
	for(i=0; i<l; i++)
	{
		if((TextIn[i]=='%') && (i<l-2))
		{
			if(TextIn[i+1]=='%')
			{
				switch(TextIn[i+2])
				{
				case 'o':
				case 'O':
				case 'u':
				case 'U':
					break;
				case 'd':
				case 'D':
					TextOut[j++]='?';
					break;
				case 'p':
				case 'P':
					TextOut[j++]='?';
					break;
				case 'c':
				case 'C':
					TextOut[j++]=char(216);
					break;
				case '%':
					TextOut[j++]='%';
					break;
				default:
					TextOut[j++]=TextIn[i+2];
				}
				i += 2;
			}
			else
				TextOut[j++]=TextIn[i];
		}
		else
			TextOut[j++]=TextIn[i];
	}

	TextOut[j]=0;
	return j;
}

/*-------------------------------------------------------------------*
 |  PlotText                                                         |
 |  Descriptions:                                                    |
 |      Plots a text into given Drawing Context                      |
 |  Inputs:                                                          |
 |      PDRAWING pDrawing = pointer to the current DRAWING structure |
 |      HDC hdc= handle of DC for drawing                            |
 |      PENTITYHEADER pEntityHeader = pointer to entity data header  |
 |      PENTTEXT pText = pointer to text data                        |
 |      double x = x position of insertion point (default = 0)       |
 |      double y = y position of insertion point (default = 0)       |
 |      double xScale = x scale factor (default = 1)                 |
 |      double yScale = y scale factor (default = 1)                 |
 |      double Rotation = rotation angle (default = 0)               |
 |  Output: Nothing                                                  |
 *-------------------------------------------------------------------*/
void PlotText(PDRAWING pDrawing, HDC hdc, PENTITYHEADER pEntityHeader, PENTTEXT pText, double x=0, double y=0, double xScale=1, double yScale=1, double Rotation=0)
{
	LAYER		Layer;
//	LTYPE		LType;
	STYLE		Style;
	COLORREF	rgbcolor;
	short		color;
	HPEN		pen, oldpen;
	HFONT		font, oldfont;

//	if(!((pText->Point0.y>=pDrawing->View.ViewBottom) && (pText->Point0.y<=pDrawing->View.ViewBottom + pDrawing->View.ViewHeight) &&
//	     (pText->Point0.x>=pDrawing->View.ViewLeft) && (pText->Point0.x<=pDrawing->View.ViewLeft + pDrawing->View.ViewWidth)))
//		return;

	// Initialize LayerName & LineTypeName & Color
	color = pEntityHeader->Color;

	Layer.Objhandle = pEntityHeader->LayerObjhandle;
	if(drwFindTableType_Direct(pDrawing, TAB_LAYER, FIND_BYHANDLE, &Layer)>0)
	{
		if((pEntityHeader->Color==COLOR_BYBLOCK) || (pEntityHeader->Color==COLOR_BYLAYER))
			color = Layer.Color;
		else if (Layer.Color<0)
		{
			return;
		}
	}
	else if(color>=COLOR_BYLAYER)
		color = 0;


	rgbcolor = CadColor[color];

	pen = CreatePen(PS_SOLID, 0, rgbcolor);
	oldpen = (HPEN)SelectObject(hdc, pen);


//	LType.Objhandle = pEntityHeader->LTypeObjhandle;
//	drwFindTableType(hDrawing, TAB_LTYPE, FIND_BYHANDLE, &LType);

	// Plot the Text in DC
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	strcpy(logfont.lfFaceName, "Times New Roman");
	logfont.lfWeight = FW_DONTCARE;
	logfont.lfOutPrecision = OUT_TT_PRECIS;
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfQuality = NONANTIALIASED_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	logfont.lfHeight = -MulDiv(int(pText->TextData.Height*yScale*pDrawing->View.PPU*pDrawing->View.ZoomLevel), GetDeviceCaps(hdc, LOGPIXELSY), 72);
	logfont.lfEscapement = int(pText->TextData.RotationAngle*10);
	logfont.lfOrientation = logfont.lfEscapement;

	// Find Text Style
	Style.Objhandle = pText->TextData.TextStyleObjhandle;
	if(drwFindTableType_Direct(pDrawing, TAB_STYLE, FIND_BYHANDLE, &Style)>0)
	{
		if(pText->TextData.Height<=0)
			logfont.lfHeight = -MulDiv(int(Style.LastHeightUsed*yScale*pDrawing->View.PPU*pDrawing->View.ZoomLevel), GetDeviceCaps(hdc, LOGPIXELSY), 72);
	}

	if(abs(logfont.lfHeight)>1) 
	{
		logfont.lfWidth = int(pText->TextData.WidthFactor * abs(logfont.lfHeight)/2);

		font = CreateFontIndirect(&logfont);
		oldfont = (HFONT)SelectObject(hdc, font);

		SetTextColor(hdc, rgbcolor);
		SetBkMode(hdc, TRANSPARENT);

		// Decode the Text to windows text format
		char strDecodedText[2048];
		UINT DecodedTextLen;
		DecodedTextLen = DecodeText(pText->strText, strDecodedText);

		RECT rect;
		SIZE size;
		UINT uFormat = 0;

		// Calculate text rect and format
		GetTextExtentPoint32(hdc, strDecodedText, DecodedTextLen, &size);
		switch(pText->TextData.Justification)
		{
		case TJ_CENTER:
			uFormat |= TA_BOTTOM | TA_CENTER;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
							+(size.cy*0.2)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
							  +(size.cy*0.2)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		case TJ_RIGHT:
			uFormat |= TA_TOP | TA_LEFT;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
						- (size.cx)*cos(pText->TextData.RotationAngle*PI/180)
						- (size.cy*0.8)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
						+ (size.cx)*sin(pText->TextData.RotationAngle*PI/180)
						- (size.cy*0.8)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		case TJ_MIDDLE:
			uFormat |= TA_TOP | TA_LEFT;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
						- (size.cx/2)*cos(pText->TextData.RotationAngle*PI/180)
						- (size.cy/2)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
						+ (size.cx/2)*sin(pText->TextData.RotationAngle*PI/180)
						- (size.cy/2)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		case TJ_BOTTOMLEFT:
			uFormat |= TA_BOTTOM | TA_LEFT;
			rect.left = U2PX(pText->TextData.SecondAlignmentPoint.x*xScale);
			rect.bottom = U2PY(pText->TextData.SecondAlignmentPoint.y*yScale);
			break;
		case TJ_BOTTOMCENTER:
			uFormat |= TA_BOTTOM | TA_CENTER;
			rect.left = U2PX(pText->TextData.SecondAlignmentPoint.x*xScale);
			rect.bottom = U2PY(pText->TextData.SecondAlignmentPoint.y*yScale);
			break;
		case TJ_BOTTOMRIGHT:
			uFormat |= TA_BOTTOM | TA_RIGHT;
			rect.left = U2PX(pText->TextData.SecondAlignmentPoint.x*xScale);
			rect.bottom = U2PY(pText->TextData.SecondAlignmentPoint.y*yScale);
			break;
		case TJ_MIDDLELEFT:
			uFormat |= TA_TOP | TA_LEFT;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
							-(size.cy/2)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
							  -(size.cy/2)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		case TJ_MIDDLECENTER:
			uFormat |= TA_TOP | TA_CENTER;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
							-(size.cy/2)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
							  -(size.cy/2)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		case TJ_MIDDLERIGHT:
			uFormat |= TA_TOP | TA_RIGHT;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
							-(size.cy/2)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
							  -(size.cy/2)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		case TJ_TOPLEFT:
			uFormat |= TA_TOP | TA_LEFT;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
							-(size.cy*0.17)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
							  -(size.cy*0.17)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		case TJ_TOPCENTER:
			uFormat |= TA_TOP | TA_CENTER;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
							-(size.cy*0.17)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
							  -(size.cy*0.17)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		case TJ_TOPRIGHT:
			uFormat |= TA_TOP | TA_RIGHT;
			rect.left = int(U2PX(pText->TextData.SecondAlignmentPoint.x*xScale)
							-(size.cy*0.17)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->TextData.SecondAlignmentPoint.y*yScale)
							  -(size.cy*0.17)*cos(pText->TextData.RotationAngle*PI/180));
			break;
		default:
			uFormat |= TA_BOTTOM | TA_LEFT;
			rect.left = int(U2PX(pText->Point0.x)+(size.cy*0.2)*sin(pText->TextData.RotationAngle*PI/180));
			rect.bottom = int(U2PY(pText->Point0.y)+(size.cy*0.2)*cos(pText->TextData.RotationAngle*PI/180));
		}
		rect.right = rect.left;
		rect.top = rect.bottom;

		// Draw Text
//		if(isTextInViewableArea(&pDrawing->View, &rect))
		{
			SetTextAlign(hdc, uFormat);
			TextOut(
				hdc,
				rect.left,
				rect.top,
				strDecodedText,
				DecodedTextLen
			);
		}

		SelectObject(hdc, oldfont);
		DeleteObject(font);
	}

	SelectObject(hdc, oldpen);
	DeleteObject(pen);
}


