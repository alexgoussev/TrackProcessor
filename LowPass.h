//---------------------------------------------------------------------------

#ifndef LowPassH
#define LowPassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "GPXData.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TLowPassForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TGroupBox *GroupBox1;
  TLabeledEdit *Smoothing;
  TCheckBox *Altitude;
  TCheckBox *Speed;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  TCheckBox *Coordinates;
  void __fastcall GoClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:
  PGPXTrack m_pCurrentTrack;		// User declarations
  __fastcall TLowPassForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLowPassForm *LowPassForm;
//---------------------------------------------------------------------------
#endif
