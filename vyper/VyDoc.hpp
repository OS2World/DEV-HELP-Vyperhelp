/*******************************************************************************
* FILE NAME: VyDoc.hpp                                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     VyDoc- Represents a VYD file                                             *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _VYDOC_
#define _VYDOC_  

class VyDoc;

#ifndef _ISTDNTFY_
#include <istdntfy.hpp>
#endif

#ifndef _IOBSERVR_
#include <iobservr.hpp>
#endif

#ifndef _VYSECTION_
#include "VySection.hpp"
#endif

#ifndef _IVSEQ_H_
#include <ivseq.h>
#endif

#ifndef _VYART_
#include "VyArt.hpp"
#endif

#ifndef LOADTHREAD_HPP
#include "LoadThread.hpp"
#endif

#ifndef SAVETHREAD_HPP
#include "SaveThread.hpp"
#endif

#ifndef LOADPARSER_HPP
#include "LoadParser.hpp"
#endif

#ifndef _ICNRCTL_
#include <icnrctl.hpp>
#endif

#ifndef TEXTPARSER_HPP
#include "TextParser.hpp"
#endif

#ifndef WINHELPPARSER_HPP
#include "WinHelpParser.hpp"
#endif

#ifndef TRACEGENERATOR_HPP
#include "TraceGenerator.hpp"
#endif

#ifndef IPFGENERATOR_HPP
#include "IPFGenerator.hpp"
#endif

#ifndef TEXTGENERATOR_HPP
#include "TextGenerator.hpp"
#endif

#ifndef _ICNROLST_
#include <icnrolst.hpp>
#endif

#ifndef _ICOMBOBX_
#include <icombobx.hpp>
#endif

#ifndef _IAPP_
#include <iapp.hpp>
#endif

#ifndef IPFPARSER_HPP
#include "IpfParser.hpp"
#endif

#ifndef HEADERGENERATOR_HPP
#include "HeaderGenerator.hpp"
#endif

#ifndef RTFGENERATOR_HPP
#include "RtfGenerator.hpp"
#endif

#ifndef HPJGENERATOR_HPP
#include "HpjGenerator.hpp"
#endif

#ifndef _VYWAIT_
#include "VyWait.hpp"
#endif

#ifndef VYINDICATOR_HPP
#include "VyIndicator.hpp"
#endif

#ifndef HTMLGENERATOR_HPP
#include "HtmlGenerator.hpp"
#endif

#ifndef _VYSETTINGS_
#include "VySettings.hpp"
#endif

#include "VyDoc.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for VyDoc
//*****************************************************************************
class VyDoc : public IStandardNotifier, public IObserver {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   VyDoc();

   virtual  ~VyDoc();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   virtual VyDoc & initializePart();
   IVSequence< VySection* > *  getSections() const { return iSections; };
   IVSequence< VyArt* > *  getArts() const { return iArts; };

   //---------------------------------------------------------------------------
   // public member data
   //---------------------------------------------------------------------------
   static const INotificationId readyId;
   static const INotificationId sections;
   static const INotificationId arts;

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
   IVSequence< VySection* > * iSections;
   IVSequence< VyArt* > * iArts;


#include "VyDoc.hpv"
};   //VyDoc


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif