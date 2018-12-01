/***************************************************************************
 * File...... Gin.ipp
 * Author.... Mat
 * Date...... 4/16/98
 *
 * Inline functions for Gins
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

//
// DOCUMENT CONTROL --------------------------------------------------------
//

/***************************************************************************
 * Class...... SectionGin
 ***************************************************************************/

inline SectionGin & SectionGin::setTitle( const IString & title )
{ _title = title; return *this; }

inline SectionGin & SectionGin::setWindow( int windowId )
{ _windowId = windowId; return *this; }

inline const IString & SectionGin::title() const
{ return _title; }

inline int             SectionGin::level() const
{ return _level; }

inline Boolean         SectionGin::isListed() const
{ return _isListed; }

inline int             SectionGin::windowId() const
{ return _windowId; }


/***************************************************************************
 * Class...... KeywordGin
 ***************************************************************************/

inline KeywordGin & KeywordGin::setExternal( Boolean isExternal )
{ _isExternal = isExternal; return *this; }

inline const IString & KeywordGin::text() const
{ return _text; }

inline Boolean         KeywordGin::isListed() const
{ return _isListed; }

inline Boolean         KeywordGin::isExternal() const
{ return _isExternal; }


/***************************************************************************
 * Class...... LabelGin
 ***************************************************************************/

inline LabelGin & LabelGin::setExternal( Boolean isExternal )
{ _isExternal = isExternal; return *this; }

inline LabelGin & LabelGin::setId( int id )
{ _id = id; return *this; }

inline const IString & LabelGin::name() const
{ return _name; }

inline Boolean         LabelGin::isExternal() const
{ return _isExternal; }

inline int             LabelGin::id() const
{ return _id; }


//
// PARAGRAPH CONTROL -------------------------------------------------------
//


/***************************************************************************
 * Class...... MarginGin
 ***************************************************************************/

inline Boolean          MarginGin::isRelative() const
{ return _isRelative; }

inline const Distance & MarginGin::margin() const
{ return _margin; }


/***************************************************************************
 * Class...... BorderGin
 ***************************************************************************/

inline int BorderGin::borders() const
{ return _borders; }


/***************************************************************************
 * Class...... AlignGin
 ***************************************************************************/

inline AlignGin::Alignment AlignGin::alignment() const
{ return _alignment; }


/***************************************************************************
 * Class...... SpacingGin
 ***************************************************************************/

inline const Distance & SpacingGin::spacing() const
{ return _spacing; }


/***************************************************************************
 * Class...... RulerGin
 ***************************************************************************/

inline RulerGin & RulerGin::setLineBreak( LineBreak lineBreak )
{ _lineBreak = lineBreak; return *this; }

inline RulerGin::Mode             RulerGin::mode() const
{ return _mode; }

inline RulerGin::LineBreak        RulerGin::lineBreak() const
{ return _lineBreak; }


/***************************************************************************
 * Class...... OutlineGin
 ***************************************************************************/

inline OutlineGin & OutlineGin::setPrefix( const IString & prefix )
{ _prefix = prefix; return *this; }

inline OutlineGin & OutlineGin::setSuffix( const IString & suffix )
{ _suffix = suffix; return *this; }

inline OutlineGin::Style           OutlineGin::style() const
{ return _style; }

inline const IString & OutlineGin::prefix() const
{ return _prefix; }

inline const IString & OutlineGin::suffix() const
{ return _suffix; }



//
// CHARACTER CONTROL -------------------------------------------------------
//


/***************************************************************************
 * Class...... FontGin
 ***************************************************************************/

inline FontGin & FontGin::setCodepage( CodePage codepage )
{ _codepage = codepage; return *this; }

inline FontInfo::Family          FontGin::family() const
{ return _family; }

inline const IString & FontGin::facename() const
{ return _facename; }

inline CodePage FontGin::codepage() const
{ return _codepage; }


/***************************************************************************
 * Class...... SizeGin
 ***************************************************************************/

inline const Distance & SizeGin::height() const
{ return _height; }


/***************************************************************************
 * Class...... ColorGin
 ***************************************************************************/

inline const IColor & ColorGin::color() const
{ return _color; }


/***************************************************************************
 * Class...... AttributeGin
 ***************************************************************************/

inline Boolean AttributeGin::isOn() const
{ return _isOn; }


/***************************************************************************
 * Class...... LinkGin
 ***************************************************************************/

inline LinkGin & LinkGin::setFile( const IString & file )
{ _file = file; return *this; }

inline const IString & LinkGin::file() const
{ return _file; }

inline const IString & LinkGin::target() const
{ return _target; }


/***************************************************************************
 * Class...... HideGin
 ***************************************************************************/

inline const IString & HideGin::key() const
{ return _key; }


//
// ARTWORK -----------------------------------------------------------------
//


/***************************************************************************
 * Class...... ArtGin
 ***************************************************************************/

inline const IString & ArtGin::id() const
{ return _id; }

inline ArtGin::ArtSpot    ArtGin::spot() const
{ return _spot; }


//
// TEXT --------------------------------------------------------------------
//

/***************************************************************************
 * Class...... TextGin
 ***************************************************************************/

inline const IString & TextGin::string() const
{ return _string; }


/***************************************************************************
 * Class...... SymbolGin
 ***************************************************************************/

inline const Symbol & SymbolGin::symbol() const
{ return _symbol; }


