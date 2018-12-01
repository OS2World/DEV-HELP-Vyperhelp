/***************************************************************************
 * File...... HtmlCharStyle.cpp
 *
 * Contains all Gin stream operators which are related to character style.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

// Html Generator
#include "HtmlCommon.hpp"


void HtmlGenerator::handleFont( const FontGin         & font )
{
  // IE extensions support <font face> attribute, but it's not standard
  // so we ignore the facename for now

  // check for monospaced font
  _next->isMonospaced = (font.family() == FontInfo::mono);

  // save the actual input codepage for translation
  _next->inputCodepage = font.codepage();
}


void HtmlGenerator::handleSize( const SizeGin         & size )
{
  int twips = size.height().asTwips();
  if (twips > 0 && twips < 200)
  {
    // less then 10 pt is small
    _next->fontSize = fontSmall;
  }
  else if (twips > 240)
  {
    // greater than 12 pt is large
    _next->fontSize = fontLarge;
  }
  else
  {
    _next->fontSize = fontRegular;
  }
}


void HtmlGenerator::handleColor( const ColorGin        & color )
{
  _next->color = color.color();
}


void HtmlGenerator::handleBold( const BoldGin         & bold )
{
  _next->isBold = bold.isOn();
}


void HtmlGenerator::handleItalic( const ItalicGin       & italic )
{
  _next->isItalic = italic.isOn();
}


void HtmlGenerator::handleUnderline( const UnderlineGin    & uline )
{
  _next->isUnderline = uline.isOn();
}


void HtmlGenerator::handleComment( const CommentGin      & comment )
{
  _next->isComment = comment.isOn();
}


void HtmlGenerator::handleLink( const LinkGin         & link )
{
  if ( link.isOn() )
  {
    _next->linkTarget = link.target();
    if ( link.isExternal() )
    {
      _next->linkTargetFile = link.file();
    }
    else
    {
      // empty string flags "not external"
      _next->linkTargetFile = IString();
    }
  }
  else
  {
    // empty string flags "link off"
    _next->linkTarget = IString();
  }
}


