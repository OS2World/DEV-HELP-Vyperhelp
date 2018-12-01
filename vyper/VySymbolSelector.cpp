/*******************************************************************************
* FILE NAME: VySymbolSelector.cpp                                              *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Class implementation of the class:                                         *
*     VySymbolSelector- Symbol selection list                                  *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _IFRAME_
#include <iframe.hpp>
#endif

#ifndef _IPOINT_
#include <ipoint.hpp>
#endif

#ifndef _INOTIFEV_
#include <inotifev.hpp>
#endif

#ifndef _IOBSERVR_
#include <iobservr.hpp>
#endif

#ifndef _ISTDNTFY_
#include <istdntfy.hpp>
#endif

#ifndef _VYSYMBOLSELECTOR_
#include "VySymbolSelector.hpp"
#endif

#ifndef _IVBDEFS_
#include <ivbdefs.h>
#endif

#ifndef _ITRACE_
#include <itrace.hpp>
#endif


//*****************************************************************************
// Class definition for VySymbolSelectorConn0
//*****************************************************************************
class VySymbolSelectorConn0 : public IObserver, public IStandardNotifier {
public:
   virtual  ~VySymbolSelectorConn0(){};

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   void initialize(IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj > * aSource, IFrameWindow * aTarget)
      {source = aSource; target = aTarget; enableNotification(); };

protected:
   //---------------------------------------------------------------------------
   // protected member functions
   //---------------------------------------------------------------------------
   virtual IObserver & dispatchNotificationEvent(const INotificationEvent & anEvent)
      {
      if (anEvent.notificationId() == IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj >::enterId)
         {
         IFUNCTRACE_DEVELOP();
         ITRACE_DEVELOP(" firing connection : Symbols(enterEvent) to FrameWindow(close))");
         try {target->close();}
         catch (IException& exc) {};
         }
      return(*this);
      };

private:
   //---------------------------------------------------------------------------
   // private member data
   //---------------------------------------------------------------------------
   IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj > * source;
   IFrameWindow * target;


};   //VySymbolSelectorConn0


//*****************************************************************************
// Class definition for VySymbolSelectorConn3
//*****************************************************************************
class VySymbolSelectorConn3 : public IObserver, public IStandardNotifier {
public:
   virtual  ~VySymbolSelectorConn3(){};

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   void initialize(VySymbolSelector * aSource, IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj > * aTarget)
      {source = aSource; target = aTarget; enableNotification(); };

protected:
   //---------------------------------------------------------------------------
   // protected member functions
   //---------------------------------------------------------------------------
   virtual IObserver & dispatchNotificationEvent(const INotificationEvent & anEvent)
      {
      if (anEvent.notificationId() == VySymbolSelector::readyId)
         {
         IFUNCTRACE_DEVELOP();
         ITRACE_DEVELOP(" firing connection : (ready) to Symbols(setFocus))");
         try {target->setFocus();}
         catch (IException& exc) {};
         }
      return(*this);
      };

private:
   //---------------------------------------------------------------------------
   // private member data
   //---------------------------------------------------------------------------
   VySymbolSelector * source;
   IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj > * target;


};   //VySymbolSelectorConn3


//------------------------------------------------------------------------------
// VySymbolSelectorSymbolsCnrObj :: VySymbolSelectorSymbolsCnrObj
//------------------------------------------------------------------------------
#pragma export (VySymbolSelectorSymbolsCnrObj::VySymbolSelectorSymbolsCnrObj(VySymbol&))
VySymbolSelectorSymbolsCnrObj::VySymbolSelectorSymbolsCnrObj(VySymbol& anObject)
 :IVBContainerObject(anObject.text(), ISystemPointerHandle(ISystemPointerHandle::warning)), pRealObject(anObject)
{
   enableDataUpdate();
   handleNotificationsFor(pRealObject);
}


//------------------------------------------------------------------------------
// VySymbolSelectorSymbolsCnrObj :: VySymbolSelectorSymbolsCnrObj
//------------------------------------------------------------------------------
#pragma export (VySymbolSelectorSymbolsCnrObj::VySymbolSelectorSymbolsCnrObj(VySymbolSelectorSymbolsCnrObj&))
VySymbolSelectorSymbolsCnrObj::VySymbolSelectorSymbolsCnrObj(VySymbolSelectorSymbolsCnrObj& anObject)
 :IVBContainerObject(anObject.iconText(), anObject.icon()), pRealObject(anObject.pRealObject)
{  }


//------------------------------------------------------------------------------
// VySymbolSelectorSymbolsCnrObj :: ~VySymbolSelectorSymbolsCnrObj
//------------------------------------------------------------------------------
#pragma export (VySymbolSelectorSymbolsCnrObj::~VySymbolSelectorSymbolsCnrObj())
VySymbolSelectorSymbolsCnrObj::~VySymbolSelectorSymbolsCnrObj()
{
   this->stopHandlingNotificationsFor(pRealObject);
}


//------------------------------------------------------------------------------
// VySymbolSelectorSymbolsCnrObj :: refreshFromObject
//------------------------------------------------------------------------------
#pragma export (VySymbolSelectorSymbolsCnrObj::refreshFromObject())
IVBContainerObject & VySymbolSelectorSymbolsCnrObj::refreshFromObject()
{
   setIconText(pRealObject.text());
   setIcon(ISystemPointerHandle(ISystemPointerHandle::warning));
   return *this;
}


//------------------------------------------------------------------------------
// VySymbolSelectorSymbolsCnrObj :: updateToObject
//------------------------------------------------------------------------------
IVBContainerObject & VySymbolSelectorSymbolsCnrObj::updateToObject()
{
   return *this;
}


//------------------------------------------------------------------------------
// VySymbolSelectorSymbolsCnrObj :: operator=
//------------------------------------------------------------------------------
VySymbolSelectorSymbolsCnrObj& VySymbolSelectorSymbolsCnrObj::operator=(const VySymbolSelectorSymbolsCnrObj& anObject)
{
   if (this == &anObject) {
      return *this;
      }
   IVBContainerObject::operator=(anObject);
   return *this;
}



#pragma export (VySymbolSelector::readyId)
const INotificationId VySymbolSelector::readyId = "VySymbolSelector::readyId";

#pragma export (VySymbolSelector::enterEvent)
const INotificationId VySymbolSelector::enterEvent = "VySymbolSelector::enterEvent";

#pragma export (VySymbolSelector::containerItems)
const INotificationId VySymbolSelector::containerItems = "VySymbolSelector::containerItems";

#pragma export (VySymbolSelector::selectedSymbol)
const INotificationId VySymbolSelector::selectedSymbol = "VySymbolSelector::selectedSymbol";

//------------------------------------------------------------------------------
// VySymbolSelector :: defaultFramingSpec
//------------------------------------------------------------------------------
#pragma export (VySymbolSelector::defaultFramingSpec())
const IRectangle VySymbolSelector::defaultFramingSpec()
{
   return(IRectangle(IPoint(30, IWindow::desktopWindow()->size().height() - 28 - 380),ISize(503, 380)));
}


//------------------------------------------------------------------------------
// VySymbolSelector :: defaultStyle
//------------------------------------------------------------------------------
#pragma export (VySymbolSelector::defaultStyle())
const IFrameWindow::Style VySymbolSelector::defaultStyle()
{
   return(IFrameWindow::defaultStyle() & ~IFrameWindow::windowList);
}


//------------------------------------------------------------------------------
// VySymbolSelector :: defaultTitle
//------------------------------------------------------------------------------
#pragma export (VySymbolSelector::defaultTitle())
IString VySymbolSelector::defaultTitle()
{
   return("Select symbol");
}


//------------------------------------------------------------------------------
// VySymbolSelector :: VySymbolSelector
//------------------------------------------------------------------------------
#pragma export (VySymbolSelector::VySymbolSelector(unsigned long, IWindow*, IWindow*, const IRectangle&, const IFrameWindow::Style&, const char*))
VySymbolSelector::VySymbolSelector(
      unsigned long id, 
      IWindow* parent, 
      IWindow* owner, 
      const IRectangle& rect, 
      const IFrameWindow::Style& style, 
      const char* title)
   : IFrameWindow(id, parent, owner, rect, style, title)
{
   iSymbols = new IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj >(
      IC_FRAME_CLIENT_ID, 
      this, 
      this, 
      IRectangle ( ), 
      IContainerControl::defaultStyle() | IContainerControl::readOnly | IContainerControl::autoPosition | IContainerControl::singleSelection | IControl::group | IControl::tabStop | IWindow::visible);

   conn0 = new VySymbolSelectorConn0();
   conn3 = new VySymbolSelectorConn3();

   iSymbolsContainerHelpHandler = new ContainerHelpHandler( 7000 );
   iSymbolsContainerHelpHandler->handleEventsFor(iSymbols);
   iSymbols->showFlowedTextView();
   iSymbols->setMinimumSize(ISize(371, 275));
   this->setFocus();
   this->setAutoDeleteObject(true);
   this->setClient(iSymbols);
}     //end constructor


//------------------------------------------------------------------------------
// VySymbolSelector :: ~VySymbolSelector
//------------------------------------------------------------------------------
#pragma export (VySymbolSelector::~VySymbolSelector())
VySymbolSelector::~VySymbolSelector()
{
   this->stopHandlingNotificationsFor(*iSymbols);
   conn0->stopHandlingNotificationsFor(*iSymbols);
   conn3->stopHandlingNotificationsFor(*this);

   delete conn0;
   delete conn3;

   iSymbolsContainerHelpHandler->stopHandlingEventsFor(iSymbols);
   delete iSymbolsContainerHelpHandler;

   delete iSymbols;
}

//------------------------------------------------------------------------------
// VySymbolSelector :: initializePart
//------------------------------------------------------------------------------
#pragma export (VySymbolSelector::initializePart())
VySymbolSelector & VySymbolSelector::initializePart()
{
   this->handleNotificationsFor(*iSymbols);
   makeConnections();
   notifyObservers(INotificationEvent(readyId, *this));
   return *this;
}


//------------------------------------------------------------------------------
// VySymbolSelector :: makeConnections
//------------------------------------------------------------------------------
#pragma export (VySymbolSelector::makeConnections())
Boolean VySymbolSelector::makeConnections()
{
   this->enableNotification();
   iSymbols->enableNotification();

   unsigned long id = this->id();
   conn0->initialize(iSymbols, this);
   conn0->handleNotificationsFor(*iSymbols);
   conn3->initialize(this, iSymbols);
   conn3->handleNotificationsFor(*this);

   return true;
}


//------------------------------------------------------------------------------
// VySymbolSelector :: dispatchNotificationEvent
//------------------------------------------------------------------------------
#pragma export (VySymbolSelector::dispatchNotificationEvent(const INotificationEvent &))
IObserver & VySymbolSelector::dispatchNotificationEvent(const INotificationEvent & anEvent)
{
   if ((anEvent.notificationId() == IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj >::enterId) && (iSymbols == &anEvent.notifier()))
       notifyObservers(INotificationEvent(enterEvent, *this, anEvent.hasNotifierAttrChanged(), IEventData((void *)anEvent.eventData()), anEvent.observerData()));
   else
   if ((anEvent.notificationId() == IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj >::itemsId) && (iSymbols == &anEvent.notifier()))
       notifyObservers(INotificationEvent(containerItems, *this, anEvent.hasNotifierAttrChanged(), IEventData((void *)anEvent.eventData()), anEvent.observerData()));
   else
   if ((anEvent.notificationId() == IVBContainerControl< VySymbol*, IVSequence<VySymbol*>, VySymbolSelectorSymbolsCnrObj >::selectId) && (iSymbols == &anEvent.notifier()))
       notifyObservers(INotificationEvent(selectedSymbol, *this, anEvent.hasNotifierAttrChanged(), IEventData((void *)anEvent.eventData()), anEvent.observerData()));
   return *this;
}


