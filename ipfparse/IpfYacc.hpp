/***************************************************************************
 * File...... IpfYacc.hpp
 * Author.... Mat
 *
 * IPF Parser.  Includes a parseTo function which will parse the supplied
 * IPF file to a Generator.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef IPFYACC_HPP
#define IPFYACC_HPP

// Generator
#include "EnhancedYacc.hpp"
#include "Gin.hpp"

// IPF Parser
class IpfParser;


/***************************************************************************
 * Class...... IpfYacc
 * Author..... Mat
 *
 * Derived from SSYacc to handle parsing of an IPF file.  The parseTo
 * function takes references to a IpfParser and manipulates it as it gets
 * info from parsing.
 ***************************************************************************/
class IpfYacc : public EnhancedYacc
{
  public:
    // constructor
    SSConstr             IpfYacc( const char * fileToParse, Boolean isTopLevel = true );
    SSConstr             IpfYacc( SSLexConsumer & consumer );

    // parser
    SSBooleanValue       parseTo( IpfParser & parser, int pass );

    // from SSYacc
    SSYaccStackElement * reduce( SSUnsigned32 production , SSUnsigned32 size );
    SSBooleanValue       error( SSUnsigned32 qulState, SSLexLexeme & qLexeme );

  private:
    // enumerations
    enum LinkType
    {
      unknownLink,
      headingLink,
      footnoteLink,
      launchLink,
      informLink
    };
    enum BarType
    {
      emptyBar,
      sysmenuBar,
      minmaxBar,
      fullBar,
      noBar
    };

    // pasrer nesting level
    Boolean _isTopLevel;

    // pass 1
    void prepareSection();
    void prepareIndex();
    void prepareSynonym();
    int _pass;
    void pass1( SSUnsigned32 production );
    void pass2( SSUnsigned32 production );
    void common( SSUnsigned32 production );

    // helpers
    unsigned lexemeAsTocLevel();
    IString  lexemeAsParameter();
    void     setValue( IString & setting );
    void     handleSection( int level );
    void     handlePop();
    void     handleIndex( Boolean isPrimary );
    void     handleLink();
    void     handleReference();
    void     handleTable();
    void     handleSpacing();
    void     handleFont();
    void     handleDoubleList( RulerGin::LineBreak & lineBreak, RulerGin::LineBreak defaultLineBreak );
    void     handleNewLine( Boolean isEmpty = false );
    void     handleSymbol( const Symbol & symbol );
    void     handleSymbol( const IString & symbolName, Boolean isExtra = false );
    void     handleText( const IString & text );
    void     handleBreak();
    void     handleHide();
    void     handleNameit();
    void     handleArtwork();
    void     beginCentered();
    void     endCentered();
    void     warning( const IString & text, const SSLexLexeme * lexeme = 0 );

    // collaborators
    IpfParser * _parser;

    // attribute data
    IString              _artFile;
    ArtGin::ArtSpot      _artSpot;
    IString              _cautionText;
    IColor               _color;
    Boolean              _isColorSet;
    RulerGin::LineBreak  _dlLineBreak;
    Boolean              _isCompact;
    int                  _termSizeChars;
    IString              _facename;
    int                  _codepage;
    unsigned             _pointSize;
    Boolean              _isHidden;
    unsigned             _id;
    IString              _name;
    IString              _name2;
    Boolean              _isExternal;
    unsigned             _tocLevel;
    unsigned             _sectionTocLevel;
    Position             _x;
    Position             _y;
    Distance             _width;
    Distance             _height;
    WindowDef::Border    _border;
    WindowDef::Scrolling _scrolling;
    BarType              _bar;
    Boolean              _isWindowSet;
    AlignGin::Alignment  _alignment;
    IString              _roots;
    LinkType             _linkType;
    Boolean              _isAuto;
    IString              _externalFile;
    unsigned             _margin;
    IString              _noteText;
    RulerGin::LineBreak  _parmlLineBreak;
    int                  _borders;
    IString              _columns;
    IString              _warningText;
    IString              _nameitText;
    IString              _value;
    Position             _position;
    Distance             _distance;
    IString              _extra;

    // defaults
    static ArtGin::ArtSpot      _defaultArtSpot;
    static IString              _defaultCautionText;
    static RulerGin::LineBreak  _defaultDlLineBreak;
    static int                  _defaultTermSizeChars;
    static unsigned             _defaultTocLevel;
    static Position             _defaultX;
    static Position             _defaultY;
    static Distance             _defaultWidth;
    static Distance             _defaultHeight;
    static WindowDef::Border    _defaultBorder;
    static WindowDef::Scrolling _defaultScrolling;
    static BarType              _defaultBar;
    static AlignGin::Alignment  _defaultAlignment;
    static LinkType             _defaultLinkType;
    static unsigned             _defaultMargin;
    static IString              _defaultNoteText;
    static RulerGin::LineBreak  _defaultParmlLineBreak;
    static int                  _defaultBorders;
    static IString              _defaultWarningText;
};


#endif
