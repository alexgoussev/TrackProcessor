//---------------------------------------------------------------------------
#include <vcl.h>
#include <io.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "OSMCache.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

static const char *pCacheDirNames[] = {"landscape",
                                       "cycle",
                                       "streetmap",
                                       "outdoors",
                                       "topomap"};
__fastcall TOSMCache::TOSMCache()
{
  m_pMemoryCache = new TMemoryCache;
  m_pCachePathes = NULL;

  Init();
}

__fastcall TOSMCache::~TOSMCache()
{
  delete m_pMemoryCache;
  if(m_pCachePathes) delete m_pCachePathes;
}

void __fastcall TOSMCache::Init()
{
  if(m_pCachePathes) delete m_pCachePathes;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));

  m_asCachePath = pini->ReadString("Path", "MapCachePath", "");

  if(m_asCachePath.IsEmpty())
    m_asCachePath = ExcludeTrailingPathDelimiter(ExtractFileDir(Application->ExeName)) + AnsiString("\\cache\\");
  else
    m_asCachePath = IncludeTrailingPathDelimiter(m_asCachePath);

  if(!DirectoryExists(m_asCachePath)) CreateDir(m_asCachePath);

  int nCaches = sizeof(pCacheDirNames) / sizeof(char*);
  m_pCachePathes = new TStringList;
  
  for(int i = 0; i < nCaches; i++)
  {
    AnsiString asPath = m_asCachePath + AnsiString(pCacheDirNames[i]) + AnsiString("\\");
    if(!DirectoryExists(asPath)) CreateDir(asPath);
    m_pCachePathes->Add(asPath);
  }

  m_pMemoryCache->CachePathes = m_pCachePathes;
  
  m_i64FileAge = (unsigned __int64)pini->ReadInteger("Map", "TileAge", 30) *
				 (unsigned __int64)864000000000LL;
  delete pini;
}

bool __fastcall TOSMCache::GetTile(eMapType eType, int nX, int nY, int nZ, AnsiString asFName, PMemoryStream pStream)
{
  bool bResult = m_pMemoryCache->GetTile(eType, nX, nY, nZ, pStream);

  if(!bResult)
  {
    //int nDaysNow = (int)TDateTime::CurrentDate();

    if(FileExists(asFName))
    {
      //int nDaysFile = (int)FileDateToDateTime(FileAge(asFName));
      //int nFileAge = (nDaysNow - nDaysFile);

      //if(m_nFileAge == 0 || nFileAge < m_nFileAge)
      if(CheckFileTime(asFName))
      {
        pStream->LoadFromFile(asFName);
        m_pMemoryCache->PutTile(eType, nX, nY, nZ, pStream, asFName, true);
        bResult = true;

        SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILECACHE, 0, (LPARAM)asFName.c_str());
      }
      else DeleteFile(asFName);
    }
  }
  
  return bResult;
}

void __fastcall TOSMCache::PutTile(eMapType eType, int nX, int nY, int nZ, AnsiString asFName, TMemoryStream * pStream)
{
  if(!asFName.IsEmpty())
    m_pMemoryCache->PutTile(eType, nX, nY, nZ, pStream, asFName, FileExists(asFName));
}

AnsiString __fastcall TOSMCache::GetCacheFileName(eMapType eType, int nX, int nY, int nZ)
{
  AnsiString asFName = m_pCachePathes->Strings[(int)eType] +
                       AnsiString(nZ) + AnsiString("\\")   +
                       AnsiString(nX) + AnsiString("\\")   +
                       AnsiString(nY) + AnsiString(".png");

  return asFName;
}

void __fastcall TOSMCache::Idle()
{
  m_pMemoryCache->Idle();
}

bool __fastcall TOSMCache::CheckFileTime(AnsiString asFileName)
{
  bool bResult = false;

  HANDLE hFile = CreateFile(String(asFileName).c_str(),
                            GENERIC_READ,
                            FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);
                            
  if(hFile != INVALID_HANDLE_VALUE)
  {
    FILETIME sFileTime;

    if(GetFileTime(hFile, &sFileTime, NULL, NULL))
    {
      ULARGE_INTEGER uFileTime;
      ULARGE_INTEGER uCurrentTime;
      FILETIME sCurrentTime;

      GetSystemTimeAsFileTime(&sCurrentTime);

      uFileTime.LowPart = sFileTime.dwLowDateTime;
      uFileTime.HighPart = sFileTime.dwHighDateTime;
      uCurrentTime.LowPart = sCurrentTime.dwLowDateTime;
      uCurrentTime.HighPart = sCurrentTime.dwHighDateTime;

      unsigned __int64 ui64FileAge = uCurrentTime.QuadPart - uFileTime.QuadPart;
      if(ui64FileAge < m_i64FileAge) bResult = true;
    }
    
    CloseHandle(hFile);
  }

  return bResult;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

__fastcall TMemoryCache::TMemoryCache(int nCacheSize)
{
  m_nCacheSize = nCacheSize;
  m_pCache = new tagMemoryCacheEntry[m_nCacheSize];
  m_pCachePathes = NULL;

  for(int i = 0; i < m_nCacheSize; i++)
  {
    m_pCache[i].dwTicks = 0;
    m_pCache[i].bSaved = true;
    m_pCache[i].pStream = new TMemoryStream;
  }
}

__fastcall TMemoryCache::~TMemoryCache()
{
  for(int i = 0; i < m_nCacheSize; i++)
  {
    if(!m_pCache[i].bSaved)
      SaveFile(m_pCache[i].eType, m_pCache[i].nX, m_pCache[i].nY, m_pCache[i].nZ, m_pCache[i].asFileName, m_pCache[i].pStream);

    delete m_pCache[i].pStream;
  }
  delete [] m_pCache;

  if(m_pCachePathes) delete m_pCachePathes;
}

bool __fastcall TMemoryCache::GetTile(eMapType eType, int nX, int nY, int nZ, PMemoryStream pStream)
{
  bool brc = false;

  for(int i = 0; i < m_nCacheSize; i++)
  {
    if(m_pCache[i].eType == eType &&
       m_pCache[i].nZ == nZ       &&
       m_pCache[i].nX == nX       &&
       m_pCache[i].nY == nY)
    {
      m_pCache[i].dwTicks = GetTickCount();
      pStream->LoadFromStream(m_pCache[i].pStream);

      SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILEMEMORY, 0, (LPARAM)m_pCache[i].asFileName.c_str());
      
      brc = true;
      break;
    }
  }

  return brc;
}

void __fastcall TMemoryCache::PutTile(eMapType eType, int nX, int nY, int nZ, PMemoryStream pStream, AnsiString asFileName, bool bSave)
{
  int nCache;
  DWORD dwTicks = GetTickCount();

  for(int i = 0; i < m_nCacheSize; i++)
  {
    if(m_pCache[i].eType == eType &&
       m_pCache[i].nZ == nZ       &&
       m_pCache[i].nX == nX       &&
       m_pCache[i].nY == nY)
    {
      nCache = i;
      break;
    }

    if(m_pCache[i].dwTicks < dwTicks)
    {
      dwTicks = m_pCache[i].dwTicks;
      nCache = i;
    }
  }

  m_pCache[nCache].dwTicks = GetTickCount();
  m_pCache[nCache].eType = eType;
  m_pCache[nCache].nZ = nZ;
  m_pCache[nCache].nX = nX;
  m_pCache[nCache].nY = nY;
  m_pCache[nCache].pStream->LoadFromStream(pStream);
  m_pCache[nCache].asFileName = asFileName;
  m_pCache[nCache].bSaved = bSave;

  //if(bSave) SetFileCurrentTime(asFileName);
}

void __fastcall TMemoryCache::Idle()
{
  if(m_pCachePathes)
  {
	for(int i = 0; i < m_nCacheSize; i++)
    {
      if(!m_pCache[i].bSaved)
      {
		SaveFile(m_pCache[i].eType, m_pCache[i].nX, m_pCache[i].nY, m_pCache[i].nZ, m_pCache[i].asFileName, m_pCache[i].pStream);
		m_pCache[i].bSaved = true;

        break;
      }
    }
  }
}

void __fastcall TMemoryCache::SetCachePathes(TStringList * value)
{
  if(m_pCachePathes) delete m_pCachePathes;
  m_pCachePathes = NULL;

  if(value)
  {
    m_pCachePathes = new TStringList;

    for(int i = 0; i < value->Count; i++)
      m_pCachePathes->Add(value->Strings[i]);
  }
}

void __fastcall TMemoryCache::SaveFile(eMapType eType, int nX, int nY, int nZ, AnsiString asFName, TMemoryStream * pStream)
{
  AnsiString asPName = m_pCachePathes->Strings[(int)eType] + AnsiString(nZ) + AnsiString("\\");
  if(!DirectoryExists(asPName)) CreateDir(asPName);

  asPName += (AnsiString(nX) + AnsiString("\\"));
  if(!DirectoryExists(asPName)) CreateDir(asPName);

  pStream->SaveToFile(asFName);
  SetFileCurrentTime(asFName);
}

void __fastcall TMemoryCache::SetFileCurrentTime(AnsiString asFileName)
{
  HANDLE hFile = CreateFile(String(asFileName).c_str(),
                            GENERIC_WRITE,
                            FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
							NULL);

  if(hFile != INVALID_HANDLE_VALUE)
  {
    FILETIME sFileTime;

    GetSystemTimeAsFileTime(&sFileTime);
    SetFileTime(hFile, &sFileTime, NULL, NULL);
    CloseHandle(hFile);
  }
}


