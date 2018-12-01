/***************************************************************************
 * File...... IpfYacc.cpp
 * Author.... Mat
 *
 * Implementation for IPF parser
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // ITHROW macros
#include <ifont.hpp>        // IFont
#include <igbitmap.hpp>     // IGBitmap

// MekTek
#include "MekTek.hpp"

// Generator
#include "ParserIndicator.hpp"

// IPF Parser
#include "IpfParser.hpp"
#include "IpfYacc.hpp"
#define ALexClass  IpfLexUnused   // change default names, avoid multiple definitions
#define AYaccClass IpfYaccUnused  // these are unused anyways
#include "ipf.yhh"       // generated from rule file


// static defaults (from IPF reference)
ArtGin::ArtSpot      IpfYacc::_defaultArtSpot = ArtGin::left;
IString              IpfYacc::_defaultCautionText( "CAUTION:" );
RulerGin::LineBreak  IpfYacc::_defaultDlLineBreak = RulerGin::none;
int                  IpfYacc::_defaultTermSizeChars = 10;
AlignGin::Alignment  IpfYacc::_defaultAlignment = AlignGin::left;
Position             IpfYacc::_defaultX( Position::min );
Position             IpfYacc::_defaultY( Position::min );
Distance             IpfYacc::_defaultWidth( 100, Distance::percent );
Distance             IpfYacc::_defaultHeight( 100, Distance::percent );
WindowDef::Border    IpfYacc::_defaultBorder( WindowDef::sizeBorder );
WindowDef::Scrolling IpfYacc::_defaultScrolling( WindowDef::allScroll );
IpfYacc::BarType     IpfYacc::_defaultBar( IpfYacc::fullBar );
IpfYacc::LinkType    IpfYacc::_defaultLinkType = IpfYacc::unknownLink;
unsigned             IpfYacc::_defaultMargin = 1;
IString              IpfYacc::_defaultNoteText( "Note:" );
RulerGin::LineBreak  IpfYacc::_defaultParmlLineBreak = RulerGin::all;
int                  IpfYacc::_defaultBorders = BorderGin::all;
IString              IpfYacc::_defaultWarningText( "Warning:" );

// const data
static const char ipfTable[] = "ipf";


/***************************************************************************
 * Procedure.. IpfYacc::IpfYacc()
 * Author..... Mat
 * Date....... 12/21/95
 *
 * IpfYacc constructor loads Lex and Yacc tables and sets file to parse.
 ***************************************************************************/
IpfYacc::IpfYacc( const char* fileToParse, Boolean isTopLevel ) :
    EnhancedYacc( ipfTable, fileToParse ),
    _isTopLevel( isTopLevel ),
    _color( IColor::black ),
    _isColorSet( false )
{}


/***************************************************************************
 * Procedure.. IpfYacc::IpfYacc()
 * Author..... Mat
 * Date....... 12/21/95
 *
 * IpfYacc constructor loads Lex and Yacc tables and sets consumer to parse.
 ***************************************************************************/
IpfYacc::IpfYacc( SSLexConsumer & consumer ) :
    EnhancedYacc( ipfTable, consumer ),
    _isTopLevel( false ),
    _color( IColor::black ),
    _isColorSet( false )
{}


/***************************************************************************
 * Procedure.. IpfYacc::error
 * Author..... Mat
 * Date....... 10/13/98
 *
 * This function is called by VisualParse whenever a parsing error occurs.
 ***************************************************************************/
SSBooleanValue IpfYacc::error( SSUnsigned32 qulState, SSLexLexeme & qLexeme)
{
  // construct warning text (pass 1 only)
  if ( _pass == 1)
  {
    IString text;
    if ( qLexeme.token() == ALexBadAttr)
       text = IString( "Invalid attribute" );
    else
       text = IString( "Syntax error" );
    warning( text, &qLexeme );
  }

  // standard error recovery
  return errorToken();
}


/***************************************************************************
 * Procedure.. IpfYacc::reduce()
 * Author..... Mat
 * Date....... 12/21/95
 *
 * The reduce function is called for every production in the Yacc grammar.
 * The productions are defined in the .yhh created by the VP compiler.
 * When the rule file is changed, new cases should be added here from the
 * examples generated in the .yhh file.
 *
 * NOTE: different passes through the file have different behavior, as
 * defined by pass1() and pass2(). common() defines behavior supported
 * by both passes.
 ***************************************************************************/
SSYaccStackElement* IpfYacc::reduce( SSUnsigned32 production, SSUnsigned32 size)
{
  MEKASSERT( _pass >= 1 && _pass <= 2 );

  // pass-specific processing
  if ( _pass == 1 )
    pass1( production );
  else
    pass2( production );

  // common processing (all passes)
  common( production );

  return stackElement();
}


/***************************************************************************
 * Procedure.. IpfYacc::pass1
 * Author..... Mat
 * Date....... 10/29/98
 *
 * Handle a production for pass 1.  Pass 1 is used to:
 *  - build a collection of panel resource numbers with their corresponding
 *    label names.  This is used to resolve link numbers with names.
 *  - build a collection of primary index entries.  This is used to create
 *    text for secondary index entries.
 *  - build a collection of index synonyms.  This is used to create a set
 *    of un-listed keywords when roots/synonyms are used.
 *  - warn of syntax errors
 ***************************************************************************/
void IpfYacc::pass1( SSUnsigned32 production )
{
  switch ( production )
  {
    case AYaccH1:
    // tag -> H1 hnAttrs DotWhite extraText
    case AYaccH2:
    // tag -> H2 hnAttrs DotWhite extraText
    case AYaccH3:
    // tag -> H3 hnAttrs DotWhite extraText
    case AYaccH4:
    // tag -> H4 hnAttrs DotWhite extraText
    case AYaccH5:
    // tag -> H5 hnAttrs DotWhite extraText
    case AYaccH6:
    // tag -> H6 hnAttrs DotWhite extraText
      prepareSection();
      break;

    case AYaccHnAttrRes:
    // hnAttr -> Res Equal Num
      _id = lexemeAsULong( 2, true );
      break;

    case AYaccHnAttrId:
    // hnAttr -> Id Equal anyParm
    case AYaccHnAttrName:
    // hnAttr -> Name Equal anyParm
      // support two different label names
      if ( ! _name.length() )
        setValue( _name );
      break;

    case AYaccI1:
    // tag -> I1 i1Attrs DotWhite extraText
      prepareIndex();
      break;

    case AYaccI1AttrId:
    // i1Attr -> Id Equal anyParm
      setValue( _name );
      break;

    case AYaccISyn:
    // tag -> Isyn Root Equal anyParm DotWhite extraText
      prepareSynonym();
      break;

    case AYaccBadControl:
      warning( "Invalid control word" );
      break;

    case AYaccEmptyControl:
      warning( "Empty control word" );
      break;

    case AYaccItemEmptyTag:
      warning( "Empty tag" );
      break;

    case AYaccItemBadTag:
      warning( "Invalid tag" );
      break;

  }
}


/***************************************************************************
 * Procedure.. IpfYacc::prepareSection
 * Author..... Mat
 * Date....... 10/29/98
 *
 * Called during pass 1 to handle a section.  The resource ID is associated
 * with the panel id/name for linking.  If no name was given, one is
 * assigned based on the supplied resource ID.
 ***************************************************************************/
void IpfYacc::prepareSection()
{
  MEKASSERT( _pass == 1 );

  // insert a progress indicator
  if ( _isTopLevel && _parser->indicator() )
    _parser->indicator()->setCurrent( offset() );

  // only need to registers panels which have resource IDs
  if ( _id )
  {
    // generate a name, if necessary
    if ( ! _name.length() )
      _name = IString("res_") + IString( _id ).rightJustify( 5, '0' );
    _parser->addPanelLink( PanelLink( _name, _id ) );
  }

  // reset variables
  _id = 0;
  _name = IString();
}


/***************************************************************************
 * Procedure.. IpfYacc::prepareIndex
 * Author..... Mat
 * Date....... 10/29/98
 *
 * Called during pass 1 to record a primary index name and text.
 ***************************************************************************/
void IpfYacc::prepareIndex()
{
  MEKASSERT( _pass == 1 );

  _parser->addEntry( StringPair( _name, _extra ) );
  _name = IString();
}


/***************************************************************************
 * Procedure.. IpfYacc::prepareSynonym
 * Author..... Mat
 * Date....... 10/29/98
 *
 * Called during pass 1 to record a list of index synonyms.
 ***************************************************************************/
void IpfYacc::prepareSynonym()
{
  MEKASSERT( _pass == 1 );

  _parser->addSynonyms( _value, _extra );
  _value = IString();
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    void prepareSynonym();


/***************************************************************************
 * Procedure.. IpfYacc::pass2
 * Author..... Mat
 * Date....... 10/29/98
 *
 * Handle a production for pass 2.  Pass 2 does nost of the work, but uses
 * some of the tables set up in pass 1.
 ***************************************************************************/
void IpfYacc::pass2( SSUnsigned32 production )
{
  switch ( production )
  {
    case AYaccUserdoc:
    // tag -> Userdoc Dot;
      _parser->setMode( IpfParser::limbo );
      break;

    case AYaccEUserdoc:
    // tag -> Euserdoc Dot;
      _parser->finishSection();
      _parser->setMode( IpfParser::front );
      break;

    case AYaccItemChunk:
    // item -> Chunk
    case AYaccItemWhite:
    // item -> White
      handleText( lexemeAsString( 0 ) );
      break;

    case AYaccItemEnd:
    // item -> EndOfLine
      handleNewLine();
      break;

    case AYaccItemEmpty:
    // item -> EmptyLine
      handleNewLine( true );
      break;

    case AYaccItemSymbol:
    // item -> Symbol
      handleSymbol( lexemeAsString( 0 ) );
      break;

    case AYaccItemDot:
    // item -> Dot
      handleSymbol( Symbol( Symbol::per ) );
      break;

    case AYaccItemColonWhite:
    // item -> ColonWhite
      handleSymbol( Symbol( Symbol::colon ) );
      handleText( lexemeAsString( 0 ) );
      break;

    case AYaccItemColonEnd:
      handleSymbol( Symbol( Symbol::colon ) );
      handleNewLine();
    // item -> ColonEnd
      break;

    case AYaccComment:
    // control -> Comment
      // ignore comment text
      _parser->clearLineFlag();
      break;

    case AYaccImbed:
    // control -> Im Fname
      _parser->clearLineFlag();
      break;

    case AYaccNameit:
    // control -> Nameit nameAttrs
      handleNameit();
      break;

    case AYaccNameitSymbol:
    // nameAttr -> NameSymbol Equal nameValue
      _name = _name2;
      break;

    case AYaccNameitText:
    // nameAttr -> NameText Equal nameValue
      _nameitText = _name2;
      break;

    case AYaccNameValue:
    // nameValue -> NameValue
    case AYaccNameQuote:
    // nameValue -> NameQuote
      _name2 = lexemeAsString( 0 );
      break;

    case AYaccControlBreak:
    // control -> Br
      handleBreak();
      break;

    case AYaccControlCenter:
    // control -> Ce centers
      endCentered();
      break;

    case AYaccCenterText:
    // center -> CenterText
    case AYaccCenterWhite:
    // center -> CenterWhite
      beginCentered();
      handleText( lexemeAsString( 0 ) );
      break;

    case AYaccCenterSymbol:
    // center -> CenterSymbol
      beginCentered();
      handleSymbol( lexemeAsString( 0 ) );
      break;

    case AYaccArtlink:
    // tag -> Artlink Dot
      _parser->pushMode( IpfParser::artlink );
      break;

    case AYaccEArtlink:
    // tag -> Eartlink Dot
      _parser->popMode( IpfParser::artlink );
      break;

    case AYaccArtwork:
    // tag -> Artwork artAttrs Dot
      handleArtwork();
      break;

    case AYaccArtworkAttrName:
    // artAttr -> Name Equal anyParm
      setValue( _artFile );
      break;

    case AYaccArtworkAttrAlignLeft:
    // artAttr -> Align Equal Left
      _artSpot = ArtGin::left;
      break;

    case AYaccArtworkAttrAlignRight:
    // artAttr -> Align Equal Right
      _artSpot = ArtGin::right;
      break;

    case AYaccArtworkAttrAlignCent:
    // artAttr -> Align Equal Center
      _artSpot = ArtGin::center;
      break;

    case AYaccArtworkAttrLinkfile:
    // artAttr -> Linkfile Equal anyParm
      // process imbedded file (deferred until :artwork processing)
      setValue( _externalFile );
      break;

    case AYaccArtworkAttrRunin:
    // artAttr -> Runin
      _artSpot = ArtGin::bottom;
      break;

    case AYaccArtworkAttrFit:
    // artAttr -> FitAttr
      _artSpot = ArtGin::fit;
      break;

    case AYaccC:
    // tag -> C Dot
      {
        Boolean isFirstRow;
        if ( ! _parser->tableColumn( isFirstRow ) )
          warning( "Unexpected table column" );
        if ( ! isFirstRow )
          _parser->sendGin( new ColumnGin() );
      }
      break;

    case AYaccCaution:
    // tag -> Caution cautionAttrs Dot
      // this tag resets bold, italic and underline
      _parser->sendPara();
      _parser->setItalic( false );
      _parser->setUnderline( false );
      _parser->setBold( true );
      _parser->sendText( _cautionText, false );
      _parser->setBold( false );
      _parser->sendLine();
      _cautionText = _defaultCautionText;
      break;

    case AYaccCautionAttrs:
    // cautionAttrs -> Text Equal anyParm
      setValue( _cautionText );
      break;

    case AYaccECaution:
    // tag -> Ecaution Dot
      _parser->sendPara();
      break;

    case AYaccCgraphic:
    // tag -> Cgraphic Dot
      _parser->sendPara();
      _parser->sendGin( new PushGin() );
      _parser->sendGin( new RulerGin( RulerGin::preformat ) );
      _parser->resetFont( FontInfo::mono );
      _parser->pushMode( IpfParser::literal );
      break;

    case AYaccECgraphic:
    // tag -> Ecgraphic Dot
      _parser->handlePop();
      _parser->resetFont( FontInfo::system );
      _parser->popMode( IpfParser::literal );
      break;

    case AYaccColorAttrFc:
    // colorAttr -> Fc Equal color
      if ( _isColorSet )
        _parser->setForegroundColor( _color );
      else
        _parser->setForegroundColor( IDeviceColor( IDeviceColor::defaultColor ) );
      _isColorSet = false;
      break;

    case AYaccColorAttrBc:
    // colorAttr -> Bc Equal color
      if ( _isColorSet )
        _parser->setBackgroundColor( _color );
      else
        _parser->setBackgroundColor( IDeviceColor( IDeviceColor::background ) );
      _isColorSet = false;
      break;

    case AYaccColorDefault:
    // color -> Default
      // don't set color for 'default' case, so appropriate default color is used
      break;

    case AYaccColorBlue:
    // color -> Blue
      _color = IColor( IColor::blue );
      _isColorSet = true;
      break;

    case AYaccColorCyan:
    // color -> Cyan
      _color = IColor( IColor::cyan );
      _isColorSet = true;
      break;

    case AYaccColorGreen:
    // color -> Green
      _color = IColor( IColor::green );
      _isColorSet = true;
      break;

    case AYaccColorNeutral:
    // color -> Neutral
      _color = IColor( IColor::brown );
      _isColorSet = true;
      break;

    case AYaccColorRed:
    // color -> Red
      _color = IColor( IColor::red );
      _isColorSet = true;
      break;

    case AYaccColorYellow:
    // color -> Yellow
      _color = IColor( IColor::yellow );
      _isColorSet = true;
      break;

    case AYaccColorBlack:
    // color -> Black
      _color = IColor( IColor::black );
      _isColorSet = true;
      break;

    case AYaccColorPink:
    // color -> Pink
      _color = IColor( IColor::pink );
      _isColorSet = true;
      break;

    case AYaccColorBrown:
    // color -> Brown
      _color = IColor( IColor::brown );
      _isColorSet = true;
      break;

    case AYaccColorDarkgray:
    // color -> Darkgray
      _color = IColor( IColor::darkGray );
      _isColorSet = true;
      break;

    case AYaccColorDarkblue:
    // color -> Darkblue
      _color = IColor( IColor::darkBlue );
      _isColorSet = true;
      break;

    case AYaccColorDarkred:
    // color -> Darkred
      _color = IColor( IColor::darkRed );
      _isColorSet = true;
      break;

    case AYaccColorDarkpink:
    // color -> Darkpink
      _color = IColor( IColor::darkPink );
      _isColorSet = true;
      break;

    case AYaccColorDarkgreen:
    // color -> Darkgreen
      _color = IColor( IColor::darkGreen );
      _isColorSet = true;
      break;

    case AYaccColorDarkcyan:
    // color -> Darkcyan
      _color = IColor( IColor::darkCyan );
      _isColorSet = true;
      break;

    case AYaccColorPalegray:
    // color -> Palegray
      _color = IColor( IColor::paleGray );
      _isColorSet = true;
      break;

    case AYaccDl:
    // tag -> Dl dlAttrs Dot
      handleDoubleList( _dlLineBreak, _defaultDlLineBreak );
      break;

    case AYaccDlAttrCompact:
    // dlAttr -> Compact
      _isCompact = true;
      break;

    case AYaccDlAttrTsize:
    // dlAttr -> Tsize Equal Num
      _termSizeChars = lexemeAsULong( 2, true );
      break;

    case AYaccDlAttrBreakNone:
    // dlAttr -> Break Equal None
      _dlLineBreak = RulerGin::none;
      break;

    case AYaccDlAttrBreakFit:
    // dlAttr -> Break Equal Fit
      _dlLineBreak = RulerGin::fit;
      break;

    case AYaccDlAttrBreakAll:
    // dlAttr -> Break Equal All
      _dlLineBreak = RulerGin::all;
      break;

    case AYaccEDl:
    // tag -> Edl Dot
      _parser->handlePop();
      break;

    case AYaccDd:
    // tag -> Dd Dot
      _parser->sendGin( new ColumnGin() );
      break;

    case AYaccDdhd:
    // tag -> Ddhd Dot
      _parser->sendGin( new ColumnGin() );
      break;

    case AYaccDt:
    // tag -> Dt Dot
      _parser->sendGin( new RowGin() );
      break;

    case AYaccDthd:
    // tag -> Dthd Dot
      _parser->sendGin( new RowGin() );
      break;

    case AYaccDocAttrToc:
    // docAttr -> Toc Equal Num
      _parser->setTocLevel( lexemeAsTocLevel() );
      break;

    case AYaccFig:
    // tag -> Fig Dot
      _parser->sendPara();
      _parser->sendGin( new PushGin() );
      _parser->sendGin( new RulerGin( RulerGin::preformat ) );
      _parser->pushMode( IpfParser::literal );
      break;

    case AYaccEFig:
    // tag -> Efig Dot
      _parser->handlePop();
      _parser->popMode( IpfParser::literal );
      break;

    case AYaccFigcap:
    // tag -> Figcap DotWhite extraText
      _parser->sendLine();
      _parser->sendText( _extra );
      _parser->sendPara();
      _parser->clearLineFlag();
      break;

    case AYaccFont:
    // tag -> Font fontAttrs Dot
      handleFont();
      break;

    case AYaccFontAttrFacename:
    // fontAttr -> Facename Equal anyParm
      setValue( _facename );
      break;

    case AYaccFontAttrSize:
    // fontAttr -> Size Equal Numfont
      // size will be in the form "HxW"
      // we ignore the width (W), and use the height to set the point size
      // height of 0 means use the default size
      _pointSize = lexemeAsULong( 2, true );
      break;

    case AYaccFontAttrCodePage:
    // fontAttr -> Codepage Equal Num
      _codepage = lexemeAsULong( 2, true );
      break;

    case AYaccFn:
    // tag -> FnTag Id Equal anyParm Dot
      // set mode, parser will queue contents for later sending
      setValue( _name );
      _parser->pushMode( IpfParser::footnote, &_name );
      _name = IString();
      break;

    case AYaccEFn:
    // tag -> Efn Dot
      _parser->popMode( IpfParser::footnote );
      break;

    case AYaccH1:
    // tag -> H1 hnAttrs DotWhite extraText
      handleSection( 1 );
      break;

    case AYaccH2:
    // tag -> H2 hnAttrs DotWhite extraText
      handleSection( 2 );
      break;

    case AYaccH3:
    // tag -> H3 hnAttrs DotWhite extraText
      handleSection( 3 );
      break;

    case AYaccH4:
    // tag -> H4 hnAttrs DotWhite extraText
      handleSection( 4 );
      break;

    case AYaccH5:
    // tag -> H5 hnAttrs DotWhite extraText
      handleSection( 5 );
      break;

    case AYaccH6:
    // tag -> H6 hnAttrs DotWhite extraText
      handleSection( 6 );
      break;

    case AYaccHnAttrTitlebarYes:
    // hnAttr -> Titlebar Equal Yes
      _bar = emptyBar;
      _isWindowSet = true;
      break;

    case AYaccHnAttrTitlebarNone:
    // hnAttr -> Titlebar Equal None
      _bar = noBar;
      _isWindowSet = true;
      break;

    case AYaccHnAttrTitlebarBoth:
    // hnAttr -> Titlebar Equal Both
      _bar = fullBar;
      _isWindowSet = true;
      break;

    case AYaccHnAttrTitlebarSys:
    // hnAttr -> Titlebar Equal Sysmenu
      _bar = sysmenuBar;
      _isWindowSet = true;
      break;

    case AYaccHnAttrTitlebarMin:
    // hnAttr -> Titlebar Equal Minmax
      _bar = minmaxBar;
      _isWindowSet = true;
      break;

    case AYaccHnAttrScrollHorz:
    // hnAttr -> Scroll Equal Horizontal
      _scrolling = WindowDef::horizScroll;
      _isWindowSet = true;
      break;

    case AYaccHnAttrScrollVert:
    // hnAttr -> Scroll Equal Vertical
      _scrolling = WindowDef::vertScroll;
      _isWindowSet = true;
      break;

    case AYaccHnAttrScrollBoth:
    // hnAttr -> Scroll Equal Both
      _scrolling = WindowDef::allScroll;
      _isWindowSet = true;
      break;

    case AYaccHnAttrScrollNone:
    // hnAttr -> Scroll Equal None
      _scrolling = WindowDef::noScroll;
      _isWindowSet = true;
      break;

    case AYaccHnAttrRulesBorder:
    // hnAttr -> RulesAttr Equal Border
      _border = WindowDef::staticBorder;
      _isWindowSet = true;
      break;

    case AYaccHnAttrRulesSize:
    // hnAttr -> RulesAttr Equal Sizeborder
      _border = WindowDef::sizeBorder;
      _isWindowSet = true;
      break;

    case AYaccHnAttrRulesNone:
    // hnAttr -> RulesAttr Equal None
      _border = WindowDef::noBorder;
      _isWindowSet = true;
      break;

    case AYaccHnAttrHide:
    // hnAttr -> Hide
      _isHidden = true;
      break;

    case AYaccHnAttrToc:
    // hnAttr -> Toc Equal Num
      _sectionTocLevel = lexemeAsTocLevel();
      break;

    case AYaccHnAttrRes:
    // hnAttr -> Res Equal Num
      _id = lexemeAsULong( 2, true );
      break;

    case AYaccHnAttrId:
    // hnAttr -> Id Equal anyParm
    case AYaccHnAttrName:
    // hnAttr -> Name Equal anyParm
      // support two different label names
      setValue( _name.length()? _name2: _name );
      break;

    case AYaccHnAttrGlobal:
    // hnAttr -> Global
      _isExternal = true;
      break;

    case AYaccHnAttrX:
    // hnAttr -> X Equal dynamic
      _x = _position;
      _isWindowSet = true;
      break;

    case AYaccHnAttrY:
    // hnAttr -> Y Equal dynamic
      _y = _position;
      _isWindowSet = true;
      break;

    case AYaccHnAttrWidth:
    // hnAttr -> Width Equal relative
      _width = _distance;
      _isWindowSet = true;
      break;

    case AYaccHnAttrHeight:
    // hnAttr -> Height Equal relative
      _height = _distance;
      _isWindowSet = true;
      break;

    case AYaccHdref:
    // tag -> Hdref hdrefAttrs Dot
      handleReference();
      break;

    case AYaccHdrefAttrRes:
    // hdrefAttr -> Res Equal Num
      _name = _parser->getPanelName( lexemeAsULong( 2, true ) );
      break;

    case AYaccHdrefAttrRefid:
    // hdrefAttr -> Refid Equal anyParm
      setValue( _name );
      break;

    case AYaccHide:
      handleHide();
      break;

    case AYaccEHide:
    // tag -> Ehide Dot
      _parser->handlePop();
      break;

    // NOTE: :hp. tags cannot be nested so we don't need to push/pop

    case AYaccHp1:
    // tag -> Hp1 Dot
    case AYaccEHp1:
    // tag -> Ehp1 Dot
      _parser->setItalic( production == AYaccHp1 );
      break;

    case AYaccHp2:
    // tag -> Hp2 Dot
    case AYaccEHp2:
    // tag -> Ehp2 Dot
      _parser->setBold( production == AYaccHp2 );
      break;

    case AYaccHp3:
    // tag -> Hp3 Dot
    case AYaccEHp3:
    // tag -> Ehp3 Dot
      _parser->setItalic( production == AYaccHp3 );
      _parser->setBold( production == AYaccHp3 );
      break;

    case AYaccHp5:
    // tag -> Hp5 Dot
    case AYaccEHp5:
    // tag -> Ehp5 Dot
      _parser->setUnderline( production == AYaccHp5 );
      break;

    case AYaccHp6:
    // tag -> Hp6 Dot
    case AYaccEHp6:
    // tag -> Ehp6 Dot
      _parser->setUnderline( production == AYaccHp6 );
      _parser->setItalic( production == AYaccHp6 );
      break;

    case AYaccHp7:
    // tag -> Hp7 Dot
    case AYaccEHp7:
    // tag -> Ehp7 Dot
      _parser->setUnderline( production == AYaccHp7 );
      _parser->setBold( production == AYaccHp7 );
      break;

    case AYaccHp4:
    // tag -> Hp4 Dot
      _parser->setForegroundColor( IColor( IColor::blue ) );
      break;

    case AYaccHp8:
    // tag -> Hp8 Dot
      _parser->setForegroundColor( IColor( IColor::red ) );
      break;

    case AYaccHp9:
    // tag -> Hp9 Dot
      _parser->setForegroundColor( IColor( IColor::pink ) );
      break;

    case AYaccEHp4:
    // tag -> Ehp4 Dot
    case AYaccEHp8:
    // tag -> Ehp8 Dot
    case AYaccEHp9:
    // tag -> Ehp9 Dot
      // the :ehp color tags simply force the foreground color to default
      _parser->setForegroundColor( IDeviceColor( IDeviceColor::defaultColor ) );
      break;

    case AYaccI1:
    // tag -> I1 i1Attrs DotWhite extraText
      handleIndex( true );
      break;

    case AYaccI1AttrId:
    // i1Attr -> Id Equal anyParm
      setValue( _name );
      break;

    case AYaccI1AttrGlobal:
    // i1Attr -> Global
      _isExternal = true;
      break;

    case AYaccI1AttrRoots:
    // i1Attr -> Roots Equal anyParm
      setValue( _roots );
      break;

    case AYaccI2:
    // tag -> I2 i2Attrs DotWhite extraText
      handleIndex( false );
      break;

    case AYaccI2AttrRefid:
    // i2Attr -> Refid Equal anyParm
      setValue( _name );
      break;

    case AYaccI2AttrGlobal:
    // i2Attr -> Global
      _isExternal = true;
      break;

    case AYaccLi:
    // tag -> Li Dot
      _parser->sendGin( new ItemGin() );
      break;

    case AYaccLines:
    // tag -> Lines linesAttr Dot
      _parser->sendPara();
      _parser->sendGin( new PushGin() );
      _parser->sendGin( new RulerGin( RulerGin::preformat ) );
      _parser->sendGin( new AlignGin( _alignment ) );
      _alignment = _defaultAlignment;
      _parser->pushMode( IpfParser::literal );
      break;

    case AYaccLinesAttrAlignLeft:
    // linesAttr -> Align Equal Left
      _alignment = AlignGin::left;
      break;

    case AYaccLinesAttrAlignCenter:
    // linesAttr -> Align Equal Center
      _alignment = AlignGin::center;
      break;

    case AYaccLinesAttrAlignRight:
    // linesAttr -> Align Equal Right
      _alignment = AlignGin::right;
      break;

    case AYaccELines:
    // tag -> Elines Dot
      _parser->handlePop();
      _parser->popMode( IpfParser::literal );
      break;

    case AYaccLink:
    // link -> Link linkAttrs Dot
      handleLink();
      break;

    case AYaccLinkAttrReftypeHd:
    // linkAttr -> Reftype Equal Hd
      _linkType = headingLink;
      break;

    case AYaccLinkAttrReftypeFn:
    // linkAttr -> Reftype Equal Fn
      _linkType = footnoteLink;
      break;

    case AYaccLinkAttrReftypeLaunch:
    // linkAttr -> Reftype Equal Launch
      _linkType = launchLink;
      break;

    case AYaccLinkAttrReftypeInform:
    // linkAttr -> Reftype Equal Inform
      _linkType = informLink;
      break;

    case AYaccLinkAttrRes:
    // linkAttr -> Res Equal Num
      _name = _parser->getPanelName( lexemeAsULong( 2, true ) );
      break;

    case AYaccLinkAttrRefid:
    // linkAttr -> Refid Equal anyParm
      setValue( _name );
      break;

    case AYaccLinkAttrDatabase:
    // linkAttr -> Database Equal anyParm
      setValue( _externalFile );
      break;

    case AYaccLinkAttrAuto:
    // linkAttr -> Auto
      _isAuto = true;
      break;

    case AYaccElink:
    // tag -> Elink Dot
      _parser->handlePop();
      break;

    case AYaccLm:
    // tag -> Lm Margin Equal Num Dot
      _parser->setLeftMargin( _margin );
      _margin = _defaultMargin;
      break;

    case AYaccRm:
    // tag -> Rm Margin Equal Num Dot
      _parser->setRightMargin( _margin );
      _margin = _defaultMargin;
      break;

    case AYaccMarginAttrMargin:
    // marginAttr -> Margin Equal Num
      _margin = lexemeAsULong( 2, true );
      break;

    case AYaccLp:
    // tag -> Lp Dot
      _parser->sendPara();
      break;

    case AYaccNote:
    // tag -> Note noteAttr Dot
      // this tag resets bold, italic and underline
      _parser->sendPara();
      _parser->setItalic( false );
      _parser->setUnderline( false );
      _parser->setBold( true );
      _parser->sendText( _noteText + "  ", false );
      _parser->setBold( false );
      _noteText = _defaultNoteText;
      break;

    case AYaccNoteAttr:
    // noteAttr -> Text Equal anyParm
      setValue( _noteText );
      break;

    case AYaccNt:
    // tag -> Nt ntAttr Dot
      // this tag resets bold, italic and underline
      _parser->sendPara();
      _parser->setItalic( false );
      _parser->setUnderline( false );
      _parser->sendGin( new PushGin() );
      _parser->sendGin( new RulerGin( RulerGin::quote ) );  // use "quote" mode for indent effect
      _parser->setBold( true );
      _parser->sendText( _noteText + "  ", false );
      _parser->setBold( false );
      _noteText = _defaultNoteText;
      break;

    case AYaccNtAttr:
    // ntAttr -> Text Equal anyParm
      setValue( _noteText );
      break;

    case AYaccENt:
    // tag -> Ent Dot
      _parser->handlePop();
      _parser->sendPara();
      break;

    case AYaccOl:
    // tag -> Ol olAttr Dot
      _parser->sendGin( new PushGin() );
      _parser->sendGin( new RulerGin( RulerGin::list ) );
      _parser->sendGin( new OutlineGin( OutlineGin::arabic ) );
      handleSpacing();
      break;

    case AYaccOlAttr:
    // olAttr -> Compact
      _isCompact = true;
      break;

    case AYaccEOl:
    // tag -> Eol Dot
      _parser->handlePop();
      break;

    case AYaccP:
    // tag -> P Dot
      _parser->sendPara();
      break;

    case AYaccParml:
    // tag -> Parml parmlAttrs Dot
      handleDoubleList( _parmlLineBreak, _defaultParmlLineBreak );
      break;

    case AYaccParmlAttrCompact:
    // parmlAttr -> Compact
      _isCompact = true;
      break;

    case AYaccParmlAttrTsize:
    // parmlAttr -> Tsize Equal Num
      _termSizeChars = lexemeAsULong( 2, true );
      break;

    case AYaccParmlAttrBreakNone:
    // parmlAttr -> Break Equal None
      _parmlLineBreak = RulerGin::none;
      break;

    case AYaccParmlAttrBreakFit:
    // parmlAttr -> Break Equal Fit
      _parmlLineBreak = RulerGin::fit;
      break;

    case AYaccParmlAttrBreakAll:
    // parmlAttr -> Break Equal All
      _parmlLineBreak = RulerGin::all;
      break;

    case AYaccEParml:
    // tag -> Eparml Dot
      _parser->handlePop();
      break;

    case AYaccPd:
    // tag -> Pd Dot
      _parser->sendGin( new ColumnGin() );
      break;

    case AYaccPt:
    // tag -> Pt Dot
      _parser->sendGin( new RowGin() );
      break;

    case AYaccRow:
    // tag -> Row Dot
      if ( ! _parser->tableRow() )
        warning( "Unexpected table row" );
      _parser->sendGin( new RowGin() );
      break;

    case AYaccSl:
    // tag -> Sl slAttr Dot
      _parser->sendGin( new PushGin() );
      _parser->sendGin( new RulerGin( RulerGin::list ) );
      _parser->sendGin( new OutlineGin( OutlineGin::none ) );
      handleSpacing();
      break;

    case AYaccSlAttr:
    // slAttr -> Compact
      _isCompact = true;
      break;

    case AYaccESl:
    // tag -> Esl Dot
      _parser->handlePop();
      break;

    case AYaccTable:
    // tag -> Table tableAttrs Dot
      handleTable();
      // :table always resets to default colors
      _parser->setForegroundColor( IDeviceColor( IDeviceColor::defaultColor ) );
      _parser->setBackgroundColor( IDeviceColor( IDeviceColor::background ) );
      break;

    case AYaccTableAttrCols:
    // tableAttr -> Cols Equal anyParm
      setValue( _columns );
      break;

    case AYaccTableAttrRulesBoth:
    // tableAttr -> RulesAttr Equal Both
      _borders |= BorderGin::rules;
      break;

    case AYaccTableAttrRulesHoriz:
    // tableAttr -> RulesAttr Equal Horiz
      _borders = ( _borders & ~BorderGin::rules ) | BorderGin::horizontal;
      break;

    case AYaccTableAttrRulesVert:
    // tableAttr -> RulesAttr Equal Vert
      _borders = ( _borders & ~BorderGin::rules ) | BorderGin::vertical;
      break;

    case AYaccTableAttrRulesNone:
    // tableAttr -> RulesAttr Equal None
      _borders &= ~BorderGin::rules;
      break;

    case AYaccTableAttrFrameRules:
    // tableAttr -> Frame Equal Rules
      _borders = ( _borders & ~BorderGin::frame ) | BorderGin::top | BorderGin::bottom;
      break;

    case AYaccTableAttrFrameBox:
    // tableAttr -> Frame Equal Box
      _borders |= BorderGin::frame;
      break;

    case AYaccTableAttrFrameNone:
    // tableAttr -> Frame Equal None
      _borders &= ~BorderGin::frame;
      break;

    case AYaccETable:
    // tag -> Etable Dot
      _parser->handlePop();
      // :etable always resets to default colors
      _parser->setForegroundColor( IDeviceColor( IDeviceColor::defaultColor ) );
      _parser->setBackgroundColor( IDeviceColor( IDeviceColor::background ) );
      // tell the parser
      if ( ! _parser->tableStop() )
        warning( "Unexpected end of table" );
      break;

    case AYaccTitle:
    // tag -> Title DotWhite extraText
      _parser->setTitle( _extra );
      break;

    case AYaccUl:
    // tag -> Ul ulAttr Dot
      _parser->sendGin( new PushGin() );
      _parser->sendGin( new RulerGin( RulerGin::list ) );
      _parser->sendGin( new OutlineGin( OutlineGin::bullet ) );
      handleSpacing();
      break;

    case AYaccUlAttr:
    // ulAttr -> Compact
      _isCompact = true;
      break;

    case AYaccEUl:
    // tag -> Eul Dot
      _parser->handlePop();
      break;

    case AYaccWarning:
    // tag -> Warning warningAttr Dot
      // this tag resets bold, italic and underline
      _parser->sendPara();
      _parser->setItalic( false );
      _parser->setUnderline( false );
      _parser->setBold( true );
      _parser->sendText( _warningText + IString(' '), false );
      _parser->setBold( false );
      _warningText = _defaultWarningText;
      break;

    case AYaccWarningAttr:
    // warningAttr -> Text Equal anyParm
      setValue( _warningText );
      break;

    case AYaccEWarning:
    // tag -> Ewarning Dot
      _parser->sendPara();
      break;

    case AYaccXmp:
    // tag -> Xmp Dot
      _parser->sendPara();
      _parser->sendGin( new PushGin() );
      _parser->sendGin( new RulerGin( RulerGin::preformat ) );
      _parser->resetFont( FontInfo::mono );
      _parser->pushMode( IpfParser::literal );
      break;

    case AYaccEXmp:
    // tag -> Exmp Dot
      _parser->handlePop();
      _parser->resetFont( FontInfo::system );
      _parser->popMode( IpfParser::literal );
      break;

    case AYaccExtraColon:
    // extra -> ExtraColon
    case AYaccExtraEnd:
    // extra -> ExtraEnd
      _parser->clearLineFlag();
  }
}


/***************************************************************************
 * Procedure.. IpfYacc::common
 * Author..... Mat
 * Date....... 10/29/98
 *
 * Handles common production across both paths.  Manages the following
 * lower-level variables:
 *   _value
 *   _position
 *   _distance
 *   _extra
 ***************************************************************************/
void IpfYacc::common( SSUnsigned32 production )
{
  switch ( production )
  {
    case AYaccFigcap:
    // tag -> Figcap DotWhite extraText
    case AYaccH1:
    // tag -> H1 hnAttrs DotWhite extraText
    case AYaccH2:
    // tag -> H2 hnAttrs DotWhite extraText
    case AYaccH3:
    // tag -> H3 hnAttrs DotWhite extraText
    case AYaccH4:
    // tag -> H4 hnAttrs DotWhite extraText
    case AYaccH5:
    // tag -> H5 hnAttrs DotWhite extraText
    case AYaccH6:
    // tag -> H6 hnAttrs DotWhite extraText
    case AYaccI1:
    // tag -> I1 i1Attrs DotWhite extraText
    case AYaccI2:
    // tag -> I2 i2Attrs DotWhite extraText
    case AYaccICmd:
    // tag -> Icmd DotWhite extraText
    case AYaccISyn:
    // tag -> Isyn Root Equal anyParm DotWhite extraText
    case AYaccTitle:
    // tag -> Title DotWhite extraText
      // these tags all use "extra" text, so it must be reset
      _extra = IString();
      // also clear the line flag
      _parser->clearLineFlag();
      break;

    case AYaccImbed:
    // control -> Im Fname
      _parser->imbedFile( lexemeAsString( 1 ), Filename::imbed );
      break;

    case AYaccDynamicLeft:
    // dynamic -> Left
      _position = Position( Position::min );
      break;

    case AYaccDynamicRight:
    // dynamic -> Right
      _position = Position( Position::max );
      break;

    case AYaccDynamicCenter:
    // dynamic -> Center
      _position = Position( Position::center );
      break;

    case AYaccDynamicBottom:
    // dynamic -> Bottom
      _position = Position( Position::min );
      break;

    case AYaccDynamicTop:
    // dynamic -> Top
      _position = Position( Position::max );
      break;

    case AYaccDynamicRelative:
    // dynamic -> relative
      _position = Position( _distance );
      break;

    case AYaccRelativePercent:
    // relative -> Numper
      _value = lexemeAsParameter();
      _distance = Distance( lexemeAsULong( 0, true ), Distance::percent );
      break;

    case AYaccAbsoluteChar:
    // absolute -> Numc
      _value = lexemeAsParameter();
      _distance = Distance( lexemeAsULong( 0, true ), Distance::chars );
      break;

    case AYaccAbsolutePixel:
    // absolute -> Numx
      _distance = Distance( lexemeAsULong( 0, true ), Distance::pixels );
      _value = lexemeAsParameter();
      break;

    case AYaccAbsolutePoint:
    // absolute -> Nump
      _distance = Distance( lexemeAsULong( 0, true ) * 20, Distance::twips );
      _value = lexemeAsParameter();
      break;

    case AYaccExtraColon:
    // extra -> ExtraColon
      _extra += IString( ':' );
      break;

    case AYaccExtraWhite:
    // extra -> ExtraWhite
    case AYaccExtraChunk:
    // extra -> ExtraChunk
    case AYaccExtraColonWhite:
    // extra -> ExtraColonWhite
      _extra += lexemeAsString( 0 );
      break;

    case AYaccExtraSymbol:
    // extra -> ExtraSymbol
      handleSymbol( lexemeAsString( 0 ), true );
      break;

    case AYaccAnyParmName:
    // anyParm -> ParmName
    case AYaccAnyParmQuote:
    // anyParm -> ParmQuote
      _value = lexemeAsParameter();
      break;

    case AYaccAnyParmNum:
    // anyParm -> Num
    case AYaccAnyParmSpecial:
    // anyParm -> Numfont
    case AYaccAnyParmKeyAll:
    // parmKey -> All
    case AYaccAnyParmKeyBlack:
    // parmKey -> Black
    case AYaccAnyParmKeyBlue:
    // parmKey -> Blue
    case AYaccAnyParmKeyBorder:
    // parmKey -> Border
    case AYaccAnyParmKeyBoth:
    // parmKey -> Both
    case AYaccAnyParmKeyBottom:
    // parmKey -> Bottom
    case AYaccAnyParmKeyBox:
    // parmKey -> Box
    case AYaccAnyParmKeyBreak:
    // parmKey -> Break
    case AYaccAnyParmKeyBrown:
    // parmKey -> Brown
    case AYaccAnyParmKeyCenter:
    // parmKey -> Center
    case AYaccAnyParmKeyCoverpage:
    // parmKey -> Coverpage
    case AYaccAnyParmKeyCyan:
    // parmKey -> Cyan
    case AYaccAnyParmKeyDarkBlue:
    // parmKey -> Darkblue
    case AYaccAnyParmKeyDarkCyan:
    // parmKey -> Darkcyan
    case AYaccAnyParmKeyDarkGray:
    // parmKey -> Darkgray
    case AYaccAnyParmKeyDarkGreen:
    // parmKey -> Darkgreen
    case AYaccAnyParmKeyDarkPink:
    // parmKey -> Darkpink
    case AYaccAnyParmKeyDarkRed:
    // parmKey -> Darkred
    case AYaccAnyParmKeyDefault:
    // parmKey -> Default
    case AYaccAnyParmKeyFit:
    // parmKey -> Fit
    case AYaccAnyParmKeyFn:
    // parmKey -> Fn
    case AYaccAnyParmKeyGreen:
    // parmKey -> Green
    case AYaccAnyParmKeyHd:
    // parmKey -> Hd
    case AYaccAnyParmKeyHoriz:
    // parmKey -> Horiz
    case AYaccAnyParmKeyHorizontal:
    // parmKey -> Horizontal
    case AYaccAnyParmKeyInform:
    // parmKey -> Inform
    case AYaccAnyParmKeyLaunch:
    // parmKey -> Launch
    case AYaccAnyParmKeyLeft:
    // parmKey -> Left
    case AYaccAnyParmKeyMinmax:
    // parmKey -> Minmax
    case AYaccAnyParmKeyNeutral:
    // parmKey -> Neutral
    case AYaccAnyParmKeyNone:
    // parmKey -> None
    case AYaccAnyParmKeyPage:
    // parmKey -> Page
    case AYaccAnyParmKeyPaleGray:
    // parmKey -> Palegray
    case AYaccAnyParmKeyPink:
    // parmKey -> Pink
    case AYaccAnyParmKeyRed:
    // parmKey -> Red
    case AYaccAnyParmKeyRight:
    // parmKey -> Right
    case AYaccAnyParmKeyRules:
    // parmKey -> Rules
    case AYaccAnyParmKeyTop:
    // parmKey -> Top
    case AYaccAnyParmKeySizeborder:
    // parmKey -> Sizeborder
    case AYaccAnyParmKeyVert:
    // parmKey -> Vert
    case AYaccAnyParmKeyVertical:
    // parmKey -> Vertical
    case AYaccAnyParmKeyYellow:
    // parmKey -> Yellow
    case AYaccAnyParmKeyYes:
    // parmKey -> Yes
      _value = lexemeAsParameter();
      break;

  }
}


/***************************************************************************
 * Procedure.. IpfYacc::handleSection
 * Author..... Mat
 * Date....... 9/22/98
 *
 * Crate a new section at the given level.  Assume we are processing a
 * rule with the following form:
 *   tag -> H1 hnAttrs DotWhite extraText
 * The "extraText" is the section title.  Other settings have been parsed
 * member variables.
 *
 * This function will generate a SectionGin and then a LabelGin to set
 * the id.
 ***************************************************************************/
void IpfYacc::handleSection( int level )
{
  MEKASSERT( _pass == 2 );

  if ( level > _parser->tocLevel() )
  {
    // SECTION IS CONSUMED

    // output title text only
    _parser->sendText( _extra );
  }
  else
  {
    // SECTION STANDS ALONE

    // let the parser reset styles and flush footnotes
    _parser->finishSection();
    if ( _isTopLevel && _parser->indicator() )
      _parser->indicator()->setCurrent( offset() );

    // create window definition if necessary
    int windowId = 0;
    if ( _isWindowSet )
    {
      windowId = _parser->nextWindowId();
      WindowDef window( windowId,
                        IString(),
                        _x,
                        _y,
                        _width,
                        _height,
                        _border,
                        _scrolling,
                        _bar != noBar,
                        _bar == minmaxBar || _bar == fullBar,
                        _bar == minmaxBar || _bar == fullBar,
                        _bar == sysmenuBar || _bar == fullBar );
      _parser->addWindow( window );
    }

    // create section
    SectionGin * section = new SectionGin( level, ! _isHidden );
    IString title = _extra;
    if ( title.length() )
      section->setTitle( title );
    if ( windowId )
      section->setWindow( windowId );
    _parser->sendGin( section );

    // update TOC level for next section (if specified)
    if ( _sectionTocLevel )
      _parser->setTocLevel( _sectionTocLevel );

    // create label
    // if we have a matching context ID, use that first!
    if ( _id && _parser->hasContext(_id) )
    {
      // create a LabelGin
      LabelGin * label = new LabelGin( _parser->getContext(_id) );
      label->setId( _id );
      label->setExternal( _isExternal );
      _parser->sendGin( label );
      _id = 0;
    }
    // create primary label first
    if ( _name.length() || _id )
    {
      // generate a name, if necessary
      // names are saved or generated during pass 1
      if ( ! _name.length() )
      {
        _name = _parser->getPanelName( _id );
        MEKASSERT( _name.length() );
      }
      // create a LabelGin
      LabelGin * label = new LabelGin( _name );
      label->setExternal( _isExternal );
      if ( _id )
        label->setId( _id );
      _parser->sendGin( label );
    }
    // create second label if needed
    if ( _name2.length() )
    {
      // create a LabelGin
      LabelGin * label = new LabelGin( _name2 );
      label->setExternal( _isExternal );
      _parser->sendGin( label );
    }

    // set default spacing
    _parser->sendGin( new SpacingGin( Distance( 1, Distance::chars ) ) );

    // send default font to assert codepage
    _parser->sendGin( new FontGin( FontInfo::system, IString() ) );
  }

  // reset attributes
  _isHidden        = false;
  _name            = IString();
  _name2           = IString();
  _isExternal      = false;
  _id              = 0;
  _sectionTocLevel = 0;
  _isWindowSet     = false;
  _x               = _defaultX;
  _y               = _defaultY;
  _width           = _defaultWidth;
  _height          = _defaultHeight;
  _border          = _defaultBorder;
  _scrolling       = _defaultScrolling;
  _bar             = _defaultBar;
}


/***************************************************************************
 * Procedure.. IpfYacc::lexemeAsTocLevel
 * Author..... Mat
 * Date....... 9/22/98
 *
 * This functions returns a "toc" attribute, which IPF uses to control which
 * levels in the table of contents (TOC) are visible.  This attribute can
 * appear either in a :docprof tag or in an :h1 - :h6 tag, and it has a
 * global affect.
 *
 * This function assumes lexeme 2 will be the toc value.  IPF only looks at
 * the last digit of the attribute.
 ***************************************************************************/
unsigned IpfYacc::lexemeAsTocLevel()
{
  SSUnsigned32 value = lexemeAsULong( 2, true );
  return value % 10;
}


/***************************************************************************
 * Procedure.. IpfYacc::lexemeAsParameter
 * Author..... Mat
 * Date....... 11/10/98
 *
 * Gets lexeme(0), strips single quotes if present, and convert all white
 * to blanks.
 ***************************************************************************/
IString IpfYacc::lexemeAsParameter()
{
  return lexemeAsString( 0 ).strip( '\'' ).translate( "\t\n", "  " );
}


/***************************************************************************
 * Procedure.. IpfYacc::handleIndex
 * Author..... Mat
 * Date....... 9/22/98
 *
 * This function create a new keyword (index entry).  The index entry
 * is either a primary (:i1) or secondary (:i2).  Assume we are processing
 * a rule with the following form:
 *   tag -> I1 i1Attrs Dot extraText
 * The "extraText" is the index text.
 ***************************************************************************/
void IpfYacc::handleIndex( Boolean isPrimary )
{
  MEKASSERT( _pass == 2 );

  if ( _extra.length() == 0 )
    warning("Empty index entry");
  else
  {
    // get keyword text (for secondary, prepend primary text)
    IString text;
    if ( ! isPrimary )
      text = _parser->getEntryText( _name );
    if ( text.length() )
      text += IString(", ");
    text += _extra;

    // create keyword
    KeywordGin * keyword = new KeywordGin( text, true );
    keyword->setExternal( _isExternal );
    _parser->sendGin( keyword );
  }

  // for primary index, send synonyms
  if ( isPrimary && _roots.length() )
    _parser->sendSynonyms( _roots );

  // reset attributes
  _name = IString();
  _isExternal = false;
  _roots = IString();
}


/***************************************************************************
 * Procedure.. IpfYacc::handleLink
 * Author..... Mat
 * Date....... 9/22/98
 *
 * This function handles (or ignores) a link.  Auto links and links found
 * in an artlink context are ignored AND they don't have a companion
 * :elink tag.  Launch and inform links are not supported, but a PushGin
 * is still sent to match the PopGin from the :elink.
 ***************************************************************************/
void IpfYacc::handleLink()
{
  MEKASSERT( _pass == 2 );

  // warning for unknown link type
  // still need to process it so :elink processing works
  if ( _linkType == unknownLink )
    warning( "Unknown link type" );

  _parser->sendGin( new PushGin() );

  Boolean isLink = false;
  if ( _linkType != unknownLink &&
       _linkType != launchLink &&
       _linkType != informLink )
  {
    if ( _name.length() == 0 )
      warning( "No link target given" );
    else
    {
      // create LinkGin for a supported link type
      LinkGin * link = new LinkGin( _name );
      if ( _externalFile.length() )
        link->setFile( _externalFile );
      _parser->sendGin( link );
      isLink = true;
    }
  }

  // for art-links or auto-links, insert dummy text and end context
  if ( _isAuto || _parser->mode() == IpfParser::artlink )
  {
    if ( isLink )
      _parser->sendText( IString("[Link to ") + _name + IString(']'), false );
    _parser->handlePop();
  }

  // reset attributes
  _name = IString();
  _externalFile = IString();
  _linkType = _defaultLinkType;
  _isAuto = false;
}


/***************************************************************************
 * Procedure.. IpfYacc::handleReference
 * Author..... Mat
 *
 * This function handles a :hdref (Reference) tag.  The word "Reference"
 * is inserted and set as a link to the specified target.
 ***************************************************************************/
void IpfYacc::handleReference()
{
  MEKASSERT( _pass == 2 );

  if ( _name.length() == 0 )
    warning( "No reference target given" );
  else
  {
    _parser->sendGin( new PushGin() );
    LinkGin * link = new LinkGin( _name );
    _parser->sendGin( link );
    _parser->sendText( "Reference", false );
    _parser->handlePop();
  }

  // reset attributes
  _name = IString();
}



/***************************************************************************
 * Procedure.. IpfYacc::handleTable
 * Author..... Mat
 * Date....... 9/22/98
 *
 * This function creates a table.
 ***************************************************************************/
void IpfYacc::handleTable()
{
  MEKASSERT( _pass == 2 );

  // push context
  _parser->sendGin( new PushGin() );

  // create Ruler
  RulerGin * ruler = new RulerGin( RulerGin::table );

  // parse column string and add columns
  int i;
  int n = _columns.numWords();
  if ( n < 1 )
  {
    warning( "Table with no columns defined" );
    delete ruler;
    return;
  }
  for ( i = 1; i <= n; i++ )
    ruler->addColumn( Distance( _columns.word( i ).asUnsigned(), Distance::chars ) );

  // send the ruler and border settings
  _parser->sendGin( ruler );
  _parser->sendGin( new BorderGin( _borders ) );

  // tell parser
  if ( ! _parser->tableStart( n ) )
    warning( "Nested tables not supported" );

  // reset attributes
  _columns = IString();
  _borders = _defaultBorders;
}


/***************************************************************************
 * Procedure.. IpfYacc::handleFont
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle a font definition.
 ***************************************************************************/
void IpfYacc::handleFont()
{
  MEKASSERT( _pass == 2 );

  if ( ! _facename.length() )
    warning( "Font with missing facename" );
  else
  {
    // determine family
    FontInfo::Family family;
    if ( _pointSize == 0 || _facename == "default" )
    {
      family = FontInfo::system;
      _facename = IString();  // "default" is not really a facename
      _pointSize = 0;
    }
    else
    {
      FontInfo info( _facename );
      family = info.family();
    }

    // create FontGin
    FontGin font( family, _facename );
    if ( _codepage )
      font.setCodepage( _codepage );
    SizeGin size( Distance( _pointSize * 20, Distance::twips ) );

    // set current font
    _parser->setFont( font, size );
  }

  _facename = IString();
  _codepage = 0;
  _pointSize = 0;
}


/***************************************************************************
 * Procedure.. IpfYacc::handleDoubleList
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle a double column list (:dl. or :parml.).  The lineBreak and
 * defaultLineBreak parameters are different for :dl. and :parml.
 ***************************************************************************/
void IpfYacc::handleDoubleList( RulerGin::LineBreak & lineBreak, RulerGin::LineBreak defaultLineBreak )
{
  MEKASSERT( _pass == 2 );

  RulerGin * ruler = new RulerGin( RulerGin::dlist );
  ruler->setLineBreak( lineBreak );
  ruler->addColumn( Distance( _termSizeChars, Distance::chars ) );
  _parser->sendGin( new PushGin() );
  _parser->sendGin( ruler );
  handleSpacing();
  lineBreak = defaultLineBreak;
  _termSizeChars = _defaultTermSizeChars;
}


/***************************************************************************
 * Procedure.. IpfYacc::handleNewLine
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle a new line (\n).  Unless in literal mode, IPFC interprets these
 * as spaces.  The IpfParser "line flag" is cleared after each newline,
 * and set whenever content is output.  Newlines in the input are ignored
 * if there was no content on that line.  EXCEPTION: empty lines are always
 * processed.
 *
 * isEmpty is true if the new line immediately follows another (i.e., it's
 * an empty line)
 ***************************************************************************/
void IpfYacc::handleNewLine( Boolean isEmpty )
{
  MEKASSERT( _pass == 2 );

  if ( isEmpty || _parser->isLineFlagSet() )
  {
    if ( _parser->mode() == IpfParser::literal )
      _parser->sendLine();
    else if ( _parser->mode() >= IpfParser::normal )
      _parser->sendText( IString( ' ' ) );
  }
  _parser->clearLineFlag();
}


/***************************************************************************
 * Procedure.. IpfYacc::handleSymbol
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle a Symbol.
 ***************************************************************************/
void IpfYacc::handleSymbol( const Symbol & symbol )
{
  if ( _parser->mode() < IpfParser::normal )
    warning( "Unexpected symbol" );
  else
    _parser->sendSymbol( symbol );
}


/***************************************************************************
 * Procedure.. IpfYacc::handleSymbol
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle a symbol from a symbol name.  Look up the symbol name either in
 * the table of "nameit" definitions, or in the standard symbol table.
 ***************************************************************************/
void IpfYacc::handleSymbol( const IString & symbolName, Boolean isExtra )
{
  IString            text;
  Symbol::Identifier id = Symbol::unknown;
  unsigned           length = symbolName.length();

  if ( _parser->mode() < IpfParser::normal && ! isExtra )
    warning( "Unexpected symbol" );
  else if ( length <= 2 )
    warning( "Empty symbol name" );
  else
  {
    MEKASSERT( symbolName[1] == '&' && symbolName[length] == '.' );

    // locate symbol in lookup table
    IString symbol = symbolName.subString( 2, length - 2 );
    SymbolLookup lookup( symbol, IString() );
    if ( _parser->locateSymbol( lookup ) )
    {
      // symbol found in lookup table
      if ( lookup.isMacro() )
      {
        // handle a nameit macro
        if ( isExtra )
        {
          // macros are treated as raw text in "extra" text
          text = lookup.macro();
        }
        else
        {
          // process imbedded macro text
          _parser->imbedText( lookup.macro() );
          return;
        }
      }
      else
      {
        // get ID of a built-in symbol
        id = lookup.id();
      }
    }
    else
    {
      // symbol not found in lookup table, send raw text
      text = symbolName;
    }

    if ( isExtra )
    {
      // add to extra text
      if ( id != Symbol::unknown )
        text = Symbol( id ).asChar();
      _extra += text;
    }
    else
    {
      // handle a normal symbol (outside of "extra text")
      if ( id != Symbol::unknown )
        _parser->sendSymbol( Symbol( id ) );
      else
        _parser->sendText( text );
    }
  }
}


/***************************************************************************
 * Procedure.. IpfYacc::handleText
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle basic text.
 ***************************************************************************/
void IpfYacc::handleText( const IString & text )
{
  MEKASSERT( _pass == 2 );

  if ( _parser->mode() >= IpfParser::normal )
    _parser->sendText( text );
  else if ( ! text.isWhiteSpace() )
    warning( "Unexpected text" );
}


/***************************************************************************
 * Procedure.. handleBreak
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle a break (.br) control word.
 ***************************************************************************/
void IpfYacc::handleBreak()
{
  MEKASSERT( _pass == 2 );

  if ( _parser->mode() < IpfParser::normal )
    warning( "Unexpected break" );
  else
    _parser->sendLine();
  _parser->clearLineFlag();
}


/***************************************************************************
 * Procedure.. IpfYacc::handleSpacing
 * Author..... Mat
 * Date....... 9/22/98
 *
 * This function uses the current setting of _isCompact to determine the
 * correct line spacing for a list.  _isCompact is reset.
 ***************************************************************************/
void IpfYacc::handleSpacing()
{
  MEKASSERT( _pass == 2 );

  Distance lines( _isCompact? 0: 1, Distance::chars );
  SpacingGin * spacing = new SpacingGin( lines );
  _parser->sendGin( spacing );
  _isCompact = false;
}


/***************************************************************************
 * Procedure.. IpfYacc::handleHide
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle a :hide. tag.
 ***************************************************************************/
void IpfYacc::handleHide()
{
  MEKASSERT( _pass == 2 );

  IString key;
  setValue( key );
  _parser->sendGin( new PushGin() );
  _parser->sendGin( new HideGin( key ) );
}


/***************************************************************************
 * Procedure.. IpfYacc::handleNameit
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Handle .nameit control word.
 ***************************************************************************/
void IpfYacc::handleNameit()
{
  MEKASSERT( _pass == 2 );

  IBoolean added = _parser->addSymbol( SymbolLookup( _name, _nameitText ) );
  if ( ! added )
    warning( "Nameit symbol already defined" );
  _name = IString();
  _nameitText = IString();
  _name2 = IString();
  _parser->clearLineFlag();
}


/***************************************************************************
 * Procedure.. IpfYacc::handleArtwork
 * Author..... Mat
 * Date....... 10/22/98
 *
 * Handle an :artwork tag.
 ***************************************************************************/
void IpfYacc::handleArtwork()
{
  MEKASSERT( _pass == 2 );

  if ( ! _artFile.length() )
    warning( "No artwork filename specified" );
  else
  {
    IString id;
    if ( _parser->addBitmap( _artFile, id ) )
    {
      // send ArtGin
      _parser->sendGin( new ArtGin( id, _artSpot ) );
      _parser->setLineFlag();

      // imbed linkfile if specified
      if ( _externalFile.length() )
      {
        _parser->imbedFile( _externalFile, Filename::artwork );
        _externalFile = IString();
      }
    }
  }

  _artFile = IString();
  _artSpot = _defaultArtSpot;
}


/***************************************************************************
 * Procedure.. IpfYacc::beginCentered
 * Author..... Mat
 * Date....... 10/27/98
 *
 * Begin "centered" mode to handle text within a .ce control word.  This must
 * be handled specially because there are no begin/end tags.  This function
 * is called whenever any centered text needs to be output, to make sure
 * the mode is set correctly.
 ***************************************************************************/
void IpfYacc::beginCentered()
{
  MEKASSERT( _pass == 2 );

  if ( _parser->mode() != IpfParser::centered )
  {
    handleBreak();
    _parser->sendGin( new PushGin() );
    _parser->sendGin( new RulerGin( RulerGin::preformat ) );
    _parser->sendGin( new AlignGin( AlignGin::center ) );
    _parser->pushMode( IpfParser::centered );
  }
}


/***************************************************************************
 * Procedure.. IpfYacc::endCentered
 * Author..... Mat
 * Date....... 10/27/98
 *
 * This function is called after the entire .ce line is recognized.  If there
 * was any centered text processed, then the mode will be "centered", and
 * it should be popped off the stack here.
 ***************************************************************************/
void IpfYacc::endCentered()
{
  MEKASSERT( _pass == 2 );

  if ( _parser->mode() == IpfParser::centered )
  {
    handleBreak();
    _parser->handlePop();
    _parser->popMode( IpfParser::centered );
  }
  _parser->clearLineFlag();
}


/***************************************************************************
 * Procedure.. IpfYacc::setValue
 * Author..... Mat
 * Date....... 10/8/98
 *
 * Sets a generic parameter value, which may also be a keyword or a special
 * value like "10%".  Resets the _value text after returning it.
 ***************************************************************************/
void IpfYacc::setValue( IString & setting )
{
  setting = _value;
  _value = IString();
}


/***************************************************************************
 * Procedure.. IpfYacc::warning
 * Author..... Mat
 * Date....... 10/13/98
 *
 * Log a warning condition.
 ***************************************************************************/
void IpfYacc::warning( const IString & text, const SSLexLexeme * lexeme )
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

  _parser->warning( text, line, col );
}


/***************************************************************************
 * Procedure.. IpfYacc::parseTo
 * Author..... Mat
 *
 * Saves a pointer to the IpfParser, which holds state info, then begins parsing.
 * Returns true if an error occured, as per SSYacc::parse().
 ***************************************************************************/
SSBooleanValue IpfYacc::parseTo( IpfParser & parser, int pass )
{
  // set pass number
  _pass = pass;

  // set all attribute defaults
  _artSpot         = _defaultArtSpot;
  _cautionText     = _defaultCautionText;
  _dlLineBreak     = _defaultDlLineBreak;
  _termSizeChars   = _defaultTermSizeChars;
  _x               = _defaultX;
  _y               = _defaultY;
  _width           = _defaultWidth;
  _height          = _defaultHeight;
  _border          = _defaultBorder;
  _scrolling       = _defaultScrolling;
  _bar             = _defaultBar;
  _alignment       = _defaultAlignment;
  _linkType        = _defaultLinkType;
  _margin          = _defaultMargin;
  _noteText        = _defaultNoteText;
  _parmlLineBreak  = _defaultParmlLineBreak;
  _borders         = _defaultBorders;
  _warningText     = _defaultWarningText;
  _pointSize       = 0;
  _isCompact       = false;
  _isAuto          = false;
  _facename        = IString();
  _roots           = IString();
  _codepage        = 0;
  _isHidden        = false;
  _id              = 0;
  _name            = IString();
  _isExternal      = false;
  _sectionTocLevel = 0;
  _externalFile    = IString();
  _nameitText      = IString();
  _value           = IString();
  _extra           = IString();
  _isWindowSet     = false;

  // parse the file
  _parser = &parser;
  return parse();
}


