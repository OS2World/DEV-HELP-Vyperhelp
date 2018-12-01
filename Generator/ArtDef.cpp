/***************************************************************************
 * File...... ArtDef.cpp
 * Author.... Mat
 * Date...... 10/13/95
 *
 * Implementation for ArtDef, which defines a window size, position and
 * attributes.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// Generator
#include "Filename.hpp"
#include "ArtDef.hpp"


ArtDef::ArtDef( const IString & id, const IGBitmap & bitmap ):
  _id ( id ),
  _bitmap( 0 )
{
  _bitmap = copyBitmap( bitmap );
}


ArtDef::ArtDef( const ArtDef & copy ):
  _id ( copy.id() ),
  _bitmap( 0 )
{
  _bitmap = copyBitmap( copy.bitmap() );
}


ArtDef::~ArtDef()
{
  delete _bitmap;
}


ArtDef & ArtDef::operator=( const ArtDef & copy )
{
  _id = copy.id();
  delete _bitmap;
  _bitmap = copyBitmap( copy.bitmap() );
  return *this;
}


void ArtDef::saveBitmap(IGBitmap::ImageFormat format) const
{
  MEKASSERT( _bitmap && _id.length() );

  IString filename = _id;
  switch (format)
  {
    case IGBitmap::GIF:
      filename += ".gif";
      break;

    case IGBitmap::Bitmap:
      filename += ".bmp";
      break;

    default:
      // no auto-extension for other formats
      break;
  }
  saveBitmap(*_bitmap, filename, false, format);
}


void ArtDef::saveBitmap(const IString & filename, Boolean forceOverwrite, IGBitmap::ImageFormat format) const
{
  MEKASSERT( _bitmap );
  saveBitmap( *_bitmap, filename, forceOverwrite, format);
}


/***************************************************************************
 * Procedure.. ArtDef::saveBitmap
 * Author..... Mat
 * Date....... 3/25/99
 *
 * This utility function saves the given bitmap to the given bitmap file.
 * The IGBitmap::writeToFile is unfortunately a non-const function, so
 * we override that, since it should not actually affect the bitmap
 * appearance.
 ***************************************************************************/
void ArtDef::saveBitmap( const IGBitmap & bitmap, const Filename & filename, Boolean forceOverwrite,
    IGBitmap::ImageFormat format)
{
  // only save the file if it doesn't already exist!
  if ( forceOverwrite || ! filename.isAccessible( Filename::readAccess ) )
  {
    IGBitmap * nonConstBitmap = (IGBitmap *)&bitmap;
    nonConstBitmap->writeToFile(filename, format);
  }
}


/***************************************************************************
 * Procedure.. ArtDef::copyBitmap
 * Author..... Mat
 * Date....... 3/25/99
 *
 * This utility function allocates a new copy of the given bitmap.  If the
 * copy is made by using an IGBitmap constructor, the palette infomation
 * may be lost!  The only way to assure a correct copy is to use a temporary
 * file.
 ***************************************************************************/
IGBitmap * ArtDef::copyBitmap( const IGBitmap & bitmap )
{
  IGBitmap * result = 0;

  Filename filename( Filename::temporary );
  try
  {
    saveBitmap( bitmap, filename, true );
    result = new IGBitmap( filename, IGBitmap::Bitmap );
  }
  catch ( IException & x )
  {
    filename.remove();
    IRETHROW( x );
  }

  filename.remove();
  return result;
}

