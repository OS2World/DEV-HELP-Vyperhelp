/***************************************************************************
 * File...... HtmlLabel.hpp
 *
 * Holds information about which file each label is found in.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/
#ifndef HTMLLABEL_HPP
#define HTMLLABEL_HPP


#include <ikeyset.h>
#include <istring.hpp>


class HtmlLabel
{
  public:
    HtmlLabel(const IString & name, int fileNum);

    IString _name;
    int     _fileNum;
};



class HtmlLabelSet: public IKeySet<HtmlLabel, IString>
{
  public:
    HtmlLabelSet(INumber numberOfElements = 100);
    virtual ~HtmlLabelSet();
};


// key function
const IString & key(const HtmlLabel & label);


#endif

