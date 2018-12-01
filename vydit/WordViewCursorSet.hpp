/***************************************************************************
 * File...... WordViewCursorSet.hpp
 * Author.... Mat
 * Date...... 9/18/97
 *
 * Defines a collection (set) of WordViewCursors maintained by the WordView.
 * WordView needs to keep a set of all cursors it has created so they can be
 * updated when the index changes.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef WORDVIEWCURSORSET_HPP
#define WORDVIEWCURSORSET_HPP

// OpenClass
#include <iset.h>

// TextEditor
class WordViewCursor;


typedef ISet< WordViewCursor * > WordViewCursorSet;


#endif

