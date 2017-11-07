//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "NMEAFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TNMEAFile::TNMEAFile()
{
  m_pParser = new TNMEAParser;
}

__fastcall TNMEAFile::TNMEAFile(AnsiString asFileName)
{
  m_asFileName = asFileName;
  m_pParser = new TNMEAParser;
}

__fastcall TNMEAFile::~TNMEAFile()
{
  delete m_pParser;
}

void __fastcall TNMEAFile::SetFile(AnsiString value)
{
  if(m_asFileName != value) {
    m_asFileName = value;
  }
}

AnsiString __fastcall TNMEAFile::GetFile()
{
  return m_asFileName;
}

PGPXTrack __fastcall TNMEAFile::GetTrack()
{
  PGPXTrack pTrack = NULL;

  if(m_pParser->Ready)
  {
    pTrack = m_pParser->Track;

    if(pTrack)
    {
      pTrack->m_asSrc = AnsiString("NMEA file");
      pTrack->m_asName = ExtractFileName(ChangeFileExt(m_asFileName, ""));
      pTrack->m_asFileName = ExtractFileName(m_asFileName);
    }
  }

  return pTrack;
}

void __fastcall TNMEAFile::Load()
{
  if(m_pParser->Ready)
  {
    m_pParser->LoadFromFile(m_asFileName);

    if(m_pParser->StringsCount)
    {
      SendMessage(Application->MainForm->Handle, NMEAFILE_MSG_NEWTRACK,
                  0, (LPARAM)(ExtractFileName(ChangeFileExt(m_asFileName, "")).c_str()));
      Application->ProcessMessages();
    }

    m_pParser->ParseStrings();
  }
}

void __fastcall TNMEAFile::Load(AnsiString asFileName)
{
  m_asFileName = asFileName;
  Load();
}
