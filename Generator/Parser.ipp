/***************************************************************************
 * File...... Parser.ipp
 * Author.... Mat
 * Date...... 5/11/99
 *
 * Inline functions for Parser
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


inline void Parser::setId( const IString & contextName, int id )
{}


inline void Parser::setIndicator( ParserIndicator * indicator )
{
  _indicator = indicator;
}


inline ParserIndicator * Parser::indicator()
{
  return _indicator;
}

