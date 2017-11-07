//---------------------------------------------------------------------------

#ifndef MetadataH
#define MetadataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "GPXData.h"
#include <ActnList.hpp>
#include <System.Actions.hpp>
//---------------------------------------------------------------------------
class TEditMetadata : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TPanel *Panel1;
  TEdit *Name;
  TLabel *Label2;
  TMemo *Description;
  TLabel *Label3;
  TMemo *Comment;
  TLabel *Label4;
  TMemo *Source;
  TButton *Set;
  TButton *Cancel;
  TActionList *EtitMetadataActions;
  TAction *SetAction;
  void __fastcall SetActionExecute(TObject *Sender);
  void __fastcall SetActionUpdate(TObject *Sender);
private:
  PGPXTrack m_pCurrentTrack;
  void __fastcall SetCurrentTrack(PGPXTrack value);
  PGPXTrack __fastcall GetCurrentTrack();	// User declarations
public:		// User declarations
  __fastcall TEditMetadata(TComponent* Owner);
  __property PGPXTrack CurrentTrack  = { read=GetCurrentTrack, write=SetCurrentTrack };
};
//---------------------------------------------------------------------------
extern PACKAGE TEditMetadata *EditMetadata;
//---------------------------------------------------------------------------
#endif
