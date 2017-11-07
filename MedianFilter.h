//---------------------------------------------------------------------------

#ifndef MedianFilterH
#define MedianFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <cspin.h>
#include <ComCtrls.hpp>

#include "GPXData.h"

//---------------------------------------------------------------------------
class TMedianForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TCSpinEdit *Aperture;
  TLabel *Label1;
  TCheckBox *UseRank;
  TEdit *RankCoeff;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  TGroupBox *GroupBox1;
  TCheckBox *Altitude;
  TCheckBox *Speed;
  TCheckBox *Coordinates;
  void __fastcall UseRankClick(TObject *Sender);
  void __fastcall GoClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:
  PGPXTrack m_pCurrentTrack;		// User declarations
  __fastcall TMedianForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMedianForm *MedianForm;
//---------------------------------------------------------------------------
#endif
