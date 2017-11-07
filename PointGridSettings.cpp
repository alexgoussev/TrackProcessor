//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PointGridSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGridSettings *GridSettings;
//---------------------------------------------------------------------------
__fastcall TGridSettings::TGridSettings(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

