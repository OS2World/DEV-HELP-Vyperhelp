/*******************************************************************************
* FILE NAME: LogicalNot.hpp                                                    *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     LogicalNot- Negates a condition                                          *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _LOGICALNOT_
#define _LOGICALNOT_  

class LogicalNot;

#ifndef _ISTDNTFY_
#include <istdntfy.hpp>
#endif

#include "LogicalNot.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for LogicalNot
//*****************************************************************************
class LogicalNot : public IStandardNotifier {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   LogicalNot();

   virtual  ~LogicalNot();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   virtual LogicalNot & initializePart();

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

#include "LogicalNot.hpv"
};   //LogicalNot


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif
