//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "StaticNav.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSNEForm *SNEForm;
//---------------------------------------------------------------------------
__fastcall TSNEForm::TSNEForm(TComponent* Owner)
  : TForm(Owner)
{
  m_pCurrentTrack = NULL;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_dPinSpeed = pini->ReadFloat("Pinning", "PinSpeed", 2.5);
  m_dUnpinSpeed = pini->ReadFloat("Pinning", "UnpinSpeed", 3.0);
  m_nPinTime = pini->ReadInteger("Pinning", "PinTime", 3);
  m_nUnpinTime = pini->ReadInteger("Pinning", "UnpinTime", 3);
  delete pini;

  PinSpeedTh->Text = AnsiString(m_dPinSpeed);
  UnpinSpeedTh->Text = AnsiString(m_dUnpinSpeed);
  PinTimeTh->Text = AnsiString(m_nPinTime);
  UnpinTimeTh->Text = AnsiString(m_nUnpinTime);
}
//---------------------------------------------------------------------------

void __fastcall TSNEForm::SetCurrentTrack(PGPXTrack value)
{
  if(m_pCurrentTrack != value)
    m_pCurrentTrack = value;
}

void __fastcall TSNEForm::GoClick(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    m_dPinSpeed = PinSpeedTh->Text.ToDouble();
    m_dUnpinSpeed = UnpinSpeedTh->Text.ToDouble();
    m_nPinTime = PinTimeTh->Text.ToInt();
    m_nUnpinTime = UnpinTimeTh->Text.ToInt();

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteFloat("Pinning", "PinSpeed", m_dPinSpeed);
    pini->WriteFloat("Pinning", "UnpinSpeed", m_dUnpinSpeed);
    pini->WriteInteger("Pinning", "PinTime", m_nPinTime);
    pini->WriteInteger("Pinning", "UnpinTime", m_nUnpinTime);
    delete pini;

    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    ProgressBar->Max = m_pCurrentTrack->PointCount;
    ProgressBar->Position = 0;

    ePinningState PinState = eNoPinning;
    PGPXTrackPoint pPin;
    double dTime;

    for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
    {
      PGPXTrackPoint p = m_pCurrentTrack->Points[i];
      double dSpeed = p->m_dSpeed * 3.6;

      switch(PinState)
      {
        case eNoPinning:
        {
          if(dSpeed <= m_dPinSpeed)
          {
            dTime = p->m_dTime;
            PinState = eReadyToPin;
          }

          break;
        }

        case eReadyToPin:
        {
          if(dSpeed > m_dPinSpeed) PinState = eNoPinning;
          else if(p->m_dTime - dTime >= m_nPinTime)
          {
            pPin = p;
            PinState = ePinning;
          }

          break;
        }

        case ePinning:
        {
          pPin->m_dLat = (pPin->m_dLat + p->m_dLat) / 2.0;
          pPin->m_dLon = (pPin->m_dLon + p->m_dLon) / 2.0;
          pPin->m_dAlt = (pPin->m_dAlt + p->m_dAlt) / 2.0;
          p->m_bActive = false;

          if(dSpeed >= m_dUnpinSpeed)
          {
            dTime = p->m_dTime;
            PinState = eReadyToUnpin;
          }

          break;
        }

        case eReadyToUnpin:
        {
          pPin->m_dLat = (pPin->m_dLat + p->m_dLat) / 2.0;
          pPin->m_dLon = (pPin->m_dLon + p->m_dLon) / 2.0;
          pPin->m_dAlt = (pPin->m_dAlt + p->m_dAlt) / 2.0;
          p->m_bActive = false;

          if(dSpeed < m_dUnpinSpeed) PinState = ePinning;
          else if(p->m_dTime - dTime >= m_nUnpinTime)
          {
            p->m_dLat = pPin->m_dLat;
            p->m_dLon = pPin->m_dLon;
            p->m_dAlt = pPin->m_dAlt;
            p->m_bActive = true;
            PinState = eNoPinning;
          }
          
          break;
        }
      }

      ProgressBar->Position = i;
      Application->ProcessMessages();
    }

    m_pCurrentTrack->RemoveInactivePoints();
    m_pCurrentTrack->RecalcDistClimb();
    
    Screen->Cursor = Save_Cursor;
  }
}
//---------------------------------------------------------------------------
