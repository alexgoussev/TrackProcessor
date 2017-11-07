//---------------------------------------------------------------------------

#ifndef WAvgFilterH
#define WAvgFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "WAvg.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TWAvgForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TGroupBox *GroupBox1;
  TLabel *Label1;
  TEdit *Distance;
  TLabel *Label2;
  TEdit *Time;
  TLabel *Label3;
  TEdit *Speed;
  TLabel *Label4;
  TEdit *Angle;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  TLabel *Status;
  TLabel *Label5;
  TEdit *AvgTime;
  TEdit *SmoothFactor;
  TLabel *Label6;
  TGroupBox *GroupBox2;
  TCheckBox *Altitude;
  TCheckBox *SpeedCourse;
  void __fastcall GoClick(TObject *Sender);
private:	// User declarations
public:
  PGPXTrack m_pCurrentTrack;		// User declarations
  __fastcall TWAvgForm(TComponent* Owner);
  void __fastcall OnPoint(TMessage & msg);
protected:
  BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(WAVGFILTER_POINT, TMessage, OnPoint)
  END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TWAvgForm *WAvgForm;
//---------------------------------------------------------------------------
#endif
