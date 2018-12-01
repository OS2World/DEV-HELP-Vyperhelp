/***************************************************************************
 * File...... WinHelpParser.cpp
 * Author.... Mat
 * Date...... 12/23/95
 *
 * Implementation for WinHelpParser which holds information for a Windows
 * Help Project.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/

// OpenClass
#include <iostream.h>
#include <iexcept.hpp>
#include <igbitmap.hpp>

// MekTek
#include "MekTek.hpp"

// Generator
#include "Generator.hpp"
#include "ParserException.hpp"
#include "ParserIndicator.hpp"

// WinHelpParser
#include "Identifier.hpp"
#include "HpjYacc.hpp"
#include "CntYacc.hpp"
#include "WinHelpParser.hpp"
#include "RtfYacc.hpp"
#include "RtfSection.hpp"


// local constants
static const char cntExtension[] = ".CNT";


/***************************************************************************
 * Procedure.. WinHelpParser::WinHelpParser
 * Author..... Mat
 * Date....... 12/26/95
 *
 * Constructor, sets initial sizes for containers.
 ***************************************************************************/
WinHelpParser::WinHelpParser( const IString & hpjFile ):
    _separator( ';' ),
    _numFontSizeMaps( 0 ),
    _bitmaps( 30 ),
    _rtfs( 20 ),
    _sections( new RtfSectionList( 100 ) ),
    _windows( new IdentifierSet( 10 ) ),
    _contexts( new IdentifierSet( 100 ) ),
    _hpjFile( hpjFile ),
    _cntFile( IString() ),
    _hlpFile( IString() ),
    _titleSent( false )
{}

WinHelpParser::~WinHelpParser()
{
  delete _sections;
  delete _windows;
  delete _contexts;
}



/***************************************************************************
 * Procedure.. WinHelpParser::fixPath
 * Author..... Mat
 * Date....... 1/21/99
 *
 * Fixes a path from the HPJ by (1) converting separators from comma to
 * semi-colon, and (2) resolving the paths relative to the HPJ directory.
 ***************************************************************************/
IString WinHelpParser::fixPath( const IString & path )
{
  Filename hpjDir = _hpjFile.dir();
  IString result;
  int i = 0;
  do
  {
    Filename dir( "" );
    int start = i + 1;
    i = path.indexOf( ',', start );
    if ( i )
      dir = path.subString( start, i - start ).strip();
    else
      dir = path.subString( start ).strip();
    dir.locateTo( hpjDir );

    // append this directory
    if ( start > 1)
      result += IString(';');
    result += dir;
  }
  while( i );

  return result;
}



//
// SETTERS
//

WinHelpParser & WinHelpParser::setRootPath( const IString & rootPath )
{
  _rootPath = fixPath( rootPath );
  return *this;
}

WinHelpParser & WinHelpParser::setBitmapPath( const IString & bitmapPath )
{
  _bitmapPath = fixPath( bitmapPath );
  return *this;
}

WinHelpParser & WinHelpParser::setCntFile( const IString & filename )
{
  _cntFile = filename;
  if (! _cntFile.locateOnPath( _hpjFile.dir() ) )
    _cntFile.locateTo( _hpjFile.dir() );
  return *this;
}

WinHelpParser & WinHelpParser::setHlpFile( const IString & filename )
{
  // HLP filename is stored in whatever form specified
  // (i.e., the path is not resolved)
  _hlpFile = filename;

  // use the HLP file for a default CNT, if none is already specified
  if ( ! _cntFile.length() )
  {
    if ( _hlpFile != _hlpFile.name() )
    {
      // use full path, if given
      _cntFile = _hlpFile.fullBase() + cntExtension;
    }
    else
    {
      // use same base name, look in HPJ directory
      _cntFile = _hlpFile.base() + cntExtension;
      if (! _cntFile.locateOnPath( _hpjFile.dir() ) )
        _cntFile.locateTo( _hpjFile.dir() );
    }
  }
  return *this;
}

WinHelpParser & WinHelpParser::addBitmapFile( const IString & bmpFile )
{
  _bitmaps.add( bmpFile );
  return *this;
}

WinHelpParser & WinHelpParser::addRtfFile( const IString & rtfFile )
{
  _rtfs.add( rtfFile );
  return *this;
}

WinHelpParser & WinHelpParser::addFontSizeMap( const FontSizeMap & fontSizeMap )
{
  IASSERTSTATE( _numFontSizeMaps < MAX_FONTSIZE_MAP );
  _map[ _numFontSizeMaps++ ] = fontSizeMap;
  return *this;
}

WinHelpParser & WinHelpParser::addAlias( const IString & contextName1, const IString & contextName2 )
{
  _contexts->associateIds( contextName1, contextName2 );
  return *this;
}

void WinHelpParser::setId( const IString & contextName, int id )
{
  _contexts->setId( contextName, id );
}

WinHelpParser & WinHelpParser::addWindow( const IString & windowName, int id )
{
  _windows->setId( windowName, id );
  return *this;
}


/***************************************************************************
 * Procedure.. WinHelpParser::setSeparators
 * Author..... Mat
 * Date....... 1/19/99
 *
 * Sets the separators for index entries.  This string comes from the
 * INDEX_SEPARATORS option in the HPJ file.  It should consist of two
 * characters.  The first separates sub-entries, which we don't care about.
 * The second (which we save) separates major entries.
 ***************************************************************************/
WinHelpParser & WinHelpParser::setSeparators( const IString & separators )
{
  if ( separators.length() != 2 )
  {
    warning( "Skipping INDEX_SEPARATORS, expected 2 characters" );
  }
  else
  {
    // take the second character (major separator)
    _separator = separators[2];
  }
  return *this;
}


//
// ACCESSORS
//

/***************************************************************************
 * Procedure.. WinHelpParser::getWindowId
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Given a window name, get the corresponding ID number.  Return 0 if none
 * was found.
 ***************************************************************************/
int             WinHelpParser::getWindowId( const IString & windowName ) const
{
  IdentifierSet::Cursor cursor( *_windows );
  if ( _windows->locateElementWithKey( windowName, cursor ) )
    return cursor.element()->value;
  return 0;
}


/***************************************************************************
 * Procedure.. WinHelpParser::getBitmapId
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Given a filename for a bitmap, find the full pathname of the file.
 * Searching is done on the following order:
 *   BMROOT path (from HPJ)
 *   ROOT path (from HPJ)
 *   BITMAPS list (from HPJ)
 * If the file is not found in any of these directories, the original
 * name is used and the file is assumed to be in the current directory.
 *
 * Next, add the file to the _bitmapsUsed or find the position if it is
 * already in there.  If it was added to the list, also define it for the
 * Generator.
 *
 * Return 0 if the bitmap was not found or not a supported format.
 ***************************************************************************/
Boolean WinHelpParser::getBitmapId( const IString & filename, IString & id )
{
  Boolean found = false;

  try
  {
    // find full path
    Filename fullname = filename;
    if ( ! fullname.locateOnPath( _bitmapPath, false ) )
      if ( ! fullname.locateOnPath( _rootPath, false ) )
        if (! _bitmaps.locateFullPath( fullname ) )
          fullname.locateTo( _hpjFile.dir() );

    // see if file is already in "used" list
    FilenameList::Cursor cursor( _bitmapsUsed );
    found = _bitmapsUsed.locate( fullname, cursor );

    // add to collection if necessary
    IGBitmap * bitmap = 0;
    if ( ! found )
    {
      // check input file date
      _gen->setInputFilename( fullname );

      // try to load the bitmap
      bitmap = new IGBitmap( fullname );

      // add to collection
      IBoolean ok = _bitmapsUsed.add( fullname, cursor );
      IASSERTSTATE( ok );
    }

    // use filename base as ID
    id = fullname.base();
    found = true;

    // add to generator if necessary
    if ( bitmap )
    {
      _gen->defineArt( ArtDef( id, *bitmap ) );
      delete bitmap;
    }
  }
  catch ( IException & except )
  {
    // unable to load bitmap
    warning( IString("Unable to find or convert ") + filename );
    return false;
  }

  return found;
}


/***************************************************************************
 * Procedure.. WinHelpParser::isUniqueContext
 * Author..... Mat
 * Date....... 12/28/95
 *
 * A context name is given.  If it is not in the current list, it is added
 * with id=0.  The use count of the context name is incremented, and if the
 * context name has previously been used (via this call), false is returned.
 * Otherwise true is returned and the id is set.
 ***************************************************************************/
Boolean WinHelpParser::isUniqueContext( const IString & contextName, int & id )
{
  IdentifierSet::Cursor cursor( *_contexts );

  // locate context name in the collection
  if ( ! _contexts->locateElementWithKey( contextName, cursor ) )
  {
    // add context name with id = 0
    setId( contextName, 0 );
    Boolean found = _contexts->locateElementWithKey( contextName, cursor );
    IASSERTSTATE( found );
  }

  // set current id
  Identifier * context = cursor.element();
  id = context->value;

  // increment and check usage count
  return ( context->count++ == 0 );
}


/***************************************************************************
 * Procedure.. WinHelpParser::numberOfAliases
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Returns the number of aliases for the given context name (including
 * the name itself).  0 is returned if none are found.
 ***************************************************************************/
int             WinHelpParser::numberOfAliases( const IString & contextName ) const
{
  IdentifierSet::Cursor cursor( *_contexts );
  if ( ! _contexts->locateElementWithKey( contextName, cursor ) )
    return 0;
  return _contexts->numberInGroup( cursor.element()->group );
}


/***************************************************************************
 * Procedure.. WinHelpParser::getAlias
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Gets the nth (index) alias of the specified contextName and returns it
 * in aliasName.  Returns true if the alias was found
 ***************************************************************************/
Boolean WinHelpParser::getAlias( const IString & contextName, int index, IString & aliasName ) const
{
  IdentifierSet::Cursor cursor( *_contexts );
  if ( ! _contexts->locateElementWithKey( contextName, cursor ) )
    return false;  // can't find this context
  if ( ! _contexts->locateGroupMember( cursor.element()->group, index, cursor ) )
    return false;  // can't locate that index
  aliasName = cursor.element()->name;
  return true;
}


/***************************************************************************
 * Procedure.. WinHelpParser::mapFontSize
 * Author..... Mat
 * Date....... 12/28/95
 *
 * Given a font size (in twips), check the font mapping table and return
 * a modified font size (in twips also).  If no mapping is found, then
 * the original is returned.
 ***************************************************************************/
int             WinHelpParser::mapFontSize( int sizeInTwips ) const
{
  static const int twipsPerPoint = 20;
  int sizeInPoints = sizeInTwips / twipsPerPoint;
  for (int i = _numFontSizeMaps; i; i--) {
    if ( (sizeInPoints >= _map[i].minPoints) &&
         (sizeInPoints <= _map[i].maxPoints) ) {
      return _map[i].targetPoints * twipsPerPoint;
    } /* endif */
  } /* endfor */
  return sizeInTwips;  // return original
}


/***************************************************************************
 * Procedure.. WinHelpParser::parseTopicId
 * Author..... Mat
 * Date....... 1/14/99
 *
 * Takes a topic ID, and strips it to the bare ID, returning the filename
 * and window in separate strings if provided.  Returns true if a non-empty
 * ID was found.  Topic IDs come from links in the RTF and from topic
 * specifications in the CNT file.
 ***************************************************************************/
Boolean WinHelpParser::parseTopicId( IString & topic, IString * filename, IString * window )
{
  // initialize output strings
  if ( filename )
    *filename = IString();
  if ( window )
    *window = IString();

  // strip blanks and optional prefixes
  topic.strip("*% ");

  // check for empty string
  if ( ! topic.length() )
    return false;

  // check for macro
  if ( topic[1] == '!' )
    return false;

  // check for a target filename or window
  int fileIndex = topic.lastIndexOf('@');
  int windowIndex = topic.indexOf('>');
  while ( fileIndex || windowIndex )
  {
    if ( fileIndex > windowIndex )
    {
      // extract filename from end of topic string
      if ( filename )
        *filename = topic.subString( fileIndex + 1 ).strip();
      topic.remove( fileIndex );
      fileIndex = 0;
    }
    else
    {
      // extract window name from end of topic string
      if ( window )
        *window = topic.subString( windowIndex + 1 ).strip();
      topic.remove( windowIndex );
      windowIndex = 0;
    }
  }

  // strip spaces from topic ID
  topic.strip();

  return true;
}



/***************************************************************************
 * Procedure.. WinHelpParser::addSection
 * Author..... Mat
 * Date....... 1/12/99
 *
 * Called from the RTF parser to add a complete section to the list.  These
 * sections are later streamed to the Generator, after the order of contents
 * is determined.  Returns true if the section was added.
 ***************************************************************************/
Boolean WinHelpParser::addSection( RtfSection * section )
{
  return _sections->add( RtfSectionPtr( section, IINIT ) );
}


/***************************************************************************
 * Procedure.. WinHelpParser::sendSection
 * Author..... Mat
 * Date....... 1/14/99
 *
 * Called from either the CNT parser or from sendContents.  Sends the
 * given section header info and then tries to locate the topic by ID.
 * If levels are skipped, they are filled in with dummy sections.  If no
 * topic or an invalid topic is given, then the section will have no content.
 *
 * If a matching topic is found, it is removed from the collection after being
 * sent.
 ***************************************************************************/
void WinHelpParser::sendSection( unsigned level, const IString & title, const IString & topic )
{
  // make sure the title has been sent before any sections
  if ( ! _titleSent )
  {
    _gen->setTitle( _title );
    _titleSent = true;
  }

  // insert dummy sections if levels were skipped
  MEKASSERT( level );
  while ( level > _level + 1 )
    *_gen << SectionGin( ++_level, true );
  _level = level;

  // send section info
  *_gen << SectionGin( level, true ).setTitle( title );

  IString id( topic );
  Filename filename( "" );
  if ( parseTopicId( id, &filename, 0 ) )
  {
    if ( filename.length() && ( _hlpFile != filename ) )
    {
      // for external topics, insert a dummy link
      *_gen << LinkGin( id ).setFile( filename );
      *_gen << TextGin( IString("[Link to ") + filename + IString("]") );
      *_gen << LinkGin();
    }
    else
    {
      // locate the corresponding RtfSection
      RtfSectionList::Cursor cursor( *_sections );

      id.upperCase();
      forCursor( cursor )
      {
        RtfSection * section = cursor.element();
        if ( section->label() && ( IString::upperCase( section->label()->name() ) == id ) )
        {
          // found a match

          // send section label and content
          *_gen << *section->label();
          section->generate( *_gen );

          // remove the section from the collection
          _sections->removeAt( cursor );
          return;
        }
      }
    }
  }
}


/***************************************************************************
 * Procedure.. WinHelpParser::sendContents
 * Author..... Mat
 * Date....... 1/12/99
 *
 * This is called when no CNT file is available, to generate a default
 * ordering for a table of contents, or after the CNT has been parsed, to
 * send the remaining sections as hidden sections (if fullContents is false).
 ***************************************************************************/
void WinHelpParser::sendContents( Boolean fullContents )
{
  // make sure the title has been sent
  if ( ! _titleSent )
  {
    _gen->setTitle( _title );
    _titleSent = true;
  }

  // if no sections left, get out
  if ( ! _sections->numberOfElements() )
    return;

  // put the "CONTENTS" section first
  if ( fullContents && _contentsContext.length() )
    sendSection( 1, "Contents", _contentsContext );

  // if no additional sections left, get out
  if ( ! _sections->numberOfElements() )
    return;

  // setup indicator
  if ( indicator() )
  {
    indicator()->setStatus( "Contents" );
    indicator()->setCurrent( 0 );
    indicator()->setTotal( _sections->numberOfElements() );
  }

  // send each remaining section
  unsigned n = 0;
  RtfSectionList::Cursor cursor( *_sections );
  forCursor( cursor )
  {
    const RtfSection * section = cursor.element();

    // stream section title and label
    *_gen << SectionGin( 1, fullContents ).setTitle( section->title() );
    if ( section->label() )
      *_gen << *section->label();

    // stream section content
    section->generate( *_gen );

    // update indicator
    if ( indicator() )
      indicator()->setCurrent( ++n );
  }

  // delete and remove all
  while ( cursor.setToFirst() )
    _sections->removeAt( cursor );
}


/***************************************************************************
 * Procedure.. WinHelpParser::warning
 * Author..... Mat
 *
 * Log a warning condition.  If line = 0, then line/col is unknown.
 * If line = SSUnsigned32Max, then error is at EOF.
 ***************************************************************************/
void WinHelpParser::warning( const IString & text, unsigned line, unsigned col )
{
  // make sure there is an indicator attached
  if ( ! indicator() )
    return;

  // construct output string
  IString warningText = text;
  if ( line )
  {
    warningText += IString(" [") + _activeFile + IString(':');
    if ( line == SSUnsigned32Max )
      warningText += IString("EOF");
    else
      warningText += IString(line) + IString('/') + IString(col);
    warningText += IString(']');
  }

  // send to user
  indicator()->warning( warningText );
}


/***************************************************************************
 * Procedure.. WinHelpParser::generate
 * Author..... Mat
 * Date....... 12/28/95
 *
 * The biggie!  This orchestrates parsing of the HPJ and RTF files and
 * routing of output to the specified Generator.
 ***************************************************************************/
void WinHelpParser::generate( Generator & gen )
{
  // save pointer to Generator
  _gen = &gen;

  // set default code page
  _gen->setCodePage( CodePage( CodePage::windows ) );

  try
  {
    // parse the HPJ file
    _activeFile = _hpjFile.name();
    HpjYacc hpj( _hpjFile );
    hpj.parseTo( gen, *this );

    // if no root path, use HPJ home directory
    if ( ! _rootPath.length() )
      _rootPath = _hpjFile.dir();

    // parse each RTF file in order
    // RtfParser saves header info from first file
    RtfYacc rtf;
    FilenameList::Cursor cursor( _rtfs );
    forCursor ( cursor )
    {
      Filename rtfFile( cursor.element() );
      if (! rtfFile.locateOnPath( _rootPath, false ))
        rtfFile.locateTo(_hpjFile.dir());
      _activeFile = rtfFile.name();
      _gen->setInputFilename( rtfFile );
      rtf.parseTo( rtfFile, gen, *this );
    }

    // if no CNT or HLP file specified, use same name as HPJ
    if ( ! _cntFile.length() )
      _cntFile = _hpjFile.fullBase() + cntExtension;
    if ( ! _hlpFile.length() )
      _hlpFile = _hpjFile.base() + ".HLP";

    // parse the CNT file if one exists
    Boolean foundCnt = false;
    _level = 1;
    if ( _cntFile.isAccessible( Filename::readAccess ) )
    {
      _activeFile = _cntFile.name();
      _gen->setInputFilename( _cntFile );
      CntYacc cnt( _cntFile );

      // update indicator
      if ( indicator() )
      {
        indicator()->setStatus( _activeFile );
        indicator()->setCurrent( 0 );
        indicator()->setTotal( cnt.size() );
      }

      // save section count before parsing CNT
      unsigned count = _sections->numberOfElements();

      // parse it!
      cnt.parseTo( gen, *this );

      // only set flag TRUE if at least one section was processed
      foundCnt = ( count > _sections->numberOfElements() );
    }

    // send remaining sections (use default ordering if no CNT)
    sendContents( ! foundCnt );
  }
  catch ( const SSException & ss )
  {
    gen.finish();
    ITHROW( ParserException( ss ) );
  }

  // finish generator output
  gen.finish();
}

