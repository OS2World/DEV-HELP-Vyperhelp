/***************************************************************************
 * File...... HtmlControl.cpp
 *
 * Contains all Gin stream operators for document control.  Depending on
 * the current context's mode, these Gins are interpreted differently.
 * The following table lists HTML tags sent for each mode (rows), when the
 * Gin (column) is streamed.
 *
 *               ParaGin  LineGin  RowGin   ColGin   ItemGin
 * paragraph     <p>      <br>
 * singleList    <p>      <br>                       <li>
 * lines         CR/LF    CR/LF
 * doubleList    <p>      <br>     <dt>     <dd>
 * table         <p>      <br>     <tr><td> <td>
 *
 * Also, the SpacingGin determines the number of lines between paragraphs.
 * If spacing=0, then <br> is used in place of <p>.  If spacing >1 then
 * that many <p>'s will be used.
 *
 * Copyright (C) 2000 MekTek
 ***************************************************************************/

#include "HtmlCommon.hpp"


// tag definitions
const HtmlTag HtmlGenerator::tagListItem("li", true, HtmlGenerator::breakBefore);
const HtmlTag HtmlGenerator::tagListTerm("dt", true, HtmlGenerator::breakBefore);
const HtmlTag HtmlGenerator::tagListDef("dd", true, HtmlGenerator::breakBefore);
const HtmlTag HtmlGenerator::tagTableRow("tr", true, HtmlGenerator::breakBefore);
const HtmlTag HtmlGenerator::tagTableCol("td", true, HtmlGenerator::breakBefore);



/***************************************************************************
 * Procedure.. HtmlGenerator::sendControlTag
 *
 * This is used to send tags that always begin immediately after they end.
 * For example: <li> and <p>.  Also supports list items, and table rows
 * and columns.
 ***************************************************************************/
void HtmlGenerator::sendControlTag(const HtmlTag & tag, const HtmlTag & tagNext, Boolean isRow)
{
  // make sure tags are output and stack is up to date
  prepareToSend();

  // find start of last matching tag
  IPosition posStart = findStartTag(tag.plain());

  // undo intervening inline tags
  if (! posStart)
  {
    ITRACE_DEVELOP(IString("sendControlTag: no start tag found: ") + tag.start());
    DumpTagStack();
    return;
  }
  else
  {
    undoTags();
  }

  // end the previous
  sendTag(tag.end(), tag.flags());

  // if changing tags, then remove this tag from the stack
  Boolean changeTag = (tag != tagNext);
  if (changeTag)
  {
    deleteTag(posStart);
  }

  // handle row transition
  if (isRow)
  {
    IPosition posRow = findStartTag(tagTableRow.plain());
    if (! posRow)
    {
      ITRACE_DEVELOP("nextCell: no <tr> found");
      DumpTagStack();
    }
    else
    {
      sendTag(tagTableRow.end(), tagTableRow.flags());
      sendTag(tagTableRow.start(), tagTableRow.flags());
    }
  }

  // if changing tags, insert the new tag in the start position
  if (changeTag)
  {
    insertTag(tagNext, posStart);
  }

  // start the new
  sendTag(tagNext.start(), tagNext.flags());

  // redo inline tags
  redoTags();
}


/***************************************************************************
 * Procedure.. HtmlGenerator::sendEndTag
 *
 * This is the standard routine for ending a tag group.  Some tags (tr, td,
 * li, etc) require special handling because of their relation with other
 * tags.
 *
 * If mode is other than paragraph, then this tag is ending a paragraph
 * mode.
 *
 * This routine also handles row/item and column tags for lists and tables.
 *
 * Callers would generally update the HtmlContext after calling this routine.
 ***************************************************************************/
void HtmlGenerator::sendEndTag(const IString & tagString, HtmlMode mode)
{
  // locate start tag on stack
  IPosition pos = findStartTag(tagString);
  if (! pos)
  {
    // start tag not found
    ITRACE_DEVELOP(IString("SendEndTag: start not found, ") + tagString);
    DumpTagStack();
    return;
  }

  // locate "item" tag if applicable
  IPosition posItem = 0;
  const HtmlTag * tagItem = 0;
  switch (mode)
  {
    case singleList:
      tagItem = &tagListItem;
      posItem = findStartTag(tagListItem.plain());
      if (! posItem)
      {
        ITRACE_DEVELOP("sendEndTag: no <li> found");
        DumpTagStack();
      }
      break;

    case doubleList:
      tagItem = &tagListDef;
      posItem = findStartTag(tagListDef.plain());
      if (! posItem)
      {
        ITRACE_DEVELOP("sendEndTag: no <dd> found");
        DumpTagStack();
        tagItem = &tagListTerm;
        posItem = findStartTag(tagListTerm.plain());
        if (! posItem)
        {
          ITRACE_DEVELOP("sendEndTag: no <dt> found");
          DumpTagStack();
        }
      }
      break;

    case table:
      tagItem = &tagTableRow;
      posItem = findStartTag(tagTableRow.plain());
      if (! posItem)
      {
        ITRACE_DEVELOP("sendEndTag: no <tr> found");
        DumpTagStack();
      }
      break;
  }
  // make sure the item found is not from an earlier list
  if (posItem < pos)
  {
    posItem = 0;
  }

  // locate "column" tag if applicable
  IPosition posCol = 0;
  const HtmlTag * tagCol = 0;
  if (posItem && (mode == table))
  {
      tagCol = &tagTableCol;
      posCol = findStartTag(tagTableCol.plain());
      // make sure the column found is not from an earlier row
      if (posCol < posItem || posCol < pos)
      {
        ITRACE_DEVELOP("sendEndTag: no <td> found");
        DumpTagStack();
        posCol = 0;
      }
  }

  IPosition posUndo;
  const HtmlTag & tag = _tags->elementAtPosition(pos);
  if (tag.isBlock())
  {
    posUndo = 0;
  }
  else
  {
    posUndo = pos;
  }



  // undo tags after start
  undoTags(posUndo, posItem);


  // undo tags after column, then end the column
  if (posCol)
  {
    sendTag(tagCol->end(), tagCol->flags());
  }

  // undo tags between column and item/row, then end the item
  if (posItem)
  {
    sendTag(tagItem->end(), tagItem->flags());
  }

  // send the end tag
  sendTag(tag.end(), tag.flags());

  // restore the inline tags
  redoTags(posUndo);

  // remove the tags (remember: delete in reverse order to maintain valid pos!)
  if (posCol)
  {
    deleteTag(posCol);
  }
  if (posItem)
  {
    deleteTag(posItem);
  }
  deleteTag(pos);
}


/***************************************************************************
 * Procedure.. HtmlGenerator::sendStartTag
 *
 * Send the start tag for a pair of tags.  If the tag is a block tag, we need
 * special logic to avoid nexting it inside an inline tag.
 *
 * If mode is other than paragraph, then this tag is ending a paragraph
 * mode.
 *
 * This routine also handles row/item and column tags for lists and tables.
 ***************************************************************************/
void HtmlGenerator::sendStartTag(const HtmlTag & tag, HtmlMode mode)
{
  // if this is a block tag, undo all inline tags since last block (or since start)
  if (tag.isBlock())
  {
    undoTags();
  }

  // for tables, prefix with a <p> for better spacing
  if (mode == table)
  {
    sendTag("p", breakBefore);
  }

  // send the new tag and push
  sendTag(tag.start(), tag.flags());
  _tags->add(tag);

  // special cases for lists and tables
  switch (mode)
  {
  case singleList:
    // send list item separator and push
    sendTag(tagListItem.start(), tagListItem.flags());
    _tags->add(tagListItem);
    break;

  case doubleList:
    // send list item separator and push
    sendTag(tagListTerm.start(), tagListTerm.flags());
    _tags->add(tagListTerm);
    break;

  case table:
    // send table row and column
    sendTag(tagTableRow.start(), tagTableRow.flags());
    _tags->add(tagTableRow);
    sendTag(tagTableCol.start(), tagTableCol.flags());
    _tags->add(tagTableCol);
    break;
  }

  // redo inline tags (if they were undone)
  if (tag.isBlock())
  {
    redoTags();
  }
}


/***************************************************************************
 * Procedure.. HtmlGenerator::findStartTag
 *
 * Search from the top of the stack to find the topmost tag that matches
 * the given tag.
 ***************************************************************************/
IPosition HtmlGenerator::findStartTag(const IString & tag)
{
  // strip attributes from tag
  IString word = tag.word(1);

  // search from top of stack for match
  HtmlTagStack::Cursor cursor(*_tags);
  IBoolean ok = cursor.setToLast();
  while (ok)
  {
    if (cursor.element().plain() == word)
    {
      // found a block tag
      return _tags->position(cursor);
    }
    ok = cursor.setToPrevious();
  }

  // none found
  return 0;
}


/***************************************************************************
 * Procedure.. HtmlGenerator::findLastBlock
 *
 * Search from the top of the tag stack and return the position of the
 * topmost block tag.  Return 0 if none is found.
 ***************************************************************************/
IPosition HtmlGenerator::findLastBlock()
{
  // search from top of stack for block tag
  HtmlTagStack::Cursor cursor(*_tags);
  IBoolean ok = cursor.setToLast();
  while (ok)
  {
    if (cursor.element().isBlock())
    {
      // found a block tag
      return _tags->position(cursor);
    }
    ok = cursor.setToPrevious();
  }

  // none found
  return 0;
}


/***************************************************************************
 * Procedure.. HtmlGenerator::undoTags
 *
 * Undo all inline tags after the specified position (in reverse order).
 * See comments with redoTags().  If top is 0 (default), use the real top
 * of the stack.
 ***************************************************************************/
void HtmlGenerator::undoTags(IPosition pos, IPosition top)
{
  // if no top was provided, query the stack
  if (! top)
  {
    top = _tags->numberOfElements();
  }

  // starting at top back to index + 1, undo all non-block tags
  while (top > pos)
  {
    const HtmlTag & tag = _tags->elementAtPosition(top);
    if (! tag.isBlock())
    {
      sendTag(tag.end(), tag.flags());
    }
    top--;
  }
}


/***************************************************************************
 * Procedure.. HtmlGenerator::redoTags
 *
 * Redo all inline tags after the specified position.  This is done to handle
 * the fact that inline tags can not include block tags.  Generator has
 * no such restriction, so the tag context must be undone and then redone
 * in order to support a nested block tag.
 ***************************************************************************/
void HtmlGenerator::redoTags(IPosition pos)
{
  // starting at pos + 1, redo all non-block tags
  while (++pos <= _tags->numberOfElements())
  {
    const HtmlTag & tag = _tags->elementAtPosition(pos);
    if (! tag.isBlock())
    {
      sendTag(tag.start(), tag.flags());
    }
  }
}


/***************************************************************************
 * Procedure.. HtmlGenerator::deleteTag
 *
 * Remove the tag from the specified position on the stack. The tags above
 * are not affected.
 ***************************************************************************/
void HtmlGenerator::deleteTag(IPosition pos)
{
  _tags->removeAtPosition(pos);
}


/***************************************************************************
 * Procedure.. HtmlGenerator::insertTag
 *
 * Insert a tag at (before) the given position.
 ***************************************************************************/
void HtmlGenerator::insertTag(const HtmlTag & tag, IPosition pos)
{
  _tags->addAtPosition(pos, tag);
}


#ifdef IC_TRACE_DEVELOP
void HtmlGenerator::DumpTagStack() const
{
  IString stack = "|";
  HtmlTagStack::Cursor cursor(*_tags);
  forCursor(cursor)
  {
    stack += cursor.element().start() + IString("|");
  }
  ITRACE_DEVELOP(stack);
}
#endif


/***************************************************************************
 * GIN HANDLERS
 ***************************************************************************/

void HtmlGenerator::handleRow( const RowGin          & row )
{
  switch ( _next->mode )
  {
  case table:
    if (_next->hasItem)
    {
      sendControlTag(tagTableCol, tagTableCol, true);
    }
    else
    {
      _next->hasItem = true;
    }
    break;

  case doubleList:
    if (_next->hasItem)
    {
      sendControlTag(tagListDef, tagListTerm);
    }
    else
    {
      _next->hasItem = true;
    }
    break;
  }
}


void HtmlGenerator::handleColumn( const ColumnGin       & column )
{
  switch ( _next->mode )
  {
  case table:
    sendControlTag(tagTableCol, tagTableCol);
    break;

  case doubleList:
    sendControlTag(tagListTerm, tagListDef);
    break;
  }
}


void HtmlGenerator::handleItem( const ItemGin         & item )
{
  if (_next->mode == singleList)
  {
    if (_next->hasItem)
    {
      sendControlTag(tagListItem, tagListItem);
    }
    else
    {
      _next->hasItem = true;
    }
  }
}


void HtmlGenerator::handlePara( const ParaGin         & para )
{
  int numberOfLines = _next->spacingLines;

  // use LineGin if no space between lines
  if ( ! numberOfLines )
  {
    handleLine( LineGin() );
    return;
  }

  // if in "lines" mode, use outputLine()
  if ( _next->mode == lines )
  {
    prepareToSend();
    while ( numberOfLines-- )
      outputLine();
    return;
  }

  // use <br> for additional line spacing
  while ( --numberOfLines )
    sendTag("br");

  // make sure tags are output and stack is up to date
  prepareToSend();

  // use <p> for last line (need to undo/redo around it)
  undoTags();
  sendTag("p", breakBefore);
  redoTags();
}


void HtmlGenerator::handleLine( const LineGin         & line )
{
  if ( _next->mode == lines )
  {
    // use outputLine in lines (literal) mode
    prepareToSend();
    outputLine();
  }
  else
  {
    sendTag("br", breakBefore);
  }
}


