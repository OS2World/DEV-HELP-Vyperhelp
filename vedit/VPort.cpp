/***************************************************************************
 * File...... VPort.cpp
 * Author.... Mat
 * Date...... 5/16/96
 *
 * Implementation of VPort
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// Streams
#include <fstream.h>
#include <strstrea.h>

// OpenClass
#include <itrace.hpp>       // ITRACE macros
#include <iclipbrd.hpp>     // IClipboard
#include <ifiledlg.hpp>     // IFileDialog
#include <ifont.hpp>        // IFont
#include <ifontdlg.hpp>     // IFontDialog
#include <imsgbox.hpp>      // IMessageBox
#include <inotifev.hpp>     // INotificationEvent

// Generator
#include "TraceGenerator.hpp"
#include "TextParser.hpp"
#include "ArtDef.hpp"
#include "Filename.hpp"

// Editor
#include "FlowItem.hpp"
#include "FontItem.hpp"
#include "SymbolItem.hpp"
#include "BoldItem.hpp"
#include "ItalicItem.hpp"
#include "UnderlineItem.hpp"
#include "LinkItem.hpp"
#include "ArtItem.hpp"
#include "SpaceItem.hpp"
#include "WordItem.hpp"
#include "SymbolItem.hpp"
#include "NewLineItem.hpp"
#include "FlowGenerator.hpp"

// Tester
#include "vedit.h"
#include "VPort.hpp"

// local constants
const char defaultArtId[] = "DefaultArt";


VPort::VPort( unsigned long windowId, IWindow * owner, FlowItem & flow )
  : IViewPort( windowId, owner, owner, IRectangle(),
        IWindow::visible |
        IViewPort::alwaysHorizontalScrollBar |
        IViewPort::alwaysVerticalScrollBar |
        IViewPort::noViewWindowFill ),
    _editor( WND_PAGE, this, &flow ),
    _flow( flow )
{
  // set background color
  setBackgroundColor( IGUIColor( IGUIColor::dialogBgnd ) );

  // setup handlers
  IResizeHandler::handleEventsFor( this );
  IKeyboardHandler::handleEventsFor( this );
  handleNotificationsFor( _editor );

  // define a default bitmap
  ISystemBitmapHandle handle( ISystemBitmapHandle::menuCheckMark );
  IGBitmap bitmap( handle );
  _editor.defineArt( ArtDef( defaultArtId, bitmap ) );

  // display initial help
  _editor.insertText( "F12 for help" );
}


VPort::~VPort()
{
  stopHandlingNotificationsFor( _editor );
}


IObserver & VPort::dispatchNotificationEvent( const INotificationEvent & event )
{
  if ( event.notificationId() == Editor::editFontId )
  {
    // change font to 48 pt
    FontItem * font = (FontItem *)(char *)event.eventData();
    FontInfo info;
    info.setFont( font->name(), 48 );
    font->setInfo( info );
  }
  else if ( event.notificationId() == Editor::editLinkId )
  {
    // change target to 'modified'
    LinkItem * link = (LinkItem *)(char *)event.eventData();
    LinkGin gin( "modified" );
    link->setGin( gin );
  }
  else if ( event.notificationId() == Editor::editArtId )
  {
    // change bitmap to default (id 1)
    ArtItem * art = (ArtItem *)(char *)event.eventData();
    art->setId( defaultArtId );
  }
  else if ( event.notificationId() == Editor::exceptionId )
  {
    // change target to 'modified'
    IException * exception = (IException *)(char *)event.eventData();
    IMessageBox box( this );
    box.setTitle( "Caught by Editor (internal)" );
    box.show( *exception );
  }
  return *this;
}


Boolean VPort::windowResize( IResizeEvent &event )
{
  // only need to reformat if width changed
  if ( event.newSize().width() != event.oldSize().width() )
    _editor.resize();

  return false;
}


void VPort::paste()
{
  IClipboard clipboard( this->handle() );

  if ( clipboard.hasText() )
    _editor.insertText( clipboard.text() );
}


void VPort::importFile()
{
  IFileDialog::Settings fdSettings;
  fdSettings.setOpenDialog();
  IFileDialog fileDlg( NULL, this, fdSettings );
  if ( fileDlg.pressedOK() )
  {
    // open file stream
    ifstream  ifs( fileDlg.fileName() );

    // copy to memory via string stream
    strstream ss;
    ifs >> ss.rdbuf();

    // insert entire file contents as an IString
    const IString string( ss.str() );
    _editor.insertText( string );
  }
}


void VPort::importBitmap()
{
  IFileDialog::Settings fdSettings;
  fdSettings.setOpenDialog();
  IFileDialog fileDlg( NULL, this, fdSettings );
  if ( fileDlg.pressedOK() )
  {
    Filename filename( fileDlg.fileName() );
    IGBitmap bitmap( filename );
    _editor.defineArt( ArtDef( filename.base(), bitmap ) );
    _editor.insertArt( filename.base() );
  }
}


/***************************************************************************
 * Procedure.. VPort::virtualKeyPress()
 *
 * Handle non-text keystrokes.  This is only called on key-down transitions.
 ***************************************************************************/
Boolean VPort::virtualKeyPress( IKeyboardEvent &event )
{
  try
  {
    IKeyboardEvent::VirtualKey virtualKey = event.virtualKey();
    switch ( virtualKey ) {
    case IKeyboardEvent::insert:
      if ( event.isShiftDown() )
      {
        paste();
        return true;
      }
      break;
    case IKeyboardEvent::deleteKey:
    case IKeyboardEvent::backSpace:
      {
        _editor.deleteItems( virtualKey == IKeyboardEvent::deleteKey );
        return true;
      }
    case IKeyboardEvent::f8:
      if ( event.isCtrlDown() )
      {
        TraceGenerator gen( "vedit.vyt" );
        _editor.generateSelection( gen );
        return true;
      }

      if ( event.isShiftDown() )
      {
        // insert bitmap (Shift+F8)
        importBitmap();
        return true;
      }

      // import (F8)
      importFile();
      return true;

    case IKeyboardEvent::esc:
      {
        _editor.dump( 9 );
        return true;
      }
    case IKeyboardEvent::f12:
      {
        insertHelpText();
        return true;
      }
    case IKeyboardEvent::f2:
      {
        _editor.insertBold( ! event.isShiftDown() );
        return true;
      }
    case IKeyboardEvent::f3:
      {
        _editor.insertItalic( ! event.isShiftDown() );
        return true;
      }
    case IKeyboardEvent::f4:
      {
        _editor.insertUnderline( ! event.isShiftDown() );
        return true;
      }
    case IKeyboardEvent::f11:
      {
        if ( event.isShiftDown() )
          _editor.insertLink( LinkGin() );
        else
          _editor.insertLink( LinkGin( "target" ) );
        return true;
      }
    case IKeyboardEvent::f5:
      {
        _editor.resize();
        return true;
      }
    case IKeyboardEvent::f7:
      {
        if ( event.isCtrlDown() )
        {
          // display all fonts!
          IFont::FaceNameCursor faces( IFont::FaceNameCursor::both, presSpace() );
          IFont font( presSpace() );
          unsigned long size = 18;
          forCursor( faces )
          {
            font.setPointSize( size );
            font.setName( font.faceNameAt( faces ) );
            _editor.insertItem( new FontItem( &_flow, FontInfo( font ) ) );
            _editor.insertText( font.name() + IString("\n") );
            size = 48 - size;
          }
          return true;
        }

        // get an IFont to insert (F7)
        IFont font( this );
        IFontDialog::Settings settings( &font );
        IFontDialog dialog( NULL, this, settings );
        if ( dialog.pressedOK() )
          _editor.insertFont( FontInfo( font ) );
        return true;
      }
    case IKeyboardEvent::f9:
      {
        if ( event.isCtrlDown() )
        {
          // modify a font or link
          _editor.editProperties();
          return true;
        }

        // insert an empty group
        _editor.createGroup( NULL );
        return true;
      }
    case IKeyboardEvent::f6:
      {
        if ( event.isCtrlDown() )
        {
          // display all symbols (Ctrl+F6)
          _editor.insertText( "\nSymbols by ID:" );
          for (
            Symbol::Identifier id = Symbol::startOfList;
            id <= Symbol::endOfList;
            id = Symbol::Identifier(id+1) )
          {
            _editor.insertText( IString(" ") + IString(id) + IString("=") );
            _editor.insertSymbol( Symbol( id ) );
          }
          _editor.insertText( "\nSymbols by char:" );
          for ( int i = 1; i < 256; i++ )
          {
            _editor.insertText( IString(" ") + IString(i) + IString("=") );
            _editor.insertSymbol( Symbol( (unsigned char)i ) );
          }
          return true;
        }

        // dump attributes (F6)
        ITRACE_ALL( IString("Info: ") + _editor.info() );
        ITRACE_ALL( IString("Modified: ") + IString(_editor.isModified()) + IString(" (cleared)") );
        _editor.setModified( false );
        ITRACE_ALL( IString("Selection: ") + IString(_editor.hasSelection()) );
        ITRACE_ALL( IString("Properties: ") + IString(_editor.canEditProperties()) );
        return true;
      }
    } /* endswitch */
  }  /* end try */

  catch (IException& exc)
  {
    IMessageBox box( this );
    box.setTitle( "Caught by Tester (external)" );
    box.show( exc );
  }

  return false;
}


/***************************************************************************
 * Procedure.. VPort::insertHelpText
 * Author..... Mat
 * Date....... 4/7/98
 *
 * Inserts a list of key assignments.  Excercises the ability to insert
 * contents of an entire FlowItem into the editor.  Tests the following
 * functions:
 *   Editor::insertText
 *   FlowItem::insertItem
 *   FlowGenerator()
 *   Editor::insertItem( FlowItem )
 ***************************************************************************/
void VPort::insertHelpText()
{
  // insert header
  _editor.insertParagraph();
  _editor.insertText( "Vyper" );
  _editor.insertSymbol( Symbol( Symbol::reg ) );
  _editor.insertText( " Editor Tester " );
  _editor.insertArt( defaultArtId );
  _editor.insertNewLine();

  // create a flow view
  FlowItem flow( 0 );

  // get a cursor to the EndView (automatically placed by FlowView)
  OrderedItemCursor * cursor = flow.newCursor();
  cursor->setToLast();
  IASSERTSTATE( cursor->isValid() );
  Item * endItem = cursor->item();
  delete cursor;

  // populate the FlowView with bold, italic, and underline keys
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new WordItem( &flow, "F2" ), endItem );
  flow.insertItem( new SymbolItem( Symbol(':'), &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new BoldItem( &flow, true ), endItem );
  flow.insertItem( new WordItem( &flow, "bold" ), endItem );
  flow.insertItem( new BoldItem( &flow, false ), endItem );
  flow.insertItem( new NewLineItem( &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new WordItem( &flow, "F3" ), endItem );
  flow.insertItem( new SymbolItem( Symbol(':'), &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new ItalicItem( &flow, true ), endItem );
  flow.insertItem( new WordItem( &flow, "italic" ), endItem );
  flow.insertItem( new ItalicItem( &flow, false ), endItem );
  flow.insertItem( new NewLineItem( &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new WordItem( &flow, "F4" ), endItem );
  flow.insertItem( new SymbolItem( Symbol(':'), &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new UnderlineItem( &flow, true ), endItem );
  flow.insertItem( new WordItem( &flow, "underline" ), endItem );
  flow.insertItem( new UnderlineItem( &flow, false ), endItem );
  flow.insertItem( new NewLineItem( &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new WordItem( &flow, "F11" ), endItem );
  flow.insertItem( new SymbolItem( Symbol(':'), &flow ), endItem );
  flow.insertItem( new SpaceItem( &flow ), endItem );
  flow.insertItem( new LinkItem( &flow, LinkGin( "target" ).setFile( "file" ) ), endItem );
  flow.insertItem( new WordItem( &flow, "link" ), endItem );
  flow.insertItem( new LinkItem( &flow, LinkGin() ), endItem );
  flow.insertItem( new NewLineItem( &flow ), endItem );

  // insert remainder of the text via FlowGenerator
  strstream stream;
  stream << "  Shift+above: attribute off\n";
  stream << "  F5: refresh\n";
  stream << "  F6: dump attributes\n";
  stream << "  Ctrl+F6: symbol test\n";
  stream << "  F7: font\n";
  stream << "  Ctrl+F7: font test\n";
  stream << "  F8: insert text file\n";
  stream << "  Shift+F8: load bitmap art\n";
  stream << "  Ctrl+F8: save selection to vedit.vyt\n";
  stream << "  F9: group\n";
  stream << "  Ctrl+F9: modify font/link/art\n";
  stream << "  F12: this help\n";
  stream << "  Shift+Ins: paste text\n";
  stream << "  Esc: dump contents\n";
  stream.seekg( 0 );
  {
    TextParser parser( stream );
    FlowGenerator gen( flow );
    parser.generate( gen );
  }

  // insert the local FlowItem into the Editor's FlowItem
  _editor.insertItem( &flow );
}


