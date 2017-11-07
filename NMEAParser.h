//---------------------------------------------------------------------------

#ifndef NMEAParserH
#define NMEAParserH
//---------------------------------------------------------------------------

#include "GPXData.h"
#include "nmeap\\nmeap.h"

#define NMEAFILE_MSG_NEWPOINT    (WM_APP + 602)

#define NMEAC_GPGSA (NMEAP_USER + 0)

#define NMEA_GGA_PRESENT          0x00000001
#define NMEA_GSA_PRESENT          0x00000002
#define NMEA_RMC_PRESENT          0x00000004

/*
  GSA      Satellite status
  A        Auto selection of 2D or 3D fix (M = manual)
  3        3D fix - values include: 1 = no fix
                                   2 = 2D fix
                                   3 = 3D fix
  04,05... PRNs of satellites used for fix (space for 12)
  2.5      PDOP (dilution of precision)
  1.3      Horizontal dilution of precision (HDOP)
  2.1      Vertical dilution of precision (VDOP)
  *39      the checksum data, always begins with *
*/
struct nmeap_gsa_t {
  char cFixType;
  int nFixCode;
  int nSatPRN[12];
  double dPDOP;
  double dHDOP;
  double dVDOP;
};

struct nmea_point_t {
  DWORD dwNMEAMask;
  nmeap_gga_t gga;
  nmeap_rmc_t rmc;
  nmeap_gsa_t gsa;
};

typedef nmeap_context_t * pnmeap_context;
typedef nmeap_gsa_t* pnmeap_gsa;
typedef nmea_point_t* pnmea_point;
typedef nmeap_context_t* pnmeap_context;
typedef nmeap_gga_t* pnmeap_gga;
typedef nmeap_rmc_t* pnmeap_rmc;


class TNMEAParser {
public:
  __fastcall TNMEAParser();
  __fastcall ~TNMEAParser();
  __fastcall TNMEAParser(int nBufferSize);
  int __fastcall ParseString(AnsiString asString);
  void __fastcall LoadFromFile(AnsiString asFileName);
  void __fastcall AddString(AnsiString asNMEAString);
  void __fastcall ParseStrings();
  __property char * Buffer  = { read=GetBuffer };
  __property PGPXTrack Track  = { read=GetTrack };
  __property bool Ready  = { read=GetReady };
  __property int BufferSize  = { read=GetBufferSize };
  __property int StringsCount  = { read=GetStringsCount };
protected:
  char *m_pBuffer;
  pnmeap_context m_pNMEA;
  pnmeap_gga m_pGGA;
  pnmeap_rmc m_pRMC;
  pnmeap_gsa m_pGSA;
  pnmea_point m_pNMEAPoint;
  DWORD * m_pUserData;
  double m_dLastTime;
  TStringList * m_pNMEAStrings;
  DWORD m_dwNMEAMask;
  double __fastcall Time2UNIX(unsigned long ulDate, double dTime);
  bool __fastcall IsHex(char ch);
  void __fastcall CompleteNMEAPoint();
  static int custom_gpgsa(nmeap_context_t * context, nmeap_sentence_t * sentence);
  void __fastcall InitNMEAPointStruct();
  int __fastcall ParseBuffer();
private:
  PGPXTrack m_pTrack;
  bool m_bReady;
  int m_nBufferSize;
  char * __fastcall GetBuffer();
  PGPXTrack __fastcall GetTrack();
  bool __fastcall GetReady();
  int __fastcall GetBufferSize();
  int __fastcall GetStringsCount();
};

#endif
