/***************************************************************************
 * File...... View.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for View
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline View * View::parent() const
{
  return _parent;
}


inline void View::setParent( View * view )
{
  _parent = view;
}


inline int View::orderViews( View & view1, View & view2 )
{
  return 0;
}


inline Boolean View::isLine() const
{
  return false;
}


inline Boolean View::isTransient() const
{
  return false;
}


inline Boolean View::isOrphan() const
{
  return false;
}


inline void View::removeChild( View * view )
{}


inline void View::insertItem( Item * item )
{}


inline IPoint View::position() const
{
  return IPoint( 0, 0 );
}


inline Coord View::width() const
{
  return 0;
}


inline Coord View::height() const
{
  return 0;
}


inline Coord View::descent() const
{
  return 0;
}


inline void View::generate( View * from, View * to, unsigned & level )
{}


