/***************************************************************************
 * File...... HtmlContext.hpp
 *
 * HtmlContext holds the current state of the HTML generator
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/
#ifndef HTMLCONTEXT_HPP
#define HTMLCONTEXT_HPP

// OpenClass
#include <istring.hpp>
#include <istack.h>
#include <iqueue.h>

// Generator
#include "CodePage.hpp"


// enumerators
enum HtmlFont
{
  fontSmall,    // < 10 pt
  fontRegular,
  fontLarge     // > 12 pt
};

enum HtmlAlign {
  alignLeft,
  alignRight,
  alignCenter
};

enum HtmlList {
  listSimple,
  listUnordered,
  listOrdered
};


typedef IQueue< int > WidthList;


class HtmlContext {

  public:
    // -- Constructor --
    HtmlContext();

    // -- Paragraph Styles --
    int         spacingLines;
    HtmlAlign   align;
    HtmlList    list;
    Boolean     hasItem;
    HtmlMode    mode;
    int         level;              // nesting level
    int         minLevel;
    int         maxLevel;
    Boolean     forceModeChange;    // true to force a context switch
    WidthList   columnPixels;       // holds column widths
    Boolean     hasBorder;

    // -- Character Styles --
    Boolean     isBold;
    Boolean     isItalic;
    Boolean     isUnderline;
    Boolean     isComment;
    IColor      color;
    HtmlFont    fontSize;
    Boolean     isMonospaced;
    IString     linkTarget;         // empty = no target
    IString     linkTargetFile;     // empty = not external

    // -- Input Characteristics --
    CodePage    inputCodepage;      // 0 = active codepage
};


class HtmlContextStack: public IStack<HtmlContext>
{
  public:
    HtmlContextStack(INumber numberOfElements = 20);
};


#endif

