/***************************************************************************
 * File...... ParserIndicator.ipp
 * Author.... Mat
 * Date...... 5/11/99
 *
 * Inline functions for ParserIndicator.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

inline void ParserIndicator::setTask( const IString & task )
{}

inline void ParserIndicator::setStatus( const IString & status )
{}

inline void ParserIndicator::warning( const IString & text )
{}

inline void ParserIndicator::error( const IString & text )
{}

inline unsigned ParserIndicator::percentDone() const
{
  return _percent;
}


