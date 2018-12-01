/***************************************************************************
 * File...... Symbol.ipp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Inline functions for Symbol
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline Symbol::Identifier Symbol::id() const
{
  return _ch? unknown: _id;
}


