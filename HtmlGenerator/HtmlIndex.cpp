/***************************************************************************
 * File...... HtmlIndex.cpp
 *
 * Implementation for HtmlIndex and HtmlIndexSet
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

#include "HtmlIndex.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <ilnseq.c>

#pragma define(IGLnSqCrs<HtmlIndex,IStdOps<HtmlIndex> >)
#pragma define(IGLinkedSequenceNode<HtmlIndex,IStdOps<HtmlIndex> >)
#pragma define(ILinkedSequence<HtmlIndex>)
#pragma define(IGLnSq<HtmlIndex,IStdOps<HtmlIndex> >)


HtmlIndex::HtmlIndex(const IString & phrase, int fileNum, int labelNum):
  _phrase(phrase),
  _fileNum(fileNum),
  _labelNum(labelNum)
{}


HtmlIndexSet::HtmlIndexSet(INumber numberOfElements):
  ISequence<HtmlIndex>(numberOfElements)
{}


HtmlIndexSet::~HtmlIndexSet()
{}

// case-insensitive ordering
// NOTE: I tried to use ISortedBag as an ordered collection but it had problems with
//       elements which have the same key AND it required special ordering operators
long sort(const HtmlIndex & index1, const HtmlIndex & index2)
{
  return stricmp(index1._phrase, index2._phrase);
}


