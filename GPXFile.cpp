//---------------------------------------------------------------------------
#include <vcl.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "GPXFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//#define CONSOLE

__fastcall GPXFile::GPXFile()
{
  m_pTrackList = new TList;
}

__fastcall GPXFile::~GPXFile()
{
  ClearTracks();
  delete m_pTrackList;
}

__fastcall GPXFile::GPXFile(AnsiString asFile)
{
  m_asFile = asFile;
  m_pTrackList = new TList;
}

void __fastcall GPXFile::SetFile(AnsiString value)
{
  m_asFile = ChangeFileExt(value, ".gpx");
}

AnsiString __fastcall GPXFile::GetFile()
{
  return m_asFile;
}

void __fastcall GPXFile::Load(AnsiString strFileName)
{
  m_asFile = strFileName;
  Load();
}

void __fastcall GPXFile::Load()
{
  if(m_asFile.Length())
  {
	tinyxml2::XMLDocument *pGPX = new tinyxml2::XMLDocument();

	if(XML_SUCCESS == pGPX->LoadFile(m_asFile.c_str()))
	{
	  XMLElement *pGPX_GPX = pGPX->FirstChildElement("gpx");
	  if(pGPX_GPX) ParseGPX_gpx(pGPX_GPX);
    }
  }
}

void __fastcall GPXFile::ParseGPX_gpx(XMLElement *pGPX_GPX)
{
  m_bIncorrectSpeedTag = false;

  AnsiString asCreator = XMLAttribute(pGPX_GPX, "creator");

  if(asCreator == AnsiString("NMEA Processor"))
    m_bIncorrectSpeedTag = true;

  XMLElement *pGPX_TRK = pGPX_GPX->FirstChildElement("trk");

  while(pGPX_TRK)
  {
    ParseGPX_trk(pGPX_TRK);
    pGPX_TRK = pGPX_GPX->NextSiblingElement("trk");
  }
}

void __fastcall GPXFile::ParseGPX_trk(XMLElement *pGPX_TRK)
{
  m_pCurrentTrack = new GPXTrack;
  m_pCurrentTrack->m_dSpeedAveragingTime = AveragingTime;

  XMLElement *pGPX_TRK_Child = pGPX_TRK->FirstChildElement();

  while(pGPX_TRK_Child)
  {
    AnsiString asValue = XMLValue(pGPX_TRK_Child);

    if(asValue == AnsiString("name"))
    {
      m_pCurrentTrack->m_asName = XMLText(pGPX_TRK_Child);

#ifndef CONSOLE
      PostMessage(Application->MainForm->Handle, GPXFILE_MSG_NEWTRACK, 0,
                  (LPARAM)m_pCurrentTrack->m_asName.c_str());
      Application->ProcessMessages();
#endif
    }
    else if(asValue == AnsiString("desc"))
      m_pCurrentTrack->m_asDesc = XMLText(pGPX_TRK_Child);
    else if(asValue == AnsiString("cmt"))
      m_pCurrentTrack->m_asCmt = XMLText(pGPX_TRK_Child);
    else if(asValue == AnsiString("src"))
      m_pCurrentTrack->m_asSrc = XMLText(pGPX_TRK_Child);
    else if(asValue == AnsiString("trkseg"))
    {
#ifndef CONSOLE
      PostMessage(Application->MainForm->Handle, GPXFILE_MSG_NEWSEG, 0, 0);
      Application->ProcessMessages();
#endif

      ParseGPX_trkseg(pGPX_TRK_Child);
    }

    pGPX_TRK_Child = pGPX_TRK_Child->NextSiblingElement();
  }

  if(m_pCurrentTrack->PointCount)
  {
    m_pCurrentTrack->m_asFileName = ExtractFileName(m_asFile);
    m_pTrackList->Add(m_pCurrentTrack);
  }
  else delete m_pCurrentTrack;

  m_pCurrentTrack = NULL;
}

void __fastcall GPXFile::ParseGPX_trkseg(XMLElement *pGPX_TRKSEG)
{
  XMLElement *pGPX_TRKPT = pGPX_TRKSEG->FirstChildElement("trkpt");

  while(pGPX_TRKPT)
  {
    ParseGPX_trkpt(pGPX_TRKPT);
    pGPX_TRKPT = pGPX_TRKPT->NextSiblingElement("trkpt");
  }
}

void __fastcall GPXFile::ParseGPX_trkpt(XMLElement *pGPX_TRKPT)
{
  PGPXTrackPoint pNewPoint = m_pCurrentTrack->NewPoint;
  bool bSpeedPresent = false, bCoursePresent = false, bDescPresent = false;

  AnsiString asCoord = XMLAttribute(pGPX_TRKPT, "lat");
  if(asCoord.Length()) pNewPoint->m_dLat = asCoord.ToDouble();

  asCoord = XMLAttribute(pGPX_TRKPT, "lon");
  if(asCoord.Length()) pNewPoint->m_dLon = asCoord.ToDouble();

  XMLElement *pGPX_TRKPT_Child = pGPX_TRKPT->FirstChildElement();

  while(pGPX_TRKPT_Child)
  {
    bDescPresent = false;
    AnsiString asValue = XMLValue(pGPX_TRKPT_Child);

    if(asValue == AnsiString("ele"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length()) pNewPoint->m_dAlt = asText.ToDouble();
    }
    else if(asValue == AnsiString("speed"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length())
      {
        pNewPoint->m_dSpeed = asText.ToDouble();
        if(m_bIncorrectSpeedTag) pNewPoint->m_dSpeed /= 3.6;
        bSpeedPresent = true;
      }
    }
    else if(asValue == AnsiString("course"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length())
      {
        pNewPoint->m_dCourse = asText.ToDouble();
        bCoursePresent = true;
      }
    }
    else if(asValue == AnsiString("hdop"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length()) pNewPoint->m_dHDOP = asText.ToDouble();
    }
    else if(asValue == AnsiString("pdop"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length()) pNewPoint->m_dPDOP = asText.ToDouble();
    }
    else if(asValue == AnsiString("vdop"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length()) pNewPoint->m_dVDOP = asText.ToDouble();
    }
    else if(asValue == AnsiString("time"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);

      struct date d;
      struct time t;

      char *p = strdup(asText.c_str());
      strupr(p);

      char *pp = strtok(p, "-T:Z");
      d.da_year = atoi(pp);

      pp = strtok(NULL, "-T:Z");
      d.da_mon = atoi(pp);

      pp = strtok(NULL, "-T:Z");
      d.da_day = atoi(pp);

      pp = strtok(NULL, "-T:Z");
      t.ti_hour = atoi(pp);

      pp = strtok(NULL, "-T:Z");
      t.ti_min = atoi(pp);

      pp = strtok(NULL, "-T:Z");
      double sec = atof(pp);
      t.ti_sec = (int)sec;
      double msec = sec - (double)t.ti_sec;

      pNewPoint->m_dTime = (double)(dostounix(&d, &t) - _timezone) + msec;

      free(p);
    }
    else if(asValue == AnsiString("desc"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length())
      {
        pNewPoint->m_asDesc = asText;
        bDescPresent = true;
      }
    }
    else if(asValue == AnsiString("sat"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length()) pNewPoint->m_nSats = asText.ToInt();
    }
    else if(asValue == AnsiString("fix"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length())
      {
        char *p = strdup(asText.c_str());
        strlwr(p);

        if(!strcmp(p, "none")) pNewPoint->m_nFix = 1;
        else if(!strcmp(p, "2d")) pNewPoint->m_nFix = 2;
        else if(!strcmp(p, "3d")) pNewPoint->m_nFix = 3;

        free(p);
      }
    }
    else if(asValue == AnsiString("magvar"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length()) pNewPoint->m_dMagvar = asText.ToDouble();
    }
    else if(asValue == AnsiString("geoidheight"))
    {
      AnsiString asText = XMLText(pGPX_TRKPT_Child);
      if(asText.Length()) pNewPoint->m_dGeoidHeigtht = asText.ToDouble();
    }
    else if(asValue == AnsiString("extensions"))
      ParseGPX_trkptext(pGPX_TRKPT_Child, pNewPoint, bSpeedPresent, bCoursePresent);

    pGPX_TRKPT_Child = pGPX_TRKPT_Child->NextSiblingElement();
  }

  if(bDescPresent)
  {
    if(!bSpeedPresent && bDescPresent)
    {
      char *pstr = strdup(pNewPoint->m_asDesc.c_str());
      strlwr(pstr);

      char *_pstr = (char*)malloc(strlen(pstr) + 1);

      char ch;
      char strval[64];
      int i = 0, j =0;

      do
      {
        ch = pstr[i++];
        if(ch != ' ') _pstr[j++] = ch;
      } while (ch != '\0');

      free(pstr);

      pstr = strstr(_pstr, "speed=");

      if(pstr && strlen(pstr) > 6)
      {
        pstr += 6;
        i = 0; j =0;

        for(;;)
        {
          ch = pstr[i++];

          if(isdigit(ch) || ch == '.') strval[j++] = ch;
          else
          {
            strval[j] = '\0';
            break;
          }
        }

        pNewPoint->m_dSpeed = atof(strval) / 3.6;
      }

      free(_pstr);
    }

    if(!bCoursePresent && bDescPresent)
    {
      char *pstr = strdup(pNewPoint->m_asDesc.c_str());
      strlwr(pstr);

      char *_pstr = (char*)malloc(strlen(pstr) + 1);

      char ch;
      char strval[64];
      int i = 0, j =0;

      do
      {
        ch = pstr[i++];
        if(ch != ' ') _pstr[j++] = ch;
      } while (ch != '\0');

      free(pstr);

      pstr = strstr(_pstr, "course=");

      if(pstr && strlen(pstr) > 7)
      {
        pstr += 7;
        i = 0; j =0;

        for(;;)
        {
          ch = pstr[i++];

          if(isdigit(ch) || ch == '.' || ch == '-') strval[j++] = ch;
          else
          {
            strval[j] = '\0';
            break;
          }
        }

        pNewPoint->m_dCourse = atof(strval);
      }

      free(_pstr);
    }

    if(pNewPoint->m_asDesc.SubString(0, 6) == AnsiString("Lat = "))
    {
      pNewPoint->m_asDesc = AnsiString("");
      bDescPresent = false;
    }
  }

  m_pCurrentTrack->AddPoint(pNewPoint);

#ifndef CONSOLE
  PostMessage(Application->MainForm->Handle, GPXFILE_MSG_NEWPOINT,
              0, (LPARAM)(m_pCurrentTrack->PointCount));
  Application->ProcessMessages();
#endif
}

void __fastcall GPXFile::ParseGPX_trkptext(XMLElement *pGPX_TRKPT_Ext,
                                           PGPXTrackPoint pNewPoint,
                                           bool &bSpeedPresent,
                                           bool &bCoursePresent)
{
  XMLElement *pGPX_TRKPT_GTEExt = pGPX_TRKPT_Ext->FirstChildElement("gte:gps");

  if(pGPX_TRKPT_GTEExt)
  {
    AnsiString asExt = XMLAttribute(pGPX_TRKPT_GTEExt, "speed");
    if(asExt.Length())
    {
      pNewPoint->m_dSpeed = asExt.ToDouble() / 3.6;
      bSpeedPresent = true;
    }

    asExt = XMLAttribute(pGPX_TRKPT_GTEExt, "azimuth");
    if(asExt.Length())
    {
      pNewPoint->m_dCourse = asExt.ToDouble();
      bCoursePresent = true;
    }
  }
}

void __fastcall GPXFile::ClearTracks()
{
  while(m_pTrackList->Count)
  {
    PGPXTrack pTrack = (PGPXTrack)m_pTrackList->Items[0];
    m_pTrackList->Delete(0);
    delete pTrack;
  }
}

void __fastcall GPXFile::SetAveragingTime(double value)
{
  TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  ini->WriteFloat("GPXFile", "AvgTime", value);
  delete ini;
}

double __fastcall GPXFile::GetAveragingTime()
{
  TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  double value = ini->ReadFloat("GPXFile", "AvgTime", 3.0);
  delete ini;

  return value;
}

PGPXTrack __fastcall GPXFile::GetTrack(int i)
{
  return (PGPXTrack)m_pTrackList->Items[i];
}


void __fastcall GPXFile::Save(AnsiString asFile)
{
  File = asFile;
  Save();
}

void __fastcall GPXFile::Save()
{
  if(m_asFile.Length())
  {
    FILE *pGPX = fopen(m_asFile.c_str(), "wt");

    if(pGPX)
    {
      char *pbuff = new char[1024];
      
      fwsprintf(pGPX, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
      fwsprintf(pGPX, "<gpx xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:gte=\"http://www.gpstrackeditor.com/xmlschemas/General/1\" xmlns=\"http://www.topografix.com/GPX/1/1\" version=\"1.1\" creator=\"Track Processor\">");

      double dMinLat = 999.0, dMaxLat = -999.0, dMinLon = 999.0, dMaxLon = -999.0;

      for(int i = 0; i < TrackCount; i++)
      {
        PGPXTrack pTrack = Tracks[i];

        if(pTrack->m_dMinLat >=  -90.0 && pTrack->m_dMinLat <=  90.0 &&
           pTrack->m_dMaxLat >=  -90.0 && pTrack->m_dMaxLat <=  90.0 &&
           pTrack->m_dMinLon >= -180.0 && pTrack->m_dMinLon <= 180.0 &&
           pTrack->m_dMaxLon >= -180.0 && pTrack->m_dMaxLon <= 180.0)
        {
          if(dMinLat > pTrack->m_dMinLat) dMinLat = pTrack->m_dMinLat;
          if(dMinLon > pTrack->m_dMinLon) dMinLon = pTrack->m_dMinLon;
          if(dMaxLat < pTrack->m_dMaxLat) dMaxLat = pTrack->m_dMaxLat;
          if(dMaxLon < pTrack->m_dMaxLon) dMaxLon = pTrack->m_dMaxLon;
        }
      }

      if(dMinLat >=  -90.0 && dMinLat <=  90.0 &&
         dMaxLat >=  -90.0 && dMaxLat <=  90.0 &&
         dMinLon >= -180.0 && dMinLon <= 180.0 &&
         dMaxLon >= -180.0 && dMaxLon <= 180.0)
      {
        dMinLat = (dMinLat > 0.0) ? ((double)((int)((dMinLat * 1000000.0) + 0.5))) / 1000000.0 :
                                    ((double)((int)((dMinLat * 1000000.0) - 0.5))) / 1000000.0;
        dMinLon = (dMinLon > 0.0) ? ((double)((int)((dMinLon * 1000000.0) + 0.5))) / 1000000.0 :
                                    ((double)((int)((dMinLon * 1000000.0) - 0.5))) / 1000000.0;
        dMaxLat = (dMaxLat > 0.0) ? ((double)((int)((dMaxLat * 1000000.0) + 0.5))) / 1000000.0 :
                                    ((double)((int)((dMaxLat * 1000000.0) - 0.5))) / 1000000.0;
        dMaxLon = (dMaxLon > 0.0) ? ((double)((int)((dMaxLon * 1000000.0) + 0.5))) / 1000000.0 :
                                    ((double)((int)((dMaxLon * 1000000.0) - 0.5))) / 1000000.0;

        fwsprintf(pGPX, "  <metadata>");
        sprintf(pbuff, "    <bounds minlat=\"%.6lf\" minlon=\"%.6lf\" maxlat=\"%.6lf\" maxlon=\"%.6lf\" />",
                dMinLat, dMinLon, dMaxLat, dMaxLon);
        fwsprintf(pGPX, pbuff);
        fwsprintf(pGPX, "  </metadata>");
      }

      for(int i = 0; i < TrackCount; i++)
      {
        PGPXTrack pTrack = Tracks[i];

#ifndef CONSOLE
        SendMessage(Application->MainForm->Handle, GPXFILE_MSG_SAVETRACK,
                    0, (LPARAM)(pTrack->m_asName.c_str()));
        Application->ProcessMessages();
#endif

        if(pTrack->PointCount)
        {
          fwsprintf(pGPX, "  <trk>");

          if(pTrack->m_asName.Length())
          {
            sprintf(pbuff, "    <name>%s</name>", pTrack->m_asName.c_str());
            fwsprintf(pGPX, pbuff);
          }

          if(pTrack->m_asDesc.Length())
          {
            sprintf(pbuff, "    <desc>%s</desc>", pTrack->m_asDesc.c_str());
            fwsprintf(pGPX, pbuff);
          }

          if(pTrack->m_asCmt.Length())
          {
            sprintf(pbuff, "    <cmt>%s</cmt>", pTrack->m_asCmt.c_str());
            fwsprintf(pGPX, pbuff);
          }

          if(pTrack->m_asSrc.Length())
          {
            sprintf(pbuff, "    <src>%s</src>", pTrack->m_asSrc.c_str());
            fwsprintf(pGPX, pbuff);
          }

          fwsprintf(pGPX, "    <trkseg>");

          for(int j = 0; j < pTrack->PointCount; j++)
          {
#ifndef CONSOLE
            SendMessage(Application->MainForm->Handle, GPXFILE_MSG_SAVEPOINT,
                      0, (LPARAM)(j + 1));
            Application->ProcessMessages();
#endif

            PGPXTrackPoint pPoint = pTrack->Points[j];

            //<trkpt lat="56.850954550" lon="60.384995050">
            //  <ele>262.0</ele>
            //  <time>2013-08-26T12:42:00.000Z</time>
            //  <desc>Lat = 56.850955, Lon = 60.384995, Alt = 262.0m, Speed = 0.5km/h, Course = 200.3</desc>
            //  <hdop>2.69</hdop>
            //  <speed>0.5</speed>
            //  <course>200.3</course>
            //</trkpt>

            sprintf(pbuff, "      <trkpt lat=\"%s\" lon=\"%s\">", pPoint->GPXLatString.c_str(), pPoint->GPXLonString.c_str());
            fwsprintf(pGPX, pbuff);

            if(pPoint->m_dTime != GPS_PT_NO_VALUE)
            {
              sprintf(pbuff, "        <time>%s</time>", pPoint->GPXTimeString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_dAlt != GPS_PT_NO_VALUE)
            {
              sprintf(pbuff, "        <ele>%s</ele>", pPoint->GPXEleString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_dHDOP != GPS_PT_NO_VALUE)
            {
              sprintf(pbuff, "        <hdop>%s</hdop>", pPoint->GPXHDOPString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_dPDOP != GPS_PT_NO_VALUE)
            {
              sprintf(pbuff, "        <pdop>%s</pdop>", pPoint->GPXPDOPString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_dVDOP != GPS_PT_NO_VALUE)
            {
              sprintf(pbuff, "        <vdop>%s</vdop>", pPoint->GPXVDOPString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_nSats > 0)
            {
              sprintf(pbuff, "        <sat>%s</sat>", pPoint->GPXSatNumString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_nFix > 0)
            {
              sprintf(pbuff, "        <fix>%s</fix>", pPoint->GPXFixString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_dMagvar != GPS_PT_NO_VALUE)
            {
              sprintf(pbuff, "        <magvar>%s</magvar>", pPoint->GPXMagvarString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_dGeoidHeigtht != GPS_PT_NO_VALUE)
            {
              sprintf(pbuff, "        <geoidheight>%s</geoidheight>", pPoint->GPXGeoidHeightString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            if(pPoint->m_dSpeed != GPS_PT_NO_VALUE || pPoint->m_dCourse != GPS_PT_NO_VALUE)
            {
              fwsprintf(pGPX, "        <extensions>");

              sprintf(pbuff, "          <gte:gps speed=\"%s\" azimuth=\"%s\"/>", pPoint->GPXSpeedString.c_str(), pPoint->GPXCourseString.c_str());
              fwsprintf(pGPX, pbuff);

              fwsprintf(pGPX, "        </extensions>");
            }

            if(pPoint->m_dSpeed != GPS_PT_NO_VALUE &&
               pPoint->m_dCourse != GPS_PT_NO_VALUE &&
              pPoint->m_dAlt != GPS_PT_NO_VALUE)
            {
              sprintf(pbuff, "        <desc>%s</desc>", pPoint->GPXDescString.c_str());
              fwsprintf(pGPX, pbuff);
            }

            fwsprintf(pGPX, "      </trkpt>");
          }

          fwsprintf(pGPX, "    </trkseg>");
          fwsprintf(pGPX, "  </trk>");
        }
      }

      fwsprintf(pGPX, "</gpx>");
      
      delete [] pbuff;
      fclose(pGPX);
    }
  }
}

int __fastcall GPXFile::GetTrackCount()
{
  return m_pTrackList->Count;
}

void __fastcall GPXFile::AddTrack(PGPXTrack pTrack)
{
  PGPXTrack pNewTrack = new GPXTrack(pTrack);
  m_pTrackList->Add(pNewTrack);
}

void __fastcall GPXFile::DeleteTrack(int i)
{
  PGPXTrack pTrack = Tracks[i];
  m_pTrackList->Delete(i);
  delete pTrack;
}

void __fastcall GPXFile::fwsprintf(FILE * pStream, const char * pbuff)
{
  UTF8String utf8Line = UTF8Encode(WideString(pbuff));
  fprintf(pStream, "%s\n", utf8Line.c_str());
}


AnsiString __fastcall GPXFile::XMLValue(XMLElement * pElement)
{
  AnsiString asValue = AnsiString("");

  const char *pText = pElement->Value();
  if(pText) asValue = AnsiString(UTF8ToString(UTF8String(pText)));

  return asValue;
}

AnsiString __fastcall GPXFile::XMLText(XMLElement * pElement)
{
  AnsiString asValue = AnsiString("");

  const char *pText = pElement->GetText();
  if(pText) asValue = AnsiString(UTF8ToString(UTF8String(pText)));

  return asValue;
}

AnsiString __fastcall GPXFile::XMLAttribute(XMLElement * pElement, const char * pAttribute)
{
  AnsiString asValue = AnsiString("");

  const char *pText = pElement->Attribute(pAttribute);
  if(pText) asValue = AnsiString(UTF8ToString(UTF8String(pText)));

  return asValue;
}
