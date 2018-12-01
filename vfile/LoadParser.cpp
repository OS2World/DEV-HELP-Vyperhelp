/***************************************************************************
 * File...... LoadParser.cpp
 * Author.... Mat
 * Date...... 11/20/97
 *
 * Implementation of LoadParser.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>

// MekTek
#include "MekTek.hpp"

// Generator
#include "Filename.hpp"
#include "ParserIndicator.hpp"

// VFile
#include "vyd.h"
#include "LoadGenerator.hpp"
#include "LoadParser.hpp"


/***************************************************************************
 * Procedure.. LoadParser::LoadParser
 * Author..... Mat
 * Date....... 2/11/99
 *
 * Constructor.  offset determines if this is a full file, partial file, or a
 * section within a file, as follows:
 *   -1: parse full VYD file
 *    0: section-only in this file/stream (temporary holding file)
 *   >0: parse single section from a full file
 ***************************************************************************/
LoadParser::LoadParser( const IString & filename, int offset ):
    _input( filename ),
    _gen( 0 ),
    _version( VYD_VERSION ),
    _sectionOffset( offset ),
    _sectionOnly( offset != -1 )
{
  initialize();
}

LoadParser::LoadParser( istream & stream, int offset ):
    _input( stream ),
    _gen( 0 ),
    _version( VYD_VERSION ),
    _sectionOffset( offset ),
    _sectionOnly( offset != -1 )
{
  initialize();
}


void LoadParser::initialize()
{
  // get signature and version if this is a full VYD file
  if ( _sectionOffset != 0 )
  {
    // get signature
    short signature = _input.readShort();
    if ( signature != VYD_SIGNATURE )
      ITHROW( IInvalidRequest( "Improper VYD file signature", 0 ) );

    // get version
    _version = _input.readShort();
    if ( _version == 0 || _version > VYD_VERSION )
      ITHROW( IInvalidRequest( "Improper VYD file version", 0 ) );
  }

  if ( ! _sectionOnly )
  {
    // get offset of first section
    _sectionOffset = _input.readLong();
  }
}


void LoadParser::load( LoadGenerator & gen )
{
  MEKASSERT( ! _sectionOnly );

  // set total size for progress indicator
  if ( indicator() )
    indicator()->setTotal( _input.size() );

  // load global info and definitions
  _gen = &gen;
  loadDocument();

  // scan all sections
  while ( _sectionOffset )
  {
    // get offset of next section
    unsigned nextSectionOffset = _input.readLong();

    // register this section (use offset+4 to get SectionGin data)
    gen.registerSection( _sectionOffset + 4, readSection() );

    // jump ahead to next section
    _sectionOffset = nextSectionOffset;
    _input.setOffset( _sectionOffset );

    // update indicator
    if ( indicator() )
      indicator()->setCurrent( _sectionOffset );
  }

  // finish output
  gen.finish();
}


void LoadParser::loadSection( Generator & gen )
{
  MEKASSERT( _sectionOnly );
  _gen = &gen;
  _input.setOffset( _sectionOffset );
  loadSection( true );
  gen.finish();
}


void LoadParser::generateSection( Generator & gen )
{
  MEKASSERT( _sectionOnly );
  _gen = &gen;
  _input.setOffset( _sectionOffset );
  loadSection( false );
}


void LoadParser::generate( Generator & gen )
{
  MEKASSERT( ! _sectionOnly );

  // set total size for progress indicator
  if ( indicator() )
    indicator()->setTotal( _input.size() );

  // load global info and definitions
  _gen = &gen;
  loadDocument();

  // load all sections
  while ( _sectionOffset )
  {
    // load section
    _sectionOffset = _input.readLong();
    loadSection( true );

    // update indicator
    if ( indicator() )
      indicator()->setCurrent( _sectionOffset );
  }

  // finish output
  gen.finish();
}


void LoadParser::loadDocument()
{
  // get offset to definitions
  unsigned definitionOffset = _input.readLong();

  // load document info
  _gen->setTitle( _input.readString() );
  _gen->setSubject( _input.readString() );
  _gen->setVersion( _input.readString() );
  _gen->setCopyright( _input.readString() );
  _gen->setAuthor( _input.readString() );
  _gen->setLanguage( Generator::Language( _input.readVariable() ) );
  if ( _version >= 2 )
    _gen->setCodePage( CodePage( _input.readLong() ) );

  // load keywords (comma separated)
  IString keywords = _input.readString();
  unsigned index = 1;
  unsigned nextIndex;
  do
  {
    nextIndex = keywords.indexOf( ',', index );
    if ( nextIndex )
    {
      _gen->addKeyword( keywords.subString( index, nextIndex - index ) );
      index = nextIndex + 1;
    }
  } while ( nextIndex );
  if ( index <= keywords.length() )
    _gen->addKeyword( keywords.subString( index ) );

  // jump to definition area
  _input.setOffset( definitionOffset );

  // load window definitions
  int n = _input.readVariable();  // number of window defintions
  while ( n-- )
  {
    int id = _input.readLong();
    IString title = _input.readString();
    Position originX = readPosition();
    Position originY = readPosition();
    Distance width = readDistance();
    Distance height = readDistance();
    WindowDef::Border border = WindowDef::Border( _input.readVariable() );
    WindowDef::Scrolling scrolling = WindowDef::Scrolling( _input.readVariable() );
    Boolean hasTitleBar = _input.readBoolean();
    Boolean hasMinimize = _input.readBoolean();
    Boolean hasMaximize = _input.readBoolean();
    Boolean hasSystemMenu = _input.readBoolean();
    _gen->defineWindow( WindowDef(
        id,
        title,
        originX,
        originY,
        width,
        height,
        border,
        scrolling,
        hasTitleBar,
        hasMinimize,
        hasMaximize,
        hasSystemMenu ) );
  }

  // load artwork definitions
  n = _input.readVariable();  // number of art defintions
  if ( n )
  {
    // load each bitmap into a temporary file, then create IGBitmap from it
    Filename filename( Filename::temporary );
    while ( n-- )
    {
      IString id = readArtId();
      _input.readBinary( filename );
      _gen->defineArt( ArtDef( id, IGBitmap( filename, IGBitmap::Bitmap ) ) );
    }
    filename.remove();
  }

  // jump back to first section
  if ( _sectionOffset )
    _input.setOffset( _sectionOffset );
}


IString LoadParser::readArtId()
{
  // before version 5, art id was a long
  return ( _version < 5 )? IString( _input.readLong() ): _input.readString();
}


Distance LoadParser::readDistance()
{
  int length = _input.readLong();
  Distance::Units units = Distance::Units( _input.readVariable() );
  return Distance( length, units );
}


Position LoadParser::readPosition()
{
  Boolean isDynamic = _input.readBoolean();
  if ( isDynamic )
    return Position( Position::Dynamic( _input.readVariable() ) );
  else
    return Position( readDistance() );
}


SectionGin LoadParser::readSection()
{
  IString title = _input.readString();
  int level = _input.readVariable();
  Boolean isListed = _input.readBoolean();
  int windowId = _input.readLong();

  SectionGin section( level, isListed );
  section.setTitle( title );
  section.setWindow( windowId );

  return section;
}


void LoadParser::loadSection( Boolean includeHeader )
{
  // send the SectionGin
  if ( includeHeader )
    *_gen << readSection();

  // send all Gins that follow (end of list is ginNone)
  GinType type;
  do
  {
    type = GinType( _input.readVariable() );
    switch ( type )
    {
      case ginKeyword:
        loadKeyword();
        break;
      case ginLabel:
        loadLabel();
        break;
      case ginRow:
        loadRow();
        break;
      case ginColumn:
        loadColumn();
        break;
      case ginItem:
        loadItem();
        break;
      case ginPara:
        loadPara();
        break;
      case ginLine:
        loadLine();
        break;
      case ginPush:
        loadPush();
        break;
      case ginPop:
        loadPop();
        break;
      case ginRightMargin:
        loadRightMargin();
        break;
      case ginLeftMargin:
        loadLeftMargin();
        break;
      case ginBorder:
        loadBorder();
        break;
      case ginAlign:
        loadAlign();
        break;
      case ginSpacing:
        loadSpacing();
        break;
      case ginRuler:
        loadRuler();
        break;
      case ginOutline:
        loadOutline();
        break;
      case ginDivider:
        loadDivider();
        break;
      case ginFont:
        loadFont();
        break;
      case ginSize:
        loadSize();
        break;
      case ginColor:
        loadColor();
        break;
      case ginBackColor:
        loadBackColor();
        break;
      case ginBold:
        loadBold();
        break;
      case ginItalic:
        loadItalic();
        break;
      case ginUnderline:
        loadUnderline();
        break;
      case ginComment:
        loadComment();
        break;
      case ginLink:
        loadLink();
        break;
      case ginHide:
        loadHide();
        break;
      case ginArt:
        loadArt();
        break;
      case ginText:
        loadText();
        break;
      case ginSymbol:
        loadSymbol();
        break;
    }
  } while ( type != ginNone );
}


void LoadParser::loadKeyword()
{
  IString text = _input.readString();
  Boolean isListed = _input.readBoolean();
  Boolean isExternal = _input.readBoolean();
  KeywordGin keyword( text, isListed );
  keyword.setExternal( isExternal );
  *_gen << keyword;
}


void LoadParser::loadLabel()
{
  IString name = _input.readString();
  Boolean isExternal = _input.readBoolean();
  int id = _input.readLong();
  LabelGin label( name );
  label.setExternal( isExternal );
  label.setId( id );
  *_gen << label;
}


void LoadParser::loadRow()
{
  *_gen << RowGin();
}


void LoadParser::loadColumn()
{
  *_gen << ColumnGin();
}


void LoadParser::loadItem()
{
  *_gen << ItemGin();
}


void LoadParser::loadPara()
{
  *_gen << ParaGin();
}


void LoadParser::loadLine()
{
  *_gen << LineGin();
}


void LoadParser::loadPush()
{
  *_gen << PushGin();
}


void LoadParser::loadPop()
{
  *_gen << PopGin();
}


void LoadParser::loadRightMargin()
{
  Boolean isRelative = _input.readBoolean();
  Distance distance = readDistance();
  RightMarginGin margin( isRelative, distance );
  *_gen << margin;
}


void LoadParser::loadLeftMargin()
{
  Boolean isRelative = _input.readBoolean();
  Distance distance = readDistance();
  LeftMarginGin margin( isRelative, distance );
  *_gen << margin;
}


void LoadParser::loadBorder()
{
  BorderGin border( _input.readShort() );
  *_gen << border;
}


void LoadParser::loadAlign()
{
  AlignGin align( AlignGin::Alignment( _input.readVariable() ) );
  *_gen << align;
}


void LoadParser::loadSpacing()
{
  SpacingGin spacing( readDistance() );
  *_gen << spacing;
}


void LoadParser::loadRuler()
{
  // create ruler
  RulerGin::Mode mode = RulerGin::Mode( _input.readVariable() );
  RulerGin ruler( mode );

  // add line break mode if necessary
  if ( mode == RulerGin::dlist )
  {
    RulerGin::LineBreak lineBreak = RulerGin::LineBreak( _input.readVariable() );
    ruler.setLineBreak( lineBreak );
  }

  // load columns
  unsigned n = _input.readVariable();
  while ( n-- )
    ruler.addColumn( readDistance() );

  // send the ruler
  *_gen << ruler;
}


void LoadParser::loadOutline()
{
  OutlineGin::Style style = OutlineGin::Style( _input.readVariable() );
  IString prefix = _input.readString();
  IString suffix = _input.readString();
  OutlineGin outline( style );
  outline.setPrefix( prefix );
  outline.setSuffix( suffix );
  *_gen << outline;
}


void LoadParser::loadDivider()
{
  *_gen << DividerGin();
}


void LoadParser::loadFont()
{
  FontInfo::Family family = FontInfo::Family( _input.readVariable() );
  IString facename = _input.readString();
  int codepage = _input.readLong();
  FontGin font( family, facename );
  font.setCodepage( codepage );
  *_gen << font;
}


void LoadParser::loadSize()
{
  *_gen << SizeGin( readDistance() );
}


void LoadParser::loadColor()
{
  *_gen << ColorGin( readColor() );
}


void LoadParser::loadBackColor()
{
  *_gen << BackColorGin( readColor() );
}


void LoadParser::loadBold()
{
  *_gen << BoldGin( _input.readBoolean() );
}


void LoadParser::loadItalic()
{
  *_gen << ItalicGin( _input.readBoolean() );
}


void LoadParser::loadUnderline()
{
  *_gen << UnderlineGin( _input.readBoolean() );
}


void LoadParser::loadComment()
{
  *_gen << CommentGin( _input.readBoolean() );
}


void LoadParser::loadLink()
{
  Boolean isOn = _input.readBoolean();
  if ( isOn )
  {
    // link ON
    IString target = _input.readString();
    LinkGin link( target );
    Boolean isExternal = _input.readBoolean();
    if ( isExternal )
      link.setFile( _input.readString() );
    *_gen << link;
  }
  else
  {
    // link OFF
    *_gen << LinkGin();
  }
}


void LoadParser::loadHide()
{
  Boolean isOn = _input.readBoolean();
  if ( isOn )
    *_gen << HideGin( _input.readString() );
  else
    *_gen << HideGin();
}


void LoadParser::loadArt()
{
  // get ID for art definition
  IString id = readArtId();

  // get placement info
  ArtGin::ArtSpot spot = ArtGin::ArtSpot( _input.readVariable() );

  // read size (no longer supported), check for 100% = 'fit'
  if ( _version < 4 )
  {
    Boolean hasSize = _input.readBoolean();
    if ( hasSize )
    {
      Distance x = readDistance();
      Distance y = readDistance();
      if ( ( x.length() >= 100 && x.units() == Distance::percent )
        && ( y.length() >= 100 && y.units() == Distance::percent ) )
      {
        // this is equivalent to the current 'fit' mode
        spot = ArtGin::fit;
      }
    }
  }

  // send the ArtGin
  *_gen << ArtGin( id, spot );
}


void LoadParser::loadText()
{
  *_gen << _input.readString();
}


void LoadParser::loadSymbol()
{
  if ( _version < 3 )
  {
    //
    // before version 3, the codepage was not stored
    //
    unsigned n = _input.readShort();
    if ( n >= Symbol::startOfList )
    {
      // symbol from identifier
      Symbol::Identifier id = Symbol::Identifier( n );
      Symbol symbol( id );
      *_gen << symbol;
    }
    else
    {
      // symbol from char
      char c = char( n );
      Symbol symbol( c );
      *_gen << symbol;
    }
  }
  else
  {
    //
    // starting with version 3, codepage is stored
    //
    unsigned char ch = _input.readVariable();
    if ( ch )
    {
      // char + codepage stored
      // symbol from char
      CodePage cp( _input.readShort() );
      Symbol symbol( ch, cp );
      *_gen << symbol;
    }
    else
    {
      // symbol from identifier
      Symbol::Identifier id = Symbol::Identifier( _input.readShort() );
      Symbol symbol( id );
      *_gen << symbol;
    }
  }
}


IColor LoadParser::readColor()
{
  unsigned char red = _input.readVariable();
  unsigned char green = _input.readVariable();
  unsigned char blue = _input.readVariable();
  return IColor( red, green, blue );
}


