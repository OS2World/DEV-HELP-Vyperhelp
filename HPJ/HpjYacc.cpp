/***************************************************************************
 * File...... HpjYacc.cpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * Implementation for HPJ parser
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// Standard C
#include <stdlib.h>

// OpenClass
#include <iexcept.hpp>
#include <istparse.hpp>

// MekTek
#include "MekTek.hpp"

// Generator
#include "Generator.hpp"
#include "Filename.hpp"

// WinHelpParser
#include "WinHelpParser.hpp"
#include "CDefineYacc.hpp"
#include "HpjListYacc.hpp"
#include "HpjYacc.hpp"

#define ALexClass  HpjLexUnused   // change default names, avoid multiple definitions
#define AYaccClass HpjYaccUnused  // these are unused anyways
#include "hpj.yhh"       // generated from rule file



/***************************************************************************
 * Procedure.. HpjYacc::HpjYacc()
 * Author..... Mat
 * Date....... 12/21/95
 *
 * HpjYacc constructor loads Lex and Yacc tables and sets file to parse.
 ***************************************************************************/
HpjYacc::HpjYacc( const char* fileToParse ) :
    EnhancedYacc( "hpj", fileToParse ),
    _winId( 100 )
{
  resetWindow();
  _homeDir = Filename( fileToParse ).dir();
}


/***************************************************************************
 * Procedure.. HpjYacc::resetWindow
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Resets window settings to their default values.  Not sure about default
 * size and position, but it appears to be origin (0,0), height 100%, width
 * 60%.
 ***************************************************************************/
void HpjYacc::resetWindow()
{
  _winCaption = IString();
  _isWinMaximized = false;
  _winXPercent = 0;
  _winYPercent = 0;
  _winWidthPercent = 60;
  _winHeightPercent = 100;
}


/***************************************************************************
 * Procedure.. HpjYacc::parseFontSizeMap
 * Author..... Mat
 * Date....... 12/23/95
 *
 * Takes a font size mapping from a MAPFONTSIZE statement in the form
 *   m[-n]:p
 * where m, n, and p are point sizes.  "m" gives a source point size (or
 * alternately, "m-n" gives a range)  "p" give sthe target point size.
 ***************************************************************************/
void HpjYacc::parseFontSizeMap( const IString & mapString )
{
  // parse the string into components
  IString mn, m, n, p;
  mapString >> mn >> ':' >> p;
  mn >> m >> '-' >> n;

  // build mapping and save to the project
  FontSizeMap map;
  map.minPoints = m.asInt();
  map.maxPoints = n.length()? n.asInt(): map.minPoints;
  map.targetPoints = p.asInt();
  _whp->addFontSizeMap( map );
}


void HpjYacc::setReplaceString( const IString & string )
{
  unsigned equal = string.indexOf( '=' );
  if ( equal )
  {
    _replaceOld = string.subString( 1, equal - 1 ).strip();
    _replaceNew = string.subString( equal + 1 ).strip();
  }
  else
  {
    warning( "Invalid REPLACE option", NULL );
  }
}


IString HpjYacc::replacePath( const IString & string )
{
  if ( _replaceOld.length() )
    return IString::change( string, _replaceOld, _replaceNew, 1, 1 );
  else
    return string;
}


/***************************************************************************
 * Procedure.. HpjYacc::error
 * Author..... Mat
 * Date....... 11/24/98
 *
 * Prints an error if a message occurs
 ***************************************************************************/
SSBooleanValue HpjYacc::error( SSUnsigned32 qulState, SSLexLexeme & qLexeme)
{
  // print warning
  warning( "Parsing error", &qLexeme );

  // standard error recovery
  return errorToken();
}


/***************************************************************************
 * Procedure.. HpjYacc::reduce()
 * Author..... Mat
 * Date....... 12/21/95
 *
 * The reduce function is called for every production in the Yacc grammar.
 * The productions are defined in the .yhh created by the VP compiler.
 * When the rule file is changed, new cases should be added here from the
 * examples generated in the .yhh file.
 ***************************************************************************/
SSYaccStackElement* HpjYacc::reduce( SSUnsigned32 production, SSUnsigned32 size)
{
  switch ( production )
  {
    case AYaccBmRoot:
    // option -> BmRoot Equal Parm
       _whp->setBitmapPath( replacePath( lexemeAsString( 2 ) ) );
       break;

    case AYaccContents:
    // option -> Contents Equal Parm
       _whp->setContents( lexemeAsString( 2 ) );
       break;

    case AYaccCopyright:
    // option -> Copyright Equal Parm
       _gen->setCopyright( lexemeAsString( 2 ) );
       break;

    case AYaccMapFontSize:
    // option -> MapFontSize Equal Parm
       parseFontSizeMap( lexemeAsString( 2 ) );
       break;

    case AYaccRoot:
    // option -> Root Equal Parm
       _whp->setRootPath( replacePath( lexemeAsString( 2 ) ) );
       break;

    case AYaccCnt:
    // option -> Cnt Equal Parm
       _whp->setCntFile( replacePath( lexemeAsString( 2 ) ) );
       break;

    case AYaccHlp:
    // option -> Hlp Equal Parm
       _whp->setHlpFile( replacePath( lexemeAsString( 2 ) ) );
       break;

    case AYaccTitle:
    // option -> Title Equal Parm
       _whp->setTitle( lexemeAsString( 2 ) );
       break;

    case AYaccIndexSeparators:
    // option -> IndexSeparators Equal Parm
       // strip double-quotes from separator list
       _whp->setSeparators( lexemeAsString( 2 ).strip('"') );
       break;

    case AYaccReplace:
    // option -> Replace Equal Parm
       setReplaceString( lexemeAsString( 2 ) );
       break;

    case AYaccForceFont:
    // option -> ForceFont Equal Parm
       _whp->setForceFont( lexemeAsString( 2 ) );
       break;

    case AYaccFileItem:
    // file -> Item
       _whp->addRtfFile( replacePath( lexemeAsString( 0 ) ) );
       break;

    case AYaccFileInclude:
    // file -> Include AngleString
    {
       // "<>" delimiters are used, but the INCLUDE path is not searched
       Filename listFile =
           lexemeAsString( 1 ).stripLeading( '<' ).stripTrailing( '>' );
       if (! listFile.locateOnPath( _homeDir, false ))
         listFile.locateTo( _homeDir );
       _gen->setInputFilename( listFile );
       HpjListYacc fileList( listFile );
       fileList.parseTo( *_whp );
       break;
    }

    case AYaccBitmapList:
    // bitmaps -> Item bitmaps
       _whp->addBitmapFile( replacePath( lexemeAsString( 0 ) ) );
       break;

    case AYaccMapSimple:
    // mapping -> Name Name
       _whp->setId( lexemeAsString( 0 ), lexemeAsULong( 1 ) );
       break;

    case AYaccMapDefine:
    // mapping -> Define Name Name
       _whp->setId( lexemeAsString( 1 ), lexemeAsULong( 2 ) );
       break;

    case AYaccMapInclude:
    // mapping -> Include QuoteString
    case AYaccMapIncludeSys:
    // mapping -> Include AngleString
    case AYaccMapIncludeOther:
    // mapping -> Include MapName
    {
       // help compiler treats <> and "" the same
       Filename filename = lexemeAsString( 1 );
       if ( production == AYaccMapInclude )
         filename.stripLeading( '\"' ).stripTrailing( '\"' );
       else if ( production == AYaccMapIncludeSys )
         filename.stripLeading( '<' ).stripTrailing( '>' );

       // search home directory, then INCLUDE path
       if ( ! filename.locateOnPath( _homeDir ) )
         if ( ! filename.locateOnStandardPath( Filename::include ) )
           filename.locateTo( _homeDir );
       _gen->setInputFilename( filename );
       CDefineYacc cdef( filename, _homeDir );
       cdef.parseTo( *_whp );
       break;
    }

    case AYaccAliasAssign:
    // alias -> Name Equal Name
       _whp->addAlias( lexemeAsString( 0 ), lexemeAsString( 2 ) );
       break;

    case AYaccWindowDef:
    // window -> WindowName Equal captionWith
    case AYaccWindowDefR:
    // window -> PrefixR Equal captionWith
    case AYaccWindowDefF:
    // window -> PrefixF Equal captionWith
    {
       // register window definition with Generator
       WindowDef window(
           _winId,
           _winCaption,
           Position( Distance( _winXPercent, Distance::percent ) ),
           Position( Distance( _winYPercent, Distance::percent ) ),
           Distance( _winWidthPercent, Distance::percent ),
           Distance( _winHeightPercent, Distance::percent ),
           WindowDef::sizeBorder,
           WindowDef::allScroll,
           true,  // title bar
           true,  // minimize button
           true,  // maximize button
           true   // system menu
       );
       _gen->defineWindow( window );
       // save mapping from WinHelp "name" to WindowDef "id"
       _whp->addWindow( lexemeAsString( 0 ), _winId );
       // increment to next ID
       _winId++;
       // reset window properties
       resetWindow();
       break;
    }

    case AYaccCaption:
    // caption -> QuoteText
       _winCaption = lexemeAsString( 0 );
       break;

    case AYaccPosition:
    // position -> OpenParen Number Comma Number Comma Number Comma Number CloseParen
    {
       // Numbers are ordered: x-coord, y-coord, width, height
       // Numbers are in WinHelp's "01023" coordinate system (1024x1024)
       // WinHelp coordinates are based on upper-left, not lower-left!
       const ULong maxCoord = 1023;
       _winXPercent = lexemeAsULong( 1 ) / maxCoord;
       _winYPercent = 100 - lexemeAsULong( 3 ) / maxCoord;  // convert to lower-left
       _winWidthPercent   = lexemeAsULong( 5 ) / maxCoord;
       _winHeightPercent  = lexemeAsULong( 7 ) / maxCoord;
       break;
    }

    case AYaccState:
    // state -> Number
       // check low bit for maximized state
       _isWinMaximized = (lexemeAsULong( 0 ) & 1);
       break;

  }

  return stackElement();
}


/***************************************************************************
 * Procedure.. HpjYacc::parseTo
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Sets the target Generator and WinHelpParser, then begins the parsing.
 * Returns true if an error occured, as per SSYacc::parse().
 ***************************************************************************/
SSBooleanValue HpjYacc::parseTo( Generator & gen, WinHelpParser & whp )
{
  _gen = &gen;
  _whp = &whp;
  return parse();
}


/***************************************************************************
 * Procedure.. HpjYacc::warning
 * Author..... Mat
 *
 * Log a warning condition.
 ***************************************************************************/
void HpjYacc::warning( const IString & text, const SSLexLexeme * lexeme )
{
  unsigned line = 0;
  unsigned col = 0;

  // get current location, if possible
  if ( ! lexeme )
  {
    SSYaccStackElement * element = elementFromStack( 0 );
    MEKASSERT( element );
    lexeme = element->lexeme();
  }

  // determine line and column
  if ( lexeme )
  {
    line = lexeme->line();
    col = lexeme->offset();
  }

  _whp->warning( text, line, col );
}


