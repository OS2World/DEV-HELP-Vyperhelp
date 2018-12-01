/***************************************************************************
 * File...... ContainerHelpHandler.cpp
 * Author.... Mat
 * Date...... 5/27/99
 *
 * Implementation of ContainerHelpHandler.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


// OpenClass
#include <ihelp.hpp>        // IHelpWindow
#include <icnrctl.hpp>      // IContainerControl
#include <ireslib.hpp>      // IResourceId

// Vyper
#include "ContainerHelpHandler.hpp"


ContainerHelpHandler::ContainerHelpHandler( unsigned long helpId ):
    _helpId( helpId )
{}


Boolean ContainerHelpHandler::help( ICnrHelpEvent & evt )
{
   IContainerControl* pcnrctl = (IContainerControl*)evt.controlWindow();
   if ( pcnrctl )
   {
      IHelpWindow * pHelp = IHelpWindow::helpWindow( pcnrctl );
      if ( pHelp )
      {
        pHelp->show( IResourceId( _helpId ) );
        return true;
      }
   }
   return false;
}

