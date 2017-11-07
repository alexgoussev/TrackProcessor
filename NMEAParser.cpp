//---------------------------------------------------------------------------
#include <vcl.h>
#include <time.h>
#include <dos.h>
#pragma hdrstop

#include "NMEAParser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TNMEAParser::TNMEAParser()
{
  m_dLastTime = 0.0;
  m_nBufferSize = 1024;
  m_dwNMEAMask = 0;
  m_pBuffer = new char[m_nBufferSize];
  m_pUserData = new DWORD;
  m_pNMEAStrings = new TStringList;
  m_pNMEA = new nmeap_context_t;
  m_pGGA = new nmeap_gga_t;
  m_pRMC = new nmeap_rmc_t;
  m_pGSA = new nmeap_gsa_t;
  m_pNMEAPoint = new nmea_point_t;

  InitNMEAPointStruct();

  m_pTrack = NULL;
  m_bReady = false;

  if(!nmeap_init(m_pNMEA, (void*)m_pUserData)                   &&
     !nmeap_addParser(m_pNMEA, "GPGGA", nmeap_gpgga, 0, m_pGGA) &&
     !nmeap_addParser(m_pNMEA, "GPRMC", nmeap_gprmc, 0, m_pRMC) &&
     !nmeap_addParser(m_pNMEA, "GPGSA", TNMEAParser::custom_gpgsa, 0, m_pGSA))
    m_bReady = true;
}

__fastcall TNMEAParser::TNMEAParser(int nBufferSize)
{
  m_dLastTime = 0.0;
  m_nBufferSize = nBufferSize;
  m_dwNMEAMask = 0;
  m_pBuffer = new char[m_nBufferSize];
  m_pUserData = new DWORD;
  m_pNMEAStrings = new TStringList;
  m_pNMEA = new nmeap_context_t;
  m_pGGA = new nmeap_gga_t;
  m_pRMC = new nmeap_rmc_t;
  m_pGSA = new nmeap_gsa_t;
  m_pNMEAPoint = new nmea_point_t;

  InitNMEAPointStruct();

  m_pTrack = NULL;
  m_bReady = false;

  if(!nmeap_init(m_pNMEA, (void*)m_pUserData)                   &&
     !nmeap_addParser(m_pNMEA, "GPGGA", nmeap_gpgga, 0, m_pGGA) &&
     !nmeap_addParser(m_pNMEA, "GPRMC", nmeap_gprmc, 0, m_pRMC) &&
     !nmeap_addParser(m_pNMEA, "GPGSA", TNMEAParser::custom_gpgsa, 0, m_pGSA))
    m_bReady = true;
}

__fastcall TNMEAParser::~TNMEAParser()
{
  delete [] m_pBuffer;
  delete m_pUserData;
  delete m_pNMEAStrings;
  delete m_pNMEA;
  delete m_pGGA;
  delete m_pRMC;
  delete m_pGSA;
  delete m_pNMEAPoint;

  if(m_pTrack)
  {
    delete m_pTrack;
    m_pTrack = NULL;
  }
}

double __fastcall TNMEAParser::Time2UNIX(unsigned long ulDate, double dTime)
{
  unsigned long ulTime = (unsigned long)dTime;
  double dMsecs = dTime - (double)ulTime;

  struct date d;
  struct time t;

  d.da_day = ulDate / 10000;
  ulDate %= 10000;
  d.da_mon = ulDate / 100;
  d.da_year = ulDate % 100 + 2000;

  t.ti_hour = ulTime / 10000;
  ulTime %= 10000;
  t.ti_min = ulTime / 100;
  t.ti_sec = ulTime % 100;

  return ((double)(dostounix(&d, &t) - _timezone)) + dMsecs;
}

int __fastcall TNMEAParser::ParseString(AnsiString asString)
{
  strcpy(m_pBuffer, asString.c_str());
  return ParseBuffer();
}

int __fastcall TNMEAParser::ParseBuffer()
{
  int rc = -1;
  static double dLastTime = 0.0;

  char *p = strchr(m_pBuffer, '*');
  if(p)
  {
    if(IsHex(p[1]) && IsHex(p[2]))
    {
      p[3] = '\r';
      p[4] = '\n';
      p[5] = '\0';

      int len = strlen(m_pBuffer);
      rc = nmeap_parseBuffer(m_pNMEA, m_pBuffer, &len);

      if(rc == NMEAP_GPGGA)
      {
        if(dLastTime > 1.0 && dLastTime != m_pGGA->time) CompleteNMEAPoint();
        dLastTime = m_pGGA->time;
        memcpy(&m_pNMEAPoint->gga, m_pGGA, sizeof(nmeap_gga_t));
        m_pNMEAPoint->dwNMEAMask |= NMEA_GGA_PRESENT;
      }
      else if(rc == NMEAP_GPRMC)
      {
        if(dLastTime > 1.0 && dLastTime != m_pRMC->time) CompleteNMEAPoint();
        dLastTime = m_pRMC->time;
        memcpy(&m_pNMEAPoint->rmc, m_pRMC, sizeof(nmeap_rmc_t));
        m_pNMEAPoint->dwNMEAMask |= NMEA_RMC_PRESENT;
      }
      else if(rc == NMEAC_GPGSA)
      {
        memcpy(&m_pNMEAPoint->gsa, m_pGSA, sizeof(nmeap_gsa_t));
        m_pNMEAPoint->dwNMEAMask |= NMEA_GSA_PRESENT;
      }
    }
  }

  return rc;
}

char * __fastcall TNMEAParser::GetBuffer()
{
  return m_pBuffer;
}

bool __fastcall TNMEAParser::IsHex(char ch)
{
  return ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')); 
}

void __fastcall TNMEAParser::CompleteNMEAPoint()
{
  if(m_pNMEAPoint->rmc.warn == 'A' &&
     m_pNMEAPoint->rmc.date > 0    &&
     m_pNMEAPoint->rmc.time > 0    &&
     m_pNMEAPoint->dwNMEAMask == m_dwNMEAMask)
  {
    double dTime = Time2UNIX(m_pNMEAPoint->rmc.date, m_pNMEAPoint->rmc.time);

    if(dTime > m_dLastTime)
    {
      m_dLastTime = dTime;

      if(!m_pTrack) m_pTrack = new GPXTrack;

      PGPXTrackPoint pPoint = m_pTrack->NewPoint;

      pPoint->m_dLat = m_pNMEAPoint->rmc.latitude;
      pPoint->m_dLon = m_pNMEAPoint->rmc.longitude;
      pPoint->m_dTime = dTime;
      pPoint->m_dAlt = m_pNMEAPoint->gga.altitude;
      pPoint->m_dSpeed = m_pNMEAPoint->rmc.speed * 0.514444;
      pPoint->m_dCourse = m_pNMEAPoint->rmc.course;
      if(m_pNMEAPoint->gsa.dHDOP > 0.0) pPoint->m_dHDOP = m_pNMEAPoint->gsa.dHDOP;
      if(m_pNMEAPoint->gsa.dPDOP > 0.0) pPoint->m_dPDOP = m_pNMEAPoint->gsa.dPDOP;
      if(m_pNMEAPoint->gsa.dVDOP > 0.0) pPoint->m_dVDOP = m_pNMEAPoint->gsa.dVDOP;
      if(m_pNMEAPoint->gga.satellites > 0) pPoint->m_nSats = m_pNMEAPoint->gga.satellites;
      if(m_pNMEAPoint->gsa.nFixCode > 0) pPoint->m_nFix = m_pNMEAPoint->gsa.nFixCode;
      pPoint->m_dMagvar = m_pNMEAPoint->rmc.magvar;
      pPoint->m_dGeoidHeigtht = m_pNMEAPoint->gga.geoid;

      m_pTrack->AddPoint(pPoint);

      SendMessage(Application->MainForm->Handle, NMEAFILE_MSG_NEWPOINT,
                  0, (LPARAM)(m_pTrack->PointCount));
      Application->ProcessMessages();
    }
  }

  InitNMEAPointStruct();
}

PGPXTrack __fastcall TNMEAParser::GetTrack()
{
  if(m_pNMEAPoint->dwNMEAMask == m_dwNMEAMask)
    CompleteNMEAPoint();

  return m_pTrack;
}

int TNMEAParser::custom_gpgsa(nmeap_context_t * context, nmeap_sentence_t * sentence)
{
  int rcode = -1;

  pnmeap_gsa pgsa = (pnmeap_gsa)sentence->data;

  if(pgsa)
  {
    pgsa->cFixType = context->token[1][0];
    pgsa->nFixCode = atoi(context->token[2]);

    for(int i = 0; i < 12; i++)
    {
      if(strlen(context->token[i + 3]))
        pgsa->nSatPRN[i] = atoi(context->token[i + 3]);
      else
        pgsa->nSatPRN[i] = -1;
    }

    if(strlen(context->token[15]))
      pgsa->dPDOP = atof(context->token[15]);
    else
      pgsa->dPDOP = GPS_PT_NO_VALUE;

    if(strlen(context->token[16]))
      pgsa->dHDOP = atof(context->token[16]);
    else
      pgsa->dHDOP = GPS_PT_NO_VALUE;

    if(strlen(context->token[17]))
      pgsa->dVDOP = atof(context->token[17]);
    else
      pgsa->dVDOP = GPS_PT_NO_VALUE;

    if(sentence->callout)
      (*sentence->callout)(context, pgsa, context->user_data);

    rcode = NMEAC_GPGSA;
  }

  return rcode;
}

bool __fastcall TNMEAParser::GetReady()
{
  return m_bReady;
}

int __fastcall TNMEAParser::GetBufferSize()
{
  return m_nBufferSize;
}


void __fastcall TNMEAParser::InitNMEAPointStruct()
{
  m_pNMEAPoint->dwNMEAMask = 0;

  m_pNMEAPoint->gga.latitude = GPS_PT_NO_VALUE;
  m_pNMEAPoint->gga.longitude = GPS_PT_NO_VALUE;
  m_pNMEAPoint->gga.altitude = GPS_PT_NO_VALUE;
  m_pNMEAPoint->gga.time = GPS_PT_NO_VALUE;
  m_pNMEAPoint->gga.satellites = -1;
  m_pNMEAPoint->gga.quality = -1;
  m_pNMEAPoint->gga.hdop = GPS_PT_NO_VALUE;
  m_pNMEAPoint->gga.geoid = GPS_PT_NO_VALUE;

  m_pNMEAPoint->rmc.time = GPS_PT_NO_VALUE;
  m_pNMEAPoint->rmc.warn = 'V';
  m_pNMEAPoint->rmc.latitude = GPS_PT_NO_VALUE;
  m_pNMEAPoint->rmc.longitude = GPS_PT_NO_VALUE;
  m_pNMEAPoint->rmc.speed = GPS_PT_NO_VALUE;
  m_pNMEAPoint->rmc.course = GPS_PT_NO_VALUE;
  m_pNMEAPoint->rmc.date = 0;
  m_pNMEAPoint->rmc.magvar = GPS_PT_NO_VALUE;

  m_pNMEAPoint->gsa.cFixType = 'A';
  m_pNMEAPoint->gsa.nFixCode = 1;
  for(int i = 0; i < 12; i++) m_pNMEAPoint->gsa.nSatPRN[i] = -1;
  m_pNMEAPoint->gsa.dPDOP = GPS_PT_NO_VALUE;
  m_pNMEAPoint->gsa.dHDOP = GPS_PT_NO_VALUE;
  m_pNMEAPoint->gsa.dVDOP = GPS_PT_NO_VALUE;
}

void __fastcall TNMEAParser::LoadFromFile(AnsiString asFileName)
{
  m_pNMEAStrings->LoadFromFile(asFileName);
}

void __fastcall TNMEAParser::AddString(AnsiString asNMEAString)
{
  m_pNMEAStrings->Add(asNMEAString);
}

void __fastcall TNMEAParser::ParseStrings()
{
  for(int i = 0; i < m_pNMEAStrings->Count; i++)
  {
    AnsiString asSentence = m_pNMEAStrings->Strings[i].SubString(2, 5);

    if(asSentence == AnsiString("GPGGA"))
      m_dwNMEAMask |= NMEA_GGA_PRESENT;
    else if(asSentence == AnsiString("GPGSA"))
      m_dwNMEAMask |= NMEA_GSA_PRESENT;
    else if(asSentence == AnsiString("GPRMC"))
      m_dwNMEAMask |= NMEA_RMC_PRESENT;

    if(m_dwNMEAMask == 0x00000007) break;
  }

  for(int i = 0; i < m_pNMEAStrings->Count; i++)
    ParseString(m_pNMEAStrings->Strings[i]);
}

int __fastcall TNMEAParser::GetStringsCount()
{
  return m_pNMEAStrings->Count;
}
