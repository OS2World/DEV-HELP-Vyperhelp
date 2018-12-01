/***************************************************************************
 * File...... SpaceView.hpp
 * Author.... Mat
 * Date...... 5/7/96
 *
 * SpaceView is the view of a space character.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef SPACEVIEW_HPP
#define SPACEVIEW_HPP


// TextEditor
#include "PlacedView.hpp"


class SpaceView: public PlacedView
{
  public:
    // constructor
    SpaceView( Item * subject, View * parent );

    // from PlacedView
    virtual FormatChange format( FormatPen & pen );
    virtual FitMode      fitMode() const;

    // from View
    virtual Coord   width() const;
    virtual void    draw( DrawPen & pen, const IRectangle & zone );
    virtual IString dumpString() const;
};


// inline functions
#include "SpaceView.ipp"


#endif

