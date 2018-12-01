/***************************************************************************
 * File...... HtmlContext.cpp
 *
 * Implementation for HtmlContext.  The HtmlGenerator holds two different
 * contexts: _active and _next.  _active is the current context as
 * represented by the state of the output tags.  _next is the requested
 * state as manipulated by the incoming Gins.  _next is used to cache
 * changes until tags or text need to be output to the HTML destination.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

// HtmlGenerator
#include "HtmlCommon.hpp"


//
// TEMPLATE DEFINITIONS
//
#include <ilnseq.c>
#include <iqueseq.c>
#include <istkseq.c>

// HtmlContextStack
#pragma define(IGLnSqCrs<HtmlContext,IStdOps<HtmlContext> >)
#pragma define(IGLinkedSequenceNode<HtmlContext,IStdOps<HtmlContext> >)
#pragma define(IGLnSq<HtmlContext,IStdOps<HtmlContext> >)
#pragma define(IWStackOnSeqCursor<HtmlContext,IStdOps<HtmlContext>,IGLnSq<HtmlContext,IStdOps<HtmlContext> > >)
#pragma define(IWStOnSq<HtmlContext,IStdOps<HtmlContext>,IGLnSq<HtmlContext,IStdOps<HtmlContext> > >)

// ColumnList
#pragma define(IGLnSqCrs<int,IStdOps<int> >)
#pragma define(IGLinkedSequenceNode<int,IStdOps<int> >)
#pragma define(IGLnSq<int,IStdOps<int> >)
#pragma define(IWQueueOnSeqCursor<int,IStdOps<int>,IGLnSq<int,IStdOps<int> > >)
#pragma define(IWQuOnSq<int,IStdOps<int>,IGLnSq<int,IStdOps<int> > >)



/***************************************************************************
 * Procedure.. HtmlContextStack::HtmlContextStack
 *
 * Constructor for HtmlContextStack
 ***************************************************************************/
HtmlContextStack::HtmlContextStack(INumber numberOfElements):
    IStack<HtmlContext>(numberOfElements)
{}


/***************************************************************************
 * Procedure.. HtmlContext::HtmlContext
 *
 * Constructor for HtmlContext sets up a default context
 ***************************************************************************/
HtmlContext::HtmlContext():
    spacingLines( 1 ),
    align( alignLeft ),
    list( listSimple ),
    hasItem( false ),
    mode( paragraph ),
    level( 0 ),
    minLevel( 0 ),
    maxLevel( 0 ),
    forceModeChange( false ),
    columnPixels( 10 ),
    hasBorder( false ),
    isBold( false ),
    isItalic( false ),
    isUnderline( false ),
    isComment( false ),
    color( IColor::black ),
    fontSize( fontRegular ),
    isMonospaced( false ),
    inputCodepage(0)
{}



/***************************************************************************
 * Procedure.. HtmlGenerator::needsSwitchTo()
 *
 * Returns true if the _active context needs to be switched in order to
 * match the given test context.
 ***************************************************************************/
Boolean HtmlGenerator::needsSwitchTo( const HtmlContext & test )
{
  if (
    ( _active->isBold           != test.isBold           )   ||
    ( _active->isItalic         != test.isItalic         )   ||
    ( _active->isUnderline      != test.isUnderline      )   ||
    ( _active->fontSize         != test.fontSize         )   ||
    ( _active->isMonospaced     != test.isMonospaced     )   ||
    ( _active->color            != test.color            )   ||
    ( _active->linkTarget       != test.linkTarget       )   ||
    ( _active->linkTargetFile   != test.linkTargetFile   )   ||
    ( _active->mode             != test.mode             )   ||
    ( _active->align            != test.align            )   ||
    test.forceModeChange                                     ||
    modeAttributesDiffer( *_active, test )  )
  {
    return true;
  }

  return false;
}



/***************************************************************************
 * Procedure.. HtmlGenerator::operator<<( const PushGin         & )
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
void HtmlGenerator::handlePush( const PushGin         & push )
{
  switchContext( *_next );
  _contextStack->push( *_next );
  _next->minLevel = _next->level;
  _next->maxLevel = _next->level + 1;
}


/***************************************************************************
 * Procedure.. HtmlGenerator::operator<<( const PopGin          & )
 *
 * Pops the _next context from the stack and updates the active context.
 ***************************************************************************/
void HtmlGenerator::handlePop( const PopGin          & pop )
{
  _contextStack->pop( *_next );
  switchContext( *_next );
}


/***************************************************************************
 * Procedure.. HtmlGenerator::switchContext
 *
 * This function will output the necessary Html tags to change the context
 * from _active to the supplied new context.
 *
 ***************************************************************************/
HtmlGenerator & HtmlGenerator::switchContext( const HtmlContext & next )
{
  static const HtmlTag tagBold("b", false);
  static const HtmlTag tagItalic("i", false);
  static const HtmlTag tagUnderline("u", false);
  static const HtmlTag tagTeletype("tt", false);
  static const HtmlTag tagSmall("small", false);
  static const HtmlTag tagBig("big", false);
  static const IColor  colorBlack(IColor::black);

  // --- FLAG THAT UPDATE IS IN PROGRESS ---
  IASSERTSTATE( _mode == ready );
  _mode = updatingContext;

  // --- CLOSE CHARACTER STYLES IF NECESSARY ---

  // link
  Boolean needLinkReset =
      ( _active->linkTarget     != next.linkTarget ) ||
      ( _active->linkTargetFile != next.linkTargetFile );
  if ( needLinkReset && _active->linkTarget.length() )
  {
    sendEndTag("a");
    _active->linkTarget = IString();
  }

  // bold
  if (_active->isBold && ! next.isBold)
  {
    sendEndTag(tagBold.plain());
    _active->isBold = false;
  }

  // italic
  if (_active->isItalic && ! next.isItalic)
  {
    sendEndTag(tagItalic.plain());
    _active->isItalic = false;
  }

  // underline
  if (_active->isUnderline && ! next.isUnderline)
  {
    sendEndTag(tagUnderline.plain());
    _active->isUnderline = false;
  }

  // color
  if ((_active->color != colorBlack) && (_active->color != next.color))
  {
    sendEndTag("font");
    _active->color = colorBlack;
  }

  // monospaced
  if (_active->isMonospaced && ! next.isMonospaced)
  {
    sendEndTag(tagTeletype.plain());
    _active->isMonospaced = false;
  }

  // font size
  if (_active->fontSize != next.fontSize)
  {
    switch (_active->fontSize)
    {
    case fontSmall:
      sendEndTag(tagSmall.plain());
      break;
    case fontLarge:
      sendEndTag(tagBig.plain());
      break;
    }
    _active->fontSize = fontRegular;
  }

  // --- PARAGRAPH STYLES ---

  //
  // Mode change occurs if any one of the following is true:
  //   - different modes
  //   - different nesting levels
  //   - explicit forced mode change (certain Gins imply mode change)
  //   - different mode attributes (non-paragraph modes)
  //
  Boolean modeChange =
       ( _active->mode != next.mode ) ||
       ( _active->level != next.level ) ||
       ( next.forceModeChange ) ||
       ( modeAttributesDiffer( *_active, next ) );

  // alignment
  if ((_active->align != alignLeft) && ((_active->align != next.align) || modeChange))
  {
    sendEndTag("div");
  }

  if (modeChange)
  {
    // send close tag for active mode, if necessary
    if ( _active->mode != paragraph )
    {
      // non-paragraph mode requires tags
      if ( next.level <= _active->level )
      {
        // not nesting: close the active mode
        sendEndTag(getModeTag(*_active).plain(), _active->mode);
      }
    }

    // send open tag for new mode, if necessary
    if ( next.mode != paragraph )
    {
      if ( next.level >= _active->level )
      {
        // not returning from nesting: open the next mode
        sendStartTag(getModeTag(next), next.mode);
      }
    }
  }

  // alignment
  if ((next.align != alignLeft) && ((_active->align != next.align) || modeChange))
  {
    IString text = "div align=";
    if (next.align == alignCenter)
      text += "center";
    else
      text += "right";
    HtmlTag tag(text, true, breakBefore);
    sendStartTag(tag);
  }

  // update active context (paragraph styles)
  _active->mode = next.mode;
  _active->spacingLines = next.spacingLines;
  _active->align = next.align;
  _active->list = next.list;
  _active->hasItem = next.hasItem;
  _active->columnPixels = next.columnPixels;
  _active->hasBorder = next.hasBorder;
  _active->level = next.level;
  _active->minLevel = next.minLevel;
  _active->maxLevel = next.maxLevel;


  // --- OPEN CHARACTER STYLES ---

  // link
  if ( needLinkReset )
  {
    if (next.linkTarget.length())
    {
      // note: we don't support external links (linkTargetFile is ignored)
      IString ref = "a href=\"";
      if (_type != singleFile)
      {
        // internal link across files (potentially)
        // insert a dummy filename to be fixed later during fixupLabels()
        ref += numberedFile(0);
      }
      ref += IString('#') + fixedLabel(next.linkTarget) + "\"";
      HtmlTag tag(ref, false);
      sendStartTag(tag);
    }
    _active->linkTarget = next.linkTarget;
    _active->linkTargetFile = next.linkTargetFile;
  }

  // bold
  if (next.isBold && ! _active->isBold)
  {
    sendStartTag(tagBold);
    _active->isBold = true;
  }

  // italic
  if (next.isItalic && ! _active->isItalic)
  {
    sendStartTag(tagItalic);
    _active->isItalic = true;
  }

  // underline
  if (next.isUnderline && ! _active->isUnderline)
  {
    sendStartTag(tagUnderline);
    _active->isUnderline = true;
  }

  // color
  if ((next.color != colorBlack) && (_active->color != next.color))
  {
    HtmlTag tag(getFontColorTag(next.color), false);
    sendStartTag(tag);
    _active->color = next.color;
  }

  // monospaced
  if (next.isMonospaced && ! _active->isMonospaced)
  {
    sendStartTag(tagTeletype);
    _active->isMonospaced = true;
  }

  // font size
  if (_active->fontSize != next.fontSize)
  {
    switch (next.fontSize)
    {
    case fontSmall:
      sendStartTag(tagSmall);
      break;
    case fontLarge:
      sendStartTag(tagBig);
      break;
    }
    _active->fontSize = next.fontSize;
  }

  // --- SAVE NEW CONTEXT ---
  *_next = next;
  _next->forceModeChange = false;
  _mode = ready;

  return *this;
}


/***************************************************************************
 * Procedure.. getFontColorTag
 *
 * Construct an HTML <font> tag with a color attribute based on the given
 * IColor.  Color in HTML is in the form #RRGGBB, where RR is the hex value
 * of the red component, etc.
 ***************************************************************************/
IString HtmlGenerator::getFontColorTag(const IColor & color)
{
  IString tag = "font color=#";
  tag += IString((unsigned)color.redMix()).d2x().rightJustify(2, '0');
  tag += IString((unsigned)color.greenMix()).d2x().rightJustify(2, '0');
  tag += IString((unsigned)color.blueMix()).d2x().rightJustify(2, '0');
  return tag;
}



/***************************************************************************
 * Procedure.. getModeTag
 *
 * Returns the tag for the given context's modes, including all attributes.
 * This should not be called if context.mode = paragraph.
 ***************************************************************************/
HtmlTag HtmlGenerator::getModeTag( const HtmlContext & context )
{
  Boolean checkCompact = false;
  Boolean checkTable = false;
  IString tag;
  Boolean isBlock = true;

  //
  // GET TAG NAME
  //
  switch ( context.mode )
  {
  case singleList:
    switch ( context.list )
    {
    case listSimple:
      tag += "ul";
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
    }
    checkCompact = true;
    break;

  case lines:
    tag += "pre";
    // <pre> cannot contain other block tags, so treat it like an inline
    isBlock = false;
    break;

  case quotation:
    tag += "blockquote";
    break;

  case doubleList:
    tag += "dl";
    checkCompact = true;
    break;

  case table:
    tag += "table";
    checkTable = true;
    break;

  default:
    BADSWITCH;
    break;
  }

  //
  // GET ATTRIBUTES
  //
  if ( checkCompact && context.spacingLines < 1 )
  {
    tag += " compact";
  }

  if ( checkTable )
  {
    // RULES
    if (context.hasBorder)
    {
      tag += " border";
    }
  }

  return HtmlTag(tag, isBlock, breakBefore | breakAfter);
}


/***************************************************************************
 * Procedure.. modeAttributesDiffer
 *
 * Returns true if the tag attributes for the two modes are different.
 * Assumes the two modes are the same.
 ***************************************************************************/
Boolean HtmlGenerator::modeAttributesDiffer(
    const HtmlContext & context1,
    const HtmlContext & context2 )
{
  IASSERTSTATE( context1.mode == context2.mode );
  if ( context1.mode == paragraph )
  {
    return false;  // no attributes for paragraph
  }
  return ( getModeTag( context1 ) != getModeTag( context2 ) );
}


