/***************************************************************************
 * File...... Editor.ipp
 * Author.... Mat
 * Date...... 4/2/98
 *
 * Inline functions for Editor
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/


inline IString Editor::info() const
{
  return _info;
}


inline Boolean Editor::isModified() const
{
  return _isModified;
}


inline Boolean Editor::hasSelection() const
{
  return _hasSelection;
}


inline Boolean Editor::canEditProperties() const
{
  return _canEditProperties;
}

