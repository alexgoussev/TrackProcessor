//---------------------------------------------------------------------------

#ifndef TrackStatH
#define TrackStatH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "GPXData.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

enum eSegType {stStop, stFlat, stDown, stUp};

class TTrackStatForm : public TForm
{
__published:	// IDE-managed Components
  TGroupBox *GroupBox1;
  TGroupBox *GroupBox2;
  TGroupBox *GroupBox3;
  TGroupBox *GroupBox4;
  TGroupBox *GroupBox5;
  TGroupBox *GroupBox6;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label7;
  TLabel *Label8;
  TLabel *Label9;
  TLabel *Label10;
  TLabel *Label11;
  TLabel *Label12;
  TLabel *Label13;
  TLabel *Label14;
  TLabel *Label15;
  TLabel *Label16;
  TLabel *Label17;
  TLabel *Label18;
  TLabel *Label19;
  TLabel *Label20;
  TLabel *Label21;
  TLabel *Label22;
  TLabel *Label23;
  TLabel *Label24;
  TLabel *Label25;
  TLabel *Label26;
  TLabel *Label27;
  TButton *Save;
  TLabel *TimeTotal;
  TLabel *TimeMovement;
  TLabel *TimeStop;
  TLabel *TimeUphill;
  TLabel *TimeDownhill;
  TLabel *TimeFlat;
  TLabel *AltMax;
  TLabel *AltMin;
  TLabel *AltDiff;
  TLabel *AltAccent;
  TLabel *AltDescent;
  TLabel *AltRelAcc;
  TLabel *AltRelDesc;
  TLabel *ClimbUphill;
  TLabel *ClimbDownhill;
  TLabel *DistTotal;
  TLabel *DistUphill;
  TLabel *DistDownhill;
  TLabel *DistFlat;
  TLabel *SpdMax;
  TLabel *SpdAvg;
  TLabel *SpdMovAvg;
  TLabel *SpdAvgUp;
  TLabel *SpdAvgDn;
  TLabel *IndexGPSies;
  TLabel *IndexClimbByBike;
  TLabel *IndexFIETS;
  TLabel *Label28;
  TLabel *Label29;
  TLabel *Label30;
  TPanel *Panel2;
  TGroupBox *GroupBox7;
  TButton *Set;
  TEdit *SetMinSpeed;
  TEdit *SetMinClimb;
  TEdit *SetMinAccend;
  TLabel *Label31;
  TLabel *SpdAvgFlat;
  TSaveDialog *SaveStatDialog;
  void __fastcall SaveClick(TObject *Sender);
  void __fastcall CancelClick(TObject *Sender);
  void __fastcall SetClick(TObject *Sender);
  void __fastcall SetMinSpeedKeyPress(TObject *Sender, char &Key);
  void __fastcall SetMinClimbKeyPress(TObject *Sender, char &Key);
private:
  void __fastcall SetCurrentTrack(PGPXTrack value);
  PGPXTrack __fastcall GetCurrentTrack();	// User declarations
public:		// User declarations
  __fastcall TTrackStatForm(TComponent* Owner);
  __property PGPXTrack CurrentTrack  = { read=GetCurrentTrack, write=SetCurrentTrack };
protected:
  PGPXTrack m_pCurrentTrack;
  double m_dMinSpeed;
  double m_dMinClimb;
  double m_dMinAccend;
  TLabel *m_pLabels[56];
  TList * m_pStopSegs;
  TList * m_pMovingUpSegs;
  TList * m_pMovingDownSegs;
  void __fastcall ClearFields();
  void __fastcall CalcStat();
  AnsiString __fastcall TimeString(double dTime);
  AnsiString __fastcall DistanceString(double dDist);
  AnsiString __fastcall ClimbString(double dClimb);
  AnsiString __fastcall AltString(double dAlt);
  AnsiString __fastcall SpeedString(double dSpeed);
  AnsiString __fastcall IndexString(double dIndex);
  double __fastcall GPSIESIndex(double dAccent, double dDistance, double dMaxAlt);
  double __fastcall ClimbByBikeIndex(double dAccent, double dDistance, double dMaxAlt);
  double __fastcall FIETSIndex(double dAccent, double dDistance, double dMaxAlt);
  PGPXTrack __fastcall NewSegment(int i, eSegType eSegment);
  eSegType __fastcall SegmentType(int i);
  void __fastcall AddPointToSegment(PGPXTrack pSegment, int i);
};
//---------------------------------------------------------------------------
extern PACKAGE TTrackStatForm *TrackStatForm;
//---------------------------------------------------------------------------
#endif
