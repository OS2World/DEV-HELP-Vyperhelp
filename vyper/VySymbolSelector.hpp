/*******************************************************************************
* FILE NAME: VySymbolSelector.hpp                                              *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     VySymbolSelector- Symbol selection list                                  *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _VYSYMBOLSELECTOR_
#define _VYSYMBOLSELECTOR_  

class VySymbolSelector;

#ifndef _IFRAME_
#include <iframe.hpp>
#endif

#ifndef _IOBSERVR_
#include <iobservr.hpp>
#endif

#define INCL_WINSTDCNR
#include <os2.h>

#ifndef _VYSYMBOL_
#include "VySymbol.hpp"
#endif

#ifndef _IVSEQ_H_
#include <ivseq.h>
#endif

#ifndef _IVBCNR_
#include <ivbcnr.h>
#endif

#ifndef _IRECT_
#include <irect.hpp>
#endif

#ifndef _ISTRING_
#include <istring.hpp>
#endif

#ifndef CONTAINERHELPHANDLER_HPP
#include "ContainerHelpHandler.hpp"
#endif

class VySymbolSelectorConn0;
class VySymbolSelectorConn3;
#include "VySymbolSelector.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for VySymbolSelectorSymbolsCnrObj
//*****************************************************************************
class VySymbolSelectorSymbolsCnrObj : public IVBContainerObject {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   VySymbolSelectorSymbolsCnrObj(VySymbol& anObject);
   VySymbolSelectorSymbolsCnrObj(VySymbolSelectorSymbolsCnrObj& anObject);

   virtual  ~VySymbolSelectorSymbolsCnrObj();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   IVBContainerObject & refreshFromObject();
   IVBContainerObject & updateToObject();
   VySymbolSelectorSymbolsCnrObj& operator=(const VySymbolSelectorSymbolsCnrObj& anObject);
   VySymbol* realObject() { return &pRealObject; }
;

protected:
private:
   //---------------------------------------------------------------------------
   // private member data
   //---------------------------------------------------------------------------
   VySymbol & pRealObject;


};   //VySymbolSelectorSymbolsCnrObj


//*****************************************************************************
// Class definition for VySymbolSelector
//*****************************************************************************
class VySymbolSelector : public IFrameWindow, public IObserver {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   VySymbolSelector(
      unsigned long id = WND_VySymbolSelector,
      IWindow* parent = IWindow::desktopWindow(),
      IWindow* owner = 0,
      const IRectangle& rect = defaultFramingSpec(),
      const IFrameWindow::Style& style = defaultStyle(),
      const char* title = defaultTitle());

   virtual  ~VySymbolSelector();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   static const IRectangle defaultFramingSpec();
   static const IFrameWindow::Style defaultStyle();
   static IString defaultTitle();
   virtual VySymbolSelector & initializePart();
   VySymbolSelector * getFrameWindow() { return this; };
   IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj > *  getSymbols() const { return iSymbols; };

   //---------------------------------------------------------------------------
   // public member data
   //---------------------------------------------------------------------------
   static const INotificationId readyId;
   static const INotificationId enterEvent;
   static const INotificationId containerItems;
   static const INotificationId selectedSymbol;

protected:
   //---------------------------------------------------------------------------
   // protected member functions
   //---------------------------------------------------------------------------
   Boolean makeConnections();
   virtual IObserver & dispatchNotificationEvent(const INotificationEvent & anEvent);

private:
   //---------------------------------------------------------------------------
   // private member data
   //---------------------------------------------------------------------------
   IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj > * iSymbols;
   ContainerHelpHandler * iSymbolsContainerHelpHandler;
   VySymbolSelectorConn0 * conn0;
   VySymbolSelectorConn3 * conn3;


};   //VySymbolSelector


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif