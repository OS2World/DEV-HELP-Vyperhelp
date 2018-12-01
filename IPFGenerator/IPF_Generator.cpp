/***************************************************************************
 * File...... D:\proj\Generator\IPFGenerator.cpp
 * Author.... Mat
 * Date...... 10/5/95
 *
 * IPFGenerator implementation
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// IPFGenerator
#include "IPF_Common.hpp"
#include "IpfForwarderSequence.hpp"

// OpenClass
#include <idate.hpp>


/***************************************************************************
 * Procedure.. IPFGenerator::IPFGenerator
 * Author..... Mat
 * Date....... 10/24/95
 *
 * Constructor for IPFGenerator.  os is the text stream for the IPF output,
 * maxLineLength is the maximum number of characters per line, and
 * workDir is the directory where BMP files should be put (need trailing \)
 ***************************************************************************/
IPFGenerator::IPFGenerator( const IString & filename, unsigned maxLineLength, const IString & workDir ) :
    _file( filename ),
    _mode( header ),
    _section( 1, false ),  // dummy section info
    _label( IString() ),   // dummy label info
    _workDir( workDir ),
    _nextId( 30000 ),
    _tableMode( notInTable ),
    _moreLabels( new LabelList ),
    _active( new IPFContext ),
    _next( new IPFContext ),
    _contextStack( new ContextStack ),
    _index( new IndexSet ),
    _forwarders( new IpfForwarderSequence )
{
  IASSERTSTATE( _moreLabels );
  IASSERTSTATE( _active );
  IASSERTSTATE( _next );
  IASSERTSTATE( _contextStack );
  IASSERTSTATE( _index );
  IASSERTSTATE( _forwarders );

  // initialize ASCII output from Generator
  setOutput( _file, maxLineLength, Generator::ipf );

  // initialize document
  sendControl( IString("* ") + credits(), noModeCheck | isNotContent );
  sendTag( "userdoc", breakBefore | breakAfter | noModeCheck | isNotContent );
  sendTag( "docprof toc=123456", breakBefore | breakAfter | noModeCheck | isNotContent );
}


IPFGenerator::~IPFGenerator()
{
  delete _moreLabels;
  delete _active;
  delete _next;
  delete _contextStack;
  delete _index;
  delete _forwarders;
}


/***************************************************************************
 * Procedure.. IPFGenerator::finish()
 * Author..... Mat
 * Date....... 10/24/95
 *
 * Ends output to target.
 ***************************************************************************/
void IPFGenerator::finish()
{
  sendEnd();
  closeOutput();
  _file.close();
}


/***************************************************************************
 * Procedure.. IPFGenerator::nextId
 * Author..... Mat
 * Date....... 10/24/95
 *
 * Gets the next available ID by incrementing from the last one used.
 ***************************************************************************/
int IPFGenerator::nextId()
{
  return _nextId++;
}


// HEADER ELEMENTS

Generator & IPFGenerator::setTitle( const IString & title )
{
  return sendTag( "title", breakBefore | noModeCheck | isNotContent )
        .sendText( translate( title, true ), noWrap | breakAfter | noModeCheck | isNotContent | noSymbolCheck );
}

Generator & IPFGenerator::setSubject( const IString & subject )
{
  return sendControl( "* Subject: " + translate( subject ), noModeCheck | isNotContent );
}

Generator & IPFGenerator::setVersion( const IString & version )
{
  return sendControl( "* Version: " + translate( version ), noModeCheck | isNotContent );
}

Generator & IPFGenerator::setCopyright( const IString & copyright )
{
  return sendControl( "* Copyright: " + translate( copyright ), noModeCheck | isNotContent );
}

Generator & IPFGenerator::setAuthor( const IString & author )
{
  return sendControl( "* Author: " + translate( author ), noModeCheck | isNotContent );
}

Generator & IPFGenerator::addKeyword( const IString & keyword )
{
  return sendControl( "* Keyword: " + translate( keyword ), noModeCheck | isNotContent );
}


// CODE PAGE SUPPORT

Generator & IPFGenerator::setCodePage( CodePage cp )
{
  _docCodePage = cp;
  return *this;
}


/***************************************************************************
 * Procedure.. IPFGenerator::translate
 * Author..... Mat
 * Date....... 12/15/98
 *
 * Translates the given string for IPF output.  These strings are in the
 * codepage set by the setCodePage call.  Currently, we assume that IPF output
 * should use code page 850.
 *
 * NOTE: be sure to use "noSymbolCheck" on any translated text which is sent.
 ***************************************************************************/
IString IPFGenerator::translate( const IString & string, Boolean allowSymbols ) const
{
  IString xlat;
  IString symbol;
  CodePage output;  // output to active code page
  const unsigned char * pch = string;

  while ( *pch )
  {
    if ( ( *pch >= 0x20 ) && ( *pch <= 0x7f ) && ( *pch != '&' ) && ( *pch != ':' ) )
    {
      // assume characters in this range don't need translation
      xlat += IString( *pch );
    }
    else
    {
      // check for a matching IPF symbol first
      if ( allowSymbols && translateChar( *pch, symbol ) )
        xlat += symbol;
      else
      {
        // otherwise use code page translation
        // this is less desireable because the IPF viewer always uses the
        //    active code page (IPF source cannot control).
        unsigned char ch = output.translate( *pch, _docCodePage );
        xlat += IString( ch? ch: CodePage::substitution );
      }
    }
    pch++;
  }

  return xlat;
}


// TEXT

void IPFGenerator::handleText( const TextGin & text )
{
  if ( _next->isComment )
  {
    // send as a comment line
    // not concerned with wrapping or lines or combining IStrings
    sendControl( "* " + text.string(), isNotContent );
  }
  else
    sendText( text.string(), mapCodePage );
}


