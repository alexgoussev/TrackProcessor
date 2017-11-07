//---------------------------------------------------------------------------

#ifndef OSMCacheH
#define OSMCacheH
//---------------------------------------------------------------------------

#define OSMMAP_MSG_TILECACHE      (WM_APP + 975)
#define OSMMAP_MSG_TILEMEMORY     (WM_APP + 976)

enum eMapType {mpLandscape, mpCycle, mpStreetMap, mpOutdoors, mpTopo};

typedef TMemoryStream* PMemoryStream;

struct tagMemoryCacheEntry {
  DWORD dwTicks;
  eMapType eType;
  int nZ;
  int nX;
  int nY;
  bool bSaved;
  PMemoryStream pStream;
  AnsiString asFileName;
};

typedef tagMemoryCacheEntry* PMemoryCacheEntry;

class TMemoryCache {
public:
  __fastcall TMemoryCache(int nCacheSize = 128);
  __fastcall ~TMemoryCache();
  bool __fastcall GetTile(eMapType eType, int nX, int nY, int nZ, PMemoryStream pStream);
  void __fastcall PutTile(eMapType eType, int nX, int nY, int nZ, PMemoryStream pStream, AnsiString asFileName, bool bSave);
  void __fastcall Idle();
  void __fastcall SetFileCurrentTime(AnsiString asFileName);
  __property TStringList * CachePathes  = { write=SetCachePathes };
protected:
  int m_nCacheSize;
  PMemoryCacheEntry m_pCache;
  TStringList *m_pCachePathes;
  void __fastcall SaveFile(eMapType eType, int nX, int nY, int nZ, AnsiString asFName, TMemoryStream * pStream);
private:
  void __fastcall SetCachePathes(TStringList * value);
};

class TOSMCache {
public:
  __fastcall TOSMCache();
  __fastcall ~TOSMCache();
  bool __fastcall GetTile(eMapType eType, int nX, int nY, int nZ, AnsiString asFName, PMemoryStream pStream);
  void __fastcall PutTile(eMapType eType, int nX, int nY, int nZ, AnsiString asFName, TMemoryStream * pStream);
  AnsiString __fastcall GetCacheFileName(eMapType eType, int nX, int nY, int nZ);
  void __fastcall Init();
  void __fastcall Idle();
protected:
  AnsiString m_asCachePath;
  TStringList *m_pCachePathes;
  unsigned __int64 m_i64FileAge;
  TMemoryCache * m_pMemoryCache;
  bool __fastcall CheckFileTime(AnsiString asFileName);
};

#endif
