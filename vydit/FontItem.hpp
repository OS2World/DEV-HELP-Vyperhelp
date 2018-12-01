/***************************************************************************
 * File...... FontItem.hpp
 * Author.... Mat
 * Date...... 2/4/97
 *
 * FontItem represents a change in font, using the IFont
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef FONTITEM_HPP
#define FONTITEM_HPP


// OpenClass
#include <istring.hpp>

// TextEditor
#include "FontInfo.hpp"
#include "Item.hpp"


class _Export FontItem: public Item
{
  public:
    // constructor
    FontItem( Item * parent, const FontInfo & info );

    // accessor
    const IString & name() const;
    unsigned long   pointSize() const;

    // setter
    void setInfo( const FontInfo & info );

    // from Item
    virtual INotificationId editPropertiesId() const;
    virtual View *     newView( View * parent );
    virtual void       generate();
    virtual IString    dumpString() const;

  private:
    FontInfo _info;  // attributes are ignored
};


// inline functions
#include "FontItem.ipp"


#endif

