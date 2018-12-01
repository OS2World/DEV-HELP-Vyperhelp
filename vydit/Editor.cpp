/***************************************************************************
 * File...... Editor.cpp
 * Author.... Mat
 * Date...... 8/17/95
 *
 * Implementation for Editor
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros
#include <itrace.hpp>       // ITRACE macros
#include <ivport.hpp>       // IViewPort
#include <iscroll.hpp>      // IScrollBar
#include <istring.hpp>      // IString
#include <inotifev.hpp>     // INotificationEvent

// Performance Analyzer
#include <icsperf.h>

// MekTek
#include "MekTek.hpp"

// Editor
#include "FlowFormatter.hpp"
#include "Page.hpp"
#include "DrawPen.hpp"
#include "FlowItem.hpp"
#include "FlowView.hpp"
#include "LineView.hpp"
#include "FontItem.hpp"
#include "SymbolItem.hpp"
#include "ViewCursor.hpp"
#include "SmartText.hpp"
#include "BoldItem.hpp"
#include "ItalicItem.hpp"
#include "UnderlineItem.hpp"
#include "LinkItem.hpp"
#include "ParagraphItem.hpp"
#include "NewLineItem.hpp"
#include "PushItem.hpp"
#include "PopItem.hpp"
#include "PushView.hpp"
#include "PopView.hpp"
#include "ArtItem.hpp"
#include "EditorTrigger.hpp"
#include "Editor.hpp"
#include "Types.hpp"


//
// Constructor and Destructor ---------------------------------------
//

Editor::Editor( unsigned long windowId, IViewPort * viewport, FlowItem * flow )
  : ICanvas( windowId, viewport, viewport ),
    _viewport( viewport ),
    _flowItem( flow ),
    _kursor( this ),
    _isOvertype( false ),
    _isDragActive( false ),
    _current( *this, true ),
    _anchor( *this, true ),
    _anchorOrder( 0 ),
    _startFormatView( 0 ),
    _endFormatView( 0 ),
    _beginChange( *this ),
    _endChange( *this ),
    _holdFormatting( 0 ),
    _isSelectionEnabled( true ),
    _trigger( new EditorTrigger( *this ) ),
    _isModified( false ),
    _hasSelection( false ),
    _canEditProperties( false ),
    _isPaintValid( true )
{
  // set background color
  setBackgroundColor( IColor( IColor::white ) );

  // setup handlers
  IPaintHandler::handleEventsFor( this );
  IKeyboardHandler::handleEventsFor( this );
  IMouseHandler::handleEventsFor( this );
  enableNotification();

  // create a FlowView
  _flow = (FlowView *) flow->newView( NULL );
  _flow->setEditor( this );

  // initialize cursor to first leaf
  _current.set( _flow->newLeafCursor( true ) );
}


Editor::~Editor()
{
  // reset info string
  setInfo( IString() );

  // clear cursors
  _current.clear();
  _anchor.clear();
  _beginChange.clear();
  _endChange.clear();

  // delete FlowView and all subviews
  delete _flow;

  disableNotification();

  IMouseHandler::stopHandlingEventsFor( this );
  IKeyboardHandler::stopHandlingEventsFor( this );
  IPaintHandler::stopHandlingEventsFor( this );
}


//
// Version -----------------------------------------------------------------
//

#ifndef VVERSION
  #define VVERSION
#endif
IString Editor::version()
{
  return IString(VVERSION);
}


//
// Attributes --------------------------------------------------------------
//

INotificationId Editor::modifiedId   = "Editor::modified";
INotificationId Editor::infoId       = "Editor::info";
INotificationId Editor::selectionId  = "Editor::selection";
INotificationId Editor::propertiesId = "Editor::properties";


void Editor::setModified( Boolean enable )
{
  if ( _isModified != enable )
  {
    _isModified = enable;
    notifyObservers(INotificationEvent(Editor::modifiedId, *this));
  }
}


void Editor::setInfo( const IString & info )
{
  if ( _info != info )
  {
    _info = info;
    notifyObservers(INotificationEvent(Editor::infoId, *this));
  }
}


void Editor::setSelectionFlag( Boolean enable )
{
  if ( _hasSelection != enable )
  {
    _hasSelection = enable;
    notifyObservers(INotificationEvent(Editor::selectionId, *this));
  }
}


void Editor::checkForProperties()
{
  Item * item = _current.placed()->subject();
  Boolean enable = item && item->editPropertiesId();
  if ( _canEditProperties != enable )
  {
    _canEditProperties = enable;
    notifyObservers(INotificationEvent(Editor::propertiesId, *this));
  }
}


//
// Actions -----------------------------------------------------------------
//

INotificationId Editor::editFontId = "Editor::editFont";
INotificationId Editor::editLinkId = "Editor::editLink";
INotificationId Editor::editArtId = "Editor::editArt";
INotificationId Editor::exceptionId = "Editor::exception";


/***************************************************************************
 * Procedure.. Editor::editProperties
 * Author..... Mat
 * Date....... 10/5/99
 *
 * Send notification that the current item is to be edited.  The application
 * will allow the user to edit properties of the Item, based on the item
 * type.
 ***************************************************************************/
void Editor::editProperties()
{
  Item * item = _current.placed()->subject();
  if ( item )
  {
    INotificationId id = item->editPropertiesId();
    if ( id )
      notifyObservers( INotificationEvent( id, *this, true, IEventData( (void *)item) ) );
  }
}


void Editor::generate( Generator & gen )
{
  _flowItem->setGenerator( &gen );
  _flowItem->generate();
  _flowItem->finishGenerate();
}


void Editor::generateSelection( Generator & gen )
{
  EditorCursor cursor1( *this );
  EditorCursor cursor2( *this );

  if ( ! getSelection( cursor1, cursor2 ) )
    return;

  // send a dummy SectionGin
  gen << SectionGin( 1, true );

  // generate the selected area
  _flowItem->setGenerator( &gen );
  unsigned level = 0;
  _flow->generate( cursor1.leaf(), cursor2.leaf(), level );
  _flowItem->finishGenerate();
}


//
// Formatter ---------------------------------------------------------------
//

// format full FlowView
void Editor::resize()
{
  MEKASSERT( ! _holdFormatting );
  flushInput();
  format();
}


// format starting at the given line
void Editor::formatStartingAt( PlacedView & startingView )
{
  // if a line is already queued, check which line is earlier
  setFormatView( startingView );

  // if formatting is not on hold, go ahead and format
  if ( ! _holdFormatting )
    checkForFormat();
}


// the given view is compared with the current format view, and if it
// appears earlier, then the format view is updated.  Setting view
// of formatView to NULL indicates first View.  _startFormatView and
// _enadFormatView are 0 to indicate that no formatting is needed
void Editor::setFormatView( PlacedView & view )
{
  if ( ! _startFormatView || _flow->orderViews( *_startFormatView, view ) > 0 )
    _startFormatView = &view;
  if ( ! _endFormatView || _flow->orderViews( *_endFormatView, view ) < 0 )
    _endFormatView = &view;
}


void Editor::clearFormatView()
{
  _startFormatView = _endFormatView = 0;
}


/***************************************************************************
 * Procedure.. Editor::changeFormatView
 * Author..... Mat
 * Date....... 6/22/99
 *
 * Called from a lower level when a PlacedView is being replaced by another
 * PlacedView (e.g., word merge).  This function checks to see if the
 * old placed view has been stored as a format view, and if so, updates
 * the reference.
 ***************************************************************************/
void Editor::changeFormatView( PlacedView * viewOld, PlacedView * viewNew )
{
  IASSERTPARM( viewOld && viewNew );
  if ( _startFormatView == viewOld )
    _startFormatView = viewNew;
  if ( _endFormatView == viewOld )
    _endFormatView = viewNew;
}


void Editor::checkForFormat()
{
  // release formatting hold (if it is currently held)
  if ( _holdFormatting )
  {
    // decrement count, don't format if count is still > 0
    if ( --_holdFormatting )
      return;
  }

  // if any request has been queued, then do it!
  if ( _startFormatView || _endFormatView )
    format( _startFormatView, _endFormatView );
  else
    checkForRedraw();
}


// format the flow using a starting view (if NULL format entire flow)
// keep cursor integrity by avoiding transient views during format
// resize view window (if necessary) and repaint the invalid area
void Editor::format( PlacedView * startingView, PlacedView * endingView )
{
  // skip transient views
  _current.skip();
  _anchor.skip();
  _beginChange.skip();
  _endChange.skip();

  // determine visible width
  Coord visibleWidth = _viewport->size().width()
                     - _viewport->verticalScrollBar()->size().width();

  // do the formatting
  FlowFormatter formatter( *_flow, _page );
  IRectangle repaintZone = formatter.format( startingView, endingView, visibleWidth );

  // return to the skipped transient view, if possible
  _current.unskip();
  _anchor.unskip();
  _beginChange.unskip();
  _endChange.unskip();

  // clear cached format
  clearFormatView();

  // size to match the FlowView
  ISize newSize( _flow->width(), _flow->extent() );
  if ( newSize != size() )
  {
    sizeTo( newSize );
    _viewport->setViewWindowSize( newSize );
  }

  // repaint the "zone"
  redraw( repaintZone );
}


// do all necessary formatting BEFORE any movement, because it may affect
//   how the movement is performed.
void Editor::prepareForMove( Boolean wantAnchor )
{
  _holdFormatting++;
  try
  {
    flushInput();
    dropAnchor( wantAnchor );
  }
  catch ( IException & x )
  {
    _holdFormatting--;
    IRETHROW( x );
  }
  checkForFormat();
}


void Editor::checkForRedraw()
{
  // if selection has changed, redraw
  if ( _beginChange.isSet() )
  {
    // pass an empty rect to redraw() to force selected area update
    IRectangle rect;
    redraw( rect );
  }
}


// forces a refresh of the indicated area
void Editor::redraw( IRectangle & rect )
{
  ITRACE_DEVELOP( IString("Redraw ") + rect.asString() );

  // update selected region
  showSelection( rect );

  // refresh the screen
  refresh( rect );

  // position the keyboard cursor
  positionKursor( true );
}


/***************************************************************************
 * Procedure.. Editor::showSelection
 * Author..... Mat
 * Date....... 8/28/97
 *
 * 1. set the pages selection region.
 * 2. update the refresh area if a selection change has occurred.
 ***************************************************************************/
void Editor::showSelection( IRectangle & rect )
{
  // set page's selection region
  _page.clearSelection();
  if ( _anchorOrder != 0 )
  {
    MEKASSERT( _anchor.isSet() );
    View * view1 = ( _anchorOrder > 0 )? _current.leaf(): _anchor.leaf();
    View * view2 = ( _anchorOrder > 0 )? _anchor.leaf(): _current.leaf();
    MEKASSERT( view1 && view2 );
    IRectangle rect1 = view1->fullRect();
    IRectangle rect2 = view2->fullRect();

    // add rectangles to the selected region
    // NOTE: exclude x and y extremes (outside border) from rectangle
    if ( rect1.minY() == rect2.minY() )
    {
      // same line
      _page.addToSelection( IRectangle( rect1.minX(), rect1.minY(), rect2.minX()-1, rect2.maxY()-1 ) );
    }
    else
    {
      // multiple lines (extend to edge of page)
      _page.addToSelection( IRectangle( rect1.minX(), rect1.minY(), _flow->width()-1, rect1.maxY()-1 ) );
      if ( rect1.minY() > rect2.maxY() )
        _page.addToSelection( IRectangle( 0, rect2.maxY(), _flow->width()-1, rect1.minY()-1 ) );
      if ( rect2.minX() > 0 )
        _page.addToSelection( IRectangle( 0, rect2.minY(), rect2.minX()-1, rect2.maxY()-1 ) );
    }
  }

  // update redraw rectangle with changed area if applicable
  if ( _beginChange.isSet() )
  {
    // get begin and end view
    MEKASSERT( _endChange.isSet() );
    View * view1 = _beginChange.leaf();
    View * view2 = _endChange.leaf();
    MEKASSERT( view1 && view2 );

    // get rectangle for begin and end view
    IRectangle rect1 = view1->fullRect();
    IRectangle rect2 = view2->fullRect();
    IRectangle changeRect;
    if ( rect1.minY() == rect2.minY() )
    {
      // same line
      changeRect = IRectangle( rect1.minXMinY(), rect2.minXMaxY() );
    }
    else
    {
      // multiple lines (extend to edge of page)
      changeRect = IRectangle( 0, rect2.minY(), _flow->width(), rect1.maxY() );
    }
    if ( rect.area() )
      rect |= changeRect;
    else
      rect = changeRect;
    ITRACE_DEVELOP( IString("Redraw selection ") + rect.asString() );

    // remove begin/end markers
    resetSelection();
  }
}



//
// Paint Handler -----------------------------------------------------------
//

/***************************************************************************
 * Procedure.. Editor::paintWindow()
 *
 * Called as part of IPaintHandler responsibilities.  Copies invalid region
 * from the memory bitmap which was drawn during formatting.
 ***************************************************************************/
Boolean Editor::paintWindow( IPaintEvent &event )
{
  // if paint is not valid, just clear the background
  if ( ! _isPaintValid )
  {
    ITRACE_ALL("Paint failed");
    event.clearBackground( backgroundColor() );
    return 1;
  }

  try
  {
    // determine if part of region needs to be drawn
    IRectangle zone = event.rect() & _viewport->viewWindowDrawRectangle();
    _page.sizeToInvalid( zone );
    if ( zone.area() )
    {
      // draw the zone and validate
      DrawPen pen( _page );
      _flow->draw( pen, zone );
      _page.validate( zone );
    }

    // hide the keyboard cursor
    _kursor.show( false );

    // paint the region
    _page.paint( event );

    // restore the keyboard cursor
    _kursor.show( true );
  }
  catch( IException & x )
  {
    x.logExceptionData();
    _isPaintValid = false;
    notifyObservers( INotificationEvent( Editor::exceptionId, *this, true, IEventData( (void *)&x ) ) );
    _isPaintValid = true;
  }

  return 1;
}




//
// Keyboard Handler --------------------------------------------------------
//

/***************************************************************************
 * Procedure.. Editor::virtualKeyPress()
 *
 * Handle non-text keystrokes.  This is only called on key-down transitions.
 ***************************************************************************/
Boolean Editor::virtualKeyPress( IKeyboardEvent &event )
{
  Boolean result = false;

  try
  {
    IKeyboardEvent::VirtualKey virtualKey = event.virtualKey();
    switch ( virtualKey ) {
    case IKeyboardEvent::space:
      {
        result = true;
        insertText( " ", true );
        break;
      }
    case IKeyboardEvent::enter:
    case IKeyboardEvent::newLine:
      {
        result = true;
        insertText( "\r", true );
        break;
        // to workaround compiler bug, moved Shift+Enter handling to key()
      }
    case IKeyboardEvent::insert:
      {
        if ( ! event.isShiftDown() )
        {
          // Ins = toggle insert mode
          result = true;
          _isOvertype = ! _isOvertype;
          _kursor.setType( _isOvertype? Kursor::overtype: Kursor::insert ).update();
        }
        break;
      }
    case IKeyboardEvent::deleteKey:
    case IKeyboardEvent::backSpace:
      {
        result = true;
        deleteItems( virtualKey == IKeyboardEvent::deleteKey );
        break;
      }
    case IKeyboardEvent::up:
    case IKeyboardEvent::down:
      {
        result = true;
        prepareForMove( event.isShiftDown() );
        if ( event.isCtrlDown() )
          moveKursorByGroup( virtualKey == IKeyboardEvent::down );
        else
          moveKursorUpDown( virtualKey == IKeyboardEvent::up );
        checkForRedraw();
        break;
      }
    case IKeyboardEvent::right:
    case IKeyboardEvent::left:
      {
        result = true;
        prepareForMove( event.isShiftDown() );
        moveKursorRightLeft( virtualKey == IKeyboardEvent::right, event.isCtrlDown() );
        checkForRedraw();
        break;
      }
    case IKeyboardEvent::home:
    case IKeyboardEvent::end:
      {
        result = true;
        prepareForMove( event.isShiftDown() );
        if ( event.isCtrlDown() )
          moveKursorTopBottom( virtualKey == IKeyboardEvent::home );
        else
          moveKursorHomeEnd( virtualKey == IKeyboardEvent::home );
        checkForRedraw();
        break;
      }
    } /* endswitch */
  }
  catch( IException & x )
  {
    x.logExceptionData();
    notifyObservers( INotificationEvent( Editor::exceptionId, *this, true, IEventData( (void *)&x ) ) );
  }

  return result;
}

// workaround for OpenClass bug, defect #21395/talcm
// virtualKeyPress is not called for Shift+Enter combination
Boolean Editor::key(IKeyboardEvent& event)
{
  Boolean result = false;

  try
  {
    if ( ! event.isUpTransition() && event.isVirtual() && event.isShiftDown() )
      if ( event.virtualKey() == IKeyboardEvent::enter ||
           event.virtualKey() == IKeyboardEvent::newLine)
      {
        result = true;
        insertText( "\n", true );
      }
  }
  catch( IException & x )
  {
    x.logExceptionData();
    notifyObservers( INotificationEvent( Editor::exceptionId, *this, true, IEventData( (void *)&x ) ) );
  }

  return result;
}


/***************************************************************************
 * Procedure.. Editor::characterKeyPress()
 *
 * Handle text keystrokes.  This is only called for key-down transitions.
 ***************************************************************************/
Boolean Editor::characterKeyPress( IKeyboardEvent &event )
{
  Boolean result = false;

  try
  {
    if ( ! ( event.isCtrlDown() || event.isAltDown() ) )
    {
      result = true;
      insertText( event.mixedCharacter(), true );
    }
  }
  catch( IException & x )
  {
    x.logExceptionData();
    notifyObservers( INotificationEvent( Editor::exceptionId, *this, true, IEventData( (void *)&x ) ) );
  }

  return result;
}


//
// Mouse Handler ----------------------------------------------------
//

/***************************************************************************
 * Procedure.. Editor::mouseClicked()
 *
 * Handle a mouse click.  Invoke newCorrelateCursor() recursively to find the
 * item view nearest to the click point.
 ***************************************************************************/
Boolean Editor::mouseClicked( IMouseClickEvent &event )
{
  try
  {
    setFocus();

    // only handle button 1 clicks
    if ( event.mouseButton() == IMouseClickEvent::button1 )
      switch ( event.mouseAction() )
      {
        case IMouseClickEvent::click:
          handleMouseMove( event, event.isShiftKeyDown() );
          _isDragActive = false;
          break;
        case IMouseClickEvent::doubleClick:
          handleDoubleClick();
          _isDragActive = false;
          break;
        case IMouseClickEvent::down:
          handleMouseMove( event, event.isShiftKeyDown() );
          _isDragActive = true;
          capturePointer( true );
          break;
        case IMouseClickEvent::up:
          handleMouseMove( event, true );
          _isDragActive = false;
          capturePointer( false );
          break;
      }
 }
  catch( IException & x )
  {
    x.logExceptionData();
    notifyObservers( INotificationEvent( Editor::exceptionId, *this, true, IEventData( (void *)&x ) ) );
  }

  return false;
}


Boolean Editor::mouseMoved( IMouseEvent & event )
{
  try
  {
    if ( _isDragActive )
      handleMouseMove( event, true );
  }
  catch( IException & x )
  {
    x.logExceptionData();
    notifyObservers( INotificationEvent( Editor::exceptionId, *this, true, IEventData( (void *)&x ) ) );
  }

  // return false for default handling
  return false;
}


void Editor::handleMouseMove( IMouseEvent & event, Boolean wantAnchor )
{
  IPoint position = event.mousePosition();

  if ( position != _mousePoint )
  {
    // mouse has moved
    prepareForMove( wantAnchor );
    _kursorPoint = _mousePoint = position;
    adjustKursor();
    checkForRedraw();
  }
}


void Editor::handleDoubleClick()
{
  // extend selection to include word
  if ( _current.isWord() )
  {
    ViewCursor * wordCursor;
    Boolean ok;

    _holdFormatting++;
    try
    {
      // select only this word if no current selection, or if anchor in same word
      if ( ! _anchor.isSet() || _current.placed() == _anchor.placed() )
      {
        // set current to beginning of word
        wordCursor = _current.placed()->newCursor();
        MEKASSERT( wordCursor );
        ok = wordCursor->setToFirst();
        MEKASSERT( ok );
        _current.set( wordCursor );
        // set anchor to end of word
        wordCursor = _current.placed()->newCursor();
        MEKASSERT( wordCursor );
        ok = wordCursor->setToLast();
        MEKASSERT( ok );
        _anchor.set( wordCursor );
        // advance to next leaf after the word
        ok = _anchor.move( true );
        MEKASSERT( ok );
        // refresh entire word
        changeSelection( *_anchor.cursor(), *_current.cursor() );
      }

      // otherwise, extend selection through this word
      else
      {
        // go to start or end or word, whichever extends selection
        wordCursor = _current.placed()->newCursor();
        MEKASSERT( wordCursor );
        if ( _anchorOrder < 0 )
          ok = wordCursor->setToLast();
        else
          ok = wordCursor->setToFirst();
        MEKASSERT( ok );
        _current.set( wordCursor );
        // advance to next leaf after the word
        if ( _anchorOrder < 0 )
        {
          ok = _current.move( true );
          MEKASSERT( ok );
        }
      }
    }
    catch ( IException & x )
    {
      _holdFormatting--;
      IRETHROW( x );
    }

    checkForFormat();
    positionKursor( true );
  }
}


/***************************************************************************
 * Procedure.. Editor::mousePointerChange()
 *
 * Sets the mouse pointer to the text insertion bar while over this window.
 ***************************************************************************/
Boolean Editor::mousePointerChange( IMousePointerEvent & event )
{
  event.setMousePointer( ISystemPointerHandle( ISystemPointerHandle::text ) );
  return true;
}



//
// Kursor Movement --------------------------------------------------
//


/***************************************************************************
 * Procedure.. Editor::moveKursorTopBottom
 * Author..... Mat
 * Date....... 9/3/96
 *
 * Respond to the Ctrl+Home/End keys by moving the kursor to the very top
 * or bottom of the flow (i.e. the very first or very last leaf view)
 ***************************************************************************/
void Editor::moveKursorTopBottom( Boolean moveTop )
{
  // replace current cursor with first/last leaf
  _current.set( _flow->newLeafCursor( moveTop ) );

  // reposition the kursor
  positionKursor( true );
}


/***************************************************************************
 * Procedure.. Editor::moveKursorHomeEnd
 * Author..... Mat
 * Date....... 9/3/96
 *
 * Respond to the Home/End keys by moving the kursor to the "home" or "end"
 * position on the line.
 ***************************************************************************/
void Editor::moveKursorHomeEnd( Boolean moveHome )
{
  // get cursor to current line
  ViewCursor * lineCursor = _current.leaf()->newLineCursor();
  MEKASSERT( lineCursor );

  // replace current cursor with first/last leaf from current line
  _current.set( lineCursor->view()->newLeafCursor( moveHome ) );
  delete lineCursor;

  // reposition the kursor
  positionKursor( true );
}


/***************************************************************************
 * Procedure.. Editor::moveKursorUpDown
 * Author..... Mat
 * Date....... 8/13/96
 *
 * Respond to up/down keys.  Move kursor to line above/below if possible
 ***************************************************************************/
void Editor::moveKursorUpDown( Boolean moveUp )
{
  ViewCursor * lineCursor = _current.leaf()->newLineCursor();

  // move to next/previous line
  if ( moveUp )
    lineCursor->setToPrevious();
  else
    lineCursor->setToNext();

  // if move was successful, correlate and reposition view
  if ( lineCursor->isValid() )
  {
    // keep x-value unchanged
    _kursorPoint.setY( lineCursor->view()->position().y() );
    adjustKursor();
  }

  delete lineCursor;
}


/***************************************************************************
 * Procedure.. Editor::moveKursorRightLeft
 * Author..... Mat
 * Date....... 8/13/96
 *
 * Respond to right/left arrow keys.  Move kursor to next/previous leaf view
 * if possible.  Move by word if specified.
 ***************************************************************************/
void Editor::moveKursorRightLeft( Boolean moveRight, Boolean moveByWord )
{
  EditorCursor leafCursor( *this );
  leafCursor = _current;
  Boolean ok = leafCursor.move( moveRight );
  if ( ! ok )
    return;

  // extend the movement through the word
  if ( moveByWord )
  {
    // determine initial state (depending on direction)
    enum { findSpace, findWord, done } state;
    if ( moveRight )
    {
      // if started on a word, then find space first
      state = _current.isWord()? findSpace: findWord;
    }
    else
      state = findWord;

    // search for the next word
    EditorCursor nextLeafCursor( *this );
    do
    {
      Boolean inWord = leafCursor.isWord();
      if ( ( state == findWord && ! inWord ) || ( state == findSpace && inWord ) )
      {
        // keep looking!
        nextLeafCursor = leafCursor;
        ok = nextLeafCursor.move( moveRight );
        if ( ok )
          leafCursor = nextLeafCursor;
        else
          state = done;  // no more Views!
      }
      else if ( state == findSpace )
      {
        state = findWord;  // start looking for a word now
      }
      else
      {
        // looking for a word and found one!
        state = done;
        // if moving left, jump to first character in the word
        if ( ! moveRight )
        {
          ViewCursor * wordCursor = leafCursor.placed()->newCursor();
          MEKASSERT( wordCursor );
          Boolean ok = wordCursor->setToFirst();
          MEKASSERT( ok );
          leafCursor.set( wordCursor );
        }
      }
    } while ( state < done );
  }

  // replace current cursor with the new leaf cursor
  _current = leafCursor;

  // reposition the kursor
  positionKursor( true );
}


/***************************************************************************
 * Procedure.. Editor::moveKursorByGroup
 * Author..... Mat
 *
 * Respond to Ctrl+up/down arrow keys.  Move kursor to previous/next group
 * indicator if possible.
 ***************************************************************************/
void Editor::moveKursorByGroup( Boolean moveRight )
{
  EditorCursor leafCursor( *this );
  leafCursor = _current;
  Boolean ok = leafCursor.move( moveRight );
  if ( ! ok )
    return;

  // search for the next word
  EditorCursor nextLeafCursor( *this );
  Boolean done = false;
  Boolean almostDone = false;
  do
  {
    if ( moveRight )
      almostDone = leafCursor.isPop();
    else
      done = leafCursor.isPush();
    if ( ! done )
    {
      // keep looking!
      nextLeafCursor = leafCursor;
      done = ! nextLeafCursor.move( moveRight );
      if ( ! done )
      {
        leafCursor = nextLeafCursor;
        done = almostDone;
      }
    }
  } while ( ! done );

  // replace current cursor with the new leaf cursor
  _current = leafCursor;

  // reposition the kursor
  positionKursor( true );
}


/***************************************************************************
 * Procedure.. Editor::adjustKursor()
 *
 * Clips the current _kursorPoint to be in the visible area, then correlates
 * the clipped point to an item view and updates the _current view.
 ***************************************************************************/
void Editor::adjustKursor()
{
  IRectangle visible = _viewport->viewWindowDrawRectangle();
  IPoint point = _kursorPoint;

  // adjust the point if necessary
  if ( ! visible.contains( point ) ) {
    // adjust x-value
    if ( point.x() > visible.right() ) {
      point.setX( visible.right() );
    } else if ( point.x() < visible.left() ) {
      point.setX( visible.left() );
    } /* endif */
  } /* endif */

  // find the nearest item view & replace the _current cursor
  ViewCursor * cursor = _flow->newCorrelateCursor( point );
  IASSERTSTATE( cursor );
  _current.set( cursor );

  // reposition the kursor
  positionKursor( false );
}


/***************************************************************************
 * Procedure.. Editor::positionKursor()
 *
 * Position the kursor to the "current" item/char.
 * The width is set to the current character, the height is set to the
 * current line height.
 *
 * if setKursorPoint = true, then the kursor is reset to
 * the _current leaf view.
 ***************************************************************************/
void Editor::positionKursor( Boolean setKursorPoint )
{
  View * view = _current.leaf();
  IPoint viewPosition = view->fullPosition();

  // set cursor size (height = line height)
  ViewCursor * lineCursor = view->newLineCursor();
  MEKASSERT( lineCursor );
  _kursor.sizeTo( ISize( view->width(), lineCursor->view()->extent() ) );
  delete lineCursor;

  // set kursor position
  _kursor.moveTo( viewPosition );

  // update the kursor
  _kursor.update();

  // save the new kursor point
  if ( setKursorPoint )
    _kursorPoint = viewPosition;

  scrollToFit( *_current.cursor() );

  // update attributes for cursored item
  setInfo( _current.placed()->info() );
  checkForProperties();
}


/***************************************************************************
 * Procedure.. Editor::scrollToFit()
 *
 * Scroll the viewport so that the specified item is fully visible.  If
 * the item is larger then the viewable area, just make sure that it is
 * at least partially visible.
 ***************************************************************************/
void Editor::scrollToFit( ViewCursor & viewCursor )
{
  View * view = viewCursor.view();
  Coord itemWidth = view->width();
  Coord itemLeft = view->fullPosition().x();
  Coord itemRight = itemLeft + itemWidth;

  // scroll the viewport horizintally as necessary
  IRectangle visible = _viewport->viewWindowDrawRectangle();
  if ( itemWidth > visible.width() ) {
    // item is wider than viewport!
    if ( itemRight < visible.minX() || itemLeft > visible.maxX() ) {
      // no portion is visible, fit as much as you can
      _viewport->scrollViewHorizontallyTo( itemLeft );
    } /* endif */
    // otherwise don't scroll -- item is already partially visible
  } else if ( itemLeft < visible.minX() ) {
    // shift view left to fit the character
    _viewport->scrollViewHorizontallyTo( itemLeft );
  } else if ( itemRight > visible.maxX() ) {
    // shift view right to fit the character
    _viewport->scrollViewHorizontallyTo( itemRight - visible.width() );
  } /* endif */

  // scroll the viewport vertically as necessary
  ViewCursor * lineCursor = view->newLineCursor();
  LineView * line = (LineView *) lineCursor->view();
  scrollToFitLine( line );
  delete lineCursor;
}


/***************************************************************************
 * Procedure.. Editor::scrollToFitLine
 *
 * Given a pointer to a LineView in the view hierarchy, scroll the viewport
 * to make sure that the line is fully visible.  If the line is taller than
 * the viewport, make sure at least some of it is visible.
 ***************************************************************************/
void Editor::scrollToFitLine( const LineView * lineView )
{
  Coord lineBottom = lineView->fullPosition().y();
  Coord lineTop = lineBottom + lineView->extent();
  Coord height = size().height();

  // scroll the viewport as necessary
  IRectangle visible = _viewport->viewWindowDrawRectangle();
  if ( lineView->extent() > visible.height() ) {
    // item is taller than viewport!
    if ( lineBottom > visible.maxY() || lineTop < visible.minY() ) {
      // no portion is visible, fit as much as you can
      _viewport->scrollViewVerticallyTo( height - lineBottom - visible.height() );
    } /* endif */
    // otherwise don't scroll -- item is already partially visible
  } else if ( lineBottom < visible.minY() ) {
    // shift view up to fit entire line
    _viewport->scrollViewVerticallyTo( height - lineBottom - visible.height() );
  } else if ( lineTop > visible.maxY() ) {
    // shift view down to fit entire line
    _viewport->scrollViewVerticallyTo( height - lineTop );
  } /* endif */
}


//
// Cursor Support ------------------------------------------------------------
//

/***************************************************************************
 * Procedure.. Editor::dropAnchor
 * Author..... Mat
 * Date....... 11/5/96
 *
 * Drop or lift a selection anchor. (1) If there is no anchor and wantAnchor
 * is set, then an anchor is set at the current position -- it is assumed
 * that the current position will be moved and a leaf anchor will be set at
 * that position, too. (2) if there is an an anchor and wantAnchor is false,
 * then the anchors will be removed.
 *
 * Caller should call checkForFormat to see that format is called if necess.
 ***************************************************************************/
void Editor::dropAnchor( Boolean wantAnchor )
{
  if ( wantAnchor && ! _anchor.isSet() )
  {
    // set the anchor at the current cursor position
    _anchor = _current;
    MEKASSERT( _anchor.isSet() );
  }
  else if ( ! wantAnchor && _anchor.isSet() )
  {
    // remove anchors
    // mark change in selection
    changeSelection( *_anchor.cursor(), *_current.cursor() );
    // clear anchor
    _anchorOrder = 0;
    _anchor.clear();
    setSelectionFlag( false );
  }
}


void Editor::changeSelection( ViewCursor & cursor1, ViewCursor & cursor2 )
{
  // return if selection is disabled
  if ( ! _isSelectionEnabled || ! _anchor.isSet() )
    return;

  // get first view
  View * view1 = cursor1.view();
  MEKASSERT( view1 );

  // get second view
  View * view2 = cursor2.view();
  MEKASSERT( view2 );

  // sort the views
  if ( _flow->orderViews( *view1, *view2 ) > 0 )
  {
    View * swap = view1;
    view1 = view2;
    view2 = swap;
  }
  ITRACE_RUNTIME( IString("Select ") + view1->dumpString() + IString(" to ") + view2->dumpString() );

  // get begin and end views, or 0 if not defined
  View * begin = _beginChange.leaf();
  View * end = _endChange.leaf();

  // test for new begin or end view
  if ( ! begin || _flow->orderViews( *view1, *begin ) < 0 )
    _beginChange.set( view1->newCursorToThis() );
  if ( ! end || _flow->orderViews( *view2, *end ) > 0 )
    _endChange.set( view2->newCursorToThis() );
}


// set selection-state, based on ordering of the two views
void Editor::refreshSelection()
{
  if ( _anchor.isSet() && _isSelectionEnabled )
  {
    _anchorOrder = _flow->orderViews( *_anchor.leaf(), *_current.leaf() );
    setSelectionFlag( _anchorOrder != 0 );
  }
}


void Editor::resetSelection()
{
  _beginChange.clear();
  _endChange.clear();
}


// delete existing anchor
// set new anchor adjacent to current cursor
void Editor::autoSelect( Boolean moveRight )
{
  _anchor = _current;
  Boolean ok = _anchor.move( moveRight );
  if ( ! ok )
    _anchorOrder = 0;
  else if ( moveRight )
    _anchorOrder = 1;
  else
    _anchorOrder = -1;
  setSelectionFlag( ok );
}


//
// Item Manipulation ------------------------------------------------
//


void Editor::insertText( const IString & string, Boolean checkOvertype )
{
  if ( ! string.length() )
    return;

  // append to the input string
  _input += string;

  if ( checkOvertype && _isOvertype )
  {
    // overtype requires that input be processed now
    if ( _anchorOrder == 0 )
      autoSelect( true );
    flushInput();
  }
  else
  {
    // defer the input so it can be batched if possible
    if ( ! _timer.isStarted() )
      _timer.start( _trigger, 100 );
  }

  setModified();
}


void Editor::triggerAction()
{
  try
  {
    flushInput();
  }
  catch( IException & x )
  {
    notifyObservers( INotificationEvent( Editor::exceptionId, *this, true, IEventData( (void *)&x ) ) );
  }
}


void Editor::flushInput()
{
  _timer.stop();
  if ( _input.length() )
  {
    // hold all formatting
    _holdFormatting++;

    try
    {
      // overwrite selection, if present
      removeSelection();

      // skip transient views
      _current.skip();

      // get the current leaf view and parent for item
      View * leaf = _current.leaf();

      // insert all text items
      SmartText text( _input );
      unsigned n = text.numItems();
      for ( int i = 0; i < n; i++ )
      {
        Item * item = text.createItem( i, _flowItem );
        ITRACE_DEVELOP( item->dumpString() );
        leaf->insertItem( item );
      }
    }
    catch ( IException & x )
    {
      _holdFormatting--;
      IRETHROW( x );
    }

    // check if format is necessary
    checkForFormat();

    // clear the input string
    _input = IString();
  }
}


void Editor::insertBold( Boolean enable )
{
  ITRACE_RUNTIME( IString("Insert bold ") + IString(enable) );
  createGroup( new BoldItem( _flowItem, enable ) );
}


void Editor::insertUnderline( Boolean enable )
{
  ITRACE_RUNTIME( IString("Insert underline ") + IString(enable) );
  createGroup( new UnderlineItem( _flowItem, enable ) );
}


void Editor::insertItalic( Boolean enable )
{
  ITRACE_RUNTIME( IString("Insert italic ") + IString(enable) );
  createGroup( new ItalicItem( _flowItem, enable ) );
}


void Editor::insertFont( const FontInfo & info )
{
  ITRACE_RUNTIME( IString("Insert font ") + info.name() + IString(".") + IString(info.pointSize()) );
  createGroup( new FontItem( _flowItem, info ) );
}


void Editor::insertLink( const LinkGin & link )
{
  ITRACE_RUNTIME( IString("Insert link ") + link.target() + IString(" @ ") + link.file() );
  createGroup( new LinkItem( _flowItem, link ) );
}


void Editor::insertParagraph()
{
  ITRACE_RUNTIME( IString("Insert paragraph") );
  PerfStart();
  insertItem( new ParagraphItem( _flowItem ) );
  PerfStop();
}


void Editor::insertNewLine()
{
  ITRACE_RUNTIME( IString("Insert newline") );
  insertItem( new NewLineItem( _flowItem ) );
}


void Editor::insertSymbol( const Symbol & symbol )
{
  ITRACE_RUNTIME( IString("Insert symbol ") + symbol.description() );
  insertItem( new SymbolItem( symbol, _flowItem ) );
}


void Editor::insertArt( const IString & id )
{
  ITRACE_RUNTIME( IString("Insert art ") + IString(id) );
  insertItem( new ArtItem( _flowItem, id ) );
}


void Editor::defineArt( const ArtDef & art )
{
  _flowItem->addArt( art );
}


void Editor::removeArt( const IString & id )
{
  if ( _flowItem->deleteArt( id ) )
    resize();
}


void Editor::insertItem( Item * item )
{
  if ( ! item )
    return;
  ITRACE_RUNTIME( IString("Insert item ") + item->dumpString() );

  _holdFormatting++;
  try
  {
    flushInput();
    // overwrite selection, if present
    removeSelection();
    // skip transient views
    _current.skip();
    // insert the item
    _current.leaf()->insertItem( item );
  }
  catch ( IException & x )
  {
    _holdFormatting--;
    IRETHROW( x );
  }

  checkForFormat();
  setModified();
}


void Editor::deleteItems( Boolean forward )
{
  _holdFormatting++;
  try
  {
    flushInput();
    if ( _anchorOrder == 0 )
      autoSelect( forward );
    if ( _anchor.isSet() )
    {
      removeSelection();
      setModified();
    }
  }
  catch ( IException & x )
  {
    _holdFormatting--;
    IRETHROW( x );
  }

  checkForFormat();
}


// create a Push-Pop pair around the selected stuff
// if nothing is selected, create an empty Push-Pop pair
// if item is non-NULL, insert it after the Push
void Editor::createGroup( Item * item )
{
  // hold all formatting
  _holdFormatting++;

  try
  {
    // flush any pending text input
    flushInput();

    EditorCursor cursor1( *this );
    EditorCursor cursor2( *this );
    if ( ! getSelection( cursor1, cursor2, true ) )
    {
      // no selection, use current cursor as both first and last
      cursor1 = cursor2 = _current;
    }

    // move cursor1 back as necessary to avoid cross-nested groups
    EditorCursor check( *this );
    Boolean looking;
    check = cursor1;
    do
    {
      looking = false;
      if ( check.move( false ) && check.isPush() )
      {
        PushView * push = (PushView *) check.placed();
        MEKASSERT( push );
        PopView * pop = push->pop();
        if ( pop && _flow->orderPlacedViews( pop, cursor2.placed() ) < 0 )
        {
          // the previous view is a Push whose Pop is in the selected region
          cursor1 = check;
          looking = true;
        }
      }
    }
    while ( looking );

    // move cursor2 forward as necessary to avoid cross-nested groups
    check = cursor2;
    do
    {
      looking = false;
      if ( check.isPop() )
      {
        PopView * pop = (PopView *) check.placed();
        MEKASSERT( pop );
        PushView * push = pop->push();
        if ( push && _flow->orderPlacedViews( push, cursor1.placed() ) >= 0 )
        {
          // the next view is a Pop whose Push is in the selected region
          looking = true;
        }
      }
      if ( looking )
      {
        if ( check.move( true ) )
          cursor2 = check;
        else
          looking = false;
      }
    }
    while ( looking );

    // check for un-balanced group
    if ( isUnbalanced( cursor1, cursor2 ) )
      ITHROW( IException( "Cannot create cross-nested groups." ) );

    // get the leaf views
    View * leaf1 = cursor1.leaf();
    MEKASSERT( leaf1 );
    View * leaf2 = cursor2.leaf();
    MEKASSERT( leaf2 );

    // create Push/Pop pair
    PushItem * push = new PushItem( _flowItem );
    PopItem * pop = new PopItem( _flowItem, push );

    // insert the Push/Pop and the item, if provided
    leaf1->insertItem( push );
    if ( item )
    {
      leaf1->insertItem( item );
      item = 0;
    }
    leaf2->insertItem( pop );

    // unselect region (otherwise need to extend selection to include new insertions)
    dropAnchor( false );
  }
  catch ( IException & x )
  {
    delete item;
    _holdFormatting--;
    IRETHROW( x );
  }

  // check if format is necessary
  checkForFormat();

  setModified();
}


//
// Commands ---------------------------------------------------------
//


Boolean Editor::getSelection( EditorCursor & start, EditorCursor & end, Boolean includeEnd )
{
  // nothing to do if there is no selection
  if ( ! _anchor.isSet() )
    return false;

  // skip transient views
  _current.skip();
  _anchor.skip();

  // nothing to do if _anchor and _current point to same view (order=0)
  if ( ! _anchorOrder )
  {
    _current.unskip();
    _anchor.unskip();
    return false;
  }

  // determine order of cursors
  if ( _anchorOrder > 0 )
  {
    start = _current;
    end = _anchor;
  }
  else
  {
    start = _anchor;
    end = _current;
  }

  // move end cursor back one position
  if ( ! includeEnd )
  {
    Boolean ok = end.move( false );
    MEKASSERT( ok );
  }

  return true;
}


// check to see that the selection only includes entire groups (no cross-nesting)
// return true if cross-nesting is detected
Boolean Editor::isUnbalanced( EditorCursor & start, EditorCursor & end )
{
  PlacedView * endView = end.placed();
  MEKASSERT( endView );
  PlacedView * view = start.placed();
  MEKASSERT( view );
  OrderedViewCursor * cursor = _flow->newViewCursor( view );
  MEKASSERT( cursor );

  Boolean isBalanced = true;
  Boolean ok;
  while ( isBalanced && ( _flow->orderPlacedViews( view, endView ) < 0 ) )
  {
    Item * item = view->subject();
    if ( item )
    {
      // check PushView
      if ( item->asPush() )
      {
        PushView * push = (PushView *) view;
        PopView * pop = push->pop();
        if ( ! pop || _flow->orderPlacedViews( pop, endView ) >= 0 )
          isBalanced = false;
        else
        {
          // resume checking at end of this group
          ok = cursor->locate( pop );
          MEKASSERT( ok );
        }
      }
      // any PopView indicates an unbalanced selection
      else if ( item->asPop() )
        isBalanced = false;
    }
    ok = cursor->setToNext();
    MEKASSERT( ok );
    view = (PlacedView *) cursor->view();
    MEKASSERT( view );
  }

  delete cursor;
  return ! isBalanced;
}


// remove the selected region (as specified by _anchor and _current)
// views outside of (*after*) the delete region may also be deleted if
//   they are orphaned (e.g., PopViews which have their PushView deleted)
void Editor::removeSelection()
{
  EditorCursor cursor1( *this );
  EditorCursor cursor2( *this );
  Boolean      ok;

  if ( ! getSelection( cursor1, cursor2 ) )
  {
    dropAnchor( false );
    return;
  }

  // assume that _startFormatView == _endFormatView == 0 upon entry
  // in order to avoid _startFormatView and _endFormatView being set to a deleted view, this
  //   procedure will take control of the setting of them such
  //   that correct reformatting is done
  IASSERTSTATE( ! _startFormatView && ! _endFormatView );

  // reset selection and formatViews in preparation for delete
  ITRACE_RUNTIME( "Remove selection" );
  resetSelection();
  _isSelectionEnabled = false;
  clearFormatView();

  // move _current back one from the first cursor
  // after deletion is done, _current will be advanced to next valid view
  // _current will not be set if very first view is being deleted
  _current = cursor1;
  _current.move( false );
  Boolean isFormatSet = false;

  // see if in same placed view as cursor1
  // if so, that placed view should be set for formatting
  if ( _current.placed() == cursor1.placed() )
  {
    PlacedView * placed = _current.placed();
    placed->clearFormatted();
    setFormatView( *placed );
    isFormatSet = true;
  }

  // clear anchor in case it references a deleted View
  _anchor.clear();
  _anchorOrder = 0;
  setSelectionFlag( false );

  // remove the selected area (also deletes cursors)
  _flow->remove( cursor1.give(), cursor2.give() );

  // move _current forward to next valid view
  // this may  not be the same as cursor2, since it may have been orphaned and deleted
  if ( _current.isSet() )
  {
    ok = _current.move( true );
    MEKASSERT( ok );
  }
  else
  {
    // move to first leaf cursor
    _current.set( _flow->newLeafCursor( true ) );
  }
  _current.skip();

  // check if adjacent words need to be merged
  // need to pass a placedView cursor to checkForWordMerge
  OrderedViewCursor * placedCursor = _flow->newViewCursor( _current.placed() );
  MEKASSERT( placedCursor );
  if ( _flow->checkForWordMerge( placedCursor ) )
  {
    // words were merged, so reset formatViews and set flag
    clearFormatView();
    isFormatSet = false;
  }
  delete placedCursor;

  // set format view to current if it isn't set yet
  if ( ! isFormatSet )
  {
    PlacedView * placed = _current.placed();
    placed->clearFormatted();
    setFormatView( *placed );
  }

  // re-enable selection
  _isSelectionEnabled = true;
}


//
// OpenClass Extensions ---------------------------------------------
//

/***************************************************************************
 * Procedure.. Editor::enableTabStop
 * Author..... Mat
 *
 * Bypasses ICanvas to use the IControl method.  The default ICanvas method
 * looks for children with tab stops, instead of checking its own style bit.
 ***************************************************************************/
Boolean Editor::isTabStop() const
{
  return IControl::isTabStop();
}


/***************************************************************************
 * Procedure.. Editor::enableTabStop
 * Author..... Mat
 *
 * Extends enableTabStop to make sure that the parent IViewPort also
 * gets recognized as a tabstop.  There is actually a "view window" between
 * the Editor and the IViewPort.  Initially it will be a sibling to the
 * Editor, but eventually it will become the parent.  Calling
 * IViewPort::viewWindow() will trigger the change.
 ***************************************************************************/
IControl & Editor::enableTabStop( Boolean enable )
{
  // trigger the IViewPort to setup correct window relationships
  _viewport->viewWindow();

  // set tab stop on this control
  IControl::enableTabStop( enable );

  // set tab stop on intermediate window
  ((IControl *)parent())->enableTabStop( enable );

  // set tab stop on IViewPort
  _viewport->enableTabStop( enable );

  return *this;
}



//
// Diagnostic -------------------------------------------------------
//

void Editor::dump( unsigned level )
{
  flushInput();

  // dump item tree
  dumpItemNode( _flow->subject(), 0, level );

  // dump page size
  ITRACE_DEVELOP( IString("Page ") + _page.size().asString() );

  // dump view tree
  dumpViewNode( _flow, 0, level );
}

// recursive function to dump Item tree
void Editor::dumpItemNode( Item * item, unsigned indent, unsigned level )
{
  ITRACE_ALL( IString().center( indent )
            + item->dumpString()
            + IString(" [") + IString( (unsigned)item ) + IString("]") );
  if ( ++indent <= level )
  {
    ItemCursor * itemCursor = item->newCursor();
    forCursor( *itemCursor )
    {
      dumpItemNode( itemCursor->item(), indent, level );
    }
    delete itemCursor;
  }
}

// recursive function to dump View tree
void Editor::dumpViewNode( View * view, unsigned indent, unsigned level )
{
  ITRACE_ALL( IString().center( indent )
            + view->dumpString() );
  if ( ++indent <= level )
  {
    ViewCursor * viewCursor = view->newCursor();
    forCursor( *viewCursor )
    {
      dumpViewNode( viewCursor->view(), indent, level );
    }
    delete viewCursor;
  }
}

