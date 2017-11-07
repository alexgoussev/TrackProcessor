//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PVTFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TPVTFile::TPVTFile()
{
  m_pParser = new TNMEAParser;
  m_bMsgSent = false;
}

__fastcall TPVTFile::TPVTFile(AnsiString asFileName)
{
  m_pParser = new TNMEAParser;
  m_asFileName = asFileName;
  m_bMsgSent = false;
}

__fastcall TPVTFile::~TPVTFile()
{
  delete m_pParser;
}

void __fastcall TPVTFile::Load()
{
  if(m_pParser->Ready)
  {
    TMemoryStream *pStream = new TMemoryStream;
    char *pBuffer = m_pParser->Buffer;
    tagPVTHeader sPVTHdr;

    pStream->LoadFromFile(m_asFileName);
    //pStream->Seek(0, soFromBeginning);

	while(pStream->Read(&sPVTHdr, sizeof(sPVTHdr)) == (int)sizeof(sPVTHdr))
    {
      if(pStream->Read(pBuffer, sPVTHdr.dwLength) == (int)sPVTHdr.dwLength)
      {
        pBuffer[sPVTHdr.dwLength] = 0;
        AnsiString asBuffer = AnsiString(pBuffer);
        AnsiString asSentence = asBuffer.SubString(1, 7);

        if(asSentence == AnsiString("$GPGGA,") ||
           asSentence == AnsiString("$GPGSA,") ||
           asSentence == AnsiString("$GPRMC,"))
        {
          if(!m_bMsgSent)
          {
            m_bMsgSent = true;
            SendMessage(Application->MainForm->Handle, PVTFILE_MSG_NEWTRACK,
                        0, (LPARAM)(ExtractFileName(ChangeFileExt(m_asFileName, "")).c_str()));
            Application->ProcessMessages();
          }

          m_pParser->AddString(asBuffer);
        }
      }
      else break;
    }

    /*
    bool inMessage = false;
    char *pBuffer = m_pParser->Buffer;
    int nBuffSize = m_pParser->BufferSize - 4;
    int pos = 0, countdown = 0;
    unsigned char ch;

    while(pStream->Read(&ch, 1))
    {
      if(ch == '$')
      {
        inMessage = true;
        pos = 0;
        countdown = 0;
        pBuffer[pos++] = ch;
        continue;
      }

      if(inMessage)
      {
        pBuffer[pos++] = ch;

        if(pos == 7)
        {
          pBuffer[pos] = '\0';

          if(strcmp(pBuffer, "$GPGGA,") &&
             strcmp(pBuffer, "$GPGSA,") &&
             strcmp(pBuffer, "$GPRMC,"))
          {
            pos = 0;
            countdown = 0;
            inMessage = false;
          }

          continue;
        }
        else if(pos > nBuffSize)
        {
          pos = 0;
          countdown = 0;
          inMessage = false;
          continue;
        }

        if(ch == '*')
        {
          countdown = 2;
          continue;
        }

        if(countdown)
        {
          if(!IsHex(ch))
          {
            pos = 0;
            countdown = 0;
            inMessage = false;
            continue;
          }

          countdown--;

          if(!countdown)
          {
            pBuffer[pos] = '\0';
            pos = 0;
            countdown = 0;
            inMessage = false;

            if(!m_bMsgSent)
            {
              m_bMsgSent = true;
              SendMessage(Application->MainForm->Handle, PVTFILE_MSG_NEWTRACK,
                          0, (LPARAM)(ExtractFileName(ChangeFileExt(m_asFileName, "")).c_str()));
              Application->ProcessMessages();
            }

            //m_pParser->ParseBuffer();
            m_pParser->AddString(AnsiString(pBuffer));
          }
        }
      }
    }
    */

    delete pStream;
    m_pParser->ParseStrings();
  }
}

void __fastcall TPVTFile::Load(AnsiString asFileName)
{
  m_asFileName = asFileName;
  Load();
}

PGPXTrack __fastcall TPVTFile::GetTrack()
{
  PGPXTrack pTrack = NULL;

  if(m_pParser->Ready)
  {
    pTrack = m_pParser->Track;

    if(pTrack)
    {
      pTrack->m_asSrc = AnsiString("Magellan eXplorist x10 debug file");
      pTrack->m_asName = ExtractFileName(ChangeFileExt(m_asFileName, ""));
      pTrack->m_asFileName = ExtractFileName(m_asFileName);
    }
  }

  return pTrack;
}

void __fastcall TPVTFile::SetFile(AnsiString value)
{
  if(m_asFileName != value) {
    m_asFileName = value;
  }
}

AnsiString __fastcall TPVTFile::GetFile()
{
  return m_asFileName;
}

bool __fastcall TPVTFile::IsHex(char ch)
{
  return ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F'));
}