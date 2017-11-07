//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "GPSKalman.h"
#include "GPSKalmanForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGPSKalmanFilter *GPSKalmanFilter;
//---------------------------------------------------------------------------
__fastcall TGPSKalmanFilter::TGPSKalmanFilter(TComponent* Owner)
  : TForm(Owner)
{
  m_pFilteredTrack = NULL;
  m_pCurrentTrack = NULL;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  RValue->Text = AnsiString(pini->ReadFloat("Kalman", "R", 15.0));
  SaveSpeed->Checked = pini->ReadBool("Kalman", "SaveSpeed", true);
  SaveAlt->Checked = pini->ReadBool("Kalman", "SaveAlt", true);
  delete pini;
}
//---------------------------------------------------------------------------

void __fastcall TGPSKalmanFilter::m_OkClick(TObject *Sender)
{
  double dRValue = RValue->Text.ToDouble();
  bool bSaveSpeed = SaveSpeed->Checked;
  bool bSaveAlt = SaveAlt->Checked;

  ProgressBar->Max = m_pCurrentTrack->PointCount * 3 - 1;
  ProgressBar->Position = 0;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  pini->WriteFloat("Kalman", "R", dRValue);
  pini->WriteBool("Kalman", "SaveSpeed", bSaveSpeed);
  if(SaveAlt->Enabled) pini->WriteBool("Kalman", "SaveAlt", bSaveAlt);
  delete pini;

  m_pFilteredTrack = new GPXTrack;

  double *pAcc = new double[m_pCurrentTrack->PointCount];
  double *pAltSpd = new double[m_pCurrentTrack->PointCount];
  double *pAltAcc = new double[m_pCurrentTrack->PointCount];
  double dAvgAcc = 0.0;
  double dAvgAltAcc = 0.0;
  pAcc[0] = 0.0;
  pAltSpd[0] = 0.0;
  pAltAcc[0] = 0.0;

  for(int i = 1; i < m_pCurrentTrack->PointCount; i++)
  {
    ProgressBar->Position = ProgressBar->Position + 1;

    pAcc[i] = pAltSpd[i] = pAltAcc[i] = 0.0;
    double dT = m_pCurrentTrack->Points[i]->m_dTime - m_pCurrentTrack->Points[i - 1]->m_dTime;

    if(dT > 0.0)
    {
      pAcc[i] = (m_pCurrentTrack->Points[i]->m_dSpeed - m_pCurrentTrack->Points[i - 1]->m_dSpeed) / dT;

      if(!m_pCurrentTrack->m_bNoAlt)
      {
        pAltSpd[i] = (m_pCurrentTrack->Points[i]->m_dAlt - m_pCurrentTrack->Points[i - 1]->m_dAlt) / dT;
        pAltAcc[i] = (pAltSpd[i] - pAltSpd[i - 1]) / dT;
      }
    }

    dAvgAcc += pAcc[i];
    if(!m_pCurrentTrack->m_bNoAlt) dAvgAltAcc += pAltAcc[i];
  }

  dAvgAcc /= (double)m_pCurrentTrack->PointCount;
  if(!m_pCurrentTrack->m_bNoAlt) dAvgAltAcc /= (double)m_pCurrentTrack->PointCount;
    
  double dSigma2 = 0.0;
  double dAltSigma2 = 0.0;

  for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
  {
    ProgressBar->Position = ProgressBar->Position + 1;

    double dA = pAcc[i] - dAvgAcc;
    dSigma2 += (dA * dA);

    if(!m_pCurrentTrack->m_bNoAlt)
    {
      dA = pAltAcc[i] - dAvgAltAcc;
      dAltSigma2 += (dA * dA);
    }
  }

  dSigma2 /= (double)m_pCurrentTrack->PointCount;
  if(!m_pCurrentTrack->m_bNoAlt) dAltSigma2 /= (double)m_pCurrentTrack->PointCount;

  GPSKalman2D *pKalman = new GPSKalman2D();
  pKalman->R = dRValue; pKalman->Sigma2 = dSigma2;
  if(!m_pCurrentTrack->m_bNoAlt) pKalman->AltSigma = dAltSigma2;
  pKalman->FilterAlt = !m_pCurrentTrack->m_bNoAlt;

  for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
  {
    ProgressBar->Position = ProgressBar->Position + 1;
    Application->ProcessMessages();

    PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];

    double dTime = pPoint->m_dTime;
    double dLat = pPoint->m_dLat;
    double dLon = pPoint->m_dLon;
    double dSpeed = pPoint->m_dSpeed;
    double dCourse = pPoint->m_dCourse;
    double dAlt = pPoint->m_dAlt;
    double dAltSpeed = pAltSpd[i];

    pKalman->Filter(dTime, dLat, dLon, dSpeed, dCourse, dAlt, dAltSpeed);

    PGPXTrackPoint pFilteredPoint = m_pFilteredTrack->NewPoint;
    pFilteredPoint->SetPoint(pPoint);

    pFilteredPoint->m_dLat = dLat;
    pFilteredPoint->m_dLon = dLon;

    if(bSaveSpeed)
    {
      pFilteredPoint->m_dSpeed = dSpeed;
      pFilteredPoint->m_dCourse = dCourse;
    }

    if(bSaveAlt)
      pFilteredPoint->m_dAlt = dAlt;

    m_pFilteredTrack->AddPoint(pFilteredPoint);
  }

  delete [] pAcc;
  delete [] pAltAcc;
  delete [] pAltSpd;
  delete pKalman;

  ProgressBar->Position = 0;
}
//---------------------------------------------------------------------------

void __fastcall TGPSKalmanFilter::SetPCurrentTrack(PGPXTrack value)
{
  if(m_pCurrentTrack != value) {
    m_pCurrentTrack = value;
    m_pFilteredTrack = NULL;

    if(m_pCurrentTrack)
    {
      if(m_pCurrentTrack->m_bNoAlt)
      {
        SaveAlt->Checked = false;
        SaveAlt->Enabled = false;
      }
      else
      {
        SaveAlt->Enabled = true;
        TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
        SaveAlt->Checked = pini->ReadBool("Kalman", "SaveAlt", true);
        delete pini;
      }
    }
  }
}

PGPXTrack __fastcall TGPSKalmanFilter::GetPCurrentTrack()
{
  return m_pFilteredTrack;
}

void __fastcall TGPSKalmanFilter::ValueKeyPress(TObject *Sender,
      char &Key)
{
  if((Key >= '0' && Key <= '9') ||
     Key == VK_BACK             ||
     Key == VK_RETURN           ||
     Key == '.') {}
  else Key = 0;
}
//---------------------------------------------------------------------------


