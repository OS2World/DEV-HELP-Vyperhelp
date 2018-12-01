/***************************************************************************
 * File...... WinHelpParser.hpp
 * Author.... Mat
 * Date...... 12/23/95
 *
 * WinHelpParser holds all information about a Windows Help Project, parsed
 * from an HPJ file by a HpjYacc parser.  The generate member function is
 * used to convert the WinHelpParser using a Generator.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef WINHELPPARSER_HPP
#define WINHELPPARSER_HPP

// OpenClass
#include <istring.hpp>

// Generator
#include "Parser.hpp"
#include "ParserIndicator.hpp"

// WinHelp
#include "Filename.hpp"
class IdentifierSet;
class RtfSection;
class RtfSectionList;


#define MAX_FONTSIZE_MAP 5  // maximum number of FontSizeMaps per project

struct FontSizeMap
{
  int minPoints;
  int maxPoints;
  int targetPoints;
};


class _Export WinHelpParser: public Parser
{
  public:
    // constructor
    WinHelpParser( const IString & hpjFile );
    virtual ~WinHelpParser();

    // setters
    WinHelpParser
      & setActiveFile( const Filename & filename ),
      & setTitle( const IString & title ),
      & setSeparators( const IString & separators ),
      & setRootPath( const IString & rootPath ),
      & setBitmapPath( const IString & bitmapPath ),
      & setCntFile( const IString & filename ),
      & setHlpFile( const IString & filename ),
      & addBitmapFile( const IString & bmpFile ),
      & addRtfFile( const IString & rtfFile ),
      & addFontSizeMap( const FontSizeMap & fontSizeMap ),
      & addAlias( const IString & contextName1, const IString & contextName2 ),
      & setContents( const IString & contextName ),
      & setForceFont( const IString & fontName ),
      & addWindow( const IString & windowName, int id );

    // accessors
    char            separator() const;
    const IString & forceFont() const;
    int             getWindowId( const IString & windowName ) const;
    Boolean         getBitmapId( const IString & filename, IString & id );
    Boolean         isUniqueContext( const IString & contextName, int & id );
    Boolean         isContents( const IString & contextName );
    int             numberOfAliases( const IString & contextName ) const;
    Boolean         getAlias( const IString & contextName, int index, IString & aliasName ) const;
    int             mapFontSize( int sizeInTwips ) const;

    // section ID parser
    Boolean parseTopicId( IString & topic, IString * filename, IString * window );

    // section management
    Boolean addSection( RtfSection * section );
    void sendSection( unsigned level, const IString & title, const IString & topic );

    // user feedback
    void warning( const IString & text, unsigned line = 0, unsigned col = 0 );

    // from Parser
    virtual void generate( Generator & gen );
    virtual void setId( const IString & contextName, int id );

  private:
    // helpers
    void sendContents( Boolean fullContents );
    IString fixPath( const IString & path );

    // data
    int              _numFontSizeMaps;
    FontSizeMap      _map[ MAX_FONTSIZE_MAP ];
    IString          _contentsContext;  // "*" for first panel, empty for none
    IString          _rootPath;
    IString          _bitmapPath;
    FilenameList     _bitmaps;      // bitmaps specified in the HPJ
    FilenameList     _bitmapsUsed;  // bitmaps used in the RTF
    FilenameList     _rtfs;
    IdentifierSet *  _windows;
    IdentifierSet *  _contexts;
    Filename         _hpjFile;
    Generator *      _gen;
    IString          _activeFile;
    char             _separator;
    IString          _forceFont;
    Filename         _cntFile;
    Filename         _hlpFile;
    IString          _title;
    Boolean          _titleSent;
    RtfSectionList * _sections;
    unsigned         _level;
};


// inline functions
#include "WinHelpParser.ipp"


#endif

