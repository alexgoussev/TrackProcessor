//---------------------------------------------------------------------------

#ifndef PointGridSettingsH
#define PointGridSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TGridSettings : public TForm
{
__published:	// IDE-managed Components
  TRadioGroup *TimeFormat;
  TRadioGroup *CoordFormat;
  TButton *Ok;
  TButton *Cancel;
private:	// User declarations
public:		// User declarations
  __fastcall TGridSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGridSettings *GridSettings;
//---------------------------------------------------------------------------
#endif
