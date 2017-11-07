//---------------------------------------------------------------------------

#ifndef HODRECKPRESCOTT
#define HODRECKPRESCOTT
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
struct tagDataSet {
  double *pX, *pY;
  double *pA;
  //double *pS;
  double *pVx;
  double *pVy;
};

struct tagHPData {
  double *a, *b, *c;
  double H1, H2, H3, H4, H5;
  double HH1, HH2, HH3, HH4, HH5;
  double Z;
  double HB, HC;
};

typedef tagDataSet* PDataSet;
typedef tagHPData* PHPData;

class THPForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TLabel *Label1;
  TGroupBox *GroupBox1;
  TCheckBox *m_pCoordinates;
  TCheckBox *m_pAltitude;
  TCheckBox *m_pSpeed;
  TButton *Button1;
  TButton *Button2;
  TProgressBar *ProgressBar;
  TEdit *m_pLambda;
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall OnLambdaKeyPress(TObject *Sender, char &Key);
private:
  void __fastcall SetTrack(PGPXTrack value);
  PGPXTrack __fastcall GetTrack();	// User declarations
public:
  		// User declarations
  __fastcall THPForm(TComponent* Owner);
  __fastcall ~THPForm();
  __property PGPXTrack Track  = { read=GetTrack, write=SetTrack };
protected:
  double m_dLambda;
  int m_nProgress;
  PGPXTrack m_pTrack;
  void __fastcall ProcessHP();
  int __fastcall HPFilterProc(PDataSet pInput, int nobs, double lambda, PDataSet pOutput);
};
//---------------------------------------------------------------------------
extern PACKAGE THPForm *HPForm;
//---------------------------------------------------------------------------
#endif
