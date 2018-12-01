/*******************************************************************************
* FILE NAME: VyLabel.hpp                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     VyLabel- hypertext label/anchor (adapts LabelGin)                        *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _VYLABEL_
#define _VYLABEL_  

class VyLabel;

#ifndef _ISTDNTFY_
#include <istdntfy.hpp>
#endif

#ifndef BOOLEANSTRING_HPP
#include "BooleanString.hpp"
#endif

#include "VyLabel.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for VyLabel
//*****************************************************************************
class VyLabel : public IStandardNotifier {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   VyLabel();

   virtual  ~VyLabel();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   virtual VyLabel & initializePart();

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

#include "VyLabel.hpv"
};   //VyLabel


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif
