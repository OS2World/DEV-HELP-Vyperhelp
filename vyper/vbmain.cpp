/*******************************************************************************
* FILE NAME: vbmain.cpp                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Main program stub for class:                                               *
*     Vyper- Vyper main window                                                 *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _IWINDOW_
#include <iwindow.hpp>
#endif

#ifndef _IAPP_
#include <iapp.hpp>
#endif

#ifndef _IMSGBOX_
#include <imsgbox.hpp>
#endif

#ifndef _VYPER_
#include "Vyper.hpp"
#endif


//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char ** argv)
{
   IApplication::current().setArgs(argc, argv);

   Vyper *iPart;
   IMessageBox::Response resp = IMessageBox::unknown;

   do {
      try {
         iPart = new Vyper();
         iPart->initializePart();
         }
      catch (IException& exc) {
         resp = IMessageBox(IWindow::desktopWindow()).show(
                  exc.text(),
                  IMessageBox::retryCancelButton);
         }
      }
   while (resp == IMessageBox::retry);

   if (resp == IMessageBox::cancel) IApplication::current().exit();

   iPart->setAutoDeleteObject();
   iPart->show();
   IApplication::current().run();

   // 5/25/99 (Mat): added for memory debugging
   _dump_allocated( 0 );

   return 0;
}   //end main
