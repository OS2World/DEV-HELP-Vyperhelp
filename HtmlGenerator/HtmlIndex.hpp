/***************************************************************************
 * File...... HtmlIndex.hpp
 *
 * Holds index entries.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/
#ifndef HTMLINDEX_HPP
#define HTMLINDEX_HPP

#include <istring.hpp>
#include <iseq.h>     // ISequence

class HtmlIndex
{
  public:
    HtmlIndex(const IString & phrase, int fileNum, int labelNum);

    IString _phrase;
    int     _fileNum;
    int     _labelNum;
};


class HtmlIndexSet: public ISequence<HtmlIndex>
{
  public:
    HtmlIndexSet(INumber numberOfElements = 100);
    virtual ~HtmlIndexSet();
};


// sort function
long sort(const HtmlIndex & index1, const HtmlIndex & index2);


#endif

