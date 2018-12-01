/***************************************************************************
 * File...... SubjectView.hpp
 * Author.... Mat
 * Date...... 9/9/97
 *
 * SubjectViews are Views that have a corresponding subject item.  It
 * supports a notifier/observer or model/view protocol, designed around
 * INotifier and IObserver.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef SUBJECTVIEW_HPP
#define SUBJECTVIEW_HPP

// OpenClass
#include <ievtdata.hpp>     // IEventData
#include <inotify.hpp>      // INotificationId

// TextEditor
#include "View.hpp"
class Item;


class _Export SubjectView: public View
{
  public:
    // constructor
    SubjectView( Item * subject, View * parent );
      // parent is NULL for the top level View
    virtual ~SubjectView();

    // subject support
    virtual Item * subject() const;
      // default returns the subject provided in constructor

    // from View
    virtual unsigned remove( ViewCursor * fromCursor, ViewCursor * toCursor );

    // IObserver emulation
    virtual void dispatchNotificationEvent( INotificationId id, const IEventData & event );

  private:
    Item * _subject;
};


// inline functions
#include "SubjectView.ipp"


#endif

