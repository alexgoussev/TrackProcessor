//---------------------------------------------------------------------------

#ifndef NMEAFileH
#define NMEAFileH
//---------------------------------------------------------------------------

#include "NMEAParser.h"

#define NMEAFILE_MSG_NEWTRACK    (WM_APP + 600)

class TNMEAFile {

public:
  __fastcall TNMEAFile();
  __fastcall TNMEAFile(AnsiString asFileName);
  __fastcall ~TNMEAFile();
  void __fastcall Load();
  void __fastcall Load(AnsiString asFileName);
  __property AnsiString File  = { read=GetFile, write=SetFile };
  __property PGPXTrack Track  = { read=GetTrack };
private:
  AnsiString m_asFileName;
  PGPXTrack m_pTrack;
  void __fastcall SetFile(AnsiString value);
  AnsiString __fastcall GetFile();
  PGPXTrack __fastcall GetTrack();
protected:
  TNMEAParser * m_pParser;
};

#endif
