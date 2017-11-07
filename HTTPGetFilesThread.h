//---------------------------------------------------------------------------

#ifndef HTTPGetFilesThreadH
#define HTTPGetFilesThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <System.SyncObjs.hpp>
#include <Wininet.h>

//---------------------------------------------------------------------------
#define     FILE_DESCR_ID_SIZE      2

struct tagFileDescr {
  DWORD dwFileId[FILE_DESCR_ID_SIZE];
  AnsiString asFileName;
};

struct tagFileList {
  DWORD dwTaskId;
  AnsiString asHost;
  TList *pFiles;
};

struct tagReadyFile {
  DWORD dwTaskId;
  tagFileDescr sFileDescr;
  DWORD dwErrorCode;
  TMemoryStream *pData;
};

typedef tagFileDescr* PFileDescr;
typedef tagFileList* PFileList;
typedef tagReadyFile* PReadyFile;

enum eTaskStatus {tsReady, tsReqSent, tsFileReading, tsFileReady, tsComplete, tsError};

#define BUFFER_SIZE     65536

class Task {

public:
  __fastcall Task(PFileList pFileList, HINTERNET hInternet);
  __fastcall ~Task();
  void __fastcall Touch();
  __property bool Connected  = { read=GetConnected };
  __property DWORD Id  = { read=GetId };
  __property TList* ReadyFiles  = { read=GetReadyFiles };
  __property PReadyFile ReadyFile  = { read=GetReadyFile };
  __property bool Complete  = { read=GetComplete };
  __property bool FileReady  = { read=GetFileReady };
protected:
  eTaskStatus m_eStatus;
  byte* m_pBuffer;
  TList* m_pReadyFiles;
  PReadyFile m_pCurrentFile;
  DWORD m_dwCurrentFileLength;
  DWORD m_dwWaitFor;
  DWORD m_dwTimeout;
  TMemoryStream* m_pStream;
  DWORD m_dwId;
  AnsiString m_asHost;
  TList* m_pFiles;
  HINTERNET m_hConnect;
  HINTERNET m_hRequest;
private:
  bool __fastcall GetConnected();
  DWORD __fastcall GetId();
  TList* __fastcall GetReadyFiles();
  PReadyFile __fastcall GetReadyFile();
  bool __fastcall GetComplete();
  bool __fastcall GetFileReady();
};

typedef Task* PTask;

class THTTPGetFilesThread : public TThread
{            
private:
  PReadyFile __fastcall GetReadyFile();
  bool __fastcall GetFileReady();
  bool __fastcall GetComplete();
protected:
  HINTERNET m_hInternet;
  DWORD m_dwNewTaskId;
  DWORD m_dwCurrentTaskId;
  DWORD m_dwTaskToDelete;
  PFileList m_pNewTask;
  TList* m_pTaskList;
  TThreadList *m_pReadyTasks;
  TSimpleEvent *m_pNewTaskEvent;
  TSimpleEvent *m_pFileReadyEvent;
  TSimpleEvent *m_pDeleteTaskEvent;
  void __fastcall Execute();
public:
  __fastcall THTTPGetFilesThread(bool CreateSuspended);
  __fastcall ~THTTPGetFilesThread();
  DWORD __fastcall AddTask(PFileList pFiles);
  bool __fastcall DeleteTask(DWORD deTaskId);
  __property PReadyFile ReadyFile  = { read=GetReadyFile };
  __property bool FileReady  = { read=GetFileReady };
  __property bool Complete  = { read=GetComplete };
};
//---------------------------------------------------------------------------
#endif
