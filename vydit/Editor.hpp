/***************************************************************************
 * File...... Editor.hpp
 * Author.... Mat
 * Date...... 8/17/95
 *
 * Editor is the client window of the scrollable viewport.  It contains
 * one FlowView.  The FlowView is a composite view containing all of the
 * Views being edited.  Each View corresponds to an Item in the document.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef EDITOR_HPP
#define EDITOR_HPP

// OpenClass
#include <icanvas.hpp>      // ICanvas
#include <ipainhdr.hpp>     // IPaintHandler
#include <ikeyhdr.hpp>      // IKeyboardHandler
#include <imoushdr.hpp>     // IMouseHandler
#include <itimer.hpp>       // ITimer
class IViewPort;

// Generator
#include "ArtDef.hpp"
class Generator;
class Symbol;
class LinkGin;

// Editor
#include "Kursor.hpp"
#include "Page.hpp"
#include "PlacedView.hpp"
#include "EditorCursor.hpp"
class ViewCursor;
class FlowItem;
class FlowView;
class LineView;
class Item;


class _Export Editor:
  public ICanvas,
  public IPaintHandler,
  public IKeyboardHandler,
  public IMouseHandler
{
  public:
    // constructor
    Editor( unsigned long windowId, IViewPort * viewport, FlowItem * flow );
    ~Editor();

    // version
    static IString version();

    // attributes
    void    setInfo( const IString & info );
    IString info() const;
      // infomation string for currently selected view
    Boolean isModified() const;
      // true if any item has been modified since the last clearModified
    void    setModified( Boolean enable = true );
    Boolean hasSelection() const;
    Boolean canEditProperties() const;

    static INotificationId infoId;
    static INotificationId modifiedId;
    static INotificationId selectionId;
    static INotificationId propertiesId;
    static INotificationId editFontId;
    static INotificationId editLinkId;
    static INotificationId editArtId;
    static INotificationId exceptionId;

    // actions
    void defineArt( const ArtDef & art );
    void removeArt( const IString & id );
    void insertArt( const IString & id );
    void insertBold( Boolean enable = true );
    void insertUnderline( Boolean enable = true );
    void insertItalic( Boolean enable = true );
    void insertFont( const FontInfo & info );
    void insertLink( const LinkGin & link );
    void insertParagraph();
    void insertNewLine();
    void insertSymbol( const Symbol & symbol );
    void insertText( const IString & string, Boolean checkOvertype = false );
    void insertItem( Item * item );
    void createGroup( Item * item );
    void deleteItems( Boolean forward = true );
    void resize();
      // called when width changes
    void generate( Generator & gen );
    void generateSelection( Generator & gen );
    void editProperties();

    // formatter
    void formatStartingAt( PlacedView & startingView );
      // format the editor, starting on the given view
    void changeFormatView( PlacedView * viewOld, PlacedView * viewNew );
      // called to notify when a PlacedView is being replaced

    // used by EditorTrigger
    void triggerAction();

    // used by EditorCursor
    void changeSelection( ViewCursor & cursor1, ViewCursor & cursor2 );
    void refreshSelection();

    // from ICanvas
    virtual Boolean isTabStop() const;

    // from IControl
    virtual IControl & enableTabStop( Boolean enable = true );

    // diagnostic
    void dump( unsigned level );

  protected:
    // IPaintHandler
    virtual Boolean paintWindow( IPaintEvent &event );

    // IKeyboardHandler
    virtual Boolean virtualKeyPress( IKeyboardEvent & event );
    virtual Boolean characterKeyPress( IKeyboardEvent & event );
    virtual Boolean key( IKeyboardEvent & event );

    // IMouseHandler
    virtual Boolean mouseClicked( IMouseClickEvent & event );
    virtual Boolean mousePointerChange( IMousePointerEvent & event );
    virtual Boolean mouseMoved( IMouseEvent & event );

  private:
    // kursor movement
    void moveKursorTopBottom( Boolean moveTop );
    void moveKursorHomeEnd( Boolean moveHome );
    void moveKursorUpDown( Boolean moveUp );
    void moveKursorRightLeft( Boolean moveRight, Boolean moveByWord );
    void moveKursorByGroup( Boolean moveRight );
    void adjustKursor();
    void positionKursor( Boolean setKursorPoint );
    void scrollToFit( ViewCursor & viewCursor );
    void scrollToFitLine( const LineView * lineView );

    // cursor and selection support
    void dropAnchor( Boolean wantAnchor );
    void resetSelection();
    void autoSelect( Boolean moveRight );

    // formatting helpers
    void format( PlacedView * startingView = 0, PlacedView * endingView = 0 );
    void prepareForMove( Boolean wantAnchor );
    void checkForFormat();
    void checkForRedraw();
    void setFormatView( PlacedView & view );
    void clearFormatView();
    void showSelection( IRectangle & rect );
    void redraw( IRectangle & rect );
      // repaint the given region

    // mouse helpers
    void handleMouseMove( IMouseEvent & event, Boolean wantAnchor );
    void handleDoubleClick();

    // attribute helpers
    void setSelectionFlag( Boolean enable = true );
    void checkForProperties();

    // command helpers
    Boolean getSelection( EditorCursor & start, EditorCursor & end, Boolean includeEnd = false );
    Boolean isUnbalanced( EditorCursor & start, EditorCursor & end );
    void    removeSelection();
    void    flushInput();

    // diagnostic
    void dumpItemNode( Item * item, unsigned indent, unsigned level );
    void dumpViewNode( View * view, unsigned indent, unsigned level );

    // data
    Page            _page;
    IViewPort *     _viewport;
    FlowView *      _flow;
    FlowItem *      _flowItem;
    EditorCursor    _current;
    EditorCursor    _anchor;
    int             _anchorOrder;    // -1,0,1 to indicate order relative to _current
    unsigned        _holdFormatting; // >0 to hold formatting (during operations)
    PlacedView *    _startFormatView;   // starting PlacedView requiring format
    PlacedView *    _endFormatView;     // ending PlacedView requiring format
                                        // (both are 0 if formatting is not needed)
    Boolean         _isSelectionEnabled;  // true if selection updating is active
    EditorCursor    _beginChange;    // begin/end of area where selection has changed
    EditorCursor    _endChange;
    Kursor          _kursor;
    IPoint          _kursorPoint;
    Boolean         _isOvertype;
    Boolean         _isDragActive;
    IPoint          _mousePoint;     // position of last mouse click or drag
    ITimer          _timer;
    IString         _input;
    IReference<ITimerFn> _trigger;   // ITimer requires a REFERENCE to a timer function!
    IString         _info;
    Boolean         _isModified;
    Boolean         _hasSelection;
    Boolean         _canEditProperties;
    Boolean         _isPaintValid;
};


// inline functions
#include "Editor.ipp"


#endif

