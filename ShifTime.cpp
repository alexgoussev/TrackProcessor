//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "ShifTime.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TTimeShift *TimeShift;
//---------------------------------------------------------------------------
__fastcall TTimeShift::TTimeShift(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  int nShift = pini->ReadInteger("TimeShift", "Shift", nShift);
  delete pini;

  if(nShift < 0)
  {
    ShiftDir->ItemIndex = 1;
    nShift = -nShift;
  }
  else ShiftDir->ItemIndex = 0;

  ShiftH->Text = AnsiString(nShift / 3600);
  nShift %= 3600;
  ShiftM->Text = AnsiString(nShift / 60);
  nShift %= 60;
  ShiftS->Text = AnsiString(nShift);
}
//---------------------------------------------------------------------------
void __fastcall TTimeShift::GoClick(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    int nShift = ShiftH->Text.ToInt() * 3600 +
                 ShiftM->Text.ToInt() * 60 +
                 ShiftS->Text.ToInt();

    if(nShift > 0)
    {
      TCursor Save_Cursor = Screen->Cursor;
      Screen->Cursor = crHourGlass;

      ProgressBar->Max = m_pCurrentTrack->PointCount;
      ProgressBar->Position = 0;

      if(ShiftDir->ItemIndex == 1) nShift = -nShift;

      TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
      pini->WriteInteger("TimeShift", "Shift", nShift);
      delete pini;

      for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
      {
        PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];
        pPoint->m_dTime += (double)nShift;

        ProgressBar->Position = i;
        Application->ProcessMessages();
      }

      ProgressBar->Position = 0;
      Screen->Cursor = Save_Cursor;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TTimeShift::FormShow(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    PGPXTrackPoint pFirst = m_pCurrentTrack->Points[0];
    PGPXTrackPoint pLast = m_pCurrentTrack->Points[m_pCurrentTrack->PointCount - 1];

    FirstPtTime->Text = pFirst->TimeString;
    LastPtTime->Text = pLast->TimeString;
  }
}
//---------------------------------------------------------------------------
