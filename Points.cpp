//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <mem.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "PointGridSettings.h"
#include "Points.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPointsForm *PointsForm;

static char* pHeaders[] = {"¹", "Time", "Latitude", "Longitude", "Altitude (m)",
                           "Speed (km/h)", "Heading (°)", "Interval (sec)", "Elapsed",
                           "Spacing (m)", "Distance (km)", "Climb (°)", "HDOP", "VDOP", "PDOP",
                           "Satellites", "Fix"};
static int FormatWidth[] = {5, 23, 17, 17, 12, 12, 11, 14, 13, 11, 13, 9, 6, 6, 6, 4, 4};

//---------------------------------------------------------------------------
__fastcall TPointsForm::TPointsForm(TComponent* Owner)
  : TForm(Owner)
{
  m_bFirstShow = true;
  m_pChecked = NULL;
  m_nChecked = 0;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_bLocalTime = pini->ReadBool("Points", "LocalTime", true);
  m_bDMSCoord = pini->ReadBool("Points", "DMSCoord", true);
  delete pini;
}

__fastcall TPointsForm::~TPointsForm()
{
  if(m_pChecked) delete [] m_pChecked;
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::SetCurrentTrack(PGPXTrack value)
{
  m_pCurrentTrack = value;
  PointsList->Items->Count = 0;

  if(m_pChecked)
  {
    delete [] m_pChecked;
    m_pChecked = NULL;
    m_nChecked = 0;
  }

  if(m_bLocalTime)
    PointsList->Column[1]->Caption = AnsiString("Time (Local)");
  else
    PointsList->Column[1]->Caption = AnsiString("Time (UTC)");

  if(m_pCurrentTrack)
  {
    m_pChecked = new bool[m_pCurrentTrack->PointCount];
    memset(m_pChecked, 0, m_pCurrentTrack->PointCount * sizeof(bool));
    
    PointsList->Items->Count = m_pCurrentTrack->PointCount;
    PointsList->Items->Item[0]->Selected = true;
  }
}

PGPXTrack __fastcall TPointsForm::GetCurrentTrack()
{
  return m_pCurrentTrack;
}

void __fastcall TPointsForm::FormShow(TObject *Sender)
{
  if(m_bFirstShow)
  {
    if(m_bLocalTime)
      PointsList->Column[1]->Caption = AnsiString("Time (Local)");
    else
      PointsList->Column[1]->Caption = AnsiString("Time (UTC)");

    m_bFirstShow = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::SettingsClick(TObject *Sender)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_bLocalTime = pini->ReadBool("Points", "LocalTime", true);
  m_bDMSCoord = pini->ReadBool("Points", "DMSCoord", true);
  delete pini;

  GridSettings->TimeFormat->ItemIndex = (m_bLocalTime) ? 1 : 0;
  GridSettings->CoordFormat->ItemIndex = (m_bDMSCoord) ? 0 : 1;

  if(GridSettings->ShowModal() == mrOk)
  {
    m_bLocalTime = (GridSettings->TimeFormat->ItemIndex == 1);
    m_bDMSCoord = (GridSettings->CoordFormat->ItemIndex == 0);

    pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteBool("Points", "LocalTime", m_bLocalTime);
    pini->WriteBool("Points", "DMSCoord", m_bDMSCoord);
    delete pini;

    if(m_bLocalTime)
      PointsList->Column[1]->Caption = AnsiString("Time (Local)");
    else
      PointsList->Column[1]->Caption = AnsiString("Time (UTC)");

    PointsList->UpdateItems(0, m_pCurrentTrack->PointCount - 1);
  }
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::SaveTextClick(TObject *Sender)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  AnsiString asPath = pini->ReadString("Path", "TextPath", "");
  delete pini;

  SaveDialog->InitialDir = asPath;

  if(SaveDialog->Execute())
  {
    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteString("Path", "TextPath", ExtractFileDir(SaveDialog->FileName));
    delete pini;

    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

	FILE *pOut = fopen(AnsiString(SaveDialog->FileName).c_str(), "w+t");
	if(pOut)
    {
      int nCols = sizeof(FormatWidth) / sizeof(int);
      
      for(int i = 0; i < nCols; i++)
      {
        if(i) fprintf(pOut, "  ");
        fprintf(pOut, "%*s", FormatWidth[i], pHeaders[i]);
      }

      fprintf(pOut, "\n");

      for(int i = 0; i < PointsList->Items->Count; i++)
      {
        TListItem *pItem = PointsList->Items->Item[i];
        fprintf(pOut, "%*s", FormatWidth[0], pItem->Caption.c_str());

        for(int j = 0; j < pItem->SubItems->Count; j++)
          fprintf(pOut, " %*s", FormatWidth[j + 1], pItem->SubItems->Strings[j].c_str());

        fprintf(pOut, "\n");
      }
       
      fclose(pOut);
    }

    Screen->Cursor = Save_Cursor;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::PointsListSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
  int nPoint = (Selected) ? Item->Caption.ToInt() - 1 : -1;
  SendMessage(Application->MainForm->Handle, SHOW_POINT, (WPARAM)Selected, (LPARAM)nPoint);
  Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::DelCheckedActionExecute(TObject *Sender)
{
  TCursor Save_Cursor = Screen->Cursor;
  Screen->Cursor = crHourGlass;

  PGPXTrack pNewTrack = new GPXTrack(m_pCurrentTrack);

  for(int i = 0; i < pNewTrack->PointCount; i++)
  {
    if(m_pChecked[i]) pNewTrack->Points[i]->m_bActive = false;
  }

  pNewTrack->RemoveInactivePoints();
  pNewTrack->RecalcDistClimb();

  SendMessage(Application->MainForm->Handle, MANUAL_TRACK_CHANGE, 0, (LPARAM)pNewTrack);
  Application->ProcessMessages();

  Screen->Cursor = Save_Cursor;
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::DelCheckedActionUpdate(TObject *Sender)
{
  DelCheckedAction->Enabled = (m_nChecked > 0);  
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::UncheckAllActionExecute(TObject *Sender)
{
  int nUpdFirst = -1, nUpdLast = -1;

  for(int i = 0; i < PointsList->Items->Count && m_nChecked > 0; i++)
  {
    if(m_pChecked[i])
    {
      if(nUpdFirst == -1) nUpdFirst = i;
      nUpdLast = i;
      
      m_pChecked[i] = false;
      m_nChecked--;
    }
  }

  if(nUpdFirst > -1 && nUpdLast > -1) PointsList->UpdateItems(nUpdFirst, nUpdLast);
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::UncheckAllActionUpdate(TObject *Sender)
{
  UncheckAllAction->Enabled = (m_nChecked > 0);
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  SendMessage(Application->MainForm->Handle, SHOW_POINT, (WPARAM)false, (LPARAM)-1);
  Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::SetMouseWnd(eMouseWnd value)
{
  if(m_eMouseWnd != value) {
    m_eMouseWnd = value;
  }
}

eMouseWnd __fastcall TPointsForm::GetMouseWnd()
{
  return m_eMouseWnd;
}

void __fastcall TPointsForm::PointsListMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  m_nX = X; m_nY = Y;
  
  if(MouseWnd != mwPoints)
  {
    MouseWnd = mwPoints;

    TListItem *pItem = PointsList->Selected;
    int nPoint = -1;
    bool Selected = false;

    if(pItem)
    {
      nPoint = pItem->Caption.ToInt() - 1;
      Selected = true;
    }

    SendMessage(Application->MainForm->Handle, SHOW_POINT, (WPARAM)Selected, (LPARAM)nPoint);
    Application->ProcessMessages();
  }
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::SetSelectedPoint(int value)
{
  if(m_nSelectedPoint != value)
  {
    m_nSelectedPoint = value;

    if(m_nSelectedPoint > -1 && m_pCurrentTrack)
    {
      if(m_nSelectedPoint < m_pCurrentTrack->PointCount)
      {
        PointsList->Items->Item[m_nSelectedPoint]->MakeVisible(false);
        PointsList->Items->Item[m_nSelectedPoint]->Selected = true;
      }
      else m_nSelectedPoint = -1;
    }
  }
}

int __fastcall TPointsForm::GetSelectedPoint()
{
  return m_nSelectedPoint;
}

void __fastcall TPointsForm::CheckToBeginExecute(TObject *Sender)
{
  TListItem *pSelected = PointsList->Selected;

  if(pSelected)
  {
    int nUpdEnd = pSelected->Index;

    for(int i = nUpdEnd; i >= 0; i--)
    {
      m_pChecked[i] = true;
      m_nChecked++;
    }

    PointsList->UpdateItems(0, nUpdEnd);
  }
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::CheckToBeginUpdate(TObject *Sender)
{
  bool bEnabled = false;

  TListItem *pSelected = PointsList->Selected;

  if(pSelected)
  {
    int nPoint = pSelected->Caption.ToInt() - 1;
    if(nPoint > 0) bEnabled = true;
  }

  CheckToBegin->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::CheckToEndExecute(TObject *Sender)
{
  TListItem *pSelected = PointsList->Selected;

  if(pSelected)
  {
    int nUpdBegin = pSelected->Index, nUpdEnd = PointsList->Items->Count - 1;

    for(int i = nUpdBegin; i <= nUpdEnd; i++)
    {
      m_pChecked[i] = true;
      m_nChecked++;
    }

    PointsList->UpdateItems(nUpdBegin, nUpdEnd);
  }
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::CheckToEndUpdate(TObject *Sender)
{
  bool bEnabled = false;

  TListItem *pSelected = PointsList->Selected;

  if(pSelected)
  {
    int nPoint = pSelected->Caption.ToInt() - 1;
    if(nPoint < PointsList->Items->Count - 1) bEnabled = true;
  }

  CheckToEnd->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::UncheckToBeginExecute(TObject *Sender)
{
  TListItem *pSelected = PointsList->Selected;

  if(pSelected)
  {
    int nUpdEnd = pSelected->Index;

    for(int i = nUpdEnd; i >= 0; i--)
    {
      m_pChecked[i] = false;
      m_nChecked--;
    }

    PointsList->UpdateItems(0, nUpdEnd);
  }
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::UncheckToBeginUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(m_nChecked > 0)
  {
    TListItem *pSelected = PointsList->Selected;

    if(pSelected)
    {
      int nPoint = pSelected->Caption.ToInt() - 1;
      if(nPoint > 0) bEnabled = true;
    }
  }

  UncheckToBegin->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::UncheckToEndExecute(TObject *Sender)
{
  TListItem *pSelected = PointsList->Selected;

  if(pSelected)
  {
    int nUpdBegin = pSelected->Index, nUpdEnd = PointsList->Items->Count - 1;

    for(int i = nUpdBegin; i <= nUpdEnd; i++)
    {
      m_pChecked[i] = false;
      m_nChecked--;
    }

    PointsList->UpdateItems(nUpdBegin, nUpdEnd);
  }
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::UncheckToEndUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(m_nChecked > 0)
  {
    TListItem *pSelected = PointsList->Selected;

    if(pSelected)
    {
      int nPoint = pSelected->Caption.ToInt() - 1;
      if(nPoint < PointsList->Items->Count - 1) bEnabled = true;
    }
  }

  UncheckToEnd->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::PointsListData(TObject *Sender,
      TListItem *pItem)
{
  PGPXTrackPoint p = m_pCurrentTrack->Points[pItem->Index];

  pItem->ImageIndex = m_pChecked[pItem->Index] ? 1 : 0;
  pItem->Caption = AnsiString(pItem->Index + 1);

  if(m_bLocalTime)
    pItem->SubItems->Add(p->LocalTimeString);
  else
    pItem->SubItems->Add(p->TimeString);

  if(m_bDMSCoord)
  {
    pItem->SubItems->Add(p->LatDMSString);
    pItem->SubItems->Add(p->LonDMSString);
  }
  else
  {
    pItem->SubItems->Add(p->LatD_DString);
    pItem->SubItems->Add(p->LonD_DString);
  }

  pItem->SubItems->Add(p->GPXEleString);
  pItem->SubItems->Add(p->GPXSpeedString);
  pItem->SubItems->Add(p->GPXCourseString);
  pItem->SubItems->Add(p->IntervalString);
  pItem->SubItems->Add(p->ElapsedString);
  pItem->SubItems->Add(p->SpacingString);
  pItem->SubItems->Add(p->DistanceString);
  pItem->SubItems->Add(p->GPXClimbString);
  pItem->SubItems->Add(p->GPXHDOPString);
  pItem->SubItems->Add(p->GPXVDOPString);
  pItem->SubItems->Add(p->GPXPDOPString);
  pItem->SubItems->Add(p->GPXSatNumString);
  pItem->SubItems->Add(p->GPXFixString);
}
//---------------------------------------------------------------------------

void __fastcall TPointsForm::PointsListClick(TObject *Sender)
{
  TListView *pListView = dynamic_cast<TListView*>(Sender);

  if(m_nX < 16)
  {
    TListItem *pItem = pListView->GetItemAt(m_nX, m_nY);

    if(pItem)
    {
      m_pChecked[pItem->Index] = !m_pChecked[pItem->Index];

      if(m_pChecked[pItem->Index]) m_nChecked++;
      else m_nChecked--;
      
      pItem->ImageIndex = m_pChecked[pItem->Index] ? 1 : 0;
      pItem->Update();
    }
  }
}
//---------------------------------------------------------------------------

