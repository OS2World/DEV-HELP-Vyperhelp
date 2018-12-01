/***************************************************************************
 * File...... CodePage.hpp
 * Author.... Mat
 * Date...... 12/18/97
 *
 * Represents a "code page" (also know as character set), which is a defined
 * mapping of byte values to characters/glyphs.  This class is intended to
 * encapsulate all platform-dependent aspects of code page support.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef CODEPAGE_HPP
#define CODEPAGE_HPP

// OpenClass
class IString;
class IGraphicContext;
class IFont;


class _Export CodePage
{
  public:
    // enumerator (<0 for non-IBM character sets)
    enum Identifier
    {
      ansi    =  1004,
      windows =  ansi,
      latin1  =  819,
      unicode =  1200,
      unknown = -1,
      symbol  = -998,
      mac     = -999
    };

    // constructors
    CodePage();
    CodePage( Identifier id );
    CodePage( long cp );

    // conversion to long
    operator long() const;

    // translators
    unsigned char translate( unsigned char ch, CodePage from ) const;
    void translateString( IString & string, CodePage from ) const;
    static unsigned char substitution;

    // active code page
    static CodePage activeCodePage();

    // graphic context support
    static void setContextFont( IGraphicContext & context, IFont & font );

  private:
    // helpers
    static long getActiveCodePage();

    // class data
    static long _activeCodePage;

    // data for this CodePage
    long _cp;
};


// inline functions
#include "CodePage.ipp"


#endif

