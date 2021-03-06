/*******************************************************************************
* FILE NAME: VyWait.hpp                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     VyWait- Progress and status indicator                                    *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _VYWAIT_
#define _VYWAIT_  

class VyWait;

#ifndef _IFRAME_
#include <iframe.hpp>
#endif

#ifndef _IOBSERVR_
#include <iobservr.hpp>
#endif

class ISetCanvas;
#ifndef _ISTATTXT_
#include <istattxt.hpp>
#endif

#ifndef _ISLIDER_
#include <islider.hpp>
#endif

#ifndef _IMLE_
#include <imle.hpp>
#endif

#ifndef _IPUSHBUT_
#include <ipushbut.hpp>
#endif

#ifndef _IRECT_
#include <irect.hpp>
#endif

#ifndef _ISTRING_
#include <istring.hpp>
#endif

#ifndef VYINDICATOR_HPP
#include "VyIndicator.hpp"
#endif

class VyWaitConn0;
#include "VyWait.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for VyWait
//*****************************************************************************
class VyWait : public IFrameWindow, public IObserver {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   VyWait(
      unsigned long id = WND_VyWait,
      IWindow* parent = IWindow::desktopWindow(),
      IWindow* owner = 0,
      const IRectangle& rect = defaultFramingSpec(),
      const IFrameWindow::Style& style = defaultStyle(),
      const char* title = defaultTitle());

   virtual  ~VyWait();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   static const IRectangle defaultFramingSpec();
   static const IFrameWindow::Style defaultStyle();
   static IString defaultTitle();
   virtual VyWait & initializePart();
   VyWait * getFrameWindow() { return this; };
   IStaticText *  getTask() const { return iTask; };
   IStaticText *  getStatus() const { return iStatus; };
   IProgressIndicator *  getProgressIndicator() const { return iProgressIndicator; };
   IMultiLineEdit *  getErrorList() const { return iErrorList; };
   IPushButton *  getOK() const { return iOK; };

   //---------------------------------------------------------------------------
   // public member data
   //---------------------------------------------------------------------------
   static const INotificationId readyId;
   static const INotificationId taskText;
   static const INotificationId statusText;
   static const INotificationId progressIndicatorArmTickOffset;
   static const INotificationId oKEnabled;

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
   ISetCanvas * iSetCanvas1;
   IStaticText * iTask;
   IStaticText * iStatus;
   IProgressIndicator * iProgressIndicator;
   IMultiLineEdit * iErrorList;
   IPushButton * iOK;
   VyWaitConn0 * conn0;


#include "VyWait.hpv"
};   //VyWait


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif
