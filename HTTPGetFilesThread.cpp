//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HTTPGetFilesThread.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------

__fastcall Task::Task(PFileList pFileList, HINTERNET hInternet)
{
  m_dwId = pFileList->dwTaskId;
  m_asHost = pFileList->asHost;
  m_pFiles = pFileList->pFiles;
  m_pBuffer = new byte[BUFFER_SIZE];
  m_hConnect = m_hRequest = NULL;
  m_pReadyFiles = new TList;
  m_pCurrentFile = NULL;
  m_dwTimeout = 3000;

  if(hInternet)
	m_hConnect = InternetConnect(hInternet, String(m_asHost).c_str(),
								 INTERNET_DEFAULT_HTTP_PORT,
                                 NULL,NULL,
                                 INTERNET_SERVICE_HTTP,
								 0, m_dwId);
  m_eStatus = (m_hConnect) ? tsReady : tsError;
}

__fastcall Task::~Task()
{
  if(m_hConnect) InternetCloseHandle(m_hConnect);
  if(m_hRequest) InternetCloseHandle(m_hRequest);

  while(m_pFiles->Count)
  {
    PFileDescr pItem = (PFileDescr)m_pFiles->Items[0];
    delete pItem;
    m_pFiles->Delete(0);
  }

  delete m_pFiles;

  if(m_pCurrentFile)
  {
    if(m_pCurrentFile->pData) delete m_pCurrentFile->pData;
    delete m_pCurrentFile;
  }

  while(m_pReadyFiles->Count)
  {
    PReadyFile pItem = (PReadyFile)m_pReadyFiles->Items[0];
    if(pItem->pData) delete pItem->pData;
    delete pItem;
    m_pReadyFiles->Delete(0);
  }

  delete m_pReadyFiles;

  delete [] m_pBuffer;
}

void __fastcall Task::Touch()
{
  if(m_hConnect)
  {
    if(!m_pCurrentFile)
    {                                                 
      m_pCurrentFile = new tagReadyFile;
      m_pCurrentFile->dwTaskId = m_dwId;
      for(int i = 0; i < FILE_DESCR_ID_SIZE; i++)
        m_pCurrentFile->sFileDescr.dwFileId[i] = ((PFileDescr)m_pFiles->Items[0])->dwFileId[i];
      m_pCurrentFile->sFileDescr.asFileName = ((PFileDescr)m_pFiles->Items[0])->asFileName;
      m_pCurrentFile->dwErrorCode = 0;
      m_pCurrentFile->pData = NULL;
      m_dwCurrentFileLength = 0;
    }

    if(m_eStatus == tsReady || m_eStatus == tsFileReady)
    {
      LPCTSTR rgpszAcceptTypes[] = {TEXT("*/*"), NULL};
	  m_hRequest = HttpOpenRequest(m_hConnect,
								   TEXT("GET"),
								   String(((PFileDescr)m_pFiles->Items[0])->asFileName).c_str(),
								   TEXT("HTTP/1.1"), NULL,
								   rgpszAcceptTypes,
                                   INTERNET_FLAG_KEEP_CONNECTION |
                                   INTERNET_FLAG_NO_CACHE_WRITE  |
                                   INTERNET_FLAG_PRAGMA_NOCACHE  |
                                   INTERNET_FLAG_RELOAD,
								   m_dwId);
                                   
      if(m_hRequest && HttpSendRequest(m_hRequest, NULL, 0, NULL, 0))
      {
        m_eStatus = tsReqSent;
        m_dwWaitFor = GetTickCount() + m_dwTimeout;
      }
      else m_eStatus = tsError;
    }
    else if(m_eStatus == tsReqSent)
    {
      DWORD dwIndex = 0;
      DWORD dwBufferSize = BUFFER_SIZE - 1;

      //if(InternetQueryDataAvailable(m_hRequest, &m_dwCurrentFileLength, 0, 0))
      if(HttpQueryInfo(m_hRequest, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_CONTENT_LENGTH, m_pBuffer, &dwBufferSize, &dwIndex))
      {
        m_dwCurrentFileLength = *((DWORD*)m_pBuffer);
        m_eStatus = tsFileReading;
        m_dwWaitFor = GetTickCount() + m_dwTimeout;
      }
      else if(GetTickCount() > m_dwWaitFor) m_eStatus = tsError;
    }
    else if(m_eStatus == tsFileReading)
    {
      DWORD dwBytesRead = 0;

      if(InternetReadFile(m_hRequest, m_pBuffer, BUFFER_SIZE - 1, &dwBytesRead))
      {
        if(dwBytesRead)
        {
          m_dwWaitFor = GetTickCount() + m_dwTimeout;
          
          if(!m_pCurrentFile->pData) m_pCurrentFile->pData = new TMemoryStream;

          m_pCurrentFile->pData->Write(m_pBuffer, dwBytesRead);

          if(m_pCurrentFile->pData->Size == m_dwCurrentFileLength)
          {
            m_pCurrentFile->pData->Seek(0, soFromBeginning);
            m_pReadyFiles->Add(m_pCurrentFile);
            m_pCurrentFile = NULL;
            m_dwCurrentFileLength = 0;
            PFileDescr pFileDescr = (PFileDescr)m_pFiles->Items[0];
            delete pFileDescr;
            m_pFiles->Delete(0);

            if(!m_pFiles->Count) m_eStatus = tsComplete;
            else m_eStatus = tsFileReady;
          }
        }
        else if(GetTickCount() > m_dwWaitFor) m_eStatus = tsError;
      }
      else m_eStatus = tsError;
    }

    if(m_eStatus == tsError)
    {
      if(m_pCurrentFile->pData)
      {
        delete m_pCurrentFile->pData;
        m_pCurrentFile->pData = NULL;
      }

      m_pCurrentFile->dwErrorCode = GetLastError();
      m_pReadyFiles->Add(m_pCurrentFile);
      m_pCurrentFile = NULL;
      m_dwCurrentFileLength = 0;
      PFileDescr pFileDescr = (PFileDescr)m_pFiles->Items[0];
      delete pFileDescr;
      m_pFiles->Delete(0);

      if(!m_pFiles->Count) m_eStatus = tsComplete;
      else m_eStatus = tsFileReady;
    }
  }
}

bool __fastcall Task::GetConnected()
{
  return(m_hConnect != NULL);
}

DWORD __fastcall Task::GetId()
{
  return m_dwId;
}

TList* __fastcall Task::GetReadyFiles()
{
  return m_pReadyFiles;
}

PReadyFile __fastcall Task::GetReadyFile()
{
  PReadyFile pReadyFile = NULL;

  if(m_pReadyFiles->Count)
  {
    pReadyFile = (PReadyFile)m_pReadyFiles->Items[0];
    m_pReadyFiles->Delete(0);
  }

  return pReadyFile;
}

bool __fastcall Task::GetComplete()
{
  return (m_eStatus == tsComplete);
}

bool __fastcall Task::GetFileReady()
{
  return (m_pReadyFiles->Count > 0);
}

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall THTTPGetFilesThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall THTTPGetFilesThread::THTTPGetFilesThread(bool CreateSuspended)
  : TThread(CreateSuspended)
{
  m_pTaskList = new TList;
  m_pReadyTasks = new TThreadList;
  m_pNewTaskEvent = new TSimpleEvent(false);
  m_pFileReadyEvent = new TSimpleEvent(false);
  m_pDeleteTaskEvent = new TSimpleEvent(false);
  m_dwNewTaskId = 1;
  m_hInternet = NULL;
  m_pNewTask = NULL;
  m_dwCurrentTaskId = 0xFFFFFFFF;
  m_dwTaskToDelete = 0xFFFFFFFF;
}

//---------------------------------------------------------------------------
void __fastcall THTTPGetFilesThread::Execute()
{
  m_hInternet = InternetOpen(TEXT("Mozilla/4.0 (compatible; MSIE 6.0b; Windows NT 5.0; .NET CLR 1.0.2914)"),
                             INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

  if(m_hInternet)
  {
    while(!Terminated)
    {
      if(m_pNewTaskEvent->WaitFor(1) == wrSignaled)
      {
        PTask pNewTask = new Task(m_pNewTask, m_hInternet);
        m_pTaskList->Add(pNewTask);
        delete m_pNewTask;
        m_pNewTask = NULL;

        m_pNewTaskEvent->ResetEvent();
      }

      if(m_pDeleteTaskEvent->WaitFor(1) == wrSignaled)
      {
        for(int i = 0; i < m_pTaskList->Count; i++)
        {
          PTask pTask = (PTask)m_pTaskList->Items[i];

          if(m_dwTaskToDelete == pTask->Id)
          {
            m_pTaskList->Delete(i);
            delete pTask;

            break;
          }
        }

        m_pDeleteTaskEvent->ResetEvent();
      }

      int i = 0;
      while(i < m_pTaskList->Count)
      {
        PTask pTask = (PTask)m_pTaskList->Items[i];

        if(pTask && pTask->Connected)
        {
          pTask->Touch();

          if(pTask->FileReady || pTask->Complete)
          {
            PReadyFile pReadyFile = pTask->ReadyFile;

            while(pReadyFile)
            {
              m_pReadyTasks->LockList()->Add(pReadyFile);
              m_pReadyTasks->UnlockList();

              m_pFileReadyEvent->SetEvent();
              pReadyFile = pTask->ReadyFile;
            }
          }

          if(pTask->Complete && !pTask->FileReady)
          {
            delete pTask;
            m_pTaskList->Delete(i);
          }
          else i++;
        }
      }

      Sleep(5);
    }

    InternetCloseHandle(m_hInternet);
  }
}
//---------------------------------------------------------------------------


__fastcall THTTPGetFilesThread::~THTTPGetFilesThread()
{
  delete m_pFileReadyEvent;
  delete m_pNewTaskEvent;
  delete m_pDeleteTaskEvent;

  while(m_pTaskList->Count)
  {
    PTask pItem = (PTask)m_pTaskList->Items[0];
    delete pItem;
    m_pTaskList->Delete(0);
  }

  delete m_pTaskList;

  TList *pList = m_pReadyTasks->LockList();

  while(pList->Count)
  {
    PReadyFile pTask = (PReadyFile)pList->Items[0];
    delete pTask->pData;
    delete pTask;
    pList->Delete(0);
  }

  m_pReadyTasks->UnlockList();
  delete m_pReadyTasks;
}

DWORD __fastcall THTTPGetFilesThread::AddTask(PFileList pFiles)
{
  DWORD dwTaskId = 0xFFFFFFFF;
  DWORD dwWaitFor = GetTickCount() + 1000;

  while(m_pNewTaskEvent->WaitFor(1) == wrSignaled && dwWaitFor < GetTickCount());

  if(m_pNewTaskEvent->WaitFor(1) == wrTimeout)
  {
    dwTaskId = m_dwNewTaskId++;
    if(m_dwNewTaskId == 0xFFFFFFFF) m_dwNewTaskId = 1;
    pFiles->dwTaskId = dwTaskId;
    m_pNewTask = new tagFileList;
    m_pNewTask->pFiles = new TList;

    m_pNewTask->asHost = pFiles->asHost;
    m_pNewTask->dwTaskId = pFiles->dwTaskId;

    for(int i = 0; i < pFiles->pFiles->Count; i++)
    {
      PFileDescr pNewFileDescr = new tagFileDescr;
      PFileDescr pFiledescr = (PFileDescr)pFiles->pFiles->Items[i];
      for(int j = 0; j < FILE_DESCR_ID_SIZE; j++)
        pNewFileDescr->dwFileId[j] = pFiledescr->dwFileId[j];
      pNewFileDescr->asFileName = pFiledescr->asFileName;

      m_pNewTask->pFiles->Add(pNewFileDescr);
    }

    m_pNewTaskEvent->SetEvent();
  }

  return dwTaskId;
}

PReadyFile __fastcall THTTPGetFilesThread::GetReadyFile()
{
  PReadyFile pReadyFile = NULL;
  TList *pList = m_pReadyTasks->LockList();

  if(pList->Count)
  {
    pReadyFile = (PReadyFile)pList->Items[0];
    pList->Delete(0);

    if(!pList->Count) m_pFileReadyEvent->ResetEvent();
  }

  m_pReadyTasks->UnlockList();

  return pReadyFile;
}

bool __fastcall THTTPGetFilesThread::GetFileReady()
{
  return (m_pFileReadyEvent->WaitFor(1) == wrSignaled);
}

bool __fastcall THTTPGetFilesThread::DeleteTask(DWORD dwTaskId)
{
  bool brc = false;
  DWORD dwWaitFor = GetTickCount() + 1000;

  while(m_pDeleteTaskEvent->WaitFor(1) == wrSignaled && dwWaitFor < GetTickCount());

  if(m_pDeleteTaskEvent->WaitFor(1) == wrTimeout)
  {
    m_dwTaskToDelete = dwTaskId;
    m_pDeleteTaskEvent->SetEvent();
  }
  
  return brc;
}

bool __fastcall THTTPGetFilesThread::GetComplete()
{
  return (m_pTaskList->Count == 0);
}
