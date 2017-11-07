//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Cursors.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "DragCursor.res"

static const int crDragCursor = 1;

__fastcall TCursorSet::TCursorSet()
{
  Screen->Cursors[crDragCursor] = LoadCursor(HInstance, TEXT("DRAGCURSOR"));
  m_bWait = false;
  m_bDrag = false;
  m_bFinger = false;
}

__fastcall TCursorSet::~TCursorSet()
{
  //TODO: Add your source code here
}

void __fastcall TCursorSet::SetWait(bool value)
{
  if(value != m_bWait)
  {
    if(value)
    {
      m_cWaitSavedCursor = Screen->Cursor;
      Screen->Cursor = crHourGlass;
    }
    else Screen->Cursor = m_cWaitSavedCursor;

    m_bWait = value;
  }
}

void __fastcall TCursorSet::SetDrag(bool value)
{
  if(value != m_bDrag)
  {
    if(value)
    {
      m_cDragSavedCursor = Screen->Cursor;
      Screen->Cursor = (TCursor)crDragCursor;
    }
    else Screen->Cursor = m_cDragSavedCursor;

    m_bDrag = value;
  }
}

void __fastcall TCursorSet::SetFinger(bool value)
{
  if(value != m_bFinger)
  {
    if(value)
    {
      m_cFingerSavedCursor = Screen->Cursor;
      Screen->Cursor = crHandPoint;
    }
    else Screen->Cursor = m_cFingerSavedCursor;

    m_bFinger = value;
  }
}

TCursor __fastcall TCursorSet::GetWaitCursor()
{
  return crHourGlass;
}

TCursor __fastcall TCursorSet::GetDragCursor()
{
  return (TCursor)crDragCursor;
}

TCursor __fastcall TCursorSet::GetFingerCursor()
{
  return crHandPoint;
}
