/***************************************************************************
 * File...... FastContext.hpp
 * Author.... Mat
 * Date...... 9/23/97
 *
 * FastContext adds some platform-specific code to provide fast basic
 * graphics operations.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef FASTCONTEXT_HPP
#define FASTCONTEXT_HPP

// OpenClass
#include <icolor.hpp>       // IColor
#include <igrafctx.hpp>     // IGraphicContext

// TextEditor
#include "Types.hpp"


class FastContext: public IGraphicContext
{
  public:
    // constructors
    FastContext( const IWindowHandle & windowHandle, const IColor & backgroundColor );
    FastContext( const ISize & contextSize, const IColor & backgroundColor );

    // fast operations
    void drawChar( char ch, Coord width, const IPoint & position );
    void drawString( const IString & string, const Coord * widths, const IPoint & position );
    void drawBitmap( const IBitmapHandle & bitmap, const IPoint & position );
    void clearRect( const IRectangle & rect );
    void copyRect( const IRectangle & rect, const IPoint & position );

  private:
    IColor _backgroundColor;
};


#endif

