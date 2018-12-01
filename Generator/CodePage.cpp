/***************************************************************************
 * File...... CodePage.cpp
 * Author.... Mat
 * Date...... 12/18/97
 *
 * Implementation of CodePage.
 *
 * NOTE: This is not platform independent.  It uses PM calls.  It also
 * assumes that PM has been initialized for the current thread.  Also
 * assumes that the active code page is static and will not change during
 * the program's run.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// Standard C
#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>

// OS/2
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_PM
#include <os2.h>

// OpenClass
#include <iexcept.hpp>      // exception macros
#include <itrace.hpp>       // ITRACE macros
#include <ithread.hpp>      // IThread
#include <igrafctx.hpp>     // IGraphicContext
#include <ifont.hpp>        // IFont
#include <istring.hpp>      // IString

// Generator
#include "CodePage.hpp"

// static class data
unsigned char CodePage::substitution = 0x7f;
long          CodePage::_activeCodePage;


CodePage::CodePage()
{
  _cp = activeCodePage();
}

CodePage::CodePage( Identifier id ):
    _cp( id )
{}

CodePage::CodePage( long cp ):
    _cp( cp )
{
  if ( ! _cp )
    _cp = activeCodePage();
}


/***************************************************************************
 * Procedure.. CodePage::setContextFont
 *
 * This is a workaround for a problem with the OpenClass library.  When
 * you use IGraphicContext::setFont directly, the code page will sometimes
 * get reset to 850 (depending on which font you are using).  This routine
 * forces the code page to remain at the active code page.  This program
 * assumes that the code page will not change throughout the run of the
 * program.
 ***************************************************************************/
void CodePage::setContextFont( IGraphicContext & context, IFont & font )
{
  // cheat and get a non-const pointer to the font's attributes
  PFATTRS pfattrs = (PFATTRS) font.fattrs();

  // set it to the active code page
  pfattrs->usCodePage = activeCodePage();

  // go ahead and call IGraphicContext
  context.setFont( font );
}


CodePage CodePage::activeCodePage()
{
  // load active code page if it isn't already loaded
  if ( ! _activeCodePage )
    _activeCodePage = getActiveCodePage();

  // return the active code page
  return CodePage( _activeCodePage );
}


long CodePage::getActiveCodePage()
{
  ULONG cp;

  if ( IThread::current().isGUIInitialized() )
  {
    // use PM services
    cp = WinQueryCp( IThread::current().messageQueue() );
    if ( ! cp )
      ITHROWGUIERROR( "WinQueryCp" );
  }
  else
  {
    // use base OS/2 services
    APIRET rc;
    ULONG  ccp;
    rc = DosQueryCp( sizeof(ULONG), &cp, &ccp );
    if ( rc == ERROR_CPLIST_TOO_SMALL )
      rc = 0;  // ignore this error
    if ( rc || ccp != sizeof(ULONG) )
      ITHROWSYSTEMERROR( rc, "DosQueryCp", IErrorInfo::invalidRequest, IException::recoverable );
  }

  return cp;
}


// returns 0 if unable to translate
unsigned char CodePage::translate( unsigned char ch, CodePage from ) const
{
  // check for same code page
  if ( from == *this )
    return ch;

  // otherwise use string translate function
  IString string( ch );
  translateString( string, from );
  unsigned char result = string[1];
  return result == CodePage::substitution? 0: result;
}


/***************************************************************************
 * Procedure.. CodePage::translateString
 * Author..... Mat
 * Date....... 12/18/97
 *
 * Translates a string, char-by-char, from the given code page to this
 * code page.  Any character which cannot be translated is given the value
 * of the CodePage::substitution character
 *
 * Assumes only single-byte character sets are used.
 ***************************************************************************/
void CodePage::translateString( IString & string, CodePage from ) const
{
  // check for same code page
  if ( from == *this )
    return;

  // can't convert non-IBM code pages!
  long cpFrom = from;
  if ( cpFrom <= 0 || _cp <= 0 )
    return;

  // allocate another string and get buffer pointer and lengths
  char * pc1 = string;
  size_t size1 = string.length();
  size_t size2 = size1;
  char * pc2 = (char *)_alloca( size2 + 1 );

  // TRY TO CALL PM TO CONVERT
  // create target buffer same size as source
  BOOL ok = WinCpTranslateString(
      IThread::current().anchorBlock(),
      cpFrom, pc1, _cp, size2 + 1, pc2 );
  if ( ok )
  {
    // change substitution characters (PM uses 0xFF)
    string.overlayWith( pc2 ).change( "\xff", IString( substitution ) );
    return;
  }

  // TRY TO CONVERT VIA ICONV() FUNCTIONS
  // construct name of codepages
  static const char nameTemplate[] = "IBM-%d";
  char fromName[20];
  char toName[20];
  sprintf( fromName, nameTemplate, cpFrom );
  sprintf( toName, nameTemplate, _cp );
  iconv_t converter = iconv_open( toName, fromName );
  if ( converter != (iconv_t)-1 )
  {
    size_t result = iconv( converter, &pc1, &size1, &pc2, &size2 );
    iconv_close( converter );
    if ( result != 0 && result != (size_t)-1 )
    {
      // change substitution characters (iconv uses 0x1a)
      string.overlayWith( pc2 ).change( "\x1a", IString( substitution ) );
    }
  }
}


