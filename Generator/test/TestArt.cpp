/***************************************************************************
 * File...... TestArt.cpp
 * Author.... Mat
 * Date...... 12/1/95
 *
 * Test ArtGin output
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

#include "Generator.hpp"


// art IDs
static const char idMat[] = "Mat";
static const char idFolder[] = "Folder";
static const char sample[] = "Here is some sample text to show how the text wraps around the image.";


void DefineArt( Generator & gen )
{
  ISystemBitmapHandle hb( ISystemBitmapHandle::folder );
  IGBitmap folderBitmap( hb );
  IGBitmap matBitmap( "mat.bmp" );

  gen.defineArt( ArtDef( idMat, matBitmap ) )
     .defineArt( ArtDef( idFolder, folderBitmap ) );
}


void TestArt( Generator & gen )
{
  gen << SectionGin( 1, true ).setTitle( "Art" );
  gen << SpacingGin( Distance( 1, Distance::chars ) );

  gen << ParaGin()
      << "Here is some runin art.  First, top aligned. "
      << ArtGin( idFolder, ArtGin::top )
      << "Next, middle aligned. "
      << ArtGin( idFolder, ArtGin::middle )
      << "Finally, bottom aligned. "
      << ArtGin( idFolder, ArtGin::bottom );

  gen << ParaGin()
      << "Here is a left aligned signature:"
      << ArtGin( idMat, ArtGin::left )
      << sample
      << ParaGin()
      << "Here is a centered signature:"
      << ArtGin( idMat, ArtGin::center )
      << sample
      << ParaGin()
      << "Here is a right aligned signature:"
      << ArtGin( idMat, ArtGin::right )
      << sample
      << ParaGin();

  gen << SectionGin( 1, true ).setTitle( "Art to fit window" )
      << ArtGin( idMat, ArtGin::fit );
}

