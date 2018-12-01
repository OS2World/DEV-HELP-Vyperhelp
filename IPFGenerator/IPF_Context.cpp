/***************************************************************************
 * File...... IPF_Context.cpp
 * Author.... Mat
 * Date...... 10/24/95
 *
 * Implementation for IPFContext.  The IPFGenerator holds two different
 * contexts: _active and _next.  _active is the current context as
 * represented by the state of the output tags.  _next is the requested
 * state as manipulated by the incoming Gins.  _next is used to cache
 * changes until tags or text need to be output to the IPF destination.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// IPFGenerator
#include "IPF_Common.hpp"


// PROTOTYPES
static IString GetFontTag( const IPFContext & context );
static int GetHp( const IPFContext & context );
static IString GetModeTag( const IPFContext & context, Boolean wantEndTag );
static Boolean ModeAttributesDiffer( const IPFContext &, const IPFContext & );


//
// TEMPLATE DEFINITIONS
//
#include <ilnseq.c>
#include <iqueseq.c>
#include <istkseq.c>

// ContextStack
#pragma define(IGLnSqCrs<IPFContext,IStdOps<IPFContext> >)
#pragma define(IGLinkedSequenceNode<IPFContext,IStdOps<IPFContext> >)
#pragma define(IGLnSq<IPFContext,IStdOps<IPFContext> >)
#pragma define(IWStackOnSeqCursor<IPFContext,IStdOps<IPFContext>,IGLnSq<IPFContext,IStdOps<IPFContext> > >)
#pragma define(IWStOnSq<IPFContext,IStdOps<IPFContext>,IGLnSq<IPFContext,IStdOps<IPFContext> > >)

// ColumnList
#pragma define(IGLnSqCrs<int,IStdOps<int> >)
#pragma define(IGLinkedSequenceNode<int,IStdOps<int> >)
#pragma define(IGLnSq<int,IStdOps<int> >)
#pragma define(IWQueueOnSeqCursor<int,IStdOps<int>,IGLnSq<int,IStdOps<int> > >)
#pragma define(IWQuOnSq<int,IStdOps<int>,IGLnSq<int,IStdOps<int> > >)



/***************************************************************************
 * Procedure.. ContextStack::ContextStack
 * Author..... Mat
 * Date....... 3/12/96
 *
 * Constructor for ContextStack
 ***************************************************************************/
ContextStack::ContextStack( INumber numberOfElements ):
    IStack< IPFContext >( numberOfElements )
{}


/***************************************************************************
 * Procedure.. IPFContext::IPFContext
 * Author..... Mat
 * Date....... 10/13/95
 *
 * Constructor for IPFContext sets up a default context
 ***************************************************************************/
IPFContext::IPFContext():
    leftMarginChars( 1 ),
    rightMarginChars( 1 ),
    spacingLines( 1 ),
    align( alignLeft ),
    list( listSimple ),
    mode( paragraph ),
    level( 0 ),
    minLevel( 0 ),
    maxLevel( 0 ),
    forceModeChange( false ),
    columnChars( 20 ),
    borders( 0xFF ),  // default table borders = all
    isBold( false ),
    isItalic( false ),
    isUnderline( false ),
    isComment( false ),
    foregroundColor( colorDefault ),
    backgroundColor( colorDefault ),
    font( fontDefault ),
    pointSize( 0 )
{}



/***************************************************************************
 * Procedure.. IPFGenerator::needsSwitchTo()
 * Author..... Mat
 * Date....... 10/31/95
 *
 * Returns true if the _active context needs to be switched in order to
 * match the given test context, considering the current tableMode.
 ***************************************************************************/
Boolean IPFGenerator::needsSwitchTo( const IPFContext & test )
{
  if ( _tableMode == notInTable )
  {
    // margins, font and attribute cannot change in a table
    if (
      ( _active->leftMarginChars  != test.leftMarginChars  )   ||
      ( _active->rightMarginChars != test.rightMarginChars )   ||
      ( _active->isBold           != test.isBold           )   ||
      ( _active->isItalic         != test.isItalic         )   ||
      ( _active->isUnderline      != test.isUnderline      )   ||
      ( _active->font             != test.font             )   ||
      ( _active->pointSize        != test.pointSize        )   ||
      ( _active->outputCodepage         != test.outputCodepage         )   )
    {
      return true;
    }
  } /* endif */

  if ( _tableMode != inLimbo )
  {
    // color, link and hide cannot change while in "limbo" (before first cell)
    if (
      ( _active->foregroundColor  != test.foregroundColor  )   ||
      ( _active->backgroundColor  != test.backgroundColor  )   ||
      ( _active->key              != test.key              )   ||
      ( _active->linkTarget       != test.linkTarget       )   ||
      ( _active->linkTargetFile   != test.linkTargetFile   )   )
    {
      return true;
    }
  } /* endif */

  if (
    test.forceModeChange                    ||
    ( _active->mode != test.mode )          ||
    ModeAttributesDiffer( *_active, test )  )
  {
    return true;
  } /* endif */

  return false;
}


/***************************************************************************
 * Procedure.. IPFGenerator::nextCell
 * Author..... Mat
 * Date....... 3/14/96
 *
 * Prepares to move to the next cell in a table.  IPF tables don't allow
 * links or hidden text to span a cell, so we must turn them off as we
 * move over the cell boundary.
 *
 * isRow is true if this is called from a RowGin as opposed to a ColumnGin.
 ***************************************************************************/
IPFGenerator & IPFGenerator::nextCell( Boolean isRow )
{
  // save the current context
  IString saveLink =     _next->linkTarget;
  IString saveLinkFile = _next->linkTargetFile;
  IString saveHide =     _next->key;

  // turn off links and hides
  _next->linkTarget = _next->key = IString();

  // send the tags (and update the context)
  if ( isRow )
    sendTag( "row", breakBefore | isFormat );
  sendTag( "c", breakBefore | isFormat );
  _tableMode = inCell;

  // restore the context
  _next->linkTarget =     saveLink;
  _next->linkTargetFile = saveLinkFile;
  _next->key =            saveHide;

  return *this;
}



/***************************************************************************
 * Procedure.. IPFGenerator::operator<<( const PushGin         & )
 * Author..... Mat
 * Date....... 10/27/95
 *
 * Pushes the current caller context (_next) on the stack, and sets the
 * minLevel and maxLevel to allow for nesting.  minLevel and maxLevel
 * are used to determine when nesting or ruler modes should be used.
 * Initially both are 0, indicating that no nesting is allowed.
 * Whenever a Push is encountered, the maxLevel is incremented by one
 * to allow for one level of nesting.  minLevel is maintained for
 * modes which don't support nesting (such as "paragraph") so they can
 * return to an "un-nested" state.
 *
 * Before saving the context the active context is updated.
 ***************************************************************************/
void IPFGenerator::handlePush( const PushGin         & push )
{
  switchContext( *_next );
  _contextStack->push( *_next );
  _next->minLevel = _next->level;
  _next->maxLevel = _next->level + 1;
}


/***************************************************************************
 * Procedure.. IPFGenerator::operator<<( const PopGin          & )
 * Author..... Mat
 * Date....... 10/27/95
 *
 * Pops the _next context from the stack and updates the active context.
 ***************************************************************************/
void IPFGenerator::handlePop( const PopGin          & pop )
{
  _contextStack->pop( *_next );
  switchContext( *_next );
}



/***************************************************************************
 * Procedure.. IPFGenerator::switchContext
 * Author..... Mat
 * Date....... 10/27/95
 *
 * This function will output the necessary IPF tags to change the context
 * from _active to the supplied new context.
 *
 * IPF tables impose the following restrictions:
 *   1. no artwork (see ArtGin handler)
 *   2. no nesting of modes (see RulerGin handler)
 *   3. no changing of font face, attributes, or margings
 *   4. color is reset upon entering table
 *   5. within a table, links and hidden text cannot cross cell boundaries
 *   6. no tags should appear between :table :row and the first :cell
 ***************************************************************************/
IPFGenerator & IPFGenerator::switchContext( const IPFContext & next )
{
  static const char * colors[17] = {
      "default",
      "blue",
      "cyan",
      "green",
      "neutral",
      "red",
      "yellow",
      "black",
      "pink",
      "neutral",  // IPFC doesn't accept brown, so use neutral which is the same
      "darkgray",
      "darkblue",
      "darkred",
      "darkpink",
      "darkgreen",
      "darkcyan",
      "palegray"
  };
  Boolean needFontReset =
      ( _active->font      != next.font ) ||
      ( _active->pointSize != next.pointSize ) ||
      ( _active->outputCodepage  != next.outputCodepage );
  Boolean needAttrReset =
      needFontReset ||               // :font tag clears all attributes
      ( _active->isBold      != next.isBold ) ||
      ( _active->isItalic    != next.isItalic ) ||
      ( _active->isUnderline != next.isUnderline );
  Boolean needColorReset =
      ( _active->foregroundColor != next.foregroundColor ) ||
      ( _active->backgroundColor != next.backgroundColor );
  Boolean needLinkReset =
      ( _active->linkTarget     != next.linkTarget ) ||
      ( _active->linkTargetFile != next.linkTargetFile );
  Boolean needHideReset =
      ( _active->key != next.key );
  Boolean needLeftMarginReset =
      ( _active->leftMarginChars != next.leftMarginChars );
  Boolean needRightMarginReset =
      ( _active->rightMarginChars != next.rightMarginChars );


  // FLAG THAT UPDATE IS IN PROGRESS
  // output previous heading tag if it is still waiting (empty section)
  if ( _mode == waitingForLabel ) {
    tag_h();
  } /* endif */
  IASSERTSTATE( _mode == ready );
  _mode = updatingContext;

  // CLOSE CHARACTER STYLES IF NECESSARY
  if ( needHideReset && _tableMode != inLimbo ) {
    if ( _active->key.length() ) {
      sendTag( "ehide", 0 );
      _active->key = IString();
    } /* endif */
  } /* endif */

  if ( needLinkReset && _tableMode != inLimbo ) {
    if ( _active->linkTarget.length() ) {
      sendTag( "elink", 0 );
      _active->linkTarget = IString();
    } /* endif */
  } /* endif */

  if ( needAttrReset && _tableMode == notInTable ) {
    // turn off existing hp tag
    int hpnum = GetHp( *_active );
    if ( hpnum ) {
      sendTag( "ehp" + IString( hpnum ), 0 );
      _active->isBold =
      _active->isItalic =
      _active->isUnderline = false;
    } /* endif */
  } /* endif */

  // PARAGRAPH MODE
  //
  // Mode change occurs if any one of the following is true:
  //   - different modes
  //   - different nesting levels
  //   - explicit forced mode change (certain Gins imply mode change)
  //   - different mode attributes (non-paragraph modes)
  //
  if ( ( _active->mode != next.mode ) ||
       ( _active->level != next.level ) ||
       ( next.forceModeChange ) ||
       ( ModeAttributesDiffer( *_active, next ) ) )
  {

    // send close tag for active mode, if necessary
    if ( _active->mode != paragraph ) {
      // non-paragraph mode requires tags
      if ( next.level <= _active->level ) {
        // not nesting: close the active mode
        sendTag( GetModeTag( *_active, true ), breakBefore | breakAfter | isFormat );
        if ( _active->mode == table ) {
          // leaving table (colors are reset by IPF)
          _tableMode = notInTable;
          _active->foregroundColor = _active->backgroundColor = colorDefault;
        }
      } /* endif */
    } /* endif */

    // send open tag for new mode, if necessary
    if ( next.mode != paragraph ) {
      if ( next.level >= _active->level ) {
        // not returning from nesting: open the next mode
        if ( next.mode == table ) {
          prepForTable();
        } /* endif */
        sendTag( GetModeTag( next, false ), breakBefore | breakAfter | isFormat );
        if ( next.mode == table ) {
          // entering table (colors are reset by IPF)
          _tableMode = inLimbo;
          _active->foregroundColor = _active->backgroundColor = colorDefault;
        } /* endif */
      } /* endif */
    } /* endif */

    // update active context
    _active->mode = next.mode;
    _active->spacingLines = next.spacingLines;
    _active->align = next.align;
    _active->list = next.list;
    _active->columnChars = next.columnChars;
    _active->borders = next.borders;
    _active->level = next.level;
    _active->minLevel = next.minLevel;
    _active->maxLevel = next.maxLevel;
  } /* endif */

  // MARGINS
  if ( needLeftMarginReset && _tableMode == notInTable )
  {
    sendTag(
        "lm margin=" + IString( next.leftMarginChars ),
        breakBefore | breakAfter | isFormat );
    _active->leftMarginChars = next.leftMarginChars;
  }
  if ( needRightMarginReset && _tableMode == notInTable )
  {
    sendTag(
        "rm margin=" + IString( next.rightMarginChars ),
        breakBefore | breakAfter | isFormat );
    _active->rightMarginChars = next.rightMarginChars;
  }

  // OPEN CHARACTER STYLES
  if ( needFontReset && _tableMode == notInTable )
  {
    sendTag( GetFontTag( next ), noWrap );
    _active->font = next.font;
    _active->pointSize = next.pointSize;
    _active->outputCodepage = next.outputCodepage;
  }

  if ( needAttrReset && _tableMode == notInTable )
  {
    // turn on the next hp tag
    int hpnum = GetHp( next );
    if ( hpnum ) {
      sendTag( "hp" + IString( hpnum ), 0 );
    } /* endif */
    _active->isBold = next.isBold;
    _active->isItalic = next.isItalic;
    _active->isUnderline = next.isUnderline;
  }

  if ( needLinkReset && _tableMode != inLimbo )
  {
    sendLinkTag( next );
    _active->linkTarget = next.linkTarget;
    _active->linkTargetFile = next.linkTargetFile;
  }

  if ( needHideReset && _tableMode != inLimbo ) {
    if ( next.key.length() ) {
      sendTag( "hide key='" + next.key + "'", 0 );
    } /* endif */
    _active->key = next.key;
  } /* endif */

  if ( needColorReset && _tableMode != inLimbo ) {
    sendTag(
        "color fc=" + IString( colors[ next.foregroundColor ] ) +
        " bc=" + IString( colors[ next.backgroundColor ] ),
        0 );
    _active->foregroundColor = next.foregroundColor;
    _active->backgroundColor = next.backgroundColor;
  } /* endif */

  // SAVE NEW CONTEXT
  *_next = next;
  _next->forceModeChange = false;
  _mode = ready;

  return *this;
}


/***************************************************************************
 * Procedure.. IPFGenerator::sendLinkTag
 * Author..... Mat
 * Date....... 1/6/98
 *
 * Send a link tag constructed from the given context information.
 ***************************************************************************/
void IPFGenerator::sendLinkTag( const IPFContext & next )
{
  if ( next.linkTarget.length() )
  {
    IString tag = "link reftype=hd refid='" + next.linkTarget + "'";
    if ( next.linkTargetFile.length() )
      tag += " database='" + next.linkTargetFile + "'";
    sendTag( tag, 0 );
  }
}


/***************************************************************************
 * Procedure.. IPFGenerator::prepForTable
 * Author..... Mat
 * Date....... 3/14/96
 *
 * Prepares the active context for a table.  IPF tables are picky: they
 * require that you use a monospaced font and that you don't use bold
 * or italic.  Underline is OK, but for consistency we don't allow it here.
 * IPFC also assumes that the codepage will support line-drawing characters.
 *
 ***************************************************************************/
IPFGenerator & IPFGenerator::prepForTable()
{
  // turn off attributes
  int hpnum = GetHp( *_active );
  if ( hpnum ) {
    sendTag( "ehp" + IString( hpnum ), 0 );
    _active->isBold =
    _active->isItalic =
    _active->isUnderline = false;
  } /* endif */


  // make sure font is monospaced (Courier)
  Boolean needFontReset = false;
  if ( _active->font != fontCourier )
  {
    _active->font = fontCourier;
    needFontReset = true;
  }

  // if active code page doesn't support code linedraw code points as 850
  // then reset codepage to 850 while in table
  static const char tableChars[] = "³´¿ÀÁÂÃÄÅÙÚ";
  IString translated( tableChars );
  _active->outputCodepage.translateString( translated, 850 );
  if ( translated != tableChars )
  {
    _active->outputCodepage = 850;
    needFontReset = true;
  }

  if ( needFontReset )
    sendTag( GetFontTag( *_active ), noWrap );

  return *this;
}


/***************************************************************************
 * Procedure.. GetFontTag
 * Author..... Mat
 * Date....... 3/14/96
 *
 * For the given context, return the appropriate :font tag.
 *
 * NOTE: this tag should not be split across lines, because spaces in facenames
 * may not translate correctly.  Use noWrap in sendTag().
 ***************************************************************************/
static IString GetFontTag( const IPFContext & context )
{
  static const char * facenames[4] = {
      "Courier",
      "'Tms Rmn'",
      "Helv",
      "default"
  };

  IString tag = "font facename=" + IString( facenames[ context.font ] ) +
      " size=" + IString( context.pointSize ) + "x" + IString( context.pointSize );
  IASSERTSTATE( context.outputCodepage >= 0 );
  // NOTE! codepage is ignored if facename=default ("active" system codepage is used)
  if ( context.outputCodepage )
    tag += " codepage=" + IString( context.outputCodepage );
  return tag;
}


/***************************************************************************
 * Procedure.. GetHp
 * Author..... Mat
 * Date....... 10/27/95
 *
 * Returns the number of the :hp tag for the given contest's combination
 * of italic, bold and underline.  IPF doesn't support all three
 * attributes, so in that case, we use the otherwise unused :hp9 (pink).
 * If no attributes are ON, 0 is returned.
 ***************************************************************************/
static int GetHp( const IPFContext & context )
{
  static const int hpnum[7] = {  // uline  bold  italic
      1,                  //     0     0       1
      2,                  //     0     1       0
      3,                  //     0     1       1
      5,                  //     1     0       0
      6,                  //     1     0       1
      7,                  //     1     1       0
      9                   //     1     1       1  (use :hp9=pink!)
  };
  int hpLookup =
      context.isItalic +           // bit 0
      (context.isBold << 1) +      // bit 1
      (context.isUnderline << 2);  // bit 2
  if ( hpLookup ) {
    return hpnum[ hpLookup - 1 ];
  } /* endif */
  return 0;  // no special attributes
}


/***************************************************************************
 * Procedure.. GetModeTag
 * Author..... Mat
 * Date....... 10/27/95
 *
 * Returns the tag for the given context's modes, including all attributes.
 * If wantEndTag is set, then the attributes are not included, and the tag
 * is prefixed by an "e".  This should not be called if context.mode =
 * paragraph.
 ***************************************************************************/
static IString GetModeTag( const IPFContext & context, Boolean wantEndTag )
{
  Boolean checkCompact = false;
  Boolean checkAlign = false;
  Boolean checkDList = false;
  Boolean checkTable = false;
  IString tag;

  //
  // GET TAG NAME
  //
  if ( wantEndTag ) {
    tag = "e";  // prefix
  } /* endif */

  switch ( context.mode ) {
  case singleList:
    switch ( context.list ) {
    case listSimple:
      tag += "sl";
      break;
    case listUnordered:
      tag += "ul";
      break;
    case listOrdered:
      tag += "ol";
      break;
    default:
      BADSWITCH;
      break;
    } /* endswitch */
    checkCompact = true;
    break;
  case lines:
    tag += "lines";
    checkAlign = true;
    break;
  case doubleList:
  case doubleListBreak:
  case doubleListFit:
    tag += "dl";
    checkCompact = true;
    checkDList = true;
    break;
  case table:
    tag += "table";
    checkTable = true;
    break;
  default:
    BADSWITCH;
    break;
  } /* endswitch */

  if ( wantEndTag ) {
    return tag;  // no attributes for end tag
  } /* endif */


  //
  // GET ATTRIBUTES
  //
  if ( checkCompact && context.spacingLines < 1 ) {
    tag += " compact";
  } /* endif */

  if ( checkAlign ) {
    tag += " align=";
    switch ( context.align ) {
    case alignLeft:
      tag += "left";
      break;
    case alignCenter:
      tag += "center";
      break;
    case alignRight:
      tag += "right";
      break;
    default:
      BADSWITCH;
      break;
    } /* endswitch */
  } /* endif */

  if ( checkDList ) {
    tag += " tsize=" + IString( context.columnChars.elementAtPosition( 1 ) );
    tag += " break=";
    switch ( context.mode ) {
    case doubleList:
      tag += "none";
      break;
    case doubleListBreak:
      tag += "all";
      break;
    case doubleListFit:
      tag += "fit";
      break;
    default:
      BADSWITCH;
      break;
    } /* endswitch */
  } /* endif */

  if ( checkTable ) {

    // COLUMNS
    int i, n = context.columnChars.numberOfElements();
    IASSERTSTATE( n > 0 );
    tag += " cols='";
    for (i = 1; i <= n; i++) {
      tag += IString( context.columnChars.elementAtPosition( i ) );
      if ( i < n ) {
        tag += " ";
      } /* endif */
    } /* endfor */

    // RULES
    int vertAndHoriz = BorderGin::vertical | BorderGin::horizontal;
    tag += "' rules=";
    if ( ( context.borders & vertAndHoriz ) == vertAndHoriz ) {
      tag += "both";
    } else if ( context.borders & BorderGin::vertical ) {
      tag += "vert";
    } else if ( context.borders & BorderGin::horizontal ) {
      tag += "horiz";
    } else {
      tag += "none";
    } /* endif */

    // FRAME
    tag += " frame=";
    if ( context.borders & ( BorderGin::right | BorderGin::left ) ) {
      tag += "box";  // full box if any vertical border was requested
    } else if ( context.borders & ( BorderGin::top | BorderGin::bottom ) ) {
      tag += "rules";
    } else {
      tag += "none";
    } /* endif */

  } /* endif */

  return tag;
}


/***************************************************************************
 * Procedure.. ModeAttributesDiffer
 * Author..... Mat
 * Date....... 1/23/96
 *
 * Returns true if the tag attributes for the two modes are different.
 * Assumes the two modes are the same.
 ***************************************************************************/
static Boolean ModeAttributesDiffer(
    const IPFContext & context1,
    const IPFContext & context2 )
{
  IASSERTSTATE( context1.mode == context2.mode );
  if ( context1.mode == paragraph ) {
    return false;  // no attributes for paragraph
  } /* endif */
  return ( GetModeTag( context1, false ) != GetModeTag( context2, false ) );
}

