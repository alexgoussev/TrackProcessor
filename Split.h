//---------------------------------------------------------------------------

#ifndef SplitH
#define SplitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#include "GPXData.h"
//---------------------------------------------------------------------------
class TSplitForm : public TForm
{
__published:	// IDE-managed Components
  TButton *Go;
  TButton *Cancel;
  TEdit *Length;
  TProgressBar *ProgressBar;
  TEdit *Distance;
  TEdit *HourD;
  TEdit *MinD;
  TEdit *SecD;
  TEdit *SecL;
  TEdit *MinL;
  TEdit *HourL;
  TRadioButton *TrackDuration;
  TRadioButton *PointsInterval;
  TRadioButton *TrackLength;
  TRadioButton *PointsDistance;
  TPanel *Panel1;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  void __fastcall GoClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall CriterionClick(TObject *Sender);
private:	// User declarations
  TRadioButton *m_pCriterion[4];
  int m_nCriterion;
public:
  PGPXTrack m_pCurrentTrack; // User declarations
  TList *m_pResult;
  __fastcall TSplitForm(TComponent* Owner);
  __fastcall ~TSplitForm();
protected:
  void __fastcall CopyPointToTrack(PGPXTrack pTrack, PGPXTrackPoint pPoint);
  PGPXTrack __fastcall AddTrackToResult(PGPXTrack pNewTrack);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplitForm *SplitForm;
//---------------------------------------------------------------------------
#endif
