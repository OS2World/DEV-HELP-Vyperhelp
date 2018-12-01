/***************************************************************************
 * File...... HtmlSymbols.cpp
 *
 * Performs mapping from Generator-accepted symbols to HTML symbols.  Where
 * there is not a matching HTML symbol, an attempt to match is made using
 * the best combination of ASCII characters and symbols.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

#include "HtmlCommon.hpp"


struct HtmlSymbol
{
  char *  output;
  Boolean isAlt;
};

const HtmlSymbol symtable[ Symbol::endOfList - Symbol::startOfList + 1 ] =
{
 { "&aacute;", false },               // a acute
 { "&Aacute;", false },               // A acute
 { "&acirc;", false },                // a circumflex
 { "&Acirc;", false },                // A circumflex
 { "&acute;", false },                // acute accent
 { "&auml;", false },                 // a umlaut
 { "&Auml;", false },                 // A umlaut
 { "&aelig;", false },                // ae ligature
 { "&AElig;", false },                // AE ligature
 { "&agrave;", false },               // a grave
 { "&Agrave;", false  },              // A grave
 { "a", true },                       // alpha
 { "A", true },                       // Alpha
 { "&amp;", false },                  // ampersand
 { "^", true },                       // and
 { "&Aring;", false },                // angstrom
 { "&aring;", false },                // a overcircle
 { "&Aring;", false },                // A overcircle
 { "'", false },                      // apostrophe
 { "*", false },                      // asterisk
 { "&Atilde;", false },               // A tilde
 { "&atilde;", false },               // a tilde
 { "@", false },                      // at sign
 { "&ordf;", false },                 // underscored a
 { "&szlig;", false },                // Beta
 { "[]", true },                      // solid box
 { "[]", true },                      // shaded box 1/2 dots
 { "[]", true },                      // shaded box 1/4 dots
 { "[]", true },                      // shaded box 3/4 dots
 { "_", true },                       // solid box bottom half
 { "[", true },                       // solid box left half
 { "]", true },                       // solid box right half
 { "&macr;", true },                  // solid box top half
 { "\\", false },                     // back slash
 { "&middot;", false },               // bullet
 { "+", true },                       // bx0012,  *** simulate boxes with +, - and | ***
 { "+", true },                       // bx0021,
 { "+", true },                       // bx0022,
 { "+", true },                       // bx0120,
 { "+", true },                       // bx0121,
 { "-", true },                       // bx0202,
 { "+", true },                       // bx0210,
 { "+", true },                       // bx0212,
 { "+", true },                       // bx0220,
 { "+", true },                       // bx0222,
 { "+", true },                       // bx1002,
 { "+", true },                       // bx1012,
 { "+", true },                       // bx1200,
 { "+", true },                       // bx2101,
 { "+", true },                       // bx1202,
 { "+", true },                       // bx1210,
 { "+", true },                       // bx1212,
 { "+", true },                       // bx2001,
 { "+", true },                       // bx2002,
 { "|", true },                       // bx2020,
 { "+", true },                       // bx2021,
 { "+", true },                       // bx2022,
 { "+", true },                       // bx2100,
 { "+", true },                       // bx2120,
 { "+", true },                       // bx2121,
 { "+", true },                       // bx2200,
 { "+", true },                       // bx2202,
 { "+", true },                       // bx2220,
 { "+", true },                       // bx2222,
 { "+", true },                       // box ascender
 { "+", true },                       // box cross
 { "+", true },                       // box descender
 { "-", true },                       // box horizontal
 { "+", true },                       // box left junction
 { "+", true },                       // box lower-left
 { "+", true },                       // box lower-right
 { "+", true },                       // box right junction
 { "+", true },                       // box upper-left
 { "+", true },                       // box upper-right
 { "|", true },                       // box vertical
 { "^", false },                      // caret symbol
 { "&ccedil;", false },               // c cedilla
 { "&Ccedil;", false },               // C cedilla
 { "\"", true },                      // close double quote
 { "&raquo;", false },                // close French double quote
 { "&cedil;", false },                // cedilla
 { "&cent;", false },                 // cent
 { ":", false },                      // colon
 { ",", false },                      // comma
 { "&copy;", false },                 // copyright
 { "'", true },                       // close single quote
 { "&curren;", false },               // general currency sign
 { "|", true },                       // down arrow
 { "-", true },                       // dash
 { "&deg;", false },                  // degree
 { "d", true },                       // delta
 { "&divide;", false },               // divide
 { "$", false },                      // dollar sign
 { "&middot;", false },               // dot
 { "&Eacute;", false },               // E acute
 { "&eacute;", false },               // e acute
 { "&ecirc;", false },                // e circumflex
 { "&Ecirc;", false },                // E circumflex
 { "&euml;", false },                 // e umlaut
 { "&Euml;", false },                 // E umlaut
 { "&egrave;", false },               // e grave
 { "&Egrave;", false },               // E grave
 { "--", true },                      // em dash
 { "-", true },                       // en dash
 { "e", true },                       // epsilon
 { "=", false },                      // equal sign
 { "&ETH;", false },                  // Eth, Icelandic
 { "&eth;", false },                  // eth, Icelandic
 { "(f)", true  },                    // female
 { "f", true },                       // function of
 { "&frac12;", false },               // one half
 { "&frac14;", false },               // one fourth
 { "&frac34;", false },               // fraction three-quarters
 { "C", true },                       // gamma
 { "&gt;=", true },                   // greater than or equal to
 { "`", false },                      // accent grave
 { "&gt;", false },                   // greater than
 { "#", true },                       // house
 { "-", false },                      // hyphen
 { "&iacute;", false },               // i acute
 { "&Iacute;", false },               // I acute
 { "&icirc;", false },                // i circumflex
 { "&Icirc;", false },                // I circumflex
 { "=", true },                       // identical to
 { "&iuml;", false },                 // i umlaut
 { "&Iuml;", false },                 // I umlaut
 { "&igrave;", false },               // i grave
 { "&Igrave;", false },               // I grave
 { "oo", true },                      // infinity
 { "{", true },                       // integral sign bottom
 { "N", true },                       // set intersection
 { "}", true },                       // integral sign top
 { "&iexcl;", false },                // inverted exclamation mark
 { "&iquest;", false },               // inverted question mark
 { "&lt;", true },                    // left arrowhead
 { "&lt;", true },                    // left arrow
 { "{", false },                      // left brace
 { "[", false },                      // left bracket
 { "&lt;=", true },                   // less than or equal to
 { "&not;", false },                  // logical not
 { "+", true },                       // not symbol reversed
 { "|", true },                       // logical or
 { "(", false },                      // left parenthesis
 { "&pound;", false },                // pound sterling
 { "&lt;", false },                   // less than
 { "&macr;", false },                 // macron
 { "(m)", true  },                    // male
 { "&micro;", false },                // micro prefix
 { "-", false },                      // minus sign
 { "&nbsp;&nbsp;", false },           // non-breaking space, "m" width
 { "m", true },                       // mu
 { "~", true },                       // nearly equal to
 { "&nbsp;", false },                 // non-breaking space, "n" width
 { "&ntilde;", false },               // n tilde
 { "&Ntilde;", false },               // N tilde
 { "#", false },                      // number sign
 { "&oacute;", false },               // o acute
 { "&Oacute;", false },               // O acute
 { "&ocirc;", false },                // o circumflex
 { "&Ocirc;", false },                // O circumflex
 { "\"", true },                      // open double quote
 { "&laquo;", false },                // open French double quote
 { "&ouml;", false },                 // o umlaut
 { "&Ouml;", false },                 // O umlaut
 { "&ograve;", false },               // o grave
 { "&Ograve;", false },               // O grave
 { "&Oslash;", false },               // O slash
 { "&oslash;", false },               // o slash
 { "'", true },                       // open single quote
 { "&Otilde;", false },               // O tilde
 { "&otilde;", false },               // o tilde
 { "&ordm;", false },                 // underscored o
 { "&para;", false },                 // paragraph symbol
 { ".", false },                      // period
 { "%", false },                      // percent
 { "v", true },                       // lowercase phi
 { "V", true },                       // uppercase Phi
 { "p", true },                       // pi
 { "+", false },                      // plus sign
 { "&plusmn;", false },               // plusminus
 { "\"", true  },                     // double quote
 { "&gt;", true },                    // right arrowhead
 { "&gt;", true },                    // right arrow
 { "&nbsp;", false },                 // required blank (non-breaking)
 { "}", false },                      // right brace
 { "]", false },                      // right bracket
 { "&reg;", false },                  // registered sign
 { "-", true },                       // required hyphen (non-breaking)
 { ")", false },                      // right parenthesis
 { "&sect;", false },                 // section sign
 { ";", false },                      // semicolon
 { "&shy;", false },                  // soft hyphen
 { "s", true },                       // lowercase sigma
 { "S", true },                       // uppercase Sigma
 { "~", true },                       // similar to
 { "/", false },                      // slash
 { "&brvbar;", false },               // split vertical bar (piping symbol)
 { "+", true },                       // square bullet
 { "sqrt", true },                    // square root
 { "&sup1;", false },                 // superscript 1
 { "&sup2;", false },                 // superscript 2
 { "&sup3;", false },                 // superscript 3
 { "^n", true },                      // superscript n
 { "&szlig;", false },                // sharp s, German sz ligature
 { "t", true },                       // tau
 { "H", true },                       // Theta
 { "&THORN;", false },                // THORN, Icelandic
 { "&thorn;", false },                // thorn, Icelandic
 { "~", false },                      // tilde
 { "&times;", false },                // multiplication sign
 { "&uacute;", false },               // u acute
 { "&Uacute;", false },               // U acute
 { "^", true },                       // up arrow
 { "&ucirc;", false },                // u circumflex
 { "&Ucirc;", false },                // U circumflex
 { "&uuml;", false },                 // u umlaut
 { "&Uuml;", false },                 // U umlaut
 { "&ugrave;", false },               // u grave
 { "&Ugrave;", false },               // U grave
 { "&uml;", false },                  // umlaut
 { "_", false },                      // underscore
 { "|", false },                      // solid vertical bar
 { "!", false },                      // exclamation point
 { "&Yacute;", false },               // Y acute
 { "&yacute;", false },               // y acute
 { "&yuml;", false },                 // y umlaut
 { "&yen;", false },                  // Yen
 { "Z", true }                        // Omega
};


static const HtmlSymbol * findSymbol( const Symbol & symbol )
{
  Symbol::Identifier id = symbol.id();
  IASSERTPARM( id <= Symbol::endOfList );
  return id? symtable + id - Symbol::startOfList: 0;
}


/***************************************************************************
 * Procedure.. HtmlGenerator::operator<<( const Symbol )
 *
 * Inserts the given symbol as text in the output stream.  Not all
 * Generator symbols are supported by Html, so we use what we have and try
 * to use characters or symbols that make sense.
 ***************************************************************************/
void HtmlGenerator::handleSymbol( const SymbolGin & gin )
{
  // ignore in comment mode
  if ( ! _next->isComment )
  {
    const HtmlSymbol * sym = findSymbol( gin.symbol() );
    Boolean            isChApprox;
    unsigned char      ch = gin.symbol().asChar( outputCodepage, &isChApprox );

    // send output from table, or send translated char
    // - translated char is preferable to an "alternative" HTML output
    // - however, "alternative" HTML output is preferable to "approximate" translation
    if ( sym && ( ! sym->isAlt || ! ch || isChApprox ) )
      sendText( sym->output, noSymbolCheck );
    else if ( ch )
      sendText( ch, 0 );  // send translated character
    else
      sendText( CodePage::substitution, 0 );  // translation failed, send "substitution" character
  }
}


/***************************************************************************
 * Procedure.. HtmlGenerator::translateChar
 *
 * Translate a character from the current document code page to an Html
 * symbol name if possible (return the symbol name).  If not possible,
 * return false.
 ***************************************************************************/
Boolean HtmlGenerator::translateChar( unsigned char ch, IString & symbolName ) const
{
  const HtmlSymbol *  sym = findSymbol( Symbol( ch, _docCodePage ) );
  if ( sym && ! sym->isAlt )
  {
    symbolName = sym->output;
    return true;
  }

  return false;
}


