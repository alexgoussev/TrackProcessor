//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include <float.h>
#include <System.IniFiles.hpp>

#pragma hdrstop

#include "CSVFile.h"
#include "SK42WGS84\\SK42WGS84.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall ColumbusCSVFile::ColumbusCSVFile(AnsiString asFile)
{
  m_asFile = asFile;
  m_pTrack = NULL;
}

__fastcall ColumbusCSVFile::ColumbusCSVFile()
{
  m_asFile = AnsiString("");
  m_pTrack = NULL;
}

__fastcall ColumbusCSVFile::~ColumbusCSVFile()
{
  ClearTrack();
}

void __fastcall ColumbusCSVFile::Load(AnsiString asFile)
{
  if(!access(asFile.c_str(), 0))
  {
    TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".INI"));
    m_dHDOPFactor = ini->ReadFloat("ColumbusCSV", "HDOPFactor", 10.0);
    m_dLoPassAlpha = ini->ReadFloat("ColumbusCSV", "HeadingSmooth", 0.75);
    m_dAVGAlpha = ini->ReadFloat("ColumbusCSV", "ComplementaryFactor", 0.9);
    m_bRestoreSpeed = ini->ReadBool("ColumbusCSV", "RestoreSpeed", true);
    delete ini;

    m_dHeadingPrev = 0.0;
    m_dLoPassAlpha_1 = (1.0 - m_dLoPassAlpha);
    m_dAVGAlpha_1 = (1.0 - m_dAVGAlpha);
    m_dAVGAlpha1 = 1.0 / m_dAVGAlpha_1;

    FILE *in = fopen(asFile.c_str(), "rb");

    if(in)
    {
      m_pTrack = new GPXTrack;
      m_pTrack->m_asSrc = AnsiString("Columbus V-900/990");
      m_pTrack->m_asName = ExtractFileName(ChangeFileExt(asFile, ""));
      m_pTrack->m_asFileName = ExtractFileName(m_asFile);

      SendMessage(Application->MainForm->Handle, CSVFILE_MSG_NEWTRACK,
                  0, (LPARAM)(m_pTrack->m_asName.c_str()));
      Application->ProcessMessages();

      tagTokens Tokens;
      int NewLine = 0;
      bool FirstLine = true;

      Tokens.Curtok = Tokens.Curpos = 0;
      for(int i = 0; i < 15; i++) memset(Tokens.Token[i], 0, 16);

      while(!feof(in))
      {
        int ch = fgetc(in);

        switch(ch)
        {
          case ',':
          {
            Tokens.Curtok++;
            Tokens.Curpos = 0;
            NewLine = 0;
            break;
          }

          case 0x0D:
          case 0x0A:
          {
            NewLine++;

            if(NewLine == 2)
            {
              if(!FirstLine)
              {
                if(Tokens.Token[1][0] != 'C')
                  ProcessPoint(Tokens.Token[2],  Tokens.Token[3], Tokens.Token[4],
                               Tokens.Token[5],  Tokens.Token[6], Tokens.Token[7],
                               Tokens.Token[8],  Tokens.Token[9], Tokens.Token[11],
                               Tokens.Token[12], Tokens.Token[13]);
              }
              else
                FirstLine = false;

              Tokens.Curtok = 0;
              Tokens.Curpos = 0;
              NewLine = 0;
              for(int i = 0; i < 15; i++) memset(Tokens.Token[i], 0, 16);
            }

            break;
          }

          default:
          {
            Tokens.Token[Tokens.Curtok][Tokens.Curpos++] = ch;
            NewLine = 0;
            break;
          }
        }
      }

      fclose(in);
    }
  }
}

void __fastcall ColumbusCSVFile::Load()
{
  if(m_asFile.Length()) Load(m_asFile);
}

void __fastcall ColumbusCSVFile::SetFile(AnsiString value)
{
  if(m_asFile != value) {
    m_asFile = value;
  }
}
AnsiString __fastcall ColumbusCSVFile::GetFile()
{
  return m_asFile;
}

PGPXTrack __fastcall ColumbusCSVFile::GetTrack()
{
  return m_pTrack;
}

double __fastcall ColumbusCSVFile::HeadingAVG(double BaseHeading, double Heading)
{
  ArrangeCourses(&Heading, &BaseHeading);
  return NormalizeCourse(m_dAVGAlpha * BaseHeading + m_dAVGAlpha_1 * Heading);
}

double __fastcall ColumbusCSVFile::SpeedAVG(double BaseSpeed, double Speed)
{
  return (m_dAVGAlpha * BaseSpeed + m_dAVGAlpha_1 * Speed);
}

double __fastcall ColumbusCSVFile::HeadingLoPass(double Heading)
{
  ArrangeCourses(&Heading, &m_dHeadingPrev);
  Heading = m_dLoPassAlpha * Heading + m_dLoPassAlpha_1 * m_dHeadingPrev;

  Heading = NormalizeCourse(Heading);
  m_dHeadingPrev = Heading;

  return Heading;
}

double __fastcall ColumbusCSVFile::CorrectHeading(int Heading, int Speed, double HDOP, double Lat, double Lon)
{
  double dHeading = (double)Heading;
  PGPXTrackPoint pPoint;

  if(m_pTrack->PointCount)
  {
    pPoint = m_pTrack->Points[m_pTrack->PointCount - 1];

    double dhd = AzimuthLL(pPoint->m_dLat, pPoint->m_dLon, Lat, Lon);
    double dhdarr = dhd, dHeadingArr = dHeading;
    ArrangeCourses(&dhdarr, &dHeadingArr);

    if(Speed > 0 && (fabs(dhdarr - dHeadingArr) < m_dAVGAlpha1))
      dHeading = HeadingAVG(dHeading, dhd);
    else if(Speed == 0) dHeading = dhd;
  }

  dHeading = HeadingLoPass(dHeading);

  return dHeading;
}

double __fastcall ColumbusCSVFile::CorrectSpeed(double dTime, int Speed, double HDOP, double Lat, double Lon)
{
  double dSpeed = ((double)Speed) / 3.6;
  static double dAVGSpeed = 0.0;
  PGPXTrackPoint pPoint;

  for(int i = m_pTrack->PointCount - 1; i >= 0; i--)
  {
    pPoint = m_pTrack->Points[i];

    double dist = m_pTrack->GetDistance(pPoint->m_dLat, pPoint->m_dLon, Lat, Lon);

    if(dist >= (HDOP + pPoint->m_dHDOP) * m_dHDOPFactor)
    {
      double dspd = dist / (dTime - pPoint->m_dTime);

      if(Speed > 0 && (dspd < (dSpeed + m_dAVGAlpha1)))
        dSpeed = SpeedAVG(dSpeed, dspd);
      else if(Speed == 0) dSpeed = dspd;

      break;
    }
  }

  dAVGSpeed = (dSpeed + dAVGSpeed) / 2.0;

  return dAVGSpeed;
}

void __fastcall ColumbusCSVFile::CorrectSpeedXY(double dTime, double &dVx, double &dVy, double HDOP, double Lat, double Lon)
{
  static double dAVGSpeedX = 0.0, dAVGSpeedY = 0.0;
  PGPXTrackPoint pPoint;
  bool bZeroSpeed = (fabs(dVx) <= DBL_EPSILON && fabs(dVy) <= DBL_EPSILON);
  static bool bLastZeroSpeed = true;

  for(int i = m_pTrack->PointCount - 1; i >= 0; i--)
  {
    pPoint = m_pTrack->Points[i];

    double dist = m_pTrack->GetDistance(pPoint->m_dLat, pPoint->m_dLon, Lat, Lon);

    if(dist >= (HDOP + pPoint->m_dHDOP) * m_dHDOPFactor)
    {
      double dX1 = Lon2MercatorX(Lon), dX2 = Lon2MercatorX(pPoint->m_dLon);
      double dY1 = Lat2MercatorY(Lat), dY2 = Lat2MercatorY(pPoint->m_dLat);
      double dspdx = (dX2 - dX1) / (dTime - pPoint->m_dTime);
      double dspdy = (dY2 - dY1) / (dTime - pPoint->m_dTime);

      dVx = SpeedAVG(dVx, dspdx);
      dVy = SpeedAVG(dVy, dspdy);

      break;
    }
  }

  if(bZeroSpeed || bLastZeroSpeed)
  {
    dAVGSpeedX = (dVx + dAVGSpeedX) / 2.0;
    dAVGSpeedY = (dVy + dAVGSpeedY) / 2.0;
  }
  else
  {
    dAVGSpeedX = dVx;
    dAVGSpeedY = dVy;
  }

  bLastZeroSpeed = bZeroSpeed;
}


void __fastcall ColumbusCSVFile::ProcessPoint(char * pDate, char * pTime, char * pLat, char * pLon,
                                              char * pAlt, char * pSpeed, char * pHeading, char *pFix,
                                              char * pPDOP, char * pHDOP, char * pVDOP)
{
  int Year, Mon, Day, Hour, Min, Sec;
  double Latitude = GPS_PT_NO_VALUE, Longitude = GPS_PT_NO_VALUE, LatSign = 1.0, LonSign = 1.0;
  int Altitude = (int)GPS_PT_NO_VALUE, Speed = (int)GPS_PT_NO_VALUE, Heading = (int)GPS_PT_NO_VALUE;
  int nFix = -1;
  double HDOP = GPS_PT_NO_VALUE, PDOP = GPS_PT_NO_VALUE, VDOP = GPS_PT_NO_VALUE;
  static double dLastTime = 0.0;

  if(strlen(pDate) && strlen(pTime) && strlen(pLat) && strlen(pLon))
  {
    sscanf(pDate, "%02d%02d%02d", &Year, &Mon, &Day); Year += 2000;
    sscanf(pTime, "%02d%02d%02d", &Hour, &Min, &Sec);

    if(pLat[strlen(pLat) - 1] == 'S') LatSign = -1.0;
    pLat[strlen(pLat) - 1] = '\0';
    sscanf(pLat, "%lf", &Latitude); Latitude *= LatSign;

    if(pLon[strlen(pLon) - 1] == 'W') LonSign = -1.0;
    pLon[strlen(pLon) - 1] = '\0';
    sscanf(pLon, "%lf", &Longitude); Longitude *= LonSign;

    if(strlen(pAlt)) sscanf(pAlt, "%d", &Altitude);
    if(strlen(pSpeed)) sscanf(pSpeed, "%d", &Speed);
    if(strlen(pHeading)) sscanf(pHeading, "%d", &Heading);

    if(strlen(pFix))
    {
      if(pFix[0] == '2') nFix = 2;
      else if(pFix[0] == '3') nFix = 3;
    }

    if(strlen(pPDOP)) sscanf(pPDOP, "%lf", &PDOP);
    if(strlen(pHDOP)) sscanf(pHDOP, "%lf", &HDOP);
    if(strlen(pVDOP)) sscanf(pVDOP, "%lf", &VDOP);

    struct date d;
    struct time t;

    d.da_year = Year; d.da_mon = Mon; d.da_day = Day;
    t.ti_hour = Hour; t.ti_min = Min; t.ti_sec = Sec;
    double dTime = (double)(dostounix(&d, &t) - _timezone);

    if(dTime > dLastTime)
    {
      dLastTime = dTime;

      double dSpeed = GPS_PT_NO_VALUE, dHeading = GPS_PT_NO_VALUE;

      if(m_bRestoreSpeed)
      {
        if(Speed != (int)GPS_PT_NO_VALUE   &&
           Heading != (int)GPS_PT_NO_VALUE &&
           HDOP != GPS_PT_NO_VALUE)
        {
          double dVx, dVy, dV = ((double)Speed) / 3.6;

          DecomposeSpeed(dV, (double)Heading, &dVx, &dVy);
          CorrectSpeedXY(dTime, dVx, dVy, HDOP, Latitude, Longitude);
          ComposeSpeed(dVx, dVy, &dSpeed, &dHeading);
        }
      }
      else
      {
        dSpeed = ((double)Speed) / 3.6;
        dHeading = (double)Heading;
      }

      PGPXTrackPoint pPoint = m_pTrack->NewPoint;

      pPoint->m_dLat = Latitude; pPoint->m_dLon = Longitude;
      pPoint->m_dAlt = (double)Altitude;
      pPoint->m_dSpeed = dSpeed; pPoint->m_dCourse = dHeading;
      pPoint->m_dHDOP = HDOP; pPoint->m_dPDOP = PDOP; pPoint->m_dVDOP = VDOP;
      pPoint->m_dTime = dTime;
      if(nFix > 0) pPoint->m_nFix = nFix;

      m_pTrack->AddPoint(pPoint);

      SendMessage(Application->MainForm->Handle, CSVFILE_MSG_NEWPOINT,
                  0, (LPARAM)(m_pTrack->PointCount));
      Application->ProcessMessages();
    }
  }
}

void __fastcall ColumbusCSVFile::ClearTrack()
{
  if(m_pTrack)
  {
    delete m_pTrack;
    m_pTrack = NULL;
  }
}
