/***************************************************************************
 * File...... LinkItem.hpp
 * Author.... Mat
 *
 * LinkItem represents a start or end of a hypertext link.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef LINKITEM_HPP
#define LINKITEM_HPP


// OpenClass
#include <istring.hpp>

// Generator
#include "Generator.hpp"

// Editor
#include "Item.hpp"


class _Export LinkItem: public Item
{
  public:
    // constructor
    LinkItem( Item * parent, const LinkGin & gin );

    // accessor
    const LinkGin & gin() const;

    // setter
    void setGin( const LinkGin & gin );

    // from Item
    virtual INotificationId editPropertiesId() const;
    virtual View *     newView( View * parent );
    virtual void       generate();
    virtual IString    dumpString() const;

  private:
    LinkGin _gin;
};


// inline functions
#include "LinkItem.ipp"


#endif

