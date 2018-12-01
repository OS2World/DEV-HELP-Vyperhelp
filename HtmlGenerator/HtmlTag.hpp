/***************************************************************************
 * File...... HtmlTag.hpp
 *
 * Defines an HTML tag and information about it.  An HtmlTagStack is also
 * defined. It is used to maintain a context of active tags so that proper
 * nexting rules can be enforced.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/
#ifndef HTMLTAG_HPP
#define HTMLTAG_HPP

// OpenClass
#include <istring.hpp>
#include <iseq.h>


class HtmlTag
{
  public:
    // constructor
    HtmlTag(const IString & tag, Boolean isBlock, int flags = 0);

    // equality
    int operator==(const HtmlTag & tag) const;
    int operator!=(const HtmlTag & tag) const;

    // strings
    IString start() const;
    IString end() const;
    IString plain() const;

    // send flags
    int flags() const;

    // tag type
    Boolean isBlock() const;

  private:
    IString _tag;
    Boolean _isBlock;
    int     _flags;
};


class HtmlTagStack: public ISequence<HtmlTag>
{
  public:
    HtmlTagStack( INumber numberOfElements = 20 );
};


// inline functions
#include "HtmlTag.ipp"


#endif

