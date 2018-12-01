/***************************************************************************
 * File...... ArtDef.hpp
 * Author.... Mat
 * Date...... 10/5/95
 *
 * Artwork definitions.
 *
 * Copyright (C) 1997 MekTek
 ***************************************************************************/
#ifndef ARTDEF_HPP
#define ARTDEF_HPP

// Open Class
#include <igbitmap.hpp>
#include <istring.hpp>

// Generator
#include "MekTek.hpp"
class Filename;


/***************************************************************************
 * Class...... ArtDef
 * Author..... Mat
 *
 * Defines an art item by ID and bitmap.
 ***************************************************************************/
class _Export ArtDef
{
  public:
    // constructors
    ArtDef( const IString & id, const IGBitmap & bitmap );
    ArtDef( const ArtDef & copy );
    ~ArtDef();

    // assignment
    ArtDef & operator=( const ArtDef & copy );

    // query functions
    const IString &  id() const;       // return reference for key() function
    const IGBitmap & bitmap() const;

    // utility
    void              saveBitmap(IGBitmap::ImageFormat format = IGBitmap::Bitmap) const;
    void              saveBitmap(const IString & filename, Boolean forceOverwrite = false,
                                 IGBitmap::ImageFormat format = IGBitmap::Bitmap) const;
    static void       saveBitmap(const IGBitmap & bitmap, const Filename & filename, Boolean forceOverwrite = false,
                                 IGBitmap::ImageFormat format = IGBitmap::Bitmap);
    static IGBitmap * copyBitmap(const IGBitmap & bitmap);

  private:
    IString    _id;
    IGBitmap * _bitmap;
};


// global key access function
const IString & key( const ArtDef & art );


// Inline functions
#include "ArtDef.ipp"


#endif

