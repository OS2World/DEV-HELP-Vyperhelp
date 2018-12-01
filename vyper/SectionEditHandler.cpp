/***************************************************************************
 * File...... SectionEditHandler.cpp
 * Author.... Mat
 * Date...... 3/10/98
 *
 * Implementation of SectionEditHandler.  Uses IVBContainerObject to
 * update the associated object.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// OpenClass
#include <ivbcnr.h>         // IVBContainerObject
#include <itrace.hpp>       // ITRACE macros

// Vyper
#include "SectionEditHandler.hpp"


Boolean SectionEditHandler::endEdit( ICnrEndEditEvent & event )
{
  ITRACE_ALL( IString("End direct edit: ") + event.newText() );

  // assume we are attached to an IVBContainerControl
  IVBContainerObject * object = (IVBContainerObject *) event.object();
  object->updateToObject();

  return ICnrEditHandler::endEdit( event );
}


