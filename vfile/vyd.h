/***************************************************************************
 * File...... vyd.h
 * Author.... Mat
 * Date...... 11/20/97
 *
 * Definitions for VYD file support
 *
 * History of file versions:
 * 1. original file versions (1.00)
 * 2. added CodePage setting (1.02)
 * 3. added codepage to SymbolGin storage (1.03)
 * 4. removed x,y size from ArtGin, added 'fit' to ArtSpot enumeration
 * 5. changed ArtGin id from long to string (1.06)
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef VYD_H
#define VYD_H


#define VYD_SIGNATURE 0x5956  // "VY"
#define VYD_VERSION   5

// offsets for links in file header
#define VYD_SECTION_OFFSET     4   // link to first section
#define VYD_DEFINITION_OFFSET  8   // link to definitions


enum GinType
{
  ginNone,
  ginKeyword,
  ginLabel,
  ginRow,
  ginColumn,
  ginItem,
  ginPara,
  ginLine,
  ginPush,
  ginPop,
  ginRightMargin,
  ginLeftMargin,
  ginBorder,
  ginAlign,
  ginSpacing,
  ginRuler,
  ginOutline,
  ginDivider,
  ginFont,
  ginSize,
  ginColor,
  ginBackColor,
  ginBold,
  ginItalic,
  ginUnderline,
  ginComment,
  ginLink,
  ginHide,
  ginArt,
  ginText,
  ginSymbol
};


#endif

