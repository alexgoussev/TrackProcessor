//---------------------------------------------------------------------------

#ifndef SplineSmoothH
#define SplineSmoothH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>

#include "GPXData.h"
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class TSplineSmoothForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TLabel *Label1;
  TEdit *BaseSmoothFactor;
  TLabel *Label2;
  TGroupBox *GroupBox1;
  TGroupBox *GroupBox2;
  TLabel *Label3;
  TEdit *HDOPIncr;
  TLabel *Label4;
  TEdit *HDOPStep;
  TLabel *Label5;
  TEdit *HDOPLevel;
  TGroupBox *GroupBox3;
  TLabel *Label6;
  TEdit *SpeedIncr;
  TLabel *Label7;
  TEdit *SpeedStep;
  TLabel *Label8;
  TEdit *SpeedLevel;
  TLabel *Label9;
  TGroupBox *GroupBox4;
  TLabel *Label10;
  TEdit *CourseIncr;
  TLabel *Label11;
  TEdit *CourseStep;
  TLabel *Label12;
  TEdit *CourseLevel;
  TLabel *Label13;
  TGroupBox *GroupBox5;
  TLabel *Label14;
  TEdit *AccIncr;
  TLabel *Label15;
  TEdit *AccStep;
  TLabel *Label16;
  TEdit *AccLevel;
  TLabel *Label17;
  TCheckBox *InstatntSpeed;
  TGroupBox *GroupBox6;
  TCheckBox *SmoothCoordinates;
  TCheckBox *SmoothAltitude;
  TCheckBox *SmoothSpeed;
  TProgressBar *ProgressBar;
  TButton *Button1;
  TButton *Button2;
  TActionList *SmoothActionList;
  TAction *StartAction;
  TCheckBox *InstantCourse;
  void __fastcall OnKeyPress(TObject *Sender, char &Key);
  void __fastcall StartActionExecute(TObject *Sender);
  void __fastcall StartActionUpdate(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
private:
  void __fastcall SetTrack(PGPXTrack value);
  PGPXTrack __fastcall GetTrack();	// User declarations
public:		// User declarations
  __fastcall TSplineSmoothForm(TComponent* Owner);
  __fastcall ~TSplineSmoothForm();
  __property PGPXTrack Track  = { read=GetTrack, write=SetTrack };
protected:
  double m_dBaseSmoothFactor;

  double m_dHDOPIncr;
  double m_dHDOPStep;
  double m_dHDOPLevel;

  double m_dSpeedIncr;
  double m_dSpeedStep;
  double m_dSpeedLevel;

  double m_dCourseIncr;
  double m_dCourseStep;
  double m_dCourseLevel;
  bool m_bInstantCourse;

  double m_dAccIncr;
  double m_dAccStep;
  double m_dAccLevel;
  bool m_bInstatntSpeed;

  bool m_bSmoothCoordinates;
  bool m_bSmoothAltitude;
  bool m_bSmoothSpeed;

  bool m_bInProgress;
  PGPXTrack m_pTrack;
  PGPXTrack m_pFilteredTrack;
  void __fastcall LoadParams();
  void __fastcall SaveParams();
};
//---------------------------------------------------------------------------
extern PACKAGE TSplineSmoothForm *SplineSmoothForm;
//---------------------------------------------------------------------------
#endif
