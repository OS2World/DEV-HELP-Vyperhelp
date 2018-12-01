/***************************************************************************
 * File...... EditorTrigger.cpp
 * Author.... Mat
 * Date...... 9/25/97
 *
 * Implementation of EditorTrigger
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// TextEditor
#include "Editor.hpp"
#include "EditorTrigger.hpp"


EditorTrigger::EditorTrigger( Editor & editor ):
  _editor( editor )
{}


void EditorTrigger::timerExpired ( unsigned long timerId )
{
  _editor.triggerAction();
}


