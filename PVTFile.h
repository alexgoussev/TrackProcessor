//---------------------------------------------------------------------------

#ifndef PVTFileH
#define PVTFileH
//---------------------------------------------------------------------------

#include "NMEAParser.h"

#define PVTFILE_MSG_NEWTRACK    (WM_APP + 600)

struct tagPVTHeader {
  DWORD dwDummy;
  DWORD dwLength;
};

class TPVTFile {

public:
  __fastcall TPVTFile();
  __fastcall TPVTFile(AnsiString asFileName);
  __fastcall ~TPVTFile();
  void __fastcall Load();
  void __fastcall Load(AnsiString asFileName);
  __property PGPXTrack Track  = { read=GetTrack };
  __property AnsiString File  = { read=GetFile, write=SetFile };
private:
  AnsiString m_asFileName;
  PGPXTrack __fastcall GetTrack();
  void __fastcall SetFile(AnsiString value);
  AnsiString __fastcall GetFile();
protected:
  TNMEAParser* m_pParser;
  bool m_bMsgSent;
  bool __fastcall IsHex(char ch);
};

#endif
