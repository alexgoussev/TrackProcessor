//---------------------------------------------------------------------------

#ifndef DESH
#define DESH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#include "GPXData.h"
//---------------------------------------------------------------------------
class TDESForm : public TForm
{
__published:	// IDE-managed Components
  TLabeledEdit *SmoothFactor;
  TGroupBox *GroupBox1;
  TCheckBox *Coordinates;
  TCheckBox *Altitude;
  TCheckBox *Speed;
  TButton *Go;
  TButton *Button2;
  TProgressBar *Progress;
  void __fastcall GoClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:
  PGPXTrack m_pCurrentTrack;		// User declarations
  __fastcall TDESForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDESForm *DESForm;
//---------------------------------------------------------------------------
#endif
