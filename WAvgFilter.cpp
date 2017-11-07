//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "WAvgFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWAvgForm *WAvgForm;
//---------------------------------------------------------------------------
__fastcall TWAvgForm::TWAvgForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  double DistTh = pini->ReadFloat("AveraginSmooth", "AvgDistance", 10.0);
  double TimeTh = pini->ReadFloat("AveraginSmooth", "TimeTh", 2.0);
  double SpeedTh = pini->ReadFloat("AveraginSmooth", "MinSpeed", 1.0);
  double CourceTh = pini->ReadFloat("AveraginSmooth", "MinAngle", 30.0);
  double AvgTm = pini->ReadFloat("AveraginSmooth", "AverageTime", 25.0);
  double SmoothFact = pini->ReadFloat("AveraginSmooth", "SmoothFactor", 0.25);
  bool AltSmooth = pini->ReadBool("AveraginSmooth", "AltSmooth", false);
  bool SpeedSmooth = pini->ReadBool("AveraginSmooth", "SpeedSmooth", false);
  delete pini;

  Distance->Text = AnsiString(DistTh);
  Time->Text = AnsiString(TimeTh);
  Speed->Text = AnsiString(SpeedTh);
  Angle->Text = AnsiString(CourceTh);
  AvgTime->Text = AnsiString(AvgTm);
  SmoothFactor->Text = AnsiString(SmoothFact);
  Altitude->Checked = AltSmooth;
  SpeedCourse->Checked = SpeedSmooth;
}
//---------------------------------------------------------------------------

void __fastcall TWAvgForm::OnPoint(TMessage & msg)
{
  ProgressBar->Position = (int)(msg.WParam);
}

void __fastcall TWAvgForm::GoClick(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    WheitedAverage *pWAvg = new WheitedAverage(Handle);

    pWAvg->CurrentTrack = m_pCurrentTrack;
    pWAvg->DistTh = Distance->Text.ToDouble();
    pWAvg->TimeTh = Time->Text.ToDouble();
    pWAvg->SpeedTh = Speed->Text.ToDouble();
    pWAvg->CourceTh = Angle->Text.ToDouble();
    pWAvg->AvgTime = AvgTime->Text.ToDouble();
    pWAvg->SmoothFactor = SmoothFactor->Text.ToDouble();
    pWAvg->AltSmooth = Altitude->Checked;
    pWAvg->SpeedSmooth = SpeedCourse->Checked;

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteFloat("AveraginSmooth", "AvgDistance", pWAvg->DistTh);
    pini->WriteFloat("AveraginSmooth", "TimeTh", pWAvg->TimeTh);
    pini->WriteFloat("AveraginSmooth", "MinSpeed", pWAvg->SpeedTh);
    pini->WriteFloat("AveraginSmooth", "MinAngle", pWAvg->CourceTh);
    pini->WriteFloat("AveraginSmooth", "AverageTime", pWAvg->AvgTime);
    pini->WriteFloat("AveraginSmooth", "SmoothFactor", pWAvg->SmoothFactor);
    pini->WriteBool("AveraginSmooth", "AltSmooth", Altitude->Checked);
    pini->WriteBool("AveraginSmooth", "SpeedSmooth", SpeedCourse->Checked);
    delete pini;
    
    ProgressBar->Max = m_pCurrentTrack->PointCount;
    ProgressBar->Position = 0;
    Status->Caption = AnsiString("Building averaged track...");
    int n = pWAvg->BuildAvgTrack();

    ProgressBar->Max = n;
    ProgressBar->Position = 0;
    Status->Caption = AnsiString("Building splines...");
    pWAvg->BuildSplines();

    ProgressBar->Max = m_pCurrentTrack->PointCount;
    ProgressBar->Position = 0;
    Status->Caption = AnsiString("Interpolating...");
    pWAvg->Interpolate();

    ProgressBar->Position = 0;
    Status->Caption = AnsiString("Ready");
    Application->ProcessMessages();

    delete pWAvg;
    m_pCurrentTrack->RecalcDistClimb();
    Screen->Cursor = Save_Cursor;
  }
}
//---------------------------------------------------------------------------
