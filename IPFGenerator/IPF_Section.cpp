/***************************************************************************
 * File...... Section.cpp
 * Author.... Mat
 * Date...... 10/20/95
 *
 * Procedures for handling sections, labels and window definitions for IPF
 * (IPF headings).
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// IPFGenerator
#include "IPF_Common.hpp"
#include "IpfForwarderSequence.hpp"


static const char untitled[] = "Untitled";
static const int  maxLevel = 6;


//
// TEMPLATE DEFINITIONS
//
#include <ilnseq.c>
#include <iqueseq.c>

// LabelList
#pragma define(IGLnSqCrs<LabelGin,IStdOps<LabelGin> >)
#pragma define(IGLinkedSequenceNode<LabelGin,IStdOps<LabelGin> >)
#pragma define(IGLnSq<LabelGin,IStdOps<LabelGin> >)
#pragma define(IWQueueOnSeqCursor<LabelGin,IStdOps<LabelGin>,IGLnSq<LabelGin,IStdOps<LabelGin> > >)
#pragma define(IWQuOnSq<LabelGin,IStdOps<LabelGin>,IGLnSq<LabelGin,IStdOps<LabelGin> > >)



/***************************************************************************
 * Procedure.. LabelList::LabelList
 * Author..... Mat
 * Date....... 3/12/96
 *
 * Constructor for label list (queue of LabelGins)
 ***************************************************************************/
LabelList::LabelList( INumber numberOfElements ):
    IQueue< LabelGin >( numberOfElements )
{}


/***************************************************************************
 * Procedure.. AttributesFromLabel
 * Author..... Mat
 * Date....... 10/21/95
 *
 * Given a LabelGin, this procedure determines the correct IPF attributes
 * for the heading tag.  This includes resource ID (res), alpha id (id),
 * and an optional "global" tag.  The returned string is prepended with a
 * blank.
 ***************************************************************************/
static IString AttributesFromLabel( const LabelGin & label )
{
  IString attr =
      " res=" + IString( label.id() ) +
      " id='" + label.name() + "'";
  if ( label.isExternal() )
    attr += " global";
  return attr;
}


/***************************************************************************
 * Procedure.. IPFGenerator::tag_h
 * Author..... Mat
 * Date....... 10/20/95
 *
 * Sends a heading tag to the destination, using the current section and
 * label information.
 ***************************************************************************/
void IPFGenerator::tag_h()
{
  // create dummy label if necessary
  if ( _mode == waitingForLabel ) {
    int sectionId = nextId();
    _label = LabelGin( IString( sectionId ) );
    _label.setId( sectionId );
    _mode = updatingHeading;
  } /* endif */
  IASSERTSTATE( _mode == updatingHeading );

  // build tag output
  IString tag = "h" + IString( _section.level() ) + AttributesFromLabel( _label );
  if ( ! _section.isListed() ) {
    tag += " hide";
  } /* endif */

  // add window attributes
  if ( _section.windowId() ) {
    const WindowDef & window = windowWithId( _section.windowId() );
    // window size, origin, id
    tag += " x=" + convertPosition( window.originX(), true )
         + " y=" + convertPosition( window.originY(), false )
         + " width=" + convertDistance( window.width() )
         + " height=" + convertDistance( window.height() )
         + " group=" + IString( window.id() );

    // titlebar
    Boolean hasMinMax = window.hasMinimize() || window.hasMaximize();
    tag += " titlebar=";
    tag +=
        (hasMinMax && window.hasSystemMenu())? "both":
        hasMinMax?                             "minmax":
        window.hasSystemMenu()?                "sysmenu":
        window.hasTitleBar()?                  "yes":
                                               "none";

    // border
    // default border type is sizeable
    if ( window.border() != WindowDef::sizeBorder ) {
      tag += " rules=";
      tag +=
          ( window.border() == WindowDef::staticBorder )? "border": "none";
    } /* endif */

    // scroll control
    // default is both horizontal and vertical
    if ( window.scrolling() != WindowDef::allScroll ) {
      tag += " scroll=";
      switch ( window.scrolling() ) {
      case WindowDef::noScroll:
        tag += "none";
        break;
      case WindowDef::vertScroll:
        tag += "vertical";
        break;
      case WindowDef::horizScroll:
        tag += "horizontal";
        break;
      } /* endswitch */
    } /* endif */

    // use window title if no section title
    if ( ! _section.title().length() )
      _section.setTitle( translate( window.title(), true ) );
  }

  // send the tag
  sendTag( tag, breakBefore | noWrap | breakAfter | isNotContent );

  // output title text
  if ( ! _section.title().length() )
    _section.setTitle( untitled );
  sendText( _section.title(), noWrap | breakAfter | isNotContent | noSymbolCheck );
  _mode = ready;
  _emptySection = true;
}


/***************************************************************************
 * Procedure.. IPFGenerator::operator<<( const SectionGin & )
 * Author..... Mat
 * Date....... 10/20/95
 *
 * Accepts a SectionGin, which sets the title, level and window info for
 * the current section.  Output of the :h1. tag is deferred until we
 * have a label.  If we need to produce output before we get a LabelGin,
 * then we create a dummy label (see sendRawText).
 ***************************************************************************/
void IPFGenerator::handleSection( const SectionGin & section )
{
  IASSERTSTATE( section.level() > 0 );
  IASSERTSTATE( ! section.windowId() || hasWindowWithId( section.windowId() ) );

  wrapUpSection();
  _mode = waitingForLabel;

  // copy section info
  // IPF doesn't support section level > 6
  _section = SectionGin( section.level() > maxLevel? maxLevel: section.level(), section.isListed() );
  _section.setTitle( translate( section.title(), true ) );
  _section.setWindow( section.windowId() );
}



/***************************************************************************
 * Procedure.. IPFGenerator::operator<<( const LabelGin & )
 * Author..... Mat
 * Date....... 10/20/95
 *
 * Accepts a LabelGin, which gives the ID information for the current
 * heading.  If more than one Label is given for a Section, we store the
 * additional labels in the _moreLabels list -- wrapUpSection() will
 * handle them.
 ***************************************************************************/
void IPFGenerator::handleLabel( const LabelGin        & label)
{
  IASSERTSTATE( _mode > start );

  // assign an ID if none assigned
  LabelGin labelWithId = label;
  if ( ! labelWithId.id() ) {
    labelWithId.setId( nextId() );
  } /* endif */

  if ( _mode == waitingForLabel ) {
    // save label information
    _label = labelWithId;
    // output the tag
    _mode = updatingHeading;
    tag_h();
  } else {
    // heading tag was already output, add this label to the "extras" list
    _moreLabels->enqueue( labelWithId );
  } /* endif */
}



/***************************************************************************
 * Procedure.. IPFGenerator::wrapUpSection()
 * Author..... Mat
 * Date....... 10/20/95
 *
 * This procedure will take care of unfinished output for the current
 * section.  In order to support multiple IDs, we use an IPF trick.  We
 * create "dummy" headings with autolinks back to the main heading.
 *
 * On exit, the current buffered _line length is always 0.
 ***************************************************************************/
IPFGenerator & IPFGenerator::wrapUpSection()
{
  IASSERTSTATE( _mode != updatingContext );

  // finish the section
  if ( _mode > start )
  {
    // return to default context
    switchContext( IPFContext() );

    // avoid an ampty section so that IPFC doesn't complain
    if ( _emptySection )
      sendTag( "p", breakBefore | isFormat );

    // flush the current line
    outputLine( false );

    // register forwarders for additional labels
    INumber n = _moreLabels->numberOfElements();
    if ( n )
    {
      INumber i;

      sendControl( IString("* setup dependencies with auto-forwarders to support multiple labels per section"), noModeCheck | isNotContent );

      // setup "auto dependent" links to all dummy panels to set up dependencies
      // also, add to forwarder list
      for (i = 1; i <= n; i++)
      {
        const LabelGin & label = _moreLabels->elementAtPosition( i );
        sendTag(
            "link auto dependent reftype=hd res=" +
            IString( label.id() ),
            breakBefore | noWrap | breakAfter );
        _forwarders->add( IpfForwarder( label, _label.id() ) );
      }

      // lastly in this panel, setup an auto-link to self
      // so that this panel gets the final focus
      sendTag(
          "link auto reftype=hd res=" + IString( _label.id() ),
          breakBefore | noWrap | breakAfter );

      // remove all labels from the queue
      _moreLabels->removeAll();
    }

    // reset mode & label
    _mode = start;
    _label = LabelGin( IString() );
  }

  return *this;
}


/***************************************************************************
 * Procedure.. IPFGenerator::sendForwarders
 * Author..... Mat
 * Date....... 6/4/99
 *
 * Send a bogus "forwarder" section for each extra label per section.
 ***************************************************************************/
void IPFGenerator::sendForwarders()
{
  unsigned i;
  unsigned n = _forwarders->numberOfElements();

  // skip this if no forwarders have been saved
  if ( ! n )
    return;

  // send descriptive comment text
  sendControl( IString("* auto-forwarders to support multiple labels per section"), noModeCheck | isNotContent );

  // send a bogus hidden section for each forwarder
  for (i = 1; i <= n; i++)
  {
    const IpfForwarder & forwarder = _forwarders->elementAtPosition( i );
    // send heading tag
    sendTag(
        "h1" +
        AttributesFromLabel( forwarder.label() ) +
        " hide noprint nosearch width=0x height=0x titlebar=none scroll=none rules=none",
        noModeCheck | breakBefore | isNotContent );
    // send dummy title
    sendText( untitled, noModeCheck | noWrap | breakAfter | isNotContent | noSymbolCheck );
    // send autolink to main panel
    sendTag(
        "link auto reftype=hd res=" + IString( forwarder.targetId() ),
        noModeCheck | breakBefore | breakAfter );
  }

  // remove all forwarders
  _forwarders->removeAll();
}

