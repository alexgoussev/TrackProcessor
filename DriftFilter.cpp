//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "DriftFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDriftForm *DriftForm;
//---------------------------------------------------------------------------
__fastcall TDriftForm::TDriftForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  int nAperture = pini->ReadInteger("Drift", "Aperture", 50);
  double dSpeed = pini->ReadFloat("Drift", "SpeedTh", 1.0);
  int nStrength = pini->ReadInteger("Drift", "Strength", 2);
  delete pini;

  Aperture->Text = AnsiString(nAperture);
  Thereshould->Text = AnsiString(dSpeed);
  Strength->ItemIndex = nStrength;
}
//---------------------------------------------------------------------------

void __fastcall TDriftForm::OnPoint(TMessage & msg)
{
  ProgressBar->Position = ProgressBar->Position + 1;
}

void __fastcall TDriftForm::GoClick(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    int nAperture = Aperture->Text.ToInt();
    double dThereshould = Thereshould->Text.ToDouble();
    int nStrength = Strength->ItemIndex;

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteInteger("Drift", "Aperture", nAperture);
    pini->WriteFloat("Drift", "SpeedTh", dThereshould);
    pini->WriteInteger("Drift", "Strength", nStrength);
    delete pini;

    eStrength FilterStrength[4] = {eExtremely, eHigh, eAverage, eLow};
    Drift *pDrift = new Drift(Handle);
    pDrift->m_pCurrentTrack = m_pCurrentTrack;

    ProgressBar->Max = m_pCurrentTrack->PointCount;
    ProgressBar->Position = 0;
    Action->Caption = AnsiString("Detecting Drift Intervals...");

    int n = pDrift->FindIntervals(nAperture, dThereshould);

    ProgressBar->Max = n;
    ProgressBar->Position = 0;
    Action->Caption = AnsiString("Merging Drift Intervals...");

    n = pDrift->MergeIntervals();

    ProgressBar->Max = n;
    ProgressBar->Position = 0;
    Action->Caption = AnsiString("Processing Drift Intervals...");

    pDrift->ProcessIntervals(FilterStrength[nStrength]);

    delete pDrift;

    m_pCurrentTrack->RecalcDistClimb();

    Action->Caption = AnsiString("");
    ProgressBar->Position = 0;
    Screen->Cursor = Save_Cursor;
  }
}
//---------------------------------------------------------------------------
