/***************************************************************************
 * File...... ParagraphItem.hpp
 * Author.... Mat
 * Date...... 9/5/96
 *
 * ParagraphItem represents the start of a new paragraph.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef PARAGRAPHITEM_HPP
#define PARAGRAPHITEM_HPP


// TextEditor
#include "Item.hpp"


class _Export ParagraphItem: public Item
{
  public:
    // constructor
    ParagraphItem( Item * parent );

    // from Item
    virtual View *  newView( View * parent );
    virtual void    generate();
    virtual IString dumpString() const;
};



#endif

