/***************************************************************************
 * File...... Page.hpp
 * Author.... Mat
 * Date...... 6/10/97
 *
 * Represents a page in memory onto which the FlowView image is drawn.
 * Page is then used to provide quick responses to paintWindow messages.
 * Page maintains the "memory graphic context" and all settings needed
 * to work with a Pen during format/draw.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef PAGE_HPP
#define PAGE_HPP

// OpenClass
#include <igrect.hpp>       // IGRectangle
#include <igregion.hpp>     // IGRegion
class IPaintEvent;
class IFrameWindow;
class ICanvas;

// TextEditor
#include "Types.hpp"          // Coord
class FastContext;


class _Export Page
{
  public:
    // constructor
    Page();
    ~Page();

    // size control
    void          sizeBy( const IPair & offset );
      // size the page by the given offset
    void          resetSize( Coord visibleWidth );
      // reset to 0,0 size
    const ISize & size() const;
    Coord         visibleWidth() const;

    // access to graphic context
    FastContext & context();

    // operations
    void clearRect( const IRectangle & rect );
      // clear a rectangle with background color
    void shiftRect( const IRectangle & rect, const IPair & offset );
      // shift a rectangle
    IColor backgroundColor() const;
      // return the background color of the page
    IColor defaultColor() const;
      // return the default foreground color of the page

    // invalid zone
    void       resetZone();
    IRectangle zone() const;

    // selection region
    void clearSelection();
    void addToSelection( const IRectangle & rect );

    // invalid (not yet drawn) region
    void sizeToInvalid( IRectangle & rect );
    void invalidate( const IRectangle & rect );
    void validate( const IRectangle & rect );

    // paint handling
    void paint( IPaintEvent & event );

  private:
    // helpers
    void       createContext( const ISize & contextSize );
    void       setTransform( const ISize & size );
    IRectangle transform( const IRectangle & rect ) const;
      // applies the current transform to the given rectangle
    void       zoneUnion( const IRectangle & rect );

    // workarounds for OpenClass bugs!
    void       sizeRectToRegion( IRectangle & rect, const IGRegion & region );
    void       drawRegion( IGRegion & region, IGraphicContext & context );

    // data
    Coord         _visibleWidth;
    ISize         _size;
    IRectangle    _zone;
    ISize         _contextSize;
    FastContext * _context;
    Boolean       _hasSelection;
    IGRegion      _selectionRegion;
    IGRegion      _invalidRegion;  // unpainted region

    // for draw debugging
    IFrameWindow *    _frame;
    ICanvas *         _canvas;

    // static data
    static IColor _backgroundColor;
    static IColor _defaultColor;
};


// inline functions
#include "Page.ipp"


#endif

