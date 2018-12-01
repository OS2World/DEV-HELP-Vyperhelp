/***************************************************************************
 * File...... IPF_LabelList.hpp
 * Author.... Mat
 * Date...... 3/12/96
 *
 * LabelList is a queue of Labels used to store labels within a section.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef IPF_LABELLIST_HPP
#define IPF_LABELLIST_HPP

// OpenClass
#include <iqueue.h>

// Generator
#include "Gin.hpp"


class LabelList: public IQueue< LabelGin >
{
  public:
    LabelList( INumber numberOfElements = 10 );
};


#endif

