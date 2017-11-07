//---------------------------------------------------------------------------

#ifndef OSMTrackViewH
#define OSMTrackViewH

#include "GPXData.h"
#include "OSMMap.h"
#include "Cells.h"
//---------------------------------------------------------------------------

struct TrackViewItem {
  PGPXTrack pTrack;
  Graphics::TBitmap *pBitmap;
  TColor Color;
  TCells *pCells;
};

typedef TrackViewItem* PTrackViewItem;

class TOSMTrackView : public TOSMMap
{

public:
  __fastcall TOSMTrackView(double dMaxLat, double dMaxLong, double dMinLat, double dMinLong, TCanvas * pCanvas, eMapType eType, int nZoom, bool bNoMap);
  __fastcall ~TOSMTrackView();
  int __fastcall Add(PGPXTrack pTrack, TColor Color);
  void __fastcall Delete(int i);
  void __fastcall ShowTracks();
  void __fastcall Paint(int nWidth, int nHeight);
  void __fastcall Drag(int X, int Y);
  void __fastcall DragEnd(int X, int Y);
  void __fastcall Refresh();
  void __fastcall SetCenter(double dCenterLat, double dCenterLong);
  void __fastcall Reset(double dMaxLat, double dMaxLong, double dMinLat, double dMinLong);
  void __fastcall ZoomIn();
  void __fastcall ZoomOut();
  void __fastcall Delete(PGPXTrack pTrack);
  void __fastcall Redraw();
  void __fastcall ClearTracks();
  void __fastcall ZoomToFit();
  void __fastcall DrawPoint(int i);
  void __fastcall ClearPoint();
  void __fastcall UpdateCanvas();
  __property int Count  = { read=GetCount };
  __property int Topmost  = { read=GetTopmost, write=SetTopmost };
  __property PGPXTrack Tracks[int i] = { read=GetTrack };
  __property Graphics::TBitmap * Bitmaps[int i] = { read=GetBitmaps };
  __property TColor Color[int i] = { read=GetColor };
  __property int PenWidth  = { read=GetPenWidth, write=SetPenWidth };
  __property PGPXTrack TopmostTrack  = { read=GetTopmostTrack, write=SetTopmostTrack };
  __property int CurrentPoint  = { read=GetCurrentPoint, write=SetCurrentPoint };
  __property int NearestPoint[int x][int y] = { read=GetNearestPoint };
protected:
  TList * m_pTracks;
  int m_nTopmost;
  int m_nPenWidth;
  int m_nCurrentPoint;
  Graphics::TBitmap * m_pTrackPoint;
  Graphics::TBitmap * m_pBkBitmap;
  Graphics::TBitmap * __fastcall CreateBitmap(int nWidth, int nHeigth);
  void __fastcall DrawTrack(PTrackViewItem pItem);
  void __fastcall PaintTracks();
  void __fastcall DrawBackground();
private:
  int __fastcall GetCount();
  void __fastcall SetTopmost(int value);
  int __fastcall GetTopmost();
  PGPXTrack __fastcall GetTrack(int i);
  Graphics::TBitmap * __fastcall GetBitmaps(int i);
  TColor __fastcall GetColor(int i);
  void __fastcall SetPenWidth(int value);
  int __fastcall GetPenWidth();
  void __fastcall SetTopmostTrack(PGPXTrack value);
  PGPXTrack __fastcall GetTopmostTrack();
  void __fastcall SetCurrentPoint(int value);
  int __fastcall GetCurrentPoint();
  int __fastcall GetNearestPoint(int x, int y);
};


#endif
