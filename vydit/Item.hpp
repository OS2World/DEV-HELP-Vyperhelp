/***************************************************************************
 * File...... Item.hpp
 * Author.... Mat
 * Date...... 2/1/96
 *
 * Item holds all information about the logical component of the item.
 * The corresponding View holds visual info.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef ITEM_HPP
#define ITEM_HPP

// OpenClass
#include <ievtdata.hpp>     // IEventData
#include <inotify.hpp>      // INotificationId

// Generator
class Generator;

// TextEditor
class View;
class SubjectView;
class ItemCursor;
class WordItem;
class FlowItem;
class PushItem;
class PopItem;
class ArtItem;


class _Export Item
{
  public:
    // CONSTRUCTOR
    Item( Item * parent = 0 );
    virtual ~Item();

    // RELATIONSHIPS
    Item * parent() const;
    virtual void setParent( Item * parent );
    virtual void change( Item * child = 0 );
      // called when a child has been changed
    virtual ItemCursor * newCursor();
    virtual View *       newView( View * parent ) = 0;
    virtual void         removeChild( Item * item );

    // CLASS IDENTIFICATION
    // returns 0 if not the specified class
    virtual WordItem * asWord();
    virtual FlowItem * asFlow();
    virtual PushItem * asPush();
    virtual PopItem *  asPop();
    virtual ArtItem *  asArt();

    // PROPERTIES TYPE
    // returns the appropriate notification ID for Editor to send
    virtual INotificationId editPropertiesId() const;

    // NOTIFICATION SUPPORT
    // subset of full INotification support (too expensive for now)
    // handles only one observer!
    void notifyObservers( INotificationId id, const IEventData & event = IEventData() );
    void addObserver( SubjectView & view );
    void removeObserver( SubjectView  & view );
    static const INotificationId deleteId;
    static const INotificationId changeId;

    // STORAGE
    virtual void generate() = 0;

    // DEBUGGING
    virtual IString dumpString() const = 0;

  private:
    Item *        _parent;
    SubjectView * _observer;
};


// inline functions
#include "Item.ipp"


#endif

