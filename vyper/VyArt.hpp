/*******************************************************************************
* FILE NAME: VyArt.hpp                                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
*   Declaration of the class:                                                  *
*     VyArt- Art definition                                                    *
* ---------------------------------------------------------------------------- *
* Warning: This file was generated by the VisualAge C++ Visual Builder.        *
* Modifications to this source file will be lost when the part is regenerated. *
*******************************************************************************/
#ifndef _VYART_
#define _VYART_  

class VyArt;

#ifndef _ISTDNTFY_
#include <istdntfy.hpp>
#endif

#ifndef ARTDEF_HPP
#include "ArtDef.hpp"
#endif

#include "VyArt.h"


/*----------------------------------------------------------------------------*/
/* Align classes on four byte boundary.                                       */
/*----------------------------------------------------------------------------*/
#pragma pack(4)

//*****************************************************************************
// Class definition for VyArt
//*****************************************************************************
class VyArt : public IStandardNotifier {
public:
   //---------------------------------------------------------------------------
   // Constructors / destructors
   //---------------------------------------------------------------------------
   VyArt();

   virtual  ~VyArt();

   //---------------------------------------------------------------------------
   // public member functions
   //---------------------------------------------------------------------------
   virtual VyArt & initializePart();

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

#include "VyArt.hpv"
};   //VyArt


/*----------------------------------------------------------------------------*/
/* Resume compiler default packing.                                           */
/*----------------------------------------------------------------------------*/
#pragma pack()

#endif
