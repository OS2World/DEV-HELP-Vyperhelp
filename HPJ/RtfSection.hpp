/***************************************************************************
 * File...... RtfSection.hpp
 * Author.... Mat
 * Date...... 1/12/99
 *
 * RtfSection stores a title, ID, and contents (Gins) for a section which
 * has been parsed from an RTF file.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef RTFSECTION_HPP
#define RTFSECTION_HPP


// OpenClass
#include <iseq.h>           // ISequence
#include <iptr.h>           // IAutoElemPointer

// Generator
#include "Filename.hpp"
#include "Gin.hpp"
class SaveGenerator;


class RtfSection
{
  public:
    // constructor
    RtfSection();
    virtual ~RtfSection();

    // setters
    void reset();
    void setTitle( const IString & title );
    void setLabel( LabelGin * label );
    void addGin( const Gin & gin );
    void finish();
      // finish adding Gins

    // getters
    const IString &  title() const;
    const LabelGin * label() const;

    // generator
    void generate( Generator & gen ) const;

private:
    // data
    IString         _title;
    LabelGin *      _label;
    Filename        _contents;
    SaveGenerator * _save;
};


// Smart pointer
typedef IAutoElemPointer< RtfSection > RtfSectionPtr;

// Sequence collection
class RtfSectionList: public ISequence< RtfSectionPtr >
{
  public:
    // constructor
    RtfSectionList( INumber numberOfElements = 100 );
};


// inline functions
#include "RtfSection.ipp"


#endif

