//---------------------------------------------------------------------------

#ifndef CellEqH
#define CellEqH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "GPXData.h"
#include "TrackCells.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

enum eKernelType {ktNoWeights = 0, ktBiQuadratic = 1, ktTriCube = 2, ktGauss = 3};

class TCellEqualizer : public TForm
{
__published:	// IDE-managed Components
  TProgressBar *m_pProgressBar;
  TPanel *Panel1;
  TButton *Button1;
  TButton *Button2;
  TGroupBox *GroupBox1;
  TCheckBox *m_pCoordinates;
  TCheckBox *m_pAltitude;
  TLabel *Label1;
  TEdit *m_pCellSize;
  TLabel *Label2;
  TComboBox *m_pKType;
  TCheckBox *m_pMovDir;
  TEdit *m_pMaxCourseDiff;
  TLabel *Label3;
  void __fastcall GoClick(TObject *Sender);
  void __fastcall CellSizeKeyPress(TObject *Sender, char &Key);
  void __fastcall MovDirClick(TObject *Sender);
  void __fastcall MaxCourseDiffKeyPress(TObject *Sender, char &Key);
private:
  void __fastcall SetTrack(PGPXTrack value);
  PGPXTrack __fastcall GetTrack();	// User declarations
public:		// User declarations
  __fastcall TCellEqualizer(TComponent* Owner);
  __fastcall ~TCellEqualizer();
  __property PGPXTrack Track  = { read=GetTrack, write=SetTrack };
protected:
  PGPXTrack m_pTrack;
  PGPXTrack m_pResultTrack;
  double m_dCellSize;
  bool m_bAltitude;
  bool m_bCoordinates;
  bool m_bMoveDir;
  eKernelType m_eKType;
  int m_nMaxCourseDiff;
  double __fastcall FSquare(double x);
  double __fastcall FCube(double x);
  double __fastcall KernelBiQuadratic(double r);
  double __fastcall KernelTriCube(double r);
  double __fastcall KernelGauss(double r);
  void __fastcall Process();
  double __fastcall Weight(double r);
};
//---------------------------------------------------------------------------
extern PACKAGE TCellEqualizer *CellEqualizer;
//---------------------------------------------------------------------------
#endif
