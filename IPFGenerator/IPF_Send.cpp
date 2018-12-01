/***************************************************************************
 * File...... Send.cpp
 * Author.... Mat
 * Date...... 10/10/95
 *
 * "Send" functions for IPFGenerator handle sending the IPF tags, control
 * words and text to the IPF target file.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// IPFGenerator
#pragma hdrfile "IPF_Common.pch"
#include "IPF_Common.hpp"
#pragma hdrstop

// I/O Streams
#include <fstream.h>


/***************************************************************************
 * Procedure.. IPFGenerator::sendTag
 * Author..... Mat
 * Date....... 10/12/95
 *
 * Sends an IPF tag, including the proper framing with : and . (eg, :hp2.)
 * Flags control automatic wrapping and insertion of line breaks.
 ***************************************************************************/
IPFGenerator & IPFGenerator::sendTag( const IString & tag, int flags )
{
  IString rawText = ":" + tag + ".";
  return sendRawText( rawText, flags );
}


/***************************************************************************
 * Procedure.. IPFGenerator::sendText
 * Author..... Mat
 * Date....... 10/12/95
 *
 * Sends a text string and filters for special characters.
 * Flags control automatic wrapping and insertion of line breaks.
 ***************************************************************************/
IPFGenerator & IPFGenerator::sendText( const IString & text, int flags )
{
  IString rawText = text;

  // check for required symbols
  /// other symbol translation?
  if ( ! ( flags & noSymbolCheck ) )
    rawText.change( "&", "&amp.").change( ":", "&colon.");

  // code page translation
  if ( ( flags & mapCodePage ) && ( _next->inputCodepage != _next->outputCodepage ) )
    _next->outputCodepage.translateString( rawText, _next->inputCodepage );

  // handle newlines and wrapping
  if ( _next->mode == lines ) {
    // in "lines" mode, don't wrap, but do honor \n in text
    flags |= noWrap;
  } else {
    // in other (formatted) mode, remove \n from text
    rawText.change( "\n", "" );
  } /* endif */

  // send the text!
  if ( ! ( flags & isNotContent ) )
    flags |= hasContent;  // set content flag for Generator
  return sendRawText( rawText, flags );
}



/***************************************************************************
 * Procedure.. IPFGenerator::sendControl
 * Author..... Mat
 * Date....... 10/12/95
 *
 * Sends an IPF control word, after prepending a "."  The control word
 * and parameters are put on their own line.
 * Flags control automatic wrapping and insertion of line breaks.
 ***************************************************************************/
IPFGenerator & IPFGenerator::sendControl( const IString & control, int flags )
{
  IString rawText = "." + control;
  return sendRawText( rawText,
      flags | breakBefore | breakAfter | noWrap | controlWord );
}


/***************************************************************************
 * Procedure.. IPFGenerator::sendRawText
 * Author..... Mat
 * Date....... 10/12/95
 *
 * The workhorse function, which handles all necessary wrapping and line
 * breaks.  It may modify the string for wrapping.
 ***************************************************************************/
IPFGenerator & IPFGenerator::sendRawText( IString & text, int flags )
{
  // RETURN IF NO TEXT
  if ( ! text.length() )
    return *this;

  // MAKE SURE WE ARE READY TO SEND
  if ( ! ( flags & noModeCheck ) )
    prepareToSend();

  // NEED LEADING PARAGRAPH IF NO OTHER FORMATTING
  if ( ! ( flags & isNotContent ) )
  {
    if ( _emptySection && ! ( flags & isFormat ) )
      sendTag( "p", breakBefore | isFormat );
    _emptySection = false;
  }

  // USE GENERATOR SUPPORT
  if ( ! ( flags & controlWord ) )
    flags |= noLeadingPeriod;
  outputText( text, flags );

  return *this;
}


/***************************************************************************
 * Procedure.. IPFGenerator::prepareToSend
 * Author..... Mat
 * Date....... 11/2/95
 *
 * Prepares the IPFGenerator for output to the target IPF file, by:
 *   - sending the current header, if it hasn't been done yet
 *   - sending tags to update the context, if necessary
 ***************************************************************************/
IPFGenerator & IPFGenerator::prepareToSend()
{
  // COMPLETE HEADING TAG IF NECESSARY
  IASSERTSTATE( _mode > start );
  if ( _mode < updatingHeading )
  {
    // recursively calls this procedure
    tag_h();
  }

  // UPDATE IPF CONTEXT
  if ( _mode == ready && needsSwitchTo( *_next ) )
  {
    // recursively calls this procedure
    switchContext( *_next );
  }

  return *this;
}


/***************************************************************************
 * Procedure.. IPFGenerator::sendEnd
 * Author..... Mat
 * Date....... 11/16/95
 *
 * Sends the :euserdoc tag to terminate the IPF file; flushes output
 ***************************************************************************/
IPFGenerator & IPFGenerator::sendEnd()
{
  wrapUpSection();
  sendForwarders();
  sendTag( "euserdoc", noModeCheck | breakAfter | isNotContent );
  return *this;
}


