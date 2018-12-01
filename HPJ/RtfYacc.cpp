/***************************************************************************
 * File...... RtfYacc.cpp
 * Author.... Mat
 * Date...... 3/28/96
 *
 * RTF Grammer
 *
 * Copyright (C) 1996 MekTek
 ***************************************************************************/

// Standard C
#include <limits.h>
#include <ctype.h>

// OpenClass
#include <iexcept.hpp>
#include <itrace.hpp>

// MekTek
#include <mektek.hpp>

// WinHelpParser
#include "WinHelpParser.hpp"
#include "RtfYacc.hpp"

#define ALexClass  RtfLexUnused   // change default names, avoid multiple definitions
#define AYaccClass RtfYaccUnused  // these are unused anyways
#include "rtf.yhh"       // generated from rule file



#define   NOPARAM (-INT_MAX)


//
// TEMPLATE DEFINITIONS
//
#include <iqueseq.c>
#include <istkseq.c>
#include <ilnseq.c>
#include <iavlkss.c>

// typedef IStack <RtfContext> StackType;
#pragma define(IWStackOnSeqCursor<RtfContext,IStdOps<RtfContext>,IGLnSq<RtfContext,IStdOps<RtfContext> > >)
#pragma define(IWStOnSq<RtfContext,IStdOps<RtfContext>,IGLnSq<RtfContext,IStdOps<RtfContext> > >)
#pragma define(IGLinkedSequenceNode<RtfContext,IStdOps<RtfContext> >)
#pragma define(IGLnSqCrs<RtfContext,IStdOps<RtfContext> >)
#pragma define(IGLnSq<RtfContext,IStdOps<RtfContext> >)

// typedef IQueue <Distance> QueueType;
#pragma define(IWQueueOnSeqCursor<Distance,IStdOps<Distance>,IGLnSq<Distance,IStdOps<Distance> > >)
#pragma define(IWQuOnSq<Distance,IStdOps<Distance>,IGLnSq<Distance,IStdOps<Distance> > >)
#pragma define(IGLnSqCrs<Distance,IStdOps<Distance> >)
#pragma define(IGLinkedSequenceNode<Distance,IStdOps<Distance> >)
#pragma define(IGLnSq<Distance,IStdOps<Distance> >)


/***************************************************************************
 * Procedure.. RtfYacc ctor/dtor
 * Author..... Mat
 * Date....... 3/7/96
 *
 * Constructor and destructor for RtfYacc
 ***************************************************************************/
RtfYacc::RtfYacc() :
    EnhancedYacc( "rtf", "" ),
    _context( this ),
    _section( 0 ),
    _holdList( 0 )
{}


/***************************************************************************
 * Procedure...... RtfYacc::parseTo
 * Author.... Marco
 * Date...... 12/31/1995
 *
 * Parses an RTF file, this may be called multiple times for the same
 * RtfYacc instance and different RTF files.
 ***************************************************************************/
void RtfYacc::parseTo(const IString & filename, Generator& gen, WinHelpParser& whp)
{
  // set file to consume
  lex()->reset();
  setConsumerFile( filename );

  // save pointers to collaborators
  _whp = &whp;
  _gen = &gen;

  // initialize parsing data
  fonttabledefined = false;
  _fontDefaultsSet = false;
  deff = 0;  // default font index
  deffcodepage = CodePage::windows;
  fcodepage = deffcodepage;
  colortbl.removeAll();

#ifdef TABLE_SUPPORT
  colcount = 0;
  numofcolumns = 0;
  cellwidth = 0;
#endif

  _context.reset();
  _section = new RtfSection;
  _validSection = false;
  _eatNextSpace = false;
  _linkContent = NULL;
  _linkLevel = 0;
  _linkState = off;
  _holdList = new GinList;
  _isPrivateField = false;

  // setup indicator
  ParserIndicator * indicator = whp.indicator();
  if ( indicator )
  {
    indicator->setStatus( Filename(filename).name() );
    indicator->setCurrent( 0 );
    indicator->setTotal( size() );
  }

  // start parsing
  reset();
  parse();

  // cleanup
  delete _holdList;
  _holdList = 0;
  delete _section;
  _section = 0;
  fonttbl.removeAll();
  colortbl.removeAll();
}


/***************************************************************************
 * Procedure.. RtfYacc::error
 * Author..... Mat
 *
 * Prints an error if a message occurs
 ***************************************************************************/
SSBooleanValue RtfYacc::error( SSUnsigned32 qulState, SSLexLexeme & qLexeme)
{
  // print warning
  warning( "Parsing error", &qLexeme );

  // standard error recovery
  return errorToken();
}



/***************************************************************************
 * Procedure.. RtfYacc::reduce
 * Author..... Mat
 * Date....... 3/26/96
 *
 * Called when a grammer production is recognized.  For the RTF grammar,
 * this means that a new token was found.
 ***************************************************************************/
SSYaccStackElement* RtfYacc::reduce( SSUnsigned32 production, SSUnsigned32 size )
{
  int parameter;

  switch ( production ) {


    // BASIC TEXT --------------------------------------------------------

    case AYaccText:
    // element -> Text
      processText( lexemeAsString(0) );
      break;


    // NESTING -----------------------------------------------------------

    case AYaccOpen:
    // element -> Open
      {
         _stack.push( _context );
         if (_context.destination() == RtfContext::document)
         {
           addGin( new PushGin() );
           if ( _linkState > off && _linkState < ready )
             _linkLevel++;
         }
      }
      break;

    case AYaccClose:
    // element -> Close
      {
        // save destination before close
        RtfContext::Destination prevdestination = _context.destination();

        // pop the stack
        _stack.pop( _context );

        // close the old destination
        if ( prevdestination != _context.destination() )
          closeDestination( prevdestination );

        // special handling when returning to document destination
        if ( _context.destination() == RtfContext::document )
        {
          // add a PopGin
          addGin( new PopGin() );

          // set font defaults
          if ( fonttabledefined && ! _fontDefaultsSet )
          {
            // set default font
            _context.setFont( deff );
            _fontDefaultsSet = true;
          }

          // check link state
          if ( _linkState == start )
          {
            // check for end of link attribute
            if ( _linkLevel )
            {
              // end of nested group
              _linkLevel--;
            }
            else
            {
              // end of link attribute
              endLinkText();
            }
          }
        }
      }
      break;


    // SYMBOLS -----------------------------------------------------------

    case AYaccSymbol:
    // element -> Symbol
       // the second character
      processChar( lexemeAsString(0)[2] );
      break;

    case AYaccHex:
    // element -> Hex
      processChar( lexemeAsString(0).subString(3,2).x2d().asInt() );
      break;

    case AYaccOptionalHyphen:
    // element -> OptionalHyphen
      processSymbol( Symbol( Symbol::shy ) );  // soft hyphen
      break;

    case AYaccNoBreakHyphen:
    // element -> NoBreakHyphen
      processSymbol( Symbol( Symbol::rhy ) );  // required hyphen
      break;

    case AYaccNoBreakSpace:
    // element -> NoBreakSpace
      processSymbol( Symbol( Symbol::rbl ) );  // required blank
      break;

    case AYaccEmdash:
    // element -> Emdash
      processSymbol( Symbol( Symbol::emdash ) );
      break;

    case AYaccEmspace:
    // element -> Emspace
      processSymbol( Symbol( Symbol::mspace ) );
      break;

    case AYaccEndash:
    // element -> Endash
      processSymbol( Symbol( Symbol::endash ) );
      break;

    case AYaccEnspace:
    // element -> Enspace
      processSymbol( Symbol( Symbol::nspace ) );
      break;

    case AYaccBullet:
    // element -> Bullet
      processSymbol( Symbol( Symbol::bullet ) );
      break;

    case AYaccLquote:
    // element -> Lquote
      processSymbol( Symbol( Symbol::osq ) );  // open single quote
      break;

    case AYaccRquote:
    // element -> Rquote
      processSymbol( Symbol( Symbol::csq ) );  // close single quote
      break;

    case AYaccLdblquote:
    // element -> Ldblquote
      processSymbol( Symbol( Symbol::odq ) );  // open double quote
      break;

    case AYaccRdblquote:
    // element -> Rdblquote
      processSymbol( Symbol( Symbol::cdq ) );  // close double quote
      break;


    // UNRECOGNIZED CONTROL WORDS ----------------------------------------

    case AYaccCtrl:
    // element -> Ctrl
      break;


    // BITMAP REFERENCES -------------------------------------------------

    case AYaccBmc:
    // element -> Bmc
    case AYaccBmcwd:
    // element -> Bmcwd
    case AYaccBmct:
    // element -> Bmct
    case AYaccBml:
    // element -> Bml
    case AYaccBmlwd:
    // element -> Bmlwd
    case AYaccBmlt:
    // element -> Bmlt
    case AYaccBmr:
    // element -> Bmr
    case AYaccBmrwd:
    // element -> Bmrwd
    case AYaccBmrt:
    // element -> Bmrt
      _context.setDestination( RtfContext::bitmap );
      _bitmapType = production;  // save control word ID
      _bitmapName = "";
      break;


    // MULTIMEDIA ELEMENTS (UNSUPPORTED) ---------------------------------

    case AYaccEwc:
    // element -> Ewc
    case AYaccEwl:
    // element -> Ewl
    case AYaccEwr:
    // element -> Ewr
    case AYaccButton:
    // element -> Button
    case AYaccMci:
    // element -> Mci
    case AYaccMciLeft:
    // element -> MciLeft
    case AYaccMciRight:
    // element -> MciRight
      warning( IString("Skipping multimedia (") + lexemeAsString( 0 ) + IString(")") );
      _context.setDestination( RtfContext::multimedia );
      break;

    case AYaccMetaClose:
    // element -> MetaClose
      if ( _context.destination() == RtfContext::bitmap || _context.destination() == RtfContext::multimedia )
      {
        // close a meta-destination
        closeDestination( _context.destination() );
        _context.setDestination( RtfContext::document );
      }
      else
      {
        // just a } symbol
        processChar('}');
      }
      break;


    // COLOR TABLE -------------------------------------------------------

    case AYaccBlue:
    // element -> Blue
      cblue = getParameter();
      break;

    case AYaccColortbl:
    // element -> Colortbl
      {
        cnum = 0;
        cred = cblue = cgreen = -1;
        _context.setDestination( RtfContext::colortable );
      }
      break;

    case AYaccGreen:
    // element -> Green
      cgreen = getParameter();
      break;

    case AYaccRed:
    // element -> Red
      cred = getParameter();
      break;


    // FONT TABLE --------------------------------------------------------

    case AYaccCpg:
    // element -> Cpg
      fcodepage = getParameter();
      break;

    case AYaccFbidi:
    // element -> Fbidi
      ffamily = RtfFont::FBIDI;
      break;

    case AYaccFdecor:
    // element -> Fdecor
      ffamily = RtfFont::FDECOR;
      break;

    case AYaccFmodern:
    // element -> Fmodern
      ffamily = RtfFont::FMODERN;
      break;

    case AYaccFnil:
    // element -> Fnil
      ffamily = RtfFont::FNIL;
      break;

    case AYaccFonttbl:
    // element -> Fonttbl
      {
         fonttbl.removeAll();
         _context.setDestination( RtfContext::fonttable );
         fdata = "";
      }
      break;

    case AYaccFprq:
    // element -> Fprq
      ffprq = (RtfFont::Fprqtype) getParameter();
      break;

    case AYaccFroman:
    // element -> Froman
      ffamily = RtfFont::FROMAN;
      break;

    case AYaccFscript:
    // element -> Fscript
      ffamily = RtfFont::FSCRIPT;
      break;

    case AYaccFswiss:
    // element -> Fswiss
      ffamily = RtfFont::FSWISS;
      break;

    case AYaccFtech:
    // element -> Ftech
      ffamily = RtfFont::FTECH;
      break;

    case AYaccF:
    // element -> F
      fnum = getParameter();
      if (_context.destination() == RtfContext::document)
        _context.setFont( fnum );
      break;


    // DOCUMENT SETTINGS -------------------------------------------------

    case AYaccRtf:
    // element -> Rtf
      _context.setDestination( RtfContext::document );
      setParagraphDefaults();
      setCharacterDefaults();
      _context.sendAll();
      break;

    case AYaccPca:
    // element -> Pca
      deffcodepage = 850;
      _gen->setCodePage( deffcodepage );
      break;

    case AYaccPc:
    // element -> Pc
      deffcodepage = 437;
      _gen->setCodePage( deffcodepage );
      break;

    case AYaccAnsi:
    // element -> Ansi
      deffcodepage = CodePage::ansi;
      _gen->setCodePage( deffcodepage );
      break;

    case AYaccMac:
    // element -> Mac
      deffcodepage = CodePage::mac;
      _gen->setCodePage( deffcodepage );
      break;

    case AYaccWindows:
    // element -> Windows
      deffcodepage = CodePage::windows;
      _gen->setCodePage( deffcodepage );
      break;

    case AYaccDeff:
    // element -> Deff
      deff = getParameter();
      break;


    // DOCUMENT CONTROL --------------------------------------------------

    case AYaccLine:
    // element -> Line
      if (_context.destination() == RtfContext::document)
        processLine();
      break;

    case AYaccPage:
    // element -> Page
      if ( _context.destination() == RtfContext::document )
        sectionDone( false );
      break;

    case AYaccSect:
    // element -> Sect
      // insert two newlines to mark end of RTF "section"
      if (_context.destination() == RtfContext::document)
      {
        processPara();
        processPara();
      }
      break;

    case AYaccLineFeed:
    // element -> LineFeed
    case AYaccCarriageReturn:
    // element -> CarriageReturn
    case AYaccPar:
    // element -> Par
      if (_context.destination() == RtfContext::document)
        processPara();
      break;

    case AYaccAsciiTab:
    // element -> AsciiTab
    case AYaccHexTab:
    // element -> HexTab
    case AYaccTab:
    // element -> Tab
      processChar( ' ' );  // for now, just use a space
      break;


    // CHARACTER STYLES --------------------------------------------------

    case AYaccB:
    // element -> B
      {
         if (_context.destination() == RtfContext::document)
            _context.setBold( getParameter() != 0 );
      }
      break;

    case AYaccCb:
    // element -> Cb
      {
         if (_context.destination() == RtfContext::document)
            _context.setBackColor( getParameter() );
      }
      break;

    case AYaccCf:
    // element -> Cf
      {
         if (_context.destination() == RtfContext::document)
            _context.setColor( getParameter() );
      }
      break;

    case AYaccFs:
    // element -> Fs
      {
         if (_context.destination() == RtfContext::document)
            _context.setSize( getParameter() );
      }
      break;

    case AYaccI:
    // element -> I
      {
         if (_context.destination() == RtfContext::document)
            _context.setItalic( getParameter() != 0 );
      }
      break;

    case AYaccPlain:
    // element -> Plain
      if (_context.destination() == RtfContext::document)
        setCharacterDefaults();
      break;

    case AYaccStrike:
    // element -> Strike
    case AYaccUldb:
    // element -> Uldb
    case AYaccUl:
    // element -> Ul
      {
        // HYPERLINK CANDIDATES
        if ( _context.destination() == RtfContext::document )
        {
          parameter = getParameter();
          if ( parameter )
          {
            if ( _linkState == off || _linkType != production )
            {
              // attribute ON
              releaseText();
              flushLink();
              _linkType = production;
              _linkContent = new GinList;
              _linkState = start;
              _linkLevel = 0;
            }
          }
          else if ( _linkType == production )
          {
            // attribute OFF
            endLinkText();
          }
        }
      }
      break;

    case AYaccV:
    // element -> V
      {
         parameter = getParameter();
         if ( _context.destination() == RtfContext::document && parameter)
         {
           // hidden text ON

           // WinHelp forces link text to end when it encounters \v (normal RTF wouldn't)
           endLinkText();

           // set destination to either target or hidden, and prepare to collect target name
           _context.setDestination( ( _linkState == ready )? RtfContext::target: RtfContext::hidden );
           targettext = "";
         }
         else if ( ! parameter && ( _context.destination() == RtfContext::target || _context.destination() == RtfContext::hidden ) )
         {
           // hidden text OFF
           closeDestination( _context.destination() );
           _context.setDestination( RtfContext::document );
         }
      }
      break;


    // PARAGRAPH STYLES --------------------------------------------------

    case AYaccFi:
    // element -> Fi
      {
         if (_context.destination() == RtfContext::document)
         {
           parameter = getParameter();
           _firstMargin = (parameter == NOPARAM)? 0: parameter;
           _context.setLeftMargin( _leftMargin + _firstMargin );
         }
      }
      break;

    case AYaccLi:
    // element -> Li
      {
         if (_context.destination() == RtfContext::document)
         {
           parameter = getParameter();
           _leftMargin = (parameter == NOPARAM)? 0: parameter;
           _context.setLeftMargin( _leftMargin + _firstMargin );
         }
      }
      break;

    case AYaccPard:
    // element -> Pard
      {
         if (_context.destination() == RtfContext::document)
           setParagraphDefaults();
      }
      break;

    case AYaccQc:
    // element -> Qc
      {
         if (_context.destination() == RtfContext::document)
            _context.setAlignment(AlignGin::center);
      }
      break;

    case AYaccQl:
    // element -> Ql
      {
         if (_context.destination() == RtfContext::document)
            _context.setAlignment(AlignGin::left);
      }
      break;

    case AYaccQr:
    // element -> Qr
      {
         if (_context.destination() == RtfContext::document)
            _context.setAlignment(AlignGin::right);
      }
      break;

    case AYaccRi:
    // element -> Ri
      {
         if (_context.destination() == RtfContext::document)
         {
           parameter = getParameter();
           _context.setRightMargin( parameter > 0? parameter: 0 );
         }
      }
      break;

    case AYaccSa:
    // element -> Sa
      {
         if (_context.destination() == RtfContext::document)
         {
            parameter = getParameter();
            afterpara = (parameter == NOPARAM)? 0: parameter;
            _context.setSpacing( beforepara, afterpara );
         }
      }
      break;

    case AYaccSb:
    // element -> Sb
      {
         if (_context.destination() == RtfContext::document)
         {
            parameter = getParameter();
            beforepara = (parameter == NOPARAM)? 0: parameter;
            _context.setSpacing( beforepara, afterpara );
         }
      }
      break;


    // TABLES ------------------------------------------------------------

#ifdef TABLE_SUPPORT

    case AYaccCellx:
    // element -> Cellx
      {
      parameter = getParameter();
      int colwidth = parameter;
      int temp = colwidth;

         colwidth = colwidth - cellwidth;
         cellwidth = temp;
         columnwidths.enqueue(Distance(colwidth,Distance::twips));
      }
      break;

    case AYaccCell:
    // element -> Cell
      {
         if (--colcount) {
            addGin( new ColumnGin() );
         }
      }
      break;

    case AYaccIntbl:
    // element -> Intbl
      {
      int columns = columnwidths.numberOfElements();

         if (columns) {
            numofcolumns = colcount = columns;
            Distance colwidth(0,Distance::twips);
            RulerGin* table;
            table = new RulerGin(RulerGin::table);

            for (int x = columns; x > 0; x--) {
               columnwidths.dequeue(colwidth);
               table->addColumn(colwidth);
            }
            addGin( table );
            cellwidth = 0;
         }
         addGin( new RowGin() );
      }
      break;

    case AYaccRow:
    // element -> Row
      {
         colcount = numofcolumns;
/// Marco used lookahead
///         if (current.subString(cb,15).indexOf("intbl")) colcount = numofcolumns;
///            else {
///               addGin( new RulerGin(RulerGin::normal) );
///            }
      }
      break;

#else
    // NO TABLE SUPPORT -- TRANSLATE CELL AND ROW

    case AYaccCell:
    // element -> Cell
      if (_context.destination() == RtfContext::document)
        processChar( ' ' );
      break;

    case AYaccRow:
    // element -> Row
      if (_context.destination() == RtfContext::document)
        processPara();
      break;

#endif

    // FOOTNOTES ---------------------------------------------------------

    case AYaccFootnote:
    // element -> Footnote
      {
        unsigned length = _holdText.length();
        if ( length )
        {
          footchar = _holdText[length];
          _holdText.remove( length );
          _holdText.stripTrailing();  // strip any spaces immediately before footnote
          releaseText();
          _context.setDestination( RtfContext::footnote );
          foottext = "";
        }
        else
        {
          // no footnote character
          warning( IString("Missing footnote character") );
          _context.setDestination( RtfContext::unknown );
        }
      }
      break;

    case AYaccChftn:
    // element -> Chftn
      releaseText();  // release text held for footnote identification
      break;


    // OTHER DESTINATIONS ------------------------------------------------

    case AYaccBin:
    // element -> Bin
      // binary data not supported!
      IException::assertParameter("Binary picture data not supported", IEXCEPTION_LOCATION());
      break;


    // FIELDS ------------------------------------------------------------

    case AYaccField:
    // element -> Field
      _isPrivateField = false;
      _context.setDestination( RtfContext::unknown );
      break;

    case AYaccFldpriv:
    // element-> Fldpriv
      // this control indicates that the result is not to be displayed
      _isPrivateField = true;
      break;

    case AYaccFldrslt:
    // element-> Fldrslt
      // this destination is found within a \field, and it contains document text
      if ( ! _isPrivateField )
        _context.setDestination( RtfContext::document );
      break;


    // UNSUPPORTED DESTINATIONS ------------------------------------------

    case AYaccComment:
    // element -> Comment
    case AYaccDest:
    // element -> Dest
    case AYaccIndexSubentry:
    // element -> IndexSubentry
    case AYaccFalt:
    // element -> Falt
    case AYaccFooterf:
    // element -> Footerf
    case AYaccFooterl:
    // element -> Footerl
    case AYaccFooterr:
    // element -> Footerr
    case AYaccFooter:
    // element -> Footer
    case AYaccFtncn:
    // element -> Ftncn
    case AYaccFtnsepc:
    // element -> Ftnsepc
    case AYaccFtnsep:
    // element -> Ftnsep
    case AYaccHeaderf:
    // element -> Headerf
    case AYaccHeaderl:
    // element -> Headerl
    case AYaccHeaderr:
    // element -> Headerr
    case AYaccHeader:
    // element -> Header
    case AYaccInfo:
    // element -> Info
    case AYaccObject:
    // element -> Object
    case AYaccPict:
    // element -> Pict
    case AYaccRxe:
    // element -> Rxe
    case AYaccStylesheet:
    // element -> Stylesheet
    case AYaccTc:
    // element -> Tc
    case AYaccTxe:
    // element -> Txe
    case AYaccXe:
    // element -> Xe
      _context.setDestination( RtfContext::unknown );
      break;

  }

  return stackElement();
}



/***************************************************************************
 * Procedure...... RtfYacc::getParameter
 * Author.... Marco
 * Date...... 12/31/1995
 *
 * Determines the parameter value from the current control word.  Returns
 * NOPARAM if no parameter was found.
 ***************************************************************************/
int RtfYacc::getParameter()
{
  int parameter = NOPARAM;
  IString controlWord = lexemeAsString(0);
  int number = controlWord.indexOfAnyOf("-0123456789");
  if (number)
    parameter = controlWord.subString(number).asInt();
  return parameter;
}


/***************************************************************************
 * Procedure.. RtfYacc::setHoldText
 * Author..... Mat
 * Date....... 12/8/98
 *
 * Sets the output text, held for footnote identification.
 ***************************************************************************/
void RtfYacc::setHoldText( const IString & text )
{
  if ( text.length() )
  {
    _holdText = text;
    if ( ! text.isWhiteSpace() )
      _validSection = true;
  }
}


void RtfYacc::processLine()
{
  if ( _context.destination() == RtfContext::document )
  {
    releaseText();
    addGin( new LineGin() );
    _eatNextSpace = false;
  }
}


void RtfYacc::processPara()
{
  if ( _context.destination() == RtfContext::document )
  {
    releaseText();
    addGin( new ParaGin() );
    _eatNextSpace = false;
  }
}


/***************************************************************************
 * Procedure...... RtfYacc::processChar
 * Author.... Marco
 * Date...... 12/31/1995
 *
 * handles hex and other control word symbols.
 ***************************************************************************/
void RtfYacc::processChar(char ch)
{
   /// based on current codepage, need to filter for printable/symbols

   switch (_context.destination())
   {
      case RtfContext::document:
         releaseText();
         if ( ! _eatNextSpace || ch != ' ' )
           setHoldText( ch );
         _eatNextSpace = false;
         break;
      case RtfContext::footnote: foottext += ch;
         break;
      case RtfContext::target: targettext += ch;
         break;
      case RtfContext::fonttable: fdata += ch;
         break;
      case RtfContext::bitmap: _bitmapName += ch;
         break;
   }
}


/***************************************************************************
 * Procedure.. RtfYacc::processSymbol
 * Author..... Mat
 * Date....... 4/11/96
 *
 * Handle Generator (non-ASCII) symbols.
 ***************************************************************************/
void RtfYacc::processSymbol( const Symbol & sym )
{
  if ( _context.destination() == RtfContext::document )
  {
    releaseText();
    addGin( new SymbolGin( sym ) );
    _validSection = true;
    _eatNextSpace = false;
  }
  else
  {
    unsigned char c = sym.asChar( fcodepage );
    if ( c )
      processChar( c );
  }
}



/***************************************************************************
 * Procedure.. RtfYacc::processText
 * Author..... Mat
 * Date....... 3/28/96
 *
 * Sends text to the current destination.
 ***************************************************************************/
void RtfYacc::processText( const IString & text )
{
  if ( ! text.length() )
    return;

  switch (_context.destination())
  {
     case RtfContext::document:
        {
          releaseText();
          unsigned index = 1;
          if ( _eatNextSpace && ( text[1] == ' ' ) )
            index++;
          setHoldText( text.subString( index ) );
          _eatNextSpace = false;
        }
        break;
     case RtfContext::footnote:
        {
           foottext += text;
        }
        break;
     case RtfContext::target:
        {
           targettext += text;
        }
        break;
     case RtfContext::fonttable:
        {
          unsigned end = text.indexOf(';');
          if ( end )
          {
            // found a font name
            // get font name, or override with FORCEFONT
            IString override = _whp->forceFont();
            if ( override.length() )
            {
              // use override font name, and reset family to NIL (unknown)
              fdata = override;
              ffamily = RtfFont::FNIL;
            }
            else
            {
              // get font name from text
              fdata += text.subString( 1, end - 1 );
              fdata.strip();
            }
            RtfFont   fontentry(fnum,fcodepage,ffamily,ffprq,fdata);
            fonttbl.add(fontentry);
            fcodepage = deffcodepage;
            fdata = "";
          }
          else
          {
            // not a full name yet
            fdata += text;
          }
        }
        break;
     case RtfContext::colortable:
        {
          // \red, \blue and \green tags will be parsed separately
          // this loop handles multiple "null" color definitions
          unsigned pos = 0;
          while ( ( pos = text.indexOf(';', pos+1) ) != 0 )
          {
            // found a separator
            RtfColor  colorentry(cred,cgreen,cblue,cnum);
            colortbl.add(colorentry);
            cnum++;
            cred = cblue = cgreen = -1;
          }
        }
        break;
     case RtfContext::bitmap:
        {
           _bitmapName += text;
        }
        break;
  }
}


/***************************************************************************
 * Procedure.. RtfYacc::addBitmap
 * Author..... Mat
 * Date....... 3/7/96
 *
 * Adds an ArtGin
 ***************************************************************************/
void RtfYacc::addBitmap( ArtGin::ArtSpot spot )
{
  // if multiple bitmaps are specified, just use the first
  unsigned separator = _bitmapName.indexOf( ';' );
  if ( separator )
    _bitmapName.remove( separator );

  // strip white space from front and back
  _bitmapName.strip();

  // add the bitmap and/or get the ID
  IString id;
  if ( _whp->getBitmapId( _bitmapName, id ) )
    addGin( new ArtGin( id, spot) );
}



/***************************************************************************
 * Procedure.. RtfYacc::addLabel
 * Author..... Mat
 * Date....... 3/7/96
 *
 * Add the given context ID as a LabelGin.  If this is the first label
 * of the section, then save it for streaming at the front of the list.
 * If getAlias is true, then check for and add aliases as well.
 *
 * NOTE: this function is called recursively for each alias.
 ***************************************************************************/
void RtfYacc::addLabel( const IString & contextId, Boolean getAlias )
{
  // create the LabelGin
  int id;
  if ( _whp->isUniqueContext( contextId, id ) )
  {
    // create LabelGin
    LabelGin * lgin = new LabelGin( contextId );
    lgin->setId( id );

    // add to list if not the first label in current section
    if ( _section->label() )
      addGin( lgin );
    else
      _section->setLabel( lgin );
  }

  // check for aliases
  if ( getAlias )
  {
    IString alias;
    int     i;
    int     n = _whp->numberOfAliases( contextId );
    for (i = 0; i < n; i++)
      if ( _whp->getAlias( contextId, i, alias ) )
        addLabel( alias, false );
  }
}


/***************************************************************************
 * Procedure.. RtfYacc::flushLink
 * Author..... Mat
 * Date....... 4/2/96
 *
 * If link is NULL, then it was a false alarm: send the linkContent out
 * as normal text.  Otherwise, the provided link is used.
 *
 * On entry, there may be some contents in the _holdText or _holdList
 * which were received AFTER the link contents.  These contents should
 * be held for use after the link Gins are sent.  If the link was bogus
 * (e.g., underline), then these contents should be released after the
 * bogus link contents (in the original order they were received).
 *
 * Gins are added directly to the RtfSection to control the ordering.
 ***************************************************************************/
void RtfYacc::flushLink( LinkGin * link )
{
  if ( _linkState > off )
  {
    // release link text
    endLinkText();

    // reset link info
    _linkState = off;

    // start link or other mode (uldb and strike are currently ignored)
    if ( link )
    {
      _section->addGin( *link );
      delete link;
    }
    else if ( _linkType == AYaccUl )
      _section->addGin( UnderlineGin( true ) );

    // send the contents
    _section->addGin( *_linkContent );
    delete _linkContent;
    _linkContent = 0;

    // add PopGins as necessary to return to same level as Link/Underline
    int i;
    for ( i = 0; i < _linkLevel; i++ )
      _section->addGin( PopGin() );

    // if this is a bogus link, release any held contents to the section list
    if ( ! link )
      releaseText( false );

    // stop link or other mode
    if ( link )
      _section->addGin( LinkGin() );
    else if ( _linkType == AYaccUl )
      _section->addGin( UnderlineGin(false) );

    // add PushGins as necessary to return to current level
    // NOTE: this technique may lose context information
    for ( i = 0; i < _linkLevel; i++ )
      _section->addGin( PushGin() );
  }
}



/***************************************************************************
 * Procedure.. RtfYacc::endLinkText
 * Author..... Mat
 * Date....... 4/4/96
 *
 * Called when encountering the end of the link attribute (underline,
 * strikeout or double underline).  If any text has been accumulated, then
 * the state moves to "ready" waiting for a link target (\v) to be given.
 ***************************************************************************/
void RtfYacc::endLinkText()
{
  if ( _linkState == start )
  {
    // release link contents
    releaseText( false );
    // move to ready state
    _linkState = ready;
  }
}



/***************************************************************************
 * Procedure.. RtfYacc::releaseText
 * Author..... Mat
 * Date....... 4/4/96
 *
 * Release text and Gins held for footnote identification.  Default for
 * okToFlushLink is true.
 ***************************************************************************/
void RtfYacc::releaseText( Boolean okToFlushLink )
{
  // flush link if ready (failed link candidate)
  if ( okToFlushLink && _linkState == ready )
    flushLink();

  // check if there is anything held
  if ( ! _holdText.length() && ! _holdList->numberOfElements() )
    return;

  // determine where to release
  GinList * outList;
  if ( _linkState == start )
    outList = _linkContent;
  else
    outList = new GinList;

  // send held text and Gins to linkContents or RtfContext::document
  if ( _holdText.length() )
  {
    outList->add( GinPtr( new TextGin( _holdText ), IINIT ) );
    _holdText = "";
  }
  if ( _holdList->numberOfElements() )
  {
    outList->add( GinPtr( _holdList, IINIT ) );
    _holdList = new GinList;
  }

  // if not in a link, send the text to the RtfSection
  if ( _linkState != start )
  {
    _section->addGin( *outList );
    delete outList;
  }
}


/***************************************************************************
 * Procedure.. RtfYacc::addGin
 * Author..... Mat
 * Date....... 3/28/96
 *
 * Add a Gin to the output Generator.  Currently puts all Gins in a "hold
 * list" which is used to hold Gins until the next footnote can be
 * identified, or until more text comes in.
 *
 * This may be called before a hold list is properly setup (e.g., during
 * RtfContext::reset), and in that case the Gin is ignored and deleted.
 ***************************************************************************/
void RtfYacc::addGin( Gin * gin )
{
  if ( _holdList )
    _holdList->add( GinPtr( gin, IINIT ) );
  else
    delete gin;
}


/***************************************************************************
 * Procedure.. RtfYacc::setParagraphDefaults
 * Author..... Mat
 * Date....... 12/9/97
 *
 * Adds Gins to match the RTF defaults.  Called whenever paragraph defaults
 * are set and at the start of the document.
 ***************************************************************************/
void RtfYacc::setParagraphDefaults()
{
  _leftMargin = _firstMargin = beforepara = afterpara = 0;
  _context.resetParagraphStyles();
#ifdef TABLE_SUPPORT
  /// need to add to RtfContext
  addGin( new RulerGin(RulerGin::normal) );
#endif
}


/***************************************************************************
 * Procedure.. RtfYacc::setCharacterDefaults
 * Author..... Mat
 * Date....... 12/9/97
 *
 * Adds Gins to match the RTF defaults.  Called whenever character defaults
 * are set and at the start of the document.
 ***************************************************************************/
void RtfYacc::setCharacterDefaults()
{
  endLinkText();
  _context.resetCharacterStyles();
}


/***************************************************************************
 * Procedure.. RtfYacc::locateFont
 * Author..... Mat
 * Date....... 2/24/98
 *
 * Given an index into the font table, locate the corresponding
 * RtfFont.  If the element is not found, attempt to locate the
 * default font.  If neither is found, return 0.
 ***************************************************************************/
const RtfFont * RtfYacc::locateFont( int index )
{
  RtfFontTable::Cursor ftcursor(fonttbl);
  Boolean found = fonttbl.locateElementWithKey(index,ftcursor);

  // try the default font
  if ( ! found && index != deff )
    found = fonttbl.locateElementWithKey(deff,ftcursor);

  return found? &(ftcursor.element()): 0;
}


int RtfYacc::defaultFontIndex()
{
  return fonttabledefined? deff: NO_INDEX;
}


/***************************************************************************
 * Procedure.. RtfYacc::locateColor
 * Author..... Mat
 * Date....... 2/24/98
 *
 * Given a key into the color table, locate the corresponding
 * RtfColor.  If the element isn't found return 0.
 ***************************************************************************/
const RtfColor * RtfYacc::locateColor( int key )
{
  RtfColorTable::Cursor cfcursor(colortbl);
  Boolean found = colortbl.locateElementWithKey(key,cfcursor);
  return found? &(cfcursor.element()): 0;
}


int RtfYacc::mapFontSize( int sizeInTwips ) const
{
  return _whp->mapFontSize( sizeInTwips );
}


/***************************************************************************
 * Procedure.. RtfYacc::closeDestination()
 * Author..... Mat
 * Date....... 4/2/96
 *
 *
 ***************************************************************************/
void RtfYacc::closeDestination( RtfContext::Destination d )
{
  switch ( d )
  {
     case RtfContext::document:
        // check for end of document
        if ( _stack.isEmpty() )
          sectionDone( true );
        break;
     case RtfContext::footnote:          // process footnote group
        {
           foottext.strip(footchar);
           switch (footchar)
           {
              case '#':
                 addLabel( foottext.strip(), true );
                 _validSection = true;
                 break;

              case '$':
                 _section->setTitle( foottext.strip() );
                 _validSection = true;
                 break;

              case 'k':
              case 'K':
                 sendKeywords( foottext, true );
                 _validSection = true;
                 break;

              case 'a':
              case 'A':
                 sendKeywords( foottext, false );
                 _validSection = true;
                 break;

              // unsupported footnotes
              case '*':  // build tag
              case '>':  // window selection
              case '+':  // browse sequence
              case '!':  // macro
              case '@':  // comment
                 break;

              default:
                 warning( IString("Unknown footnote character (") + IString(footchar) + IString(")") );
                 break;
           }
           // WinHelp compiler eats a space if it follows a footnote
           _eatNextSpace = true;
        }
        break;
     case RtfContext::target:       // hyperlink target address
        {
           IString filename;
           // window is currently ignored, since LinkGin doesn't accept window ID
           if ( _whp->parseTopicId( targettext, &filename, 0 ) )
           {
             // send the link text
             LinkGin * link = new LinkGin( targettext );
             if ( filename.length() )
               link->setFile( filename );
             flushLink( link );
           }
        }
        break;
     case RtfContext::colortable:      // end of colortable
        break;
     case RtfContext::fonttable:       // end of font table
        fonttabledefined=true;
        break;
     case RtfContext::bitmap:          // end of an embedded bitmap
        {
          switch ( _bitmapType )
          {
            case AYaccBmc:
            case AYaccBmcwd:
            case AYaccBmct:
              addBitmap( ArtGin::bottom );
              break;

            case AYaccBml:
            case AYaccBmlwd:
            case AYaccBmlt:
              addBitmap( ArtGin::left );
              break;

            case AYaccBmr:
            case AYaccBmrwd:
            case AYaccBmrt:
              addBitmap( ArtGin::right );
              break;
          }
        }
        break;
  }
}


/***************************************************************************
 * Procedure.. RtfYacc::sendKeywords
 * Author..... Mat
 * Date....... 1/7/99
 *
 * Send keywords from a 'K' or 'A' footnote.
 ***************************************************************************/
void RtfYacc::sendKeywords( const IString & keywords, Boolean isListed )
{
  // keyword text should always be translated to default codepage
  CodePage cpOutput( deffcodepage );
  CodePage cpInput( fcodepage );

  // use the separators from WinHelpParser
  unsigned start = 1;
  unsigned end;
  IString key;
  do
  {
    // find next substring
    end = keywords.indexOf( _whp->separator(), start );
    if ( end )
      key = keywords.subString( start, end - start );
    else
      key = keywords.subString( start );
    start = end + 1;

    // translate and send the keyword
    key.strip();
    if ( key.length() )
    {
      cpOutput.translateString( key, cpInput );
      addGin( new KeywordGin( key, isListed ) );
    }
  }
  while ( end );
}


/***************************************************************************
 * Procedure.. RtfYacc::rebuildContextStack
 * Author..... Mat
 * Date....... 1/21/99
 *
 * Rebuild the context stack by sending Gins (including PushGins).  This is
 * called after a section is sent or reset.
 ***************************************************************************/
void RtfYacc::rebuildContextStack()
{
  const RtfContext * previous = 0;
  StackType::Cursor cursor( _stack );
  forCursor( cursor )
  {
    const RtfContext * current = &(cursor.element());
    if ( current->destination() == RtfContext::document )
    {
      current->sendAll( previous );
      addGin( new PushGin() );
      previous = current;
    }
  }

  // force styles to match current context
  _context.sendAll( previous );
}


/***************************************************************************
 * Procedure.. RtfYacc::sectionDone
 * Author..... Mat
 * Date....... 1/21/99
 *
 * Called when a section is done, either because of a \page tag, or because
 * of the end of the document.
 ***************************************************************************/
void RtfYacc::sectionDone( Boolean isDocumentDone )
{
  int i;

  // flush all text
  releaseText();
  flushLink();
  _eatNextSpace = false;

  // check for (and skip) invalid section
  if ( ! _validSection )
  {
    // reset the section
    _section->reset();
    if ( isDocumentDone )
    {
      delete _section;
      _section = 0;
    }
    else
    {
      rebuildContextStack();
    }

    // skip the rest
    return;
  }

  // make sure there is no open context
  StackType::Cursor cursor( _stack );
  forCursor( cursor )
  {
    if ( cursor.element().destination() == RtfContext::document )
      _section->addGin( PopGin() );
  }

  // add the section to the list managed by WinHelpParser
  _section->finish();
  Boolean added = _whp->addSection( _section );
  if ( ! added )
  {
    warning( IString("Unable to add section (") + _section->title() + IString(")") );
    delete _section;
  }
  _section = 0;

  // indicate progress to user
  if ( _whp->indicator() )
    _whp->indicator()->setCurrent( offset() );

  // prepare for next section
  if ( ! isDocumentDone )
  {
    _section = new RtfSection();
    _validSection = false;

    rebuildContextStack();
  }
}


/***************************************************************************
 * Procedure.. RtfYacc::warning
 * Author..... Mat
 *
 * Log a warning condition.
 ***************************************************************************/
void RtfYacc::warning( const IString & text, const SSLexLexeme * lexeme )
{
  unsigned line = 0;
  unsigned col = 0;

  // get current location, if possible
  if ( ! lexeme )
  {
    SSYaccStackElement * element = elementFromStack( 0 );
    MEKASSERT( element );
    lexeme = element->lexeme();
  }

  // determine line and column
  if ( lexeme )
  {
    line = lexeme->line();
    col = lexeme->offset();
  }

  _whp->warning( text, line, col );
}


