/***************************************************************************
 * File...... FontInfo.hpp
 * Author.... Mat
 * Date...... 2/20/97
 *
 * Holds information about a font, including name, size and attributes.
 * Also has methods to convert to or from an IFont.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef FONTINFO_HPP
#define FONTINFO_HPP

// OpenClass
#include <istring.hpp>
class IFont;
class IGraphicContext;


class _Export FontInfo
{
  public:
    // enumerators
    enum Family
    {
      system,
      roman,
      swiss,
      mono
    };

    // constructors
    FontInfo();
    FontInfo( const IString & facename, unsigned long pointSize = 0 );
    FontInfo( const IFont & font );

    // comparison
    int operator==( const FontInfo & font ) const;
    int operator!=( const FontInfo & font ) const;

    // query functions
    const IString & name() const;
    unsigned long   pointSize() const;
    Boolean         isBold() const;
    Boolean         isItalic() const;
    Boolean         isUnderline() const;
    Family          family() const;
    Boolean         isInstalled() const;

    // setter functions
    void setFont( const IString & name, unsigned long pointSize );
    void setBold( Boolean isBold );
    void setItalic( Boolean isItalic );
    void setUnderline( Boolean isUnderline );
    void resetName( Family family );
      // reset to standard name based on given family

    // IFont conversion
    IFont * newFont( IGraphicContext & context ) const;

    // hash function
    unsigned long hash( unsigned long modulo ) const;

    // diagnostic
    void            dumpMetrics( IGraphicContext & context ) const;
    virtual IString asString() const;

  private:
    // helpers
    Boolean selectFamily( const IString & name, Family & family ) const;
    void    removeVariants();
    Boolean addVariant( IString & name, const IString & variant, const IGraphicContext & context ) const;
    void    setVector( Boolean isVector );
    static Boolean checkVector( const IString & name );

    // data
    IString       _name;        // empty string = default
    unsigned long _pointSize;   // 0 = default

    // flags
    enum
    {
      vector     = 0x01,  // set for scalable fonts
      bold       = 0x02,
      italic     = 0x04,
      underline  = 0x08
    };
    unsigned _flags;
};


// inline functions
#include "FontInfo.ipp"


#endif

