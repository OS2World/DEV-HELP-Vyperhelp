/***************************************************************************
 * File...... IPF_Index.hpp
 * Author.... Mat
 * Date...... 10/10/95
 *
 * Defines an IPF index entry.  Entries, along with their ID number are
 * stored to ensure that a single primary entry is only used once.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#ifndef IPF_INDEX_HPP
#define IPF_INDEX_HPP

// OpenClass
#include <istring.hpp>      // IString
#include <ikeyset.h>        // IKeySet
#include <iptr.h>           // IAutoElemPointer

// Forward declarations
class IndexSet;


/***************************************************************************
 * Class...... IndexEntry
 * Author..... Mat
 * Date....... 10/10/95
 *
 * Index entry holds a single primary index entry along with a numerical ID.
 ***************************************************************************/
class IndexEntry
{
  public:
    // constructor
    IndexEntry(int id, const IString & word);
    ~IndexEntry();

    // accessors
    int             id() const;
    const IString & word() const;
    IndexSet      & subindex() const;

  private:
    int        _id;
    IString    _word;
    IndexSet * _subindex;
};


/***************************************************************************
 * Class...... IndexSet
 * Author..... Mat
 * Date....... 10/10/95
 *
 * Index set is a keyed set of IndexEntries.  The key field is the
 * "word" element of the IndexEntry.
 ***************************************************************************/
typedef IAutoElemPointer< IndexEntry > IndexPtr;
class IndexSet: public IKeySet< IndexPtr, IString >
{
  public:
    IndexSet( INumber numberOfElements = 100 );
};


// global key access function
const IString & key ( const IndexEntry & entry );


#endif

