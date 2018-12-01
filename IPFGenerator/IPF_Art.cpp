/***************************************************************************
 * File...... IPF_Art.cpp
 * Author.... Mat
 * Date...... 11/30/95
 *
 * IPF places artwork in one of these ways:
 *   size to fit the window: if x=100% and y=100%
 *   runin: if spot = top, middle or bottom
 *   on a separate line: if spot = left, right or center
 *   fit the current window
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "IPF_Common.hpp"


/***************************************************************************
 * Procedure.. IPFGenerator::operator<<( const ArtGin          & )
 * Author..... Mat
 * Date....... 12/1/95
 *
 * Saves a bitmap to a BMP file and places a reference in the IPF file.
 *
 * NOTE: special handling for artwork which is part of a link.  In those
 * cases, an :artlink group is needed.  Also, the existing :link tag must
 * be stopped and restarted to avoid IPFC errors.  (only "runin" artwork can
 * be part of a link).
 *
 * IPFC NOTE: IPFC seems to occasionally ignore artlinks if tags do not
 * begin at column 1.  Therefore, always breakBefore art-related tags.
 ***************************************************************************/
void IPFGenerator::handleArt( const ArtGin          & art )
{
  // don't allow artwork in a table
  if ( _next->mode == table )
    return;

  // start tag
  IString tag = "artwork";

  // specify position
  Boolean isRunin = false;
  switch ( art.spot() )
  {
  case ArtGin::left:
    tag += " align=left";
    break;
  case ArtGin::right:
    tag += " align=right";
    break;
  case ArtGin::center:
    tag += " align=center";
    break;
  case ArtGin::fit:
    tag += " fit";
    break;
  default:
    isRunin = true;
    tag += " runin";
    break;
  }

  // get bitmap filename
  IString bitmapFile = _workDir + art.id() + ".bmp";
  tag += " name='" + bitmapFile + "'";

  // save bitmap file
  if ( hasArtWithId( art.id() ) )
  {
    artWithId( art.id() ).saveBitmap( bitmapFile );
  }

  // first stop the exising (text) link if necessary
  IString link, linkFile;
  if ( isRunin && _next->linkTarget.length() )
  {
    // save the active or pending link for after the art
    link = _next->linkTarget;
    linkFile = _next->linkTargetFile;
    _next->linkTarget = _next->linkTargetFile = IString();
  }

  // always break after artwork, because CR may not be properly converted to space
  sendTag( tag, breakBefore | breakAfter );

  // send artlink group if this is part of a link
  if ( link.length() )
  {
    // send artlink block
    sendTag( "artlink", breakBefore | breakAfter );
    IString tag = "link reftype=hd refid='" + link + "'";
    if ( linkFile.length() )
      tag += " database='" + linkFile + "'";
    sendTag( tag, 0 );
    sendTag( "eartlink", breakBefore );

    // restore pending link
    _next->linkTarget = link;
    _next->linkTargetFile = linkFile;
  }

}

