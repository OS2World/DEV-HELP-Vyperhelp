/***************************************************************************
 * File...... Keyword.cpp
 * Author.... Mat
 * Date...... 10/6/95
 *
 * Support for keywords (index entries) in IPF.  IPF allows dual level
 * entries, and that is achieved by keeping a set of all first level
 * entries with their IDs.  The ID is then used as a reference ID (refid)
 * for the second level entry.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "IPF_Common.hpp"


//
// INDEX ENTRIES -----------------------------------------------------------
//

//
// TEMPLATE DEFINITIONS
//
#include <iavlkss.c>
#include <ikskss.c>

// IndexSet
#pragma define(IGAvlKeySortedSetNode<IAutoElemPointer<IndexEntry>,IString,IKCOps<IAutoElemPointer<IndexEntry>,IString> >)
#pragma define(IGAvlKeySortedSetCursor<IAutoElemPointer<IndexEntry>,IString,IKCOps<IAutoElemPointer<IndexEntry>,IString> >)
#pragma define(IGAvlKSS<IAutoElemPointer<IndexEntry>,IString,IKCOps<IAutoElemPointer<IndexEntry>,IString> >)
#pragma define(IWKeySetOnKSSetCursor<IAutoElemPointer<IndexEntry>,IString,IKCOps<IAutoElemPointer<IndexEntry>,IString>,IGAvlKSS<IAutoElemPointer<IndexEntry>,IString,IKCOps<IAutoElemPointer<IndexEntry>,IString> > >)
#pragma define(IWKSOnKSS<IAutoElemPointer<IndexEntry>,IString,IKCOps<IAutoElemPointer<IndexEntry>,IString>,IGAvlKSS<IAutoElemPointer<IndexEntry>,IString,IKCOps<IAutoElemPointer<IndexEntry>,IString> > >)


// Constructor
IndexEntry::IndexEntry(int id, const IString & word) :
    _id( id ),
    _word( word )
{
  _subindex = new IndexSet;
}

IndexEntry::~IndexEntry()
{
  delete _subindex;
}


// Accessors
int IndexEntry::id() const
{
  return _id;
}

const IString & IndexEntry::word() const
{
  return _word;
}

IndexSet & IndexEntry::subindex() const
{
   return *_subindex;
}


//
// INDEX SET ---------------------------------------------------------------
//

// Constructor
IndexSet::IndexSet( INumber numberOfElements ):
    IKeySet< IndexPtr, IString >( numberOfElements )
{}

// global IString key function for IndexSet collection
const IString & key ( const IndexEntry & entry )
{
  return entry.word();
}



//
// INDEX TAGS --------------------------------------------------------------
//

/***************************************************************************
 * Procedure.. tag_i1
 * Author..... Mat
 * Date....... 10/6/95
 *
 * Sends an :i1 tag, using the next available ID.  Returns the ID which
 * was obtained.
 ***************************************************************************/
int IPFGenerator::tag_i1( const IString & text, Boolean isGlobal )
{
  // get next available ID
  int id = nextId();

  // build tag output
  IString tag = "i1 id=" + IString( id );
  if (isGlobal)
    tag += " global";
  sendTag( tag, breakBefore | noWrap | isNotContent );

  // output text
  sendText( text, noWrap | breakAfter | isNotContent | noSymbolCheck );

  // return the ID
  return id;
}


/***************************************************************************
 * Procedure.. tag_i2
 * Author..... Mat
 * Date....... 10/6/95
 *
 * Sends an :i2 (second level index) tag.  The refid given is the id of
 * the associated level 1 entry.
 ***************************************************************************/
void IPFGenerator::tag_i2( const IString & text, int refid, Boolean isGlobal )
{
  // build tag output
  IString tag = "i2 refid=" + IString( refid );
  if (isGlobal) {
    tag += " global";
  } /* endif */
  sendTag( tag, breakBefore | noWrap | isNotContent );

  // output text
  sendText( text, noWrap | breakAfter | isNotContent | noSymbolCheck );
}



//
// KEYWORD STREAM OPERATOR
//


/***************************************************************************
 * Procedure.. IPFGenerator::operator<<( const KeywordGin & )
 * Author..... Mat
 * Date....... 10/6/95
 *
 * This << operator accepts a KeywordGin, which translates to one or more
 * IPF index tags.  _index is a table of all level 1 index entries, and
 * is used to ensure that there are no entries with duplicate text.
 ***************************************************************************/
void IPFGenerator::handleKeyword( const KeywordGin      & keyword )
{
  // IPF can't use unlisted keywords
  if ( ! keyword.isListed() )
    return;

  // get translated keyword string
  IString text = translate( keyword.text(), true );

  // see if this entry is already in the index list
  IndexSet::Cursor cursor( *_index );
  _index->locateElementWithKey( text, cursor );

  // add it if not already in list
  if ( ! cursor.isValid() )
  {
    int id = tag_i1( text, keyword.isExternal() );
    IndexPtr ip = IndexPtr( new IndexEntry( id, text ), IINIT );
    _index->add( ip, cursor );
  }

  // use section title (if there is one) as second-level index text
  IString subtext = _section.title();
  if ( subtext.length() )
  {
    // see if it's already been used
    IndexSet & subindex = cursor.element()->subindex();
    IndexSet::Cursor subcursor( subindex );
    subindex.locateElementWithKey( subtext, subcursor );

    // add it if not already in list
    if ( ! subcursor.isValid() )
    {
      int id = cursor.element()->id();
      IndexPtr ip = IndexPtr( new IndexEntry( id, subtext ), IINIT );
      subindex.add( ip, subcursor );
      INumber n = subindex.numberOfElements();

      // defer first sub-index until a second is encountered
      if ( n >= 2 )
        tag_i2( subtext, id, keyword.isExternal() );

      // if this was the second sub-index, find and output the first too!
      if ( n == 2 )
      {
        // this collection is not ordered, so may use this logic to find "other" element
        if ( ! subcursor.setToNext() )
          subcursor.setToFirst();
        tag_i2( subcursor.element()->word(), id, false );
      }
    }
  }
}

