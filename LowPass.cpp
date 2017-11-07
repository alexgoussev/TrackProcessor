//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "SK42WGS84\\SK42WGS84.h"
#include "LowPass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLowPassForm *LowPassForm;
//---------------------------------------------------------------------------
__fastcall TLowPassForm::TLowPassForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  Coordinates->Checked = pini->ReadBool("LowPass", "Coordinates", true);
  Altitude->Checked = pini->ReadBool("LowPass", "Altitude", false);
  Speed->Checked = pini->ReadBool("LowPass", "Speed", false);
  double dSmooth = pini->ReadFloat("LowPass", "Smooth", 0.25);
  delete pini;

  Smoothing->Text = AnsiString(dSmooth);
}
//---------------------------------------------------------------------------

void __fastcall TLowPassForm::GoClick(TObject *Sender)
{
  if(!Coordinates->Checked &&
     !Altitude->Checked    &&
     !Speed->Checked)
    MessageBox(Handle, TEXT("Nothing to do! Check at least one of Coordinates, Altitude or Speed"), TEXT("Warning"), MB_OK | MB_ICONSTOP);
  else if(m_pCurrentTrack)
  {
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    PGPXTrack pOriginalTrack = new GPXTrack(m_pCurrentTrack);

    double alpha = Smoothing->Text.ToDouble();

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteBool("LowPass", "Coordinates", Coordinates->Checked);
    if(Altitude->Enabled) pini->WriteBool("LowPass", "Altitude", Altitude->Checked);
    if(Speed->Enabled) pini->WriteBool("LowPass", "Speed", Speed->Checked);
    pini->WriteFloat("LowPass", "Smooth", alpha);
    delete pini;

    ProgressBar->Max = pOriginalTrack->PointCount;
    ProgressBar->Position = 0;

    for(int i = 1; i < pOriginalTrack->PointCount; i++)
    {
      PGPXTrackPoint inP2 = pOriginalTrack->Points[i];
      PGPXTrackPoint outP1 = m_pCurrentTrack->Points[i - 1];
      PGPXTrackPoint outP2 = m_pCurrentTrack->Points[i];

      if(Coordinates->Checked)
      {
        double inX2, inY2;
        double outX1, outX2, outY1, outY2;

        inP2->GetdProj(&inX2, &inY2);
        outP1->GetdProj(&outX1, &outY1);

        outX2 = outX1 + alpha * (inX2 - outX1);
        outY2 = outY1 + alpha * (inY2 - outY1);

        outP2->SetdProj(outX2, outY2);
      }

      if(Speed->Checked)
      {
        double dOutP1SpeedX, dOutP1SpeedY;
        double dInP2SpeedX, dInP2SpeedY;
        double dOutP2SpeedX, dOutP2SpeedY;
        double dOutP2Speed, dOutP2Course;

        DecomposeSpeed(outP1->m_dSpeed, outP1->m_dCourse, &dOutP1SpeedX, &dOutP1SpeedY);
        DecomposeSpeed(inP2->m_dSpeed, inP2->m_dCourse, &dInP2SpeedX, &dInP2SpeedY);

        dOutP2SpeedX = dOutP1SpeedX + alpha * (dInP2SpeedX - dOutP1SpeedX);
        dOutP2SpeedY = dOutP1SpeedY + alpha * (dInP2SpeedY - dOutP1SpeedY);

        //outP2->m_dSpeed = outP1->m_dSpeed + alpha * (inP2->m_dSpeed - outP1->m_dSpeed);
        //outP2->m_dCourse = outP1->m_dCourse + alpha * (inP2->m_dCourse - outP1->m_dCourse);

        ComposeSpeed(dOutP2SpeedX, dOutP2SpeedY, &dOutP2Speed, &dOutP2Course);
        outP2->m_dSpeed = dOutP2Speed;
        outP2->m_dCourse = dOutP2Course;
      }

      if(Altitude->Checked)
        outP2->m_dAlt = outP1->m_dAlt + alpha * (inP2->m_dAlt - outP1->m_dAlt);

      ProgressBar->Position = i;
      Application->ProcessMessages();
    }

    m_pCurrentTrack->RecalcDistClimb();
    
    delete pOriginalTrack;
    Screen->Cursor = Save_Cursor;
  }
}
//---------------------------------------------------------------------------
void __fastcall TLowPassForm::FormShow(TObject *Sender)
{
  if(m_pCurrentTrack && !m_pCurrentTrack->m_bNoAlt)
    Altitude->Enabled = true;
  else
  {
    Altitude->Checked = false;
    Altitude->Enabled = false;
  }

  if(m_pCurrentTrack && !m_pCurrentTrack->m_bNoTime)
    Speed->Enabled = true;
  else
  {
    Speed->Checked = false;
    Speed->Enabled = false;
  }
}
//---------------------------------------------------------------------------


