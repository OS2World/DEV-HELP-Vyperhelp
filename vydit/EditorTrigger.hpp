/***************************************************************************
 * File...... EditorTrigger.hpp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Timer function for the editor to support batching of input keystrokes.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef EDITORTRIGGER_HPP
#define EDITORTRIGGER_HPP

// OpenClass
#include <itimer.hpp>

// TextEditor
class Editor;


class EditorTrigger: public ITimerFn
{
  public:
    // constructor
    EditorTrigger( Editor & editor );

    // ITimerFn
    virtual void timerExpired ( unsigned long timerId );

  private:
    Editor & _editor;
};

#endif

