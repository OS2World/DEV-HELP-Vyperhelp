/***************************************************************************
 * File...... HtmlLabel.cpp
 *
 * Implementation for HtmlLabel and HtmlLabelSet
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

#include "HtmlLabel.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <iavlkss.c>
#include <ikskss.c>

// HtmlLabelSet
#pragma define(IGAvlKeySortedSetNode<HtmlLabel,IString,IKCOps<HtmlLabel,IString> >)
#pragma define(IGAvlKeySortedSetCursor<HtmlLabel,IString,IKCOps<HtmlLabel,IString> >)
#pragma define(IGAvlKSS<HtmlLabel,IString,IKCOps<HtmlLabel,IString> >)
#pragma define(IWKeySetOnKSSetCursor<HtmlLabel,IString,IKCOps<HtmlLabel,IString>,IGAvlKSS<HtmlLabel,IString,IKCOps<HtmlLabel,IString> > >)
#pragma define(IWKSOnKSS<HtmlLabel,IString,IKCOps<HtmlLabel,IString>,IGAvlKSS<HtmlLabel,IString,IKCOps<HtmlLabel,IString> > >)


HtmlLabel::HtmlLabel(const IString & name, int fileNum):
  _name(name),
  _fileNum(fileNum)
{}


HtmlLabelSet::HtmlLabelSet(INumber numberOfElements):
  IKeySet<HtmlLabel, IString>(numberOfElements)
{}


HtmlLabelSet::~HtmlLabelSet()
{}


const IString & key(const HtmlLabel & label)
{
  return label._name;
}


