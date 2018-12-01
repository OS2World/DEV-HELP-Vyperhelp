/***************************************************************************
 * File...... FlowGenerator.hpp
 * Author.... Mat
 * Date...... 2/3/98
 *
 * This class generates contents into a FlowItem (converting Gins into
 * Items).
 *
 * Copyright (C) 1998 MekTek
 ***************************************************************************/
#ifndef FLOWGENERATOR_HPP
#define FLOWGENERATOR_HPP

// OpenClass
#include <istack.h>

// Generator
#include "Generator.hpp"

// Editor
#include "FlowGeneratorContext.hpp"
class FlowItem;


class _Export FlowGenerator: public Generator
{
  public:
    // constructor
    FlowGenerator( FlowItem & flow, Boolean checkArt = true );

    // from Generator
    virtual void handlePara( const ParaGin & );
    virtual void handleLine( const LineGin & );
    virtual void handlePush( const PushGin & );
    virtual void handlePop( const PopGin & );
    virtual void handleFont( const FontGin & );
    virtual void handleSize( const SizeGin & );
    virtual void handleBold( const BoldGin & );
    virtual void handleItalic( const ItalicGin & );
    virtual void handleUnderline( const UnderlineGin & );
    virtual void handleLink( const LinkGin & );
    virtual void handleArt( const ArtGin & );
    virtual void handleText( const TextGin & );
    virtual void handleSymbol( const SymbolGin & );
    virtual void finish();

private:
    // helpers
    void flushText();
    void flushFont();
    void appendItem( Item * item );

    // flow being manipulated
    FlowItem &           _flow;

    // stack for push/pop correlation
    IStack< FlowGeneratorContext > _nest;

    // current context
    FlowGeneratorContext _context;
    Boolean              _isFontChanged;

    // cached text
    IString              _text;

    // art check indicator
    Boolean              _checkArt;
};


#endif

