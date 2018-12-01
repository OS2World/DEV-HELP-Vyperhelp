/***************************************************************************
 * File...... EditorCursor.ipp
 * Author.... Mat
 * Date...... 10/16/97
 *
 * Inline functions for EditorCursor
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/


inline ViewCursor * EditorCursor::cursor()
{
  return _cursor;
}


inline Boolean EditorCursor::isSet() const
{
  return ( _cursor != 0 );
}


