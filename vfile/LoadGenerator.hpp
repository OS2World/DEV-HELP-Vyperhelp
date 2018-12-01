/***************************************************************************
 * File...... LoadGenerator.hpp
 * Author.... Mat
 * Date...... 1/20/98
 *
 * LoadGenerator is an abstract Generator class which works closely with
 * a LoadParser to coordinate loading information from a VYX file on-demand.
 * The registerSection function along with LoadParser::load supports loading
 * of the table of contents without loading the actual text for each section.
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef LOADGENERATOR_HPP
#define LOADGENERATOR_HPP

#include "Generator.hpp"


class _Export LoadGenerator: public Generator
{
  public:
    // constructor
    LoadGenerator();

    virtual void registerSection( unsigned long offset, const SectionGin & section ) = 0;
};


#endif

