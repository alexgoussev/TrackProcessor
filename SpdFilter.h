//---------------------------------------------------------------------------

#ifndef SpdFilterH
#define SpdFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <cspin.h>

#include "GPXData.h"
#include "Spline.h"

//---------------------------------------------------------------------------
#define INTERVALS 100
#define BOTTOM_SPACE 20

class TSpeedFilter : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TImage *SpeedDistribution;
  TPanel *Panel2;
  TLabel *Label1;
  TCSpinButton *LowerSpin;
  TCSpinButton *UpperSpin;
  TEdit *LowerTh;
  TEdit *UpperTh;
  TLabel *Label2;
  TLabel *Label3;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall LowerSpinDownClick(TObject *Sender);
  void __fastcall LowerSpinUpClick(TObject *Sender);
  void __fastcall UpperSpinDownClick(TObject *Sender);
  void __fastcall UpperSpinUpClick(TObject *Sender);
  void __fastcall ThKeyPress(TObject *Sender, char &Key);
  void __fastcall LowerThExit(TObject *Sender);
  void __fastcall UpperThExit(TObject *Sender);
  void __fastcall GoClick(TObject *Sender);
private:	// User declarations
protected:
  double m_dMaxSpeed;
  Graphics::TBitmap *m_pBitmap;
  AkimaSpline *m_pSpline;
  void __fastcall SetLimits();
public:		// User declarations
  PGPXTrack m_pCurrentTrack;
  __fastcall TSpeedFilter(TComponent* Owner);
  __fastcall ~TSpeedFilter();
};
//---------------------------------------------------------------------------
extern PACKAGE TSpeedFilter *SpeedFilter;
//---------------------------------------------------------------------------
#endif
