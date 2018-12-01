/***************************************************************************
 * File...... SymbolLookup.hpp
 * Author.... Mat
 * Date...... 10/13/98
 *
 * Defines a class and collection used for maintaining a symbol lookup
 * table to support IPF symbols (built-in and nameit-defined).
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef SYMBOLLOOKUP_HPP
#define SYMBOLLOOKUP_HPP

// OpenClass
#include <istring.hpp>      // IString
#include <ikeyset.h>        // IKeySet

// Generator
#include "Symbol.hpp"


class SymbolLookup
{
  public:
    // constructors
    SymbolLookup( const IString & name, Symbol::Identifier id );
    SymbolLookup( const IString & name, const IString & macro );

    // query functions
    const IString &    name() const;
    Boolean            isMacro() const;
    const IString &    macro() const;
    Symbol::Identifier id() const;

  private:
    IString            _name;
    Symbol::Identifier _id;
    IString            _macro;  // for nameit symbols (_id = Symbol::unknown)
};


// collection
class SymbolLookupSet: public IKeySet< SymbolLookup, IString >
{
  public:
    // constructor
    SymbolLookupSet( INumber numberOfElements = 250 );
};


// key function
const IString & key( const SymbolLookup & lookup );


// inline functions
#include "SymbolLookup.ipp"


#endif

