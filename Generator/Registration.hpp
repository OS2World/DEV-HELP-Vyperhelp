/***************************************************************************
 * File...... Registration.hpp
 * Author.... Mat
 * Date...... 4/30/99
 *
 * Holds registration info (user name and registration key) from the
 * INI profiles.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/
#ifndef REGISTRATION_HPP
#define REGISTRATION_HPP


// OpenClass
class IProfile;

// Generator
#include "UserKey.hpp"


// size of user name buffer
#define USER_NAME_LIMIT 50


class _Export Registration
{
  public:
    Registration();

    // getters
    IString         name() const;
    IString         notice() const;
    const UserKey & key() const;

    // setter
    Boolean setKey( const IString & userName, const IString & key );

  private:
    Boolean loadFrom( IProfile & profile, Boolean needInit );
    void    save();
    void    saveTo( IProfile & profile );

    UserKey _key;

    // a char buffer is used instead of IString here because of problems accessing
    //   a static IString shared across multiple processes.
    char _name[USER_NAME_LIMIT];
};


// inline functions
#include "Registration.ipp"


#endif

