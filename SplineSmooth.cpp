//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "smoothedspline\\smoothedspline.h"
#include "SK42WGS84\\SK42WGS84.h"
#include "SplineSmooth.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSplineSmoothForm *SplineSmoothForm;
//---------------------------------------------------------------------------
__fastcall TSplineSmoothForm::TSplineSmoothForm(TComponent* Owner)
  : TForm(Owner)
{
  m_bInProgress = false;
  m_pFilteredTrack = m_pTrack = NULL;
}

__fastcall TSplineSmoothForm::~TSplineSmoothForm()
{
  //TODO: Add your source code here
}

//---------------------------------------------------------------------------
void __fastcall TSplineSmoothForm::OnKeyPress(
      TObject *Sender, char &Key)
{
  if((Key >= '0' && Key <= '9') ||
     Key == VK_BACK             ||
     Key == VK_RETURN           ||
     Key == '.') {}
  else Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TSplineSmoothForm::StartActionExecute(TObject *Sender)
{
  SaveParams();
  m_bInProgress = true;

  double *pTime = new double[m_pTrack->PointCount];
  double *pX = new double[m_pTrack->PointCount];
  double *pY = new double[m_pTrack->PointCount];
  double *pAlt = new double[m_pTrack->PointCount];
  double *pVx = new double[m_pTrack->PointCount];
  double *pVy = new double[m_pTrack->PointCount];
  //double *pSpeed = new double[m_pTrack->PointCount];
  //double *pCourse = new double[m_pTrack->PointCount];
  double *pSFactor = new double[m_pTrack->PointCount];

  for(int i = 0; i < m_pTrack->PointCount; i++)
  {
    ProgressBar->Position = i;

    pSFactor[i] = m_dBaseSmoothFactor;

    PGPXTrackPoint pPoint = m_pTrack->Points[i];
    pPoint->GetdProj(&pX[i], &pY[i]);
    if(!m_pTrack->m_bNoAlt) pAlt[i] = pPoint->m_dAlt;
    DecomposeSpeed(pPoint->m_dSpeed, pPoint->m_dCourse, &pVx[i], &pVy[i]);
    //pSpeed[i] = pPoint->m_dSpeed;
    //pCourse[i] = pPoint->m_dCourse;
    pTime[i] = pPoint->m_dTime;

    if(!!m_pTrack->m_bNoHDOP && pPoint->m_dHDOP > m_dHDOPLevel && m_dHDOPIncr > 0.0)
      pSFactor[i] += (((pPoint->m_dHDOP - m_dHDOPLevel) / m_dHDOPStep) * m_dHDOPIncr);

    if(pPoint->m_dSpeed < m_dSpeedLevel && m_dSpeedIncr > 0.0)
      pSFactor[i] += (((m_dSpeedLevel - pPoint->m_dSpeed) / m_dSpeedStep) * m_dSpeedIncr);

    if(i > 0)
    {
      if(m_dCourseIncr > 0.0)
      {
        double dC1, dC2;

        if(m_bInstantCourse)
        {
          dC2 = AzimuthLL(m_pTrack->Points[i - 1]->m_dLat,
                          m_pTrack->Points[i - 1]->m_dLon,
                          m_pTrack->Points[i]->m_dLat,
                          m_pTrack->Points[i]->m_dLon);

          if(i == 1)
            dC1 = 0.0;
          else
            dC1 = AzimuthLL(m_pTrack->Points[i - 2]->m_dLat,
                            m_pTrack->Points[i - 2]->m_dLon,
                            m_pTrack->Points[i - 1]->m_dLat,
                            m_pTrack->Points[i - 1]->m_dLon);
        }
        else
        {
          dC1 = m_pTrack->Points[i - 1]->m_dCourse;
          dC2 = m_pTrack->Points[i]->m_dCourse;
        }

        ArrangeCourses(&dC2, &dC1);
        double dC = NormalizeCourse(fabs(dC2 - dC1)) / (pTime[i] - pTime[i - 1]);

        if(dC > m_dCourseLevel)
          pSFactor[i] += (((dC - m_dCourseLevel) / m_dCourseStep) * m_dCourseIncr);
      }

      if(m_dAccIncr > 0.0)
      {
        double dV1, dV2;

        if(m_bInstatntSpeed)
        {
          dV2 = m_pTrack->Points[i]->m_dSpacing / (pTime[i] - pTime[i - 1]);

          if(i == 1)
            dV1 = 0.0;
          else
            dV1 = m_pTrack->Points[i - 1]->m_dSpacing / (pTime[i - 1] - pTime[i - 2]);
        }
        else
        {
          dV1 = m_pTrack->Points[i - 1]->m_dSpeed;
          dV2 = m_pTrack->Points[i]->m_dSpeed;
        }

        double dV = fabs(dV2 - dV1) / (pTime[i] - pTime[i - 1]);

        if(dV > m_dAccLevel)
          pSFactor[i] += (((dV - m_dAccLevel) / m_dAccStep) * m_dAccIncr);
      }
    }
  }

  SmoothedSpline *pSmCoordX = new SmoothedSpline;
  SmoothedSpline *pSmCoordY = new SmoothedSpline;
  SmoothedSpline *pSmAltitude = new SmoothedSpline;
  SmoothedSpline *pSmVx = new SmoothedSpline;
  SmoothedSpline *pSmVy = new SmoothedSpline;
  //SmoothedSpline *pSmCourse = new SmoothedSpline;
  //SmoothedSpline *pSmSpeed = new SmoothedSpline;

  double *pSX = NULL;
  double *pSY = NULL;
  double *pSAlt = NULL;
  double *pSVx = NULL;
  double *pSVy = NULL;
  //double *pSSpeed = NULL;
  //double *pSCourse = NULL;

  if(m_bSmoothCoordinates)
  {
    pSmCoordX->Create(m_pTrack->PointCount, pTime, pX, pSFactor);
    pSmCoordY->Create(m_pTrack->PointCount, pTime, pY, pSFactor);

    pSX = pSmCoordX->SmoothedY();
    pSY = pSmCoordY->SmoothedY();
  }

  if(m_bSmoothAltitude)
  {
    pSmAltitude->Create(m_pTrack->PointCount, pTime, pAlt, pSFactor);
    pSAlt = pSmAltitude->SmoothedY();
  }

  if(m_bSmoothSpeed)
  {
    pSmVx->Create(m_pTrack->PointCount, pTime, pVx, pSFactor);
    pSmVy->Create(m_pTrack->PointCount, pTime, pVy, pSFactor);
    //pSmCourse->Create(m_pTrack->PointCount, pTime, pCourse, pSFactor);
    //pSmSpeed->Create(m_pTrack->PointCount, pTime, pSpeed, pSFactor);

    pSVx = pSmVx->SmoothedY();
    pSVy = pSmVy->SmoothedY();
    //pSCourse = pSmCourse->SmoothedY();
    //pSSpeed = pSmSpeed->SmoothedY();
  }

  m_pFilteredTrack = new GPXTrack;
  m_pFilteredTrack->m_asName = m_pTrack->m_asName;
  m_pFilteredTrack->m_asDesc = m_pTrack->m_asDesc;
  m_pFilteredTrack->m_asFileName = m_pTrack->m_asFileName;
  m_pFilteredTrack->m_asCmt = m_pTrack->m_asCmt;
  m_pFilteredTrack->m_asSrc = m_pTrack->m_asSrc;

  for(int i = 0; i < m_pTrack->PointCount; i++)
  {
    ProgressBar->Position = i + m_pTrack->PointCount;

    PGPXTrackPoint pPoint = m_pTrack->Points[i];
    PGPXTrackPoint pSPoint = m_pFilteredTrack->NewPoint;
    pSPoint->SetPoint(pPoint);

    if(pSX && pSY) pSPoint->SetdProj(pSX[i], pSY[i]);
    if(pSAlt) pSPoint->m_dAlt = pSAlt[i];
    //if(pSCourse && pSSpeed)
    if(pSVx && pSVy)
    {
      double dV, dC;

      ComposeSpeed(pSVx[i], pSVy[i], &dV, &dC);

      pSPoint->m_dSpeed = dV;
      pSPoint->m_dCourse = dC;
      //pSPoint->m_dCourse = NormalizeCourse(pSCourse[i]);
      //pSPoint->m_dSpeed = (pSSpeed[i] >= 0.0) ? pSSpeed[i] : 0.0;
    }

    m_pFilteredTrack->AddPoint(pSPoint);
  }

  delete [] pTime;
  delete [] pX;
  delete [] pY;
  delete [] pAlt;
  delete [] pVx;
  delete [] pVy;
  //delete [] pSpeed;
  //delete [] pCourse;
  delete [] pSFactor;

  delete pSmCoordX;
  delete pSmCoordY;
  delete pSmAltitude;
  delete pSmVx;
  delete pSmVy;
  //delete pSmCourse;
  //delete pSmSpeed;

  m_bInProgress = false;
  ProgressBar->Position = 0;
  m_pTrack = NULL;
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TSplineSmoothForm::StartActionUpdate(TObject *Sender)
{
  bool bEnabled = true;

  if( m_bInProgress                   ||  m_pTrack == NULL                 || !m_pTrack->PointCount       ||
     !HDOPIncr->Text.Length()         || !HDOPStep->Text.Length()          || !HDOPLevel->Text.Length()   ||
     !SpeedIncr->Text.Length()        || !SpeedStep->Text.Length()         || !SpeedLevel->Text.Length()  ||
     !CourseIncr->Text.Length()       || !CourseStep->Text.Length()        || !CourseLevel->Text.Length() ||
     !AccIncr->Text.Length()          || !AccStep->Text.Length()           || !AccLevel->Text.Length()    ||
    (!SmoothCoordinates->Checked      && !SmoothAltitude->Checked          && !SmoothSpeed->Checked)      ||
     !BaseSmoothFactor->Text.Length() || BaseSmoothFactor->Text.ToDouble() <= 0.0                         || 
    (HDOPIncr->Text.ToDouble() == 0.0 && SpeedIncr->Text.ToDouble() == 0.0 && CourseIncr->Text.ToDouble() == 0.0 && AccIncr->Text.ToDouble() == 0.0))
    bEnabled = false;

  StartAction->Enabled = bEnabled;
}
//---------------------------------------------------------------------------


void __fastcall TSplineSmoothForm::LoadParams()
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_dBaseSmoothFactor = pini->ReadFloat("SplineSmooth", "BaseSmoothFactor", 0.1);
  m_dHDOPIncr = pini->ReadFloat("SplineSmooth", "HDOPIncrement", 2.5);
  m_dHDOPStep = pini->ReadFloat("SplineSmooth", "HDOPStep", 0.2);
  m_dHDOPLevel = pini->ReadFloat("SplineSmooth", "HDOPThereshould", 1.5);
  m_dSpeedIncr = pini->ReadFloat("SplineSmooth", "SpeedIncrement", 2.5);
  m_dSpeedStep = pini->ReadFloat("SplineSmooth", "SpeedStep", 1);
  m_dSpeedLevel = pini->ReadFloat("SplineSmooth", "SpeedThereshould", 5);
  m_dCourseIncr = pini->ReadFloat("SplineSmooth", "CourseIncrement", 2.5);
  m_dCourseStep = pini->ReadFloat("SplineSmooth", "CourseStep", 15);
  m_dCourseLevel = pini->ReadFloat("SplineSmooth", "CourseThereshould", 30);
  m_bInstantCourse = pini->ReadBool("SplineSmooth", "InstantCourse", true);
  m_dAccIncr = pini->ReadFloat("SplineSmooth", "AccelerationIncrement", 2.5);
  m_dAccStep = pini->ReadFloat("SplineSmooth", "AccelerationStep", 5);
  m_dAccLevel = pini->ReadFloat("SplineSmooth", "AccelerationThereshould", 5);
  m_bInstatntSpeed = pini->ReadBool("SplineSmooth", "InstantSpeed", true);
  m_bSmoothCoordinates = pini->ReadBool("SplineSmooth", "SmoothCoordinates", true);
  m_bSmoothAltitude = pini->ReadBool("SplineSmooth", "SmoothAltitude", false);
  m_bSmoothSpeed = pini->ReadBool("SplineSmooth", "SmoothSpeed", false);
  delete pini;

  BaseSmoothFactor->Text = AnsiString(m_dBaseSmoothFactor);
  HDOPIncr->Text = AnsiString(m_dHDOPIncr);
  HDOPStep->Text = AnsiString(m_dHDOPStep);
  HDOPLevel->Text = AnsiString(m_dHDOPLevel);
  SpeedIncr->Text = AnsiString(m_dSpeedIncr);
  SpeedStep->Text = AnsiString(m_dSpeedStep);
  SpeedLevel->Text = AnsiString(m_dSpeedLevel);
  CourseIncr->Text = AnsiString(m_dCourseIncr);
  CourseStep->Text = AnsiString(m_dCourseStep);
  CourseLevel->Text = AnsiString(m_dCourseLevel);
  InstantCourse->Checked = m_bInstantCourse;
  AccIncr->Text = AnsiString(m_dAccIncr);
  AccStep->Text = AnsiString(m_dAccStep);
  AccLevel->Text = AnsiString(m_dAccLevel);
  InstatntSpeed->Checked = m_bInstatntSpeed;
  SmoothCoordinates->Checked = m_bSmoothCoordinates;
  SmoothAltitude->Checked = m_bSmoothAltitude;
  SmoothSpeed->Checked = m_bSmoothSpeed;

  m_dSpeedStep /= 3.6;
  m_dSpeedLevel /= 3.6;
  m_dAccStep /= 3.6;
  m_dAccLevel /= 3.6;
}

void __fastcall TSplineSmoothForm::SaveParams()
{
  m_dBaseSmoothFactor = BaseSmoothFactor->Text.ToDouble();
  m_dHDOPIncr = HDOPIncr->Text.ToDouble();
  m_dHDOPStep = HDOPStep->Text.ToDouble();
  m_dHDOPLevel = HDOPLevel->Text.ToDouble();
  m_dSpeedIncr = SpeedIncr->Text.ToDouble();
  m_dSpeedStep = SpeedStep->Text.ToDouble();
  m_dSpeedLevel = SpeedLevel->Text.ToDouble();
  m_dCourseIncr = CourseIncr->Text.ToDouble();
  m_dCourseStep = CourseStep->Text.ToDouble();
  m_dCourseLevel = CourseLevel->Text.ToDouble();
  m_bInstantCourse = InstantCourse->Checked;
  m_dAccIncr = AccIncr->Text.ToDouble();
  m_dAccStep = AccStep->Text.ToDouble();
  m_dAccLevel = AccLevel->Text.ToDouble();
  m_bInstatntSpeed = InstatntSpeed->Checked;
  m_bSmoothCoordinates = SmoothCoordinates->Checked;
  m_bSmoothAltitude = SmoothAltitude->Checked;
  m_bSmoothSpeed = SmoothSpeed->Checked;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  pini->WriteFloat("SplineSmooth", "BaseSmoothFactor", m_dBaseSmoothFactor);
  pini->WriteFloat("SplineSmooth", "HDOPIncrement", m_dHDOPIncr);
  pini->WriteFloat("SplineSmooth", "HDOPStep", m_dHDOPStep);
  pini->WriteFloat("SplineSmooth", "HDOPThereshould", m_dHDOPLevel);
  pini->WriteFloat("SplineSmooth", "SpeedIncrement", m_dSpeedIncr);
  pini->WriteFloat("SplineSmooth", "SpeedStep", m_dSpeedStep);
  pini->WriteFloat("SplineSmooth", "SpeedThereshould", m_dSpeedLevel);
  pini->WriteFloat("SplineSmooth", "CourseIncrement", m_dCourseIncr);
  pini->WriteFloat("SplineSmooth", "CourseStep", m_dCourseStep);
  pini->WriteFloat("SplineSmooth", "CourseThereshould", m_dCourseLevel);
  pini->WriteBool("SplineSmooth", "InstantCourse", m_bInstantCourse);
  pini->WriteFloat("SplineSmooth", "AccelerationIncrement", m_dAccIncr);
  pini->WriteFloat("SplineSmooth", "AccelerationStep", m_dAccStep);
  pini->WriteFloat("SplineSmooth", "AccelerationThereshould", m_dAccLevel);
  pini->WriteBool("SplineSmooth", "InstantSpeed", m_bInstatntSpeed);
  pini->WriteBool("SplineSmooth", "SmoothCoordinates", m_bSmoothCoordinates);
  pini->WriteBool("SplineSmooth", "SmoothAltitude", m_bSmoothAltitude);
  pini->WriteBool("SplineSmooth", "SmoothSpeed", m_bSmoothSpeed);
  delete pini;

  m_dSpeedStep /= 3.6;
  m_dSpeedLevel /= 3.6;
  m_dAccStep /= 3.6;
  m_dAccLevel /= 3.6;
}

void __fastcall TSplineSmoothForm::FormCreate(TObject *Sender)
{
  LoadParams();
  ProgressBar->Max = 100;
  ProgressBar->Position = 0;
}
//---------------------------------------------------------------------------

void __fastcall TSplineSmoothForm::SetTrack(PGPXTrack value)
{
  if(m_pTrack != value) {
    m_pTrack = value;

    if(m_pTrack)
    {
      ProgressBar->Max = m_pTrack->PointCount * 2;
      ProgressBar->Position = 0;
      m_pFilteredTrack = NULL;

      if(m_pTrack->m_bNoHDOP)
      {
        HDOPIncr->Enabled = false;
        HDOPStep->Enabled = false;
        HDOPLevel->Enabled = false;
      }
      else
      {
        HDOPIncr->Enabled = true;
        HDOPStep->Enabled = true;
        HDOPLevel->Enabled = true;
      }

      if(m_pTrack->m_bNoAlt)
        SmoothAltitude->Enabled = false;
      else
        SmoothAltitude->Enabled = true;
    }
  }
}
PGPXTrack __fastcall TSplineSmoothForm::GetTrack()
{
  return m_pFilteredTrack;
}



