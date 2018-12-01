/***************************************************************************
 * File...... FontDefinition.ipp
 * Author.... Mat
 * Date...... 5/31/99
 *
 * Inline functions for FontDefinition
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


inline const IString & FontDefinition::name() const
{
  return _name;
}


inline const IString & FontDefinition::family() const
{
  return _family;
}


inline Boolean FontDefinition::isVector() const
{
  return _isVector;
}


inline Boolean FontDefinition::isFixed() const
{
  return _isFixed;
}


