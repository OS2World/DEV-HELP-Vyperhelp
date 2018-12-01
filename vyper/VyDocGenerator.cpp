/***************************************************************************
 * File...... VyDocGenerator.cpp
 * Author.... Mat
 * Date...... 2/5/98
 *
 * Implementation for VyDocGenerator
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// OpenClass
#include <iexcept.hpp>      // IASSERT macros

// WinHelpParser
#include "Filename.hpp"

// VFile
#include "SaveGenerator.hpp"

// Vyper
#include "VyDoc.hpp"
#include "VyDocGenerator.hpp"


VyDocGenerator::VyDocGenerator( VyDoc & doc ):
  _doc( doc ),
  _save( 0 )
{}


void VyDocGenerator::finish()
{
  closeSection();
}


void VyDocGenerator::registerSection( unsigned long offset, const SectionGin & gin )
{
  VySection * section = VySection::newSection();
  section->setFromGin( gin );

  section->setFilename( _doc.filename() );
  section->setOffset( offset );
  section->enableModified( false );

  _doc.addSection( section );
}



IString VyDocGenerator::translate( const IString & string )
{
  IString xlat( string );
  CodePage output;  // output uses active code page (default)
  output.translateString( xlat, _cp );
  return xlat;
}


Generator & VyDocGenerator::setCodePage( CodePage cp )
{
  _cp = cp;
  return *this;
}


Generator& VyDocGenerator::setTitle( const IString & title )
{
  _doc.setTitle( translate( title ) );
  return *this;
}


Generator& VyDocGenerator::setSubject( const IString & subject )
{
  _doc.setSubject( translate( subject ) );
  return *this;
}


Generator& VyDocGenerator::setVersion( const IString & version )
{
  _doc.setVersion( translate( version ) );
  return *this;
}


Generator& VyDocGenerator::setCopyright( const IString & copyright )
{
  _doc.setCopyright( translate( copyright ) );
  return *this;
}


Generator& VyDocGenerator::setAuthor( const IString & author )
{
  _doc.setAuthor( translate( author ) );
  return *this;
}


Generator & VyDocGenerator::defineArt( const ArtDef & artdef )
{
  VyArt * art = new VyArt;
  IASSERTSTATE( art );
  art->setName( artdef.id() );
  art->setBitmap( artdef.bitmap() );
  _doc.getArts()->add( art );
  return *this;
}


//
// The remaining functions are used only during Import, when a SaveGenerator
// is used to save each section to a temporary file.
//

void VyDocGenerator::closeSection()
{
  if ( _save )
  {
    _save->finish();
    delete _save;
    _save = 0;
  }
}


void VyDocGenerator::handleSection( const SectionGin & gin )
{
  // close the previous section, if one was open
  closeSection();

  // create a copy of the SectionGin with a translated title
  SectionGin gin2( gin );
  gin2.setTitle( translate( gin.title() ) );

  VySection * section = VySection::newSection();
  section->setFromGin( gin2 );
  section->create();
  section->enableModified( false );

  _save = new SaveGenerator( section->filename(), true );
  _save->handleSection( gin2 );

  _doc.addSection( section );
}


void VyDocGenerator::handleKeyword( const KeywordGin & gin )
{
  IASSERTSTATE( _save );

  // create copy of KeywordGin with translated text
  KeywordGin gin2( translate( gin.text() ), gin.isListed() );
  gin2.setExternal( gin.isExternal() );

  _save->handleKeyword( gin2 );
}


void VyDocGenerator::handleLabel( const LabelGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleLabel( gin );
}


void VyDocGenerator::handleRow( const RowGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleRow( gin );
}


void VyDocGenerator::handleColumn( const ColumnGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleColumn( gin );
}


void VyDocGenerator::handleItem( const ItemGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleItem( gin );
}


void VyDocGenerator::handlePara( const ParaGin & gin )
{
  IASSERTSTATE( _save );
  _save->handlePara( gin );
}


void VyDocGenerator::handleLine( const LineGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleLine( gin );
}


void VyDocGenerator::handlePush( const PushGin & gin )
{
  IASSERTSTATE( _save );
  _save->handlePush( gin );
}


void VyDocGenerator::handlePop( const PopGin & gin )
{
  IASSERTSTATE( _save );
  _save->handlePop( gin );
}


void VyDocGenerator::handleRightMargin( const RightMarginGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleRightMargin( gin );
}


void VyDocGenerator::handleLeftMargin( const LeftMarginGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleLeftMargin( gin );
}


void VyDocGenerator::handleBorder( const BorderGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleBorder( gin );
}


void VyDocGenerator::handleAlign( const AlignGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleAlign( gin );
}


void VyDocGenerator::handleSpacing( const SpacingGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleSpacing( gin );
}


void VyDocGenerator::handleRuler( const RulerGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleRuler( gin );
}


void VyDocGenerator::handleOutline( const OutlineGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleOutline( gin );
}


void VyDocGenerator::handleDivider( const DividerGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleDivider( gin );
}


void VyDocGenerator::handleFont( const FontGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleFont( gin );
}


void VyDocGenerator::handleSize( const SizeGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleSize( gin );
}


void VyDocGenerator::handleColor( const ColorGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleColor( gin );
}


void VyDocGenerator::handleBackColor( const BackColorGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleBackColor( gin );
}


void VyDocGenerator::handleBold( const BoldGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleBold( gin );
}


void VyDocGenerator::handleItalic( const ItalicGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleItalic( gin );
}


void VyDocGenerator::handleUnderline( const UnderlineGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleUnderline( gin );
}


void VyDocGenerator::handleComment( const CommentGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleComment( gin );
}


void VyDocGenerator::handleLink( const LinkGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleLink( gin );
}


void VyDocGenerator::handleHide( const HideGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleHide( gin );
}


void VyDocGenerator::handleArt( const ArtGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleArt( gin );
}


void VyDocGenerator::handleText( const TextGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleText( gin );
}


void VyDocGenerator::handleSymbol( const SymbolGin & gin )
{
  IASSERTSTATE( _save );
  _save->handleSymbol( gin );
}


