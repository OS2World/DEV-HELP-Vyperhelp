/***************************************************************************
 * File...... Identifier.hpp
 * Author.... Mat
 * Date...... 12/23/95
 *
 * An Identifier holds symbolic name along with an integer value and an
 * integer "group" number for equivalencies.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

// OpenClass
#include <istring.hpp>
#include <iptr.h>
#include <ikeyset.h>


/***************************************************************************
 * Class...... Identifier
 * Author..... Mat
 * Date....... 12/23/95
 *
 * A simple structure which holds a symbolic name along with an integer
 * value, and a "group" number for stating equivalencies.
 ***************************************************************************/
class _Export Identifier
{
  public:
    Identifier( const IString & aName, int aValue, int aGroup );
    Identifier();

    IString name;
    int     value;
    int     group;  // for associating aliases
    int     count;  // use count
};

typedef IAutoElemPointer< Identifier > IdentifierPtr;


/***************************************************************************
 * Class...... IdentifierSet
 * Author..... Mat
 * Date....... 12/23/95
 *
 * IdentifierSet is a keyed set of Identifiers.  The key field is the
 * "name" element of the Identifier.
 ***************************************************************************/
class _Export IdentifierSet: public IKeySet< IdentifierPtr, IString >
{
  public:
    // constructor
    IdentifierSet( INumber numberOfElements = 100 );

    // set an ID value (add if necessary)
    Boolean setId( const IString & name, int value );

    // associate two IDs (ensure same group number, add if necessary)
    Boolean associateIds( const IString & name1, const IString & name2 );

    // count IDs in a group
    int     numberInGroup( int group ) const;

    // get the nth ID from a group
    Boolean locateGroupMember( int group, int index, Cursor & cursor ) const;

  private:
    void mergeGroups( const Cursor & cursor1, const Cursor & cursor2 );
    int _group;  // for assigning unique group numbers
};


// global key access function
const IString & _Export key ( const Identifier & id );


#endif

