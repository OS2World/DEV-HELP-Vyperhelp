/***************************************************************************
 * File...... HpjGenerator.cpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * Implementation for HpjGenerator
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Standard C
#include <ctype.h>

// MekTek
#include "MekTek.hpp"

// Generator
#include "HeaderGenerator.hpp"
#include "RtfGenerator.hpp"
#include "Filename.hpp"
#include "HpjGenerator.hpp"

// declare this for support of user-sized windows
// the current support is incomplete and needs testing
// #define GEN_WINDOWS


HpjGenerator::HpjGenerator( const Filename & filename ):
  _cpOut( 1004 ),  // HPJ and CNT output uses codepage 1004
  _rtf( 0 ),
  _hh( 0 ),
  _state( ready ),
  _sectionLevel( 0 ),
  _cntLevel( 0 ),
  _maxCntLevel( 0 )
{
  // save base filename
  _baseName = filename.fullBase();

  // open HPJ file and write initial information
  _hpj.open( filename, ios::out );
  _hpj << "; " << credits() << "\n\n";
  check( _hpj );

  // create RtfGenerator
  _rtfName = _baseName + ".rtf";
  _rtf = new RtfGenerator( _rtfName, this );
  MEKASSERT( _rtf );

  // create HeaderGenerator
  _hhName = _baseName + ".hh";
  _hh = new HeaderGenerator( _hhName );
  MEKASSERT( _hh );
}


HpjGenerator::~HpjGenerator()
{
  finish();
}


/***************************************************************************
 * Procedure.. HpjGenerator::finish
 * Author..... Mat
 * Date....... 3/12/99
 *
 * Finish the RTF and HPJ generation.  Send all remaining output to the
 * HPJ file and close the other files.
 ***************************************************************************/
void HpjGenerator::finish()
{
  if ( _state == done )
    return;

  // finish RTF output
  if ( _rtf )
  {
    _rtf->finish();
    delete _rtf;
    _rtf = 0;
  }

  // finish HH output
  if ( _hh )
  {
    _hh->finish();
    delete _hh;
    _hh = 0;
  }

  // close CNT file
  if ( _state >= started )
  {
    sendCntEntry( 0 );
    _cnt << endl;
    check( _cnt );
    _cnt.close();
  }

  IPosition i;
#ifdef GEN_WINDOWS
  // send window definitions to HPJ
  _hpj << "[WINDOWS]\n";
  for ( i = 1; i <= numberOfWindows(); i++ )
  {
    const WindowDef & win = window( i );
    IString line = IString(win.id()) + IString('=');
    if ( win.title().length() )
      line += IString('"') + win.title() + IString('"');
    line += IString(',');
    _hpj << line << windowMetrics( win ) << '\n';
    check( _hpj );
  }
  _hpj << '\n';
  check( _hpj );
#endif

  // send options
  _hpj << "[OPTIONS]\n";
  if ( _copyright.length() )
    _hpj << "COPYRIGHT=" << translate( _copyright, false ) << '\n';
  if ( _title.length() )
    _hpj << "TITLE=" << translate( _title, false ) << "\n\n";
  check( _hpj );

  // enable browse buttons
  _hpj << "[CONFIG]\nBrowseButtons()\n\n";

  // write file references to HPJ file
  _hpj << "[FILES]\n" << _rtfName.name() << "\n\n";
  _hpj << "[MAP]\n#include <" << _hhName.name() << ">\n";
  check( _hpj );

  // close HPJ file
  _hpj << endl;
  check( _hpj );
  _hpj.close();

  // save artwork
  for ( i = 1; i <= numberOfArts(); i++ )
  {
    const ArtDef & artdef = art( i );
    artdef.saveBitmap( artFilename( artdef.id() ) );
  }

  // change state
  _state = done;
}


/***************************************************************************
 * Procedure.. HpjGenerator::check
 * Author..... Mat
 * Date....... 3/12/99
 *
 * Check if the given output file stream is in an error condition.  Throw
 * an exception if an error is present.
 ***************************************************************************/
void HpjGenerator::check( ofstream & stream ) const
{
  if ( ! stream )
    MEKTHROWCERROR();
}


/***************************************************************************
 * Procedure.. HpjGenerator::start
 * Author..... Mat
 * Date....... 3/12/99
 *
 * Start the generation.
 ***************************************************************************/
void HpjGenerator::start()
{
  // create CNT file
  IString cntName = _baseName + ".cnt";
  _cnt.open( cntName, ios::out );
  _cnt << "; " << credits() << '\n';
  _cnt << ":BASE " << _baseName.name() << ".hlp\n";
  if ( _title.length() )
    _cnt << ":TITLE " << translate( _title, true ) << '\n';
  check( _cnt );
  _tocCount = 0;

  // change state
  _state = started;
}


/***************************************************************************
 * Procedure.. HpjGenerator::sendCntEntry
 * Author..... Mat
 * Date....... 3/26/99
 *
 * When the section is ended, an entry is written to the CNT file.  The level
 * of the next section is used to determine if a "book" entry needs to
 * be written first.  Topics can only be indented under a book entry.
 *
 * 8/31/00: This function corrects for several bugs in the WinHelp viewer:
 *
 * 1. "Book" entries cannot have an associated topic.  Topic entries must be
 *    under a book entry.  We insert a book entry whenever indenting a level
 *    to compensate for this.
 * 2. You cannot have a topic at the same level as a book above it.  It gets
 *    swallowed up by the book.  We workaround this by subtracting one from
 *    the level written to the CNT file in these situations.
 * 3. The workaround mentioned in (2) doesn't work for level 1 topics.  In
 *    those cases, we insert a dummy book so that we can get the level 1 topic
 *    out of the book above it.
 *
 ***************************************************************************/
void HpjGenerator::sendCntEntry( unsigned nextLevel )
{
  // don't write previous section if this is the first section
  if ( _tocCount )
  {
    unsigned level = _cntLevel;

    if ( nextLevel > _sectionLevel || ! level)
    {
      // handle special case (3) above (signalled by level = 0)
      // also handle the case where (1) and (2) intersect:
      //   i.e., you need to back up a level, but since you're creating a book it's ok
      //   in this case _cntLevel < _sectionLevel to indicate intended workaround
      if (! level || (_cntLevel < _sectionLevel))
        level++;

      // create a "book" entry to start the next branch
      _cnt << level << ' ' << _sectionTitle << '\n';
      check( _cnt );
      level++;
    }

    // create a "topic" entry
    _cnt << level << ' ' << _sectionTitle << '=' << _sectionLabel;
#ifdef GEN_WINDOWS
    if ( _sectionWindowId )
      _cnt << '>' << _sectionWindowId;
#endif
    _cnt << '\n';
    check( _cnt );
  }

  // save next section level
  _sectionLevel = nextLevel;

  // adjust actual contents level for special cases (2) and (3) above.
  // for (3), any level 1 entry (after the first book entry) needs the workaround
  Boolean needWorkaround = (nextLevel < _cntLevel) || ((nextLevel == 1) && (_maxCntLevel > 1));
  _cntLevel = needWorkaround? nextLevel - 1: nextLevel;

  // maintain a maximum CNT level
  if (_cntLevel > _maxCntLevel)
  {
    _maxCntLevel = _cntLevel;
  }
}


/***************************************************************************
 * Procedure.. HpjGenerator::sanitize
 * Author..... Mat
 * Date....... 3/16/99
 *
 * "Sanitize" a topic ID string (label) to remove illegal characters.
 ***************************************************************************/
IString HpjGenerator::sanitize( const IString & topic ) const
{
  // NOTE: constructor must take this form or the two IStrings will share the buffer
  IString result( (const char *)topic );
  char * pch = (char *)result;

  // remove anything but [A-Z0-9._]
  while ( *pch )
  {
    if ( ! isalnum(*pch) && ( *pch != '.' ) && ( *pch != '_' ) )
      *pch = '_';
    pch++;
  }

  return result;
}


/***************************************************************************
 * Procedure.. HpjGenerator::translate
 * Author..... Mat
 * Date....... 3/18/99
 *
 * Translate a string for output to the CNT or HPJ file.  Also, remove all
 * semi-colons, which would wrongly be interpreted as comments.  For CNT
 * files, the ';' is converted to '\;', and also '=' is converted to '\=',
 * so it is not interpreted as the end of string.
 ***************************************************************************/
IString HpjGenerator::translate( const IString & text, Boolean forCnt ) const
{
  IString result( text );

  // translate to correct codepage
  _cpOut.translateString( result, _cpIn );

  // remove or modify reserved characters
  if ( forCnt )
  {
    result.change( ";", "\\;" );
    result.change( "=", "\\=" );
  }
  else
  {
    result.change( ";", "" );
  }

  return result;
}


#ifdef GEN_WINDOWS
/***************************************************************************
 * Procedure.. HpjGenerator::windowMetrics
 * Author..... Mat
 * Date....... 3/16/99
 *
 * Return a string of the form "(x,y,width,height)" which describes, in
 * WinHelp 0-1023 coordinates the position and size of the given window.
 ***************************************************************************/
IString HpjGenerator::windowMetrics( const WindowDef & win ) const
{
  // get WinHelp coordinates
  unsigned x = win.originX().asWinCoord();
  unsigned y = win.originY().asWinCoord();
  unsigned width = win.width().asWinCoord();
  unsigned height = win.height().asWinCoord();

  // make sure window stays within boundaries
  if ( x + width > 1023 )
    width = 1023 - x;
  if ( y + height > 1023 )
    height = 1023 - y;

  return IString('(')
       + IString(x) + IString(',')
       + IString(y) + IString(',')
       + IString(width) + IString(',')
       + IString(height) + IString(')');
}
#endif


/***************************************************************************
 * Procedure.. HpjGenerator::artFilename
 * Author..... Mat
 * Date....... 3/23/99
 *
 * Return the bitmap filename used for the given art definition.
 ***************************************************************************/
Filename HpjGenerator::artFilename( const IString & id ) const
{
  return Filename( _baseName.dir() + id + ".bmp" );
}


/***************************************************************************
 * Generator Settings
 ***************************************************************************/

Generator & HpjGenerator::setTitle( const IString & title )
{
  _title = title;

  MEKASSERT(_rtf);
  _rtf->setTitle( title );
  return *this;
}

Generator & HpjGenerator::setCopyright( const IString & copyright )
{
  _copyright = copyright;

  MEKASSERT(_rtf);
  _rtf->setCopyright( copyright );
  return *this;
}

Generator & HpjGenerator::setCodePage( CodePage cp )
{
  _cpIn = cp;
  MEKASSERT(_rtf);
  _rtf->setCodePage( cp );
  return *this;
}


/***************************************************************************
 * Gin Handlers
 ***************************************************************************/

void HpjGenerator::handleSection( const SectionGin & gin )
{
  // make sure we've started
  if ( _state < started )
    start();

  // process an entry for the CNT file
  if ( gin.isListed() )
  {
    // send CNT entry for the previous section
    sendCntEntry( gin.level() );

    // get title
    _sectionTitle = translate( gin.title(), true );
    if ( ! _sectionTitle.length() )
      _sectionTitle = "Untitled";

    // make a unique label
    _sectionLabel = IString("cnt") + IString(++_tocCount).rightJustify( 4, '0');

#ifdef GEN_WINDOWS
    // save window ID
    _sectionWindowId = gin.windowId();
#endif
  }

  // pass to RtfGenerator
  MEKASSERT(_rtf);
  _rtf->handleSection( gin );

  // send the label to RTF
  if ( gin.isListed() )
    *_rtf << LabelGin( _sectionLabel );
}

void HpjGenerator::handleLabel( const LabelGin & gin )
{
  // make sanitized label
  LabelGin sanitized( sanitize( gin.name() ) );
  sanitized.setId( gin.id() );
  sanitized.setExternal( gin.isExternal() );

  // pass to HeaderGenerator
  MEKASSERT(_hh);
  _hh->handleLabel( sanitized );

  // pass to RtfGenerator
  MEKASSERT(_rtf);
  _rtf->handleLabel( sanitized );
}

void HpjGenerator::handleLink( const LinkGin & gin )
{
  // make sanitized label
  LinkGin sanitized( sanitize( gin.target() ) );
  if ( gin.isExternal() )
    sanitized.setFile( gin.file() );

  MEKASSERT(_rtf);
  _rtf->handleLink( sanitized );
}


/***************************************************************************
 * RTF Passthru Functions
 ***************************************************************************/

Generator & HpjGenerator::setSubject( const IString & subject )
{
  MEKASSERT(_rtf);
  _rtf->setSubject( subject );
  return *this;
}

Generator & HpjGenerator::setVersion( const IString & version )
{
  MEKASSERT(_rtf);
  _rtf->setVersion( version );
  return *this;
}

Generator & HpjGenerator::setAuthor( const IString & author )
{
  MEKASSERT(_rtf);
  _rtf->setAuthor( author );
  return *this;
}

Generator & HpjGenerator::addKeyword( const IString & keyword )
{
  MEKASSERT(_rtf);
  _rtf->addKeyword( keyword );
  return *this;
}

void HpjGenerator::handleKeyword( const KeywordGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleKeyword( gin );
}

void HpjGenerator::handleRow( const RowGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleRow( gin );
}

void HpjGenerator::handleColumn( const ColumnGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleColumn( gin );
}

void HpjGenerator::handleItem( const ItemGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleItem( gin );
}

void HpjGenerator::handlePara( const ParaGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handlePara( gin );
}

void HpjGenerator::handleLine( const LineGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleLine( gin );
}

void HpjGenerator::handlePush( const PushGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handlePush( gin );
}

void HpjGenerator::handlePop( const PopGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handlePop( gin );
}

void HpjGenerator::handleRightMargin( const RightMarginGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleRightMargin( gin );
}

void HpjGenerator::handleLeftMargin( const LeftMarginGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleLeftMargin( gin );
}

void HpjGenerator::handleBorder( const BorderGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleBorder( gin );
}

void HpjGenerator::handleAlign( const AlignGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleAlign( gin );
}

void HpjGenerator::handleSpacing( const SpacingGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleSpacing( gin );
}

void HpjGenerator::handleRuler( const RulerGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleRuler( gin );
}

void HpjGenerator::handleOutline( const OutlineGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleOutline( gin );
}

void HpjGenerator::handleFont( const FontGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleFont( gin );
}

void HpjGenerator::handleSize( const SizeGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleSize( gin );
}

void HpjGenerator::handleColor( const ColorGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleColor( gin );
}

void HpjGenerator::handleBackColor( const BackColorGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleBackColor( gin );
}

void HpjGenerator::handleBold( const BoldGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleBold( gin );
}

void HpjGenerator::handleItalic( const ItalicGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleItalic( gin );
}

void HpjGenerator::handleUnderline( const UnderlineGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleUnderline( gin );
}

void HpjGenerator::handleComment( const CommentGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleComment( gin );
}

void HpjGenerator::handleArt( const ArtGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleArt( gin );
}

void HpjGenerator::handleText( const TextGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleText( gin );
}

void HpjGenerator::handleSymbol( const SymbolGin & gin )
{
  MEKASSERT(_rtf);
  _rtf->handleSymbol( gin );
}

