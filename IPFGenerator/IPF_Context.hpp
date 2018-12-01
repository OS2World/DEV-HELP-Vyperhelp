/***************************************************************************
 * File...... IPF_Context.hpp
 * Author.... Mat
 * Date...... 10/24/95
 *
 * IPF context holds the current state of the IPF generator
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef IPF_CONTEXT_HPP
#define IPF_CONTEXT_HPP

// OpenClass
#include <istring.hpp>
#include <istack.h>
#include <iqueue.h>

// Generator
#include "CodePage.hpp"


// enumerators
enum IPFFont {
  fontCourier,
  fontTimesRoman,
  fontHelvetica,
  fontDefault
};

enum IPFColor {
  colorDefault,
  colorBlue,
  colorCyan,
  colorGreen,
  colorNeutral,
  colorRed,
  colorYellow,
  colorBlack,   // these were added with IPFC 3.x
  colorPink,
  colorBrown,
  colorDarkGray,
  colorDarkBlue,
  colorDarkRed,
  colorDarkPink,
  colorDarkGreen,
  colorDarkCyan,
  colorPaleGray
};

enum IPFAlign {
  alignLeft,
  alignRight,
  alignCenter
};

enum IPFList {
  listSimple,
  listUnordered,
  listOrdered
};

enum IPFMode {
  paragraph,
  singleList,       // :sl :ul :ol
  lines,            // :lines
  doubleList,       // :dl
  doubleListBreak,  // :dl break=all
  doubleListFit,    // :dl break=fit
  table             // :table
};


typedef IQueue< int > WidthList;


class IPFContext {

  public:
    // -- Constructor --
    IPFContext();

    // -- Paragraph Styles --
    int         leftMarginChars;
    int         rightMarginChars;
    int         spacingLines;
    IPFAlign    align;
    IPFList     list;
    IPFMode     mode;
    int         level;              // nesting level
    int         minLevel;
    int         maxLevel;
    Boolean     forceModeChange;    // true to force a context switch
    WidthList   columnChars;        // holds column widths
    int         borders;

    // -- Character Styles --
    Boolean     isBold;
    Boolean     isItalic;
    Boolean     isUnderline;
    Boolean     isComment;
    IPFColor    foregroundColor;
    IPFColor    backgroundColor;
    IPFFont     font;
    int         pointSize;          // 0 = default size
    CodePage    outputCodepage;     // 0 = active codepage
    IString     key;                // empty = no key
    IString     linkTarget;         // empty = no target
    IString     linkTargetFile;     // empty = not external

    // -- Input Characteristics --
    CodePage    inputCodepage;      // 0 = active codepage
};


class ContextStack: public IStack< IPFContext >
{
  public:
    ContextStack( INumber numberOfElements = 20 );
};


#endif

