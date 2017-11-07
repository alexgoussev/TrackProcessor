//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "SK42WGS84\\SK42WGS84.h"
#include "DES.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDESForm *DESForm;
//---------------------------------------------------------------------------
__fastcall TDESForm::TDESForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  Coordinates->Checked = pini->ReadBool("DES", "Coordinates", true);
  Altitude->Checked = pini->ReadBool("DES", "Altitude", false);
  Speed->Checked = pini->ReadBool("DES", "Speed", false);
  double dSmooth = pini->ReadFloat("DES", "Smooth", 0.25);
  delete pini;

  SmoothFactor->Text = AnsiString(dSmooth);
  m_pCurrentTrack = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TDESForm::GoClick(TObject *Sender)
{
  if(!Coordinates->Checked &&
     !Altitude->Checked    &&
     !Speed->Checked)
    MessageBox(Handle, TEXT("Nothing to do! Check at least one of Coordinates, Altitude or Speed"), TEXT("Warning"), MB_OK | MB_ICONSTOP);
  else if(m_pCurrentTrack)
  {
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    double dBeta = SmoothFactor->Text.ToDouble();
    double dAlpha = 1.0 - dBeta;
    dAlpha = 1.0 - dAlpha * dAlpha;
    double dGamma = dBeta * dBeta / dAlpha; 

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteBool("DES", "Coordinates", Coordinates->Checked);
    if(Altitude->Enabled) pini->WriteBool("DES", "Altitude", Altitude->Checked);
    if(Speed->Enabled) pini->WriteBool("DES", "Speed", Speed->Checked);
    pini->WriteFloat("DES", "Smooth", dBeta);
    delete pini;

    Progress->Max = m_pCurrentTrack->PointCount;
    Progress->Position = 0;

    PGPXTrackPoint p1 = m_pCurrentTrack->Points[0];
    PGPXTrackPoint p2 = m_pCurrentTrack->Points[1];

    double x, y;
    double dSxprev, dS2xprev;
    double dSyprev, dS2yprev;
    //double dSvprev, dS2vprev;
    //double dScprev, dS2cprev;
    double dSvxprev, dS2vxprev;
    double dSvyprev, dS2vyprev;
    double dSaprev, dS2aprev;

    p1->GetdProj(&x, &y);
    DecomposeSpeed(p1->m_dSpeed, p1->m_dCourse, &dSvxprev, &dSvyprev);
    
    dSxprev = x;
    dSyprev = y;
    //dSvprev = p1->m_dSpeed;
    //dScprev = p1->m_dCourse;
    dSaprev = p1->m_dAlt;

    p2->GetdProj(&x, &y);
    DecomposeSpeed(p2->m_dSpeed, p2->m_dCourse, &dS2vxprev, &dS2vyprev);

    dS2xprev = x - dSxprev;
    dS2yprev = y - dSyprev;
    dS2vxprev -= dSvxprev;
    dS2vyprev -= dSvyprev;
    //dS2vprev = dS2vxprev - dSvprev;
    //dS2cprev = p2->m_dCourse - dScprev;
    dS2aprev = p2->m_dAlt - dSaprev;


    for(int i = 1; i < m_pCurrentTrack->PointCount; i++)
    {
      PGPXTrackPoint p = m_pCurrentTrack->Points[i];
      p->GetdProj(&x, &y);

      if(Coordinates->Checked)
      {
        double dSx = dAlpha * x + (1.0 - dAlpha) * (dSxprev + dS2xprev);
        double dSy = dAlpha * y + (1.0 - dAlpha) * (dSyprev + dS2yprev);
        double dS2x = dGamma * (dSx - dSxprev)  + (1.0 - dGamma) * dS2xprev;
        double dS2y = dGamma * (dSy - dSyprev) + (1.0 - dGamma) * dS2yprev;
        x = dSxprev = dSx; dS2xprev = dS2x;
        y = dSyprev = dSy; dS2yprev = dS2y;

        p->SetdProj(x, y);
      }

      if(Altitude->Checked)
      {
        double dSa = dAlpha * p->m_dAlt + (1.0 - dAlpha) * (dSaprev + dS2aprev);
        double dS2a = dGamma * (dSa - dSaprev)  + (1.0 - dGamma) * dS2aprev;
        p->m_dAlt = dSaprev = dSa; dS2aprev = dS2a;
      }

      if(Speed->Checked)
      {
        double dSvx, dSvy;
        DecomposeSpeed(p->m_dSpeed, p->m_dCourse, &dSvx, &dSvy);

        dSvx = dAlpha * dSvx + (1.0 - dAlpha) * (dSvxprev + dS2vxprev);
        dSvy = dAlpha * dSvy + (1.0 - dAlpha) * (dSvyprev + dS2vyprev);
        dS2vxprev = dGamma * (dSvx - dSvxprev) + (1.0 - dGamma) * dS2vxprev;
        dS2vyprev = dGamma * (dSvy - dSvyprev) + (1.0 - dGamma) * dS2vyprev;

        double dS, dC;
        ComposeSpeed(dSvx, dSvy, &dS, &dC);

        p->m_dSpeed = dS; p->m_dCourse = dC;
        dSvxprev = dSvx; dSvyprev = dSvy;

        //double dSv = dAlpha * p->m_dSpeed + (1.0 - dAlpha) * (dSvprev + dS2vprev);
        //double dSc = dAlpha * p->m_dCourse + (1.0 - dAlpha) * (dScprev + dS2cprev);
        //double dS2v = dGamma * (dSv - dSvprev) + (1.0 - dGamma) * dS2vprev;
        //double dS2c = dGamma * (dSc - dScprev) + (1.0 - dGamma) * dS2cprev;
        //p->m_dSpeed = dSvprev = dSv; dS2vprev = dS2v;
        //p->m_dCourse = dScprev = dSc; dS2cprev = dS2c;
      }

      Progress->Position = i;
      Application->ProcessMessages();
    }

    m_pCurrentTrack->RecalcDistClimb();

    Screen->Cursor = Save_Cursor;
    Progress->Position = 0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDESForm::FormShow(TObject *Sender)
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

