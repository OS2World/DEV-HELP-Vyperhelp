/***************************************************************************
 * File...... distance.ipp
 * Author.... Mat
 * Date...... 3/12/99
 *
 * Inline functions for Distance and Position classes
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

inline int   Distance::length() const
{ return _length; }

inline Distance::Units Distance::units() const
{ return _units; }

inline Boolean  Position::isDynamic() const
{ return _isDynamic; }

inline Position::Dynamic  Position::asDynamic() const
{ return _dynamic; }

inline const Distance & Position::asDistance() const
{ return _distance; }

