/***************************************************************************
 * File...... RtfContext.hpp
 * Author.... Mat
 * Date...... 2/24/98
 *
 * Holds an RTF context, including styles and destination.  Works with
 * RtfYacc during parsing of an RTF file.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef RTFCONTEXT_HPP
#define RTFCONTEXT_HPP

// Generator
#include "Generator.hpp"

// WinHelpParser
class RtfYacc;

// constants
#define NO_INDEX -1


class RtfContext
{
  public:
    // constructor
    RtfContext( RtfYacc * yacc );

    // reset functions
    void reset();
    void resetCharacterStyles();
    void resetParagraphStyles();

    // destination types
    enum Destination
    {
      document,    // RTF destinations
      unknown,
      footnote,
      fonttable,
      colortable,
      hidden,      // WinHelp document psuedo-destinations
      target,
      bitmap,
      multimedia
    };
    void        setDestination( Destination destination );
    Destination destination() const;

    // style setters
    void setRightMargin( int rightMarginTwips );
    void setLeftMargin( int leftMarginTwips );
    void setAlignment( AlignGin::Alignment alignment );
    void setSpacing( int beforeTwips, int afterTwips );
    void setFont( int fontIndex );
    void setSize( int fontHalfPoints );
    void setColor( int colorKey );
    void setBackColor( int backColorKey );
    void setBold( Boolean isBold );
    void setItalic( Boolean isItalic );

    // Gin creator
    void sendAll( const RtfContext * other = 0 ) const;

  private:
    // Gin creators
    void sendRightMargin() const;
    void sendLeftMargin() const;
    void sendAlignment() const;
    void sendSpacing() const;
    void sendFont() const;
    void sendSize() const;
    void sendColor() const;
    void sendBackColor() const;
    void sendBold() const;
    void sendItalic() const;

    // yacc collaborator
    RtfYacc *           _yacc;

    // destination
    Destination         _destination;

    // style settings
    int                 _rightMarginTwips;
    int                 _leftMarginTwips;
    AlignGin::Alignment _alignment;
    int                 _afterTwips;   // afterTwips + beforeTwips = spacing
    int                 _beforeTwips;
    int                 _fontIndex;
    int                 _fontHalfPoints;
    int                 _colorKey;
    int                 _backColorKey;
    Boolean             _isBold;
    Boolean             _isItalic;

};


#endif

