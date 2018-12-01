/***************************************************************************
 * File...... vport.hpp
 * Author.... Mat
 * Date...... 5/16/96
 *
 * TextEditor ViewPort: allows scrolling of Editor edit window.
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/
#ifndef VPORT_HPP
#define VPORT_HPP

// OpenClass
#include <ivport.hpp>       // IViewPort
#include <isizehdr.hpp>     // IResizeHandler
#include <ikeyhdr.hpp>      // IKeyboardHandler
#include <iobservr.hpp>     // IObserver

// TextEditor
#include "Editor.hpp"
class FlowItem;


class VPort: public IViewPort, public IResizeHandler, public IKeyboardHandler, public IObserver
{
  public:
    // constructor
    VPort( unsigned long windowId, IWindow * owner, FlowItem & flow );
    virtual ~VPort();

  protected:
    // IResizeHandler
    virtual Boolean windowResize( IResizeEvent &event );

    // IKeyboardHandler
    virtual Boolean virtualKeyPress( IKeyboardEvent &event );

    // IObserver
    virtual IObserver & dispatchNotificationEvent( const INotificationEvent & event );

  private:
    // keyboard actions
    void insertHelpText();
    void paste();
    void importFile();
    void importBitmap();

    FlowItem & _flow;
    Editor     _editor;
};

#endif
