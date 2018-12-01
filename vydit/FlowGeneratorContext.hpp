/***************************************************************************
 * File...... FlowGeneratorContext.hpp
 * Author.... Mat
 * Date...... 5/19/98
 *
 * This structure holds the context for a FlowGenerator.  It contains
 * font info for mapping from FontGin/SizeGin to FontItem, and a pointer
 * to the last PushItem.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef FLOWGENERATORCONTEXT_HPP
#define FLOWGENERATORCONTEXT_HPP

// OpenClass
#include <istring.hpp>      // IString

// Generator
#include "Gin.hpp"

// Editor
class PushItem;


struct FlowGeneratorContext
{
  // font info
  FontInfo::Family     family;
  IString              facename;
  unsigned long        pointSize;
  CodePage             codePage;
  // last PushItem
  PushItem *           push;
};


#endif

