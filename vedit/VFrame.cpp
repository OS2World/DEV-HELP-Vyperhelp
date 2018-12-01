/***************************************************************************
 * File...... VFrame.cpp
 * Author.... Mat
 * Date...... 5/21/96
 *
 * Implementation of VFrame
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// OpenClass
#include <itrace.hpp>       // ITRACE macros

// VyDit Tester
#include "VFrame.hpp"
#include "vedit.h"


VFrame::VFrame( unsigned long windowId, FlowItem & flow )
  : IFrameWindow( windowId ),
    _viewport( IC_FRAME_CLIENT_ID, this, flow )
{
  // set client, set focus and show window
  setClient( &_viewport );
  ISize desktopSize = IWindow::desktopWindow()->size();
  sizeTo( desktopSize / 2 );
  moveTo( IPoint( 0, 32 ) );
  setFocus();
  show();
}


