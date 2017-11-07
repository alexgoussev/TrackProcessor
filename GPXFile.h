//---------------------------------------------------------------------------

#ifndef GPXFileH
#define GPXFileH

#include <stdio.h>

#include "tinyxml2.h"
#include "GPXData.h"

#define GPXFILE_MSG_NEWTRACK    (WM_APP + 600)
#define GPXFILE_MSG_NEWSEG      (WM_APP + 601)
#define GPXFILE_MSG_NEWPOINT    (WM_APP + 602)
#define GPXFILE_MSG_SAVETRACK   (WM_APP + 603)
#define GPXFILE_MSG_SAVEPOINT   (WM_APP + 604)

using namespace tinyxml2;

class GPXFile
{
public:
  __fastcall GPXFile();
  __fastcall ~GPXFile();
  __fastcall GPXFile(AnsiString asFile);
  __property AnsiString File  = { read=GetFile, write=SetFile };
  void __fastcall Load(AnsiString strFileName);
  void __fastcall Load();
  void __fastcall ClearTracks();
  void __fastcall Save(AnsiString asFile);
  void __fastcall Save();
  void __fastcall AddTrack(PGPXTrack pTrack);
  void __fastcall DeleteTrack(int i);
  __property double AveragingTime  = { read=GetAveragingTime, write=SetAveragingTime };
  __property PGPXTrack Tracks[int i] = { read=GetTrack };
  __property int TrackCount  = { read=GetTrackCount };
private:
  AnsiString m_asFile;
  void __fastcall SetFile(AnsiString value);
  AnsiString __fastcall GetFile();
  void __fastcall SetAveragingTime(double value);
  double __fastcall GetAveragingTime();
  PGPXTrack __fastcall GetTrack(int i);
  int __fastcall GetTrackCount();
protected:
  TList * m_pTrackList;
  PGPXTrack m_pCurrentTrack;
  bool m_bIncorrectSpeedTag;
  void __fastcall ParseGPX_gpx(XMLElement *pGPX_GPX);
  void __fastcall ParseGPX_trk(XMLElement *pGPX_TRK);
  void __fastcall ParseGPX_trkseg(XMLElement *pGPX_TRKSEG);
  void __fastcall ParseGPX_trkpt(XMLElement *pGPX_TRKPT);
  void __fastcall ParseGPX_trkptext(XMLElement *pGPX_TRKPT_Ext, PGPXTrackPoint pNewPoint, bool &bSpeedPresent, bool &bCoursePresent);
  void __fastcall fwsprintf(FILE * pStream, const char * pbuff);
  AnsiString __fastcall XMLValue(XMLElement * pElement);
  AnsiString __fastcall XMLText(XMLElement * pElement);
  AnsiString __fastcall XMLAttribute(XMLElement * pElement, const char * pAttribute);
};

//---------------------------------------------------------------------------
#endif
