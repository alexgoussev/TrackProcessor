//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <io.h>
#include <dir.h>
#pragma hdrstop

#include "Profiler.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TProfiler *pProfiler = NULL;

__fastcall TProfiler::TProfiler()
{
  m_asFileName = ChangeFileExt(Application->ExeName, ".trc");
}

__fastcall TProfiler::~TProfiler()
{
  //TODO: Add your source code here
}

void __fastcall TProfiler::Write(const char *pStr)
{
  FILE *pDebug = fopen(m_asFileName.c_str(), "a+");

  if(pDebug)
  {
    SYSTEMTIME SysTime;
    GetLocalTime(&SysTime);

    fprintf(pDebug, "%04hu-%02hu-%02hu %02hu:%02hu:%02hu.%03hu - %s\n",
            SysTime.wYear, SysTime.wMonth, SysTime.wDay,
            SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds,
            pStr);
    fclose(pDebug);
  }
}
