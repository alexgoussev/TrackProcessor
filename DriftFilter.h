//---------------------------------------------------------------------------

#ifndef DriftFilterH
#define DriftFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "Drift.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TDriftForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TLabeledEdit *Aperture;
  TLabeledEdit *Thereshould;
  TComboBox *Strength;
  TLabel *Label1;
  TLabel *Action;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  void __fastcall GoClick(TObject *Sender);
private:	// User declarations
public:
  PGPXTrack m_pCurrentTrack;		// User declarations
  __fastcall TDriftForm(TComponent* Owner);
  void __fastcall OnPoint(TMessage & msg);
protected:
  BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(DRIFTFILTER_POINT, TMessage, OnPoint)
  END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TDriftForm *DriftForm;
//---------------------------------------------------------------------------
#endif


