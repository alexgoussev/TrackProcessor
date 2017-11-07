//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "GPXData.h"
#include "SK42WGS84\\SK42WGS84.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall GPXTrackPoint::GPXTrackPoint(PGPXTrack pTrack)
{
  m_pTrack = pTrack;

  m_dLat = GPS_PT_NO_VALUE;
  m_dLon = GPS_PT_NO_VALUE;
  m_dTime = GPS_PT_NO_VALUE;
  m_dCourse = GPS_PT_NO_VALUE;
  m_dSpeed = GPS_PT_NO_VALUE;
  m_dDistance = GPS_PT_NO_VALUE;
  m_dSpacing = GPS_PT_NO_VALUE;
  m_dAlt = GPS_PT_NO_VALUE;
  m_dHDOP = GPS_PT_NO_VALUE;
  m_dPDOP = GPS_PT_NO_VALUE;
  m_dVDOP = GPS_PT_NO_VALUE;
  m_dInterval = GPS_PT_NO_VALUE;
  m_dElapsed = GPS_PT_NO_VALUE;
  m_dClimb = GPS_PT_NO_VALUE;
  m_dMagvar = GPS_PT_NO_VALUE;
  m_dGeoidHeigtht = GPS_PT_NO_VALUE;
  m_asDesc = AnsiString("");
  m_nFix = -1;
  m_nSats = -1;
  m_bActive = true;
}

__fastcall GPXTrackPoint::~GPXTrackPoint()
{
  //TODO: Add your source code here
}

void __fastcall GPXTrackPoint::SetdProj(double x, double y)
{
  //UTMToWGS84(x, y, ((m_pTrack->m_dMinLon + m_pTrack->m_dMaxLon) / 2.0), &m_dLat, &m_dLon);
  m_dLon = MercatorX2Lon(y);
  m_dLat = MercatorY2Lat(x);

  if(m_pTrack)
  {
    if(m_pTrack->m_dMinLat > m_dLat) m_pTrack->m_dMinLat = m_dLat;
    if(m_pTrack->m_dMaxLat < m_dLat) m_pTrack->m_dMaxLat = m_dLat;
    if(m_pTrack->m_dMinLon > m_dLon) m_pTrack->m_dMinLon = m_dLon;
    if(m_pTrack->m_dMaxLon < m_dLon) m_pTrack->m_dMaxLon = m_dLon;
  }
}

void __fastcall GPXTrackPoint::GetdProj(double *px, double *py)
{
  //X - Lat, Y - Lon
  *py = Lon2MercatorX(m_dLon);
  *px = Lat2MercatorY(m_dLat);
}

AnsiString __fastcall GPXTrackPoint::GetTimeString()
{
  char str[64];

  if(m_dTime == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    DWORD dwTime = (DWORD)m_dTime;
    int nmSec = (int)((m_dTime - (double)dwTime) * 1000.0);

    dwTime += _timezone;

    struct date d;
    struct time t;

    unixtodos(dwTime, &d, &t);

    sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            d.da_year, d.da_mon, d.da_day, t.ti_hour, t.ti_min, t.ti_sec, nmSec);
  }
  
  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXTimeString()
{
  char str[64];

  if(m_dTime == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    DWORD dwTime = (DWORD)m_dTime;
    int nmSec = (int)((m_dTime - (double)dwTime) * 1000.0);

    dwTime += _timezone;

    struct date d;
    struct time t;

    unixtodos(dwTime, &d, &t);

    sprintf(str, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ",
            d.da_year, d.da_mon, d.da_day, t.ti_hour, t.ti_min, t.ti_sec, nmSec);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXEleString()
{
  char str[64];

  if(m_dAlt == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double ele = (m_dAlt > 0.0) ? ((double)((int)((m_dAlt * 10.0) + 0.5))) / 10.0 :
                                  ((double)((int)((m_dAlt * 10.0) - 0.5))) / 10.0;

    sprintf(str, "%.1lf", ele);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXLatString()
{
  char str[64];

  if(m_dLat == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double lat = (m_dLat > 0.0) ? ((double)((int)((m_dLat * 1000000.0) + 0.5))) / 1000000.0 :
                                  ((double)((int)((m_dLat * 1000000.0) - 0.5))) / 1000000.0;
    sprintf(str, "%.6lf", lat);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXLonString()
{
  char str[64];

  if(m_dLon == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double lon = (m_dLon > 0) ? ((double)((int)((m_dLon * 1000000.0) + 0.5))) / 1000000.0 :
                                ((double)((int)((m_dLon * 1000000.0) - 0.5))) / 1000000.0;
    sprintf(str, "%.6lf", lon);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXHDOPString()
{
  char str[64];

  if(m_dHDOP == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double hdop = ((double)((int)((m_dHDOP * 100.0) + 0.5))) / 100.0;
    sprintf(str, "%.2lf", hdop);
  }
  
  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXPDOPString()
{
  char str[64];

  if(m_dPDOP == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double pdop = ((double)((int)((m_dPDOP * 100.0) + 0.5))) / 100.0;
    sprintf(str, "%.2lf", pdop);
  }
  
  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXVDOPString()
{
  char str[64];

  if(m_dVDOP == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double vdop = ((double)((int)((m_dVDOP * 100.0) + 0.5))) / 100.0;
    sprintf(str, "%.2lf", vdop);
  }
  
  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXSpeedString()
{
  char str[64];

  if(m_dSpeed == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double speed = ((double)((int)((m_dSpeed * 36.0) + 0.5))) / 10.0;
    sprintf(str, "%.1lf", speed);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXCourseString()
{
  char str[64];

  if(m_dCourse == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double course = ((double)((int)((m_dCourse * 10.0) + 0.5))) / 10.0;
    sprintf(str, "%.1lf", course);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXDescString()
{
  AnsiString as;
  
  if(!m_asDesc.Length())
  {
  /*
  <desc>Lat = 56.850955, Lon = 60.384995, Alt = 262.0m, Speed = 0.5km/h, Course = 200.3</desc>
  */

    char str[128];
    sprintf(str, "Lat = %s, Lon = %s, Alt = %sm, Speed = %skm/h, Course = %s",
            GPXLatString.c_str(), GPXLonString.c_str(), GPXEleString.c_str(),
            GPXSpeedString.c_str(), GPXCourseString.c_str());
    as = AnsiString(str);
  }
  else as = m_asDesc;

  return as;
}

AnsiString __fastcall GPXTrackPoint::GetGPXClimbString()
{
  char str[64];

  if(m_dClimb == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double climb = ((double)((int)((fabs(m_dClimb) * 10.0) + 0.5))) / 10.0;
    if(m_dClimb < 0.0) climb = -climb;
    sprintf(str, "%.1lf", climb);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetLatDMSString()
{
  return m_pTrack->FormatCoordString(m_dLat, eLat, eDMS);
}

AnsiString __fastcall GPXTrackPoint::GetLonDMSString()
{
  return m_pTrack->FormatCoordString(m_dLon, eLon, eDMS);
}

AnsiString __fastcall GPXTrackPoint::GetIntervalString()
{
  char str[64];

  if(m_dInterval == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
    sprintf(str, "%.3lf", m_dInterval);

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetElapsedString()
{
  char str[64];

  if(m_dElapsed == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    DWORD dwElapsed = (DWORD)m_dElapsed;
    int nmSec = (int)((m_dElapsed - (double)dwElapsed) * 1000.0);

    int nHours = dwElapsed / 3600;
    dwElapsed %= 3600;
    int nMinutes = dwElapsed / 60;
    int nSeconds = dwElapsed % 60;

    sprintf(str, "%d:%02d:%02d.%03d", nHours, nMinutes, nSeconds, nmSec);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetSpacingString()
{
  char str[64];
  int nSpace = (int)(m_dSpacing + 0.5);
  sprintf(str, "%d", nSpace);
  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetDistanceString()
{
  char str[64];
  double dDist = m_dDistance / 10.0 + 0.5;
  dDist = ((double)((int)dDist)) / 100.0;
  sprintf(str, "%.2lf", dDist);
  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetLocalTimeString()
{
  char str[64];

  if(m_dTime == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    DWORD dwTime = (DWORD)m_dTime;
    int nmSec = (int)((m_dTime - (double)dwTime) * 1000.0);

    struct date d;
    struct time t;

    unixtodos(dwTime, &d, &t);

    sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            d.da_year, d.da_mon, d.da_day, t.ti_hour, t.ti_min, t.ti_sec, nmSec);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetLatD_DString()
{
  return m_pTrack->FormatCoordString(m_dLat, eLat, eD_D);
}

AnsiString __fastcall GPXTrackPoint::GetLonD_DString()
{
  return m_pTrack->FormatCoordString(m_dLon, eLon, eD_D);
}

void __fastcall GPXTrackPoint::SetPoint(PGPXTrackPoint pPoint)
{
  m_dLat = pPoint->m_dLat;
  m_dLon = pPoint->m_dLon;

  if(m_pTrack)
  {
    if(m_pTrack->m_dMinLat > m_dLat) m_pTrack->m_dMinLat = m_dLat;
    if(m_pTrack->m_dMaxLat < m_dLat) m_pTrack->m_dMaxLat = m_dLat;
    if(m_pTrack->m_dMinLon > m_dLon) m_pTrack->m_dMinLon = m_dLon;
    if(m_pTrack->m_dMaxLon < m_dLon) m_pTrack->m_dMaxLon = m_dLon;
  }

  m_dTime = pPoint->m_dTime;
  m_dCourse = pPoint->m_dCourse;
  m_dSpeed = pPoint->m_dSpeed;
  m_dDistance = pPoint->m_dDistance;
  m_dSpacing = pPoint->m_dSpacing;
  m_dAlt = pPoint->m_dAlt;
  m_dHDOP = pPoint->m_dHDOP;
  m_dPDOP = pPoint->m_dPDOP;
  m_dVDOP = pPoint->m_dVDOP;
  m_dInterval = pPoint->m_dInterval;
  m_dElapsed = pPoint->m_dElapsed;
  m_dClimb = pPoint->m_dClimb;
  m_asDesc = pPoint->m_asDesc;
  m_bActive = pPoint->m_bActive;
  m_dMagvar = pPoint->m_dMagvar;
  m_dGeoidHeigtht = pPoint->m_dGeoidHeigtht;
  m_nSats = pPoint->m_nSats;
  m_nFix = pPoint->m_nFix;
}

//---------------------------------------------------------------------------

__fastcall GPXTrack::GPXTrack()
{
  m_dMinLat = 1000.0;
  m_dMinLon = 1000.0;
  m_dMaxLat = -1000.0;
  m_dMaxLon = -1000.0;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_dSpeedAveragingTime = pini->ReadFloat("GPXFile", "AvgTime", 5.0);
  m_dMinClimbDAlt = pini->ReadFloat("GPXFile", "MinClimbAltDiff", 1.0);
  m_dMinClimbDist = pini->ReadFloat("GPXFile", "MinClimbDist", 5.0);
  m_dMinClimbSpeed = pini->ReadFloat("GPXFile", "MinClimbSpeed", 1.0);
  delete pini;

  m_asName = AnsiString("");
  m_asDesc = AnsiString("");
  m_asSrc = AnsiString("");
  m_asCmt = AnsiString("");
  m_asFileName = AnsiString("");

  m_bCourseCalculated = false;
  m_bSpeedCalculated = false;
  m_bNoTime = false;
  m_bNoAlt = false;
  m_bNoHDOP = false;
  m_bNoPDOP = false;
  m_bNoVDOP = false;
  
  m_pPoints = new TList;
}

__fastcall GPXTrack::GPXTrack(PGPXTrack pTrack)
{
  m_dMinLat = 1000.0;
  m_dMinLon = 1000.0;
  m_dMaxLat = -1000.0;
  m_dMaxLon = -1000.0;
  m_dSpeedAveragingTime = pTrack->m_dSpeedAveragingTime;
  m_dMinClimbDAlt = pTrack->m_dMinClimbDAlt;
  m_dMinClimbDist = pTrack->m_dMinClimbDist;
  m_dMinClimbSpeed = pTrack->m_dMinClimbSpeed;

  m_asName = pTrack->m_asName;
  m_asDesc = pTrack->m_asDesc;
  m_asSrc = pTrack->m_asSrc;
  m_asCmt = pTrack->m_asCmt;
  m_asFileName = pTrack->m_asFileName;

  m_bCourseCalculated = pTrack->m_bCourseCalculated;
  m_bSpeedCalculated = pTrack->m_bSpeedCalculated;
  m_bNoTime = pTrack->m_bNoTime;
  m_bNoAlt = pTrack->m_bNoAlt;
  m_bNoHDOP = pTrack->m_bNoHDOP;
  m_bNoPDOP = pTrack->m_bNoPDOP;
  m_bNoVDOP = pTrack->m_bNoVDOP;
  
  m_pPoints = new TList;

  for(int i = 0; i < pTrack->PointCount; i++)
  {
    PGPXTrackPoint pNewPoint = NewPoint;
    pNewPoint->SetPoint(pTrack->Points[i]);
    AddPoint(pNewPoint);
  }
}

__fastcall GPXTrack::~GPXTrack()
{
  while(PointCount) DeletePoint(0);
  delete m_pPoints;
}

void __fastcall GPXTrack::AddPoint(PGPXTrackPoint pPoint)
{
  bool bIgnoreDup = false;

  if(m_dMinLat > pPoint->m_dLat) m_dMinLat = pPoint->m_dLat;
  if(m_dMaxLat < pPoint->m_dLat) m_dMaxLat = pPoint->m_dLat;
  if(m_dMinLon > pPoint->m_dLon) m_dMinLon = pPoint->m_dLon;
  if(m_dMaxLon < pPoint->m_dLon) m_dMaxLon = pPoint->m_dLon;

  if(pPoint->m_dAlt == GPS_PT_NO_VALUE) m_bNoAlt = true;
  if(pPoint->m_dHDOP == GPS_PT_NO_VALUE) m_bNoHDOP = true;
  if(pPoint->m_dPDOP == GPS_PT_NO_VALUE) m_bNoPDOP = true;
  if(pPoint->m_dVDOP == GPS_PT_NO_VALUE) m_bNoVDOP = true;
  if(pPoint->m_dCourse > 360 || pPoint->m_dCourse < 0) pPoint->m_dCourse = GPS_PT_NO_VALUE;
  if(pPoint->m_dSpeed < 0) pPoint->m_dSpeed = GPS_PT_NO_VALUE;

  if(PointCount)
  {
    PGPXTrackPoint pLastPoint = Points[PointCount - 1];

    if(pPoint->m_dTime == GPS_PT_NO_VALUE ||
       pPoint->m_dTime < pLastPoint->m_dTime)
      m_bNoTime = true;
    else if(pPoint->m_dTime <= pLastPoint->m_dTime)
      bIgnoreDup = true;
    else
    {
      pPoint->m_dInterval = pPoint->m_dTime - pLastPoint->m_dTime;
      pPoint->m_dElapsed = pLastPoint->m_dElapsed + pPoint->m_dInterval;
    }

    if(!bIgnoreDup)
    {
      pPoint->m_dSpacing = DistanceLL(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                      pPoint->m_dLat, pPoint->m_dLon);
      pPoint->m_dDistance = pLastPoint->m_dDistance + pPoint->m_dSpacing;

  /*** Debug ***

      double bearing = AzimuthLL(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                 pPoint->m_dLat, pPoint->m_dLon);
      FILE *f = fopen("debug.csv", "a+");
      fprintf(f, "%s;%.3lf;%.1lf;%.1lf\r",
              pPoint->TimeString.c_str(), pPoint->m_dTime, bearing, pPoint->m_dSpacing / pPoint->m_dInterval);
      fclose(f);


  ***  End  ***/

      if(!m_bNoTime)
      {
        for(int i = PointCount - 1; i >= 0; i--)
        {
          pLastPoint = Points[i];

          if((pPoint->m_dTime - pLastPoint->m_dTime) >= m_dSpeedAveragingTime)
            break;
        }

        if(pPoint->m_dCourse == GPS_PT_NO_VALUE)
        {
          pPoint->m_dCourse = AzimuthLL(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                        pPoint->m_dLat, pPoint->m_dLon);
          m_bCourseCalculated = true;
        }

        if(pPoint->m_dSpeed == GPS_PT_NO_VALUE)
        {
          pPoint->m_dSpeed = DistanceLL(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                        pPoint->m_dLat, pPoint->m_dLon) /
                             (pPoint->m_dTime - pLastPoint->m_dTime);
          m_bSpeedCalculated = true;
        }

        if(!m_bNoAlt)
        {
          double dist = DistanceLL(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                   pPoint->m_dLat, pPoint->m_dLon);
          double dalt = pLastPoint->m_dAlt - pPoint->m_dAlt;
          double dalta = fabs(dalt);

          if(dalta            > m_dMinClimbDAlt &&
             dist             > m_dMinClimbDist &&
             pPoint->m_dSpeed > m_dMinClimbSpeed)
          {
            pPoint->m_dClimb = atan2(dalta, dist) * 180.0 / M_PI;
            if(dalt > 0.0) pPoint->m_dClimb = -pPoint->m_dClimb;
          }
          else pPoint->m_dClimb = 0.0;
        }
      }
      else if(m_bNoTime && pPoint->m_dCourse == GPS_PT_NO_VALUE)
      {
        pPoint->m_dCourse = AzimuthLL(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                      pPoint->m_dLat, pPoint->m_dLon);
        m_bCourseCalculated = true;
      }
    }
  }
  else
  {
    pPoint->m_dSpacing = 0.0;
    pPoint->m_dDistance = 0.0;
    pPoint->m_dInterval = 0.0;
    pPoint->m_dElapsed = 0.0;
    pPoint->m_dClimb = 0.0;

    if(pPoint->m_dCourse == GPS_PT_NO_VALUE)
    {
      pPoint->m_dCourse = 0.0;
      m_bCourseCalculated = true;
    }

    if(pPoint->m_dSpeed == GPS_PT_NO_VALUE)
    {
      pPoint->m_dSpeed = 0.0;
      m_bSpeedCalculated = true;
    }
  }

  if(!bIgnoreDup) m_pPoints->Add(pPoint);
  else delete pPoint;
}

PGPXTrackPoint __fastcall GPXTrack::GetNewPoint()
{
  return new GPXTrackPoint(this);
}

void __fastcall GPXTrack::RecalcDistClimb()
{
  for(int i = 1; i < PointCount; i++)
  {
    PGPXTrackPoint pLastPoint = Points[i - 1];
    PGPXTrackPoint pPoint = Points[i];

    pPoint->m_dSpacing = DistanceLL(pLastPoint->m_dLat, pLastPoint->m_dLon,
                                    pPoint->m_dLat, pPoint->m_dLon);
    pPoint->m_dDistance = pLastPoint->m_dDistance + pPoint->m_dSpacing;

    if(!m_bNoTime && !m_bNoAlt)
    {
      for(int j = i - 1; j >= 0; j--)
      {
        pLastPoint = Points[j];

        if((pPoint->m_dTime - pLastPoint->m_dTime) >= m_dSpeedAveragingTime)
          break;
      }

      if(pLastPoint->m_dAlt == GPS_PT_NO_VALUE || pPoint->m_dAlt == GPS_PT_NO_VALUE)
        continue;
         
      double dist = DistanceLL(pLastPoint->m_dLat, pLastPoint->m_dLon,
                               pPoint->m_dLat, pPoint->m_dLon);
      double dalt = pLastPoint->m_dAlt - pPoint->m_dAlt;

      if(dalt != 0.0 && dist != 0.0)
        pPoint->m_dClimb = atan2(fabs(dalt), dist) * 180.0 / M_PI;
      else pPoint->m_dClimb = 0.0;
      
      if(dalt > 0.0) pPoint->m_dClimb = -pPoint->m_dClimb;
    }
  }
}

void __fastcall GPXTrack::DeletePoint(int i)
{
  PGPXTrackPoint pPoint = (PGPXTrackPoint)m_pPoints->Items[i];
  m_pPoints->Delete(i);
  delete pPoint;
}

PGPXTrackPoint __fastcall GPXTrack::GetPoint(int i)
{
  return (PGPXTrackPoint)m_pPoints->Items[i];
}

int __fastcall GPXTrack::GetPointCount()
{
  return m_pPoints->Count;
}

double __fastcall GPXTrack::GetDistance(double dLatA, double dLonA, double dLatB, double dLonB)
{
  return DistanceLL(dLatA, dLonA, dLatB, dLonB);
}

double __fastcall GPXTrack::GetCource(double dLatA, double dLonA, double dLatB, double dLonB)
{
  return AzimuthLL(dLatA, dLonA, dLatB, dLonB);
}

double __fastcall GPXTrack::GetDistance(int p1, int p2)
{
  PGPXTrackPoint pP1 = Points[p1], pP2 = Points[p2];
  return DistanceLL(pP1->m_dLat, pP1->m_dLon, pP2->m_dLat, pP2->m_dLon);
}

double __fastcall GPXTrack::GetCource(int p1, int p2)
{
  PGPXTrackPoint pP1 = Points[p1], pP2 = Points[p2];
  return AzimuthLL(pP1->m_dLat, pP1->m_dLon, pP2->m_dLat, pP2->m_dLon);
}

void __fastcall GPXTrack::SetProj(double x, double y, double * plat, double * plon)
{
  *plon = MercatorX2Lon(y);
  *plat = MercatorY2Lat(x);
}

void __fastcall GPXTrack::GetProj(double *px, double *py, double lat, double lon)
{
  //WGS84ToUTM(lat, lon, ((m_dMinLon + m_dMaxLon) / 2.0), x, y);
  *py = Lon2MercatorX(lon);
  *px = Lat2MercatorY(lat);
}

AnsiString __fastcall GPXTrack::FormatCoordString(double dCoord, GEOCOORD eType, GEOCOORDFMT eFmt)
{
  char chLetter, str[64];

  if(dCoord < 0)
  {
    dCoord = fabs(dCoord);
    chLetter = (eType == eLat) ? 'S' : 'W';
  }
  else chLetter = (eType == eLat) ? 'N' : 'E';

  if(eFmt == eDMS)
  {
    int nDeg = (int)dCoord;
    double dMin = (dCoord - (double)nDeg) * 60.0;
    int nMin = (int)dMin;
    double dSec = (dMin - (int)nMin) * 60.0;
    int nSec = (int)dSec;
    double dMSec = (dSec - (double)nSec) * 100.0;
    int nMSec = (int)(dMSec + 0.5);

    sprintf(str, "%d° %02d' %02d.%03d\"%c", nDeg, nMin, nSec, nMSec, chLetter);
  }
  else if(eFmt == eD_D)
  {
    int nDeg = (int)dCoord;
    double dMDeg = (dCoord - (double)nDeg) * 100000.0;
    int nMDeg = (int)(dMDeg + 0.5);

    sprintf(str, "%d°.%05d\"%c", nDeg, nMDeg, chLetter);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrack::FormatAltString(double dAlt)
{
  char str[64];
  sprintf(str, "%.0lfm", dAlt);
  return AnsiString(str);
}

AnsiString __fastcall GPXTrack::FormatSpeedString(double dSpeed)
{
  char str[64];
  sprintf(str, "%.0lfkm/h", dSpeed * 3.6);
  return AnsiString(str);
}

AnsiString __fastcall GPXTrack::FormatCourseString(double dCourse)
{
  char str[64];
  sprintf(str, "%.1lf°", dCourse);
  return AnsiString(str);
}

AnsiString __fastcall GPXTrack::FormatDistanceString(double dDist)
{
  char str[64];
  sprintf(str, "%.2lfkm", dDist / 1000.0);
  return AnsiString(str);
}

AnsiString __fastcall GPXTrack::FormatClimbString(double dClimb)
{
  char str[64];
  sprintf(str, "%.1lf°", dClimb);
  return AnsiString(str);
}

void __fastcall GPXTrack::SetPointsActive(bool bActive)
{
  for(int i = 0; i < PointCount; i++)
  {
    PGPXTrackPoint pPoint = Points[i];
    pPoint->m_bActive = bActive;
  }
}

void __fastcall GPXTrack::RemoveInactivePoints()
{
  int i = 0;

  while(i < PointCount)
  {
    PGPXTrackPoint pPoint = Points[i];

    if(!pPoint->m_bActive) DeletePoint(i);
    else i++;
  }
}

AnsiString __fastcall GPXTrack::FormatElapsedSring(DWORD Elapsed)
{
  int nHours = Elapsed / 3600;
  Elapsed %= 3600;
  int nMinutes = Elapsed / 60;
  int nSeconds = Elapsed % 60;

  char str[64];
  sprintf(str, "%d:%02d:%02d", nHours, nMinutes, nSeconds);

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXSatNumString()
{
  char str[64];

  if(m_nSats == -1)
    sprintf(str, "N/A");
  else
    sprintf(str, "%02d", m_nSats);

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXMagvarString()
{
  char str[64];

  if(m_dMagvar == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double magvar = ((double)((int)((m_dMagvar * 10.0) + 0.5))) / 10.0;
    sprintf(str, "%.1lf", magvar);
  }

  return AnsiString(str);
}

AnsiString __fastcall GPXTrackPoint::GetGPXFixString()
{
  AnsiString asFix = AnsiString("N/A");

  switch(m_nFix)
  {
    case 1:
      asFix = AnsiString("none");
      break;

    case 2:
      asFix = AnsiString("2d");
      break;

    case 3:
      asFix = AnsiString("3d");
      break;

    default:
      asFix = AnsiString("N/A");
      break;
  }

  return asFix;
}

AnsiString __fastcall GPXTrackPoint::GetGPXGeoidHeightString()
{
  char str[64];

  if(m_dGeoidHeigtht == GPS_PT_NO_VALUE)
    sprintf(str, "N/A");
  else
  {
    double geoid = (m_dGeoidHeigtht > 0.0) ? ((double)((int)((m_dGeoidHeigtht * 10.0) + 0.5))) / 10.0 :
                                             ((double)((int)((m_dGeoidHeigtht * 10.0) - 0.5))) / 10.0;

    sprintf(str, "%.1lf", geoid);
  }

  return AnsiString(str);
}


