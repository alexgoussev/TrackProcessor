//---------------------------------------------------------------------------

#ifndef HDOPFilterH
#define HDOPFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <cspin.h>
#include "GPXData.h"
#include "Spline.h"
//---------------------------------------------------------------------------
#define INTERVALS 100
#define BOTTOM_SPACE 20

class THDOPForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TPanel *Panel2;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  TLabel *Label1;
  TCSpinButton *UpperSpin;
  TEdit *UpperTh;
  TLabel *Label2;
  TImage *HDOPDistribution;
  TGroupBox *GroupBox1;
  TEdit *m_pMax;
  TEdit *m_pAvg;
  TEdit *m_pS;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TGroupBox *GroupBox2;
  TLabel *Label6;
  TButton *m_p2S;
  TLabel *Label7;
  TButton *m_p3S;
  TLabel *Label8;
  TLabel *Label9;
  TCSpinEdit *m_pCIPers;
  TLabel *Label10;
  TButton *m_pCI;
  void __fastcall UpperSpinDownClick(TObject *Sender);
  void __fastcall UpperSpinUpClick(TObject *Sender);
  void __fastcall GoClick(TObject *Sender);
  void __fastcall UpperThKeyPress(TObject *Sender, char &Key);
  void __fastcall UpperThExit(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall m_p2SClick(TObject *Sender);
  void __fastcall m_p3SClick(TObject *Sender);
  void __fastcall m_pCIClick(TObject *Sender);
private:	// User declarations
protected:
  double m_dMaxHDOP;
  Graphics::TBitmap *m_pBitmap;
  AkimaSpline *m_pSpline;
  double m_dSigma;
  double m_dAverageHDOP;
  double m_dUpperTh;
  void __fastcall SetLimits();
  void __fastcall CalcStat();
  AnsiString __fastcall RoundStr(double dValue);
  void __fastcall HighestFirst(double *pArray, int n);
public:		// User declarations
  PGPXTrack m_pCurrentTrack;
  __fastcall THDOPForm(TComponent* Owner);
  __fastcall ~THDOPForm();
};
//---------------------------------------------------------------------------
extern PACKAGE THDOPForm *HDOPForm;
//---------------------------------------------------------------------------
#endif


