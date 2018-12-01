/***************************************************************************
 * File...... Item.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for Item
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros


inline Item * Item::parent() const
{
  return _parent;
}


inline void Item::setParent( Item * parent )
{
  _parent = parent;
}


// default does nothing, assumes no children
inline void Item::removeChild( Item * item )
{}


inline WordItem * Item::asWord()
{
  return 0;
}


inline FlowItem * Item::asFlow()
{
  return 0;
}


inline PushItem * Item::asPush()
{
  return 0;
}


inline PopItem * Item::asPop()
{
  return 0;
}


inline ArtItem * Item::asArt()
{
  return 0;
}


inline INotificationId Item::editPropertiesId() const
{
  return 0;
}


inline void Item::addObserver( SubjectView & view )
{
  IASSERTSTATE( ! _observer );
  _observer = &view;
}


inline void Item::removeObserver( SubjectView  & view )
{
  _observer = 0;
}

