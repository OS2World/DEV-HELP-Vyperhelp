/***************************************************************************
 * File...... SymbolLookup.cpp
 * Author.... Mat
 * Date...... 10/13/98
 *
 * Implementation of SymbolLookup and SymbolLookupSet
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// Generator
#include "SymbolLookup.hpp"


//
// TEMPLATE DEFINITIONS
//
// These are picked out of the tempinc directory generated by the compiler.
// Since we are creating a LIB, the tempinc files never get linked in.
// We only need to include template instances that are private to the class.
#include <iavlkss.c>
#include <ikskss.c>

// IKeySet< SymbolLookup, IString >
#pragma define(IGAvlKeySortedSetNode<SymbolLookup,IString,IKCOps<SymbolLookup,IString> >)
#pragma define(IGAvlKeySortedSetCursor<SymbolLookup,IString,IKCOps<SymbolLookup,IString> >)
#pragma define(IGAvlKSS<SymbolLookup,IString,IKCOps<SymbolLookup,IString> >)
#pragma define(IWKeySetOnKSSetCursor<SymbolLookup,IString,IKCOps<SymbolLookup,IString>,IGAvlKSS<SymbolLookup,IString,IKCOps<SymbolLookup,IString> > >)
#pragma define(IWKSOnKSS<SymbolLookup,IString,IKCOps<SymbolLookup,IString>,IGAvlKSS<SymbolLookup,IString,IKCOps<SymbolLookup,IString> > >)


SymbolLookup::SymbolLookup( const IString & name, Symbol::Identifier id ):
  _name( name ),
  _id( id )
{}


SymbolLookup::SymbolLookup( const IString & name, const IString & macro ):
  _name( name ),
  _id( Symbol::unknown ),
  _macro( macro )
{}


SymbolLookupSet::SymbolLookupSet( INumber numberOfElements ):
  IKeySet< SymbolLookup, IString >( numberOfElements )
{
  // add all the standard IPF symbols
  add( SymbolLookup( "aa", Symbol::aa ) );
  add( SymbolLookup( "ac", Symbol::ac ) );
  add( SymbolLookup( "ae", Symbol::ae ) );
  add( SymbolLookup( "Ae", Symbol::Ae ) );
  add( SymbolLookup( "ag", Symbol::ag ) );
  add( SymbolLookup( "aelig", Symbol::aelig ) );
  add( SymbolLookup( "AElig", Symbol::AElig ) );
  add( SymbolLookup( "alpha", Symbol::alpha ) );
  add( SymbolLookup( "Alpha", Symbol::Alpha ) );
  add( SymbolLookup( "amp", Symbol::amp ) );
  add( SymbolLookup( "and", Symbol::and ) );
  add( SymbolLookup( "angstrom", Symbol::angstrom ) );
  add( SymbolLookup( "ao", Symbol::ao ) );
  add( SymbolLookup( "Ao", Symbol::Ao ) );
  add( SymbolLookup( "apos", Symbol::apos ) );
  add( SymbolLookup( "bx2022", Symbol::bx2022 ) );
  add( SymbolLookup( "bx2020", Symbol::bx2020 ) );
  add( SymbolLookup( "bx0022", Symbol::bx0022 ) );
  add( SymbolLookup( "bx2002", Symbol::bx2002 ) );
  add( SymbolLookup( "bx2200", Symbol::bx2200 ) );
  add( SymbolLookup( "bx0220", Symbol::bx0220 ) );
  add( SymbolLookup( "bx2202", Symbol::bx2202 ) );
  add( SymbolLookup( "bx0222", Symbol::bx0222 ) );
  add( SymbolLookup( "bx2220", Symbol::bx2220 ) );
  add( SymbolLookup( "bx0202", Symbol::bx0202 ) );
  add( SymbolLookup( "bx2222", Symbol::bx2222 ) );
  add( SymbolLookup( "bx1012", Symbol::bx1012 ) );
  add( SymbolLookup( "bx2021", Symbol::bx2021 ) );
  add( SymbolLookup( "bx0021", Symbol::bx0021 ) );
  add( SymbolLookup( "bx0012", Symbol::bx0012 ) );
  add( SymbolLookup( "bx2001", Symbol::bx2001 ) );
  add( SymbolLookup( "bx1002", Symbol::bx1002 ) );
  add( SymbolLookup( "bx1210", Symbol::bx1210 ) );
  add( SymbolLookup( "bx2120", Symbol::bx2120 ) );
  add( SymbolLookup( "bx1202", Symbol::bx1202 ) );
  add( SymbolLookup( "bx1201", Symbol::bx2101 ) );  // NOTE: this is mis-named by IPF
  add( SymbolLookup( "bx0212", Symbol::bx0212 ) );
  add( SymbolLookup( "bx0121", Symbol::bx0121 ) );
  add( SymbolLookup( "bx2100", Symbol::bx2100 ) );
  add( SymbolLookup( "bx1200", Symbol::bx1200 ) );
  add( SymbolLookup( "bx0210", Symbol::bx0210 ) );
  add( SymbolLookup( "bx0120", Symbol::bx0120 ) );
  add( SymbolLookup( "bx2121", Symbol::bx2121 ) );
  add( SymbolLookup( "bx1212", Symbol::bx1212 ) );
  add( SymbolLookup( "asterisk", Symbol::asterisk ) );
  add( SymbolLookup( "atsign", Symbol::atsign ) );
  add( SymbolLookup( "bslash", Symbol::bslash ) );
  add( SymbolLookup( "bsl", Symbol::bslash ) );
  add( SymbolLookup( "Beta", Symbol::Beta ) );
  add( SymbolLookup( "bxas", Symbol::bxas ) );
  add( SymbolLookup( "bxbj", Symbol::bxas ) );
  add( SymbolLookup( "bxcr", Symbol::bxcr ) );
  add( SymbolLookup( "bxcj", Symbol::bxcr ) );
  add( SymbolLookup( "bxde", Symbol::bxde ) );
  add( SymbolLookup( "bxtj", Symbol::bxde ) );
  add( SymbolLookup( "bxh", Symbol::bxh ) );
  add( SymbolLookup( "bxll", Symbol::bxll ) );
  add( SymbolLookup( "bxlr", Symbol::bxlr ) );
  add( SymbolLookup( "bxri", Symbol::bxri ) );
  add( SymbolLookup( "bxrj", Symbol::bxri ) );
  add( SymbolLookup( "bxle", Symbol::bxle ) );
  add( SymbolLookup( "bxlj", Symbol::bxle ) );
  add( SymbolLookup( "bxul", Symbol::bxul ) );
  add( SymbolLookup( "bxur", Symbol::bxur ) );
  add( SymbolLookup( "bxv", Symbol::bxv ) );
  add( SymbolLookup( "bullet", Symbol::bullet ) );
  add( SymbolLookup( "bul", Symbol::bullet ) );
  add( SymbolLookup( "cc", Symbol::cc ) );
  add( SymbolLookup( "Cc", Symbol::Cc ) );
  add( SymbolLookup( "caret", Symbol::caret ) );
  add( SymbolLookup( "cent", Symbol::cent ) );
  add( SymbolLookup( "cdq", Symbol::cdq ) );
  add( SymbolLookup( "cdqf", Symbol::cdqf ) );
  add( SymbolLookup( "csq", Symbol::csq ) );
  add( SymbolLookup( "comma", Symbol::comma ) );
  add( SymbolLookup( "colon", Symbol::colon ) );
  add( SymbolLookup( "dash", Symbol::dash ) );
  add( SymbolLookup( "degree", Symbol::degree ) );
  add( SymbolLookup( "deg", Symbol::degree ) );
  add( SymbolLookup( "delta", Symbol::delta ) );
  add( SymbolLookup( "divide", Symbol::divide ) );
  add( SymbolLookup( "dollar", Symbol::dollar ) );
  add( SymbolLookup( "dot", Symbol::dot ) );
  add( SymbolLookup( "darrow", Symbol::darrow ) );
  add( SymbolLookup( "ea", Symbol::ea ) );
  add( SymbolLookup( "Ea", Symbol::Ea ) );
  add( SymbolLookup( "ec", Symbol::ec ) );
  add( SymbolLookup( "ee", Symbol::ee ) );
  add( SymbolLookup( "eg", Symbol::eg ) );
  add( SymbolLookup( "emdash", Symbol::emdash ) );
  add( SymbolLookup( "endash", Symbol::endash ) );
  add( SymbolLookup( "epsilon", Symbol::epsilon ) );
  add( SymbolLookup( "eq", Symbol::eq ) );
  add( SymbolLookup( "equals", Symbol::eq ) );
  add( SymbolLookup( "eqsym", Symbol::eq ) );
  add( SymbolLookup( "xclm", Symbol::xclm ) );
  add( SymbolLookup( "xclam", Symbol::xclm ) );
  add( SymbolLookup( "fnof", Symbol::fnof ) );
  add( SymbolLookup( "Gamma", Symbol::Gamma ) );
  add( SymbolLookup( "grave", Symbol::grave ) );
  add( SymbolLookup( "gt", Symbol::gt ) );
  add( SymbolLookup( "gtsym", Symbol::gt ) );
  add( SymbolLookup( "ge", Symbol::ge ) );
  add( SymbolLookup( "gesym", Symbol::ge ) );
  add( SymbolLookup( "house", Symbol::house ) );
  add( SymbolLookup( "hyphen", Symbol::hyphen ) );
  add( SymbolLookup( "ia", Symbol::ia ) );
  add( SymbolLookup( "ic", Symbol::ic ) );
  add( SymbolLookup( "ie", Symbol::ie ) );
  add( SymbolLookup( "ig", Symbol::ig ) );
  add( SymbolLookup( "identical", Symbol::identical ) );
  add( SymbolLookup( "infinity", Symbol::infinity ) );
  add( SymbolLookup( "intersect", Symbol::intersect ) );
  add( SymbolLookup( "inttop", Symbol::inttop ) );
  add( SymbolLookup( "intbot", Symbol::intbot ) );
  add( SymbolLookup( "inve", Symbol::inve ) );
  add( SymbolLookup( "invq", Symbol::invq ) );
  add( SymbolLookup( "larrow", Symbol::larrow ) );
  add( SymbolLookup( "lahead", Symbol::lahead ) );
  add( SymbolLookup( "lbrace", Symbol::lbrace ) );
  add( SymbolLookup( "lbrc", Symbol::lbrace ) );
  add( SymbolLookup( "lbracket", Symbol::lbracket ) );
  add( SymbolLookup( "lbrk", Symbol::lbracket ) );
  add( SymbolLookup( "lpar", Symbol::lpar ) );
  add( SymbolLookup( "lparen", Symbol::lpar ) );
  add( SymbolLookup( "lt", Symbol::lt ) );
  add( SymbolLookup( "ltsym", Symbol::lt ) );
  add( SymbolLookup( "le", Symbol::le ) );
  add( SymbolLookup( "lesym", Symbol::le ) );
  add( SymbolLookup( "lnot", Symbol::lnot ) );
  add( SymbolLookup( "lnotrev", Symbol::lnotrev ) );
  add( SymbolLookup( "lor", Symbol::lor ) );
  add( SymbolLookup( "mdash", Symbol::emdash ) );
  add( SymbolLookup( "minus", Symbol::minus ) );
  add( SymbolLookup( "mu", Symbol::mu ) );
  add( SymbolLookup( "ndash", Symbol::endash ) );
  add( SymbolLookup( "nt", Symbol::nt ) );
  add( SymbolLookup( "Nt", Symbol::Nt ) );
  add( SymbolLookup( "nearly", Symbol::nearly ) );
  add( SymbolLookup( "notsym", Symbol::lnot ) );
  add( SymbolLookup( "numsign", Symbol::numsign ) );
  add( SymbolLookup( "oa", Symbol::oa ) );
  add( SymbolLookup( "oc", Symbol::oc ) );
  add( SymbolLookup( "og", Symbol::og ) );
  add( SymbolLookup( "oe", Symbol::oe ) );
  add( SymbolLookup( "Oe", Symbol::Oe ) );
  add( SymbolLookup( "Omega", Symbol::Omega ) );
  add( SymbolLookup( "frac14", Symbol::frac14 ) );
  add( SymbolLookup( "frac12", Symbol::frac12 ) );
  add( SymbolLookup( "odq", Symbol::odq ) );
  add( SymbolLookup( "odqf", Symbol::odqf ) );
  add( SymbolLookup( "osq", Symbol::osq ) );
  add( SymbolLookup( "percent", Symbol::percent ) );
  add( SymbolLookup( "per", Symbol::per ) );
  add( SymbolLookup( "period", Symbol::per ) );
  add( SymbolLookup( "phi", Symbol::phi ) );
  add( SymbolLookup( "Phi", Symbol::Phi ) );
  add( SymbolLookup( "pi", Symbol::pi ) );
  add( SymbolLookup( "plus", Symbol::plus ) );
  add( SymbolLookup( "plusmin", Symbol::plusmin ) );
  add( SymbolLookup( "pm", Symbol::plusmin ) );
  add( SymbolLookup( "Lsterling", Symbol::Lsterling ) );
  add( SymbolLookup( "rbl", Symbol::rbl ) );
  add( SymbolLookup( "rarrow", Symbol::rarrow ) );
  add( SymbolLookup( "rahead", Symbol::rahead ) );
  add( SymbolLookup( "rbrace", Symbol::rbrace ) );
  add( SymbolLookup( "rbrc", Symbol::rbrace ) );
  add( SymbolLookup( "rbracket", Symbol::rbracket ) );
  add( SymbolLookup( "rbrk", Symbol::rbracket ) );
  add( SymbolLookup( "rpar", Symbol::rpar ) );
  add( SymbolLookup( "rparen", Symbol::rpar ) );
  add( SymbolLookup( "semi", Symbol::semi ) );
  add( SymbolLookup( "box14", Symbol::box14 ) );
  add( SymbolLookup( "box12", Symbol::box12 ) );
  add( SymbolLookup( "box34", Symbol::box34 ) );
  add( SymbolLookup( "Sigma", Symbol::Sigma ) );
  add( SymbolLookup( "sigma", Symbol::sigma ) );
  add( SymbolLookup( "similar", Symbol::similar ) );
  add( SymbolLookup( "slash", Symbol::slash ) );
  add( SymbolLookup( "slr", Symbol::slash ) );
  add( SymbolLookup( "BOX", Symbol::BOX ) );
  add( SymbolLookup( "BOXBOT", Symbol::BOXBOT ) );
  add( SymbolLookup( "BOXTOP", Symbol::BOXTOP ) );
  add( SymbolLookup( "BOXLEFT", Symbol::BOXLEFT ) );
  add( SymbolLookup( "BOXRIGHT", Symbol::BOXRIGHT ) );
  add( SymbolLookup( "splitvbar", Symbol::splitvbar ) );
  add( SymbolLookup( "sqbul", Symbol::sqbul ) );
  add( SymbolLookup( "sqrt", Symbol::sqrt ) );
  add( SymbolLookup( "sup2", Symbol::sup2 ) );
  add( SymbolLookup( "supn", Symbol::supn ) );
  add( SymbolLookup( "tau", Symbol::tau ) );
  add( SymbolLookup( "tilde", Symbol::tilde ) );
  add( SymbolLookup( "Theta", Symbol::Theta ) );
  add( SymbolLookup( "ua", Symbol::ua ) );
  add( SymbolLookup( "uc", Symbol::uc ) );
  add( SymbolLookup( "ug", Symbol::ug ) );
  add( SymbolLookup( "ue", Symbol::ue ) );
  add( SymbolLookup( "Ue", Symbol::Ue ) );
  add( SymbolLookup( "us", Symbol::us ) );
  add( SymbolLookup( "aus", Symbol::aus ) );
  add( SymbolLookup( "ous", Symbol::ous ) );
  add( SymbolLookup( "uarrow", Symbol::uarrow ) );
  add( SymbolLookup( "vbar", Symbol::vbar ) );
  add( SymbolLookup( "ye", Symbol::ye ) );
  add( SymbolLookup( "yen", Symbol::yen ) );
}

