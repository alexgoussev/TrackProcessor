//---------------------------------------------------------------------------

#ifndef ProfilesH
#define ProfilesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "GPXData.h"
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
//---------------------------------------------------------------------------
#define PROFILE_HEIGHT 300
#define PROFILE_WIDTH  800
#define BOTTOM_SPACE    20
#define LEFT_SPACE      50
#define TOP_SPACE       20
#define RIGHT_SPACE      5

enum SAVEIMG {eSaveAlt, eSaveSpd, eSaveBoth};

class TProfile : public TForm
{
__published:	// IDE-managed Components
  TPanel *AltPanel;
  TPanel *SpdPanel;
  TPaintBox *AltImage;
  TPaintBox *SpdImage;
  TPopupMenu *PopupMenu;
  TActionList *ActionList;
  TSavePictureDialog *SaveProfile;
  TAction *SaveAltAction;
  TAction *SaveSpdAction;
  TAction *SaveBothAction;
  TMenuItem *SaveAltitudeProfile1;
  TMenuItem *SaveSpeedProfile1;
  TMenuItem *SaveBothProfiles1;
  void __fastcall FormResize(TObject *Sender);
  void __fastcall SpdImagePaint(TObject *Sender);
  void __fastcall AltImagePaint(TObject *Sender);
  void __fastcall SaveAltActionExecute(TObject *Sender);
  void __fastcall SaveAltActionUpdate(TObject *Sender);
  void __fastcall SaveSpdActionExecute(TObject *Sender);
  void __fastcall SaveBothActionExecute(TObject *Sender);
  void __fastcall SaveBothActionUpdate(TObject *Sender);
  void __fastcall SaveSpdActionUpdate(TObject *Sender);
private:
  void __fastcall SetCurrentTrack(PGPXTrack value);
  PGPXTrack __fastcall GetCurrentTrack();	// User declarations
public:
  		// User declarations
  __fastcall TProfile(TComponent* Owner);
  __fastcall ~TProfile();
  void __fastcall DrawMetafile(TMetafile *pAltMetafile, TMetafile *pSpdMetafile, int SpdWidth, int SpdHeight, int AltWidth, int AltHeight);
  __property PGPXTrack CurrentTrack  = { read=GetCurrentTrack, write=SetCurrentTrack };
protected:
  PGPXTrack m_pCurrentTrack;
  Graphics::TMetafile * m_pAltMetafile;
  Graphics::TMetafile * m_pSpdMetafile;
  void __fastcall DrawMetafile(int SpdWidth, int SpdHeight, int AltWidth, int AltHeight);
  int __fastcall ValToPix(double Val, double MaxVal, double MinVal, int MaxPix, int MinPix);
  double __fastcall PixToVal(int Pix, int MaxPix, int MinPix, double MaxVal, double MinVal);
  void __fastcall SaveImage(SAVEIMG eSaveType);
};
//---------------------------------------------------------------------------
extern PACKAGE TProfile *Profile;
//---------------------------------------------------------------------------
#endif
