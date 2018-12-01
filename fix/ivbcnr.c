/***************************************************************************
 * File...... ivbcnr.c
 * Author.... Mat
 * Date...... 3/13/98
 *
 * Template source modified from IBM-supplied files to correct problems
 * in elementChanged which attempts to delete and
 * re-insert objects.  This scheme causes object attributes (such as
 * in-use and tree position) to be lost.
 *
 ***************************************************************************/

/*******************************************************************************
* FILE NAME: ivbcnr.c                                                          *
*                                                                              *
* DESCRIPTION:                                                                 *
*   This file contains the implementation of classes/functions declared        *
*   in ivbcnr.h.                                                               *
*                                                                              *
* COPYRIGHT:                                                                   *
*   IBM Open Class Library                                                     *
*   (C) Copyright International Business Machines Corporation 1992, 1995       *
*   Licensed Material - Program-Property of IBM - All Rights Reserved.         *
*   US Government Users Restricted Rights - Use, duplication, or disclosure    *
*   restricted by GSA ADP Schedule Contract with IBM Corp.                     *
*                                                                              *
*******************************************************************************/
#include  <ivbcnr.h>
#include  <icolvwd.hpp>
#include  <iseq.h>
#include  <ivseq.h>
#include  <ipartccl.h>
#include  <istring.hpp>
#include  <iexcept.hpp>
#include  <icconst.h>
#include  <itrace.hpp>


template <class Element, class ViewClass, class Collection>
  class IVBContainerControlData : public IBase {

public:

  IVBContainerControlData ( );

 ~IVBContainerControlData ( );

ICollectionViewData< Element,
                     ViewClass,
                     Collection >
 *fCollectionViewData;

private:
/*------------------------- Hidden Members -----------------------------------*/
  IVBContainerControlData
             ( const IVBContainerControlData< Element, ViewClass, Collection >& source );
IVBContainerControlData<Element,ViewClass,Collection>
 &operator=  ( const IVBContainerControlData< Element, ViewClass, Collection >& source );


};


template <class Element, class ViewClass, class Collection>
  IVBContainerControlData<Element, ViewClass, Collection>::
    IVBContainerControlData( )
{
  fCollectionViewData = new
        ICollectionViewData< Element,
                             ViewClass,
                             Collection > ( ) ;
}

template <class Element, class ViewClass, class Collection>
  IVBContainerControlData<Element,ViewClass,Collection>::
   ~IVBContainerControlData ( )
{
  delete fCollectionViewData;
}



template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>::
    IVBContainerControl ( unsigned long                       identifier,
                          IWindow*                            parent,
                          IWindow*                            owner ,
                          const IRectangle&                   initial,
                          const IContainerControl::Style&     style,
                          const IContainerControl::Attribute& attribute )
    : IContainerControl( identifier, parent, owner, initial, style, attribute )
{
  initialize( );
}


template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>::
    IVBContainerControl ( unsigned long   identifier,
                          IWindow*        parent   )
    : IContainerControl( identifier, parent )
{
  initialize( );
}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>::
    IVBContainerControl( const IWindowHandle&  handle  )
    : IContainerControl( handle )
{
  initialize( );
}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>::
   ~IVBContainerControl ( )
{
  disableNotification();

  delete fVBContainerControlData;
}



template <class Element, class Collection, class CnrElement>
  Collection*  IVBContainerControl<Element,Collection,CnrElement>:: items( ) const
{
  return  (Collection *)fVBContainerControlData->fCollectionViewData->fCollectionObserver.collection();
}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&   IVBContainerControl<Element,Collection,CnrElement>::
    setItems( Collection* collection )
{
  fVBContainerControlData->fCollectionViewData->fCollectionObserver.setCollection( collection );

  return *this;
}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement> :: elementsChanged ( )
{
  ICursor*       pCursor;
  Collection*    collection;
  collection   = items( );

  /*  Clear container and repopulate w/ current representations */
  Boolean refreshOn = isRefreshOn();
  setRefreshOff    ( );
  deleteAllObjects ( );

  if ( collection )
  {
    pCursor         = collection->newCursor( );
    forCursor( *pCursor )
    {
      addObject( new CnrElement( *( collection->elementAt( *pCursor ) ) ) );
    }
    delete pCursor;
  }

  setRefreshOn ( );
  refresh      ( );
  setRefreshOn ( refreshOn );

#if 0
  // fixed code (deleted)
  IMODTRACE_ALL( "IVBContainerControl::elementsChanged" );
  IContainerControl::ObjectCursor  cursor( *this );
  forCursor( cursor )
  {
    IVBContainerObject *cnrObject = (IVBContainerObject*) objectAt( cursor );
    cnrObject->refreshFromObject();
  }
  // end fixed code
#endif

  return *this;
}


template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement>::
      elementAdded ( unsigned long position, const Element&  element )
{
  IContainerObject* cnrObject;
  Collection*       collection;
  Boolean           refreshOn = isRefreshOn( );
  setRefreshOff    ( );

  collection      = items( );

  if ( position == IVBContainerControl<Element,Collection,CnrElement>::firstItem )
    cnrObject = 0;
  else
    cnrObject = objectAt(position - 2);

  addObjectAfter( new CnrElement( *(collection->elementAtPosition( position )) ),
                  cnrObject );

  if (refreshOn)
  {
    setRefreshOn( );
    refresh     ( );
  }

  return *this;
}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement>::
      elementDeleted ( unsigned long position )
{

  Boolean refreshOn = isRefreshOn( );
  setRefreshOff( );
  delete  objectAt( position - 1 );
  if (refreshOn)
  {
    setRefreshOn( );
    refresh     ( );
  }
  return *this;
}


template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement>::
      elementChanged ( unsigned long  position, const Element&  element )
{
#if 0
  // original IBM code
  IContainerObject* cnrObject;
  Boolean           selected;
  Boolean           cursored;
  Boolean           refreshOn = isRefreshOn( );

  setRefreshOff   ( );
  cnrObject = objectAt  ( position - 1 );
  selected  = isSelected( cnrObject );
  cursored  = isCursored( cnrObject );
  removeObject    ( cnrObject );
  elementAdded    ( position,element );

  if ( cursored )
    setCursor( objectAt  ( position - 1 ) );

  if ( selected )
    setSelected( objectAt  ( position - 1 ) );


  if ( refreshOn )
  {
    setRefreshOn( );
    refresh     ( );
  }

  delete cnrObject;
#endif

  // fixed code
  IMODTRACE_ALL( "IVBContainerControl::elementChanged" );
  IVBContainerObject* cnrObject = (IVBContainerObject *) objectAt( position - 1 );
  cnrObject->refreshFromObject();
  // end fixed code

  INotificationEvent  anEvent(
     IVBContainerControl<Element,Collection,CnrElement>::itemChangedId,
    *this,
     true ,
     IEventData((void *)&element) );
  notifyObservers( anEvent );

  return *this;
}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement> :: collectionReplaced( )
{
  INotificationEvent  anEvent(
     IVBContainerControl<Element,Collection,CnrElement>::itemsId,
    *this,
     true ,
     IEventData( (void *)items( ) ) );
  notifyObservers( anEvent );

  return *this;
}

template <class Element, class Collection, class CnrElement>
  unsigned long IVBContainerControl<Element,Collection,CnrElement>::
    selectedCollectionPosition ( )
{
  unsigned long      collectionPosition;
  IVBContainerControl<Element,Collection,CnrElement>::ObjectCursor cursor( *this );
  Boolean            proceed;
  IContainerObject*  cnrObject;

  for ( cursor.setToFirst( ), collectionPosition = 0, proceed = true;
        cursor.isValid   ( ) && proceed;
        cursor.setToNext ( ), collectionPosition++ )
  {
    cnrObject = objectAt( cursor );
    if ( isSelected( cnrObject ) )
      proceed = false;
  }

  if ( proceed )
    collectionPosition =
      IVBContainerControl<Element,Collection,CnrElement>::noSelection;

  return collectionPosition;

}


template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement>::
      select ( unsigned long collectionPosition ,
               Boolean       select )
{
  IContainerObject* cnrObject;

  if ( ( collectionPosition > 0 ) &&
       ( collectionPosition <= IContainerControl::objectCount( ) ) )
  {
    cnrObject = objectAt( collectionPosition - 1 );
    setSelected( cnrObject, select );
  }

  return *this;
}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement>::
      deselect ( unsigned long collectionPosition )
{
  return select( collectionPosition, false );
}

template <class Element, class Collection, class CnrElement>
  Element       IVBContainerControl<Element,Collection,CnrElement>::
    selectedElement ( )
{
  unsigned long                    selectedPosition;
  IPartOrderedCollection<Element>* aCollection = items( );
  IAutoPointer<ICursor>            pCursor( aCollection->newCursor( ), IINIT );

  selectedPosition = selectedCollectionPosition( );
  if ( selectedPosition ==
         IVBContainerControl<Element,Collection,CnrElement>::noSelection )
  {
    ITHROWLIBRARYERROR( IC_NO_SELECTION,
                        IErrorInfo::invalidRequest,
                        IException::recoverable );
  }

  aCollection->setToPosition   ( selectedPosition, *pCursor );
  return aCollection->elementAt( *pCursor );

}

template <class Element, class Collection, class CnrElement>
  CnrElement*  IVBContainerControl<Element,Collection,CnrElement>::
    selectedCnrObject ( )
{
  IContainerControl::ObjectCursor  cursor( *this,
                                           IContainerObject::selected  );

  cursor.setToFirst   ( );
  if ( !cursor.isValid( ) )
  {
    ITHROWLIBRARYERROR( IC_NO_SELECTION,
                        IErrorInfo::invalidRequest,
                        IException::recoverable );
  }

  return (CnrElement *)objectAt( cursor );

}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement>::
      selectedElements ( Collection& elements )
{
  IPartOrderedCollection<Element>* pSequence = items                ( );
  ICursor*                         pCursor   = pSequence->newCursor ( );

  IContainerControl::ObjectCursor  cursor( *this );

  unsigned long index = 0;

  elements.removeAll( );

  forCursor( cursor )
  {
    index++;
    IContainerObject *cnrObject = objectAt( cursor );
    if ( isSelected( cnrObject ) )
      elements.add( pSequence->elementAtPosition( index ) );
  }
  delete pCursor;

  return *this;

}

template <class Element, class Collection, class CnrElement>
  IVBContainerControl<Element,Collection,CnrElement>&
    IVBContainerControl<Element,Collection,CnrElement>::
      selectedCnrObjects  ( IVSequence<CnrElement *>& cnrObjects )
{
  IPartOrderedCollection<Element>* pSequence = items                ( );
  ICursor*                         pCursor   = pSequence->newCursor ( );

  IContainerControl::ObjectCursor  cursor( *this,
                                           IContainerObject::selected  );

  cnrObjects.removeAll( );

  forCursor( cursor )
    cnrObjects.add( (CnrElement *)objectAt( cursor ) );

  delete pCursor;

  return *this;
}

template <class Element, class Collection, class CnrElement>
  unsigned long IVBContainerControl<Element,Collection,CnrElement>::
    numberOfSelections ( ) const
{
  unsigned long                    count = 0;
  IContainerControl::ObjectCursor  cursor( *this,
                                           IContainerObject::selected  );
  forCursor ( cursor )
    count++;

  return count;
}


template <class Element, class Collection, class CnrElement>
  void IVBContainerControl<Element,Collection,CnrElement>::
    initialize ( )
{
  fVBContainerControlData = new
      IVBContainerControlData< Element,
                               IVBContainerControl<Element,
                                                  Collection,
                                                  CnrElement >,
                               Collection >( );
  fVBContainerControlData->fCollectionViewData->fCollectionObserver.setViewer( this );
  enableNotification     ( );
  setDeleteObjectsOnClose( );
}



template <class Element, class Collection, class CnrElement>
  const unsigned long
    IVBContainerControl<Element,Collection,CnrElement>::firstItem   = 1;
template <class Element, class Collection, class CnrElement>
  const unsigned long
    IVBContainerControl<Element,Collection,CnrElement>::noSelection = 0;

template <class Element, class Collection, class CnrElement>
  INotificationId const IVBContainerControl<Element,Collection,CnrElement>::itemChangedId =
    "IVBContainerControl::itemChangedId";
template <class Element, class Collection, class CnrElement>
  INotificationId const IVBContainerControl<Element,Collection,CnrElement>::itemsId =
    "IVBContainerControl::itemsId";
