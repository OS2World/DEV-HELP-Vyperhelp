/***************************************************************************
 * File...... Registration.cpp
 * Author.... Mat
 * Date...... 4/30/99
 *
 * Implementation for Registration class.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

// Standard C
#include <string.h>

// OpenClass
#include <iprofile.hpp>

// Generator
#include "Registration.hpp"


// local constants
static const char profileName[]    = "vyper.ini";
static const char profileApp[]     = "VyperHelp";
static const char profileNameKey[] = "Name";
static const char profileKeyKey[]  = "Key";


/***************************************************************************
 * Procedure.. Registration::Registration
 * Author..... Mat
 * Date....... 4/30/99
 *
 * Construct registration information by reading from redundantly stored
 * information in INI (profile files).  Default key (from default constructor)
 * is set to expire after a demo period of 30 days.
 ***************************************************************************/
Registration::Registration()
{
  Boolean fail;
  Boolean needSave = false;
  Boolean needInit = true;

  // initialize empty user name
  _name[0] = 0;

  // attempt to load registration info from profiles
  IProfile profile( profileName );
  fail = loadFrom( profile, needInit );
  if ( fail )
    needSave = true;
  else
    needInit = false;

  profile = IProfile::userProfile();
  fail = loadFrom( profile, needInit );
  if ( fail )
    needSave = true;
  else
    needInit = false;

  profile = IProfile::systemProfile();
  fail = loadFrom( profile, needInit );
  if ( fail )
    needSave = true;
  else
    needInit = false;

  if ( needSave )
    save();
}


/***************************************************************************
 * Procedure.. Registration::setKey
 * Author..... Mat
 * Date....... 4/30/99
 *
 * Set the user name and key as specified.  If an invalid key is given, then
 * false is returned, and the key is not saved.
 ***************************************************************************/
Boolean Registration::setKey( const IString & userName, const IString & keyName )
{
  UserKey key( keyName );

  // check for valid name
  if ( userName.length() >= USER_NAME_LIMIT )
    return false;

  // check for valid key
  if ( ! key.isValidFor( userName ) )
    return false;

  // save new information
  strcpy( _name, userName );
  _key = key;
  save();

  return true;
}


/***************************************************************************
 * Procedure.. Registration::save()
 * Author..... Mat
 * Date....... 4/30/99
 *
 * Save the user name and key redundantly to three different profiles.
 ***************************************************************************/
void Registration::save()
{
  IProfile profile( profileName );
  saveTo( profile );
  profile = IProfile::userProfile();
  saveTo( profile );
  profile = IProfile::systemProfile();
  saveTo( profile );
}


/***************************************************************************
 * Procedure.. Registration::saveTo( IProfile & profile )
 * Author..... Mat
 * Date....... 4/30/99
 *
 * Save the registration name and key to the given profile.
 ***************************************************************************/
void Registration::saveTo( IProfile & profile )
{
  profile.setDefaultApplicationName( profileApp );
  profile.addOrReplaceElementWithKey( profileNameKey, IString( _name ) );
  profile.addOrReplaceElementWithKey( profileKeyKey, _key.key() );
}



/***************************************************************************
 * Procedure.. Registration::loadFrom
 * Author..... Mat
 * Date....... 4/30/99
 *
 * Load registration user name and key from the specified profile.  If the
 * info could not be loaded or if the info doesn't match the previously
 * loaded info, true is returned (indicating a "save" is neeed).
 *
 * If the user name and key are valid, and the expiration date encoded
 * is later then the currently stored key (or needInit is true), then the
 * current user name and key are replaced.
 ***************************************************************************/
Boolean Registration::loadFrom( IProfile & profile, Boolean needInit )
{
  profile.setDefaultApplicationName( profileApp );

  // get user name
  if ( ! profile.containsKeyName( profileNameKey ) )
    return true;
  IString userName = profile.elementWithKey( profileNameKey );
  if ( userName.length() >= USER_NAME_LIMIT )
    return true;

  // get key name
  if ( ! profile.containsKeyName( profileKeyKey ) )
    return true;
  IString keyName = profile.elementWithKey( profileKeyKey );

  // create a key
  UserKey key( keyName );
  if ( ! key.isValidFor( userName ) )
    return true;

  // check for required initialization
  if ( needInit )
  {
    strcpy( _name, userName );
    _key = key;
    return false;
  }

  // check if they are same as currently loaded
  if ( ( userName == IString( _name ) ) && ( key == _key ) )
    return false;

  // if current key doesn't expire, keep it
  if ( ! _key.expires() )
    return true;

  // if new key doesn't expire or is later then current, replace it
  if ( ! key.expires() || ( key.expires() > _key.expires() ) )
  {
    strcpy( _name, userName );
    _key = key;
  }

  return true;
}


/***************************************************************************
 * Procedure.. Registration::notice
 * Author..... Mat
 * Date....... 4/30/99
 *
 * Returns a registration notice, stating whether or not the software has
 * been registered and if so to whom.
 ***************************************************************************/
IString Registration::notice() const
{
  if ( *_name )
    return IString( "This software is registered to " ) + IString( _name ) + IString('.');
  else
    return IString( "This software is not registered." );
}


