/***************************************************************************
 * File...... ipfparser.ipp
 * Author.... Mat
 * Date...... 10/20/98
 *
 * Inline functions for IpfParser
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/


inline IpfParser::Mode IpfParser::mode()
{
  return _mode;
}


inline void IpfParser::setMode( IpfParser::Mode mode )
{
  _mode = mode;
}


inline unsigned IpfParser::tocLevel()
{
  return _tocLevel;
}


inline void IpfParser::setTocLevel( unsigned level )
{
  _tocLevel = level;
}


inline int IpfParser::nextWindowId()
{
  return ++_windowCounter;
}


inline void IpfParser::clearLineFlag()
{
  _lineFlag = false;
}


inline void IpfParser::setLineFlag()
{
  _lineFlag = true;
}


inline Boolean IpfParser::isLineFlagSet() const
{
  return _lineFlag;
}


