//---------------------------------------------------------------------------

#ifndef OSMMapH
#define OSMMapH
//---------------------------------------------------------------------------
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Imaging.jpeg.hpp>

#include "HTTPGetFilesThread.h"
#include "OSMCache.h"

#define OSMMAP_MSG_TILESLEFT      (WM_APP + 950)
#define OSMMAP_MSG_TILEURL        (WM_APP + 951)
#define OSMMAP_MSG_CANVASUPDATED  (WM_APP + 952)

class TOSMMap {
public:
  __fastcall TOSMMap(double dMaxLat, double dMaxLong, double dMinLat, double dMinLong, TCanvas * pCanvas, eMapType eType, int nZoom, bool bNoMap, bool bDraw = false);
  __fastcall ~TOSMMap();
  virtual void __fastcall Paint(int nWidth, int nHeight);
  virtual void __fastcall ZoomIn();
  virtual void __fastcall ZoomOut();
  virtual void __fastcall DragStart(int X, int Y);
  virtual void __fastcall Drag(int X, int Y);
  virtual void __fastcall DragEnd(int X, int Y);
  virtual void __fastcall Reset(double dMaxLat, double dMaxLong, double dMinLat, double dMinLong);
  virtual void __fastcall SetCenter(double dCenterLat, double dCenterLong);
  virtual void __fastcall Refresh();
  virtual void __fastcall GetLatLong(int nX, int nY, double & dLat, double & dLong);
  virtual void __fastcall GetPixels(double dLat, double dLong, int & nX, int & nY);
  virtual void __fastcall InitCache();
  virtual void __fastcall Idle();
  void __fastcall UpdateCanvas();
  __property Graphics::TBitmap * Bitmap  = { read=GetBitmap };
  __property int OffsetX  = { read=GetOffsetX };
  __property int OffsetY  = { read=GetOffsetY };
  __property TCanvas * Canvas  = { read=GetCanvas, write=SetCanvas };
  __property double CentreLat  = { read=GetCentreLat, write=SetCentreLat };
  __property double CentreLong  = { read=GetCentreLong, write=SetCentreLong };
  __property int Zoom  = { read=GetZoom, write=SetZoom };
  __property eMapType MapType  = { read=GetMapType, write=SetMapType };
  __property AnsiString MapTypeName  = { read=GetMapTypeName };
  __property TStringList * MapTypeNamesList  = { read=GetMapTypeNamesList };
  __property double MinLat  = { read=GetMinLat };
  __property double MinLong  = { read=GetMinLong };
  __property double MaxLat  = { read=GetMaxLat };
  __property double MaxLong  = { read=GetMaxLong };
  __property bool NoMap  = { write=SetNoMap };
protected:
  TCanvas *m_pCanvas;
  TOSMCache *m_pOSMCache;
  TJPEGImage *m_pJPEG;
  TPngImage *m_pPNG;
  double m_dMinLat;
  double m_dMaxLat;
  double m_dMinLong;
  double m_dMaxLong;
  int m_nZoom;
  int m_nOffsetX;
  int m_nOffsetY;
  int m_nLastX;
  int m_nLastY;
  int m_nCurWidth;
  int m_nCurHeight;
  bool m_bDraw;
  TCHAR m_szTempPathBuffer[MAX_PATH];
  TCHAR m_szTempFileName[MAX_PATH];
  TMemoryStream *m_pTileStream;
  double m_dCentreLong;
  double m_dCentreLat;
  eMapType m_eMapType;
  TResourceStream *m_pNoMapTile;
  TResourceStream *m_pNoTile;
  virtual void __fastcall CalcTileRange(int nCentreX, int nCentreY, int & nMinX, int & nMinY, int & nMaxX, int & nMaxY);
  virtual void __fastcall DrawTiles(int nMinX, int nMinY, int nMaxX, int nMaxY, int nZoom, TCanvas * pCanvas);
  virtual void __fastcall CalcOffset(int nMinX, int nMinY, int & nOffsetX, int & nOffsetY);
  virtual void __fastcall CalcDrag(int X, int Y);
  virtual AnsiString __fastcall GetTileURL(eMapType eType, int nZoom, int x, int y);
  virtual int __fastcall GetHosts(eMapType eType, PFileList &pHosts);
private:
  Graphics::TBitmap * m_pBitmap;
  bool m_bNoMap;
  Graphics::TBitmap * __fastcall GetBitmap();
  int __fastcall GetOffsetX();
  int __fastcall GetOffsetY();
  void __fastcall SetCanvas(TCanvas * value);
  TCanvas * __fastcall GetCanvas();
  void __fastcall SetCentreLat(double value);
  double __fastcall GetCentreLat();
  void __fastcall SetCentreLong(double value);
  double __fastcall GetCentreLong();
  void __fastcall SetZoom(int value);
  int __fastcall GetZoom();
  void __fastcall SetMapType(eMapType value);
  eMapType __fastcall GetMapType();
  AnsiString __fastcall GetMapTypeName();
  TStringList * __fastcall GetMapTypeNamesList();
  double __fastcall GetMinLat();
  double __fastcall GetMinLong();
  double __fastcall GetMaxLat();
  double __fastcall GetMaxLong();
  void __fastcall SetNoMap(bool value);
};

#endif
