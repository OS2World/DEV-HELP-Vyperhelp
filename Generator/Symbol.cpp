/***************************************************************************
 * File...... Symbol.cpp
 * Author.... Mat
 * Date...... 3/27/97
 *
 * Implementation of Symbol.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

// OpenClass
#include <istring.hpp>      // IString
#include <iexcept.hpp>      // IASSERT macros

// Generator
#include "Symbol.hpp"


// constant flags for translation table (Symbol::Info::xlat)
static const unsigned short _cpNone   = 0x0000; // if character should/can not be translated
static const unsigned short _cp850    = 0x0100; // if character comes from code page 850
static const unsigned short _cp437    = 0x0200; // if character comes from code page 437
static const unsigned short _cpApprox = 0x8000; // if tranlation is approximate (not preferred)


Symbol::Symbol( Identifier id ):
  _ch( 0 ),
  _id( id )
{}


Symbol::Symbol( unsigned char ch, CodePage codePage ):
  _ch( 0 )
{
  IASSERTPARM( ch );

  // attempt to convert a char to a symbol id
  // if given code page was 437, try that first, otherwise 850
  Boolean found = searchForSymbol(
    codePage == 437? _cp437: _cp850,
    codePage == 437? 437: 850,
    ch, codePage );
  if ( ! found )
    found = searchForSymbol(
      codePage != 437? _cp850: _cp437,
      codePage != 437? 850: 437,
      ch, codePage );

  // can't find symbol -- save as CP/char pair
  if ( ! found )
  {
    _ch = ch;
    _codePage = codePage;
  }
}


Boolean Symbol::searchForSymbol(
  unsigned short tableFlag,
  CodePage tableCodePage,
  unsigned char ch,
  CodePage fromCodePage )
{
  // convert char to table code page for search
  unsigned char wantChar = tableCodePage.translate( ch, fromCodePage );
  if ( wantChar == 0 )
    return false;

  // loop through table entries looking for a match
  Identifier idApprox = unknown;
  for (int i = 0; i <= endOfList - startOfList; i++)
  {
    if ( _info[i].xlat & tableFlag )
    {
      unsigned char tableChar = (unsigned char)(_info[i].xlat & 0xff);
      if (tableChar == wantChar)
      {
        idApprox = Identifier(i + startOfList);
        if ((_info[i].xlat & _cpApprox) == 0)
        {
          // found an exact (not approximate) match
          _id = idApprox;
          return true;
        }
      }
    }
  }

  // check if an approximate match was found (better than nothing!)
  if (idApprox != unknown)
  {
    _id = idApprox;
    return true;
  }

  return false;
}


// returns 0 if can't/shouldn't be converted
// sets isApproximate to true if the translation was approximate
unsigned char Symbol::asChar( CodePage codePage, Boolean * isApproximate ) const
{
  // initialize the "approximate" flag
  if ( isApproximate )
    *isApproximate = false;

  CodePage fromCodePage;
  unsigned char fromChar;
  if ( _ch )
  {
    fromCodePage = _codePage;
    fromChar = _ch;
  }
  else
  {
    // get code page and char from ID via static table
    IASSERTSTATE( _id >= startOfList && _id <= endOfList );
    unsigned short x = _info[ _id - startOfList ].xlat;
    fromChar = (unsigned char)(x & 0xff);

    // set the "approximate" flag
    if ( isApproximate )
      *isApproximate = ( ( x & _cpApprox ) == _cpApprox );

    // first check for an exact code page match
    if ( codePage == 850 && ( x & _cp850 ) )
      return fromChar;
    if ( codePage == 437 && ( x & _cp437 ) )
      return fromChar;

    // next check for an available conversion
    if ( x & _cp850 )
      fromCodePage = 850;
    else if ( x & _cp437 )
      fromCodePage = 437;
    else
      return 0;  // no translation!
  }

  return codePage.translate( fromChar, fromCodePage );
}


void Symbol::getDetails( unsigned char & ch, CodePage & cp ) const
{
  ch = _ch;
  cp = _codePage;
}



IString Symbol::description() const
{
  if ( _ch )
    return IString( _ch );
  else
  {
    IASSERTSTATE( _id >= startOfList && _id <= endOfList );
    return IString( _info[_id - startOfList].description );
  }
}



// translation table
// lower 8-bits is the unsigned char for a given symbol
// upper 8-bits is flags which tell which code page the char is from
const Symbol::Info Symbol::_info[ endOfList - startOfList + 1 ] =
{
  {              _cp437 | _cp850 | 160, "a acute" },
  {                       _cp850 | 181, "A acute" },
  {              _cp437 | _cp850 | 131, "a circumflex" },
  {                       _cp850 | 182, "A circumflex" },
  {                       _cp850 | 239, "acute" },
  {              _cp437 | _cp850 | 132, "a umlaut" },
  {              _cp437 | _cp850 | 142, "A umlaut" },
  {              _cp437 | _cp850 | 145, "ae ligature" },
  {              _cp437 | _cp850 | 146, "AE ligature" },
  {              _cp437 | _cp850 | 133, "a grave" },
  {                       _cp850 | 183, "A grave" },
  {                       _cp437 | 224, "alpha" },
  {              _cp437 | _cp850 |  65, "Alpha" },
  {              _cp437 | _cp850 |  38, "ampersand" },
  {              _cp437 | _cp850 |  94, "and" },
  {              _cp437 | _cp850 | 143, "angstrom" },
  {              _cp437 | _cp850 | 134, "a overcircle" },
  {              _cp437 | _cp850 | 143, "A overcircle" },
  {              _cp437 | _cp850 |  39, "apostrophe" },
  {              _cp437 | _cp850 |  42, "asterisk" },
  {                       _cp850 | 199, "A tilde" },
  {                       _cp850 | 198, "a tilde" },
  {              _cp437 | _cp850 |  64, "at sign" },
  {              _cp437 | _cp850 | 166, "underscored a" },
  {              _cp437 | _cp850 | 225, "Beta" },
  {              _cp437 | _cp850 | 219, "solid box" },
  {              _cp437 | _cp850 | 177, "shaded box 1/2 dots" },
  {              _cp437 | _cp850 | 176, "shaded box 1/4 dots" },
  {              _cp437 | _cp850 | 178, "shaded box 3/4 dots" },
  {              _cp437 | _cp850 | 220, "solid box bottom half" },
  {                       _cp437 | 221, "solid box left half" },
  {                       _cp437 | 222, "solid box right half" },
  {              _cp437 | _cp850 | 223, "solid box top half" },
  {              _cp437 | _cp850 |  92, "back slash" },
  {              _cp437 | _cp850 |   7, "bullet" },
  {                       _cp437 | 184, "box 0 0 1 2" },
  {                       _cp437 | 183, "box 0 0 2 1" },
  {              _cp437 | _cp850 | 187, "box 0 0 2 2" },
  {                       _cp437 | 214, "box 0 1 2 0" },
  {                       _cp437 | 210, "box 0 1 2 1" },
  {              _cp437 | _cp850 | 205, "box 0 2 0 2" },
  {                       _cp437 | 213, "box 0 2 1 0" },
  {                       _cp437 | 209, "box 0 2 1 2" },
  {              _cp437 | _cp850 | 201, "box 0 2 2 0" },
  {              _cp437 | _cp850 | 203, "box 0 2 2 2" },
  {                       _cp437 | 190, "box 1 0 0 2" },
  {                       _cp437 | 181, "box 1 0 1 2" },
  {                       _cp437 | 212, "box 1 2 0 0" },
  {                       _cp437 | 208, "box 2 1 0 1" },
  {                       _cp437 | 207, "box 1 2 0 2" },
  {                       _cp437 | 198, "box 1 2 1 0" },
  {                       _cp437 | 216, "box 1 2 1 2" },
  {                       _cp437 | 189, "box 2 0 0 1" },
  {              _cp437 | _cp850 | 188, "box 2 0 0 2" },
  {              _cp437 | _cp850 | 186, "box 2 0 2 0" },
  {                       _cp437 | 182, "box 2 0 2 1" },
  {              _cp437 | _cp850 | 185, "box 2 0 2 2" },
  {                       _cp437 | 211, "box 2 1 0 0" },
  {                       _cp437 | 199, "box 2 1 2 0" },
  {                       _cp437 | 215, "box 2 1 2 1" },
  {              _cp437 | _cp850 | 200, "box 2 2 0 0" },
  {              _cp437 | _cp850 | 202, "box 2 2 0 2" },
  {              _cp437 | _cp850 | 204, "box 2 2 2 0" },
  {              _cp437 | _cp850 | 206, "box 2 2 2 2" },
  {              _cp437 | _cp850 | 193, "box ascender" },
  {              _cp437 | _cp850 | 197, "box cross" },
  {              _cp437 | _cp850 | 194, "box descender" },
  {              _cp437 | _cp850 | 196, "box horizontal" },
  {              _cp437 | _cp850 | 195, "box left junction" },
  {              _cp437 | _cp850 | 192, "box lower-left" },
  {              _cp437 | _cp850 | 217, "box lower-right" },
  {              _cp437 | _cp850 | 180, "box right junction" },
  {              _cp437 | _cp850 | 218, "box upper-left" },
  {              _cp437 | _cp850 | 191, "box upper-right" },
  {              _cp437 | _cp850 | 179, "box vertical" },
  {              _cp437 | _cp850 |  94, "caret symbol" },
  {              _cp437 | _cp850 | 135, "c cedilla" },
  {              _cp437 | _cp850 | 128, "C cedilla" },
  {  _cpApprox | _cp437 | _cp850 |  34, "close double quote" },
  {              _cp437 | _cp850 | 175, "close French double quote" },
  {                       _cp850 | 247, "cedilla" },
  {                       _cp850 | 189, "cent" },
  {              _cp437 | _cp850 |  58, "colon" },
  {              _cp437 | _cp850 |  44, "comma" },
  {                       _cp850 | 184, "copyright" },
  {  _cpApprox | _cp437 | _cp850 |  39, "close single quote" },
  {                       _cp850 | 207, "general currency sign" },
  {              _cp437 | _cp850 |  25, "down arrow" },
  {  _cpApprox | _cp437 | _cp850 |  45, "dash" },
  {              _cp437 | _cp850 | 248, "degree" },
  {                       _cp437 | 235, "delta" },
  {              _cp437 | _cp850 | 246, "divide" },
  {              _cp437 | _cp850 |  36, "dollar sign" },
  {              _cp437 | _cp850 | 250, "dot" },
  {              _cp437 | _cp850 | 144, "E acute" },
  {              _cp437 | _cp850 | 130, "e acute" },
  {              _cp437 | _cp850 | 136, "e circumflex" },
  {                       _cp850 | 210, "E circumflex" },
  {              _cp437 | _cp850 | 137, "e umlaut" },
  {                       _cp850 | 211, "E umlaut" },
  {              _cp437 | _cp850 | 138, "e grave" },
  {                       _cp850 | 212, "E grave" },
  {  _cpApprox | _cp437 | _cp850 |  45, "em dash" },
  {              _cp437 | _cp850 |  45, "en dash" },
  {                       _cp437 | 238, "epsilon" },
  {              _cp437 | _cp850 |  61, "equal sign" },
  {                       _cp850 | 209, "Eth Icelandic" },
  {                       _cp850 | 208, "eth Icelandic" },
  {              _cp437 | _cp850 |  12, "female" },
  {              _cp437 | _cp850 | 159, "function of" },
  {              _cp437 | _cp850 | 171, "one half" },
  {              _cp437 | _cp850 | 172, "one fourth" },
  {                       _cp850 | 243, "fraction three-quarters" },
  {                       _cp437 | 226, "gamma" },
  {                       _cp437 | 242, "greater than or equal to" },
  {              _cp437 | _cp850 |  96, "accent grave" },
  {              _cp437 | _cp850 |  62, "greater than" },
  {              _cp437 | _cp850 | 127, "house" },
  {              _cp437 | _cp850 |  45, "hyphen" },
  {              _cp437 | _cp850 | 161, "i acute" },
  {                       _cp850 | 214, "I acute" },
  {              _cp437 | _cp850 | 140, "i circumflex" },
  {                       _cp850 | 215, "I circumflex" },
  {                       _cp437 | 240, "identical to" },
  {              _cp437 | _cp850 | 139, "i umlaut" },
  {                       _cp850 | 216, "I umlaut" },
  {              _cp437 | _cp850 | 141, "i grave" },
  {                       _cp850 | 222, "I grave" },
  {                       _cp437 | 236, "infinity" },
  {                       _cp437 | 245, "integral sign bottom" },
  {                       _cp437 | 239, "set intersection" },
  {                       _cp437 | 244, "integral sign top" },
  {              _cp437 | _cp850 | 173, "inverted exclamation mark" },
  {              _cp437 | _cp850 | 168, "inverted question mark" },
  {              _cp437 | _cp850 |  17, "left arrowhead" },
  {              _cp437 | _cp850 |  27, "left arrow" },
  {              _cp437 | _cp850 | 123, "left brace" },
  {              _cp437 | _cp850 |  91, "left bracket" },
  {                       _cp437 | 243, "less than or equal to" },
  {              _cp437 | _cp850 | 170, "logical not" },
  {                       _cp437 | 169, "not symbol reversed" },
  {              _cp437 | _cp850 | 124, "logical or" },
  {              _cp437 | _cp850 |  40, "left parenthesis" },
  {              _cp437 | _cp850 | 156, "pound sterling" },
  {              _cp437 | _cp850 |  60, "less than" },
  {                       _cp850 | 238, "macron" },
  {              _cp437 | _cp850 |  11, "male" },
  {              _cp437 | _cp850 | 230, "micro prefix" },
  {              _cp437 | _cp850 |  45, "minus sign" },
  {  _cpApprox | _cp437 | _cp850 |  32, "m-space" },
  {              _cp437 | _cp850 | 230, "mu" },
  {                       _cp437 | 247, "nearly equal to" },
  {  _cpApprox | _cp437 | _cp850 |  32, "n-space" },
  {              _cp437 | _cp850 | 164, "n tilde" },
  {              _cp437 | _cp850 | 165, "N tilde" },
  {              _cp437 | _cp850 |  35, "number sign" },
  {              _cp437 | _cp850 | 162, "o acute" },
  {                       _cp850 | 224, "O acute" },
  {              _cp437 | _cp850 | 147, "o circumflex" },
  {                       _cp850 | 226, "O circumflex" },
  {  _cpApprox | _cp437 | _cp850 |  34, "open double quote" },
  {              _cp437 | _cp850 | 174, "open French double quote" },
  {              _cp437 | _cp850 | 148, "o umlaut" },
  {              _cp437 | _cp850 | 153, "O umlaut" },
  {              _cp437 | _cp850 | 149, "o grave" },
  {                       _cp850 | 227, "O grave" },
  {                       _cp850 | 157, "O slash" },
  {                       _cp850 | 155, "o slash" },
  {  _cpApprox | _cp437 | _cp850 |  39, "open single quote" },
  {                       _cp850 | 229, "O tilde" },
  {                       _cp850 | 228, "o tilde" },
  {              _cp437 | _cp850 | 167, "underscored o" },
  {                       _cp850 | 244, "paragraph symbol" },
  {              _cp437 | _cp850 |  46, "period" },
  {              _cp437 | _cp850 |  37, "percent" },
  {                       _cp437 | 237, "lowercase phi" },
  {                       _cp437 | 232, "uppercase Phi" },
  {                       _cp437 | 227, "pi" },
  {              _cp437 | _cp850 |  43, "plus sign" },
  {              _cp437 | _cp850 | 241, "plusminus" },
  {              _cp437 | _cp850 |  34, "double quote" },
  {              _cp437 | _cp850 |  16, "right arrowhead" },
  {              _cp437 | _cp850 |  26, "right arrow" },
  {  _cpApprox | _cp437 | _cp850 |  32, "required blank" },
  {              _cp437 | _cp850 | 125, "right brace" },
  {              _cp437 | _cp850 |  93, "right bracket" },
  {                       _cp850 | 169, "registered sign" },
  {  _cpApprox | _cp437 | _cp850 |  45, "required hyphen" },
  {              _cp437 | _cp850 |  41, "right parenthesis" },
  {                       _cp850 | 245, "section sign" },
  {              _cp437 | _cp850 |  59, "semicolon" },
  {                      _cpNone |   0, "soft hyphen" },
  {                       _cp437 | 229, "lowercase sigma" },
  {                       _cp437 | 228, "uppercase Sigma" },
  {  _cpApprox | _cp437 | _cp850 | 126, "similar to" },
  {              _cp437 | _cp850 |  47, "slash" },
  {                       _cp850 | 221, "split vertical bar" },
  {              _cp437 | _cp850 | 254, "square bullet" },
  {                       _cp437 | 251, "square root" },
  {                       _cp850 | 251, "superscript 1" },
  {              _cp437 | _cp850 | 253, "superscript 2" },
  {                       _cp850 | 252, "superscript 3" },
  {                       _cp437 | 252, "superscript n" },
  {              _cp437 | _cp850 | 225, "sz ligature" },
  {                       _cp437 | 231, "tau" },
  {                       _cp437 | 233, "Theta" },
  {                       _cp850 | 232, "THORN Icelandic" },
  {                       _cp850 | 231, "thorn Icelandic" },
  {              _cp437 | _cp850 | 126, "tilde" },
  {                       _cp850 | 158, "multiplication sign" },
  {              _cp437 | _cp850 | 163, "u acute" },
  {                       _cp850 | 233, "U acute" },
  {              _cp437 | _cp850 |  24, "up arrow" },
  {              _cp437 | _cp850 | 150, "u circumflex" },
  {                       _cp850 | 234, "U circumflex" },
  {              _cp437 | _cp850 | 129, "u umlaut" },
  {              _cp437 | _cp850 | 154, "U umlaut" },
  {              _cp437 | _cp850 | 151, "u grave" },
  {                       _cp850 | 235, "U grave" },
  {                       _cp850 | 249, "umlaut" },
  {              _cp437 | _cp850 |  95, "underscore" },
  {              _cp437 | _cp850 | 124, "solid vertical bar" },
  {              _cp437 | _cp850 |  33, "exclamation point" },
  {                       _cp850 | 237, "Y acute" },
  {                       _cp850 | 236, "y acute" },
  {              _cp437 | _cp850 | 152, "y umlaut" },
  {                       _cp850 | 190, "Yen" },
  {                       _cp437 | 234, "Omega" }
};






































































































































































































































