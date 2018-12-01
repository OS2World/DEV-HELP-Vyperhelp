/***************************************************************************
 * File...... CDefineYacc.hpp
 * Author.... Mat
 * Date...... 12/21/95
 *
 * Parser for #define and nested #include directives.  Currently does
 * minimal C pre-processing, enough to get at the #defines.
 *
 * DOES support:
 *   #define of simple integer constants (octal, hex, or decimal)
 *   U or L suffix for integer constants
 *   single line comments
 *   multi-line comments
 *   quoted strings (contents of strings are not parsed)
 *   nested #includes
 *
 * Does NOT support:
 *   #if, #ifdef, etc
 *   expressions (e.g., "#define NAME2 (NAME1 + 1)" )
 *   split lines using "\" as continuation character
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "EnhancedYacc.hpp"
class Parser;

class CDefineYacc : public EnhancedYacc
{
  public:
    // constructor
    SSConstr            CDefineYacc( const char * fileToParse, const IString & homeDir );

    // from SSYacc
    SSYaccStackElement* reduce( SSUnsigned32 production, SSUnsigned32 size );

    // parser
    SSBooleanValue      parseTo( Parser & parser );

  private:
    long int  _value;
    Parser *  _parser;
    IString   _homeDir;
};

