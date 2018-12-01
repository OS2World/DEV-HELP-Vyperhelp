/***************************************************************************
 * File...... Gin.hpp
 * Author.... Mat
 * Date...... 9/21/95
 *
 * Gin stands for "Generator INput".  Gins are items that are streamed
 * into a Generator using the << operator.  The Generator interprets
 * the Gin and outputs to the target format accordingly.
 *
 * All Gins are derived from the base "Gin" class.
 *
 * Copyright (C) 1995 MekTek
 ***************************************************************************/
#ifndef GIN_HPP
#define GIN_HPP

// OpenClass
#include <istring.hpp>
#include <igbitmap.hpp>
#include <icolor.hpp>
#include <iseq.h>
#include <iptr.h>

// Generator
#include "Distance.hpp"
#include "Symbol.hpp"
#include "FontInfo.hpp"
class Generator;
class ColumnList;


/***************************************************************************
 * Class...... Gin
 * Author..... Mat
 * Date....... 9/21/95
 *
 * Base class for all Gins.  The streamTo function performs a << operation
 * to the given Generator.
 ***************************************************************************/
class _Export Gin
{
  public:
    // destructor
    virtual ~Gin();

    // streamer
    virtual void streamTo( Generator & ) const = 0;
};



/***************************************************************************
 * Procedure.. GinPtr, GinList
 * Author..... Mat
 * Date....... 12/1/95
 *
 * GinPtr is a pointer to a Gin which deletes itself after being deleted
 * from the list.  GinList is a sequence of Gins which can be streamed to
 * a Generator.  Since a GinList is also a Gin, you can have lists of lists
 * and stream them with a single operation.
 ***************************************************************************/
typedef IAutoElemPointer< Gin > GinPtr;
class _Export GinList: public Gin, public ISequence< GinPtr >
{
  public:
    // destructor
    virtual ~GinList();

    // from Gin
    virtual void streamTo( Generator & ) const;
};


//
// DOCUMENT CONTROL --------------------------------------------------------
//


/***************************************************************************
 * Class...... SectionGin
 * Author..... Mat
 * Date....... 9/21/95
 *
 * Introduces a new section header in the document.  level gives the hierar-
 * ical level of the heading, numbering from 1, no skipping levels.
 * isListed says whether the section should appear in the table of
 * contents.  The default title is an empty string.  The window name should
 * correspond to a previously defines window's name.
 *
 * A section resets all paragraph and character settings to the target
 * format's defaults.
 ***************************************************************************/
class _Export SectionGin: public Gin
{
  public:
    // constructors
    SectionGin( int level, Boolean isListed );

    // optional settings
    SectionGin
        & setTitle( const IString & title ),
        & setWindow( int windowId );  // ID of a defined window

    // query functions
    const IString & title() const;
    int             level() const;
    Boolean         isListed() const;
    int             windowId() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    IString _title;
    int     _level;
    Boolean _isListed;
    int     _windowId;
};


/***************************************************************************
 * Class...... KeywordGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Sets a keyword (index entry) for the current section. isListed
 * tells whether the entry is listed in the index listing.  If not, it is
 * used for searches only.
 * isExternal tells whether the keyword is made accessible outside this
 * document.
 ***************************************************************************/
class _Export KeywordGin: public Gin
{
  public:
    // constructor
    KeywordGin( const IString & text, Boolean isListed );

    // optional settings
    KeywordGin & setExternal( Boolean isExternal );

    // query functions
    const IString & text() const;
    Boolean         isListed() const;
    Boolean         isExternal() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    IString _text;
    Boolean _isListed;
    Boolean _isExternal;
};


/***************************************************************************
 * Class...... LabelGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Labels the current position as a hyperlink target.  If no ID is provided,
 * a unique one will be created by the Generator.
 ***************************************************************************/
class _Export LabelGin: public Gin
{
  public:
    // constructor
    LabelGin( const IString & name );

    // optional settings
    LabelGin
        & setExternal( Boolean isExternal ),
        & setId( int id );

    // query functions
    const IString & name() const;
    Boolean         isExternal() const;
    int             id() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    IString _name;
    int     _id;
    Boolean _isExternal;
};


/***************************************************************************
 * Class...... RowGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Indicates that a new row is starting (return to column 1).
 ***************************************************************************/
class _Export RowGin: public Gin
{
  public:
    // streamer
    virtual void streamTo( Generator & ) const;
};

/***************************************************************************
 * Class...... ColumnGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Indicates that a new column is starting (move to next column).
 ***************************************************************************/
class _Export ColumnGin: public Gin
{
  public:
    // streamer
    virtual void streamTo( Generator & ) const;
};

/***************************************************************************
 * Class...... ItemGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Indicates that a new item in a list/outline is starting.
 ***************************************************************************/
class _Export ItemGin: public Gin
{
  public:
    // streamer
    virtual void streamTo( Generator & ) const;
};

/***************************************************************************
 * Class...... ParaGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Indicates a new paragraph.
 ***************************************************************************/
class _Export ParaGin: public Gin
{
  public:
    // streamer
    virtual void streamTo( Generator & ) const;
};

/***************************************************************************
 * Class...... LineGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Forces a new line.
 ***************************************************************************/
class _Export LineGin: public Gin
{
  public:
    // streamer
    virtual void streamTo( Generator & ) const;
};



//
// NESTING CONTROL ---------------------------------------------------------
//


/***************************************************************************
 * Class...... PushGin
 * Author..... Mat
 * Date....... 9/21/95
 *
 * Pushes the current state onto a stack.
 ***************************************************************************/
class _Export PushGin: public Gin
{
  public:
    // streamer
    virtual void streamTo( Generator & ) const;
};


/***************************************************************************
 * Class...... PopGin
 * Author..... Mat
 * Date....... 9/21/95
 *
 * Pops the last state from the stack.
 ***************************************************************************/
class _Export PopGin: public Gin
{
  public:
    // streamer
    virtual void streamTo( Generator & ) const;
};



//
// PARAGRAPH CONTROL -------------------------------------------------------
//


/***************************************************************************
 * Class...... MarginGin
 * Author..... Mat
 * Date....... 9/21/95
 *
 * Sets a margin.  isRelative tells whether the distance is relative
 * to the current margin (if not, it is absolute).
 *
 * ABSTRACT CLASS: no srtreamTo function defined
 ***************************************************************************/
class _Export MarginGin: public Gin
{
  public:
    // constructor
    MarginGin( Boolean isRelative, const Distance & margin );

    // query functions
    Boolean          isRelative() const;
    const Distance & margin() const;

  private:
    Boolean  _isRelative;  // is the margin distance relative?
    Distance _margin;
};


class _Export RightMarginGin: public MarginGin
{
  public:
    // constructor
    RightMarginGin( Boolean isRelative, const Distance & margin );

    // streamer
    virtual void streamTo( Generator & ) const;
};

class _Export LeftMarginGin: public MarginGin
{
  public:
    // constructor
    LeftMarginGin( Boolean isRelative, const Distance & margin );

    // streamer
    virtual void streamTo( Generator & ) const;
};


/***************************************************************************
 * Class...... BorderGin
 * Author..... Mat
 * Date....... 9/22/95
 *
 * Controls border around paragraphs and between columns/rows.  The borders
 * are turned on by ORing values from the Borders enum.
 ***************************************************************************/
class _Export BorderGin: public Gin
{
  public:
    // enumerators
    enum Borders {
      top         = 0x01,
      bottom      = 0x02,
      left        = 0x04,
      right       = 0x08,
      vertical    = 0x10,
      horizontal  = 0x20,
      // useful combinations follow
      frame       = 0x0f,
      rules       = 0x30,
      all         = 0x3f
    };

    // constructor
    BorderGin( int borders );

    // query functions
    int borders() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    int _borders;
};


/***************************************************************************
 * Class...... AlignGin
 * Author..... Mat
 * Date....... 10/3/95
 *
 * Sets paragraph alignment.
 ***************************************************************************/
class _Export AlignGin: public Gin
{
  public:
    // enumerations
    enum Alignment {
      left,          // align text to left edge
      right,         // align text to right edge
      center,        // center text within display area
      justify        // justify text across display area
    };

    // constructor
    AlignGin( Alignment alignment );

    // query functions
    Alignment alignment() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    Alignment _alignment;
};


/***************************************************************************
 * Class...... SpacingGin
 * Author..... Mat
 * Date....... 10/3/95
 *
 * Sets the distance (blank space) between paragraphs.
 ***************************************************************************/
class _Export SpacingGin: public Gin
{
  public:
    // constructor
    SpacingGin( const Distance & spacing );

    // query functions
    const Distance & spacing() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    Distance _spacing;
};


/***************************************************************************
 * Class...... RulerGin
 * Author..... Mat
 * Date....... 9/22/95
 *
 * RulerGin controls columns and wrapping mode.  The column information
 * is used only in the "dlist" and "table" modes.  For "dlist", only
 * the first column width is used.
 * Columns are sorted as they are added, and units may be converted.
 ***************************************************************************/
class _Export RulerGin: public Gin
{
  public:
    // enumerators & types
    enum Mode {
      normal,     // normal single column text
      list,       // single column list
      quote,      // indented slightly on both sides
      preformat,  // preformatted text, no text wrapping
      dlist,      // two-column list
      table       // multi-column table
    };
    enum LineBreak {  // for two-column lists:
      none,           //   never start 2nd column on a new line
      fit,            //   if 1st column overruns, start 2nd column on a new line
      all             //   always start 2nd column on a new line
    };

    // constructor
    RulerGin( Mode mode );
    virtual ~RulerGin();

    // additional settings
    RulerGin
        & addColumn( const Distance & column ),
        & setLineBreak( LineBreak lineBreak );

    // query functions
    Mode             mode() const;
    LineBreak        lineBreak() const;
    const Distance & column( IPosition index ) const;
    INumber          numberOfColumns() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    Mode         _mode;
    LineBreak    _lineBreak;
    ColumnList * _columns;
};


/***************************************************************************
 * Class...... OutlineGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Controls paragraph numbering & bulleting
 ***************************************************************************/
class _Export OutlineGin: public Gin
{
  public:
    // constructor
    enum Style {
      none,
      bullet,
      arabic,
      romanLower,
      romanUpper,
      alphaLower,
      alphaUpper
    };
    OutlineGin( Style style );

    // optional settings
    OutlineGin
        & setPrefix( const IString & prefix ),
        & setSuffix( const IString & suffix );

    // query functions
    Style           style() const;
    const IString & prefix() const;
    const IString & suffix() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    Style   _style;
    IString _prefix;
    IString _suffix;
};


/***************************************************************************
 * Class...... DividerGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Inserts a horizontal dividing rule. (HTML)
 ***************************************************************************/
class _Export DividerGin: public Gin
{
  public:
    // streamer
    virtual void streamTo( Generator & ) const;
};




//
// CHARACTER CONTROL -------------------------------------------------------
//



/***************************************************************************
 * Class...... FontGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Sets the font family, facename and codepage.  If the facename can't be
 * matched exactly, the family is used to find a close match.  Codepages
 * are numbers from the CODEPAGE command in the CONFIG.SYS, or one of the
 * enumerated values below.  If no codepage is specified, the system
 * default is used.
 ***************************************************************************/
class _Export FontGin: public Gin
{
  public:
    // constructor
    FontGin( FontInfo::Family family, const IString & facename );

    // optional settings
    FontGin & setCodepage( CodePage codepage );

    // query functions
    FontInfo::Family family() const;
    const IString &  facename() const;
    CodePage         codepage() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    FontInfo::Family _family;
    IString          _facename;
    CodePage         _codepage;
};


/***************************************************************************
 * Class...... SizeGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Sets text size (height).
 ***************************************************************************/
class _Export SizeGin: public Gin
{
  public:
    // constructor
    SizeGin( const Distance & height );

    // query functions
    const Distance & height() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    Distance _height;
};


/***************************************************************************
 * Class...... ColorGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Sets text foreground color
 ***************************************************************************/
class _Export ColorGin: public Gin
{
  public:
    // constructor
    ColorGin( const IColor & color );

    // query functions
    const IColor & color() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    IColor _color;
};


/***************************************************************************
 * Class...... BackColorGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Sets background color
 ***************************************************************************/
class _Export BackColorGin: public ColorGin
{
  public:
    // constructor
    BackColorGin( const IColor & color );

    // streamer
    virtual void streamTo( Generator & ) const;
};




/***************************************************************************
 * Class...... AttributeGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Base class for all Gins which control an on/off attribute, including:
 *   BoldGin       bold text
 *   ItalicGin     italic text
 *   UnderlineGin  underline text
 *   CommentGin    comment text (not shown to user, but saved)
 *   HideGin       hidden text (based on a key)
 *   LinkGin       hyperlink text
 *
 * ABSTRACT GIN CLASS: no streamTo function
 ***************************************************************************/
class _Export AttributeGin: public Gin
{
  public:
    // query functions
    Boolean isOn() const;

  protected:
    // constructor
    AttributeGin( Boolean isOn );

  private:
    Boolean _isOn;
};

class _Export BoldGin: public AttributeGin
{
  public:
    // constructor
    BoldGin( Boolean isOn );

    // streamer
    virtual void streamTo( Generator & ) const;
};

class _Export ItalicGin: public AttributeGin
{

  public:
    // constructor
    ItalicGin( Boolean isOn );

    // streamer
    virtual void streamTo( Generator & ) const;
};

class _Export UnderlineGin: public AttributeGin
{
  public:
    // constructor
    UnderlineGin( Boolean isOn );

    // streamer
    virtual void streamTo( Generator & ) const;
};

class _Export CommentGin: public AttributeGin
{
  public:
    // constructor
    CommentGin( Boolean isOn );

    // streamer
    virtual void streamTo( Generator & ) const;
};


/***************************************************************************
 * Class...... LinkGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * If no target is given, turns off hyperlink text.  Otherwise, turns on
 * hyperlink text and sets the target location.  use setFile to specify
 * an external link.
 ***************************************************************************/
class _Export LinkGin: public AttributeGin
{
  public:
    // constructor
    LinkGin( const IString & target );
    LinkGin();

    // optional settings
    LinkGin & setFile( const IString & file );

    // query functions
    Boolean         isExternal() const;
    const IString & file() const;
    const IString & target() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    IString _file;  // empty string if current file
    IString _target;
};


/***************************************************************************
 * Class...... HideGin
 * Author..... Mat
 * Date....... 9/26/95
 *
 * Turns on hidden text, based on the supplied key value. If no key is given,
 * hiding is turned off.  Multiple keys are currently not supported.
 ***************************************************************************/
class _Export HideGin: public AttributeGin
{
  public:
    // constructor
    HideGin( const IString & key );
    HideGin();

    // query functions
    const IString & key() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    IString _key;
};




//
// ARTWORK -----------------------------------------------------------------
//


/***************************************************************************
 * Class...... ArtGin
 * Author..... Mat
 * Date....... 11/30/95
 *
 * An ArtGin is used to place bitmap artwork in the document.  The bitmap
 * can be either placed inline (top, middle, or bottom aligned), at a margin
 * (left or right) or centered on its own line.
 ***************************************************************************/
class _Export ArtGin: public Gin
{
  public:
    // enumerators
    enum ArtSpot
    {
      top,      // runin, top of line
      middle,   // runin, middle of line
      bottom,   // runin, bottom of line
      left,     // at left margin, flow text around
      right,    // at right margin, flow text around
      center,   // centered on its own line
      fit       // stretch to fit the window
    };

    // constructors
    ArtGin( const IString & id, ArtSpot spot );

    // query functions
    const IString &  id() const;
    ArtSpot          spot() const;

    // streamer
    virtual void streamTo( Generator & ) const;

  private:
    IString  _id;
    ArtSpot  _spot;
};



//
// TEXT --------------------------------------------------------------------
//

/***************************************************************************
 * Class...... TextGin
 * Author..... Mat
 * Date....... 12/5/95
 *
 * TextGin holds any IString text.  newlines are ignored, except when in
 * "preformatted" mode (see RulerGin).
 ***************************************************************************/
class _Export TextGin: public Gin
{
  public:
    // constructors
    TextGin( const IString & string );

    // accessor
    const IString & string() const;

    // streamer
    virtual void streamTo( Generator & gen ) const;

  private:
    IString _string;
};


/***************************************************************************
 * Class...... SymbolGin
 * Author..... Mat
 * Date....... 12/5/95
 *
 * SymbolGin hold a Symbol (any non-ASCII typographical character)
 ***************************************************************************/
class _Export SymbolGin: public Gin
{
  public:
    // constructor
    SymbolGin( const Symbol & symbol );

    // accessor
    const Symbol & symbol() const;

    // streamer
    virtual void streamTo( Generator & gen ) const;

  private:
    Symbol _symbol;
};


// Inline functions
#include "Gin.ipp"


#endif

