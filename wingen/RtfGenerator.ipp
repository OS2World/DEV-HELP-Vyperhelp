/***************************************************************************
 * File...... RtfGenerator.ipp
 * Author.... Mat
 * Date...... 2/4/99
 *
 * Inline functions for RtfGenerator.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

inline Generator & RtfGenerator::setTitle( const IString & title )
{
  _title = title;
  return *this;
}

inline Generator & RtfGenerator::setSubject( const IString & subject )
{
  _subject = subject;
  return *this;
}

inline Generator & RtfGenerator::setVersion( const IString & version )
{
  _version = version;
  return *this;
}

inline Generator & RtfGenerator::setCopyright( const IString & copyright )
{
  _copyright = copyright;
  return *this;
}

inline Generator & RtfGenerator::setAuthor( const IString & author )
{
  _author = author;
  return *this;
}

inline Generator & RtfGenerator::setCodePage( CodePage cp )
{
  _cp = cp;
  return *this;
}

