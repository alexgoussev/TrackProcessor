//---------------------------------------------------------------------------

#ifndef SRTMAltH
#define SRTMAltH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <IdHTTP.hpp>
#include <IdStream.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <System.Zip.hpp>
#include <string>
#include <map>
#include <functional>

#include "GPXData.h"

//---------------------------------------------------------------------------

typedef struct tagHGTDATA {
  char szName[8];
  int  nIndex;
} HGTDATA;

typedef struct tagDIRITEM {
  char szDir[16];
  int  nDirNo;
} DIRITEM;

typedef HGTDATA* LPHGTDATA;
typedef DIRITEM* LPDIRITEM;
typedef std::map<std::string, int> StringMap;

class TSTRMAltitude : public TForm
{
__published:	// IDE-managed Components
  TProgressBar *ProgressBar;
  TIdHTTP *IdHTTP;
  TCheckBox *Download;
  TButton *m_Ok;
  TButton *m_Cancel;
  TLabel *Status;
  TLabeledEdit *AltSmoothFactor;
  TPanel *Panel1;
	TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL;
  void __fastcall HTTPGetSRTMWorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax);
  void __fastcall HTTPGetSRTMWork(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount);
//  void __fastcall ZipProgress(TObject *Sender, String FileName, const TZipHeader &Header, __int64 Position);
  void __fastcall m_OkClick(TObject *Sender);


private:	// User declarations
protected:
  int m_nCurPoint;
  AnsiString m_asHGTPath;
  AnsiString m_asSRTMBaseURL;
  double m_dBeta;
  double __fastcall CalcAlt(AnsiString asFileName, double dLat, double dLon);
  bool __fastcall DownloadFile(AnsiString asFileName);
  double __fastcall Degrees2Seconds(double Degrees);
  double __fastcall ReadDEMFile(int handle, int i, int j);
  int __fastcall GetHGTFileOffset(int nLatI, int nLonJ);
public:
  PGPXTrack m_pTrack;		// User declarations
  __fastcall TSTRMAltitude(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TSTRMAltitude *STRMAltitude;
//---------------------------------------------------------------------------
#endif
