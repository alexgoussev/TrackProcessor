//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Metadata.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditMetadata *EditMetadata;
//---------------------------------------------------------------------------
__fastcall TEditMetadata::TEditMetadata(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TEditMetadata::SetCurrentTrack(PGPXTrack value)
{
  if(m_pCurrentTrack != value)
  {
    m_pCurrentTrack = value;

    Name->Text = AnsiString("");
    Description->Text = AnsiString("");
    Comment->Text = AnsiString("");
    Source->Text = AnsiString("");

    if(m_pCurrentTrack)
    {
      Name->Text = m_pCurrentTrack->m_asName;
      Description->Text = m_pCurrentTrack->m_asDesc;
      Comment->Text = m_pCurrentTrack->m_asCmt;
      Source->Text = m_pCurrentTrack->m_asSrc;
    }
  }
}

PGPXTrack __fastcall TEditMetadata::GetCurrentTrack()
{
  return m_pCurrentTrack;
}

void __fastcall TEditMetadata::SetActionExecute(TObject *Sender)
{
  m_pCurrentTrack->m_asName = Name->Text;
  m_pCurrentTrack->m_asDesc = Description->Text;
  m_pCurrentTrack->m_asCmt = Comment->Text;
  m_pCurrentTrack->m_asSrc = Source->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditMetadata::SetActionUpdate(TObject *Sender)
{
  if(m_pCurrentTrack)
    SetAction->Enabled = true;
  else
    SetAction->Enabled = false;
}
//---------------------------------------------------------------------------

