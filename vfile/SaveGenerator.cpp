/***************************************************************************
 * File...... SaveGenerator.cpp
 * Author.... Mat
 * Date...... 11/13/97
 *
 * Implementation for SaveGenerator
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>

// Generator
#include "Filename.hpp"

// VFile
#include "vyd.h"
#include "SaveGenerator.hpp"


SaveGenerator::SaveGenerator( const IString & filename, Boolean sectionOnly ):
    Generator( false ),
    _language( english ),
    _sectionOffset( VYD_SECTION_OFFSET ),
    _output( filename ),
    _sectionOnly( sectionOnly ),
    _isInfoSaved( false )
{
  initialize();
}


SaveGenerator::SaveGenerator( ostream & stream, Boolean sectionOnly ):
    Generator( false ),
    _language( english ),
    _sectionOffset( VYD_SECTION_OFFSET ),
    _output( stream ),
    _sectionOnly( sectionOnly ),
    _isInfoSaved( false )
{
  initialize();
}


void SaveGenerator::initialize()
{
  if ( ! _sectionOnly )
  {
    // write signature and version
    _output.writeShort( VYD_SIGNATURE );
    _output.writeShort( VYD_VERSION );
    // write dummy offset at VYD_SECTION_OFFSET (4)
    _output.writeLong( 0 );
    // write dummy offset at VYD_DEFINITION_OFFSET (8)
    _output.writeLong( 0 );
  }
}


unsigned SaveGenerator::sectionOffset() const
{
  // use offset+4 to get SectionGin data
  return _sectionOffset + 4;
}


void SaveGenerator::finish()
{
  finishSection( true );
  _output.close();
}


Generator & SaveGenerator::setTitle( const IString & title )
{
  _title = title;
  return *this;
}


Generator & SaveGenerator::setSubject( const IString & subject )
{
  _subject = subject;
  return *this;
}


Generator & SaveGenerator::setVersion( const IString & version )
{
  _version = version;
  return *this;
}


Generator & SaveGenerator::setCopyright( const IString & copyright )
{
  _copyright = copyright;
  return *this;
}


Generator & SaveGenerator::setAuthor( const IString & author )
{
  _author = author;
  return *this;
}


Generator & SaveGenerator::setLanguage( Language language )
{
  _language = language;
  return *this;
}


Generator & SaveGenerator::setCodePage( CodePage cp )
{
  _cp = cp;
  return *this;
}


Generator & SaveGenerator::addKeyword( const IString & keyword )
{
  if ( _keywords.length() )
    _keywords += ',';  // add comma separator between strings
  _keywords += keyword;
  return *this;
}


void SaveGenerator::handleSection( const SectionGin & section )
{
  finishSection( false );
  _output.writeString( section.title() );
  _output.writeVariable( section.level() );
  _output.writeBoolean( section.isListed() );
  _output.writeLong( section.windowId() );
}


void SaveGenerator::handleKeyword( const KeywordGin & keyword )
{
  _output.writeVariable( ginKeyword );
  _output.writeString( keyword.text() );
  _output.writeBoolean( keyword.isListed() );
  _output.writeBoolean( keyword.isExternal() );
}


void SaveGenerator::handleLabel( const LabelGin & label )
{
  _output.writeVariable( ginLabel );
  _output.writeString( label.name() );
  _output.writeBoolean( label.isExternal() );
  _output.writeLong( label.id() );
}


void SaveGenerator::handleRow( const RowGin & row )
{
  _output.writeVariable( ginRow );
}


void SaveGenerator::handleColumn( const ColumnGin & column )
{
  _output.writeVariable( ginColumn );
}


void SaveGenerator::handleItem( const ItemGin & item )
{
  _output.writeVariable( ginItem );
}


void SaveGenerator::handlePara( const ParaGin & para )
{
  _output.writeVariable( ginPara );
}


void SaveGenerator::handleLine( const LineGin & line )
{
  _output.writeVariable( ginLine );
}


void SaveGenerator::handlePush( const PushGin & push )
{
  _output.writeVariable( ginPush );
}


void SaveGenerator::handlePop( const PopGin & pop )
{
  _output.writeVariable( ginPop );
}


void SaveGenerator::handleRightMargin( const RightMarginGin & margin )
{
  _output.writeVariable( ginRightMargin );
  _output.writeBoolean( margin.isRelative() );
  writeDistance( margin.margin() );
}


void SaveGenerator::handleLeftMargin( const LeftMarginGin & margin )
{
  _output.writeVariable( ginLeftMargin );
  _output.writeBoolean( margin.isRelative() );
  writeDistance( margin.margin() );
}


void SaveGenerator::handleBorder( const BorderGin & border )
{
  _output.writeVariable( ginBorder );
  _output.writeShort( border.borders() );
}


void SaveGenerator::handleAlign( const AlignGin & align )
{
  _output.writeVariable( ginAlign );
  _output.writeVariable( align.alignment() );
}


void SaveGenerator::handleSpacing( const SpacingGin & spacing )
{
  _output.writeVariable( ginSpacing );
  writeDistance( spacing.spacing() );
}


void SaveGenerator::handleRuler( const RulerGin & ruler )
{
  _output.writeVariable( ginRuler );
  _output.writeVariable( ruler.mode() );
  if ( ruler.mode() == RulerGin::dlist )
    _output.writeVariable( ruler.lineBreak() );
  _output.writeVariable( ruler.numberOfColumns() );
  for ( INumber i = 1; i <= ruler.numberOfColumns(); i++ )
    writeDistance( ruler.column( i ) );
}


void SaveGenerator::handleOutline( const OutlineGin & outline )
{
  _output.writeVariable( ginOutline );
  _output.writeVariable( outline.style() );
  _output.writeString( outline.prefix() );
  _output.writeString( outline.suffix() );
}


void SaveGenerator::handleDivider( const DividerGin & divider )
{
  _output.writeVariable( ginDivider );
}


void SaveGenerator::handleFont( const FontGin & font )
{
  _output.writeVariable( ginFont );
  _output.writeVariable( font.family() );
  _output.writeString( font.facename() );
  _output.writeLong( font.codepage() );
}


void SaveGenerator::handleSize( const SizeGin & size )
{
  _output.writeVariable( ginSize );
  writeDistance( size.height() );
}


void SaveGenerator::handleColor( const ColorGin & color )
{
  _output.writeVariable( ginColor );
  writeColor( color.color() );
}


void SaveGenerator::handleBackColor( const BackColorGin & color )
{
  _output.writeVariable( ginBackColor );
  writeColor( color.color() );
}


void SaveGenerator::handleBold( const BoldGin & bold )
{
  _output.writeVariable( ginBold );
  _output.writeBoolean( bold.isOn() );
}


void SaveGenerator::handleItalic( const ItalicGin & italic )
{
  _output.writeVariable( ginItalic );
  _output.writeBoolean( italic.isOn() );
}


void SaveGenerator::handleUnderline( const UnderlineGin & underline )
{
  _output.writeVariable( ginUnderline );
  _output.writeBoolean( underline.isOn() );
}


void SaveGenerator::handleComment( const CommentGin & comment )
{
  _output.writeVariable( ginComment );
  _output.writeBoolean( comment.isOn() );
}


void SaveGenerator::handleLink( const LinkGin & link )
{
  _output.writeVariable( ginLink );
  _output.writeBoolean( link.isOn() );
  if ( link.isOn() )
  {
    _output.writeString( link.target() );
    _output.writeBoolean( link.isExternal() );
    if ( link.isExternal() )
      _output.writeString( link.file() );
  }
}


void SaveGenerator::handleHide( const HideGin & hide )
{
  _output.writeVariable( ginHide );
  _output.writeBoolean( hide.isOn() );
  if ( hide.isOn() )
    _output.writeString( hide.key() );
}


void SaveGenerator::handleArt( const ArtGin & art )
{
  _output.writeVariable( ginArt );
  _output.writeString( art.id() );
  _output.writeVariable( art.spot() );
}


void SaveGenerator::handleText( const TextGin & text )
{
  _output.writeVariable( ginText );
  _output.writeString( text.string() );
}


void SaveGenerator::handleSymbol( const SymbolGin & symbol )
{
  _output.writeVariable( ginSymbol );
  unsigned n = symbol.symbol().id();
  if ( n )
  {
    // ID known
    _output.writeVariable( 0 );
    _output.writeShort( n );
  }
  else
  {
    // no ID, use char + codepage
    unsigned char ch;
    CodePage cp;
    symbol.symbol().getDetails( ch, cp );
    _output.writeVariable( ch );
    _output.writeShort( short(cp) );
  }
}


void SaveGenerator::writeDistance( const Distance & distance )
{
  _output.writeLong( distance.length() );
  _output.writeVariable( distance.units() );
}


void SaveGenerator::writePosition( const Position & position )
{
  _output.writeBoolean( position.isDynamic() );
  if ( position.isDynamic() )
    _output.writeVariable( position.asDynamic() );
  else
    writeDistance( position.asDistance() );
}


void SaveGenerator::writeColor( const IColor & color )
{
  _output.writeVariable( color.redMix() );
  _output.writeVariable( color.greenMix() );
  _output.writeVariable( color.blueMix() );
}


void SaveGenerator::finishSection( Boolean allDone )
{
  // special handling for section-only output
  if ( _sectionOnly )
  {
    // send ginNone to signal end of section
    if ( allDone )
      _output.writeVariable( ginNone );
    return;
  }

  if ( _isInfoSaved )
  {
    // send ginNone to signal end of current section
    _output.writeVariable( ginNone );
  }
  else
  {
    //
    // WRITE DOCUMENT INFO
    //
    _output.writeString( _title );
    _output.writeString( _subject );
    _output.writeString( _version );
    _output.writeString( _copyright );
    _output.writeString( _author );
    _output.writeVariable( _language );
    _output.writeLong( _cp );
    _output.writeString( _keywords );

    _isInfoSaved = true;
  }

  if ( allDone )
  {
    //
    // WRITE DEFINITIONS
    //
    // save offset for definitions
    _output.flush();
    unsigned offset = _output.offset();
    _output.setOffset( VYD_DEFINITION_OFFSET );
    _output.writeLong( offset );

    // return to current offset
    _output.setOffset( offset );

    // window definitions
    INumber i;
    _output.writeVariable( numberOfWindows() );
    for ( i = 1; i <= numberOfWindows(); i++ )
    {
      const WindowDef & win = window( i );
      _output.writeLong( win.id() );
      _output.writeString( win.title() );
      writePosition( win.originX() );
      writePosition( win.originY() );
      writeDistance( win.width() );
      writeDistance( win.height() );
      _output.writeVariable( win.border() );
      _output.writeVariable( win.scrolling() );
      _output.writeBoolean( win.hasTitleBar() );
      _output.writeBoolean( win.hasMinimize() );
      _output.writeBoolean( win.hasMaximize() );
      _output.writeBoolean( win.hasSystemMenu() );
    }

    // artwork definitions
    _output.writeVariable( numberOfArts() );
    if ( numberOfArts() )
    {
      // allocate temporary filename for bitmap contents (a BMP file)
      Filename filename( Filename::temporary );
      for ( i = 1; i <= numberOfArts(); i++ )
      {
        const ArtDef & artdef = art( i );
        artdef.saveBitmap( filename, true );
        _output.writeString( artdef.id() );
        _output.writeBinary( filename );
      }
      filename.remove();
    }
  }
  else
  {
    //
    // UPDATE SECTION LINKS
    //
    // save offset of next section
    _output.flush();
    unsigned offset = _output.offset();
    _output.setOffset( _sectionOffset );
    _output.writeLong( offset );

    // write new offset pointer (initialize to 0)
    _output.setOffset( offset );
    _output.writeLong( 0 );
    _sectionOffset = offset;
  }
}

