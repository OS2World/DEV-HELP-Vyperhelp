/***************************************************************************
 * File...... Gin.cpp
 * Author.... Mat
 * Date...... 10/13/95
 *
 * Implementation for Gin (Generator Input) objects.
 * Most are simple data holders, with only constructor and accessors.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>

// Generator
#include "Generator.hpp"
#include "ColumnList.hpp"



//
// GIN ---------------------------------------------------------------------
//

Gin::~Gin()
{}


//
// GIN LIST ----------------------------------------------------------------
//

/***************************************************************************
 * Procedure.. GinList::~GinList()
 * Author..... Mat
 * Date....... 4/27/99
 *
 * Destructor: removes all elements
 ***************************************************************************/
GinList::~GinList()
{
  // make sure all elements are removed (and deleted)
  removeAll();
}


/***************************************************************************
 * Procedure.. GinList::streamTo
 * Author..... Mat
 * Date....... 12/1/95
 *
 * This simply streams each Gin in the list in the order they were added.
 ***************************************************************************/
void GinList::streamTo( Generator & gen ) const
{
  GinList::Cursor cursor( *this );

  forCursor( cursor )
    cursor.element()->streamTo( gen );
}



//
// DOCUMENT CONTROL --------------------------------------------------------
//

/***************************************************************************
 * Class...... SectionGin
 ***************************************************************************/
SectionGin::SectionGin( int level, Boolean isListed ):
    _level( level ),
    _isListed( isListed ),
    _windowId( 0 )
{}

void SectionGin::streamTo( Generator & gen ) const
{
  // insert promotional text at end of section (if in trial mode)
  gen.nag();

  gen.handleSection( *this );
}



/***************************************************************************
 * Class...... KeywordGin
 ***************************************************************************/
KeywordGin::KeywordGin( const IString & text, Boolean isListed ):
    _text( text ),
    _isListed( isListed ),
    _isExternal( false )
{}

void KeywordGin::streamTo( Generator & gen ) const
{ gen.handleKeyword( *this ); }


/***************************************************************************
 * Class...... LabelGin
 ***************************************************************************/
LabelGin::LabelGin( const IString & name ):
    _name( name ),
    _id( 0 ),
    _isExternal( false )
{}

void LabelGin::streamTo( Generator & gen ) const
{ gen.handleLabel( *this ); }


/***************************************************************************
 * Class...... RowGin
 ***************************************************************************/
void RowGin::streamTo( Generator & gen ) const
{ gen.handleRow( *this ); }


/***************************************************************************
 * Class...... ColumnGin
 ***************************************************************************/
void ColumnGin::streamTo( Generator & gen ) const
{ gen.handleColumn( *this ); }


/***************************************************************************
 * Class...... ItemGin
 ***************************************************************************/
void ItemGin::streamTo( Generator & gen ) const
{ gen.handleItem( *this ); }


/***************************************************************************
 * Class...... ParaGin
 ***************************************************************************/
void ParaGin::streamTo( Generator & gen ) const
{ gen.handlePara( *this ); }


/***************************************************************************
 * Class...... LineGin
 ***************************************************************************/
void LineGin::streamTo( Generator & gen ) const
{ gen.handleLine( *this ); }



//
// NESTING CONTROL ---------------------------------------------------------
//


/***************************************************************************
 * Class...... PushGin
 ***************************************************************************/
void PushGin::streamTo( Generator & gen ) const
{ gen.handlePush( *this ); }


/***************************************************************************
 * Class...... PopGin
 ***************************************************************************/
void PopGin::streamTo( Generator & gen ) const
{ gen.handlePop( *this ); }



//
// PARAGRAPH CONTROL -------------------------------------------------------
//



/***************************************************************************
 * Class...... MarginGin
 ***************************************************************************/
MarginGin::MarginGin( Boolean isRelative, const Distance & margin ):
    _isRelative( isRelative ),
    _margin( margin )
{}

RightMarginGin::RightMarginGin( Boolean isRelative, const Distance & margin ):
    MarginGin( isRelative, margin )
{}

void RightMarginGin::streamTo( Generator & gen ) const
{ gen.handleRightMargin( *this ); }

LeftMarginGin::LeftMarginGin( Boolean isRelative, const Distance & margin ):
    MarginGin( isRelative, margin )
{}

void LeftMarginGin::streamTo( Generator & gen ) const
{ gen.handleLeftMargin( *this ); }


/***************************************************************************
 * Class...... BorderGin
 ***************************************************************************/
BorderGin::BorderGin( int borders ):
    _borders( borders )
{}

void BorderGin::streamTo( Generator & gen ) const
{ gen.handleBorder( *this ); }


/***************************************************************************
 * Class...... AlignGin
 ***************************************************************************/
AlignGin::AlignGin( Alignment alignment ):
    _alignment( alignment )
{}

void AlignGin::streamTo( Generator & gen ) const
{ gen.handleAlign( *this ); }


/***************************************************************************
 * Class...... SpacingGin
 ***************************************************************************/
SpacingGin::SpacingGin( const Distance & spacing ):
    _spacing( spacing )
{}

void SpacingGin::streamTo( Generator & gen ) const
{ gen.handleSpacing( *this ); }


/***************************************************************************
 * Class...... RulerGin
 ***************************************************************************/
RulerGin::RulerGin( Mode mode ):
    _mode( mode ),
    _lineBreak( none ),
    _columns( new ColumnList )
{
  IASSERTSTATE( _columns );
}

RulerGin::~RulerGin()
{
  delete _columns;
}

RulerGin & RulerGin::addColumn( const Distance & column )
{
  Boolean ok = _columns->add( column );
  IASSERTSTATE( ok );
  return *this;
}

const Distance & RulerGin::column( IPosition index ) const
{ return _columns->elementAtPosition( index ); }

INumber          RulerGin::numberOfColumns() const
{ return _columns->numberOfElements(); }

void RulerGin::streamTo( Generator & gen ) const
{ gen.handleRuler( *this ); }


/***************************************************************************
 * Class...... OutlineGin
 ***************************************************************************/
OutlineGin::OutlineGin( Style style ):
    _style( style )
{}

void OutlineGin::streamTo( Generator & gen ) const
{ gen.handleOutline( *this ); }


/***************************************************************************
 * Class...... DividerGin
 ***************************************************************************/

void DividerGin::streamTo( Generator & gen ) const
{ gen.handleDivider( *this ); }



//
// CHARACTER CONTROL -------------------------------------------------------
//


/***************************************************************************
 * Class...... FontGin
 ***************************************************************************/
FontGin::FontGin( FontInfo::Family family, const IString & facename ):
    _family( family ),
    _facename( facename )
{}

void FontGin::streamTo( Generator & gen ) const
{ gen.handleFont( *this ); }


/***************************************************************************
 * Class...... SizeGin
 ***************************************************************************/
SizeGin::SizeGin( const Distance & height ):
    _height( height )
{}

void SizeGin::streamTo( Generator & gen ) const
{ gen.handleSize( *this ); }


/***************************************************************************
 * Class...... ColorGin
 ***************************************************************************/
ColorGin::ColorGin( const IColor & color ):
    _color( color )
{}

void ColorGin::streamTo( Generator & gen ) const
{ gen.handleColor( *this ); }

BackColorGin::BackColorGin( const IColor & color ):
    ColorGin( color )
{}

void BackColorGin::streamTo( Generator & gen ) const
{ gen.handleBackColor( *this ); }


/***************************************************************************
 * Class...... AttributeGin
 ***************************************************************************/
AttributeGin::AttributeGin( Boolean isOn ):
    _isOn( isOn )
{}

BoldGin::BoldGin( Boolean isOn ):
    AttributeGin( isOn )
{}

void BoldGin::streamTo( Generator & gen ) const
{ gen.handleBold( *this ); }

ItalicGin::ItalicGin( Boolean isOn ):
    AttributeGin( isOn )
{}

void ItalicGin::streamTo( Generator & gen ) const
{ gen.handleItalic( *this ); }

UnderlineGin::UnderlineGin( Boolean isOn ):
    AttributeGin( isOn )
{}

void UnderlineGin::streamTo( Generator & gen ) const
{ gen.handleUnderline( *this ); }

CommentGin::CommentGin( Boolean isOn ):
    AttributeGin( isOn )
{}

void CommentGin::streamTo( Generator & gen ) const
{ gen.handleComment( *this ); }


/***************************************************************************
 * Class...... LinkGin
 ***************************************************************************/
LinkGin::LinkGin( const IString & target ):
    AttributeGin( true ),
    _target( target )
{}

LinkGin::LinkGin():
    AttributeGin( false )
{}

Boolean         LinkGin::isExternal() const
{ return ( _file.length() > 0 ); }

void LinkGin::streamTo( Generator & gen ) const
{ gen.handleLink( *this ); }


/***************************************************************************
 * Class...... HideGin
 ***************************************************************************/
HideGin::HideGin( const IString & key ):
    AttributeGin( true ),
    _key( key )
{}

HideGin::HideGin():
    AttributeGin( false )
{}

void HideGin::streamTo( Generator & gen ) const
{ gen.handleHide( *this ); }


//
// ARTWORK -----------------------------------------------------------------
//


/***************************************************************************
 * Class...... ArtGin
 ***************************************************************************/
ArtGin::ArtGin( const IString & id, ArtSpot spot ):
    _id( id ),
    _spot( spot )
{}

void ArtGin::streamTo( Generator & gen ) const
{ gen.handleArt( *this ); }


//
// TEXT --------------------------------------------------------------------
//

/***************************************************************************
 * Class...... TextGin
 ***************************************************************************/
TextGin::TextGin( const IString & string ):
    _string( string )
{}

void TextGin::streamTo( Generator & gen ) const
{ gen.handleText( *this ); }


/***************************************************************************
 * Class...... SymbolGin
 ***************************************************************************/
SymbolGin::SymbolGin( const Symbol & symbol ):
    _symbol( symbol )
{}

void SymbolGin::streamTo( Generator & gen ) const
{ gen.handleSymbol( *this ); }


