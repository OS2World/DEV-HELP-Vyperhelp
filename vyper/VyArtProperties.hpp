/*******************************************************************************
* FILE NAME: VyArtProperties.hpp                                               *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     VyArtProperties- Dialog to select art                                    *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _VYARTPROPERTIES_
#define _VYARTPROPERTIES_  

class VyArtProperties;

#ifndef _IFRAME_
#include <iframe.hpp>
#endif

#ifndef _IOBSERVR_
#include <iobservr.hpp>
#endif

class IMultiCellCanvas;
#ifndef _VYART_
#include "VyArt.hpp"
#endif

#ifndef _IVSEQ_H_
#include <ivseq.h>
#endif

template <class Element,class Collection> class ICollectionViewListBox;
class IPushButton;
class IBitmapControl;
#ifndef _IVBVPRTP_
#include <ivbvprtp.h>
#endif

#ifndef _VYDOC_
#include "VyDoc.hpp"
#endif

class ExceptionNotification;
class IVBFileDialog;
class VySettings;
#ifndef _IRECT_
#include <irect.hpp>
#endif

#ifndef _ISTRING_
#include <istring.hpp>
#endif

#ifndef FILENAME_HPP
#include "Filename.hpp"
#endif

#ifndef ARTITEM_HPP
#include "ArtItem.hpp"
#endif

#ifndef _VYSETTINGS_
#include "VySettings.hpp"
#endif

class VyArtPropertiesConn0;
class VyArtPropertiesConn2;
class VyArtPropertiesConn3;
class VyArtPropertiesConn4;
class VyArtPropertiesConn22;
class VyArtPropertiesConn5;
class VyArtPropertiesConn6;
class VyArtPropertiesConn8;
class VyArtPropertiesConn13;
class VyArtPropertiesConn18;
class VyArtPropertiesConn10;
class VyArtPropertiesConn11;
class VyArtPropertiesConn12;
class VyArtPropertiesConn15;
#include "VyArtProperties.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for VyArtProperties
//*****************************************************************************
class VyArtProperties : public IFrameWindow, public IObserver {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   VyArtProperties(
      unsigned long id = WND_VyArtProperties,
      IWindow* parent = IWindow::desktopWindow(),
      IWindow* owner = 0,
      const IRectangle& rect = defaultFramingSpec(),
      const IFrameWindow::Style& style = defaultStyle(),
      const char* title = defaultTitle());

   virtual  ~VyArtProperties();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   static const IRectangle defaultFramingSpec();
   static const IFrameWindow::Style defaultStyle();
   static IString defaultTitle();
   virtual VyArtProperties & initializePart();
   VyArtProperties * getFrameWindow() { return this; };
   IVBVariablePartPointer<VyDoc *> *  getDocument() const { return iDocument; };
   IVBVariablePartPointer<VyArt *> *  getArt() const { return iArt; };

   //---------------------------------------------------------------------------
   // public member data
   //---------------------------------------------------------------------------
   static const INotificationId readyId;
   static const INotificationId document;
   static const INotificationId artName;

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
   IMultiCellCanvas * iMultiCellCanvas1;
   ICollectionViewListBox< VyArt*, IVSequence< VyArt* > > * iArtList;
   IPushButton * iAdd;
   IPushButton * iDelete;
   IBitmapControl * iPreview;
   IPushButton * iClose;
   IVBVariablePartPointer<VyDoc *> * iDocument;
   IVBVariablePartPointer<VyArt *> * iArt;
   ExceptionNotification * iExceptionNotification;
   IVBFileDialog * iFileDialog;
   IVBVariablePartPointer<VySettings *> * iSettings;
   VyArtPropertiesConn0 * conn0;
   VyArtPropertiesConn2 * conn2;
   VyArtPropertiesConn3 * conn3;
   VyArtPropertiesConn4 * conn4;
   VyArtPropertiesConn22 * conn22;
   VyArtPropertiesConn5 * conn5;
   VyArtPropertiesConn6 * conn6;
   VyArtPropertiesConn8 * conn8;
   VyArtPropertiesConn13 * conn13;
   VyArtPropertiesConn18 * conn18;
   VyArtPropertiesConn10 * conn10;
   VyArtPropertiesConn11 * conn11;
   VyArtPropertiesConn12 * conn12;
   VyArtPropertiesConn15 * conn15;


#include "VyArtProperties.hpv"
};   //VyArtProperties


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif
