/***************************************************************************
 * File...... RtfYacc.hpp
 * Author.... Marco
 * Date...... 12/31/95
 *
 * Defines the object which holds the global font table, color table, and
 * rtf control word map as well as support functions for the hpj build
 * process.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef RTFYACC_HPP
#define RTFYACC_HPP

#include <fstream.h>
#include <istring.hpp>
#include <iksset.h>
#include <istack.h>
#include <iqueue.h>

// Generator
#include "Generator.hpp"

// WinHelpParser
class WinHelpParser;
#include "RtfContext.hpp"
#include "RtfSection.hpp"
#include "RtfFont.hpp"
#include "RtfColor.hpp"
#include "EnhancedYacc.hpp"

// #define TABLE_SUPPORT
// (table support currently needs a lot of work!)


class RtfYacc : public EnhancedYacc
{
public:
   // constructor
   RtfYacc();

   // parser function
   void parseTo(const IString & filename, Generator& gen, WinHelpParser& whp);

   // warnings
   void warning( const IString & text, const SSLexLexeme * lexeme = 0 );

   // for collaboration with RtfContext
   void             addGin( Gin * );
   const RtfFont *  locateFont( int index );
   const RtfColor * locateColor( int key );
   int              mapFontSize( int sizeInTwips ) const;
   int              defaultFontIndex();

   // from SSYacc
   SSYaccStackElement* reduce( SSUnsigned32 production , SSUnsigned32 size );
   SSBooleanValue      error( SSUnsigned32 qulState, SSLexLexeme & qLexeme );

private:

   // collaborators
   WinHelpParser *  _whp;
   Generator      * _gen;

   // parsing helper functions
   int getParameter();
   int getControlword(IString& controlword, int& param);
   void setHoldText( const IString & text );
   void processLine();
   void processPara();
   void processChar( char ch );
   void processSymbol( const Symbol & sym );
   void processText( const IString & text );
   void addBitmap( ArtGin::ArtSpot spot );
   void addLabel( const IString & contextId, Boolean getAlias );
   void setParagraphDefaults();
   void setCharacterDefaults();
   void rebuildContextStack();
   void sectionDone( Boolean isDocumentDone );
   void sendKeywords( const IString & keywords, Boolean isListed );

   // PARSING DATA FOLLOWS...

   // context stack
   typedef IStack <RtfContext> StackType;
   StackType  _stack;
   RtfContext _context;
   int        beforepara, afterpara;
   int        _leftMargin, _firstMargin;
   void closeDestination( RtfContext::Destination d );

   // hyperlinks
   void         flushLink( LinkGin * link = NULL );
   void         endLinkText();
   enum LinkState {
     off,
     start,
     ready
   }            _linkState;
   SSUnsigned32 _linkType;
   int          _linkLevel;
   GinList    * _linkContent;
   IString      targettext;

   // footnote processing
   void         releaseText( Boolean okToFlushLink = true );
   IString      _holdText;  // help for footnote identification
   GinList    * _holdList;
   char         footchar;
   IString      foottext;

   // embedded bitmaps
   SSUnsigned32 _bitmapType;
   IString      _bitmapName;

   // font
   Boolean fonttabledefined;
   Boolean _fontDefaultsSet;
   RtfFontTable fonttbl;
   int deff;
   int fnum;
   int deffcodepage;
   int fcodepage;
   RtfFont::Familyname ffamily;
   RtfFont::Fprqtype   ffprq;
   IString  fdata;                     // stores font names as they read

   // color
   RtfColorTable colortbl;
   int cred;
   int cblue;
   int cgreen;
   int cnum;

#ifdef TABLE_SUPPORT
   // tables
   typedef IQueue <Distance> QueueType;
   QueueType columnwidths;
   int colcount;
   int numofcolumns;
   int cellwidth;
#endif

   // fields
   Boolean _isPrivateField;

   // general control
   RtfSection * _section;
   Boolean      _validSection;
   Boolean      _eatNextSpace;  // eat a space immediately after a footnote command
};


#endif

