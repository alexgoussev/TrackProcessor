//---------------------------------------------------------------------------

#ifndef CSVFileH
#define CSVFileH

#include <stdio.h>

#include "GPXData.h"

#define CSVFILE_MSG_NEWTRACK    (WM_APP + 600)
#define CSVFILE_MSG_NEWPOINT    (WM_APP + 602)

struct tagTokens {
  char Token[15][16];
  int Curpos;
  int Curtok;
};

class ColumbusCSVFile {
public:
  __fastcall ColumbusCSVFile(AnsiString asFile);
  __fastcall ColumbusCSVFile();
  __fastcall ~ColumbusCSVFile();
  void __fastcall Load(AnsiString asFile);
  void __fastcall Load();
  void __fastcall ClearTrack();
  __property AnsiString File  = { read=GetFile, write=SetFile };
  __property PGPXTrack Track  = { read=GetTrack };
private:
  AnsiString m_asFile;
  PGPXTrack m_pTrack;
  double m_dHDOPFactor;
  double m_dLoPassAlpha;
  double m_dLoPassAlpha_1;
  double m_dAVGAlpha;
  double m_dAVGAlpha_1;
  double m_dAVGAlpha1;
  double m_dHeadingPrev;
  void __fastcall SetFile(AnsiString value);
  AnsiString __fastcall GetFile();
  PGPXTrack __fastcall GetTrack();
protected:
  bool m_bRestoreSpeed;
  double __fastcall HeadingAVG(double BaseHeading, double Heading);
  double __fastcall SpeedAVG(double BaseSpeed, double Speed);
  double __fastcall HeadingLoPass(double Heading);
  double __fastcall CorrectHeading(int Heading, int Speed, double HDOP, double Lat, double Lon);
  double __fastcall CorrectSpeed(double dTime, int Speed, double HDOP, double Lat, double Lon);
  void __fastcall CorrectSpeedXY(double dTime, double &dVx, double &dVy, double HDOP, double Lat, double Lon);
  void __fastcall ProcessPoint(char * pDate, char * pTime, char * pLat, char * pLon, char * pAlt,
                               char * pSpeed, char * pHeading,  char *pFix, char * pPDOP,
                               char * pHDOP, char * pVDOP);
};
//---------------------------------------------------------------------------
#endif



