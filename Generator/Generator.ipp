/***************************************************************************
 * File...... Generator.ipp
 * Author.... Mat
 * Date...... 11/11/97
 *
 * Inline functions for Generator.  These empty functions provide a null
 * default action for all settings and Gins.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/

inline Registration & Generator::registration()
{ return _registration; }

inline Generator & Generator::setTitle( const IString & title )
{ return *this; }

inline Generator & Generator::setSubject( const IString & subject )
{ return *this; }

inline Generator & Generator::setVersion( const IString & version )
{ return *this; }

inline Generator & Generator::setCopyright( const IString & copyright )
{ return *this; }

inline Generator & Generator::setAuthor( const IString & author )
{ return *this; }

inline Generator & Generator::setLanguage( Language language )
{ return *this; }

inline Generator & Generator::setCodePage( CodePage cp )
{ return *this; }

inline Generator & Generator::addKeyword( const IString & keyword )
{ return *this; }

inline void Generator::handleSection( const SectionGin & )
{}

inline void Generator::handleKeyword( const KeywordGin & )
{}

inline void Generator::handleLabel( const LabelGin & )
{}

inline void Generator::handlePara( const ParaGin & )
{}

inline void Generator::handleLine( const LineGin & )
{}

inline void Generator::handlePush( const PushGin & )
{}

inline void Generator::handlePop( const PopGin & )
{}

inline void Generator::handleRightMargin( const RightMarginGin & )
{}

inline void Generator::handleLeftMargin( const LeftMarginGin & )
{}

inline void Generator::handleBorder( const BorderGin & )
{}

inline void Generator::handleAlign( const AlignGin & )
{}

inline void Generator::handleSpacing( const SpacingGin & )
{}

inline void Generator::handleRuler( const RulerGin & )
{}

inline void Generator::handleOutline( const OutlineGin & )
{}

inline void Generator::handleFont( const FontGin & )
{}

inline void Generator::handleSize( const SizeGin & )
{}

inline void Generator::handleColor( const ColorGin & )
{}

inline void Generator::handleBackColor( const BackColorGin & )
{}

inline void Generator::handleBold( const BoldGin & )
{}

inline void Generator::handleItalic( const ItalicGin & )
{}

inline void Generator::handleUnderline( const UnderlineGin & )
{}

inline void Generator::handleComment( const CommentGin & )
{}

inline void Generator::handleLink( const LinkGin & )
{}

inline void Generator::handleHide( const HideGin & )
{}

inline void Generator::handleText( const TextGin & )
{}

inline void Generator::handleSymbol( const SymbolGin & )
{}

