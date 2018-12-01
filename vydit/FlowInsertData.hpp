/***************************************************************************
 * File...... FlowInsertData.hpp
 * Author.... Mat
 * Date...... 6/18/99
 *
 * A structure to hold data for the FlowItem::insertItem event.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef FLOWINSERTDATA_HPP
#define FLOWINSERTDATA_HPP


struct FlowInsertData
{
  OrderedItemCursor * cursor;
  Boolean             mergeWords;
};


#endif

