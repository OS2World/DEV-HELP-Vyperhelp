/***************************************************************************
 * File...... RtfContext.cpp
 * Author.... Mat
 * Date...... 2/24/98
 *
 * Implementation of RtfContext.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/

// WinHelpParser
#include "RtfYacc.hpp"
#include "RtfContext.hpp"


#define DEFAULT_FONT_HALFPOINTS 24  // 12 pt


RtfContext::RtfContext( RtfYacc * yacc ):
  _yacc( yacc )
{}


void RtfContext::reset()
{
  // initialize to default destination and style
  _destination = RtfContext::unknown;
  resetParagraphStyles();
  resetCharacterStyles();
}


void RtfContext::resetCharacterStyles()
{
  setBold(false);
  setItalic(false);
  setFont( _yacc->defaultFontIndex() );
  setSize( DEFAULT_FONT_HALFPOINTS );
  setColor( NO_INDEX );
  setBackColor( NO_INDEX );
}


void RtfContext::resetParagraphStyles()
{
  setAlignment(AlignGin::left);
  setRightMargin( 0 );
  setLeftMargin( 0 );
  setSpacing( 0, 0 );
}


void RtfContext::setDestination( Destination destination )
{
  _destination = destination;
}


RtfContext::Destination RtfContext::destination() const
{
  return _destination;
}


void RtfContext::setRightMargin( int rightMarginTwips )
{
  if ( rightMarginTwips != _rightMarginTwips )
  {
    _rightMarginTwips = rightMarginTwips;
    sendRightMargin();
  }
}


void RtfContext::setLeftMargin( int leftMarginTwips )
{
  if ( leftMarginTwips != _leftMarginTwips )
  {
    _leftMarginTwips = leftMarginTwips;
    sendLeftMargin();
  }
}


void RtfContext::setAlignment( AlignGin::Alignment alignment )
{
  if ( alignment != _alignment )
  {
    _alignment = alignment;
    sendAlignment();
  }
}


void RtfContext::setSpacing( int beforeTwips, int afterTwips )
{
  if ( beforeTwips != _beforeTwips || afterTwips != _afterTwips )
  {
    _beforeTwips = beforeTwips;
    _afterTwips = afterTwips;
    sendSpacing();
  }
}


void RtfContext::setFont( int fontIndex )
{
  if ( fontIndex != _fontIndex )
  {
    _fontIndex = fontIndex;
    sendFont();
  }
}


void RtfContext::setSize( int fontHalfPoints )
{
  if ( fontHalfPoints != _fontHalfPoints )
  {
    _fontHalfPoints = fontHalfPoints;
    sendSize();
  }
}


void RtfContext::setColor( int colorKey )
{
  if ( colorKey != _colorKey )
  {
    _colorKey = colorKey;
    sendColor();
  }
}


void RtfContext::setBackColor( int backColorKey )
{
  if ( backColorKey != _backColorKey )
  {
    _backColorKey = backColorKey;
    sendBackColor();
  }
}


void RtfContext::setBold( Boolean isBold )
{
  if ( isBold != _isBold )
  {
    _isBold = isBold;
    sendBold();
  }
}


void RtfContext::setItalic( Boolean isItalic )
{
  if ( isItalic != _isItalic )
  {
    _isItalic = isItalic;
    sendItalic();
  }
}


void RtfContext::sendAll( const RtfContext * other ) const
{
  if ( ! other || _rightMarginTwips != other->_rightMarginTwips )
    sendRightMargin();
  if ( ! other || _leftMarginTwips != other->_leftMarginTwips )
    sendLeftMargin();
  if ( ! other || _alignment != other->_alignment )
    sendAlignment();
  if ( ! other || _beforeTwips != other->_beforeTwips || _afterTwips != other->_afterTwips )
    sendSpacing();
  if ( ! other || _fontIndex != other->_fontIndex )
    sendFont();
  if ( ! other || _fontHalfPoints != other->_fontHalfPoints )
    sendSize();
  if ( ! other || _colorKey != other->_colorKey )
    sendColor();
  if ( ! other || _backColorKey != other->_backColorKey )
    sendBackColor();
  if ( ! other || _isBold != other->_isBold )
    sendBold();
  if ( ! other || _isItalic != other->_isItalic )
    sendItalic();
}


void RtfContext::sendRightMargin() const
{
  _yacc->addGin( new RightMarginGin(false,Distance(_rightMarginTwips,Distance::twips)) );
}


void RtfContext::sendLeftMargin() const
{
  _yacc->addGin( new LeftMarginGin(false,Distance(_leftMarginTwips,Distance::twips)) );
}


void RtfContext::sendAlignment() const
{
  _yacc->addGin( new AlignGin(_alignment) );
}


void RtfContext::sendSpacing() const
{
  _yacc->addGin( new SpacingGin(Distance(_beforeTwips+_afterTwips,Distance::twips)) );
}


void RtfContext::sendFont() const
{
  if ( _fontIndex != NO_INDEX )
  {
    const RtfFont * ft = _yacc->locateFont( _fontIndex );
    if ( ft )
    {
      FontGin* fg = new FontGin( ft->mapFontfamily(), ft->getFacename() );
      fg->setCodepage( ft->getCodepage() );
      _yacc->addGin( fg );
    }
  }
}


void RtfContext::sendSize() const
{
  _yacc->addGin( new SizeGin(Distance(_yacc->mapFontSize(10 * _fontHalfPoints),Distance::twips)) );
}


void RtfContext::sendColor() const
{
  IColor defaultForeground( IColor::black );
  const IColor * color = 0;

  // try to lookup color
  if ( _colorKey != NO_INDEX )
  {
    const RtfColor * ct = _yacc->locateColor( _colorKey );
    if ( ct )
      color = ct->getColorentry();
  }

  // use default if none found
  if ( ! color )
    color = &defaultForeground;

  // send Gin
  _yacc->addGin( new ColorGin( *color ) );
}


void RtfContext::sendBackColor() const
{
  IColor defaultBackground( IColor::white );
  const IColor * color = 0;

  // try to lookup color
  if ( _backColorKey != NO_INDEX )
  {
    const RtfColor * ct = _yacc->locateColor( _backColorKey );
    if ( ct )
      color = ct->getColorentry();
  }

  // use default if none found
  if ( ! color )
    color = &defaultBackground;

  // send Gin
  _yacc->addGin( new BackColorGin( *color ) );
}


void RtfContext::sendBold() const
{
  _yacc->addGin( new BoldGin(_isBold) );
}


void RtfContext::sendItalic() const
{
  _yacc->addGin( new ItalicGin(_isItalic) );
}


