/***************************************************************************
 * File...... ColumnList.cpp
 * Author.... Mat
 * Date...... 2/2/99
 *
 * Implementation of ColumnList.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/


#include "ColumnList.hpp"


//
// TEMPLATE INSTANCES
//
#include <ilnseq.c>
#include <iqueseq.c>

// ColumnList
#pragma define(IGLnSqCrs<Distance,IStdOps<Distance> >)
#pragma define(IGLinkedSequenceNode<Distance,IStdOps<Distance> >)
#pragma define(IGLnSq<Distance,IStdOps<Distance> >)
#pragma define(IWQueueOnSeqCursor<Distance,IStdOps<Distance>,IGLnSq<Distance,IStdOps<Distance> > >)
#pragma define(IWQuOnSq<Distance,IStdOps<Distance>,IGLnSq<Distance,IStdOps<Distance> > >)



ColumnList::ColumnList( INumber numberOfElements ):
    IQueue< Distance >( numberOfElements )
{}


