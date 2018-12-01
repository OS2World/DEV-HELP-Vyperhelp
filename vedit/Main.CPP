/***************************************************************************
 * File...... Main.CPP
 * Author.... Mat
 * Date...... 5/21/96
 *
 * Main entry-point for Editor driver
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// Standard C
#include <malloc.h>
#include <locale.h>

// OpenClass
#include <itrace.hpp>       // ITRACE macros
#include <iapp.hpp>         // IApplication

// TextEditor
#include "FlowItem.hpp"

// VyDit Tester
#include "vedit.h"
#include "VFrame.hpp"


int main()
{
  // setup for localization
  setlocale( LC_ALL, "" );

  {
    FlowItem flow( NULL );
    VFrame   mainWindow( WND_MAIN, flow );

    IApplication::current().run();
  }

  _dump_allocated( 0 );

  return 0;
}


// force "throw" code to be linked for debugging
void LoadThrow()
{
  throw 0;
}


