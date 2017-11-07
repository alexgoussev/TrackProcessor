//---------------------------------------------------------------------------

#ifndef ConfidenceH
#define ConfidenceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <cspin.h>

#include "GPXData.h"
#include "Spline.h"

//---------------------------------------------------------------------------

#define INTERVALS 100
#define BOTTOM_SPACE 20

class TConfidenceForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TPanel *Panel2;
  TImage *m_pResDistr;
  TLabel *Label1;
  TEdit *m_pBase;
  TLabel *Label2;
  TEdit *m_pTrend;
  TSpeedButton *m_pExchange;
  TProgressBar *m_pProgress;
  TLabel *Label3;
  TGroupBox *GroupBox1;
  TLabel *Label4;
  TEdit *m_pMax;
  TLabel *Label5;
  TEdit *m_pAvg;
  TLabel *Label6;
  TEdit *m_pS;
  TCSpinButton *UpperTh;
  TEdit *m_pUpperThereshould;
  TLabel *Label7;
  TGroupBox *GroupBox2;
  TLabel *Label8;
  TButton *m_p2S;
  TLabel *Label9;
  TButton *m_p3S;
  TLabel *Label10;
  TLabel *Label11;
  TCSpinEdit *m_pConfIntVal;
  TLabel *Label12;
  TButton *m_pConfidence;
  TButton *m_pGo;
  TButton *Button1;
  TCheckBox *m_pReplace;
  void __fastcall ExchangeClick(TObject *Sender);
  void __fastcall UpperThereshouldKeyPress(TObject *Sender, char &Key);
  void __fastcall UpperThDownClick(TObject *Sender);
  void __fastcall UpperThUpClick(TObject *Sender);
  void __fastcall S2Click(TObject *Sender);
  void __fastcall S3Click(TObject *Sender);
  void __fastcall ConfidenceClick(TObject *Sender);
  void __fastcall GoClick(TObject *Sender);
  void __fastcall UpperThereshouldExit(TObject *Sender);
private:
  PGPXTrack __fastcall GetResultTrack();
  bool __fastcall GetExchanged();	// User declarations
public:		// User declarations
  __fastcall TConfidenceForm(TComponent* Owner);
  __fastcall ~TConfidenceForm();
  void __fastcall SetTracks(PGPXTrack pBase, PGPXTrack pTrend);
  __property PGPXTrack ResultTrack  = { read=GetResultTrack };
  __property bool Exchanged  = { read=GetExchanged };
protected:
  PGPXTrack m_pBaseTrack;
  PGPXTrack m_pTrendTrack;
  PGPXTrack m_pResultTrack;
  double * m_pDeviations;
  int m_nStartPoint;
  int m_nEndPoint;
  double m_dMaxDev;
  double m_dAvgDev;
  double m_dSigma;
  double m_dUpTh;
  bool m_bExhanged;
  AkimaSpline * m_pSplineX;
  AkimaSpline * m_pSplineY;
  AkimaSpline * m_pSplineD;
  Graphics::TBitmap * m_pBitmap;
  int m_nPoints;
  void __fastcall CalcStat();
  AnsiString __fastcall RoundStr(double dValue);
  void __fastcall DrawDistribution();
  void __fastcall SetLimits();
  void __fastcall HighestFirst(double *pArray, int n);
};
//---------------------------------------------------------------------------
extern PACKAGE TConfidenceForm *ConfidenceForm;
//---------------------------------------------------------------------------
#endif


