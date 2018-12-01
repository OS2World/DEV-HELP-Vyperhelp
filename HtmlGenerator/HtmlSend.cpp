/***************************************************************************
 * File...... HtmlSend.cpp
 *
 * "Send" functions for HtmlGenerator handle sending the Html tags, control
 * words and text to the Html target file.
 *
 * Also include routines for managing the stack of active HTML tags.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

// HtmlGenerator
#include "HtmlCommon.hpp"


/***************************************************************************
 * Procedure.. HtmlGenerator::sendComment
 *
 * Send an HTML comment on a separate line.
 ***************************************************************************/
HtmlGenerator & HtmlGenerator::sendComment( const IString & comment, int flags )
{
  IString rawText = "<!-- " + comment + " -->";
  return sendRawText( rawText, flags | breakBefore | breakAfter | isNotContent );
}




/***************************************************************************
 * Procedure.. HtmlGenerator::sendMetaTag
 *
 * Send a "META" tag with header information.  Content is assumed to be
 * already translated, so no symbol check is done.
 ***************************************************************************/
HtmlGenerator & HtmlGenerator::sendMetaTag( const IString & name, const IString & content, Boolean isHttpEquiv )
{
  IString tag = "meta ";
  if (isHttpEquiv)
  {
    tag += "http-equiv";
  }
  else
  {
    tag += "name";
  }
  tag += "=\"" + name + "\" content=\"" + content + "\"";
  return sendTag(tag, breakBefore | breakAfter | noModeCheck | isNotContent | noSymbolCheck);
}


/***************************************************************************
 * Procedure.. HtmlGenerator::sendTag
 *
 * Sends an Html tag, including the proper framing with < and > (eg, <h2>)
 * Flags control automatic wrapping and insertion of line breaks.
 ***************************************************************************/
HtmlGenerator & HtmlGenerator::sendTag( const IString & tag, int flags )
{
  IString rawText = "<" + tag + ">";
  return sendRawText( rawText, flags );
}


/***************************************************************************
 * Procedure.. HtmlGenerator::sendText
 *
 * Sends a text string and filters for special characters.
 * Flags control automatic wrapping and insertion of line breaks.
 ***************************************************************************/
HtmlGenerator & HtmlGenerator::sendText( const IString & text, int flags )
{
  IString rawText = text;

  // check for required symbols
  if ( ! ( flags & noSymbolCheck ) )
  {
    // convert reserved characters
    rawText.change("&", "&amp;");
    rawText.change("<", "&lt;");
    rawText.change(">", "&gt;");
    // convert quotes to avopid problems when used in quoted parameters
    rawText.change("\"", "&quot;");
  }

  // handle newlines and wrapping
  if ( _next->mode == lines )
  {
    // in "lines" mode, don't wrap, convert \n to <br>
    flags |= noWrap;
    rawText.change( "\n", "<br>" );
  }
  else
  {
    // in other (formatted) mode, remove \n from text
    rawText.change( "\n", "" );
  }

  // code page translation
  if ( ( flags & mapCodePage ) && ( _next->inputCodepage != outputCodepage ) )
    outputCodepage.translateString( rawText, _next->inputCodepage );

  // send the text!
  if ( ! ( flags & isNotContent ) )
    flags |= hasContent;  // set content flag for Generator

  return sendRawText( rawText, flags );
}



/***************************************************************************
 * Procedure.. HtmlGenerator::sendRawText
 *
 * The workhorse function, which handles all necessary wrapping and line
 * breaks.  It may modify the string for wrapping.
 ***************************************************************************/
HtmlGenerator & HtmlGenerator::sendRawText( IString & text, int flags )
{
  // RETURN IF NO TEXT
  if ( ! text.length() )
    return *this;

  // MAKE SURE WE ARE READY TO SEND
  if ( ! ( flags & noModeCheck ) )
    prepareToSend();

  // USE GENERATOR SUPPORT
  outputText( text, flags );

  return *this;
}


/***************************************************************************
 * Procedure.. HtmlGenerator::prepareToSend
 *
 * Prepares the HtmlGenerator for output to the target Html file, by:
 *   - sending the current header, if it hasn't been done yet
 *   - sending tags to update the context, if necessary
 ***************************************************************************/
HtmlGenerator & HtmlGenerator::prepareToSend()
{
  // COMPLETE HEADING TAG IF NECESSARY
  IASSERTSTATE( _mode > start );

  // UPDATE HTML CONTEXT
  if ( _mode == ready && needsSwitchTo( *_next ) )
  {
    // recursively calls this procedure
    switchContext( *_next );
  }

  return *this;
}



/***************************************************************************
 * Procedure.. HtmlGenerator::sendHeader
 *
 * Send header information for an HTML file.
 ***************************************************************************/
void HtmlGenerator::sendHeader(const IString & title, Boolean isTop, HtmlDoctype type, Boolean hasBody)
{
  IString doctype = "!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML ";
  switch (type)
  {
    case html3:
      doctype += "3.2";
      break;
    case html4:
      doctype += "4.0";
      break;
    case html4frameset:
      doctype += "4.0 Frameset";
      break;
  }
  doctype += "//EN\"";

  // initialize document
  int flags = breakBefore | breakAfter | noModeCheck | isNotContent;
  sendTag(doctype, flags);
  sendComment( credits(), flags);
  sendTag("html", flags);
  sendTag("head", flags);
  sendMetaTag("content-type", "text/html;charset=Windows-1252", true);
  sendMetaTag("generator", IString("VyperHelp ") + version());

  if (isTop)
  {
    // description and keywords are sent for top-level file only
    if (_subject.length())
    {
      sendMetaTag("description", _subject);
    }
    if (_keywords.length())
    {
      sendMetaTag("keywords", _keywords);
    }
  }

  if (_copyright.length())
  {
    sendMetaTag("copyright", _copyright);
  }
  if (_author.length())
  {
    sendMetaTag("author", _author);
  }
  if (_version.length())
  {
    sendMetaTag("version", _version);
  }

  // send title
  IString fileTitle(title);
  if (! fileTitle.length())
  {
    fileTitle = "Untitled";
  }
  sendTag("title", breakBefore | noModeCheck | isNotContent);
  sendText(fileTitle, noModeCheck | isNotContent | noSymbolCheck );
  sendTag("/title", breakAfter | noModeCheck | isNotContent);

  // finish head and start body
  sendTag("/head", flags);
  if (hasBody)
    sendTag("body", flags);
}



/***************************************************************************
 * Procedure.. HtmlGenerator::sendEnd
 *
 * Sends the end tags to terminate the HTML file; flushes output
 ***************************************************************************/
void HtmlGenerator::sendEnd(Boolean hasBody)
{
  if (hasBody)
    sendTag( "/body", noModeCheck | breakAfter | isNotContent );
  sendTag( "/html", noModeCheck | breakAfter | isNotContent );
}


