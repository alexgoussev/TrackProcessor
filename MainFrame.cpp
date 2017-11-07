//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "CSVFile.h"
#include "GPXData.h"
#include "GPSKalmanForm.h"
#include "MedianFilter.h"
#include "DPFilter.h"
#include "SpdFilter.h"
#include "LowPass.h"
#include "DriftFilter.h"
#include "HDOPFilter.h"
#include "ShifTime.h"
#include "Profiles.h"
#include "WAvgFilter.h"
#include "Spline.h"
#include "Points.h"
#include "StaticNav.h"
#include "Statistics.h"
#include "SRTMAlt.h"
#include "DES.h"
#include "Split.h"
#include "TrackStat.h"
#include "Metadata.h"
#include "NMEAParser.h"
#include "PVTFile.h"
#include "NMEAFile.h"
#include "SplineSmooth.h"
#include "Cursors.h"
#include "HP.h"
#include "Confidence.h"
#include "CellEq.h"
#include "Settings.h"
#include "MainFrame.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

__fastcall TreeItem::TreeItem(TTreeNode *pNode)
{
  pChilds = new TList;

  asText = pNode->Text;
  pTrack = (PVisibleTrack)pNode->Data;
  nState = pNode->StateIndex;

  TTreeNode *pChild = pNode->getFirstChild();

  while(pChild)
  {
    TreeItem *pChildItem = new TreeItem(pChild);
    pChilds->Add(pChildItem);
    pChild = pNode->GetNextChild(pChild);
  }
}

__fastcall TreeItem::~TreeItem()
{
  while(pChilds->Count)
  {
    TreeItem *pItem = (TreeItem*)pChilds->Items[0];
    pChilds->Delete(0);
    delete pItem;
  }

  delete pChilds;
}

void __fastcall TreeItem::ConnectChildsTo(TTreeView *pTree, TTreeNode *pNode)
{
  for(int i = 0; i < pChilds->Count; i++)
  {
    TreeItem *pItem = (TreeItem*)pChilds->Items[i];
    TTreeNode *pNewNode = pTree->Items->AddChildObject(pNode, pItem->asText, pItem->pTrack);
    pNewNode->StateIndex = pItem->nState;
    pNewNode->ImageIndex = pItem->pTrack->ColorNo;
    pNewNode->SelectedIndex = pItem->pTrack->ColorNo;
    pItem->ConnectChildsTo(pTree, pNewNode);
  }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent* Owner)
  : TForm(Owner)
{
  m_bFirstShow = true;
  m_bInternalSelect = false;
  m_bNoDrag = false;
  m_nSelectedItems = 0;
  m_nCheckedItems = 0;
  m_pLastSelected = NULL;
  m_pTrackView = NULL;
  m_bDblClicked = false;
  m_bMousePressed = false;
  m_nCurrentColor = 0;
  m_pCursors = new TCursorSet;
  m_pTrackColors = new tagTrackColor[TrackColors->Count];

  Graphics::TBitmap *pBitmap = new Graphics::TBitmap;

  for(int i = 0; i < TrackColors->Count; i++)
  {
    TrackColors->GetBitmap(i, pBitmap);
    m_pTrackColors[i].Color = pBitmap->Canvas->Pixels[8][8];
    m_pTrackColors[i].nCount = 0;
  }

  delete pBitmap;
}

__fastcall TMainForm::~TMainForm()
{
  delete m_pCursors;
  delete [] m_pTrackColors;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenActionExecute(TObject *Sender)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  AnsiString asPath = pini->ReadString("Path", "OpenFilePath", "");
  int nFileType = pini->ReadInteger("Path", "OpenFileType", 1);
  delete pini;

  OpenDialog->InitialDir = asPath;
  OpenDialog->FilterIndex = nFileType;

  if(OpenDialog->Execute())
  {
    m_bNoDrag = true;
    m_pCursors->Wait = true;

    nFileType = OpenDialog->FilterIndex;

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteString("Path", "OpenFilePath", ExtractFileDir(OpenDialog->FileName));
    pini->WriteInteger("Path", "OpenFileType", nFileType);
    delete pini;

    LoadFile(OpenDialog->FileName, nFileType, (TracksTree->Items->Count == 0));
  }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ExitActionExecute(TObject *Sender)
{
//
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveCheckedActionExecute(TObject *Sender)
{
  GPXFile *pGPX = new GPXFile;
  AnsiString asFileName = AnsiString("");

  m_bNoDrag = true;

  for(int i = 0; i < TracksTree->Items->Count; i++)
  {
    TTreeNode *pNode = TracksTree->Items->Item[i];

    if(pNode->StateIndex > 1)
    {
      PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
      pGPX->AddTrack(pTrack->pTrack);
      if(asFileName.IsEmpty()) asFileName = pTrack->pTrack->m_asFileName;
    }
  }

  if(pGPX->TrackCount)
  {
    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    AnsiString asPath = pini->ReadString("Path", "SaveGPXPath", "");
    delete pini;

    SaveDialog->InitialDir = asPath;
    SaveDialog->FileName = ChangeFileExt(asFileName, ".gpx");

    if(SaveDialog->Execute())
    {
      m_pCursors->Wait = true;
      TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
      pini->WriteString("Path", "SaveGPXPath", ExtractFileDir(SaveDialog->FileName));
      delete pini;

      pGPX->Save(SaveDialog->FileName);
      StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
      StatusBar->Panels->Items[1]->Text = AnsiString("");
      m_pCursors->Wait = false;
    }
  }

  m_bNoDrag = false;

  delete pGPX;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::KalmanActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;

  GPSKalmanFilter->PCurrentTrack = pTrack->pTrack;

  if(GPSKalmanFilter->ShowModal() == mrOk)
    AddProcessedTrack(pNode, GPSKalmanFilter->PCurrentTrack, AnsiString(" - Kalman"));
}
//---------------------------------------------------------------------------


void TMainForm::OnNewTrack(TMessage & msg)
{
  AnsiString astr = AnsiString("Loading track: ") + AnsiString((const char*)msg.LParam);
  StatusBar->Panels->Items[0]->Text = astr;
}

void TMainForm::OnNewPoint(TMessage & msg)
{
  AnsiString astr = AnsiString("Loading point: ") + AnsiString((int)msg.LParam);
  StatusBar->Panels->Items[1]->Text = astr;
}

void TMainForm::OnSaveTrack(TMessage & msg)
{
  AnsiString astr = AnsiString("Saving track: ") + AnsiString((const char*)msg.LParam);
  StatusBar->Panels->Items[0]->Text = astr;
}

void TMainForm::OnSavePoint(TMessage & msg)
{
  AnsiString astr = AnsiString("Saving point: ") + AnsiString((int)msg.LParam);
  StatusBar->Panels->Items[1]->Text = astr;
}

void __fastcall TMainForm::MedianActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  MedianForm->m_pCurrentTrack = pNewTrack;

  if(MedianForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - Median"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::DPActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  DPForm->m_pCurrentTrack = pNewTrack;

  if(DPForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - Douglas-Peucker"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HDOPActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  HDOPForm->m_pCurrentTrack = pNewTrack;

  if(HDOPForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - HDOP"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HDOPActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack->pTrack && !pTrack->pTrack->m_bNoHDOP)
      bEnabled = true;
  }

  HDOPAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::VelocityActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  SpeedFilter->m_pCurrentTrack = pNewTrack;

  if(SpeedFilter->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - Speed"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SRTMActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  STRMAltitude->m_pTrack = pNewTrack;

  TModalResult mResult = STRMAltitude->ShowModal();

  if(mResult == mrOk || mResult == mrRetry)
  {
    if(mResult == mrRetry)
      MessageBox(Handle, TEXT("Some HGT files missing. Operation incomplete."), TEXT("Warning"), MB_OK | MB_ICONHAND);

    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - SRTM Altitude"));
  }
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DriftActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  DriftForm->m_pCurrentTrack = pNewTrack;

  if(DriftForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - Drift"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LowPassActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  LowPassForm->m_pCurrentTrack = pNewTrack;

  if(LowPassForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - LowPass"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::KalmanActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && TracksTree->Selected->Data && !m_bNoDrag)
    KalmanAction->Enabled = true;
  else
    KalmanAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MedianActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && TracksTree->Selected->Data && !m_bNoDrag)
    MedianAction->Enabled = true;
  else
    MedianAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DPActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && TracksTree->Selected->Data && !m_bNoDrag)
    DPAction->Enabled = true;
  else
    DPAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::VelocityActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack->pTrack && !pTrack->pTrack->m_bNoTime)
      bEnabled = true;
  }

  VelocityAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DriftActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack->pTrack && !pTrack->pTrack->m_bNoTime)
      bEnabled = true;
  }

  DriftAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SRTMActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && TracksTree->Selected->Data && !m_bNoDrag)
    SRTMAction->Enabled = true;
  else
    SRTMAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LowPassActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && TracksTree->Selected->Data && !m_bNoDrag)
    LowPassAction->Enabled = true;
  else
    LowPassAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TimeShiftActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  TimeShift->m_pCurrentTrack = pNewTrack;

  if(TimeShift->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - Time Shift"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TimeShiftActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack->pTrack && !pTrack->pTrack->m_bNoTime)
      bEnabled = true;
  }

  TimeShiftAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StatisticsActionExecute(TObject *Sender)
{
  if(!TrackStatForm->Visible)
  {
    TrackStatForm->CurrentTrack = ((PVisibleTrack)TracksTree->Selected->Data)->pTrack;
    TrackStatForm->Show();
  }
  else
  {
    TrackStatForm->Close();
    TrackStatForm->CurrentTrack = NULL;
  }  
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StatisticsActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && TracksTree->Selected->Data && !m_bNoDrag)
    StatisticsAction->Enabled = true;
  else
    StatisticsAction->Enabled = false;

  if(TrackStatForm->Visible)
    StatisticsAction->Checked = true;
  else
    StatisticsAction->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ProfilesActionExecute(TObject *Sender)
{
  if(!Profile->Visible)
  {
    Profile->CurrentTrack = ((PVisibleTrack)TracksTree->Selected->Data)->pTrack;
    Profile->Show();
  }
  else
  {
    Profile->Close();
    Profile->CurrentTrack = NULL;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ProfilesActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack->pTrack && (!pTrack->pTrack->m_bNoTime || !pTrack->pTrack->m_bNoAlt))
      bEnabled = true;
  }

  ProfilesAction->Enabled = bEnabled;

  if(Profile->Visible)
    ProfilesAction->Checked = true;
  else
    ProfilesAction->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PointsActionExecute(TObject *Sender)
{
  if(!PointsForm->Visible)
  {
    PointsForm->Show();
    PointsForm->CurrentTrack = ((PVisibleTrack)TracksTree->Selected->Data)->pTrack;
  }
  else
  {
    PointsForm->Close();
    PointsForm->CurrentTrack = NULL;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PointsActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && TracksTree->Selected->Data && !m_bNoDrag)
    PointsAction->Enabled = true;
  else
    PointsAction->Enabled = false;

  if(PointsForm->Visible)
    PointsAction->Checked = true;
  else
    PointsAction->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SplitActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  SplitForm->m_pCurrentTrack = pTrack->pTrack;

  if(SplitForm->ShowModal() == mrOk)
  {
    AnsiString asName = pNode->Text;
    asName += " - Split ";
    int n = 1;

    while(SplitForm->m_pResult->Count)
    {
      PGPXTrack pResult = (PGPXTrack)SplitForm->m_pResult->Items[0];
      SplitForm->m_pResult->Delete(0);

      PVisibleTrack pData = new VisibleTrack;
      pData->pTrack = pResult;
      pData->ColorNo = SelectTrackColor();

      AnsiString asNumber = asName + AnsiString(n++);
      TTreeNode *pNewNode = TracksTree->Items->AddChildObject(pNode, asNumber, pData);
      SetNodeState(pNewNode, 2);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SplitActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack && !pTrack->pTrack->m_bNoTime)
      bEnabled = true;
  }

  SplitAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MergeActionExecute(TObject *Sender)
{
  m_pCursors->Wait = true;

  TList *pTracks = new TList;

  for(int i = 0; i < TracksTree->Items->Count; i++)
  {
    TTreeNode *pNode = TracksTree->Items->Item[i];

    if(pNode->StateIndex > 1)
    {
      PVisibleTrack pData = (PVisibleTrack)pNode->Data;
      PGPXTrack pTrack = pData->pTrack;
      pTracks->Add(pTrack);
    }
  }

  StatusBar->Panels->Items[0]->Text = AnsiString("Merging Tracks");
  Application->ProcessMessages();

  int *N = new int[pTracks->Count];
  int nPoint = 0;

  for(int i = 0; i < pTracks->Count; i++) N[i] = 0;

  PGPXTrack pNewTrack = new GPXTrack;
  pNewTrack->m_asName = AnsiString("Merged track");

  for(;;)
  {
    double tmin = DBL_MAX;
    int imin = -1;

    for(int i = 0; i < pTracks->Count; i++)
    {
      PGPXTrack pTrack = (PGPXTrack)pTracks->Items[i];

      if(N[i] < pTrack->PointCount)
      {
        if(pTrack->Points[N[i]]->m_dTime < tmin)
        {
          tmin = pTrack->Points[N[i]]->m_dTime;
          imin = i;
        }
      }
    }

    if(imin > -1)
    {
      PGPXTrackPoint pNewPoint = pNewTrack->NewPoint;
      PGPXTrack pTrack = (PGPXTrack)pTracks->Items[imin];
      PGPXTrackPoint pPoint = pTrack->Points[N[imin]];
      pNewPoint->SetPoint(pPoint);
      pNewTrack->AddPoint(pNewPoint);
      N[imin]++;
    }
    else break;

    AnsiString astr = AnsiString("Merging point: ") + AnsiString(++nPoint);
    StatusBar->Panels->Items[1]->Text = astr;
    Application->ProcessMessages();
  }

  StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
  StatusBar->Panels->Items[1]->Text = AnsiString("");
  Application->ProcessMessages();

  delete [] N;
  delete pTracks;

  PVisibleTrack pData = new VisibleTrack;
  pData->pTrack = pNewTrack;
  pData->ColorNo = SelectTrackColor();

  TTreeNode *pNode = TracksTree->Items->AddObject(NULL, AnsiString("Merged track"), pData);
  pNode->ImageIndex = pData->ColorNo;
  pNode->SelectedIndex = pData->ColorNo;
  m_pTrackColors[pData->ColorNo].nCount++;
  SetNodeState(pNode, 2);

  m_pCursors->Wait = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MergeActionUpdate(TObject *Sender)
{
  if(m_nCheckedItems > 1 && !m_bNoDrag)
    MergeAction->Enabled = true;
  else
    MergeAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WAvgActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  WAvgForm->m_pCurrentTrack = pNewTrack;

  if(WAvgForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - Average Smooth"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WAvgActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack->pTrack && !pTrack->pTrack->m_bNoTime)
      bEnabled = true;
  }

  WAvgAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OnManualTrackChange(TMessage & msg)
{
  TTreeNode *pNode = NULL;
  PGPXTrack pNewTrack = (PGPXTrack)msg.LParam;

  if(pNewTrack)
  {
    for(int i = 0; i < TracksTree->Items->Count; i++)
    {
      pNode = TracksTree->Items->Item[i];
      PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;

      if(pTrack->pTrack == PointsForm->CurrentTrack) break;
    }

    if(pNode)
      AddProcessedTrack(pNode, pNewTrack, AnsiString(" - Manual Edit"));
    else delete pNewTrack;
  }
}

void __fastcall TMainForm::StaticActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  SNEForm->CurrentTrack = pNewTrack;

  if(SNEForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - Position Pinning"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StaticActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack->pTrack && !pTrack->pTrack->m_bNoTime)
      bEnabled = true;
  }

  StaticAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormShow(TObject *Sender)
{
  if(m_bFirstShow)
  {
    m_bFirstShow = false;

    TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    int nMapType = ini->ReadInteger("Map", "MapType", 0);

    if(nMapType < 0 || nMapType > 4)
    {
      nMapType = 0;
      ini->WriteInteger("Map", "MapType", 0);
    }

    NoMapAction->Checked = ini->ReadBool("Map", "NoMap", false);
    MaptypeItem->Enabled = !NoMapAction->Checked;
    MapTypePopupItem->Enabled = MaptypeItem->Enabled;
    delete ini;


    m_eMapType = (eMapType)nMapType;

    TMenuItem *pMapTypes[5] = {
      LandscapeCycleMapItem,
      OpenCycleMapItem,
      OpenStreetMapMapItem,
      OutdoorsCycleMapItem,
      OpenTopoMapItem
    };

    TMenuItem *pMapTypes1[5] = {
      LandscapeCycleMap1,
      OpenCycleMap1,
      OpenStreetMap1,
      OutdoorsCycleMap1,
      OpenTopoMapItem1
    };

    pMapTypes[nMapType]->Checked = true;
    pMapTypes1[nMapType]->Checked = true;

    if(NoMapItem->Checked)
      MaptypeItem->Enabled = false;
    else
      MaptypeItem->Enabled = true;

    m_asCmdLineFile = ParamStr(1);

    if(!m_asCmdLineFile.IsEmpty())
    {
      AnsiString strExt = ExtractFileExt(m_asCmdLineFile).UpperCase();
      int nFileType = -1;

      if(strExt == ".GPX") nFileType = 1;
      else if(strExt == ".CSV") nFileType = 2;
      else if(strExt == ".NMEA") nFileType = 3;
      else if(strExt == ".PVT") nFileType = 4;

      if(nFileType > 0 && nFileType < 5)
        PostMessage(Handle, TPM_LOADFILE, (WPARAM)nFileType, (LPARAM)0);
    }
    else
    {
      double dMinLat, dMinLon, dMaxLat,  dMaxLon;
      int nZoom;

      if(LoadLastCoordinates(dMinLat, dMinLon, dMaxLat, dMaxLon, nZoom))
      {
        m_pCursors->Wait = true;
        m_pTrackView = new TOSMTrackView(dMaxLat, dMaxLon, dMinLat, dMinLon, TrackViewBox->Canvas, m_eMapType, nZoom, NoMapItem->Checked);
        m_pCursors->Wait = false;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  if(PointsForm->Visible) PointsForm->Close();
  if(Profile->Visible) Profile->Close();
  if(TrackStatForm->Visible) TrackStatForm->Close();

  for(int i = 0; i < TracksTree->Items->Count; i++)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Items->Item[i]->Data;
    delete pTrack->pTrack;
    delete pTrack;
  }

  if(m_pTrackView) delete m_pTrackView;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::TracksTreeClick(TObject *Sender)
{
  TPoint p;
  ::GetCursorPos(&p);
  ::ScreenToClient(TracksTree->Handle, &p);

  if(TracksTree->GetHitTestInfoAt(p.x, p.y).Contains(htOnStateIcon))
  {
    TTreeNode *pNode = TracksTree->GetNodeAt(p.x, p.y);
    if(pNode->StateIndex == 1)
      SetNodeState(pNode, 3);
    else if(pNode->StateIndex > 1)
      SetNodeState(pNode, 1);
  }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::TracksTreeChanging(TObject *Sender,
      TTreeNode *Node, bool &AllowChange)
{
  if(m_bInternalSelect)
  {
    AllowChange = true;
    m_bInternalSelect = false;
  }
  else
  {
    TPoint p;
    ::GetCursorPos(&p);
    ::ScreenToClient(TracksTree->Handle, &p);

    if(TracksTree->GetHitTestInfoAt(p.x, p.y).Contains(htOnLabel))
      AllowChange = true;
    else
      AllowChange = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TracksTreeChange(TObject *Sender,
      TTreeNode *Node)
{
  m_pLastSelected = Node;

  if(Node->StateIndex == 1)
    SetNodeState(Node, 2);

  PVisibleTrack pTrack = (PVisibleTrack)Node->Data;

  if(m_pTrackView)
  {
    m_pCursors->Wait = true;
    m_pTrackView->TopmostTrack = pTrack->pTrack;
    m_pCursors->Wait = false;
  }

  if(Profile->Visible)
    Profile->CurrentTrack = pTrack->pTrack;

  if(PointsForm->Visible)
    PointsForm->CurrentTrack = pTrack->pTrack;

  if(TrackStatForm->Visible)
    TrackStatForm->CurrentTrack = pTrack->pTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SaveCheckedActionUpdate(TObject *Sender)
{
  if(m_nCheckedItems && !m_bNoDrag)
  {
    SaveCheckedAction->Enabled = true;
    DeleteCheckedAction->Enabled = true;
  }
  else
  {
    SaveCheckedAction->Enabled = false;
    DeleteCheckedAction->Enabled = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AddProcessedTrack(TTreeNode * pNode, PGPXTrack pNewTrack, AnsiString asAddition)
{
  AnsiString asName = pNode->Text;
  asName += asAddition;
  pNewTrack->m_asName = asName;

  PVisibleTrack pData = new VisibleTrack;
  pData->pTrack = pNewTrack;
  pData->ColorNo = SelectTrackColor();
  TTreeNode *pNewNode = TracksTree->Items->AddChildObject(pNode, asName, pData);
  pNewNode->ImageIndex = pData->ColorNo;
  pNewNode->SelectedIndex = pData->ColorNo;
  m_pTrackColors[pData->ColorNo].nCount++;

  m_bInternalSelect = true;
  TracksTree->Selected = pNewNode;
  SetNodeState(pNewNode, 2);
}

void __fastcall TMainForm::DESActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  PGPXTrack pNewTrack = new GPXTrack(pTrack->pTrack);
  DESForm->m_pCurrentTrack = pNewTrack;

  if(DESForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, pNewTrack, AnsiString(" - DES"));
  else delete pNewTrack;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DESActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;
    if(pTrack && pTrack->pTrack) bEnabled = true;
  }

  DESAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetNodeState(TTreeNode * pNode, int nState)
{
  int nOldState = pNode->StateIndex;
  pNode->StateIndex = nState;

  if(nOldState < 2 && nState > 1) m_nCheckedItems++;
  else if(nOldState > 1 && nState < 2) m_nCheckedItems--;

  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;

  if(nOldState < 2 && nState > 1)
  {
    m_pCursors->Wait = true;

    if(!m_pTrackView)
    {
      m_pTrackView = new TOSMTrackView(pTrack->pTrack->m_dMaxLat, pTrack->pTrack->m_dMaxLon,
                                       pTrack->pTrack->m_dMinLat, pTrack->pTrack->m_dMinLon,
                                       TrackViewBox->Canvas, m_eMapType, -1, NoMapItem->Checked);
      m_pTrackView->Paint(TrackViewBox->ClientWidth, TrackViewBox->ClientHeight);
      SaveLastCoordinates();
    }

    m_pTrackView->Add(pTrack->pTrack, m_pTrackColors[pTrack->ColorNo].Color);

    m_pCursors->Wait = false;
  }
  else if(nOldState > 1 && nState < 2)
  {
    if(m_pTrackView)
    {
      m_pCursors->Wait = true;
      m_pTrackView->Delete(pTrack->pTrack);
      m_pCursors->Wait = false;
    }
  }
}

void __fastcall TMainForm::SaveSelectedActionExecute(TObject *Sender)
{
  GPXFile *pGPX = new GPXFile;
  AnsiString asFileName = AnsiString("");

  TTreeNode *pNode = TracksTree->Selected;

  m_bNoDrag = true;

  if(pNode)
  {
    PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
    pGPX->AddTrack(pTrack->pTrack);
    if(asFileName.IsEmpty()) asFileName = pTrack->pTrack->m_asFileName;
  }

  if(pGPX->TrackCount)
  {
    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    AnsiString asPath = pini->ReadString("Path", "SaveGPXPath", "");
    delete pini;

    SaveDialog->InitialDir = asPath;
    SaveDialog->FileName = ChangeFileExt(asFileName, ".gpx");

    if(SaveDialog->Execute())
    {
      m_pCursors->Wait = true;
      TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
      pini->WriteString("Path", "SaveGPXPath", ExtractFileDir(SaveDialog->FileName));
      delete pini;

      pGPX->Save(SaveDialog->FileName);
      StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
      StatusBar->Panels->Items[1]->Text = AnsiString("");
      m_pCursors->Wait = false;
    }
  }

  m_bNoDrag = false;

  delete pGPX;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SaveSelectedActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && !m_bNoDrag)
  {
    SaveSelectedAction->Enabled = true;
    DeleteSelectedAction->Enabled = true;
    CollapseNodeAction->Enabled = true;
    ExpandNodeAction->Enabled = true;
  }
  else
  {
    SaveSelectedAction->Enabled = false;
    DeleteSelectedAction->Enabled = false;
    CollapseNodeAction->Enabled = false;
    ExpandNodeAction->Enabled = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MapTypeItemClick(TObject *Sender)
{
  TMenuItem *pItem = dynamic_cast<TMenuItem*>(Sender);

  if(pItem)
  {
    int nMapType = pItem->Tag;

    TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    ini->WriteInteger("Map", "MapType", nMapType);
    delete ini;

    TMenuItem *pMapTypes[5] = {
      LandscapeCycleMapItem,
      OpenCycleMapItem,
      OpenStreetMapMapItem,
      OutdoorsCycleMapItem,
      OpenTopoMapItem
    };

    TMenuItem *pMapTypes1[5] = {
      LandscapeCycleMap1,
      OpenCycleMap1,
      OpenStreetMap1,
      OutdoorsCycleMap1,
      OpenTopoMapItem1
    };

    pMapTypes[nMapType]->Checked = true;
    pMapTypes1[nMapType]->Checked = true;
    
    m_eMapType = (eMapType)nMapType;

    if(m_pTrackView)
    {
      m_pCursors->Wait = true;
      m_pTrackView->MapType = m_eMapType;
      m_pTrackView->Refresh();
      m_pCursors->Wait = false;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DeleteSelectedActionExecute(TObject *Sender)
{
  if(TracksTree->Selected->StateIndex > 1) m_nCheckedItems--;

  PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

  m_pCursors->Wait = true;
  m_pTrackView->Delete(pTrack->pTrack);
  m_pCursors->Wait = false;

  DeleteTreeNode(TracksTree->Selected);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DeleteCheckedActionExecute(TObject *Sender)
{
  bool bFound;
  m_nCheckedItems = 0;

  m_pCursors->Wait = true;
  m_pTrackView->ClearTracks();
  m_pTrackView->Refresh();
  m_pCursors->Wait = false;

  do {
    bFound = false;

    for(int i = 0; i < TracksTree->Items->Count; i++)
    {
      TTreeNode *pNode = TracksTree->Items->Item[i];

      if(pNode->StateIndex > 1)
      {
        DeleteTreeNode(pNode);
        bFound = true;
        break;
      }
    }
  } while(bFound);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DeleteTreeNode(TTreeNode * pNode)
{
  TTreeNode *pParent = pNode->Parent;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;

  TreeItem *pItem = new TreeItem(pNode);
  pNode->Delete();
  pItem->ConnectChildsTo(TracksTree, pParent);

  m_pTrackColors[pTrack->ColorNo].nCount--;
  delete pItem;
  delete pTrack->pTrack;
  delete pTrack;

  if(pParent) TracksTree->Selected = pParent;
  else if(TracksTree->Items->Count) TracksTree->Selected = TracksTree->Items->GetFirstNode();
}

void __fastcall TMainForm::CollapseNodeActionExecute(TObject *Sender)
{
  TracksTree->Selected->Collapse(true);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ExpandNodeActionExecute(TObject *Sender)
{
  TracksTree->Selected->Expand(true);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CollapseAllActionExecute(TObject *Sender)
{
  TracksTree->FullCollapse();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CollapseAllActionUpdate(TObject *Sender)
{
  if(TracksTree->Items->Count > 1)
  {
    CollapseAllAction->Enabled = true;
    ExpandAllAction->Enabled = true;
  }
  else
  {
    CollapseAllAction->Enabled = false;
    ExpandAllAction->Enabled = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ExpandAllActionExecute(TObject *Sender)
{
  TracksTree->FullExpand();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::CheckAllActionExecute(TObject *Sender)
{
  m_nCheckedItems = TracksTree->Items->Count;

  for(int i = 0; i < TracksTree->Items->Count; i++)
  {
    TTreeNode *pNode = TracksTree->Items->Item[i];

    if(pNode->StateIndex < 2)
    {
      pNode->StateIndex = 2;
      PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
      m_pCursors->Wait = true;
      m_pTrackView->Add(pTrack->pTrack, m_pTrackColors[pTrack->ColorNo].Color);
      m_pCursors->Wait = false;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckAllActionUpdate(TObject *Sender)
{
  if(TracksTree->Items->Count > m_nCheckedItems)
    CheckAllAction->Enabled = true;
  else
    CheckAllAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::UncheckAllActionExecute(TObject *Sender)
{
  for(int i = 0; i < TracksTree->Items->Count; i++)
  {
    TTreeNode *pNode = TracksTree->Items->Item[i];
    if(pNode->StateIndex > 1)
      SetNodeState(pNode, 1);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::UncheckAllActionUpdate(TObject *Sender)
{
  if(m_nCheckedItems)
    UncheckAllAction->Enabled = true;
  else
    UncheckAllAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MetadataActionExecute(TObject *Sender)
{
  PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;
  EditMetadata->CurrentTrack = pTrack->pTrack;
  EditMetadata->ShowModal();
  TracksTree->Selected->Text = EditMetadata->Name->Text;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MetadataActionUpdate(TObject *Sender)
{
  if(TracksTree->Selected && TracksTree->Selected->Data && !m_bNoDrag)
    MetadataAction->Enabled = true;
  else
    MetadataAction->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::LoadGPXFile(AnsiString asFileName)
{
  GPXFile *pGPX = new GPXFile(asFileName);
  pGPX->Load();

  for(int i = 0; i < pGPX->TrackCount; i++)
  {
    PGPXTrack pTrack = new GPXTrack(pGPX->Tracks[i]);
    AnsiString asText = pTrack->m_asName;

    PVisibleTrack pData = new VisibleTrack;
    pData->pTrack = pTrack;
    pData->ColorNo = SelectTrackColor();

    if(asText.IsEmpty())
      asText = pTrack->m_asFileName + AnsiString(" ¹") + AnsiString(i + 1);

    TTreeNode *pNode = TracksTree->Items->AddObject(NULL, asText, pData);
    pNode->ImageIndex = pData->ColorNo;
    pNode->SelectedIndex = pData->ColorNo;
    m_pTrackColors[pData->ColorNo].nCount++;
    SetNodeState(pNode, 2);
  }

  delete pGPX;
  StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
  StatusBar->Panels->Items[1]->Text = AnsiString("");
}

void __fastcall TMainForm::LoadCSVFile(AnsiString asFileName)
{
  ColumbusCSVFile *pCSV = new ColumbusCSVFile(asFileName);
  pCSV->Load();

  PGPXTrack pTrack = new GPXTrack(pCSV->Track);
  AnsiString asText = pTrack->m_asName;

  PVisibleTrack pData = new VisibleTrack;
  pData->pTrack = pTrack;
  pData->ColorNo = SelectTrackColor();

  if(asText.IsEmpty()) asText = pTrack->m_asFileName;

  TTreeNode *pNode = TracksTree->Items->AddObject(NULL, asText, pData);
  pNode->ImageIndex = pData->ColorNo;
  pNode->SelectedIndex = pData->ColorNo;
  m_pTrackColors[pData->ColorNo].nCount++;
  SetNodeState(pNode, 2);

  delete pCSV;
  StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
  StatusBar->Panels->Items[1]->Text = AnsiString("");
}

void __fastcall TMainForm::LoadNMEAFile(AnsiString asFileName)
{
  TNMEAFile *pNMEAFile = new TNMEAFile(asFileName);
  pNMEAFile->Load();

  if(pNMEAFile->Track)
  {
    PGPXTrack pTrack = new GPXTrack(pNMEAFile->Track);
    AnsiString asText = pTrack->m_asName;

    PVisibleTrack pData = new VisibleTrack;
    pData->pTrack = pTrack;
    pData->ColorNo = SelectTrackColor();

    if(asText.IsEmpty()) asText = pTrack->m_asFileName;

    TTreeNode *pNode = TracksTree->Items->AddObject(NULL, asText, pData);
    pNode->ImageIndex = pData->ColorNo;
    pNode->SelectedIndex = pData->ColorNo;
    m_pTrackColors[pData->ColorNo].nCount++;
    SetNodeState(pNode, 2);
  }

  delete pNMEAFile;
  StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
  StatusBar->Panels->Items[1]->Text = AnsiString("");
}

void __fastcall TMainForm::TrackViewBoxPaint(TObject *Sender)
{
  if(m_pTrackView)
  {
    m_pCursors->Wait = true;
    m_pTrackView->Paint(TrackViewBox->ClientWidth, TrackViewBox->ClientHeight);
    m_pCursors->Wait = false;
    SaveLastCoordinates();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrackViewBoxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(!m_bDblClicked && Button == mbLeft && !m_bNoDrag)
  {
    m_bMousePressed = true;
    if(m_pTrackView)
    {
      m_pTrackView->DragStart(X, Y);
      m_pCursors->Drag = true;
    }
  }

  m_bDblClicked = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrackViewBoxMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  PointsForm->MouseWnd = mwMap;

  if(!m_bNoDrag && m_pTrackView)
  {
    if(m_bMousePressed)
      m_pTrackView->Drag(X, Y);
    else
    {
      int n = m_pTrackView->NearestPoint[X][Y];
      m_pTrackView->CurrentPoint = n;

      if(m_nSelectedPoint == -1 && n > -1) TrackViewBox->Cursor = m_pCursors->FingerCursor; //m_pCursors->Finger = true;
      else if(m_nSelectedPoint > -1 && n == -1) TrackViewBox->Cursor = crDefault; //m_pCursors->Finger = false;

      ShowPointHint(n);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrackViewBoxMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(m_bMousePressed && Button == mbLeft)
  {
    m_bMousePressed = false;
    m_bNoDrag = true;
    
    if(m_pTrackView)
    {
      m_pCursors->Drag = false;
      m_pCursors->Wait = true;
      m_pTrackView->DragEnd(X, Y);
      m_pCursors->Wait = false;
      SaveLastCoordinates();
    }

    m_bNoDrag = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrackViewBoxDblClick(TObject *Sender)
{
  m_bDblClicked = true;

  if(m_nSelectedPoint > -1)
  {
    int nSelectedPoint = m_nSelectedPoint;

    if(!PointsForm->Visible)
    {
      PointsForm->Show();
      PointsForm->CurrentTrack = ((PVisibleTrack)TracksTree->Selected->Data)->pTrack;
    }

    //m_nSelectedPoint = nSelectedPoint;
    PointsForm->SelectedPoint = nSelectedPoint;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
  if(!m_bNoDrag)
  {
    m_bNoDrag = true;

    TPoint UpLeft = TrackViewBox->ClientToScreen(TPoint(0, 0));
    TPoint DnRight = TrackViewBox->ClientToScreen(TPoint(TrackViewBox->ClientWidth, TrackViewBox->ClientHeight));
    TPoint Current = ClientToScreen(MousePos);

    if(Current.x >= UpLeft.x && Current.y >= UpLeft.y &&
       Current.x <= DnRight.x && Current.y <= DnRight.y)
    {
      if(m_pTrackView)
      {
        m_pCursors->Wait = true;
        if(WheelDelta > 0) m_pTrackView->ZoomIn();
        else m_pTrackView->ZoomOut();
        m_pCursors->Wait = false;

        SaveLastCoordinates();
      }
      
      Handled = true;
    }

    m_bNoDrag = false;
  }
}
//---------------------------------------------------------------------------

bool __fastcall TMainForm::LoadLastCoordinates(double & dMinLat, double & dMinLon, double & dMaxLat, double & dMaxLon, int & nZoom)
{
  bool bResult = false;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));

  dMinLat = pini->ReadFloat("Coordinates", "MinLat", GPS_PT_NO_VALUE);
  dMinLon = pini->ReadFloat("Coordinates", "MinLon", GPS_PT_NO_VALUE);
  dMaxLat = pini->ReadFloat("Coordinates", "MaxLat", GPS_PT_NO_VALUE);
  dMaxLon = pini->ReadFloat("Coordinates", "MaxLon", GPS_PT_NO_VALUE);
  nZoom = pini->ReadInteger("Coordinates", "Zoom", -1);

  if(dMinLat >=  -90.0 && dMinLat <=  90.0  &&
     dMaxLat >=  -90.0 && dMaxLat <=  90.0  &&
     dMinLon >= -180.0 && dMinLon <= 180.0  &&
     dMaxLon >= -180.0 && dMaxLon <= 180.0  &&
     dMinLat < dMaxLat && dMinLon < dMaxLon &&
     nZoom >= 0 && nZoom <= 20)
    bResult = true;

  delete pini;

  if(!bResult)
  {
    dMinLat = dMinLon = dMaxLat = dMaxLon = GPS_PT_NO_VALUE;
    nZoom = -1;
  }

  return bResult;
}

void __fastcall TMainForm::SaveLastCoordinates()
{
  double dMinLat, dMinLon, dMaxLat, dMaxLon;
  int nZoom = m_pTrackView->Zoom;

  m_pTrackView->GetLatLong(0, TrackViewBox->ClientHeight, dMinLat, dMinLon);
  m_pTrackView->GetLatLong(TrackViewBox->ClientWidth, 0, dMaxLat, dMaxLon);

  if(dMinLat >=  -90.0 && dMinLat <=  90.0  &&
     dMaxLat >=  -90.0 && dMaxLat <=  90.0  &&
     dMinLon >= -180.0 && dMinLon <= 180.0  &&
     dMaxLon >= -180.0 && dMaxLon <= 180.0  &&
     dMinLat < dMaxLat && dMinLon < dMaxLon &&
     nZoom >= 0 && nZoom <= 20)
  {
    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));

    pini->WriteFloat("Coordinates", "MinLat", dMinLat);
    pini->WriteFloat("Coordinates", "MinLon", dMinLon);
    pini->WriteFloat("Coordinates", "MaxLat", dMaxLat);
    pini->WriteFloat("Coordinates", "MaxLon", dMaxLon);
    pini->WriteInteger("Coordinates", "Zoom", nZoom);

    delete pini;
  }
}

/*
TColor __fastcall TMainForm::GetCurrentColor()
{
  int i = m_nCurrentColor;
  m_nCurrentColor = (m_nCurrentColor + 1) % m_nMaxColor;

  return TrackColor[i];
}
*/

void __fastcall TMainForm::ShowAllActionUpdate(TObject *Sender)
{
  if(m_pTrackView && m_nCheckedItems && m_pTrackView->Count && !m_bNoDrag)
    ShowAllAction->Enabled = true;
  else ShowAllAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ShowAllActionExecute(TObject *Sender)
{
  m_pCursors->Wait = true;
  m_pTrackView->ZoomToFit();
  m_pCursors->Wait = false;
  SaveLastCoordinates();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LoadPVTFile(AnsiString asFileName)
{
  TPVTFile *pPVTFile = new TPVTFile(asFileName);
  pPVTFile->Load();

  if(pPVTFile->Track)
  {
    PGPXTrack pTrack = new GPXTrack(pPVTFile->Track);
    AnsiString asText = pTrack->m_asName;

    PVisibleTrack pData = new VisibleTrack;
    pData->pTrack = pTrack;
    pData->ColorNo = SelectTrackColor();

    if(asText.IsEmpty()) asText = pTrack->m_asFileName;

    TTreeNode *pNode = TracksTree->Items->AddObject(NULL, asText, pData);
    pNode->ImageIndex = pData->ColorNo;
    pNode->SelectedIndex = pData->ColorNo;
    m_pTrackColors[pData->ColorNo].nCount++;
    SetNodeState(pNode, 2);
  }

  delete pPVTFile;
  StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
  StatusBar->Panels->Items[1]->Text = AnsiString("");
}

void __fastcall TMainForm::OnTilesLeft(TMessage & msg)
{
  if(msg.WParam)
    StatusBar->Panels->Items[0]->Text = (AnsiString("Tiles left: ") + AnsiString((int)msg.WParam));
  else
  {
    StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
    StatusBar->Panels->Items[1]->Text = AnsiString("");
  }
}

void __fastcall TMainForm::OnTileURL(TMessage & msg)
{
  StatusBar->Panels->Items[1]->Text = (AnsiString("URL: ") + AnsiString((const char*)msg.LParam));
}

void __fastcall TMainForm::OnTileCache(TMessage & msg)
{
  StatusBar->Panels->Items[1]->Text = (AnsiString("Cache: ") + AnsiString((const char*)msg.LParam));
}

void __fastcall TMainForm::OnTileMemory(TMessage & msg)
{
  StatusBar->Panels->Items[1]->Text = (AnsiString("Memory: ") + AnsiString((const char*)msg.LParam));
}

void __fastcall TMainForm::SmoothSplineActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;
  SplineSmoothForm->Track = pTrack->pTrack;

  if(SplineSmoothForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, SplineSmoothForm->Track, AnsiString(" - Spline"));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SmoothSplineActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;

    if(pTrack && pTrack->pTrack && !pTrack->pTrack->m_bNoTime)
      bEnabled = true;
  }

  SmoothSplineAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void TMainForm::OnShowPoint(TMessage & msg)
{
  if(m_pTrackView)
  {
    bool bSelected = (bool)msg.WParam;
    int nSelected = (bSelected) ? (int)msg.LParam : -1;

    m_pTrackView->CurrentPoint = nSelected;
    ShowPointHint(nSelected);
  }
}

void __fastcall TMainForm::ShowPointHint(int n)
{
  m_nSelectedPoint = n;
  
  if(n > -1)
  {
    PGPXTrack pTrack = m_pTrackView->TopmostTrack;
    PGPXTrackPoint pPoint = pTrack->Points[n];
    StatusBar->Panels->Items[0]->Text = AnsiString("Point ¹")   +
                                        AnsiString(n + 1)       +
                                        AnsiString(", Time: ") +
                                        pPoint->TimeString;
    StatusBar->Panels->Items[1]->Text = pPoint->GPXDescString;
  }
  else
  {
    StatusBar->Panels->Items[0]->Text = AnsiString("Ready");
    StatusBar->Panels->Items[1]->Text = AnsiString("");
  }
}

void __fastcall TMainForm::LoadFile(AnsiString asFileName, int nFileType, bool bFirstItem)
{
  switch(nFileType)
  {
    case 1: //GPX File
    {
      LoadGPXFile(asFileName);
      break;
    }
    
    case 2: //CSV File
      LoadCSVFile(asFileName);
      break;

    case 3: //NMEA File
      LoadNMEAFile(asFileName);
      break;

    case 4: //PVT File
      LoadPVTFile(asFileName);
      break;
  }

  m_pCursors->Wait = false;
  m_bNoDrag = false;

  if(bFirstItem)
  {
    TTreeNode *pFirst = TracksTree->Items->GetFirstNode();

    if(pFirst)
    {
      m_bInternalSelect = true;
      TracksTree->Selected = pFirst;
      ShowAllAction->Execute();
    }
  }
}

int __fastcall TMainForm::SelectTrackColor()
{
  int nColor;
  int nMin = 99999;
  bool bZeroFound = false;

  for(int i = 2; i < TrackColors->Count; i++)
  {
    if(m_pTrackColors[i].nCount == 0)
    {
      nColor = i;
      bZeroFound = true;
      break;
    }
    else if(nMin < m_pTrackColors[i].nCount)
      nMin = m_pTrackColors[i].nCount;
  }

  if(!bZeroFound)
  {
    for(int i = 2; i < TrackColors->Count; i++)
    {
      if(m_pTrackColors[i].nCount == nMin)
      {
        nColor = i;
        break;
      }
    }
  }

  return nColor;
}

void __fastcall TMainForm::OnLoadFile(TMessage & msg)
{
  int nFileType = (int)msg.WParam;
  LoadFile(m_asCmdLineFile, nFileType, true);
}

void __fastcall TMainForm::APLAActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;

  HPForm->Track = pTrack->pTrack;

  if(HPForm->ShowModal() == mrOk)
    AddProcessedTrack(pNode, HPForm->Track, AnsiString(" - HP"));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::APLAActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;
    if(pTrack && pTrack->pTrack) bEnabled = true;
  }

  APLAAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TracksTreeEdited(TObject *Sender, TTreeNode *Node, String &S)
{
  PVisibleTrack pTrack = (PVisibleTrack)Node->Data;
  pTrack->pTrack->m_asName = AnsiString(S);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ConfidenceActionExecute(TObject *Sender)
{
  PGPXTrack pBase = NULL, pTrend = NULL;
  TTreeNode *pTrendNode, *pBaseNode;

  for(int i = 0; i < TracksTree->Items->Count; i++)
  {
    TTreeNode *pNode = TracksTree->Items->Item[i];

    if(pNode->StateIndex > 1)
    {
      PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;

      if(!pBase)
      {
        pBase = pTrack->pTrack;
        pBaseNode = pNode;
      }
      else
      {
        pTrend = pTrack->pTrack;
        pTrendNode = pNode;
        break;
      }
    }
  }

  if(pBase && pTrend)
  {
    ConfidenceForm->SetTracks(pBase, pTrend);

    if(ConfidenceForm->ShowModal() == mrOk)
    {
      TTreeNode *pNode = (ConfidenceForm->Exchanged) ? pBaseNode : pTrendNode;
      AddProcessedTrack(pNode, ConfidenceForm->ResultTrack, AnsiString(" - Confidence"));
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ConfidenceActionUpdate(TObject *Sender)
{
  ConfidenceAction->Enabled = (!m_bNoDrag && m_nCheckedItems == 2);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CellEqActionExecute(TObject *Sender)
{
  TTreeNode *pNode = TracksTree->Selected;
  PVisibleTrack pTrack = (PVisibleTrack)pNode->Data;

  CellEqualizer->Track = pTrack->pTrack;

  if(CellEqualizer->ShowModal() == mrOk)
    AddProcessedTrack(pNode, CellEqualizer->Track, AnsiString(" - Cell Eq"));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CellEqActionUpdate(TObject *Sender)
{
  bool bEnabled = false;

  if(TracksTree->Selected && !m_bNoDrag)
  {
    PVisibleTrack pTrack = (PVisibleTrack)TracksTree->Selected->Data;
    if(pTrack && pTrack->pTrack) bEnabled = true;
  }

  CellEqAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Settings1Click(TObject *Sender)
{
  if(SettingsForm->ShowModal() == mrOk) m_pTrackView->InitCache();  
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::IdleHandler(TObject *Sender, bool &Done)
{
  if (m_pTrackView)
    m_pTrackView->Idle();
}

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
  Application->OnIdle = IdleHandler;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::NoMapActionExecute(TObject *Sender)
{
  MaptypeItem->Enabled = !NoMapAction->Checked;
  MapTypePopupItem->Enabled = MaptypeItem->Enabled;

  if(m_pTrackView)
  {
    m_pCursors->Wait = true;
    m_pTrackView->NoMap = NoMapAction->Checked;
    m_pTrackView->Refresh();
    m_pCursors->Wait = false;
  }

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  pini->WriteBool("Map", "NoMap", NoMapAction->Checked);
  delete pini;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OnCanvasUpdate(TMessage & msg)
{
  m_pTrackView->UpdateCanvas();
}
