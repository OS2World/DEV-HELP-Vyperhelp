/***************************************************************************
 * File...... vykey.cpp
 * Author.... Mat
 * Date...... 4/29/99
 *
 * Makes user keys for registration, registers key in INI or shows registered key.
 *
 *     VYKEY "User Name" [year month day]       // to create key
 *     VYKEY "User Name" key                    // to register key
 *     VYKEY                                    // to show registered key
 *
 * "User Name" must be in quotes if it has more than one word.
 *
 * Copyright (C) 1999 MekTek
 ***************************************************************************/

#include <iostream.h>

#include "Generator.hpp"


// local functions
Boolean createKey( const IString & userName, const ExpirationDate & expiration );
Boolean registerKey( const IString & userName, const IString & keyName );
void showKey();


int main(int argc, char *argv[], char *envp[])
{
  Boolean ok = false;

  switch ( argc )
  {
    case 2:
      // create a key with no expiration date
      ok = createKey( argv[1], ExpirationDate() );
      break;

    case 5:
      {
      // create a key with given expiration date
      unsigned year = IString( argv[2] ).asUnsigned();
      unsigned month = IString( argv[3] ).asUnsigned();
      unsigned day = IString( argv[4] ).asUnsigned();
      ok = createKey( argv[1], ExpirationDate( ENCODE_DATE( year, month, day ) ) );
      break;
      }

    case 3:
      // register the given key
      ok = registerKey( argv[1], argv[2] );
      break;

    case 1:
      showKey();
      ok = true;
      // fall through to show usage info

    default:
      // invalid mode
      cout << "\nTo create key:   VYKEY \"User Name\" [year month day]"
              "\nTo register key: VYKEY \"User Name\" code"
              "\nTo show key:     VYKEY" << endl;
      break;
  }

  return ! ok;
}


// create a key from a user name and an expiration date
// return true if successful
Boolean createKey( const IString & userName, const ExpirationDate & expiration )
{
  Boolean ok = true;

  cout << "Name: " << userName << endl;
  cout << "Expiration: " << expiration.notice() << endl;

  // create key
  UserKey key( userName, expiration );

  // verify key
  UserKey verify( key.key() );
  if ( ! verify.isValid() )
  {
    cout << "Key is not valid" << endl;
    ok = false;
  }
  if ( ! verify.isValidFor( userName ) )
  {
    cout << "Key is not valid for " << userName << endl;
    ok = false;
  }
  if ( verify.expires() != expiration )
  {
    cout << "Key expiration is not correct" << endl;
    ok = false;
  }

  if ( ok )
    cout << "Key: " << key.key() << endl;

  return ok;
}



// register a key in the INI
// return true if successful
Boolean registerKey( const IString & userName, const IString & keyName )
{
  showKey();

  cout << "Registering Name: " << userName << endl;
  cout << "Registring Key: " << keyName << endl;
  Boolean ok = Generator::registration().setKey( userName, keyName );

  if ( ! ok )
    cout << "Invalid registration" << endl;
  else
    showKey();

  return ok;
}


// show current registration info
void showKey()
{
  Registration & reg = Generator::registration();

  cout << "Name: " << reg.name() << endl;
  cout << "Notice: " << reg.notice() << endl;
  cout << "Key: " << reg.key().key() << endl;
  cout << "Expiration: " << reg.key().expires().notice() << endl;
}

