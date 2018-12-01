/***************************************************************************
 * File...... PanelLink.ipp
 * Author.... Mat
 * Date...... 10/29/98
 *
 * Inline functions for PanelLink
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/


inline const IString & PanelLink::name() const
{
  return _name;
}


inline const int & PanelLink::id() const
{
  return _id;
}


inline const int & key( const PanelLink & link )
{
  return link.id();
}

