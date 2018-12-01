/***************************************************************************
 * File...... IPFSymbols.cpp
 * Author.... Mat
 * Date...... 11/16/95
 *
 * Performs mapping from Generator-accepted symbols to IPF symbols.  Where
 * there is not a matching IPF symbol, an attempt to match is made using
 * the best combination of ASCII characters and symbols.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#pragma hdrfile "IPF_Common.pch"
#include "IPF_Common.hpp"
#pragma hdrstop


struct IPFSymbol
{
  char *  output;
  Boolean isAlt;
};

const IPFSymbol symtable[ Symbol::endOfList - Symbol::startOfList + 1 ] =
{
 { "&aa.", false },        // aa,                a acute
 { "A", true  },           // Aa,                A acute
 { "&ac.", false },        // ac,                a circumflex
 { "A", true  },           // Ac,                A circumflex
 { "'", true  },           // acute,             acute accent
 { "&ae.", false },        // ae,                a umlaut
 { "&Ae.", false },        // Ae,                A umlaut
 { "&aelig.", false },     // aelig,             ae ligature
 { "&AElig.", false },     // AElig,             AE ligature
 { "&ag.", false },        // ag,                a grave
 { "A", true  },           // Ag,                A grave
 { "&alpha.", false },     // alpha,             alpha
 { "&Alpha.", false },     // Alpha,             Alpha
 { "&amp.", false },       // amp,               ampersand
 { "&and.", false },       // and,               and
 { "&angstrom.", false },  // angstrom,          angstrom
 { "&ao.", false },        // ao,                a overcircle
 { "&Ao.", false },        // Ao,                A overcircle
 { "&apos.", false },      // apos,              apostrophe
 { "&asterisk.", false },  // asterisk,          asterisk
 { "A", true  },           // At,                A tilde
 { "a", true  },           // at,                a tilde
 { "&atsign.", false },    // atsign,            at sign
 { "&aus.", false },       // aus,               underscored a
 { "&Beta.", false },      // Beta,              Beta
 { "&BOX.", false },       // BOX,               solid box
 { "&box12.", false },     // box12,             shaded box 1/2 dots
 { "&box14.", false },     // box14,             shaded box 1/4 dots
 { "&box34.", false },     // box34,             shaded box 3/4 dots
 { "&BOXBOT.", false },    // BOXBOT,            solid box bottom half
 { "&BOXLEFT.", false },   // BOXLEFT,           solid box left half
 { "&BOXRIGHT.", false },  // BOXRIGHT,          solid box right half
 { "&BOXTOP.", false },    // BOXTOP,            solid box top half
 { "&bsl.", false },       // bslash,            back slash
 { "&bul.", false },       // bullet,            bullet
 { "&bx0012.", false },    // bx0012,
 { "&bx0021.", false },    // bx0021,
 { "&bx0022.", false },    // bx0022,
 { "&bx0120.", false },    // bx0120,
 { "&bx0121.", false },    // bx0121,
 { "&bx0202.", false },    // bx0202,
 { "&bx0210.", false },    // bx0210,
 { "&bx0212.", false },    // bx0212,
 { "&bx0220.", false },    // bx0220,
 { "&bx0222.", false },    // bx0222,
 { "&bx1002.", false },    // bx1002,
 { "&bx1012.", false },    // bx1012,
 { "&bx1200.", false },    // bx1200,
 { "&bx1201.", false },    // bx2101, NOTE: this IPF symbol is misnamed
 { "&bx1202.", false },    // bx1202,
 { "&bx1210.", false },    // bx1210,
 { "&bx1212.", false },    // bx1212,
 { "&bx2001.", false },    // bx2001,
 { "&bx2002.", false },    // bx2002,
 { "&bx2020.", false },    // bx2020,
 { "&bx2021.", false },    // bx2021,
 { "&bx2022.", false },    // bx2022,
 { "&bx2100.", false },    // bx2100,
 { "&bx2120.", false },    // bx2120,
 { "&bx2121.", false },    // bx2121,
 { "&bx2200.", false },    // bx2200,
 { "&bx2202.", false },    // bx2202,
 { "&bx2220.", false },    // bx2220,
 { "&bx2222.", false },    // bx2222,
 { "&bxas.", false },      // bxas,              box ascender
 { "&bxcr.", false },      // bxcr,              box cross
 { "&bxde.", false },      // bxde,              box descender
 { "&bxh.", false },       // bxh,               box horizontal
 { "&bxle.", false },      // bxle,              box left junction
 { "&bxll.", false },      // bxll,              box lower-left
 { "&bxlr.", false },      // bxlr,              box lower-right
 { "&bxri.", false },      // bxri,              box right junction
 { "&bxul.", false },      // bxul,              box upper-left
 { "&bxur.", false },      // bxur,              box upper-right
 { "&bxv.", false },       // bxv,               box vertical
 { "&caret.", false },     // caret,             caret symbol
 { "&cc.", false },        // cc,                c cedilla
 { "&Cc.", false },        // Cc,                C cedilla
 { "&cdq.", false },       // cdq,               close double quote
 { "&cdqf.", false },      // cdqf,              close French double quote
 { "", true  },            // cedil,             cedilla
 { "&cent.", false },      // cent,              cent
 { "&colon.", false },     // colon,             colon
 { "&comma.", false },     // comma,             comma
 { "(c)", true  },         // copy,              copyright
 { "&csq.", false },       // csq,               close single quote
 { "", true  },           // curren,            general currency sign
 { "&darrow.", false },    // darrow,            down arrow
 { "&dash.", false },      // dash,              dash
 { "&degree.", false },    // degree,            degree
 { "&delta.", false },     // delta,             delta
 { "&divide.", false },    // divide,            divide
 { "&dollar.", false },    // dollar,            dollar sign
 { "&dot.", false },       // dot,               dot
 { "&Ea.", false },        // Ea,                E acute
 { "&ea.", false },        // ea,                e acute
 { "&ec.", false },        // ec,                e circumflex
 { "E", true  },           // Ec,                E circumflex
 { "&ee.", false },        // ee,                e umlaut
 { "E", true  },           // Ee,                E umlaut
 { "&eg.", false },        // eg,                e grave
 { "E", true  },           // Eg,                E grave
 { "&emdash.", false },    // emdash,            em dash
 { "&endash.", false },    // endash,            en dash
 { "&epsilon.", false },   // epsilon,           epsilon
 { "&eq.", false },        // eq,                equal sign
 { "D", true  },           // ETH,               Eth, Icelandic
 { "d", true  },           // eth,               eth, Icelandic
 { "", true  },           // female,            female
 { "&fnof.", false },      // fnof,              function of
 { "&frac12.", false },    // frac12,            one half
 { "&frac14.", false },    // frac14,            one fourth
 { "3/4", true  },         // frac34,            fraction three-quarters
 { "&Gamma.", false },     // Gamma,             gamma
 { "&ge.", false },        // ge,                greater than or equal to
 { "&grave.", false },     // grave,             accent grave
 { "&gt.", false },        // gt,                greater than
 { "&house.", false },     // house,             house
 { "&hyphen.", false },    // hyphen,            hyphen
 { "&ia.", false },        // ia,                i acute
 { "I", true  },           // Ia,                I acute
 { "&ic.", false },        // ic,                i circumflex
 { "I", true  },           // Ic,                I circumflex
 { "&identical.", false }, // identical,         identical to
 { "&ie.", false },        // ie,                i umlaut
 { "I", true  },           // Ie,                I umlaut
 { "&ig.", false },        // ig,                i grave
 { "I", true  },           // Ig,                I grave
 { "&infinity.", false },  // infinity,          infinity
 { "&intbot.", false },    // intbot,            integral sign bottom
 { "&intersect.", false }, // intersect,         set intersection
 { "&inttop.", false },    // inttop,            integral sign top
 { "&inve.", false },      // inve,              inverted exclamation mark
 { "&invq.", false },      // invq,              inverted question mark
 { "&lahead.", false },    // lahead,            left arrowhead
 { "&larrow.", false },    // larrow,            left arrow
 { "&lbrace.", false },    // lbrace,            left brace
 { "&lbracket.", false },  // lbracket,          left bracket
 { "&le.", false },        // le,                less than or equal to
 { "&lnot.", false },      // lnot,              logical not
 { "&lnotrev.", false },   // lnotrev,           not symbol reversed
 { "&lor.", false },       // lor,               logical or
 { "&lpar.", false },      // lpar,              left parenthesis
 { "&Lsterling.", false }, // Lsterling,         pound sterling
 { "&lt.", false },        // lt,                less than
 { "&bxh.", false },       // macr,              macron
 { "", true  },           // male,              male
 { "&mu.", false },        // micro,             micro prefix
 { "&minus.", false },     // minus,             minus sign
 { "  ", true },           // mspace,            non-breaking space, "m" width
 { "&mu.", false },        // mu,                mu
 { "&nearly.", false },    // nearly,            nearly equal to
 { " ", true },            // nspace,            non-breaking space, "n" width
 { "&nt.", false },        // nt,                n tilde
 { "&Nt.", false },        // Nt,                N tilde
 { "&numsign.", false },   // numsign,           number sign
 { "&oa.", false },        // oa,                o acute
 { "O", true  },           // Oa,                O acute
 { "&oc.", false },        // oc,                o circumflex
 { "O", true  },           // Oc,                O circumflex
 { "&odq.", false },       // odq,               open double quote
 { "&odqf.", false },      // odqf,              open French double quote
 { "&oe.", false },        // oe,                o umlaut
 { "&Oe.", false },        // Oe,                O umlaut
 { "&og.", false },        // og,                o grave
 { "O", true  },           // Og,                O grave
 { "O", true  },           // Oslash,            O slash
 { "o", true  },           // oslash,            o slash
 { "&osq.", false },       // osq,               open single quote
 { "O", true  },           // Ot,                O tilde
 { "o", true  },           // ot,                o tilde
 { "&ous.", false },       // ous,               underscored o
 { "", true  },           // para,              paragraph symbol
 { "&per.", false },       // per,               period
 { "&percent.", false },   // percent,           percent
 { "&phi.", false },       // phi,               lowercase phi
 { "&Phi.", false },       // Phi,               uppercase Phi
 { "&pi.", false },        // pi,                pi
 { "&plus.", false },      // plus,              plus sign
 { "&plusmin.", false },   // plusmin,           plusminus
 { "\"", true  },          // quot,              double quote
 { "&rahead.", false },    // rahead,            right arrowhead
 { "&rarrow.", false },    // rarrow,            right arrow
 { "&rbl.", false },       // rbl,               required blank (non-breaking)
 { "&rbrace.", false },    // rbrace,            right brace
 { "&rbracket.", false },  // rbracket,          right bracket
 { "(R)", true  },         // reg,               registered sign
 { "-", true  },           // rhy,               required hyphen (non-breaking)
 { "&rpar.", false },      // rpar,              right parenthesis
 { "", true  },           // sect,              section sign
 { "&semi.", false },      // semi,              semicolon
 { "", true  },            // shy,               soft hyphen
 { "&sigma.", false },     // sigma,             lowercase sigma
 { "&Sigma.", false },     // Sigma,             uppercase Sigma
 { "&similar.", false },   // similar,           similar to
 { "&slash.", false },     // slash,             slash
 { "&splitvbar.", false }, // splitvbar,         split vertical bar (piping symbol)
 { "&sqbul.", false },     // sqbul,             square bullet
 { "&sqrt.", false },      // sqrt,              square root
 { "&caret.1", true  },    // sup1,              superscript 1
 { "&sup2.", false },      // sup2,              superscript 2
 { "&caret.3", true  },    // sup3,              superscript 3
 { "&supn.", false },      // supn,              superscript n
 { "&Beta.", false },      // szlig,             sharp s, German sz ligature
 { "&tau.", false },       // tau,               tau
 { "&Theta.", false },     // Theta,             Theta
 { "T", true  },           // THORN,             THORN, Icelandic
 { "t", true  },           // thorn,             thorn, Icelandic
 { "&tilde.", false },     // tilde,             tilde
 { "&asterisk.", true  },  // times,             multiplication sign
 { "&ua.", false },        // ua,                u acute
 { "U", true  },           // Ua,                U acute
 { "&uarrow.", false },    // uarrow,            up arrow
 { "&uc.", false },        // uc,                u circumflex
 { "U", true  },           // Uc,                U circumflex
 { "&ue.", false },        // ue,                u umlaut
 { "&Ue.", false },        // Ue,                U umlaut
 { "&ug.", false },        // ug,                u grave
 { "U", true  },           // Ug,                U grave
 { "", true  },            // uml,               umlaut
 { "&us.", false },        // us,                underscore
 { "&vbar.", false },      // vbar,              solid vertical bar
 { "&xclm.", false },      // xclm,              exclamation point
 { "Y", true  },           // Ya,                Y acute
 { "y", true  },           // yacute,            y acute
 { "&ye.", false },        // ye,                y umlaut
 { "&yen.", false },       // yen                Yen
 { "&Omega.", false }      // Omega              Omega
};


static const IPFSymbol * findSymbol( const Symbol & symbol )
{
  Symbol::Identifier id = symbol.id();
  IASSERTPARM( id <= Symbol::endOfList );
  return id? symtable + id - Symbol::startOfList: 0;
}


/***************************************************************************
 * Procedure.. IPFGenerator::operator<<( const Symbol )
 * Author..... Mat
 * Date....... 11/16/95
 *
 * Inserts the given symbol as text in the output stream.  Not all
 * Generator symbols are supported by IPF, so we use what we have and try
 * to use characters or symbols that make sense.
 ***************************************************************************/
void IPFGenerator::handleSymbol( const SymbolGin & gin )
{
  // ignore in comment mode
  if ( ! _next->isComment )
  {
    const IPFSymbol *  ipf = findSymbol( gin.symbol() );
    Boolean            isChApprox;
    unsigned char      ch = gin.symbol().asChar( _next->outputCodepage, &isChApprox );

    // send output from table, or send translated char
    // - translated char is preferable to an "alternative" IPF output
    // - however, "alternative" IPF output is preferable to "approximate" translation
    if ( ipf && ( ! ipf->isAlt || ! ch || isChApprox ) )
      sendText( ipf->output, noSymbolCheck );
    else if ( ch )
      sendText( ch, 0 );  // send translated character
    else
      sendText( CodePage::substitution, 0 );  // translation failed, send "substitution" character
  }
}


/***************************************************************************
 * Procedure.. IPFGenerator::translateChar
 * Author..... Mat
 * Date....... 12/15/98
 *
 * Translate a character from the current document code page to an IPF
 * symbol name if possible (return the symbol name).  If not possible,
 * return false.
 ***************************************************************************/
Boolean IPFGenerator::translateChar( unsigned char ch, IString & symbolName ) const
{
  const IPFSymbol *  ipf = findSymbol( Symbol( ch, _docCodePage ) );
  if ( ipf && ! ipf->isAlt )
  {
    symbolName = ipf->output;
    return true;
  }

  return false;
}


