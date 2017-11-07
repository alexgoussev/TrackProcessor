//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "DPFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDPForm *DPForm;
//---------------------------------------------------------------------------
__fastcall TDPForm::TDPForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  double dEpsilon = pini->ReadFloat("Douglas-Peucker", "Epsilon", 1.0);
  delete pini;

  Epsilon->Text = AnsiString(dEpsilon);
}

//---------------------------------------------------------------------------
void __fastcall TDPForm::GoClick(TObject *Sender)
{
  if(m_pCurrentTrack && m_pCurrentTrack->PointCount > 2)
  {
    ProgressBar->Max = m_pCurrentTrack->PointCount * (int)log10((double)m_pCurrentTrack->PointCount);
    ProgressBar->Position = 0;

    Exit->Enabled = false;
    Go->Enabled = false;

    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    DouglasPeucker *pDP = new DouglasPeucker(Handle);

    pDP->Epsilon = Epsilon->Text.ToDouble();

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteFloat("Douglas-Peucker", "Epsilon", pDP->Epsilon);
    delete pini;

    pDP->CurrentTrack = m_pCurrentTrack;
    pDP->Simplify();

    delete pDP;

    ProgressBar->Position = 0;
    Exit->Enabled = true;
    Go->Enabled = true;

    Screen->Cursor = Save_Cursor;
  }
}

//---------------------------------------------------------------------------

void __fastcall TDPForm::OnPoint(TMessage & msg)
{
  if(ProgressBar->Position == ProgressBar->Max) ProgressBar->Position = 0;
  ProgressBar->Position = ProgressBar->Position + 1;
}
