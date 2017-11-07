//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ImgSizeForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImgSize *ImgSize;
//---------------------------------------------------------------------------
__fastcall TImgSize::TImgSize(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TImgSize::GoClick(TObject *Sender)
{
  m_nWidth = Width->Text.ToInt();
  m_nHeight = Height->Text.ToInt();  
}
//---------------------------------------------------------------------------
