/***************************************************************************
 * File...... Kursor.cpp
 * Author.... Mat
 * Date...... 8/15/96
 *
 * Support for keyboard insertion cursor.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// PM
#define INCL_WINCURSORS
#define INCL_NOCOMMON
#include <os2.h>

// OpenClass
#include <iwindow.hpp>      // IWindow
#include <inotifev.hpp>     // INotificationEvent
#include <irect.hpp>        // IRectangle
#include <itrace.hpp>       // ITrace
#include <iexcept.hpp>      // exception macros

#include "Kursor.hpp"


Kursor::Kursor( IWindow * window ):
  _windowHandle( window->handle() ),
  _kursorType( insert ),
  _hasFocus( false ),
  _isCreated( false ),
  _attributeChanged( false )
{
  handleNotificationsFor( *window );
}


Kursor::~Kursor()
{
  if ( _isCreated )
    destroy();
}


Kursor & Kursor::sizeTo( const ISize & size )
{
  if ( cursorSize( size ) != cursorSize( _rect.size() ) )
    _attributeChanged = true;
  _rect.sizeTo( size );
  return *this;
}


Kursor & Kursor::moveTo( const IPoint & point )
{
  _rect.moveTo( point );
  return *this;
}


Kursor & Kursor::setType( KursorType kursorType )
{
  if ( _kursorType != kursorType )
    _attributeChanged = true;
  _kursorType = kursorType;
  return *this;
}


IObserver & Kursor::dispatchNotificationEvent( const INotificationEvent & event )
{
  if ( event.notificationId() == IWindow::focusId ) {
    Boolean hadFocus = _hasFocus;
    _hasFocus = event.eventData();
    if (hadFocus && ! _hasFocus && _isCreated ) {
      // if losing the focus
      destroy();
    } else if (! hadFocus && _hasFocus) {
      // if gaining the focus
      update();
    } /* endif */
  } /* endif */
  return *this;
}


Kursor & Kursor::update()
{
  if ( ! _hasFocus )
    return *this;

  ULONG   kursorStyle;
  Boolean setPositionOnly = ( _isCreated && ! _attributeChanged );

  // determine cursor style
  if ( setPositionOnly )
  {
    kursorStyle = CURSOR_SETPOS;
  }
  else
  {
    kursorStyle = CURSOR_SOLID;
    if ( _kursorType == insert)
      kursorStyle |= CURSOR_FLASH;
  }

  // create or update the cursor
  BOOL ok;
  ISize size = cursorSize( _rect.size() );
  ok = WinCreateCursor(
      _windowHandle,
      _rect.left(),
      _rect.bottom(),
      size.width(),
      size.height(),
      kursorStyle,
      (PRECTL) NULL );
  if (! ok)
    ITHROWGUIERROR( "WinCreateCursor" );
  if ( ! setPositionOnly )
  {
    ok = WinShowCursor( _windowHandle, TRUE );
    if (! ok)
      ITHROWGUIERROR( "WinShowCursor" );
  }

  // reset size change flag, and set created flag
  _attributeChanged = false;
  _isCreated = true;

  return *this;
}


ISize Kursor::cursorSize( const ISize & size ) const
{
  ISize newSize = size;
  if ( _kursorType == insert )
    newSize.setWidth( 0 );
  return newSize;
}


Kursor & Kursor::destroy()
{
  BOOL ok;

  ok = WinDestroyCursor( _windowHandle );
  if (! ok)
    ITHROWGUIERROR( "WinDestroyCursor" );

  // reset created flag
  _isCreated = false;

  return *this;
}


Kursor & Kursor::show( Boolean showKursor )
{
  if ( _isCreated )
  {
    BOOL ok = WinShowCursor( _windowHandle, showKursor );
    if (! ok)
      ITHROWGUIERROR( "WinShowCursor" );
  }
  return *this;
}

