//---------------------------------------------------------------------------

#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <System.Actions.hpp>
//---------------------------------------------------------------------------
class TSettingsForm : public TForm
{
__published:	// IDE-managed Components
  TGroupBox *GroupBox1;
  TGroupBox *GroupBox2;
  TGroupBox *GroupBox3;
  TLabeledEdit *HGTPath;
  TLabeledEdit *CachePath;
  TButton *HGTPathButton;
  TButton *CachePathButton;
  TLabel *Label1;
  TEdit *TileAge;
  TUpDown *FileAgeUpDown;
  TLabel *Label2;
  TEdit *AvgTime;
  TUpDown *AvgTimeUpDown;
  TLabel *Label3;
  TEdit *ClimbAltDiff;
  TUpDown *ClimbAltDiffUpDown;
  TLabel *Label4;
  TEdit *ClimbMinDist;
  TUpDown *ClimbDistUpDown;
  TLabel *Label5;
  TEdit *ClimbMinSpeed;
  TUpDown *ClimbSpeedUpDown;
  TButton *SaveButton;
  TButton *CancelButton;
  TLabel *Label6;
  TEdit *HDOPFactor;
  TUpDown *HDOPFactprUpDown;
  TLabel *Label7;
  TEdit *ComplimentaryFactor;
  TCheckBox *ResoreSpeed;
  TLabel *Label8;
  TActionList *ActionList1;
  TAction *SaveAction;
  void __fastcall ComplimentaryFactorKeyPress(TObject *Sender, char &Key);
  void __fastcall HGTPathButtonClick(TObject *Sender);
  void __fastcall CachePathButtonClick(TObject *Sender);
  void __fastcall SaveActionExecute(TObject *Sender);
  void __fastcall SaveActionUpdate(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TSettingsForm(TComponent* Owner);
protected:
  AnsiString m_asHGTPath;
  AnsiString m_asMapCache;
  double m_dAvgTime;
  double m_dMinClimbDAlt;
  double m_dMinClimbDist;
  double m_dMinClimbSpeed;
  int m_nFileAge;
  double m_dHDOPFactor;
  double m_dAVGAlpha;
  bool m_bRestoreSpeed;
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsForm *SettingsForm;
//---------------------------------------------------------------------------
#endif
