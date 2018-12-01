/***************************************************************************
 * File...... VFrame.hpp
 * Author.... Mat
 * Date...... 5/21/96
 *
 * VFrame is the frame window which holds the Editor.  Editor and
 * VPort work together to provide scrolling of the edit contents.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef VFRAME_HPP
#define VFRAME_HPP

// OpenClass
#include <iframe.hpp>       // IFrameWindow

// VyDit
class FlowItem;

// VyDit Tester
#include "VPort.hpp"


class VFrame: public IFrameWindow
{
  public:
    // constructor
    VFrame( unsigned long windowId, FlowItem & flow );

  private:
    VPort _viewport;
};

#endif
