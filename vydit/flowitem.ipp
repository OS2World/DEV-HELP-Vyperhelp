/***************************************************************************
 * File...... FlowItem.ipp
 * Author.... Mat
 * Date...... 1/22/98
 *
 * Inline functions for FlowItem
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/


inline FlowItem * FlowItem::asFlow()
{
  return this;
}


inline void FlowItem::setGenerator( Generator * gen )
{
  _gen = gen;
}


