//---------------------------------------------------------------------------

#ifndef ProfilerH
#define ProfilerH
//---------------------------------------------------------------------------

//#define USE_PROFILER

class TProfiler {

public:
  __fastcall TProfiler();
  __fastcall ~TProfiler();
  void __fastcall Write(const char *pStr);
protected:
  AnsiString m_asFileName;
};

extern TProfiler *pProfiler;

#endif
