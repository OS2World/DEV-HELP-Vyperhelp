/***************************************************************************
 * File...... FontDefinition.hpp
 * Author.... Mat
 * Date...... 5/31/99
 *
 * This class holds static information about a given font facename.  It is
 * used instead of IFont to speed up access to this information.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef FONTDEFINITION_HPP
#define FONTDEFINITION_HPP


// OpenClass
#include <istring.hpp>      // IString
#include <ihshks.h>         // IHashKeySet



class FontDefinition;
typedef IHashKeySet< FontDefinition, IString > FontDefinitionSet;


class FontDefinition
{
  public:
    // constructor
    FontDefinition( const IString & name );

    // search
    static const FontDefinition * locate( const IString & name );

    // information
    const IString & name() const;
    const IString & family() const;
    Boolean isVector() const;
    Boolean isFixed() const;

  private:
    // static collection
    static FontDefinitionSet _set;

    // font information
    IString _name;
    IString _family;
    Boolean _isVector;
    Boolean _isFixed;
};


// global key access function
const IString & key( const FontDefinition & fd );


// inline functions
#include "FontDefinition.ipp"


#endif

