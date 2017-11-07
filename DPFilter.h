//---------------------------------------------------------------------------

#ifndef DPFilterH
#define DPFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#include "DouglasPeucker.h"
//---------------------------------------------------------------------------
class TDPForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TLabeledEdit *Epsilon;
  TButton *Go;
  TButton *Exit;
  TProgressBar *ProgressBar;
  void __fastcall GoClick(TObject *Sender);
private:	// User declarations
public:
  PGPXTrack m_pCurrentTrack;		// User declarations
  __fastcall TDPForm(TComponent* Owner);
  void __fastcall OnPoint(TMessage & msg);
protected:
  BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(DPFILTER_POINT, TMessage, OnPoint)
  END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TDPForm *DPForm;
//---------------------------------------------------------------------------
#endif


