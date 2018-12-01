/***************************************************************************
 * File...... WinHelpParser.ipp
 * Author.... Mat
 * Date...... 12/22/98
 *
 * Inline functions for WinHelpParser
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

inline WinHelpParser & WinHelpParser::setActiveFile( const Filename & filename )
{
  _activeFile = filename.name();
  return *this;
}


inline WinHelpParser & WinHelpParser::setTitle( const IString & title )
{
  _title = title;
  return *this;
}


inline WinHelpParser & WinHelpParser::setContents( const IString & contextName )
{
  _contentsContext = contextName;
  return *this;
}


inline char WinHelpParser::separator() const
{
  return _separator;
}


inline WinHelpParser & WinHelpParser::setForceFont( const IString & forceFont )
{
  _forceFont = forceFont;
  return *this;
}


inline const IString & WinHelpParser::forceFont() const
{
  return _forceFont;
}


