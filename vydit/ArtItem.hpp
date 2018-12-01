/***************************************************************************
 * File...... ArtItem.hpp
 * Author.... Mat
 * Date...... 9/28/99
 *
 * Represents a piece of bitmap art.  The bitmaps are actually held by the
 * parent FlowItem.  This Item holds an id so it can get the bitmap from
 * the Flow.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef ARTITEM_HPP
#define ARTITEM_HPP


// OpenClass
#include <istring.hpp>

// TextEditor
#include "Item.hpp"


class _Export ArtItem: public Item
{
  public:
    // constructor
    ArtItem( Item * parent, const IString & id );

    // accessors
    const IGBitmap * bitmap() const;
    const IString & id() const;
	
    // setter
    void setId( const IString & id );

    // from Item
    virtual INotificationId editPropertiesId() const;
    virtual ArtItem * asArt();
    virtual View *    newView( View * parent );
    virtual void      generate();
    virtual IString   dumpString() const;

  private:
    IString _id;
};


// inline functions
#include "ArtItem.ipp"


#endif

