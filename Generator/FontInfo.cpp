/***************************************************************************
 * File...... FontInfo.cpp
 * Author.... Mat
 * Date...... 2/20/97
 *
 * Implementation of FontInfo
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <ifont.hpp>        // IFont
#include <igrafctx.hpp>     // IGraphicContext
#include <itrace.hpp>       // ITRACE macros
#include <icursor.h>        // ICursor (for iteration)
#include <istdops.h>        // standard hash functions

// Generator
#include "CodePage.hpp"
#include "FontDefinition.hpp"
#include "FontInfo.hpp"

// Local data
// font variants
static const char boldVariant[] =   " Bold";
static const char italicVariant[] = " Italic";

// font & family names
static const char fontTimesNew[] =    "Times New Roman";
static const char fontTimes[] =       "Times";
static const char fontTms[] =         "Tms Rmn";
static const char fontHelvetica[] =   "Helvetica";
static const char fontHelv[] =        "Helv";
static const char fontCourier[] =     "Courier";
static const char fontCourierNew[] =  "Courier New";
static const char fontArial[] =       "Arial";
static const char fontRoman[] =       "Roman";
static const char fontSwiss[] =       "Swiss";
static const char fontGeneva[] =      "Geneva";
static const char fontMSSans[] =      "MS Sans Serif";
static const char fontMSSerif[] =     "MS Serif";



FontInfo::FontInfo():
    _pointSize( 0 ),  // 0 = default
    _flags( 0 )
{
  setVector( checkVector( _name ) );
}


FontInfo::FontInfo( const IString & facename, unsigned long pointSize ):
    _name( facename ),
    _pointSize( pointSize ),  // 0 = default
    _flags( 0 )
{
  removeVariants();
}


FontInfo::FontInfo( const IFont & font ):
    _name( font.name() ),
    _pointSize( font.pointSize() ),
    _flags( 0 )
{
  setBold( font.isBold() );
  setItalic( font.isItalic() );
  setUnderline( font.isUnderscore() );
  removeVariants();
}


int FontInfo::operator==( const FontInfo & font ) const
{
  return ( _name        == font._name )
      && ( _pointSize   == font._pointSize )
      && ( _flags       == font._flags );
}


int FontInfo::operator!=( const FontInfo & font ) const
{
  return ! operator==( font );
}



/***************************************************************************
 * Procedure.. FontInfo::selectFamily
 * Author..... Mat
 * Date....... 12/29/98
 *
 * Given a font or family name, this routine checks for common names and
 * returns the family, if known.  If the name given is not a common one,
 * this function returns false.
 ***************************************************************************/
Boolean FontInfo::selectFamily( const IString & name, Family & family ) const
{
  Boolean match = true;

  if ( name == fontCourier
    || name == fontCourierNew )
    family = mono;
  else if ( name == fontTimesNew
    || name == fontTimes
    || name == fontRoman
    || name == fontMSSerif
    || name == fontTms )
    family = roman;
  else if ( name == fontHelvetica
    || name == fontSwiss
    || name == fontArial
    || name == fontGeneva
    || name == fontMSSans
    || name == fontHelv )
    family = swiss;
  else
    match = false;

  return match;
}


FontInfo::Family FontInfo::family() const
{
  Boolean match;
  Family family;

  // first check if the given font name is known
  match = selectFamily( _name, family );
  if ( match )
    return family;

  // get family name from OS/2 font metrics
  Boolean isMono;
  IString familyName;
  const FontDefinition * fd = FontDefinition::locate( _name );
  if ( fd )
  {
    // use FontDefinition info (faster)
    isMono = fd->isFixed();
    familyName = fd->family();
  }
  else
  {
    // use default
    return system;
  }

  // check for mono-spaced font
  if ( isMono )
    return mono;

  // check for a known family name
  match = selectFamily( familyName, family );
  if ( match )
    return family;

  // last resort -- default is "system"
  return system;
}


void FontInfo::resetName( Family family )
{
  switch ( family )
  {
    case system:
      _name = IString();  // empty string for default font
      break;
    case roman:
      _name = fontTimesNew;
      break;
    case swiss:
      _name = fontHelvetica;
      break;
    case mono:
      _name = fontCourier;
      break;
  }
  setVector( checkVector( _name ) );
}


void FontInfo::setFont( const IString & name, unsigned long pointSize )
{
  _name = name;
  _pointSize = pointSize;
  removeVariants();
}


/***************************************************************************
 * Procedure.. FontInfo::newFont
 * Author..... Mat
 * Date....... 2/27/97
 *
 * This method creates an IFont from the FontInfo.  There are problems with
 * IBM's IFont, so you have to be careful how you create them if you want
 * to get proper metrics.  The following was arrived at by trial and error
 * and by inspecting the source to IFont.cpp which was found on DevCon
 * volume 12 at h:\services\vacppcsd\cts304\iocsrc\cppoou3\ifont.cpp
 *
 * If there is a specific font design for Bold or Italic variants,
 * then use that instead of letting it be simulated.
 ***************************************************************************/
IFont * FontInfo::newFont( IGraphicContext & context ) const
{
  // save the context's current font
  IFont contextFont = context.currentFont();

  // set default font name or point size if none specified
  IString name = _name;
  unsigned long pointSize = _pointSize;
  Boolean needDefaultName = ( name.length() == 0 );
  if ( needDefaultName || ! pointSize )
  {
    IFont defaultFont;
    if ( needDefaultName )
      name = defaultFont.name();
    if ( ! pointSize )
      pointSize = defaultFont.pointSize();
  }

  // check for a "Bold" variant
  Boolean useBold = isBold();
  if ( useBold )
  {
    if ( addVariant( name, boldVariant, context ) )
      useBold = false;
  }

  // check for a "Italic" variant
  Boolean useItalic = isItalic();
  if ( useItalic )
  {
    if ( addVariant( name, italicVariant, context ) )
      useItalic = false;
  }

  Boolean useUnderline = isUnderline();
  Boolean useVector = checkVector( name );

  // create a font in given presentation space
  IFont * newFont = new IFont( name, pointSize, false, useVector, context.handle() );

  // apply the font attributes
  if ( useBold )
    newFont->setBold( isBold() );
  if ( useItalic )
    newFont->setItalic( isItalic() );
  if ( useUnderline )
    newFont->setUnderscore( isUnderline() );

  // set name and size again to trigger reloading of metrics and char widths
  newFont->setName( name, context.handle() );
  newFont->setPointSize( pointSize, context.handle() );

  // reset the context's font
  CodePage::setContextFont( context, contextFont );

  return newFont;
}


// returns true if variant exists
Boolean FontInfo::addVariant( IString & name, const IString & variant, const IGraphicContext & context ) const
{
  // create variant name
  IString variantName = name + variant;

  // use cursor to search for match
  IFont::FaceNameCursor faces( IFont::FaceNameCursor::both, context.handle() );
  forCursor( faces )
  {
    if ( variantName == IFont::faceNameAt( faces ) )
    {
      name = variantName;
      return true;
    }
  }

  // no match found!
  return false;
}


void FontInfo::removeVariants()
{
  Boolean changed = false;

  // remove bold variant and set bold flag instead
  if ( _name.indexOfPhrase( boldVariant ) )
  {
    _name.change( boldVariant, "" );
    setBold( changed = true );
  }

  // remove italic variant and set italic flag instead
  if ( _name.indexOfPhrase( italicVariant ) )
  {
    _name.change( italicVariant, "" );
    setItalic( changed = true );
  }

  if ( changed )
    setVector( checkVector( _name ) );

}


// determine if the font name is available in vector form
Boolean FontInfo::checkVector( const IString & name )
{
  const FontDefinition * fd = FontDefinition::locate( name );
  if ( fd )
    return fd->isVector();

  return false;
}


// return true if the given font is installed on this system
Boolean FontInfo::isInstalled() const
{
  return ( _name.length() && ( FontDefinition::locate( _name ) != 0 ) );
}


// hash function for use in a hash collection
unsigned long FontInfo::hash( unsigned long modulo ) const
{
  return ( _flags + _pointSize + ::hash( (char *)_name, modulo ) ) % modulo;
}


void FontInfo::dumpMetrics( IGraphicContext & context ) const
{
#ifdef DUMP_FONTS
  IFont * font = newFont( context );
  ITRACE_ALL( font->name() + IString(".") + IString(font->pointSize())
            + IString(": m=") + IString(font->charWidth('m'))
            + IString(", ascender=") + IString(font->maxAscender())
            + IString(", descender=") + IString(font->maxDescender()) );
  delete font;
#endif
}


IString FontInfo::asString() const
{
  return _name + IString(".") + IString(_pointSize);
}


