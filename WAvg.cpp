//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "GPXFile.h"
#include "SK42WGS84\\SK42WGS84.h"
#include "WAvg.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall WheitedAverage::WheitedAverage(HWND hParentWnd)
{
  m_hParentWnd = hParentWnd;
  m_pCurrentTrack = m_pAvgTrack = NULL;
  //m_pSplineX = m_pSplineY = m_pSplineA = m_pSplineV = m_pSplineC = NULL;
  m_pSplineX = m_pSplineY = m_pSplineA = m_pSplineVx = m_pSplineVy = NULL;
  m_dDistTh = 10.0;
  m_dTimeTh = 2.0;
  m_dSpeedTh = 1.0;
  m_dCourceTh = 30.0;
  m_dAvgTimeTh = 25.0;
  m_dSmooth = 0.25;
  m_bAltSmooth = false;
  m_bSpeedSmooth = false;
}

__fastcall WheitedAverage::~WheitedAverage()
{
  if(m_pAvgTrack) delete m_pAvgTrack;
  if(m_pSplineX) delete m_pSplineX;
  if(m_pSplineY) delete m_pSplineY;
  if(m_pSplineA) delete m_pSplineA;
  if(m_pSplineVx) delete m_pSplineVx;
  if(m_pSplineVy) delete m_pSplineVy;
  //if(m_pSplineV) delete m_pSplineV;
  //if(m_pSplineC) delete m_pSplineC;
}

PGPXTrack __fastcall WheitedAverage::GetAvgTrack()
{
  return m_pAvgTrack;
}

int __fastcall WheitedAverage::GetAvgTrackPoints()
{
  return m_pAvgTrack->PointCount;
}

void __fastcall WheitedAverage::SetCurrentTrack(PGPXTrack value)
{
  if(m_pCurrentTrack != value) {
    m_pCurrentTrack = value;

    if(m_pAvgTrack) delete m_pAvgTrack;
    m_pAvgTrack = new GPXTrack;
  }
}

void __fastcall WheitedAverage::SetDistTh(double value)
{
  if(m_dDistTh != value) {
    m_dDistTh = value;
  }
}
double __fastcall WheitedAverage::GetDistTh()
{
  return m_dDistTh;
}

void __fastcall WheitedAverage::SetTimeTh(double value)
{
  if(m_dTimeTh != value) {
    m_dTimeTh = value;
  }
}
double __fastcall WheitedAverage::GetTimeTh()
{
  return m_dTimeTh;
}

void __fastcall WheitedAverage::SetSpeedTh(double value)
{
  if(m_dSpeedTh != value) {
    m_dSpeedTh = value;
  }
}
double __fastcall WheitedAverage::GetSpeedTh()
{
  return m_dSpeedTh;
}

void __fastcall WheitedAverage::SetCourceTh(double value)
{
  if(m_dCourceTh != value) {
    m_dCourceTh = value;
  }
}
double __fastcall WheitedAverage::GetCourceTh()
{
  return m_dCourceTh;
}

int __fastcall WheitedAverage::BuildAvgTrack()
{
  if(m_pCurrentTrack)
  {
    PGPXTrackPoint pNewPoint = m_pAvgTrack->NewPoint, pi, pj;
    pi = m_pCurrentTrack->Points[0];
    pNewPoint->SetPoint(pi);
    m_pAvgTrack->AddPoint(pNewPoint);
    PGPXTrackPoint pLastPoint = pNewPoint;
    int i = 0;

    while(i < m_pCurrentTrack->PointCount - 1)
    {
      pNewPoint = m_pAvgTrack->NewPoint;
      pNewPoint->m_dLat = pi->m_dLat;
      pNewPoint->m_dLon = pi->m_dLon;
      pNewPoint->m_dSpeed = pi->m_dSpeed;
      pNewPoint->m_dCourse = pi->m_dCourse;
      if(!m_pCurrentTrack->m_bNoAlt) pNewPoint->m_dAlt = pi->m_dAlt;
      pNewPoint->m_dTime = pi->m_dTime;

      for(int j = i + 1; j < m_pCurrentTrack->PointCount; j++)
      {
        pj = m_pCurrentTrack->Points[j];
        Add2Avg(pNewPoint, pj);

        double dist = m_pCurrentTrack->GetDistance(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                                   pNewPoint->m_dLat, pNewPoint->m_dLon);
        double dtime = pNewPoint->m_dTime - pLastPoint->m_dTime;

        if(pj->m_dSpeed * 3.6 < m_dSpeedTh)
        {
          if(dtime >= m_dAvgTimeTh) break;
        }
        else
        {
          if(dist >= m_dDistTh) break;
        }

        //Add averaged point if it's distanse from previous one
        //is more than thereshould
        //if(dist >= m_dDistTh) break;
        //or if sinse last point past some time
        //and current speed exeeds the thereshould
        //and azimuth changed more than thereshould
        //(turn detection)
        if(m_pAvgTrack->PointCount > 1)
        {
          if(dtime >= m_dTimeTh)
          {
            double speed = 3.6 * dist / dtime;

            if(speed >= m_dSpeedTh)
            {
              double Az1 = m_pAvgTrack->GetCource(m_pAvgTrack->PointCount - 2,
                                                  m_pAvgTrack->PointCount - 1);
              double Az2 = m_pAvgTrack->GetCource(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                                  pNewPoint->m_dLat, pNewPoint->m_dLon);
              double dAz = fabs(Az2 - Az1);
              if(dAz > 180) dAz = 360 - dAz;

              if(dAz >= m_dCourceTh) break;
            }
          }
        }
      }

      if(pLastPoint->m_dTime < pNewPoint->m_dTime)
      {
        m_pAvgTrack->AddPoint(pNewPoint);
        pLastPoint = pNewPoint;
      }
      else delete pNewPoint;

      //Next averaging period starts from the point next (by time) to last added
      while(pi->m_dTime <= pLastPoint->m_dTime)
      {
        i++;
        if(i < m_pCurrentTrack->PointCount) pi = m_pCurrentTrack->Points[i];
        else break;
      }

      if(m_hParentWnd)
      {
        SendMessage(m_hParentWnd, WAVGFILTER_POINT, i, 0);
        Application->ProcessMessages();
      }
    }
  }

  return m_pAvgTrack->PointCount;
}

void __fastcall WheitedAverage::Interpolate()
{
  //if(m_pCurrentTrack && m_pSplineX && m_pSplineY && m_pSplineV && m_pSplineC)
  if(m_pCurrentTrack && m_pSplineX && m_pSplineY && m_pSplineVx && m_pSplineVy)
  {
    for(int i = 1; i < m_pCurrentTrack->PointCount; i++)
    {
      double inX2, inY2;
      double outX1, outX2, outY1, outY2;
      //double outV1, outC1, outA1;
      double outVx1, outVy1, outA1;
      double inVx2, inVy2;
      double inV2, inC2;

      PGPXTrackPoint inP2 = m_pCurrentTrack->Points[i];
      PGPXTrackPoint outP1 = m_pCurrentTrack->Points[i - 1];

      inP2->GetdProj(&inX2, &inY2);
      m_pSplineX->Interpolate(outP1->m_dTime, outX1);
      m_pSplineY->Interpolate(outP1->m_dTime, outY1);
      outX2 = outX1 + m_dSmooth * (inX2 - outX1);
      outY2 = outY1 + m_dSmooth * (inY2 - outY1);

      inP2->SetdProj(outX2, outY2);

      if(!m_pCurrentTrack->m_bNoAlt && m_bAltSmooth)
      {
        m_pSplineA->Interpolate(outP1->m_dTime, outA1);
        inP2->m_dAlt = outA1 + m_dSmooth * (inP2->m_dAlt - outA1);
      }

      if(m_bSpeedSmooth)
      {
        m_pSplineVx->Interpolate(outP1->m_dTime, outVx1);
        m_pSplineVy->Interpolate(outP1->m_dTime, outVy1);

        DecomposeSpeed(inP2->m_dSpeed, inP2->m_dCourse, &inVx2, &inVy2);
        inVx2 = outVx1 + m_dSmooth * (inVx2 - outVx1);
        inVy2 = outVy1 + m_dSmooth * (inVy2 - outVy1);

        ComposeSpeed(inVx2, inVy2, &inV2, &inC2);
        inP2->m_dSpeed = inV2; inP2->m_dCourse = inC2;

        /*
        m_pSplineV->Interpolate(outP1->m_dTime, outV1);
        inP2->m_dSpeed = outV1 + m_dSmooth * (inP2->m_dSpeed - outV1);
        if(inP2->m_dSpeed < 0.0) inP2->m_dSpeed = 0.0;

        m_pSplineC->Interpolate(outP1->m_dTime, outC1);
        if(outC1 < 0.0) outC1 += 360.0;
        else if(outC1 > 360.0) outC1 -= 360.0;
        double dC = fabs(inP2->m_dCourse - outC1);
        if(dC > 180.0) dC = 360.0 - dC;
        if(dC > 360.0) dC -= 360.0;
        inP2->m_dCourse = outC1 + m_dSmooth * dC;
        if(inP2->m_dCourse < 0.0) inP2->m_dCourse += 360.0;
        else if(inP2->m_dCourse > 360.0) inP2->m_dCourse -= 360.0;
        */
      }

      if(m_hParentWnd)
      {
        SendMessage(m_hParentWnd, WAVGFILTER_POINT, i, 0);
        Application->ProcessMessages();
      }
    }
  }
}

void __fastcall WheitedAverage::BuildSplines()
{
  if(m_pAvgTrack && m_pCurrentTrack)
  {
    /*
    GPXFile *pFile = new GPXFile("wa.gpx");
    pFile->AddTrack(m_pAvgTrack);
    pFile->Save();
    delete pFile;
    */
    PGPXTrackPoint pi;
    //double *X, *Y, *A, *V, *C, *T;
    double *X, *Y, *A, *Vx, *Vy, *T;
    X = new double[m_pAvgTrack->PointCount];
    Y = new double[m_pAvgTrack->PointCount];
    Vx = new double[m_pAvgTrack->PointCount];
    Vy = new double[m_pAvgTrack->PointCount];
    //V = new double[m_pAvgTrack->PointCount];
    //C = new double[m_pAvgTrack->PointCount];
    T = new double[m_pAvgTrack->PointCount];
    if(!m_pCurrentTrack->m_bNoAlt) A = new double[m_pAvgTrack->PointCount];

    for(int i = 0; i < m_pAvgTrack->PointCount; i++)
    {
      pi = m_pAvgTrack->Points[i];
      T[i] = pi->m_dTime;
      m_pCurrentTrack->GetProj(&X[i], &Y[i], pi->m_dLat, pi->m_dLon);
      DecomposeSpeed(pi->m_dSpeed, pi->m_dCourse, &Vx[i], &Vy[i]);
      //V[i] = pi->m_dSpeed;
      //C[i] = pi->m_dCourse;
      if(!m_pCurrentTrack->m_bNoAlt) A[i] = pi->m_dAlt;

      if(m_hParentWnd)
      {
        SendMessage(m_hParentWnd, WAVGFILTER_POINT, i, 0);
        Application->ProcessMessages();
      }
    }

    if(m_pSplineX) delete m_pSplineX;
    if(m_pSplineY) delete m_pSplineY;
    if(m_pSplineA) delete m_pSplineA;
    if(m_pSplineVx) delete m_pSplineVx;
    if(m_pSplineVy) delete m_pSplineVy;
    //if(m_pSplineV) delete m_pSplineV;
    //if(m_pSplineC) delete m_pSplineC;

    m_pSplineX = new AkimaSpline(T, X, m_pAvgTrack->PointCount);
    m_pSplineY = new AkimaSpline(T, Y, m_pAvgTrack->PointCount);
    m_pSplineVx = new AkimaSpline(T, Vx, m_pAvgTrack->PointCount);
    m_pSplineVy = new AkimaSpline(T, Vy, m_pAvgTrack->PointCount);
    //m_pSplineV = new AkimaSpline(T, V, m_pAvgTrack->PointCount);
    //m_pSplineC = new AkimaSpline(T, C, m_pAvgTrack->PointCount);
    if(!m_pCurrentTrack->m_bNoAlt) m_pSplineA = new AkimaSpline(T, A, m_pAvgTrack->PointCount);
    else m_pSplineA = NULL;

    delete [] X;
    delete [] Y;
    delete [] Vx;
    delete [] Vy;
    //delete [] V;
    //delete [] C;
    delete [] T;
    if(!m_pCurrentTrack->m_bNoAlt) delete [] A;
  }
}

void __fastcall WheitedAverage::Add2Avg(PGPXTrackPoint pNewPoint, PGPXTrackPoint pj)
{
  pNewPoint->m_dLat = (pNewPoint->m_dLat + pj->m_dLat) / 2.0;
  pNewPoint->m_dLon = (pNewPoint->m_dLon + pj->m_dLon) / 2.0;

  if(!m_pCurrentTrack->m_bNoAlt && m_bAltSmooth)
    pNewPoint->m_dAlt = (pNewPoint->m_dAlt + pj->m_dAlt) / 2.0;

  pNewPoint->m_dTime = (pNewPoint->m_dTime + pj->m_dTime) / 2.0;

  if(m_bSpeedSmooth)
  {
    double dVx1, dVx2, dVy1, dVy2;
    double dV, dC;

    DecomposeSpeed(pNewPoint->m_dSpeed, pNewPoint->m_dCourse, &dVx1, &dVy1);
    DecomposeSpeed(pj->m_dSpeed, pj->m_dCourse, &dVx2, &dVy2);

    dVx1 = (dVx1 + dVx2) / 2.0;
    dVy1 = (dVy1 + dVy2) / 2.0;

    ComposeSpeed(dVx1, dVy1, &dV, &dC);

    pNewPoint->m_dSpeed = dV;
    pNewPoint->m_dCourse = dC;
  }

  /*
  pNewPoint->m_dSpeed = (pNewPoint->m_dSpeed + pj->m_dSpeed) / 2.0;

  double c1 = pNewPoint->m_dCourse, c2 = pj->m_dCourse;

  if(fabs(c1 - c2) > 180.0)
  {
    if(c1 < 180.0) c1 += 360.0;
    else if(c2 < 180.0) c2 += 360.0;
  }

  pNewPoint->m_dCourse = (c1 + c2) / 2.0;
  if(pNewPoint->m_dCourse >= 360.0) pNewPoint->m_dCourse -= 360.0;
  */
}

void __fastcall WheitedAverage::SetAvgTime(double value)
{
  if(m_dAvgTimeTh != value) {
    m_dAvgTimeTh = value;
  }
}
double __fastcall WheitedAverage::GetAvgTime()
{
  return m_dAvgTimeTh;
}

void __fastcall WheitedAverage::SetSmoothFactor(double value)
{
  if(m_dSmooth != value) {
    m_dSmooth = value;
  }
}
double __fastcall WheitedAverage::GetSmoothFactor()
{
  return m_dSmooth;
}

void __fastcall WheitedAverage::SetAltSmooth(bool value)
{
  if(m_bAltSmooth != value) {
    m_bAltSmooth = value;
  }
}

void __fastcall WheitedAverage::SetSpeedSmooth(bool value)
{
  if(m_bSpeedSmooth != value) {
    m_bSpeedSmooth = value;
  }
}
