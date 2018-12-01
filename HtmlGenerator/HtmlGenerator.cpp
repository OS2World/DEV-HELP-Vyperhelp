/***************************************************************************
 * File...... D:\proj\HtmlGenerator\HtmlGenerator.cpp
 * Author.... Mat
 *
 * HtmlGenerator implementation
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

// Generator
#include "HeaderGenerator.hpp"

// HtmlGenerator
#include "HtmlCommon.hpp"

// OpenClass
#include <idate.hpp>

// static data
const CodePage HtmlGenerator::outputCodepage(819);  // ISO-8859-1 "Latin 1" character set
const unsigned HtmlGenerator::_fileWidth = 75;

// local data
static const char untitled[] = "Untitled";
static const int  maxLevel = 6;


/***************************************************************************
 * HtmlGenerator::HtmlGenerator
 *
 * Constructor for HtmlGenerator.  os is the text stream for the Html output,
 * maxLineLength is the maximum number of characters per line, and
 * workDir is the directory where all files should be put (need trailing \)
 ***************************************************************************/
HtmlGenerator::HtmlGenerator( HtmlType type, const IString & filename, const IString & workDir ) :
    _type( type ),
    _filename( filename ),
    _fileCount(0),
    _tocLevel(1),
    _mode( header ),
    _workDir( workDir ),
    _hh( 0 ),
    _active( new HtmlContext ),
    _next( new HtmlContext ),
    _contextStack( new HtmlContextStack ),
    _tags( new HtmlTagStack ),
    _labels( new HtmlLabelSet ),
    _index( new HtmlIndexSet )
{
  IASSERTSTATE( _active );
  IASSERTSTATE( _next );
  IASSERTSTATE( _contextStack );
  IASSERTSTATE( _tags );
  IASSERTSTATE( _labels );
  IASSERTSTATE( _index );
}


HtmlGenerator::~HtmlGenerator()
{
  delete _active;
  delete _next;
  delete _contextStack;
  delete _tags;
  delete _labels;
  delete _index;
}


/***************************************************************************
 * HtmlGenerator::finish()
 *
 * Ends output to target.
 ***************************************************************************/
void HtmlGenerator::finish()
{
  // if we never got started, there's nothing to finish
  if (_mode == header)
  {
    return;
  }

  // finish HH output
  if ( _hh )
  {
    _hh->finish();
    delete _hh;
    _hh = 0;
  }

  // close content file
  wrapUpSection();
  sendEnd();
  closeOutput();
  _file.close();

  // close TOC
  if (_type != singleFile)
  {
    while (_tocLevel)
    {
      _tocFile << "</ul>";
      _tocLevel--;
    }
    _tocFile << endl;
    setOutput(_tocFile, _fileWidth, Generator::html);
    sendEnd();
    closeOutput();
    _tocFile.close();
  }

  // sort the index (case-insensitive)
  _index->sort( sort );

  switch (_type)
  {
  case multiFile:
    finishMultiFile();
    break;

  case htmlHelp:
    finishHtmlHelp();
    break;
  }

  if (_type != singleFile)
  {
    fixupLabels();
  }
}


/***************************************************************************
 * Procedure.. HtmlGenerator::finishMultiFile
 *
 * Finish output for multi-file.  This includes finishing and closing the
 * TOC file, creating an Index file and creating a frame set (HTML 4.0).
 ***************************************************************************/
void HtmlGenerator::finishMultiFile()
{
  // set flags for these files
  int flags = breakBefore | breakAfter | noModeCheck | isNotContent;

  // create index file
  _file.open(_indexFilename, ios::out);
  setOutput( _file, _fileWidth, Generator::html);
  sendHeader(IString("Index - ") +_title, true, html4);

  // panel heading
  sendPanelHeading(false);

  // list of index entries
  sendTag( "ul", flags );
///  HtmlIndexSet::Cursor cursor(*_index);
///  forCursor(cursor)
  INumber n = _index->numberOfElements();
  for (INumber i = 1; i <= n; i++) ///
  {

    // send an index entry
    int flags = noModeCheck | isNotContent;
///    const HtmlIndex & entry = cursor.element();
    const HtmlIndex & entry = _index->elementAtPosition(i);
    sendTag( "li", flags);
    IString tag = "a href=\"" + numberedFile(entry._fileNum);
    tag += "#index_" + IString(entry._labelNum) + "\" target=\"Body\"";
    sendTag( tag, flags );
    sendText( entry._phrase, flags | noSymbolCheck );
    sendTag( "/a", flags | breakAfter );
  }
  sendTag( "/ul", flags );
  sendEnd(false);
  closeOutput();
  _file.close();

  // create frameset
  _file.open(_filename, ios::out);
  setOutput(_file, _fileWidth, Generator::html);
  sendHeader(_title, true, html4frameset, false);
  sendTag( "frameset cols=\"30%,70%\"", flags );
  sendTag( IString("frame name=\"Contents\" title=\"Contents\" src=\"") + _tocFilename.name() + "\"", flags );
  // default to first section
  sendTag( IString("frame name=\"Body\" src=\"") + numberedFile(1) + "\"", flags );
  sendTag( "noframes", flags );
  sendTag( "body", flags );
  sendTag( "p", flags );
  sendTag( IString("a href=\"") + _tocFilename.name() + "\"", flags );
  sendText( "Table of Contents", flags );
  sendTag( "/a", flags );
  sendTag( "p", flags );
  sendTag( IString("a href=\"") + _indexFilename.name() + "\"", flags );
  sendText( "Index", flags );
  sendTag( "/a", flags );
  sendTag( "/body", flags );
  sendTag( "/noframes", flags );
  sendTag( "/frameset", flags );
  sendEnd(false);
  closeOutput();
  _file.close();
}


/***************************************************************************
 * Procedure.. HtmlGenerator::finishHtmlHelp
 *
 * Finish output for HTMLHelp.  This includes creating an Index file and
 * a project file.
 ***************************************************************************/
void HtmlGenerator::finishHtmlHelp()
{
  // set flags for these files
  int flags = breakBefore | breakAfter | noModeCheck | isNotContent;

  // create index file
  _file.open(_indexFilename, ios::out);
  setOutput( _file, _fileWidth, Generator::html);
  sendHeader("Index", false, html4);

  // send site properties (empty)
  sendTag( "object type=\"text/site properties\"", flags );
  sendTag( "/object", flags );

  // list of index entries
  sendTag( "ul", flags );
  HtmlIndexSet::Cursor cursor(*_index);
  forCursor(cursor)
  {
    // send an index entry
    int flags = noModeCheck | isNotContent;
    const HtmlIndex & entry = cursor.element();
    IString local = numberedFile(entry._fileNum) +"#index_" + IString(entry._labelNum);
    sendSitemap(_file, entry._phrase, local, entry._phrase);
  }
  sendTag( "/ul", flags );
  sendEnd(false);
  closeOutput();
  _file.close();

  // create project file
  _file.open(_filename, ios::out);

  // send main options
  _file << "[OPTIONS]\n";
  _file << "Title=" << _title << '\n';
  _file << "Contents file=" << _tocFilename.name() << '\n';
  _file << "Default topic=" << numberedFile(1) << '\n';
  _file << "Index file=" << _indexFilename.name() << '\n';
  _file << endl;
  // use "void *" operator to check error state
  if ( ! _file )
    MEKTHROWCERROR();

  // send file list
  _file << "[FILES]\n";
  for (int i = 1; i <= _fileCount; i++)
  {
    _file << numberedFile(i) << '\n';
  }
  _file << endl;
  // use "void *" operator to check error state
  if ( ! _file )
    MEKTHROWCERROR();

  // send HH help map
  _file << "[MAP]\n#include <" << _hhName.name() << ">\n";
  // use "void *" operator to check error state
  if ( ! _file )
    MEKTHROWCERROR();

  _file.close();
  // use "void *" operator to check error state
  if ( ! _file )
    MEKTHROWCERROR();
}


/***************************************************************************
 * Procedure.. HtmlGenerator::numberedFile
 *
 * Returns the filename (not full path) of a numbered multi-file.
 ***************************************************************************/
IString HtmlGenerator::numberedFile(int fileNum)
{
  IString number(fileNum);
  number.rightJustify(4, '0');
  return _filename.base() + IString('_') + number + ".html";
}


/***************************************************************************
 * Procedure.. HtmlGenerator::sendPanelHeading
 *
 * Send a heading for the index or TOC panel, which both link to each other.
 ***************************************************************************/
void HtmlGenerator::sendPanelHeading(Boolean isToc)
{
  int flags = breakBefore | breakAfter | noModeCheck | isNotContent;
  IString index("Index");
  IString toc("Contents");

  // setup links
  IString otherHeading;
  IString otherFilename;
  IString heading;
  if (isToc)
  {
    heading = toc;
    otherHeading = index;
    otherFilename = _indexFilename.name();
  }
  else
  {
    heading = index;
    otherHeading = toc;
    otherFilename = _tocFilename.name();
  }

  // link to the other
  sendTag( "p", flags );
  sendTag( "strong", flags );
  sendTag( IString("a href=\"") + otherFilename + "\" target=\"_self\"", flags );
  sendText( otherHeading, flags );
  sendTag( "/a", flags );
  sendTag( "/strong", flags );

  // header for index
  sendTag( "p", flags );
  sendTag( "strong", flags );
  sendText( heading, flags );
  sendTag( "/strong", flags );
}



// HEADER ELEMENTS


Generator & HtmlGenerator::setTitle( const IString & title )
{
  _title = translate(title, true);
  return *this;
}

Generator & HtmlGenerator::setSubject( const IString & subject )
{
  _subject = translate(subject, true);
  return *this;
}

Generator & HtmlGenerator::setVersion( const IString & version )
{
  _version = translate(version, true);
  return *this;
}

Generator & HtmlGenerator::setCopyright( const IString & copyright )
{
  _copyright = translate(copyright, true);
  return *this;
}

Generator & HtmlGenerator::setAuthor( const IString & author )
{
  _author = translate(author, true);
  return *this;
}

Generator & HtmlGenerator::addKeyword( const IString & keyword )
{
  if (_keywords.length())
  {
    _keywords += ',';
  }
  _keywords += translate(keyword, true);
  return *this;
}


// CODE PAGE SUPPORT

Generator & HtmlGenerator::setCodePage( CodePage cp )
{
  _docCodePage = cp;
  return *this;
}


/***************************************************************************
 * HtmlGenerator::translate
 *
 * Translates the given string for HTML output.  These strings are in the
 * codepage set by the setCodePage call.  HTML assumes all characters are in
 * the "Latin 1" (ISO-8859-1 or codepage 819)
 *
 * NOTE: be sure to use "noSymbolCheck" on any translated text which is sent.
 ***************************************************************************/
IString HtmlGenerator::translate( const IString & string, Boolean allowSymbols ) const
{
  IString xlat;
  IString symbol;
  const unsigned char * pch = string;

  while ( *pch )
  {
    if ((*pch >= 0x20) && (*pch <= 0x7f) && (*pch != '&') && (*pch != '<') && (*pch != '>'))
    {
      // assume characters in this range don't need translation
      xlat += IString( *pch );
    }
    else
    {
      // check for a matching HTML symbol first
      if ( allowSymbols && translateChar( *pch, symbol ) )
        xlat += symbol;
      else
      {
        // otherwise use code page translation
        unsigned char ch = outputCodepage.translate( *pch, _docCodePage );
        xlat += IString( ch? ch: CodePage::substitution );
      }
    }
    pch++;
  }

  return xlat;
}


// TEXT


/***************************************************************************
 * Procedure.. HtmlGenerator::handleText
 *
 * Handle plain text in either comment or normal mode
 ***************************************************************************/
void HtmlGenerator::handleText( const TextGin & text )
{
  if ( _next->isComment )
  {
    // send as a comment line
    // not concerned with wrapping or lines or combining IStrings
    sendComment( text.string(), isNotContent );
  }
  else
    sendText( text.string(), mapCodePage );
}



// SECTIONS


/***************************************************************************
 * Procedure.. HtmlGenerator::handleSection( const SectionGin & )
 *
 * Accepts a SectionGin, which sets the title, level and window info for
 * the current section.
 ***************************************************************************/
void HtmlGenerator::handleSection( const SectionGin & section )
{
  IASSERTSTATE( section.level() > 0 );

  // get section title
  IString title = translate(section.title(), true);
  if ( ! title.length() )
    title = untitled;

  // complete previous section or dump header
  if (_mode == header)
  {
    // get title for file
    IString fileTitle(title);

    // initialize all documents
    switch (_type)
    {
      case singleFile:
        fileTitle = _title;  // use document title for single file
        break;

      case multiFile:
        startMultiFile();
        break;

      case htmlHelp:
        startHtmlHelp();
        break;
    }

    // start the first/only output file
    startFile();
    sendHeader(fileTitle, false);
  }
  else
  {
    wrapUpSection();

    // handle multiple files
    if (_type != singleFile)
    {
      sendEnd();
      startFile();
      sendHeader(title, false);
    }
  }

  // send heading tag
  // HTML doesn't support section level > 6
  unsigned level = section.level() > maxLevel? maxLevel: section.level();
  IString tag = "h" + IString( level );
  _mode = updatingHeading;

  // send the tag
  sendTag( tag, breakBefore | isNotContent );

  // output title text
  sendText( title, isNotContent | noSymbolCheck );

  sendTag("/" + tag, breakAfter | isNotContent);

  _mode = ready;

  // make entry in table of contents
  if ((_type != singleFile) && section.isListed())
  {
    // go to correct indent level
    while (_tocLevel < level)
    {
      _tocFile << "<ul>";
      _tocLevel++;
    }
    while (_tocLevel > level)
    {
      _tocFile << "</ul>";
      _tocLevel--;
    }

    // add TOC entry
    if (_type == multiFile)
    {
      _tocFile << "<li><a href=\"" << _activeFilename.name() << "\" target=\"Body\">";
      _tocFile << title << "</a>\n";
    }
    else if (_type == htmlHelp)
    {
      sendSitemap(_tocFile, title, _activeFilename.name());
    }

    // use "void *" operator to check error state
    if ( ! _tocFile )
      MEKTHROWCERROR();
  }
}


/***************************************************************************
 * Procedure.. HtmlGenerator::startMultiFile
 *
 * Start output for multi-file.
 ***************************************************************************/
void HtmlGenerator::startMultiFile()
{
  // create index filename
  _indexFilename = _filename.fullBase() + "_index.html";

  // start the TOC and send header
  _tocFilename = _filename.fullBase() + "_toc.html";
  _tocFile.open(_tocFilename, ios::out);
  setOutput(_tocFile, _fileWidth, Generator::html);
  sendHeader(IString("Contents - ") + _title, true, html4);
  int flags = breakBefore | breakAfter | noModeCheck | isNotContent;

  // panel headings
  sendPanelHeading(true);

  // start list of entries
  sendTag( "ul", flags );
}


/***************************************************************************
 * Procedure.. HtmlGenerator::startHtmlHelp
 *
 * Start output for HTMLHelp.
 ***************************************************************************/
void HtmlGenerator::startHtmlHelp()
{
  // create HeaderGenerator
  _hhName = _filename.fullBase() + ".hh";
  _hh = new HeaderGenerator( _hhName );
  MEKASSERT( _hh );

  // create index filename
  _indexFilename = _filename.fullBase() + ".hhk";

  // start the TOC and send header
  _tocFilename = _filename.fullBase() + ".hhc";
  _tocFile.open(_tocFilename, ios::out);
  setOutput(_tocFile, _fileWidth, Generator::html);
  sendHeader("Contents", false, html4);
  int flags = breakBefore | breakAfter | noModeCheck | isNotContent;

  // send site properties
  sendTag( "object type=\"text/site properties\"", flags );
  sendTag( "param name=\"ImageType\" value=\"Folder\"", flags );
  sendTag( "/object", flags );

  // start list of entries
  sendTag( "ul", flags );
}


/***************************************************************************
 * Procedure.. HtmlGenerator::startFile
 *
 * Starts an HTML content file.  For multiple file output, the file count
 * is incremented and the count is appended to the base filename.
 ***************************************************************************/
void HtmlGenerator::startFile()
{
  if (_type == singleFile)
  {
    // single file name
    _activeFilename = _filename;
  }
  else
  {
    // add numeric identifier to base filename
    IString number(++_fileCount);
    number.rightJustify(4, '0');
    _activeFilename = _filename.fullBase() + "_" + number + ".html";
    MEKASSERT(_fileCount < 10000);
  }

  // close the file if currently open
  closeOutput();
  _file.close();

  // open the file stream
  _file.open(_activeFilename, ios::out);

  // initialize ASCII output from Generator
  setOutput(_file, _fileWidth, Generator::html);
}


/***************************************************************************
 * Procedure.. HtmlGenerator::wrapUpSection()
 *
 * This procedure will take care of unfinished output for the current
 * section.
 *
 * On exit, the current buffered _line length is always 0.
 ***************************************************************************/
HtmlGenerator & HtmlGenerator::wrapUpSection()
{
  IASSERTSTATE( _mode != updatingContext );

  // finish the section
  if ( _mode > start )
  {
    // return to default context
    switchContext( HtmlContext() );

    // make sure all tags are stopped
    prepareToSend();

    // flush the current line
    outputLine( false );

    // reset mode
    _mode = start;
  }

  return *this;
}


// LABELS


/***************************************************************************
 * Procedure.. HtmlGenerator::setAnchor
 *
 * Inserts an HTML anchor label as a link target.
 ***************************************************************************/
void HtmlGenerator::setAnchor(const IString & name)
{
  // send start tag with name
  IString tag = "a name=\"" + name + "\"";
  sendTag(tag);

  // send end tag
  sendTag("/a");
}


/***************************************************************************
 * Procedure.. HtmlGenerator::handleLabel( const LabelGin & )
 *
 * Accepts a LabelGin, which gives the ID information for the current
 * location.  Send an anchor tag to identify the location.
 ***************************************************************************/
void HtmlGenerator::handleLabel( const LabelGin & label)
{
  IASSERTSTATE( _mode > start );

  IString fixed = fixedLabel(label.name());

  // set anchor
  setAnchor(fixed);

  // add label to collection
  _labels->add(HtmlLabel(fixed, _fileCount));

  // pass to HeaderGenerator
  if (_hh)
  {
    LabelGin sanitized( fixed );
    sanitized.setId( label.id() );
    sanitized.setExternal( label.isExternal() );
    _hh->handleLabel( sanitized );
  }
}


/***************************************************************************
 * Procedure.. HtmlGenerator::fixedLabel
 *
 * Returns a "fixed" version of the given label.  The label must begin with
 * an alpha, and must only include vertain characters allowed by HTML.
 * Also, all labels are converted to lower case, because HTML labels are
 * case-sensitive.
 ***************************************************************************/
IString HtmlGenerator::fixedLabel(const IString & label)
{
  IString fixed;
  const unsigned char * pch = label;

  // check for empty string
  if (! *pch)
  {
    return fixed;
  }

  // check first character
  if (isalpha(*pch))
  {
    fixed += IString(*pch);
  }
  else
  {
    fixed += 'X';  // first character must be alpha
  }
  pch++;

  // check remainder
  while ( *pch )
  {
    if (isalnum(*pch) || (*pch == '_') || (*pch == '-') || (*pch == ':') || (*pch == '.'))
    {
      // these are valid characters for an anchor name
      fixed += IString(*pch);
    }
    else
    {
      fixed += '_';
    }
    pch++;
  }

  return fixed.lowerCase();
}


/***************************************************************************
 * Procedure.. HtmlGenerator::fixupLabels
 *
 * Fixup all label references in multi-file output.  Call fixupFile for each
 * generated file.
 ***************************************************************************/
void HtmlGenerator::fixupLabels()
{
  for (int i = 1; i <= _fileCount; i++)
  {
    IString number(i);
    number.rightJustify(4, '0');
    fixupFile(_filename.fullBase() + "_" + number + ".html");
  }
}


/***************************************************************************
 * Procedure.. HtmlGenerator::fixupFile
 *
 * This routine uses the saved set of labels to fixup all internal links in
 * the generated HTML files.  It the time the link code is written out, the
 * filename (number) may not be known.  Here we search for unfinished links
 * and fix them up.
 ***************************************************************************/
void HtmlGenerator::fixupFile(const IString & filename)
{
  fstream file(filename, ios::in | ios::out);
  // use "void *" operator to check error state
  if ( ! file )
    MEKTHROWCERROR();

  // construct search string
  IString search = IString("href=\"") + _filename.base() + "_0000.html#";

  while (! file.eof())
  {
    // save offset of line
    streampos pos = file.tellg();

    // get the next line
    IString line = IString::lineFrom(file);
    if (file.eof())
    {
      break;
    }

    // use "void *" operator to check error state
    if ( ! file )
      MEKTHROWCERROR();

    // look for matches to this pattern:
    //    href="xxxx_0000.html#label"
    // where xxxx is the base filename, and label is a valid HTML anchor/label
    // NOTE: assume this pattern will not be split across lines because there is no white space
    unsigned index = 1;
    while (index)
    {
      // look for a match of base portion: href="xxxx_0000.html#
      index = line.indexOf(search, index);
      if (! index)
      {
        // no more matches
        break;
      }

      // advance index to end
      index += search.length();

      // search for end of label string
      unsigned end = line.indexOf('\"', index);
      if (! end)
      {
        // not a valid pattern, keep searching
        ITRACE_DEVELOP(filename + IString(": missing \" at ") + IString(pos));
        continue;
      }

      // get the label
      IString label = line.subString(index, end - index);

      // make sure we have a valid label
      if (! label.length() || label != fixedLabel(label))
      {
        // not a valid label, keep searching
        ITRACE_DEVELOP(filename + IString(": bad label at ") + IString(pos));
        continue;
      }

      // lookup the label in the collection
      HtmlLabelSet::Cursor cursor(*_labels);
      if (! _labels->locateElementWithKey(label, cursor))
      {
        // not a valid label, keep searching
        ITRACE_DEVELOP(filename + IString(": label not found: ") + label);
        continue;
      }

      // create the new file number string
      IString number(cursor.element()._fileNum);
      number.rightJustify(4, '0');

      // set the write pointer (index is one-based index to start of label)
      file.seekp(pos + index - 11);

      // write the 4-digit file number
      file.write((const char *)number, 4);

      // use "void *" operator to check error state
      if ( ! file )
        MEKTHROWCERROR();
    }
  }

  // close the file
  file.close();

  // use "void *" operator to check error state
  if ( ! file )
    MEKTHROWCERROR();
}



// ART


/***************************************************************************
 * Procedure.. HtmlGenerator::handleArt( const ArtGin          & )
 *
 * Saves a bitmap to a GIF file and places a reference in the HTML file.
 ***************************************************************************/
void HtmlGenerator::handleArt( const ArtGin          & art )
{
  // start tag
  IString tag = "img align=";
  IString clearTag;

  // specify position
  Boolean isCentered = false;
  switch ( art.spot() )
  {
  case ArtGin::left:
    tag += "left";
    clearTag = "br clear=left";
    break;
  case ArtGin::right:
    tag += "right";
    clearTag = "br clear=right";
    break;
  case ArtGin::center:
  case ArtGin::fit:
    isCentered = true;
    clearTag = "br clear=all";
    // fall through to align middle
  case ArtGin::middle:
    tag += "middle";
    break;
  case ArtGin::top:
    // align art at top by specifying that adjacent text be aligned at bottom
    tag += "bottom";
    break;
  case ArtGin::bottom:
    // align art at bottom by specifying that adjacent text be aligned at top
    tag += "top";
    break;
  }

  // get bitmap filename
  IString bitmapFile = art.id() + ".gif";
  tag += " src=\"" + bitmapFile + "\"";

  // add alt name
  tag += " alt=\"" + art.id() + "\"";

  // save bitmap file
  if ( hasArtWithId( art.id() ) )
  {
    const ArtDef & artdef = artWithId(art.id());
    ISize size = artdef.bitmap().size();
    tag += " width=" + IString(size.width());
    tag += " height=" + IString(size.height());
    artdef.saveBitmap(_workDir + bitmapFile, false, IGBitmap::GIF);
  }

  // send tag(s)
  if (clearTag.length())
  {
    sendTag(clearTag, breakBefore);
  }
  if (isCentered)
  {
    undoTags();
    sendTag("center");
  }
  sendTag(tag, breakBefore);
  if (isCentered)
  {
    sendTag("/center");
    redoTags();
  }
}


// KEYWORDS

/***************************************************************************
 * Procedure.. HtmlGenerator::handleKeyword( const KeywordGin & )
 *
 * Handle an index entry.  Insert a label, and then create an entry
 ***************************************************************************/
void HtmlGenerator::handleKeyword( const KeywordGin      & keyword )
{
  // can't use unlisted keywords
  if ( ! keyword.isListed() )
    return;

  // index not supported for single file output
  if (_type == singleFile)
    return;

  // get translated keyword string
  IString text = translate( keyword.text(), true );

  // set an anchor
  IString label = IString("index_") + IString(++_indexCount);
  setAnchor(label);

  // add index to the collection
  HtmlIndex entry(text, _fileCount, _indexCount);
  IBoolean ok = _index->add(entry);
  MEKASSERT(ok);
}


/***************************************************************************
 * Procedure.. HtmlGenerator::sendSitemap
 *
 * Sends a "sitemap" object for an HTMLHelp index or TOC.  If the given file
 * is also being referenced by Generator, then you must be sure to force
 * a new line before calling this function.
 ***************************************************************************/
void HtmlGenerator::sendSitemap(
    ofstream & file,
    const IString & name,
    const IString & local,
    const IString & keyword)
{
  file << "<li><object type=\"text/sitemap\">\n";
  if (keyword.length())
  {
    file << "\t<param name=\"Keyword\" value=\"" << keyword << "\">\n";
  }
  IASSERTSTATE(name.length());
  file << "\t<param name=\"Name\" value=\"" << name << "\">\n";
  IASSERTSTATE(local.length());
  file << "\t<param name=\"Local\" value=\"" << local << "\">\n";
  file << "\t</object>\n";
}

