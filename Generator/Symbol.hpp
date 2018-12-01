/***************************************************************************
 * File...... Symbol.hpp
 * Author.... Mat
 * Date...... 3/27/97
 *
 * Symbol represents a typographical symbol.  An id enmuerator lists all
 * of the possible symbols.  This class also provides for translation
 * between id and char under different code pages.
 *
 * The enumerator list was compiled from the following documents:
 *
 *   IPF Reference 2.00 (updated with APSYMBOL.APS dated 11-17-94)
 *   HTML Markup Language 2.0 (8-4-95)
 *   RTF Specification 1.3 (1-94)
 *
 * The names of the symbols below are mostly, but not always, the same name
 * as the corresponding IPF symbol, HTML entity or RTF control word.  Some
 * were changed for consistency, and some weren't included because they were
 * already listed under a different name.
 *
 * NOTE: symbols are case-sensitive and there are many which differ only
 * by the case of one letter.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef SYMBOL_HPP
#define SYMBOL_HPP

// OpenClass
#include "CodePage.hpp"


class _Export Symbol
{
  public:
    // enumerated ids starts with 256 to avoid conflict with ASCII
    enum Identifier
    {
      unknown = 0,       // unknown symbol (no match)
      startOfList = 256,
      aa = startOfList,  // a acute
      Aa          = 257, // A acute
      ac          = 258, // a circumflex
      Ac          = 259, // A circumflex
      acute       = 260, // acute
      ae          = 261, // a umlaut
      Ae          = 262, // A umlaut
      aelig       = 263, // ae ligature
      AElig       = 264, // AE ligature
      ag          = 265, // a grave
      Ag          = 266, // A grave
      alpha       = 267, // alpha
      Alpha       = 268, // Alpha
      amp         = 269, // ampersand
      and         = 270, // and
      angstrom    = 271, // angstrom
      ao          = 272, // a overcircle
      Ao          = 273, // A overcircle
      apos        = 274, // apostrophe
      asterisk    = 275, // asterisk
      At          = 276, // A tilde
      at          = 277, // a tilde
      atsign      = 278, // at sign
      aus         = 279, // underscored a
      Beta        = 280, // Beta
      BOX         = 281, // solid box
      box12       = 282, // shaded box 1/2 dots
      box14       = 283, // shaded box 1/4 dots
      box34       = 284, // shaded box 3/4 dots
      BOXBOT      = 285, // solid box bottom half
      BOXLEFT     = 286, // solid box left half
      BOXRIGHT    = 287, // solid box right half
      BOXTOP      = 288, // solid box top half
      bslash      = 289, // back slash
      bullet      = 290, // bullet
      // bx.... are line drawing characters  where .... gives the line thickness
      // at 12  3  6 and 9 o'clock positions.  Line thickness may be 0  1  or 2.
      // thickness of 0 means no line in that direction.
      bx0012      = 291, // box 0 0 1 2
      bx0021      = 292, // box 0 0 2 1
      bx0022      = 293, // box 0 0 2 2
      bx0120      = 294, // box 0 1 2 0
      bx0121      = 295, // box 0 1 2 1
      bx0202      = 296, // box 0 2 0 2
      bx0210      = 297, // box 0 2 1 0
      bx0212      = 298, // box 0 2 1 2
      bx0220      = 299, // box 0 2 2 0
      bx0222      = 300, // box 0 2 2 2
      bx1002      = 301, // box 1 0 0 2
      bx1012      = 302, // box 1 0 1 2
      bx1200      = 303, // box 1 2 0 0
      bx2101      = 304, // box 2 1 0 1
      bx1202      = 305, // box 1 2 0 2
      bx1210      = 306, // box 1 2 1 0
      bx1212      = 307, // box 1 2 1 2
      bx2001      = 308, // box 2 0 0 1
      bx2002      = 309, // box 2 0 0 2
      bx2020      = 310, // box 2 0 2 0
      bx2021      = 311, // box 2 0 2 1
      bx2022      = 312, // box 2 0 2 2
      bx2100      = 313, // box 2 1 0 0
      bx2120      = 314, // box 2 1 2 0
      bx2121      = 315, // box 2 1 2 1
      bx2200      = 316, // box 2 2 0 0
      bx2202      = 317, // box 2 2 0 2
      bx2220      = 318, // box 2 2 2 0
      bx2222      = 319, // box 2 2 2 2
      bxas        = 320, // box ascender
      bxcr        = 321, // box cross
      bxde        = 322, // box descender
      bxh         = 323, // box horizontal
      bxle        = 324, // box left junction
      bxll        = 325, // box lower-left
      bxlr        = 326, // box lower-right
      bxri        = 327, // box right junction
      bxul        = 328, // box upper-left
      bxur        = 329, // box upper-right
      bxv         = 330, // box vertical
      caret       = 331, // caret symbol
      cc          = 332, // c cedilla
      Cc          = 333, // C cedilla
      cdq         = 334, // close double quote
      cdqf        = 335, // close French double quote
      cedil       = 336, // cedilla
      cent        = 337, // cent
      colon       = 338, // colon
      comma       = 339, // comma
      copy        = 340, // copyright
      csq         = 341, // close single quote
      curren      = 342, // general currency sign
      darrow      = 343, // down arrow
      dash        = 344, // dash
      degree      = 345, // degree
      delta       = 346, // delta
      divide      = 347, // divide
      dollar      = 348, // dollar sign
      dot         = 349, // dot
      Ea          = 350, // E acute
      ea          = 351, // e acute
      ec          = 352, // e circumflex
      Ec          = 353, // E circumflex
      ee          = 354, // e umlaut
      Ee          = 355, // E umlaut
      eg          = 356, // e grave
      Eg          = 357, // E grave
      emdash      = 358, // em dash
      endash      = 359, // en dash
      epsilon     = 360, // epsilon
      eq          = 361, // equal sign
      ETH         = 362, // Eth Icelandic
      eth         = 363, // eth Icelandic
      female      = 364, // female
      fnof        = 365, // function of
      frac12      = 366, // one half
      frac14      = 367, // one fourth
      frac34      = 368, // fraction three-quarters
      Gamma       = 369, // gamma
      ge          = 370, // greater than or equal to
      grave       = 371, // accent grave
      gt          = 372, // greater than
      house       = 373, // house
      hyphen      = 374, // hyphen
      ia          = 375, // i acute
      Ia          = 376, // I acute
      ic          = 377, // i circumflex
      Ic          = 378, // I circumflex
      identical   = 379, // identical to
      ie          = 380, // i umlaut
      Ie          = 381, // I umlaut
      ig          = 382, // i grave
      Ig          = 383, // I grave
      infinity    = 384, // infinity
      intbot      = 385, // integral sign bottom
      intersect   = 386, // set intersection
      inttop      = 387, // integral sign top
      inve        = 388, // inverted exclamation mark
      invq        = 389, // inverted question mark
      lahead      = 390, // left arrowhead
      larrow      = 391, // left arrow
      lbrace      = 392, // left brace
      lbracket    = 393, // left bracket
      le          = 394, // less than or equal to
      lnot        = 395, // logical not
      lnotrev     = 396, // not symbol reversed
      lor         = 397, // logical or
      lpar        = 398, // left parenthesis
      Lsterling   = 399, // pound sterling
      lt          = 400, // less than
      macr        = 401, // macron
      male        = 402, // male
      micro       = 403, // micro prefix
      minus       = 404, // minus sign
      mspace      = 405, // non-breaking space "m" width
      mu          = 406, // mu
      nearly      = 407, // nearly equal to
      nspace      = 408, // non-breaking space "n" width
      nt          = 409, // n tilde
      Nt          = 410, // N tilde
      numsign     = 411, // number sign
      oa          = 412, // o acute
      Oa          = 413, // O acute
      oc          = 414, // o circumflex
      Oc          = 415, // O circumflex
      odq         = 416, // open double quote
      odqf        = 417, // open French double quote
      oe          = 418, // o umlaut
      Oe          = 419, // O umlaut
      og          = 420, // o grave
      Og          = 421, // O grave
      Oslash      = 422, // O slash
      oslash      = 423, // o slash
      osq         = 424, // open single quote
      Ot          = 425, // O tilde
      ot          = 426, // o tilde
      ous         = 427, // underscored o
      para        = 428, // paragraph symbol
      per         = 429, // period
      percent     = 430, // percent
      phi         = 431, // lowercase phi
      Phi         = 432, // uppercase Phi
      pi          = 433, // pi
      plus        = 434, // plus sign
      plusmin     = 435, // plusminus
      quot        = 436, // double quote
      rahead      = 437, // right arrowhead
      rarrow      = 438, // right arrow
      rbl         = 439, // required blank (non-breaking)
      rbrace      = 440, // right brace
      rbracket    = 441, // right bracket
      reg         = 442, // registered sign
      rhy         = 443, // required hyphen (non-breaking)
      rpar        = 444, // right parenthesis
      sect        = 445, // section sign
      semi        = 446, // semicolon
      shy         = 447, // soft hyphen
      sigma       = 448, // lowercase sigma
      Sigma       = 449, // uppercase Sigma
      similar     = 450, // similar to
      slash       = 451, // slash
      splitvbar   = 452, // split vertical bar
      sqbul       = 453, // square bullet
      sqrt        = 454, // square root
      sup1        = 455, // superscript 1
      sup2        = 456, // superscript 2
      sup3        = 457, // superscript 3
      supn        = 458, // superscript n
      szlig       = 459, // sz ligature
      tau         = 460, // tau
      Theta       = 461, // Theta
      THORN       = 462, // THORN  Icelandic
      thorn       = 463, // thorn  Icelandic
      tilde       = 464, // tilde
      times       = 465, // multiplication sign
      ua          = 466, // u acute
      Ua          = 467, // U acute
      uarrow      = 468, // up arrow
      uc          = 469, // u circumflex
      Uc          = 470, // U circumflex
      ue          = 471, // u umlaut
      Ue          = 472, // U umlaut
      ug          = 473, // u grave
      Ug          = 474, // U grave
      uml         = 475, // umlaut
      us          = 476, // underscore
      vbar        = 477, // solid vertical bar
      xclm        = 478, // exclamation point
      Ya          = 479, // Y acute
      yacute      = 480, // y acute
      ye          = 481, // y umlaut
      yen         = 482, // Yen
      Omega       = 483, // Omega                   added 10/13/98
      endOfList = Omega
    };

    // constructors
    Symbol( Identifier id );
    Symbol( unsigned char ch, CodePage codePage = CodePage() );
      // attempt to convert a char to a symbol id

    // converters
    unsigned char asChar( CodePage codePage = CodePage(), Boolean * isApproximate = 0 ) const;
      // returns 0 if can't be converted
      // if isApproximate != 0, then it is set to true only if the conversion is "approximate"
      /// may want to distinguish between "can't convert" and "error" and "can't be represented"
    Identifier    id() const;
      // returns unknown if can't be identified
    void          getDetails( unsigned char & ch, CodePage & cp ) const;
      // return the stored char and codepage
    IString       description() const;
      // returns a string description of the symbol (e.g., "a acute")

  private:
    // helpers
    Boolean searchForSymbol(
      unsigned short tableFlag,
      CodePage tableCodePage,
      unsigned char ch,
      CodePage fromCodePage );

    // info structure
    static const struct Info
    {
      unsigned short xlat;
      char *         description;
    } _info[ endOfList - startOfList + 1 ];

    // data for this Symbol
    unsigned char _ch;        // 0 if stored as id
    CodePage      _codePage;  // valid if _ch != 0
    Identifier    _id;
};


// inline functions
#include "Symbol.ipp"


#endif

