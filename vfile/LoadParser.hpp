/***************************************************************************
 * File...... LoadParser.hpp
 * Author.... Mat
 * Date...... 11/20/97
 *
 * Loads a binary VYD file and parses it into Gins for a Generator.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef LOADPARSER_HPP
#define LOADPARSER_HPP

// Generator
#include "Parser.hpp"
#include "Gin.hpp"

// VFile
#include "InputBinaryFile.hpp"
class LoadGenerator;


class _Export LoadParser: public Parser
{
  public:
    // constructor
    LoadParser( const IString & filename, int offset = -1 );
    LoadParser( istream & stream, int offset = -1 );
      // if offset other than -1 is given, then it is the offset of a section
      //    and only that section will be parsed

    // coordination with LoadGenerator
    void load( LoadGenerator & gen );
    void loadSection( Generator & gen );

    // special generator to process only section contents (no SectionGin)
    void generateSection( Generator & gen );

    // from Parser
    virtual void generate( Generator & gen );

  private:
    // Gin creators
    void loadSection( Boolean includeHeader );
    void loadKeyword();
    void loadLabel();
    void loadRow();
    void loadColumn();
    void loadItem();
    void loadPara();
    void loadLine();
    void loadPush();
    void loadPop();
    void loadRightMargin();
    void loadLeftMargin();
    void loadBorder();
    void loadAlign();
    void loadSpacing();
    void loadRuler();
    void loadOutline();
    void loadDivider();
    void loadFont();
    void loadSize();
    void loadColor();
    void loadBackColor();
    void loadBold();
    void loadItalic();
    void loadUnderline();
    void loadComment();
    void loadLink();
    void loadHide();
    void loadArt();
    void loadText();
    void loadSymbol();

    // helpers
    void       loadDocument();
    IString    readArtId();
    SectionGin readSection();
    Distance   readDistance();
    Position   readPosition();
    IColor     readColor();
    void       initialize();

    // data
    InputBinaryFile  _input;
    short            _version;
    unsigned         _sectionOffset;
    Generator *      _gen;
    Boolean          _sectionOnly;
};


#endif

