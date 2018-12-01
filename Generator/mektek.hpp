/***************************************************************************
 * File...... MekTek.hpp
 * Author.... Mat
 *
 * Common declarations for MekTek projects
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef MEKTEK_HPP
#define MEKTEK_HPP

// Standard C
#include <errno.h>

// OpenClass
#include <itrace.hpp>       // ITRACE macros
#include <iexcept.hpp>      // IException


#define BADSWITCH\
    {\
       IException::assertParameter("Bad switch", IEXCEPTION_LOCATION());\
    }

// throw a C error or OS/2 (system) error, whichever is appropriate
#define MEKTHROWCERROR()\
    if ( errno == EOS2ERR )\
      ISystemErrorInfo::throwSystemError(_doserrno, "", IEXCEPTION_LOCATION(),\
      IErrorInfo::invalidRequest, IException::recoverable);\
    else\
      ICLibErrorInfo::throwCLibError("", IEXCEPTION_LOCATION(),\
      IErrorInfo::invalidRequest, IException::recoverable)

// optional assert
#ifdef NDEBUG
  #define MEKASSERT(test)
#else
  #define MEKASSERT(test) IASSERTSTATE(test)
#endif


#endif
