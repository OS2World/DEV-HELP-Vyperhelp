/***************************************************************************
 * File...... RtfGenerator.cpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * Implementation of RtfGenerator
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Standard C
#include <ctype.h>

// OpenClass
#include <itrace.hpp>

// MekTek
#include "MekTek.hpp"

// Generator
#include "HpjGenerator.hpp"
#include "RtfGenerator.hpp"
#include "RtfGeneratorContext.hpp"


// style codes
#define NUM_STYLES    10
#define TITLE_STYLE   0
static const char * styleCodes[10] =
{
  "\\sb240\\sa960\\keepn\\qc\\f1\\fs32\\b",    // Title
  "\\sb240\\sa480\\keepn\\ql\\f1\\fs32\\b",    // Section 1
  "\\sb240\\sa240\\keepn\\ql\\f1\\fs28\\b",    // Section 2
  "\\sb240\\sa240\\keepn\\ql\\f1\\fs24\\b",    // Section 3
  "\\sb240\\sa240\\keepn\\ql\\f1\\fs24\\i",    // Section 4
  "\\sb240\\sa240\\keepn\\ql\\f1\\fs20\\b",    // Section 5
  "\\sb240\\sa240\\keepn\\ql\\f1\\fs20\\i",    // Section 6
  "\\sb240\\sa240\\keepn\\ql\\f1\\fs20",       // Section 7
  "\\sb240\\sa240\\keepn\\ql\\f1\\fs20",       // Section 8
  "\\sb240\\sa240\\keepn\\ql\\f1\\fs20"        // Section 9
};

static const char * defaultCodes = "\\pard\\plain\\f0\\fs20";

// other defaults
#define DEFAULT_FONT_SIZE  20
#define LINE_LENGTH        80

// declare this to send table-of-contents and index controls (\tc and \xe)
// some RTF readers don't understand these and insert the text in the body
// #define GEN_TOC_IDX



/***************************************************************************
 * Procedure.. RtfGenerator::RtfGenerator
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Constructors take either an RTF filename or an output stream for RTF-style
 * output.
 ***************************************************************************/
RtfGenerator::RtfGenerator( const IString & filename, HpjGenerator * hpj ):
    _file( new ofstream( filename ) ),
    _stream( _file ),
    _hpj( hpj ),
    _contentName( Filename::temporary ),
    _content( _contentName, ios::in | ios::out ),
    _cpOut( RTF_CODEPAGE )
{
  initialize();
}

RtfGenerator::RtfGenerator( ostream & stream, HpjGenerator * hpj ):
    _file( 0 ),
    _stream( &stream ),
    _hpj( hpj ),
    _contentName( Filename::temporary ),
    _content( _contentName, ios::in | ios::out ),
    _cpOut( RTF_CODEPAGE )
{
  initialize();
}

RtfGenerator::~RtfGenerator()
{
  // delete content file
  _content.close();
  _contentName.remove();

  // make sure output is closed
  closeOutput();

  // free all allocations
  if ( _file )
  {
    _file->close();
    delete _file;
  }
  if ( _context )
    delete _context;
  if ( _stack )
    delete _stack;
}


/***************************************************************************
 * Procedure.. RtfGenerator::initialize
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Called from either constructor to initialize all member variables for
 * output.
 ***************************************************************************/
void RtfGenerator::initialize()
{
  // allocate context and stack
  _context = new RtfGeneratorContext;
  _stack = new RtfGeneratorContextStack;

  setOutput( _content, LINE_LENGTH, Generator::rtf );
  _needDelimiter = false;
  _cellCount = _rowCount = 0;
  _colorNum = 1;

  // seed font table with a heading font and a default font
  //   - Times is the default font for document mode, Arial is used for headings
  //   - Times is not used for WinHelp, Arial is default font for WinHelp
  _fontNum = 0;
  if ( ! _hpj )
    _fonts.add( RtfFont( _fontNum++, 0, RtfFont::FROMAN, RtfFont::DEFAULT, "Times" ) );
  _fonts.add( RtfFont( _fontNum++, 0, RtfFont::FSWISS, RtfFont::DEFAULT, "Arial" ) );

  // seed color table with a default color
  _colors.add( RtfColor( -1, -1, -1, 0 ) );
  _colorNum = 1;

  // reset context & other control info
  _context->reset( true );
  _sectionCount = 0;
  _para = ended;
}


/***************************************************************************
 * Procedure.. RtfGenerator::translate
 * Author..... Mat
 * Date....... 2/23/99
 *
 * Translate "extra" text from the codepage specified by setCodepage() to
 * RTF output codeppage.  "extra" text is that which is not direct
 * content.  For example, document title, section titles, index entries.
 ***************************************************************************/
IString RtfGenerator::translate( const IString & text )
{
  IString out( text );
  _cpOut.translateString( out, _cp );
  return out;
}


/***************************************************************************
 * Procedure.. RtfGenerator::finish
 * Author..... Mat
 * Date....... 2/4/99
 *
 * Finish the generation.  At this point the header must be generated and
 * inserted in front of the content.
 ***************************************************************************/
void RtfGenerator::finish()
{
  // end the current ruler
  endRuler();

  // close the content file
  outputLine();
  checkOutput();
  closeOutput();

  // send header
  setOutput( *_stream, LINE_LENGTH, Generator::rtf );
  sendText( "{\\rtf1\\ansi", breakAfter );
  sendFontTable();
  sendColorTable();
  sendStylesheet();
  sendInfo();

  // send default formatting
  sendText( defaultCodes, breakAfter );

  // send title
  if ( ! _hpj )
    sendHeading( _title, TITLE_STYLE );

  // rewind content file and send content
  outputLine();
  _content.seekg( 0, ios::beg );
  filebuf * file = _content.rdbuf();
  *_stream << file;
  checkOutput();

  // send closing brace
  sendText( '}', breakBefore );

  // close output file
  closeOutput();
  if ( _file )
  {
    _file->close();
    delete _file;
    _file = 0;
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendFontTable
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Send the RTF font table.
 ***************************************************************************/
void RtfGenerator::sendFontTable()
{
  sendText( "\\deff0", breakAfter );
  sendText( "{\\fonttbl", breakAfter );
  RtfFontTable::Cursor cursor( _fonts );
  forCursor( cursor )
  {
    const RtfFont & font = cursor.element();

    // font number
    sendText( IString("{\\f") + IString(font.getKey()), delimAfter );

    // font family, pitch and codepage
    sendText( font.familyWord(), delimAfter );
    sendText( font.pitchWord(), delimAfter );
    // NOTE: I found out that most RTF reader don't support the \cpg tag,
    // and support for \fcharset is inconsistent, so it looks like we need
    // do our own translation!
    // sendText( font.codepageWord(), delimAfter );

    // facename
    IString facename = font.getFacename();
    if ( ! facename.length() )
      facename = IString("Font ") + IString(font.getKey());
    sendText( facename.strip(), checkSpecial );
    sendText( ";}", breakAfter );
  }
  sendText( "}", breakAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendColorTable
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Send the RTF color table.
 ***************************************************************************/
void RtfGenerator::sendColorTable()
{
  sendText( "{\\colortbl", breakAfter );
  RtfColorTable::Cursor cursor( _colors );
  forCursor( cursor )
  {
    const IColor * color = cursor.element().getColorentry();

    if ( color )
    {
      sendText( IString("\\red") + IString(int(color->redMix())), delimAfter );
      sendText( IString("\\green") + IString(int(color->greenMix())), delimAfter );
      sendText( IString("\\blue") + IString(int(color->blueMix())), delimAfter );
    }
    sendText( IString(';'), breakAfter );
  }
  sendText( "}", breakAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendStylesheet
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Send a stylesheet to support heading and title styles for "document"
 * type output.
 ***************************************************************************/
void RtfGenerator::sendStylesheet()
{
  // no stylesheet for HPJ output
  if ( _hpj )
    return;

  sendText( "{\\stylesheet", breakAfter );

  // style 0: title
  sendText( "{\\s0", delimAfter );
  sendText( styleCodes[0], delimAfter );
  sendText( "Title;}", breakAfter );

  // style 1 - 9: section headings
  for ( int i = 1; i <= 9; i++ )
  {
    sendText( IString("{\\s") + IString(i), delimAfter );
    sendText( styleCodes[i], delimAfter );
    sendText( IString("Heading ") + IString(i) + IString(";}"), breakAfter );
  }

  sendText( "}", breakAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendInfoItem
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Send an item for the information group.
 ***************************************************************************/
void RtfGenerator::sendInfoItem( const IString & code, const IString & text )
{
  sendText( IString('{') + code, delimAfter );
  sendText( translate( text ), checkSpecial );
  sendText( IString('}'), breakAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendInfo
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Send the global strings in the RTF \info group.
 ***************************************************************************/
void RtfGenerator::sendInfo()
{
  sendText( "{\\info", breakAfter );
  if ( _title.length() )
    sendInfoItem( "\\title", _title );
  if ( _subject.length() )
    sendInfoItem( "\\subject", _subject );
  if ( _author.length() )
    sendInfoItem( "\\author", _author );
  if ( _keywords.length() )
    sendInfoItem( "\\keywords", _keywords );
  if ( _version.length() || _copyright.length() )
  {
    IString text;
    if ( _version.length() )
      text += IString("Version: ") + _version;
    if ( _version.length() && _copyright.length() )
      text += IString(", ");
    if ( _copyright.length() )
      text += IString("Copyright: ") + _copyright;
    sendInfoItem( "\\comment", text );
  }
  sendText( "}", breakAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendHeading
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Send a heading or title text along with appropriate style codes.  Used
 * for document output, not WinHelp.  Also translates the text to RTF codepage.
 ***************************************************************************/
void RtfGenerator::sendHeading( const IString & text, unsigned style )
{
  // no styles for HPJ output
  if ( _hpj )
    return;

  if ( text.length() && ( style < NUM_STYLES ) )
  {
    resetPara();
    sendText( IString("{\\pard\\plain\\s") + IString(style) + IString(styleCodes[style]), breakBefore | delimAfter );
    sendText( translate( text ), checkSpecial );
    sendText( "\\par}", breakAfter );
    _para = ended;
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::addKeyword
 * Author..... Mat
 * Date....... 2/4/99
 *
 * Add a keyword to the list.
 ***************************************************************************/
Generator & RtfGenerator::addKeyword( const IString & keyword )
{
  if ( _keywords.length() )
    _keywords += ',';  // add comma separator between strings
  _keywords += keyword;
  return *this;
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendText
 * Author..... Mat
 * Date....... 2/4/99
 *
 * Send RTF output text.  If the delimAfter flag is set, then the given
 * text ends with a control word that is not delimited.
 ***************************************************************************/
void RtfGenerator::sendText( const IString & text, unsigned flags )
{
  if ( ! text.length() )
    return;

  // check for special symbols
  IString out;
  if ( flags & checkSpecial )
  {
    // loop through string, converting characters
    unsigned char * pch = (unsigned char *)text;
    while ( *pch )
    {
      switch ( *pch )
      {
        case '\\':
          out += IString("\\\\");
          break;

        case '{':
          out += IString("\\{");
          break;

        case '}':
          out += IString("\\}");
          break;

        case '\n':
          // convert newline to space or \line
          if ( _context->mode == RulerGin::preformat )
            out += IString("\\line") + IString(delimiter);
          else
            out += IString(' ');
          break;

        default:
          if ( *pch < 0x20 || *pch >= 0x80 )
          {
            // convert to a hex literal
            out += IString("\\'") + IString((int)*pch).d2x();
          }
          else
          {
            // pass as-is
            out += IString( *pch );
          }
          break;
      }

      pch++;
    }
  }
  else
    out = text;

  // output a space delimiter if needed
  //   alphanumeric would be considered part of the control word
  //   spaces would be consumed
  //   hyphens are sometimes confused as part of the control word
  if ( _needDelimiter
      && ( ( flags & breakBefore) == 0 )
      && ( isalnum(out[1]) || out[1] == ' ' || out[1] == '-' ) )
    outputText( delimiter, 0 );

  // output the text
  outputText( out, flags );

  // set the delimiter flag
  _needDelimiter = ( ( flags & delimAfter ) == delimAfter ) && ( ( flags & breakAfter ) == 0 );
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendLine
 * Author..... Mat
 * Date....... 2/4/99
 *
 * Send an output line and reset the delimiter flag.
 ***************************************************************************/
void RtfGenerator::sendLine( Boolean force )
{
  _needDelimiter = false;
  outputLine( force );
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendFootnote
 * Author..... Mat
 * Date....... 2/4/99
 *
 * Send a WinHelp footnote.  Translate the text to RTF codepage.
 * NOTE: The WinHelp compiler would sometimes ignore footnotes if this whole
 * group was not enclosed in braces.
 ***************************************************************************/
void RtfGenerator::sendFootnote( char footnote, const IString & text )
{
  if ( text.length() )
  {
    sendText( IString('{') + IString(footnote) + IString("{\\footnote\\plain ") + IString(footnote) + IString(' '), breakBefore );
    sendText( translate( text ), checkSpecial );
    sendText( IString("}}"), breakAfter );
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleSection
 * Author..... Mat
 * Date....... 2/4/99
 *
 * Handle a section.  In WinHelp mode, use footnotes to specify the title
 * and window.  In document mode, output the section title text.
 ***************************************************************************/
void RtfGenerator::handleSection( const SectionGin & section )
{
  // end the current link
  if ( _context->link.target().length() )
    sendLink( _context->link );

  // end the current ruler mode
  endRuler();

  _para = ended;

  if ( _hpj )
  {
    // WINHELP

    // end current section
    if ( _sectionCount )
      sendText( "\\page", breakBefore | breakAfter );

    // send browse footnote
    sendFootnote( '+', IString(_sectionCount).rightJustify( 4, '0') );

    // title footnote and heading (non-scrolling)
    if ( section.title().length() )
    {
      // send title footnote
      sendFootnote( '$', section.title() );

      // put 14 pt Helvetica title text in a non-scrolling region (using \keepn)
      sendText( IString("{\\pard\\plain\\keepn\\f0\\fs28"), breakBefore | delimAfter );
      sendText( translate( section.title() ), checkSpecial );
      sendText( "\\par}", breakAfter );
    }

#ifdef GEN_WINDOWS
    // window footnote
    if ( hasWindowWithId( section.windowId() ) )
      sendFootnote( '>', section.windowId() );
#endif

    sendLine( false );
  }
  else
  {
    // DOCUMENT

    // get title text
    IString title = section.title();
    if ( ! title.length() )
      title = "Untitled";

    // make sure level is in range
    int level = section.level();
    if ( level > 9)
      level = 9;
    else if ( level < 1 )
      level = 1;

    sendHeading( title, level );

#ifdef GEN_TOC_IDX
    // RTF document table of contents entry
    sendText( IString("{\\tc\\tcl\\v") + IString(level), breakBefore | delimAfter );
    sendText( translate( title ), checkSpecial );
    sendText( '}', breakAfter );
#endif
  }

  // reset styles
  sendText( defaultCodes, breakAfter );
  _context->reset( true );

  // increment section count
  _sectionCount++;
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleKeyword
 * Author..... Mat
 * Date....... 2/4/99
 *
 * In WinHelp mode, send a footnote to register a keyword.
 ***************************************************************************/
void RtfGenerator::handleKeyword( const KeywordGin & keyword )
{
  if ( _hpj )
  {
    // use 'A' keywords for unlisted keywords
    sendFootnote( keyword.isListed()? 'K': 'A', keyword.text() );
  }
#ifdef GEN_TOC_IDX
  else if ( keyword.text().length() )
  {
    // RTF document index entry
    sendText( "{\\xe\\v", breakBefore | delimAfter );
    sendText( translate( keyword.text() ), checkSpecial );
    sendText( '}', breakAfter );
  }
#endif
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleLabel
 * Author..... Mat
 * Date....... 2/4/99
 *
 * In WinHelp mode, send a footnote to set a label (anchor).
 ***************************************************************************/
void RtfGenerator::handleLabel( const LabelGin & label )
{
  if ( _hpj )
    sendFootnote( '#', label.name() );
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleRow
 * Author..... Mat
 * Date....... 2/25/99
 *
 * Handle the start of a new row in either a table or a two-column list.
 ***************************************************************************/
void RtfGenerator::handleRow( const RowGin & )
{
  switch ( _context->mode )
  {
    case RulerGin::table:
      endCell();  // end current cell
      endRow( false );   // end current row
      break;

    case RulerGin::dlist:
      endPara();
      break;
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleColumn
 * Author..... Mat
 * Date....... 2/25/99
 *
 * Handle the start of a new column in either a table or a two-column list.
 ***************************************************************************/
void RtfGenerator::handleColumn( const ColumnGin & )
{
  switch ( _context->mode )
  {
    case RulerGin::table:
      endCell();
      break;

    case RulerGin::dlist:
      if ( _context->wrap == RulerGin::all )
        sendText( "\\line", breakBefore | delimAfter );
      else
        sendText( "\\tab", breakBefore | delimAfter );
      _para = started;
      break;
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleItem
 * Author..... Mat
 * Date....... 2/25/99
 *
 * Handle a list item.  Use the itemStyle to determine how the item should
 * be prefixed.
 ***************************************************************************/
void RtfGenerator::handleItem( const ItemGin & )
{
  if ( _context->mode == RulerGin::list )
  {
    resetPara();
    _context->itemNumber++;

    switch ( _context->itemStyle )
    {
      case OutlineGin::bullet:
        sendText( "\\bullet", delimAfter );
        break;

      case OutlineGin::none:
        break;

      default:
        // use standard arabic numbering for all others
        sendText( IString(_context->itemNumber) + IString('.') );
        break;
    }

    sendText( "\\tab", delimAfter );
    _para = started;
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::handlePara
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Handle the start of a new paragraph.  If we are currently in a list,
 * then tab in to the second column.
 ***************************************************************************/
void RtfGenerator::handlePara( const ParaGin & )
{
  endPara();

  switch ( _context->mode )
  {
    case RulerGin::list:
    case RulerGin::dlist:
      // tab out to second column if in a list
      sendText( "\\tab", delimAfter );
      _para = started;
      break;
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleLine
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Handle a line break.
 ***************************************************************************/
void RtfGenerator::handleLine( const LineGin & )
{
  sendText( "\\line", breakBefore | delimAfter );
  _para = started;
}


/***************************************************************************
 * Procedure.. RtfGenerator::handlePush
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Handle a context push.  Keep track of table nesting level and base margins.
 * The margins are changed within a context when indenting for lists.
 ***************************************************************************/
void RtfGenerator::handlePush( const PushGin & )
{
  // begin RTF group
  sendText( '{' );

  // update table nesting
  if ( _context->mode == RulerGin::table )
    _context->tableNested++;

  // push the current context
  _stack->push( *_context );

  // save base margins and tab
  _context->baseRightMargin = _context->rightMargin;
  _context->baseLeftMargin = _context->leftMargin;
  _context->baseTab = _context->tab;
}


/***************************************************************************
 * Procedure.. RtfGenerator::handlePop
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Handle a context pop.  Keep track of the table nesting level, and check
 * for links starting or ending.
 ***************************************************************************/
void RtfGenerator::handlePop( const PopGin & )
{
  // save incoming context
  RtfGeneratorContext previous = *_context;

  // pop the current context
  _stack->pop( *_context );

  // check for link activation
  if ( _hpj && linkChange( previous.link, _context->link ) )
    sendLink( previous.link );

  // update table nesting
  if ( _context->mode == RulerGin::table )
    _context->tableNested--;

  // special handling for end of a table
  if ( ( previous.mode == RulerGin::table ) && ( _context->mode != RulerGin::table ) )
    endTable();

  // end RTF group
  sendText( '}' );

  // determine if para needs to be reset because of style changes
  // NOTE: must be outside of /table group if just ended
  //       MS incident # SRZ990920000472 (10/99)
  if ( ( _para == started ) && _context->changedFrom( previous ) )
    resetPara();

  // check if a new link is starting
  if ( _hpj && linkChange( _context->link, previous.link ) )
    startLink( );
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleRightMargin
 * Procedure.. RtfGenerator::handleLeftMargin
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Handle change to right or left margin.
 ***************************************************************************/
void RtfGenerator::handleRightMargin( const RightMarginGin & margin )
{
  setRightMargin( margin.margin().asTwips(), margin.isRelative() );
  _context->baseRightMargin = _context->rightMargin;
}

void RtfGenerator::handleLeftMargin( const LeftMarginGin & margin )
{
  setLeftMargin( margin.margin().asTwips(), margin.isRelative() );
  _context->baseLeftMargin = _context->leftMargin;
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleBorder
 * Author..... Mat
 * Date....... 2/25/99
 *
 * Handle change to paragraph/table borders.
 ***************************************************************************/
void RtfGenerator::handleBorder( const BorderGin & border )
{
  if ( _context->mode == RulerGin::table )
  {
    // send table borders
    sendTableBorders( border.borders() );
  }
  else
  {
    resetPara();
    IString control;
    if ( border.borders() & BorderGin::top )
      control += IString("\\brdrt\\brdrs");
    if ( border.borders() & BorderGin::bottom )
      control += IString("\\brdrb\\brdrs");
    if ( border.borders() & BorderGin::right )
      control += IString("\\brdrr\\brdrs");
    if ( border.borders() & BorderGin::left )
      control += IString("\\brdrl\\brdrs");
    sendText( control, delimAfter );
  }

  // save borders for tables
  _context->borders = border.borders();
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleAlign
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Handle a change to text alignment.
 ***************************************************************************/
void RtfGenerator::handleAlign( const AlignGin & align )
{
  IString control;

  resetPara();

  switch ( align.alignment() )
  {
    case AlignGin::left:
      control = "\\ql";
      break;

    case AlignGin::right:
      control = "\\qr";
      break;

    case AlignGin::center:
      control = "\\qc";
      break;

    case AlignGin::justify:
      control = "\\qj";
      break;

    default:
      BADSWITCH;
      break;
  }

  sendText( control, delimAfter );

  // save alignment in context
  _context->alignment = align.alignment();
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleSpacing
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Handle a change to line spacing.  The line spacing is stored as part of
 * the context.
 ***************************************************************************/
void RtfGenerator::handleSpacing( const SpacingGin & spacing )
{
  setSpacing( spacing.spacing().asTwips( false ) );
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleRuler
 * Author..... Mat
 * Date....... 2/25/99
 *
 * Handle a ruler change.  Update margins, tabs and/or table settings.
 ***************************************************************************/
void RtfGenerator::handleRuler( const RulerGin & ruler )
{
  // end the current mode
  endRuler();

  switch ( ruler.mode() )
  {
    case RulerGin::table:
      if ( ! _context->tableNested )
        startTable( ruler );
      break;

    case RulerGin::preformat:
      // use overloaded \keep to make text non-wrapping
      if ( _hpj )
        sendText( "\\keep", delimAfter );
      break;

    case RulerGin::quote:
      setLeftMargin( +720, true );
      setRightMargin( +720, true );
      break;

    case RulerGin::list:
      setTab( +360 );
      break;

    case RulerGin::dlist:
      setTab( ruler.column( 1 ).asTwips() );
      break;
  }

  // save mode in context
  _context->mode = ruler.mode();
  _context->wrap = ruler.lineBreak();
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleOutline
 * Author..... Mat
 * Date....... 2/25/99
 *
 * Handle a change to the outline mode.  Save the style and reset numbering.
 ***************************************************************************/
void RtfGenerator::handleOutline( const OutlineGin & outline )
{
  _context->itemStyle = outline.style();
  _context->itemNumber = 0;
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleFont
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Handle a font change.  Convert info from FontGin to an RtfFont, make sure
 * the RtfFont is in the table, and send a font selection control \f.
 ***************************************************************************/
void RtfGenerator::handleFont( const FontGin & gin )
{
  // map family name and pitch
  RtfFont::Familyname family = _fonts.elementWithKey( 0 ).family();  // get default font's family
  RtfFont::Fprqtype pitch = RtfFont::DEFAULT;
  switch ( gin.family() )
  {
    case FontInfo::roman:
      family = RtfFont::FROMAN;
      break;

    case FontInfo::swiss:
      family = RtfFont::FSWISS;
      break;

    case FontInfo::mono:
      family = RtfFont::FMODERN;
      pitch = RtfFont::FIX;
      break;
  }

  // add the font if not already added
  RtfFont font( _fontNum, gin.codepage(), family, pitch, gin.facename() );
  RtfFontTable::Cursor cursor( _fonts );
  forCursor( cursor )
  {
    // use equality operator to check for match with previous entry (key is ignored)
    if ( cursor.element() == font )
      break;
  }
  if ( ! cursor.isValid() )
  {
    // need to add font to list
    IBoolean added = _fonts.add( font, cursor );
    IASSERTSTATE( added );
    // increment font counter
    _fontNum++;
  }

  // get font number
  unsigned n = cursor.element().getKey();

  // send tag to set the font
  sendText( IString("\\f") + IString(n), delimAfter );

  // set active codepage
  int codepage = _fonts.elementWithKey( n ).getCodepage();
  if ( codepage <= 0 )
    codepage = _cpOut;
  _context->cp = codepage;
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleSize
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Handle a SizeGin which sets the text size.  \fs specifies the RTF text
 * size in half-points.
 ***************************************************************************/
void RtfGenerator::handleSize( const SizeGin & size )
{
  // convert twips to half-points
  unsigned halfPoints = size.height().asTwips( false ) / 10;
  if ( ! halfPoints )
    halfPoints = DEFAULT_FONT_SIZE;

  sendText( IString("\\fs") + IString(halfPoints), delimAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::addColor
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Add the specified color to the table if it isn't already in there.
 * Increment _colorNum if color was added.  Return the color index.
 ***************************************************************************/
int RtfGenerator::addColor( const RtfColor & color )
{
  // add the font if not already added
  RtfColorTable::Cursor cursor( _colors );
  forCursor( cursor )
  {
    // use equality operator to check for match with previous entry (key is ignored)
    if ( cursor.element() == color )
      break;
  }
  if ( ! cursor.isValid() )
  {
    // need to add font to list
    IBoolean added = _colors.add( color, cursor );
    IASSERTSTATE( added );
    // increment font counter
    _colorNum++;
  }

  // send tag to set the font
  return cursor.element().getKey();
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleColor
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Handle a foreground color change.
 ***************************************************************************/
void RtfGenerator::handleColor( const ColorGin & gin )
{
  RtfColor color( gin.color().redMix(), gin.color().greenMix(), gin.color().blueMix(), _colorNum );
  int n = addColor( color );
  sendText( IString("\\cf") + IString(n), delimAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleBackColor
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Handle background color change.
 ***************************************************************************/
void RtfGenerator::handleBackColor( const BackColorGin & gin )
{
  RtfColor color( gin.color().redMix(), gin.color().greenMix(), gin.color().blueMix(), _colorNum );
  int n = addColor( color );
  sendText( IString("\\cb") + IString(n), delimAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleBold
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Handle bold attribute.
 ***************************************************************************/
void RtfGenerator::handleBold( const BoldGin & bold )
{
  sendText( IString("\\b") + IString(bold.isOn()), delimAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleItalic
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Handle italic attribute.
 ***************************************************************************/
void RtfGenerator::handleItalic( const ItalicGin & italic )
{
  sendText( IString("\\i") + IString(italic.isOn()), delimAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleUnderline
 * Author..... Mat
 * Date....... 2/18/99
 *
 * Handle underline attribute.
 ***************************************************************************/
void RtfGenerator::handleUnderline( const UnderlineGin & underline )
{
  sendText( IString("\\ul") + IString(underline.isOn()), delimAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleComment
 * Author..... Mat
 * Date....... 3/4/99
 *
 * Handle comment attribute which turns comment mode on or off.
 ***************************************************************************/
void RtfGenerator::handleComment( const CommentGin & comment )
{
  _context->isComment = comment.isOn();
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleLink
 * Author..... Mat
 * Date....... 2/19/99
 *
 * Handle a change to the link attribute.  This may mean the link is being
 * turned on or off or changed.
 ***************************************************************************/
void RtfGenerator::handleLink( const LinkGin & link )
{
  // ignored for document output
  if ( ! _hpj )
     return;

  // check if a link is ending
  if ( linkChange( _context->link, link ) )
    sendLink( _context->link );

  // start saving link text
  if ( linkChange( link, _context->link ) )
    startLink();

  // save link info
  _context->link = link;
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleArt
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Handle inline artwork.  This is not supported in document mode.
 ***************************************************************************/
void RtfGenerator::handleArt( const ArtGin & art )
{
  if ( _hpj )
  {
    // determine position
    Boolean isCentered = false;
    char align = 'c';
    switch ( art.spot() )
    {
    case ArtGin::left:
      align = 'l';
      break;
    case ArtGin::right:
      align = 'r';
      break;
    case ArtGin::center:
    case ArtGin::fit:
      isCentered = true;
      break;
    }

    // start centered block
    if ( isCentered )
    {
      resetPara();
      sendText( "{\\qc", breakBefore | delimAfter );
    }

    // send a meta-tag
    sendText( IString("\\{bm") + IString(align) + IString(' ') + _hpj->artFilename( art.id() ).name() + IString("\\}") );
    _para = started;

    // end centered block
    if ( isCentered )
    {
      sendText( '}' );
      resetPara();
    }
  }
  else
  {
    // default action for document mode
    Generator::handleArt( art );
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleText
 * Author..... Mat
 * Date....... 3/4/99
 *
 * Handle normal text.  If comment is active, ignore it.
 ***************************************************************************/
void RtfGenerator::handleText( const TextGin & text )
{
  if ( ! _context->isComment )
  {
    // convert text to RTF codepage
    IString out( text.string() );
    _cpOut.translateString( out, _context->cp );

    // send the text
    sendText( out, checkSpecial );

    // mark paragraph started
    _para = started;
  }
}


/***************************************************************************
 * Procedure.. RtfGenerator::handleSymbol
 * Author..... Mat
 * Date....... 2/23/99
 *
 * Handle a symbol.  If the symbol has a special RTF control word, use that.
 * Otherwise, convert the symbol to a character.  If comment is active,
 * ignore it.
 ***************************************************************************/
void RtfGenerator::handleSymbol( const SymbolGin & symbol )
{
  if ( _context->isComment )
    return;

  switch ( symbol.symbol().id() )
  {
    case Symbol::bullet:
      sendText( "\\bullet", delimAfter );
      break;
    case Symbol::odq:
      sendText( "\\ldblquote", delimAfter );
      break;
    case Symbol::cdq:
      sendText( "\\rdblquote", delimAfter );
      break;
    case Symbol::osq:
      sendText( "\\lquote", delimAfter );
      break;
    case Symbol::csq:
      sendText( "\\rquote", delimAfter );
      break;
    case Symbol::mspace:
      sendText( "\\emspace", delimAfter );
      break;
    case Symbol::nspace:
      sendText( "\\enspace", delimAfter );
      break;
    case Symbol::emdash:
      sendText( "\\emdash", delimAfter );
      break;
    case Symbol::endash:
      sendText( "\\endash", delimAfter );
      break;
    case Symbol::shy:
      // soft (optional) hyphen
      sendText( "\\-", delimAfter );
      break;
    case Symbol::rhy:
      // required (non-breaking) hyphen
      sendText( "\\_", delimAfter );
      break;
    case Symbol::rbl:
      // required (non-breaking) space
      sendText( "\\~", delimAfter );
      break;
    default:
      // send as text
      unsigned char ch = symbol.symbol().asChar( _cpOut );
      sendText( ch? ch: CodePage::substitution, checkSpecial );
      break;
  }

  _para = started;
}


/***************************************************************************
 * MARGINS & SPACING
 ***************************************************************************/

/***************************************************************************
 * Procedure.. RtfGenerator::setRightMargin
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Sets a new right margin.  If isRelative, then the new margin is relative to
 * the current margin.
 ***************************************************************************/
void RtfGenerator::setRightMargin( int twips, Boolean isRelative )
{
  int currentRightMargin = _context->rightMargin;

  if ( isRelative )
    _context->rightMargin = _context->baseRightMargin + twips;
  else
    _context->rightMargin = twips;

  if ( currentRightMargin != _context->rightMargin )
  {
    resetPara();
    sendText( IString("\\ri") + IString(_context->rightMargin), breakBefore | delimAfter );
  }
}

/***************************************************************************
 * Procedure.. RtfGenerator::setLeftMargin
 * Author..... Mat
 * Date....... 2/25/99
 *
 * Sets a new left margin.  If isRelative, then the new margin is relative to
 * the current margin.
 ***************************************************************************/
void RtfGenerator::setLeftMargin( int twips, Boolean isRelative )
{
  int currentLeftMargin = _context->leftMargin;

  // update current margin
  if ( isRelative )
    _context->leftMargin = _context->baseLeftMargin + twips;
  else
    _context->leftMargin = twips;

  if ( currentLeftMargin != _context->leftMargin )
    sendLeftMarginTab();
}

/***************************************************************************
 * Procedure.. RtfGenerator::setTab
 * Author..... Mat
 * Date....... 3/23/99
 *
 * Set the tab position for lists.  Indent the left margin to the previous
 * tab stop (if there was one), then set the tab to the given value.
 ***************************************************************************/
void RtfGenerator::setTab( int twips )
{
  _context->leftMargin += _context->tab;
  _context->tab = twips;
  sendLeftMarginTab();
}

/***************************************************************************
 * Procedure.. RtfGenerator::sendLeftMarginTab
 * Author..... Mat
 * Date....... 3/23/99
 *
 * Send updated left indent, first indent and tab settings.
 ***************************************************************************/
void RtfGenerator::sendLeftMarginTab()
{
  resetPara();
  IString margin( _context->leftMargin + _context->tab );
  sendText( IString("\\li") + margin, breakBefore | delimAfter );
  sendText( IString("\\fi") + IString(-_context->tab), delimAfter );
  sendText( IString("\\tx") + margin, delimAfter );
}

/***************************************************************************
 * Procedure.. RtfGenerator::setSpacing( int twips )
 * Author..... Mat
 * Date....... 3/2/99
 *
 * Sets the line spacing (the space after each paragraph in twips).
 ***************************************************************************/
void RtfGenerator::setSpacing( int twips )
{
  _context->spacing = twips;
  resetPara();
  sendText( IString("\\sa") + IString(_context->spacing), breakBefore | delimAfter );
}


/***************************************************************************
 * LINKS
 ***************************************************************************/

/***************************************************************************
 * Procedure.. RtfGenerator::linkChange
 * Author..... Mat
 * Date....... 2/19/99
 *
 * Returns true if the link is valid and different from the test link.
 ***************************************************************************/
Boolean RtfGenerator::linkChange( const LinkGin & link, const LinkGin & test )
{
  return link.target().length()
      && ( ( link.target() != test.target() )
        || ( link.file() != test.file() ) );
}


/***************************************************************************
 * Procedure.. RtfGenerator::startLink
 * Author..... Mat
 * Date....... 2/23/99
 *
 * Start capturing text for a hyperlink.
 ***************************************************************************/
void RtfGenerator::startLink()
{
  sendText( "\\strike", delimAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::sendLink
 * Author..... Mat
 * Date....... 2/19/99
 *
 * Send link information for the specified link.
 ***************************************************************************/
void RtfGenerator::sendLink( const LinkGin & link )
{
  // get link terget with filename
  IString target = link.target();
  if ( link.isExternal() )
    target += IString('@') + link.file();

  // end strikeout and send link target
  sendText( "\\strike0{\\v", delimAfter );
  sendText( target, checkSpecial );
  sendText( "}" );
}


/***************************************************************************
 * TABLES
 ***************************************************************************/

/***************************************************************************
 * Procedure.. RtfGenerator::sendTableBorders
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Send controls to set table borders, based on the given border settings.
 ***************************************************************************/
void RtfGenerator::sendTableBorders( unsigned borders )
{
  resetPara();
  IString control;
  if ( borders & BorderGin::top )
    control += IString("\\trbrdrt\\brdrs");
  if ( borders & BorderGin::bottom )
    control += IString("\\trbrdrb\\brdrs");
  if ( borders & BorderGin::right )
    control += IString("\\trbrdrr\\brdrs");
  if ( borders & BorderGin::left )
    control += IString("\\trbrdrl\\brdrs");
  if ( borders & BorderGin::vertical )
    control += IString("\\trbrdrv\\brdrs");
  if ( borders & BorderGin::horizontal )
    control += IString("\\trbrdrh\\brdrs");
  sendText( control, delimAfter );
}


/***************************************************************************
 * Procedure.. RtfGenerator::startTable
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Send controls to start a table and initialize cell and row counters.
 ***************************************************************************/
void RtfGenerator::startTable( const RulerGin & ruler )
{
  resetPara();

  // set table defaults
  sendText( "\\trowd\\trgaph0", breakBefore );

  // add borders
  sendTableBorders( _context->borders );

  // add cell widths
  IString control;
  INumber i;
  int x = 0;
  for ( i = 1; i <= ruler.numberOfColumns(); i++ )
  {
    x += ruler.column(i).asTwips();
    control += IString("\\cellx") + IString(x);
  }
  sendText( control, breakAfter );

  // reset counters
  _cellCount = _rowCount = 0;
}


/***************************************************************************
 * Procedure.. RtfGenerator::endTable
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Send controls to end a table.
 ***************************************************************************/
void RtfGenerator::endTable()
{
  // end current cell and row
  endCell();
  endRow( true );
}


/***************************************************************************
 * Procedure.. RtfGenerator::endRow
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Send controls to end a row.  If another row is coming, then send \intbl
 * to re-assert that we are in a table.
 ***************************************************************************/
void RtfGenerator::endRow( Boolean isLastRow )
{
  if ( _rowCount++ )
    sendText( "\\row", delimAfter );
  if ( ! isLastRow )
    sendText( "\\intbl", delimAfter );
  _cellCount = 1;
}


/***************************************************************************
 * Procedure.. RtfGenerator::endCell
 * Author..... Mat
 * Date....... 3/11/99
 *
 * Send control to end the current table cell.
 ***************************************************************************/
void RtfGenerator::endCell()
{
  if ( _cellCount++ )
    sendText( "\\cell", breakAfter | delimAfter );
}


/***************************************************************************
 * RULER MODE
 ***************************************************************************/

/***************************************************************************
 * Procedure.. RtfGenerator::endRuler
 * Author..... Mat
 * Date....... 2/26/99
 *
 * Called when the ruler changes and the current mode must be ended.
 ***************************************************************************/
void RtfGenerator::endRuler()
{
  // special code for ending table
  if ( ( _context->mode == RulerGin::table ) && ! _context->tableNested )
    endTable();

  resetPara();

  // for all modes, reset paragraph styles and restore margins and spacing
  sendText( "\\pard", delimAfter );
  _context->rightMargin = _context->leftMargin = _context->tab = 0;
  // set left margin and tab together, if needed
  if ( _context->baseLeftMargin )
    _context->leftMargin = _context->baseLeftMargin;
  if ( _context->baseTab )
    _context->tab = _context->baseTab;
  if ( _context->baseLeftMargin || _context->baseTab )
    sendLeftMarginTab();
  if ( _context->baseRightMargin )
    setRightMargin( _context->baseRightMargin, false );
  if ( _context->spacing )
    setSpacing( _context->spacing );
  if ( _context->tableNested )
    sendText( "\\intbl", delimAfter );

  // reset context settings
  _context->reset( false );
}



/***************************************************************************
 * PARAGRAPH SUPPORT
 ***************************************************************************/

// end a paragraph from a caller request
void RtfGenerator::endPara()
{
  sendText( "\\par", breakBefore | delimAfter );
  _para = ended;
}

// force a paragraph end from a style change
void RtfGenerator::resetPara()
{
  if ( _para == started )
  {
    sendText( "\\par", breakBefore | delimAfter );
    _para = reset;
  }
}


