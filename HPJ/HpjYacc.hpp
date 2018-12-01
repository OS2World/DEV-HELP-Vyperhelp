/***************************************************************************
 * File...... HpjYacc.hpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * HPJ Parser.  Includes a parseTo function which will parse the supplied
 * HPJ file (along with the listed RTF files) to a Generator.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef HPJYACC_HPP
#define HPJYACC_HPP

// Generator
#include "EnhancedYacc.hpp"
class Generator;

// WinHelpParser
class WinHelpParser;
typedef unsigned long int ULong;


/***************************************************************************
 * Class...... HpjYacc
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Derived from SSYacc to handle parsing of an HPJ file.  The parseTo
 * function takes references to a Generator and a WinHelpParser and
 * manipulates them as it gets info from parsing.
 ***************************************************************************/
class HpjYacc : public EnhancedYacc
{
  public:
    // constructor
    SSConstr            HpjYacc( const char * fileToParse );

    // from SSYacc
    virtual SSBooleanValue      error( SSUnsigned32 qulState, SSLexLexeme & qLexeme);
    virtual SSYaccStackElement* reduce( SSUnsigned32 production , SSUnsigned32 size );

    // parser
    SSBooleanValue      parseTo( Generator & gen, WinHelpParser & whp );

  private:
    // helper functions
    void resetWindow();
    void parseFontSizeMap( const IString & mapString );
    void warning( const IString & text, const SSLexLexeme * lexeme );
    void setReplaceString( const IString & string );
    IString replacePath( const IString & string );

    // collaborators
    Generator *         _gen;
    WinHelpParser *     _whp;

    // window settings
    ULong               _winId;
    IString             _winCaption;
    Boolean             _isWinMaximized;
    ULong               _winXPercent;
    ULong               _winYPercent;
    ULong               _winWidthPercent;
    ULong               _winHeightPercent;

    // path replacement
    IString             _replaceOld;
    IString             _replaceNew;

    // home directory
    IString             _homeDir;
};

#endif

