/***************************************************************************
 * File...... SymbolLookup.ipp
 * Author.... Mat
 * Date...... 10/13/98
 *
 * Inline functions for SymbolLookup and SymbolLookupSet
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/


inline const IString & SymbolLookup::name() const
{
  return _name;
}


inline Boolean SymbolLookup::isMacro() const
{
  return ( _id == Symbol::unknown );
}


inline const IString & SymbolLookup::macro() const
{
  return _macro;
}


inline Symbol::Identifier SymbolLookup:: id() const
{
  return _id;
}


inline const IString & key( const SymbolLookup & lookup )
{
  return lookup.name();
}


