//---------------------------------------------------------------------------

#ifndef GPSKalmanFormH
#define GPSKalmanFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include "GPXData.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TGPSKalmanFilter : public TForm
{
__published:	// IDE-managed Components
  TProgressBar *ProgressBar;
  TButton *m_Ok;
  TButton *m_Cancel;
  TLabel *Label3;
  TEdit *RValue;
  TCheckBox *SaveSpeed;
  TPanel *Panel1;
  TCheckBox *SaveAlt;
  void __fastcall m_OkClick(TObject *Sender);
  void __fastcall ValueKeyPress(TObject *Sender, char &Key);
private:
  PGPXTrack m_pCurrentTrack;
  PGPXTrack m_pFilteredTrack;
  void __fastcall SetPCurrentTrack(PGPXTrack value);
  PGPXTrack __fastcall GetPCurrentTrack();	// User declarations
public:		// User declarations
  __fastcall TGPSKalmanFilter(TComponent* Owner);
  __property PGPXTrack PCurrentTrack  = { read=GetPCurrentTrack, write=SetPCurrentTrack };
};
//---------------------------------------------------------------------------
extern PACKAGE TGPSKalmanFilter *GPSKalmanFilter;
//---------------------------------------------------------------------------
#endif
