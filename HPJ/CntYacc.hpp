/***************************************************************************
 * File...... CntYacc.hpp
 * Author.... Mat
 * Date...... 1/8/99
 *
 * CNT file parser.  The CNT file gives ordering and title information for
 * a WinHelp project.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef CNTYACC_HPP
#define CNTYACC_HPP

// Generator
#include "EnhancedYacc.hpp"
#include "Filename.hpp"
class Generator;

// WinHelpParser
class WinHelpParser;


/***************************************************************************
 * Class...... CntYacc
 * Author..... Mat
 * Date....... 12/21/95
 *
 * Derived from SSYacc to handle parsing of a CNT file.  The parseTo
 * function takes references to a Generator and a WinHelpParser and
 * manipulates them as it gets info from parsing.
 ***************************************************************************/
class CntYacc : public EnhancedYacc
{
  public:
    // constructor
    SSConstr            CntYacc( const char * fileToParse, Boolean isPrimary = true );

    // from SSYacc
    virtual SSBooleanValue      error( SSUnsigned32 qulState, SSLexLexeme & qLexeme);
    virtual SSYaccStackElement* reduce( SSUnsigned32 production , SSUnsigned32 size );

    // parser
    SSBooleanValue      parseTo( Generator & gen, WinHelpParser & whp );

  private:
    // helpers
    void handleSection( SSUnsigned32 level );
    void warning( const IString & text, const SSLexLexeme * lexeme = 0 );

    // collaborators
    Generator *         _gen;
    WinHelpParser *     _whp;

    // constructor data
    Boolean  _isPrimary;
    Filename _filename;

    // parsing state
    IString _sectionTitle;
    IString _sectionTopic;
};

#endif

