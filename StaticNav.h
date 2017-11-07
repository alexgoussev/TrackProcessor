//---------------------------------------------------------------------------

#ifndef StaticNavH
#define StaticNavH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "GPXData.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
enum ePinningState {eNoPinning, eReadyToPin, ePinning, eReadyToUnpin};

class TSNEForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *PinSpeedTh;
  TEdit *PinTimeTh;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  TEdit *UnpinSpeedTh;
  TEdit *UnpinTimeTh;
  TLabel *Label3;
  TLabel *Label4;
  void __fastcall GoClick(TObject *Sender);
private:
  PGPXTrack m_pCurrentTrack;
  void __fastcall SetCurrentTrack(PGPXTrack value);
public:		// User declarations
  __fastcall TSNEForm(TComponent* Owner);
  __property PGPXTrack CurrentTrack  = { write=SetCurrentTrack };
protected:
  double m_dPinSpeed;
  double m_dUnpinSpeed;
  int m_nPinTime;
  int m_nUnpinTime;
};
//---------------------------------------------------------------------------
extern PACKAGE TSNEForm *SNEForm;
//---------------------------------------------------------------------------
#endif
