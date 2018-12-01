/*******************************************************************************
* FILE NAME: ExceptionNotification.hpp                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     ExceptionNotification- Exception message box                             *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _EXCEPTIONNOTIFICATION_
#define _EXCEPTIONNOTIFICATION_  

class ExceptionNotification;

#ifndef _ISTDNTFY_
#include <istdntfy.hpp>
#endif

#ifndef _IMSGBOX_
#include <imsgbox.hpp>
#endif

#include "ExceptionNotification.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for ExceptionNotification
//*****************************************************************************
class ExceptionNotification : public IStandardNotifier {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   ExceptionNotification();

   virtual  ~ExceptionNotification();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   virtual ExceptionNotification & initializePart();

   //---------------------------------------------------------------------------
   // public member data
   //---------------------------------------------------------------------------
   static const INotificationId readyId;

protected:
   //---------------------------------------------------------------------------
   // protected member functions
   //---------------------------------------------------------------------------
   Boolean makeConnections();

private:

#include "ExceptionNotification.hpv"
};   //ExceptionNotification


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif