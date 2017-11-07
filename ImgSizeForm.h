//---------------------------------------------------------------------------

#ifndef ImgSizeFormH
#define ImgSizeFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TImgSize : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TLabel *Label1;
  TEdit *Width;
  TLabel *Label2;
  TEdit *Height;
  TLabel *Label3;
  TButton *Go;
  TButton *Exit;
  void __fastcall GoClick(TObject *Sender);
private:	// User declarations
public:
  int m_nWidth;
  int m_nHeight;		// User declarations
  __fastcall TImgSize(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TImgSize *ImgSize;
//---------------------------------------------------------------------------
#endif
