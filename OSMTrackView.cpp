//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "OSMTrackView.h"

//---------------------------------------------------------------------------

#pragma resource "TrackPoint.res"
#pragma package(smart_init)

__fastcall TOSMTrackView::TOSMTrackView(double dMaxLat, double dMaxLong,
                                        double dMinLat, double dMinLong,
                                        TCanvas * pCanvas, eMapType eType, int nZoom, bool bNoMap) :
                          TOSMMap(dMaxLat, dMaxLong, dMinLat, dMinLong, pCanvas, eType, nZoom, bNoMap)
{
  m_pTracks = new TList;
  m_pTrackPoint = new Graphics::TBitmap;
  m_pTrackPoint->LoadFromResourceName((int)HInstance, "TRACKPOINT");
  m_pTrackPoint->Transparent = true;
  m_pTrackPoint->TransparentMode = tmAuto;
  m_nPenWidth = 3;
  m_nTopmost = -1;
  m_nCurrentPoint = -1;
  m_pBkBitmap = NULL;
}

__fastcall TOSMTrackView::~TOSMTrackView()
{
  ClearTracks();
  delete m_pTracks;
  delete m_pTrackPoint;
  delete m_pBkBitmap;
}

int __fastcall TOSMTrackView::GetCount()
{
  return m_pTracks->Count;
}

void __fastcall TOSMTrackView::SetTopmost(int value)
{
  if(value > -1 && value < Count) {
    m_nTopmost = value;
    Redraw();
  }
}

int __fastcall TOSMTrackView::GetTopmost()
{
  return m_nTopmost;
}

PGPXTrack __fastcall TOSMTrackView::GetTrack(int i)
{
  PGPXTrack pTrack = NULL;

  if(i > -1 && i < Count)
  {
    PTrackViewItem pTrackItem = (PTrackViewItem)m_pTracks->Items[i];
    pTrack = pTrackItem->pTrack;
  }

  return pTrack;
}

Graphics::TBitmap * __fastcall TOSMTrackView::GetBitmaps(int i)
{
  Graphics::TBitmap *pBitmap = NULL;

  if(i > -1 && i < Count)
  {
    PTrackViewItem pTrackItem = (PTrackViewItem)m_pTracks->Items[i];
    pBitmap = pTrackItem->pBitmap;
  }

  return pBitmap;
}

TColor __fastcall TOSMTrackView::GetColor(int i)
{
  TColor Color = Vcl::Graphics::clNone;

  if(i > -1 && i < Count)
  {
    PTrackViewItem pTrackItem = (PTrackViewItem)m_pTracks->Items[i];
    Color = pTrackItem->Color;
  }

  return Color;
}

void __fastcall TOSMTrackView::SetPenWidth(int value)
{
  if(m_nPenWidth != value && value > 0) {
    m_nPenWidth = value;
    DrawBackground();
    PaintTracks();
  }
}

int __fastcall TOSMTrackView::GetPenWidth()
{
  return m_nPenWidth;
}

int __fastcall TOSMTrackView::Add(PGPXTrack pTrack, TColor Color)
{
  bool bFound = false;

  for(int i = 0; i < Count; i++)
  {
    PTrackViewItem pTrackItem = (PTrackViewItem)m_pTracks->Items[i];

    if(pTrackItem->pTrack == pTrack)
    {
      bFound = true;
      m_nTopmost = i;
      break;
    }
  }

  if(!bFound)
  {
    PTrackViewItem pNewItem = new TrackViewItem;

    pNewItem->pTrack = pTrack;
    pNewItem->pBitmap = NULL;
    pNewItem->pCells = NULL;
    pNewItem->Color = Color;

    m_pTracks->Add(pNewItem);

    DrawTrack(pNewItem);
    m_nTopmost = Count - 1;
  }

  Redraw();

  return m_nTopmost;
}

void __fastcall TOSMTrackView::Delete(int i)
{
  PTrackViewItem pTopmostItem = (PTrackViewItem)m_pTracks->Items[m_nTopmost];
  PTrackViewItem pTrackItem = (PTrackViewItem)m_pTracks->Items[i];
  bool bDelTopmost = false;

  if(pTopmostItem == pTrackItem) bDelTopmost = true;

  delete pTrackItem->pBitmap;
  delete pTrackItem->pCells;
  delete pTrackItem;
  m_pTracks->Delete(i);

  if(Count)
  {
    int n = Topmost;
    n %= Count;

    if(!bDelTopmost)
    {
      for(int i = 0; i < Count; i++)
      {
        PTrackViewItem pItem = (PTrackViewItem)m_pTracks->Items[i];

        if(pItem->pTrack == pTopmostItem->pTrack)
        {
          n = i;
          break;
        }
      }
    }

    Topmost = n;
  }
  else
  {
    m_nTopmost = -1;
    Redraw();
  }
}

Graphics::TBitmap * __fastcall TOSMTrackView::CreateBitmap(int nWidth, int nHeigth)
{
  Graphics::TBitmap *pBitmap = new Graphics::TBitmap;

  pBitmap->Width = nWidth;
  pBitmap->Height = nHeigth;

  TBrush *pBrush = new TBrush;
  pBrush->Color = clWhite;
  pBitmap->Canvas->Brush = pBrush;

  TPen *pPen = new TPen;
  pPen->Style = psSolid; pPen->Width = 1;
  pPen->Color = clWhite;
  pBitmap->Canvas->Pen = pPen;

  pBitmap->Canvas->Rectangle(0, 0, nWidth, nHeigth);
  pBitmap->Transparent = true;
  pBitmap->TransparentColor = pBitmap->Canvas->Pixels[0][0];

  delete pPen;
  delete pBrush;

  return pBitmap;
}

void __fastcall TOSMTrackView::DrawTrack(PTrackViewItem pItem)
{
  double dMinLat, dMinLon, dMaxLat, dMaxLon;
  int nXMin = OffsetX;
  int nYMin = OffsetY;
  int nXMax = nXMin + Bitmap->Width;
  int nYMax = nYMin + Bitmap->Height;

  TOSMMap::GetLatLong(nXMin, nYMax, dMinLat, dMinLon);
  TOSMMap::GetLatLong(nXMax, nYMin, dMaxLat, dMaxLon);

  if(pItem->pBitmap) delete pItem->pBitmap;
  pItem->pBitmap = CreateBitmap(Bitmap->Width, Bitmap->Height);

  TCanvas *pCanvas = pItem->pBitmap->Canvas;

  if(pItem->pCells) delete pItem->pCells;
  pItem->pCells = new TCells(Bitmap->Width, Bitmap->Height);

  PGPXTrack pTrack = pItem->pTrack;
  bool bStartSeg = true;

  for(int i = 0; i < pTrack->PointCount; i++)
  {
    PGPXTrackPoint pPoint = pTrack->Points[i];

    if(pPoint->m_dLat >= dMinLat &&
       pPoint->m_dLat <= dMaxLat &&
       pPoint->m_dLon >= dMinLon &&
       pPoint->m_dLon <= dMaxLon)
    {
      int nX, nY;

      TOSMMap::GetPixels(pPoint->m_dLat, pPoint->m_dLon, nX, nY);

      nX -= OffsetX; nY -= OffsetY;
      
      TPen *pPen = new TPen;
      pPen->Style = psSolid; pPen->Width = m_nPenWidth;
      pPen->Color = pItem->Color;
      pCanvas->Pen = pPen;

      if(bStartSeg)
      {
        pCanvas->MoveTo(nX, nY);
        bStartSeg = false;
      }
      else pCanvas->LineTo(nX, nY);

      pItem->pCells->Point[nX][nY] = i;
    }
    else bStartSeg = true;
  }
}

void __fastcall TOSMTrackView::ShowTracks()
{
  int n = m_nTopmost + 1;

  for(int i = 0; i < Count; i++)
  {
    n %= Count;

    PTrackViewItem pItem = (PTrackViewItem)m_pTracks->Items[n];
    m_pBkBitmap->Canvas->Draw(0, 0, pItem->pBitmap);

    n++;
  }

  Canvas->Draw(OffsetX, OffsetY, m_pBkBitmap);

  if(m_nCurrentPoint > -1) DrawPoint(m_nCurrentPoint);
}

void __fastcall TOSMTrackView::Paint(int nWidth, int nHeight)
{
  TOSMMap::Paint(nWidth, nHeight);
  DrawBackground();
  PaintTracks();
}

void __fastcall TOSMTrackView::Drag(int X, int Y)
{
  TOSMMap::Drag(X, Y);
  Canvas->Draw(OffsetX, OffsetY, m_pBkBitmap);
}

void __fastcall TOSMTrackView::DragEnd(int X, int Y)
{
  TOSMMap::DragEnd(X, Y);
  DrawBackground();
  PaintTracks();
}

void __fastcall TOSMTrackView::PaintTracks()
{
  for(int i = 0; i < Count; i++)
  {
    PTrackViewItem pItem = (PTrackViewItem)m_pTracks->Items[i];
    DrawTrack(pItem);
  }

  ShowTracks();
}

void __fastcall TOSMTrackView::Refresh()
{
  TOSMMap::Refresh();
  DrawBackground();
  PaintTracks();
}

void __fastcall TOSMTrackView::SetCenter(double dCenterLat, double dCenterLong)
{
  TOSMMap::SetCenter(dCenterLat, dCenterLong);
  DrawBackground();
  PaintTracks();
}

void __fastcall TOSMTrackView::Reset(double dMaxLat, double dMaxLong, double dMinLat, double dMinLong)
{
  TOSMMap::Reset(dMaxLat, dMaxLong, dMinLat, dMinLong);
  DrawBackground();
  ClearTracks();
}

void __fastcall TOSMTrackView::ClearTracks()
{
  while(Count)
  {
    PTrackViewItem pTrackItem = (PTrackViewItem)m_pTracks->Items[0];

    delete pTrackItem->pBitmap;
    delete pTrackItem->pCells;
    delete pTrackItem;
    m_pTracks->Delete(0);
  }

  m_nTopmost = -1;
}

void __fastcall TOSMTrackView::ZoomIn()
{
  TOSMMap::ZoomIn();
  DrawBackground();
  PaintTracks();
}

void __fastcall TOSMTrackView::ZoomOut()
{
  TOSMMap::ZoomOut();
  DrawBackground();
  PaintTracks();
}

void __fastcall TOSMTrackView::SetTopmostTrack(PGPXTrack value)
{
  int n = -1;

  for(int i = 0; i < Count; i++)
  {
    PTrackViewItem pItem = (PTrackViewItem)m_pTracks->Items[i];

    if(pItem->pTrack == value)
    {
      n = i;
      break;
    }
  }

  if(n > -1) Topmost = n;
}

PGPXTrack __fastcall TOSMTrackView::GetTopmostTrack()
{
  PTrackViewItem pItem = (PTrackViewItem)m_pTracks->Items[m_nTopmost];
  return pItem->pTrack;
}

void __fastcall TOSMTrackView::Delete(PGPXTrack pTrack)
{
  int n = -1;

  for(int i = 0; i < Count; i++)
  {
    PTrackViewItem pItem = (PTrackViewItem)m_pTracks->Items[i];

    if(pItem->pTrack == pTrack)
    {
      n = i;
      break;
    }
  }

  if(n > -1) Delete(n);
}

void __fastcall TOSMTrackView::Redraw()
{
  Canvas->Draw(OffsetX, OffsetY, Bitmap);
  DrawBackground();
  ShowTracks();
}

void __fastcall TOSMTrackView::ZoomToFit()
{
  double dMaxLat = -999.0, dMaxLon = -999.0, dMinLat = 999.0, dMinLon = 999.0;

  for(int i = 0; i < m_pTracks->Count; i++)
  {
    PTrackViewItem pTrackItem = (PTrackViewItem)m_pTracks->Items[i];

    if(pTrackItem->pTrack->m_dMaxLat > dMaxLat) dMaxLat = pTrackItem->pTrack->m_dMaxLat;
    if(pTrackItem->pTrack->m_dMaxLon > dMaxLon) dMaxLon = pTrackItem->pTrack->m_dMaxLon;
    if(pTrackItem->pTrack->m_dMinLat < dMinLat) dMinLat = pTrackItem->pTrack->m_dMinLat;
    if(pTrackItem->pTrack->m_dMinLon < dMinLon) dMinLon = pTrackItem->pTrack->m_dMinLon;
  }

  eMapType eType = MapType;
  TOSMMap::Reset(dMaxLat, dMaxLon, dMinLat, dMinLon);
  MapType = eType;

  TOSMMap::Refresh();
  DrawBackground();
  PaintTracks();
}

void __fastcall TOSMTrackView::DrawPoint(int n)
{
  if(m_nTopmost > -1)
  {
    PTrackViewItem pTopmostItem = (PTrackViewItem)m_pTracks->Items[m_nTopmost];

    double dMinLat, dMinLon, dMaxLat, dMaxLon;
    int nXMin = OffsetX;
    int nYMin = OffsetY;
    int nXMax = nXMin + Bitmap->Width;
    int nYMax = nYMin + Bitmap->Height;

    TOSMMap::GetLatLong(nXMin, nYMax, dMinLat, dMinLon);
    TOSMMap::GetLatLong(nXMax, nYMin, dMaxLat, dMaxLon);

    if(n > -1)
    {
      PGPXTrackPoint pPoint = pTopmostItem->pTrack->Points[n];

      if(pPoint->m_dLat >= dMinLat &&
         pPoint->m_dLat <= dMaxLat &&
         pPoint->m_dLon >= dMinLon &&
         pPoint->m_dLon <= dMaxLon)
      {
        int nX, nY;

        TOSMMap::GetPixels(pPoint->m_dLat, pPoint->m_dLon, nX, nY);

        nX -= (m_pTrackPoint->Width / 2); nY -= (m_pTrackPoint->Height / 2);
        Canvas->Draw(nX, nY, m_pTrackPoint);
      }
    }
  }
}

void __fastcall TOSMTrackView::ClearPoint()
{
  if(m_nTopmost > -1)
  {
    PTrackViewItem pTopmostItem = (PTrackViewItem)m_pTracks->Items[m_nTopmost];

    double dMinLat, dMinLon, dMaxLat, dMaxLon;
    int nXMin = OffsetX;
    int nYMin = OffsetY;
    int nXMax = nXMin + Bitmap->Width;
    int nYMax = nYMin + Bitmap->Height;

    TOSMMap::GetLatLong(nXMin, nYMax, dMinLat, dMinLon);
    TOSMMap::GetLatLong(nXMax, nYMin, dMaxLat, dMaxLon);

    if(m_nCurrentPoint > -1)
    {
      PGPXTrackPoint pPoint = pTopmostItem->pTrack->Points[m_nCurrentPoint];

      if(pPoint->m_dLat >= dMinLat &&
         pPoint->m_dLat <= dMaxLat &&
         pPoint->m_dLon >= dMinLon &&
         pPoint->m_dLon <= dMaxLon)
      {
        int nX, nY;

        TOSMMap::GetPixels(pPoint->m_dLat, pPoint->m_dLon, nX, nY);
        nX -= (m_pTrackPoint->Width / 2); nY -= (m_pTrackPoint->Height / 2);

        TRect rcFrom, rcTo;

        rcFrom.left = nX - m_nOffsetX;
        rcFrom.top = nY - m_nOffsetY;
        rcFrom.right = rcFrom.left + m_pTrackPoint->Width;
        rcFrom.bottom = rcFrom.top + m_pTrackPoint->Height;
        rcTo.left = nX;
        rcTo.top = nY;
        rcTo.right = rcTo.left + m_pTrackPoint->Width;
        rcTo.bottom = rcTo.top + m_pTrackPoint->Height;

        Canvas->CopyRect(rcTo, m_pBkBitmap->Canvas, rcFrom);
      }
    }
  }
}

void __fastcall TOSMTrackView::SetCurrentPoint(int value)
{
  if(m_nCurrentPoint != value)
  {
    if(m_nCurrentPoint > -1) ClearPoint();
    if(value > -1) DrawPoint(value);
    m_nCurrentPoint = value;
  }
}

int __fastcall TOSMTrackView::GetCurrentPoint()
{
  return m_nCurrentPoint;
}

int __fastcall TOSMTrackView::GetNearestPoint(int x, int y)
{
  int np = -1;

  if(m_nTopmost > -1)
  {
    PTrackViewItem pTopmostItem = (PTrackViewItem)m_pTracks->Items[m_nTopmost];
    np = pTopmostItem->pCells->NearestPoint[x - OffsetX][y - OffsetY];
  }

  return np;
}

void __fastcall TOSMTrackView::DrawBackground()
{
  if(m_pBkBitmap) delete m_pBkBitmap;

  m_pBkBitmap = new Graphics::TBitmap;
  m_pBkBitmap->Width = Bitmap->Width;
  m_pBkBitmap->Height = Bitmap->Height;
  m_pBkBitmap->Canvas->Draw(0, 0, Bitmap);
}

void __fastcall TOSMTrackView::UpdateCanvas()
{
  TOSMMap::UpdateCanvas();
  DrawBackground();
  PaintTracks();
}
