/***************************************************************************
 * File...... Identifier.cpp
 * Author.... Mat
 * Date...... 12/23/95
 *
 * Implementation for Identifier.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "MekTek.hpp"
#include "Identifier.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <ikskss.c>
#include <iavlkss.c>

// IKeySet< IdentifierPtr, IString >
#pragma define(IWKeySetOnKSSetCursor<IAutoElemPointer<Identifier>,IString,IKCOps<IAutoElemPointer<Identifier>,IString>,IGAvlKSS<IAutoElemPointer<Identifier>,IString,IKCOps<IAutoElemPointer<Identifier>,IString> > >)
#pragma define(IWKSOnKSS<IAutoElemPointer<Identifier>,IString,IKCOps<IAutoElemPointer<Identifier>,IString>,IGAvlKSS<IAutoElemPointer<Identifier>,IString,IKCOps<IAutoElemPointer<Identifier>,IString> > >)
#pragma define(IGAvlKeySortedSetNode<IAutoElemPointer<Identifier>,IString,IKCOps<IAutoElemPointer<Identifier>,IString> >)
#pragma define(IGAvlKeySortedSetCursor<IAutoElemPointer<Identifier>,IString,IKCOps<IAutoElemPointer<Identifier>,IString> >)
#pragma define(IGAvlKSS<IAutoElemPointer<Identifier>,IString,IKCOps<IAutoElemPointer<Identifier>,IString> >)



/***************************************************************************
 * Procedure.. Identifier::Identifier
 * Author..... Mat
 * Date....... 12/26/95
 *
 * Constructors for Identifier, allowing setting of all three elements,
 * or defaulting all to 0.
 ***************************************************************************/
Identifier::Identifier( const IString & aName, int aValue, int aGroup ):
    name( aName ),
    value( aValue ),
    group( aGroup ),
    count( 0 )
{}

Identifier::Identifier():
    value( 0 ),
    group( 1 ),
    count( 0 )
{}



/***************************************************************************
 * Procedure.. IdentifierSet::IdentifierSet
 * Author..... Mat
 * Date....... 12/26/95
 *
 * Constructor, initialize group enumerator to 0.
 ***************************************************************************/
IdentifierSet::IdentifierSet( INumber numberOfElements ):
    IKeySet< IdentifierPtr, IString >( numberOfElements ),
    _group( 0 )
{}


/***************************************************************************
 * Procedure.. IdentifierSet::setId
 * Author..... Mat
 * Date....... 12/26/95
 *
 * If the ID name already exists, set the value, otherwise add the name/value
 * pair with a new (unique) group number.  Return true if an ID was added.
 ***************************************************************************/
Boolean IdentifierSet::setId( const IString & name, int value )
{
  Cursor cursor( *this );
  Boolean found = locateElementWithKey( name, cursor );
  if ( found )
  {
    // set the ID for the found entry
    cursor.element()->value = value;
  }
  else
  {
    // create new ID with a unique group number
    Identifier * id = new Identifier( name, value, _group++ );
    add( IdentifierPtr( id, IINIT ) );
  }
  return ! found;
}


/***************************************************************************
 * Procedure.. IdentifierSet::associateIds
 * Author..... Mat
 * Date....... 12/26/95
 *
 * Associate two IDs by making sure that they have the same group number.
 * If either of the IDs is not in the set, then add it and return true.
 * The initial value for any ID which is added will be 0.
 ***************************************************************************/
Boolean IdentifierSet::associateIds( const IString & name1, const IString & name2 )
{
  Boolean addedId = true;  // assume true for now
  Cursor cursor1( *this ), cursor2( *this );
  Boolean found1 = locateElementWithKey( name1, cursor1 );
  Boolean found2 = locateElementWithKey( name2, cursor2 );
  if ( ! (found1 || found2) )
  {
    // neither was found, add both with a new (unique) group number
    // initial value for both is 0
    Identifier * id = new Identifier( name1, 0, _group );
    add( IdentifierPtr( id, IINIT ) );
    id = new Identifier( name2, 0, _group++ );
    add( IdentifierPtr( id, IINIT ) );
  }
  else if ( found1 && found2 )
  {
    // both were found, need to merge groups
    mergeGroups( cursor1, cursor2 );
    addedId = false;
  }
  else if ( found1 )
  {
    // first was found, use that group number
    Identifier * id = new Identifier( name2, 0, cursor1.element()->group );
    add( IdentifierPtr( id, IINIT ) );
  }
  else
  {
    // second was found, use that group number
    Identifier * id = new Identifier( name1, 0, cursor2.element()->group );
    add( IdentifierPtr( id, IINIT ) );
  }

  return addedId;
}


/***************************************************************************
 * Procedure.. IdentifierSet::numberInGroup
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Returns the number of IDs that match the given group number.
 ***************************************************************************/
int IdentifierSet::numberInGroup( int group ) const
{
  Cursor cursor( *this );
  int count = 0;
  forCursor( cursor )
    if ( cursor.element()->group == group )
      count++;
  return count;
}


/***************************************************************************
 * Procedure.. IdentifierSet::locateGroupMember
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Set the cursor to the nth (index) ID in the specified group.  Return
 * true if found, other wise false is returned and the cursor is invalid.
 ***************************************************************************/
Boolean IdentifierSet::locateGroupMember( int group, int index, Cursor & cursor ) const
{
  if ( ! cursor.setToFirst() )
    return false;  // empty set

  MEKASSERT( index >= 0 && index < numberOfElements() );
  do
  {
    // find next ID in this group
    while ( cursor.element()->group != group )
    {
      if ( ! cursor.setToNext() )
        return false;  // no more IDs
    }

    // need to search for another match
    if ( index )
    {
      if ( ! cursor.setToNext() )
        return false;  // no more IDs
    }
  }
  while ( index-- );

  return true;
}


/***************************************************************************
 * Procedure.. IdentifierSet::mergeGroups
 * Author..... Mat
 * Date....... 12/26/95
 *
 * Re-assign group# for all matching cursor2 to match cursor1
 ***************************************************************************/
void IdentifierSet::mergeGroups( const Cursor & cursor1, const Cursor & cursor2 )
{
  // get group numbers
  int group1 = cursor1.element()->group;
  int group2 = cursor2.element()->group;
  if ( group1 == group2 )
    return;  // already same group number

  // loop through all matching group2 and set to group1
  Cursor cursor( *this );
  forCursor( cursor )
  {
    Identifier * id = cursor.element();
    if ( id->group == group2 )
      id->group = group1;
  }
}


/***************************************************************************
 * Procedure.. key
 * Author..... Mat
 * Date....... 12/26/95
 *
 * key function for IdentifierSet, which gets the ID's name field
 ***************************************************************************/
const IString & key ( const Identifier & id )
{
  return id.name;
}

