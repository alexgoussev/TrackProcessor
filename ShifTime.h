//---------------------------------------------------------------------------

#ifndef ShifTimeH
#define ShifTimeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <cspin.h>

#include "GPXData.h"
//---------------------------------------------------------------------------
class TTimeShift : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *FirstPtTime;
  TEdit *LastPtTime;
  TLabel *Label3;
  TCSpinEdit *ShiftH;
  TCSpinEdit *ShiftM;
  TCSpinEdit *ShiftS;
  TLabel *Label4;
  TLabel *Label5;
  TComboBox *ShiftDir;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  void __fastcall GoClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:
  PGPXTrack m_pCurrentTrack;		// User declarations
  __fastcall TTimeShift(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTimeShift *TimeShift;
//---------------------------------------------------------------------------
#endif
