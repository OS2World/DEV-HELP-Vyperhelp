/***************************************************************************
 * File...... RtfGeneratorContext.hpp
 * Author.... Mat
 * Date...... 2/18/99
 *
 * Context information for RtfGenerator.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef RTFGENERATORCONTEXT_HPP
#define RTFGENERATORCONTEXT_HPP


// OpenClass
#include <istack.h>         // IStack

// Generator
#include "Gin.hpp"

// constants
#define RTF_CODEPAGE  1004    // ANSI character set


class RtfGeneratorContext
{
  public:
    // helpers
    void reset( Boolean all );
    Boolean changedFrom( const RtfGeneratorContext & other ) const;

    // public data members
    unsigned            tableNested;      // level of nesting for table (0 if no table active)
    unsigned            borders;          // see BorderGin::Borders flags
    int                 leftMargin;       // current margins (twips)
    int                 rightMargin;
    int                 tab;              // tab position for lists (twips)
    int                 baseLeftMargin;   // base margins and tab position, saved at Push (twips)
    int                 baseRightMargin;
    int                 baseTab;
    int                 spacing;          // spacing after paragraph (twips)
    LinkGin             link;
    Boolean             isComment;
    CodePage            cp;               // active RTF codepage
    RulerGin::Mode      mode;
    RulerGin::LineBreak wrap;
    int                 itemNumber;
    OutlineGin::Style   itemStyle;
    AlignGin::Alignment alignment;
};


class RtfGeneratorContextStack: public IStack< RtfGeneratorContext >
{
  public:
    // constructor
      RtfGeneratorContextStack( INumber numberOfElements = 15 );
};

#endif

