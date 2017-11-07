//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "Split.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSplitForm *SplitForm;
//---------------------------------------------------------------------------
__fastcall TSplitForm::TSplitForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_nCriterion = pini->ReadInteger("Split", "Criterion", 0);
  int nDuration = pini->ReadInteger("Split", "Duration", 3600);
  int nInterval = pini->ReadInteger("Split", "Interval", 600);
  int nLength = pini->ReadInteger("Split", "Length", 10000);
  int nDistance = pini->ReadInteger("Split", "Distance", 100);
  delete pini;

  m_pCriterion[0] = TrackDuration;
  m_pCriterion[1] = PointsInterval;
  m_pCriterion[2] = TrackLength;
  m_pCriterion[3] = PointsDistance;

  for(int i = 0; i < 4; i++)
  {
    if(m_nCriterion == i) m_pCriterion[i]->Checked = true;
    else m_pCriterion[i]->Checked = false;
  }

  HourD->Text = AnsiString(nDuration / 3600);
  nDuration %= 3600;
  MinD->Text = AnsiString(nDuration / 60);
  nDuration %= 60;
  SecD->Text = AnsiString(nDuration);

  HourL->Text = AnsiString(nInterval / 3600);
  nInterval %= 3600;
  MinL->Text = AnsiString(nInterval / 60);
  nInterval %= 60;
  SecL->Text = AnsiString(nInterval);

  Length->Text = AnsiString(nLength);
  Distance->Text = AnsiString(nDistance);

  m_pResult = new TList;
}
//---------------------------------------------------------------------------

__fastcall TSplitForm::~TSplitForm()
{
  m_pResult->Clear();
  delete m_pResult;
}

void __fastcall TSplitForm::GoClick(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    m_pResult->Clear();

    int nDuration = HourD->Text.ToInt() * 3600 +
                    MinD->Text.ToInt() * 60 +
                    SecD->Text.ToInt();
    int nInterval = HourL->Text.ToInt() * 3600 +
                    MinL->Text.ToInt() * 60 +
                    SecL->Text.ToInt();
    int nLength = Length->Text.ToInt();
    int nDistance = Distance->Text.ToInt();

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteInteger("Split", "Criterion", m_nCriterion);
    pini->WriteInteger("Split", "Duration", nDuration);
    pini->WriteInteger("Split", "Interval", nInterval);
    pini->WriteInteger("Split", "Length", nLength);
    pini->WriteInteger("Split", "Distance", nDistance);
    delete pini;

    ProgressBar->Max = m_pCurrentTrack->PointCount;
    ProgressBar->Position = 0;

    PGPXTrack pNewTrack = new GPXTrack;
    PGPXTrackPoint pBasePoint = m_pCurrentTrack->Points[0];
    CopyPointToTrack(pNewTrack, pBasePoint);

    double dDuration = 0.0, dLength = 0.0;

    for(int i = 1; i < m_pCurrentTrack->PointCount; i++)
    {
      PGPXTrackPoint p = m_pCurrentTrack->Points[i];

      switch(m_nCriterion)
      {
        case 0:
        {
          if(dDuration >= (double)nDuration)
          {
            pNewTrack = AddTrackToResult(pNewTrack);
            dDuration = 0.0;
          }

          dDuration += p->m_dInterval;
          break;
        }

        case 1:
        {
          if(p->m_dInterval >= (double)nInterval)
            pNewTrack = AddTrackToResult(pNewTrack);

          break;
        }

        case 2:
        {
          if(dLength >= (double)nLength)
          {
            pNewTrack = AddTrackToResult(pNewTrack);
            dLength = 0.0;
          }

          dLength += p->m_dSpacing;
          break;
        }

        case 3:
        {
          if(p->m_dSpacing >= (double)nDistance)
            pNewTrack = AddTrackToResult(pNewTrack);

          break;
        }
      }

      CopyPointToTrack(pNewTrack, p);
      ProgressBar->Position = i;
      Application->ProcessMessages();
    }

    m_pResult->Add(pNewTrack);
    ProgressBar->Position = 0;
    Screen->Cursor = Save_Cursor;
  }
}

//---------------------------------------------------------------------------
void __fastcall TSplitForm::FormShow(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    if(m_pCurrentTrack->m_bNoTime)
    {
      if(m_nCriterion < 2)
      {
        m_pCriterion[m_nCriterion]->Checked = false;
        m_nCriterion = 2;
        m_pCriterion[m_nCriterion]->Checked = true;
      }

      m_pCriterion[0]->Enabled = false;
      m_pCriterion[1]->Enabled = false;
      HourD->Enabled = false;
      MinD->Enabled = false;
      SecD->Enabled = false;
      HourL->Enabled = false;
      MinL->Enabled = false;
      SecL->Enabled = false;
    }
    else
    {
      m_pCriterion[0]->Enabled = true;
      m_pCriterion[1]->Enabled = true;
      HourD->Enabled = true;
      MinD->Enabled = true;
      SecD->Enabled = true;
      HourL->Enabled = true;
      MinL->Enabled = true;
      SecL->Enabled = true;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TSplitForm::CriterionClick(TObject *Sender)
{
  for(int i = 0; i < 4; i++)
  {
    if(m_pCriterion[i]->Checked) m_nCriterion = i;
  }
}
//---------------------------------------------------------------------------

void __fastcall TSplitForm::CopyPointToTrack(PGPXTrack pTrack, PGPXTrackPoint pPoint)
{
  PGPXTrackPoint pNewPoint = pTrack->NewPoint;
  pNewPoint->SetPoint(pPoint);
  pTrack->AddPoint(pNewPoint);
}

PGPXTrack __fastcall TSplitForm::AddTrackToResult(PGPXTrack pNewTrack)
{
  m_pResult->Add(pNewTrack);
  return (new GPXTrack);
}
