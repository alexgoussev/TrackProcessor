//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Profiler.h"
#include "SK42WGS84\\SK42WGS84.h"
#include "OSMMap.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "notile.res"

static const char *pMapNames[] = {"Landscape Cycle Map",
                                  "Open Cycle Map",
                                  "Open Street Map",
                                  "Outdoors Cycle Map",
                                  "Open Topo Map"};

static const int nMaxZoom[] = {20, 20, 20, 20, 20};

__fastcall TOSMMap::TOSMMap(double dMaxLat, double dMaxLong, double dMinLat, double dMinLong, TCanvas * pCanvas, eMapType eType, int nZoom, bool bNoMap, bool bDraw)
{
  m_dMaxLat = dMaxLat;
  m_dMinLat = dMinLat;
  m_dMinLong = dMinLong;
  m_dMaxLong = dMaxLong;
  m_nZoom = nZoom;
  m_dCentreLong = -500.0;
  m_dCentreLat = -500.0;
  m_eMapType = eType;
  m_bDraw = bDraw;
  m_bNoMap = bNoMap;

  m_pOSMCache = new TOSMCache;
  m_pTileStream = new TMemoryStream;
  m_pJPEG = new TJPEGImage;
  m_pPNG = new TPngImage;
  m_pBitmap = NULL;
  m_pCanvas = pCanvas;

  //GetTempPath(MAX_PATH, m_szTempPathBuffer);
  m_pNoMapTile = new TResourceStream((int)HInstance, AnsiString("NO_MAP_PNG"), RT_RCDATA);
  m_pNoTile = new TResourceStream((int)HInstance, AnsiString("NO_TILE_PNG"), RT_RCDATA);
}

__fastcall TOSMMap::~TOSMMap()
{
  delete m_pOSMCache;
  delete m_pTileStream;
  delete m_pJPEG;
  delete m_pPNG;
  delete m_pNoMapTile;
  delete m_pNoTile;
  if(m_pBitmap) delete m_pBitmap;
}

Graphics::TBitmap * __fastcall TOSMMap::GetBitmap()
{
  return m_pBitmap;
}

void __fastcall TOSMMap::Paint(int nWidth, int nHeight)
{
  m_nCurWidth = nWidth; m_nCurHeight = nHeight;
  
  if(m_nZoom < 0) m_nZoom = OSMCalcZoomLevel(m_dMinLat, m_dMinLong, m_dMaxLat, m_dMaxLong,
                                             nWidth, nHeight);

  if(m_dCentreLong == -500.0) m_dCentreLong = (m_dMinLong + m_dMaxLong) / 2.0;
  if(m_dCentreLat == -500.0) m_dCentreLat = (m_dMinLat + m_dMaxLat) / 2.0;
  
  int nCentreX = OSMLong2TileX(m_dCentreLong, m_nZoom), nCentreY = OSMLat2TileY(m_dCentreLat, m_nZoom);

  int nMinX, nMinY, nMaxX, nMaxY;
  CalcTileRange(nCentreX, nCentreY, nMinX, nMinY, nMaxX, nMaxY);

  if(m_pBitmap) delete m_pBitmap;

  m_pBitmap = new Graphics::TBitmap;
  m_pBitmap->Width = (nMaxX - nMinX + 1) * 256;
  m_pBitmap->Height = (nMaxY - nMinY + 1) * 256;

  CalcOffset(nMinX, nMinY, m_nOffsetX, m_nOffsetY);

  #ifdef USE_PROFILER
    DWORD dwProfTickStart = GetTickCount();
    AnsiString asProfStr = AnsiString("DrawTiles call ") +
                           AnsiString((int)m_eMapType);
    pProfiler->Write(asProfStr.c_str());
  #endif

  DrawTiles(nMinX, nMinY, nMaxX, nMaxY, m_nZoom, m_pBitmap->Canvas);

  #ifdef USE_PROFILER
    DWORD dwProfTickCount = GetTickCount() - dwProfTickStart;
    asProfStr = AnsiString("DrawTiles returns: ") +
                AnsiString(dwProfTickCount)       +
                AnsiString(" msecs");
    pProfiler->Write(asProfStr.c_str());
  #endif

  UpdateCanvas();

  GetLatLong(0, m_nCurHeight, m_dMinLat, m_dMinLong);
  GetLatLong(m_nCurWidth, 0, m_dMaxLat, m_dMaxLong);
}

void __fastcall TOSMMap::CalcTileRange(int nCentreX, int nCentreY, int & nMinX, int & nMinY, int & nMaxX, int & nMaxY)
{
  int nTilesX = m_nCurWidth / 256; if(nTilesX * 256 < m_nCurWidth) nTilesX++;
  int nTilesY = m_nCurHeight / 256; if(nTilesY * 256 < m_nCurHeight) nTilesY++;
  int nTilesUp = (nTilesY - 1) / 2 + 1, nTilesLeft = (nTilesX - 1) / 2 + 1;

  nMinX = nCentreX - nTilesLeft; nMaxX = nCentreX + nTilesLeft;
  nMinY = nCentreY - nTilesUp; nMaxY = nCentreY + nTilesUp;
}

void __fastcall TOSMMap::DrawTiles(int nMinX, int nMinY, int nMaxX, int nMaxY, int nZoom, TCanvas * pCanvas)
{
  int i, j, x, y;
  int nTiles = (nMaxY - nMinY + 1) * (nMaxX - nMinX + 1);
  PFileList pHosts;
  int nHosts = GetHosts(m_eMapType, pHosts), nHostNo = 0;
  AnsiString asCacheFileName;

  if(m_bNoMap)
  {
    try
    {
      m_pNoMapTile->Seek(0, soFromBeginning);
      m_pPNG->LoadFromStream(m_pNoMapTile);
    }
    catch(...)
    {
    }
  }

  for(y = nMinY, i = 0; y <= nMaxY; y++, i++)
  {
    int nOffsetY = i * 256;

    for(x = nMinX, j = 0; x <= nMaxX; x++, j++)
    {
      int nOffsetX = j * 256;
      m_pTileStream->Clear();

      if(!m_bNoMap)
      {
        asCacheFileName = m_pOSMCache->GetCacheFileName(m_eMapType, x, y, nZoom);
        bool bCacheGetTile = m_pOSMCache->GetTile(m_eMapType, x, y, nZoom, asCacheFileName, m_pTileStream);

        if(bCacheGetTile)
        {
          try
          {
            m_pTileStream->Seek(0, soFromBeginning);
            m_pPNG->LoadFromStream(m_pTileStream);
            pCanvas->Draw(nOffsetX, nOffsetY, m_pPNG);
            SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILESLEFT, (WPARAM)(nTiles--), 0);
            //SendMessage(Application->MainForm->Handle, OSMMAP_MSG_CANVASUPDATED, 0, 0);
            Application->ProcessMessages();
          }
          catch(...)
          {
          }
        }
        else
        {
          if(x >= 0 && y >= 0 && nZoom >=0 && nZoom <= 20)
          {
            PFileDescr pFDescr = new tagFileDescr;
            pFDescr->dwFileId[0] = y; pFDescr->dwFileId[1] = x;
            pFDescr->asFileName = GetTileURL(m_eMapType, nZoom, x, y);
            pHosts[nHostNo].pFiles->Add(pFDescr);
            nHostNo++; nHostNo %= nHosts;
          }
          else
          {
            try
            {
              m_pNoTile->Seek(0, soFromBeginning);
              m_pPNG->LoadFromStream(m_pNoTile);
              pCanvas->Draw(nOffsetX, nOffsetY, m_pPNG);
              SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILESLEFT, (WPARAM)(nTiles--), 0);
              //SendMessage(Application->MainForm->Handle, OSMMAP_MSG_CANVASUPDATED, 0, 0);
              Application->ProcessMessages();
            }
            catch(...)
            {
            }
          }
        }
      }
      else
      {
        pCanvas->Draw(nOffsetX, nOffsetY, m_pPNG);
        SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILESLEFT, (WPARAM)(nTiles--), 0);
        //SendMessage(Application->MainForm->Handle, OSMMAP_MSG_CANVASUPDATED, 0, 0);
        Application->ProcessMessages();
      }
    }
  }

  THTTPGetFilesThread *pThread = new THTTPGetFilesThread(false);

  i = 0;

  while(i < nHosts)
  {
    if(pHosts[i].pFiles->Count)
    {
      if(pThread->AddTask(&pHosts[i]) != 0xFFFFFFFF)
      {
        while(pHosts[i].pFiles->Count)
        {
          PFileDescr pFDescr = (PFileDescr)pHosts[i].pFiles->Items[0];
          pHosts[i].pFiles->Delete(0);
          delete pFDescr;
        }

        delete pHosts[i].pFiles;
        i++;
      }
    }
    else
    {
      delete pHosts[i].pFiles;
      i++;
    }
  }

  for(;;)
  {
    if(pThread->FileReady)
    {
	  PReadyFile pFile = pThread->ReadyFile;

      if(pFile)
      {
        x = pFile->sFileDescr.dwFileId[1]; y = pFile->sFileDescr.dwFileId[0];
		int nOffsetY = (y - nMinY) * 256, nOffsetX = (x - nMinX) * 256;

		bool bIsPNG = false;
		if(pFile->pData) {
		  pFile->pData->Position = 1;
		  char *pPNGHead = new char[4];
		  pFile->pData->ReadBuffer(pPNGHead, 3);
		  pPNGHead[3] = 0;
		  bIsPNG = (0 == CompareStr(pPNGHead, "PNG"));
		}

		if(bIsPNG)
		{
		  try
		  {
			pFile->pData->Position = 0;
            m_pPNG->LoadFromStream(pFile->pData);
            pCanvas->Draw(nOffsetX, nOffsetY, m_pPNG);

            asCacheFileName = m_pOSMCache->GetCacheFileName(m_eMapType, x, y, nZoom);
            m_pOSMCache->PutTile(m_eMapType, x, y, nZoom, asCacheFileName, pFile->pData);

            AnsiString asTileURL = AnsiString("");

            for(i = 0; i < nHosts; i++)
            {
              if(pHosts[i].dwTaskId == pFile->dwTaskId)
              {
                asTileURL = AnsiString("http://") + pHosts[i].asHost;
                break;
              }
            }

            asTileURL += pFile->sFileDescr.asFileName;
            SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILEURL, 0, (LPARAM)asTileURL.c_str());
            SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILESLEFT, (WPARAM)(nTiles--), 0);
            //SendMessage(Application->MainForm->Handle, OSMMAP_MSG_CANVASUPDATED, 0, 0);
          }
          catch(...)
          {
          }
        }
        else
        {
          try
          {
			m_pNoTile->Position = 0;
            m_pPNG->LoadFromStream(m_pNoTile);
            pCanvas->Draw(nOffsetX, nOffsetY, m_pPNG);
            SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILESLEFT, (WPARAM)(nTiles--), 0);
            //SendMessage(Application->MainForm->Handle, OSMMAP_MSG_CANVASUPDATED, 0, 0);
          }
          catch(...)
          {
          }
        }
      }
    }
    else
    {
      if(pThread->Complete) break;
      else Sleep(50);
    }

    Application->ProcessMessages();
  }

  pThread->Terminate();
  pThread->WaitFor();
  delete pThread;

  SendMessage(Application->MainForm->Handle, OSMMAP_MSG_TILESLEFT, 0, 0);
}

void __fastcall TOSMMap::CalcOffset(int nMinX, int nMinY, int & nOffsetX, int & nOffsetY)
{
  double dTopMercY = Lat2MercatorY(OSMTileY2Lat(nMinY, m_nZoom));
  double dLeftMercX = Lon2MercatorX(OSMTileX2Long(nMinX, m_nZoom));
  double dCenterMercX = Lon2MercatorX(m_dCentreLong);
  double dCenterMercY = Lat2MercatorY(m_dCentreLat);
  double dRes = OSMResolution(m_nZoom);

  int nCenterXPix = (int)((dCenterMercX - dLeftMercX) / dRes);
  int nCenterYPix = (int)((dTopMercY - dCenterMercY) / dRes);

  nOffsetX = m_nCurWidth / 2 - nCenterXPix;
  nOffsetY = m_nCurHeight / 2 - nCenterYPix;
}

int __fastcall TOSMMap::GetOffsetX()
{
  return m_nOffsetX;
}

int __fastcall TOSMMap::GetOffsetY()
{
  return m_nOffsetY;
}

void __fastcall TOSMMap::ZoomIn()
{
  if(m_nZoom < nMaxZoom[(int)m_eMapType])
  {
    m_nZoom++;
    Refresh();
  }
}

void __fastcall TOSMMap::ZoomOut()
{
  if(m_nZoom > 0)
  {
    m_nZoom--;
    Refresh();
  }
}

void __fastcall TOSMMap::DragStart(int X, int Y)
{
  m_nLastX = X; m_nLastY = Y;
}

void __fastcall TOSMMap::Drag(int X, int Y)
{
  CalcDrag(X, Y);
  
  if(m_pBitmap)
  {
    if(m_nOffsetX > 0                               ||
      (m_nCurWidth - m_nOffsetX) > m_pBitmap->Width ||
       m_nOffsetY > 0                               ||
      (m_nCurHeight - m_nOffsetY) > m_pBitmap->Height)
    {
      TBrush *pBrush = new TBrush;
      pBrush->Color = clBtnFace;
      m_pCanvas->Brush = pBrush;

      TPen *pPen = new TPen;
      pPen->Style = psSolid; pPen->Width = 1;
      pPen->Color = clBtnFace;
      m_pCanvas->Pen = pPen;

      m_pCanvas->Rectangle(0, 0, m_nCurWidth, m_nCurHeight);

      delete pPen;
      delete pBrush;
    }

    if(m_bDraw) m_pCanvas->Draw(m_nOffsetX, m_nOffsetY, m_pBitmap);

    GetLatLong(0, m_nCurHeight, m_dMinLat, m_dMinLong);
    GetLatLong(m_nCurWidth, 0, m_dMaxLat, m_dMaxLong);
  }
}

void __fastcall TOSMMap::DragEnd(int X, int Y)
{
  CalcDrag(X, Y);
  Refresh();
}

void __fastcall TOSMMap::CalcDrag(int X, int Y)
{
  int nDX = (X - m_nLastX), nDY = (Y - m_nLastY);
  double dRes = OSMResolution(m_nZoom);
  double dMercDX = (double)nDX * dRes, dMercDY = (double)nDY * dRes;
  double dCenterX = Lon2MercatorX(m_dCentreLong), dCenterY = Lat2MercatorY(m_dCentreLat);

  dCenterX -= dMercDX; dCenterY += dMercDY;
  m_dCentreLong = MercatorX2Lon(dCenterX); m_dCentreLat = MercatorY2Lat(dCenterY);

  m_nOffsetX += nDX; m_nOffsetY += nDY;
  m_nLastX = X; m_nLastY = Y;
}

void __fastcall TOSMMap::Reset(double dMaxLat, double dMaxLong, double dMinLat, double dMinLong)
{
  m_dMaxLat = dMaxLat;
  m_dMinLat = dMinLat;
  m_dMinLong = dMinLong;
  m_dMaxLong = dMaxLong;
  m_nZoom = -1;
  m_dCentreLong = -500.0;
  m_dCentreLat = -500.0;
  m_eMapType = mpLandscape;

  if(m_pBitmap)
  {
    delete m_pBitmap;
    m_pBitmap = NULL;
  }
}

void __fastcall TOSMMap::SetCanvas(TCanvas * value)
{
  if(m_pCanvas != value) {
    m_pCanvas = value;
  }
}

TCanvas * __fastcall TOSMMap::GetCanvas()
{
  return m_pCanvas;
}

void __fastcall TOSMMap::SetCentreLat(double value)
{
  if(m_dCentreLat != value) {
    m_dCentreLat = value;
  }
}

double __fastcall TOSMMap::GetCentreLat()
{
  return m_dCentreLat;
}

void __fastcall TOSMMap::SetCentreLong(double value)
{
  if(m_dCentreLong != value) {
    m_dCentreLong = value;
  }
}

double __fastcall TOSMMap::GetCentreLong()
{
  return m_dCentreLong;
}

void __fastcall TOSMMap::SetZoom(int value)
{
  if(m_nZoom != value && value >= 0 && value <= nMaxZoom[(int)m_eMapType]) {
    m_nZoom = value;
  }
}

int __fastcall TOSMMap::GetZoom()
{
  return m_nZoom;
}

void __fastcall TOSMMap::SetMapType(eMapType value)
{
  if(m_eMapType != value) {
    m_eMapType = value;
  }
}

eMapType __fastcall TOSMMap::GetMapType()
{
  return m_eMapType;
}

AnsiString __fastcall TOSMMap::GetMapTypeName()
{
  return AnsiString(pMapNames[(int)m_eMapType]);
}

TStringList * __fastcall TOSMMap::GetMapTypeNamesList()
{
  int nTypes = sizeof(pMapNames) / sizeof(char*);
  TStringList *pList = new TStringList;

  for(int i = 0; i < nTypes; i++)
    pList->Add(AnsiString(pMapNames[i]));

  return pList;
}

/*
 * "http://[abc].tile.thunderforest.com/landscape/" + zoom + "/" + coord.x + "/" + coord.y + ".png"
 * "http://[abc].tile.thunderforest.com/cycle/" + zoom + "/" + coord.x + "/" + coord.y + ".png"
 * "http://tiles-base.openstreetbrowser.org/tiles/basemap_base/" + zoom + "/" + coord.x + "/" + coord.y + ".png"
 * "http://[abc].tile.openstreetmap.org/" + zoom + "/" + coord.x + "/" + coord.y + ".png"
 * "http://[abc].tile.thunderforest.com/outdoors/" + zoom + "/" + coord.x + "/" + coord.y + ".png"
 * "http://[abc].tile.opentopomap.org/" + zoom + "/" + coord.x + "/" + coord.y + ".png"

 * "http://maps2.atlogis.com/cgi-bin/tilecache-2.11/tilecache.py/1.0.0/topomapper_gmerc/" + zoom + "/" + coord.x + "/" + coord.y
 * "http://maps.marshruty.ru/ml.ashx?al=1&i=1&x=" + coord.x + '&y=' + coord.y + '&z=' + zoom
 
 * "http://tile[1234].maps.2gis.ru/tiles?v=1112&x=" + coord.x + "&y=" + coord.y + "&z=" + zoom
 * ?apikey=e617f1fb0f934dfbba0848128c094e5e
 */

 AnsiString __fastcall TOSMMap::GetTileURL(eMapType eType, int nZoom, int x, int y)
{
  AnsiString asTileURL;
  
  switch(eType)
  {
    case mpLandscape:
    {
      asTileURL = AnsiString("/landscape/");
      break;
    }

    case mpCycle:
    {
      asTileURL = AnsiString("/cycle/");
      break;
    }

    case mpStreetMap:
    {
      asTileURL = AnsiString("/");
      break;
    }

    case mpOutdoors:
    {
      asTileURL = AnsiString("/outdoors/");
      break;
    }

    case mpTopo:
    {
      asTileURL = AnsiString("/");
      break;
    }
  }

  asTileURL += (AnsiString(nZoom) + AnsiString("/") +
                AnsiString(x)     + AnsiString("/") +
                AnsiString(y)     + AnsiString(".png"));

  //if(eType == mpLandscape || eType == mpCycle || eType == mpOutdoors)
  //  asTileURL += AnsiString("?apikey=e617f1fb0f934dfbba0848128c094e5e");

  return asTileURL;
}

int __fastcall TOSMMap::GetHosts(eMapType eType, PFileList &pHosts)
{
  int nHosts = 0;
  static char *TileLett[] = {"a", "b", "c"};

  switch(eType)
  {
    case mpLandscape:
    case mpCycle:
    case mpOutdoors:
    {
      nHosts = 3;
      pHosts = new tagFileList[nHosts];

      for(int i = 0; i < nHosts; i++)
      {
        pHosts[i].asHost = AnsiString(TileLett[i]) + AnsiString(".tile.thunderforest.com");
        pHosts[i].pFiles = new TList;
      }

      break;
    }

    case mpStreetMap:
    {
      nHosts = 3;
      pHosts = new tagFileList[nHosts];

      for(int i = 0; i < nHosts; i++)
      {
        pHosts[i].asHost = AnsiString(TileLett[i]) + AnsiString(".tile.openstreetmap.org");
        pHosts[i].pFiles = new TList;
      }

      break;
    }

    case mpTopo:
    {
      nHosts = 3;
      pHosts = new tagFileList[nHosts];

      for(int i = 0; i < nHosts; i++)
      {
        pHosts[i].asHost = AnsiString(TileLett[i]) + AnsiString(".tile.opentopomap.org");
        pHosts[i].pFiles = new TList;
      }

      break;
    }
  }

  return nHosts;
}

void __fastcall TOSMMap::SetCenter(double dCenterLat, double dCenterLong)
{
  m_dCentreLat = dCenterLat; m_dCentreLong = dCenterLong;
  Refresh();
}

void __fastcall TOSMMap::Refresh()
{
  Paint(m_nCurWidth, m_nCurHeight);
}

void __fastcall TOSMMap::GetLatLong(int nX, int nY, double & dLat, double & dLong)
{
  double dRes = OSMResolution(m_nZoom);
  double dX = Lon2MercatorX(m_dCentreLong) + (double)(nX - m_nCurWidth / 2) * dRes;
  double dY = Lat2MercatorY(m_dCentreLat) - (double)(nY - m_nCurHeight / 2) * dRes;

  dLat = MercatorY2Lat(dY); dLong = MercatorX2Lon(dX);
}

double __fastcall TOSMMap::GetMinLat()
{
  return m_dMinLat;
}

double __fastcall TOSMMap::GetMinLong()
{
  return m_dMinLong;
}

double __fastcall TOSMMap::GetMaxLat()
{
  return m_dMaxLat;
}

double __fastcall TOSMMap::GetMaxLong()
{
  return m_dMaxLong;
}

void __fastcall TOSMMap::GetPixels(double dLat, double dLong, int & nX, int & nY)
{
  double dRes = OSMResolution(m_nZoom);
  int ndX = (int)((Lon2MercatorX(m_dCentreLong) - Lon2MercatorX(dLong)) / dRes);
  int ndY = (int)((Lat2MercatorY(m_dCentreLat) - Lat2MercatorY(dLat)) / dRes);

  nX = m_nCurWidth / 2 - ndX;
  nY = m_nCurHeight / 2 + ndY;
}

void __fastcall TOSMMap::InitCache()
{
  m_pOSMCache->Init();
}

void __fastcall TOSMMap::Idle()
{
  m_pOSMCache->Idle();
}

void __fastcall TOSMMap::SetNoMap(bool value)
{
  if(m_bNoMap != value) {
    m_bNoMap = value;
  }
}

void __fastcall TOSMMap::UpdateCanvas()
{
  if(m_bDraw) m_pCanvas->Draw(m_nOffsetX, m_nOffsetY, m_pBitmap);
}
