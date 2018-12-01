/***************************************************************************
 * File...... HtmlTag.cpp
 *
 * Implementation for HtmlTag and HtmlTagStack
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

#include "HtmlTag.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <ilnseq.c>

// HtmlTagStack
#pragma define(IGLinkedSequenceNode<HtmlTag,IStdOps<HtmlTag> >)
#pragma define(IGLnSqCrs<HtmlTag,IStdOps<HtmlTag> >)
#pragma define(ILinkedSequence<HtmlTag>)
#pragma define(IGLnSq<HtmlTag,IStdOps<HtmlTag> >)


HtmlTagStack::HtmlTagStack(INumber numberOfElements):
  ISequence<HtmlTag>(numberOfElements)
{}


HtmlTag::HtmlTag(const IString & tag, Boolean isBlock, int flags):
  _tag(tag),
  _isBlock(isBlock),
  _flags(flags)
{}


/***************************************************************************
 * Procedure.. HtmlTag::end()
 *
 * Strip all attributes from the tag and return the base tag name with a
 * "/" prepended.
 ***************************************************************************/
IString HtmlTag::end() const
{
  return IString("/") + plain();
}


/***************************************************************************
 * Procedure.. HtmlTag::plain()
 *
 * Returns plain tag with no attributes. Forces lower case.
 ***************************************************************************/
IString HtmlTag::plain() const
{
  return _tag.word(1).lowerCase();
}


int HtmlTag::operator==(const HtmlTag & tag) const
{
  return (_tag == tag._tag);
}


int HtmlTag::operator!=(const HtmlTag & tag) const
{
  return (_tag != tag._tag);
}


