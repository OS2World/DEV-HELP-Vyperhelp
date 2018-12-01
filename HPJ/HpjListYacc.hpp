/***************************************************************************
 * File...... HpjListYacc.hpp
 * Author.... Mat
 * Date...... 12/23/95
 *
 * Parser for a list of files as #included in the [FILES] section of an HPJ
 * file.  The parseTo function will add the found files to the file list of
 * the specified WinHelpParser.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "EnhancedYacc.hpp"
class WinHelpParser;

class HpjListYacc : public EnhancedYacc
{
  public:
    // constructor
    SSConstr            HpjListYacc( const char * fileToParse );

    // from SSYacc
    SSYaccStackElement* reduce( SSUnsigned32 production, SSUnsigned32 size );

    // parser
    SSBooleanValue      parseTo( WinHelpParser & whp );

  protected:
    WinHelpParser * _whp;
};

