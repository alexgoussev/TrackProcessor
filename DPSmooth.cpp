//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "DPSmooth.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall DP_HDOP_Smooth::DP_HDOP_Smooth(HWND hParentWnd)
{
  m_hParentWnd = hParentWnd;
}

__fastcall DP_HDOP_Smooth::~DP_HDOP_Smooth()
{
  
}

void __fastcall DP_HDOP_Smooth::Process()
{
  m_pOriginalTrack = new GPXTrack(m_pCurrentTrack);
  m_bNeedReduce = false;

  int iApp2 = m_nAperture / 2;
  int iFirst = iApp2;
  int iLast = m_pOriginalTrack->PointCount - iApp2 - 1;

  for(int i = 1; i < m_pOriginalTrack->PointCount - 1; i++)
  {
    PGPXTrackPoint pFirstPoint, pLastPoint, pPoint;
    int n1, n2;

    if(i < iFirst)
    {
      n1 = 0;
      n2 = 2 * i;
    }
    else if(i > iLast)
    {
      n1 = 2 * i - (m_pOriginalTrack->PointCount - 1);
      n2 = m_pOriginalTrack->PointCount - 1;
    }
    else
    {
      n1 = i - iApp2;
      n2 = i + iApp2;
    }

    //pFirstPoint = m_pOriginalTrack->Points[n1];
    //pLastPoint = m_pOriginalTrack->Points[n2];
    pFirstPoint = m_pCurrentTrack->Points[n1];
    pLastPoint = m_pCurrentTrack->Points[n2];

    double nMaxDist;
    double x1, y1, x2, y2, x0, y0;
    pFirstPoint->GetdProj(&x1, &y1);
    pLastPoint->GetdProj(&x2, &y2);

    //pPoint = m_pOriginalTrack->Points[i];
    pPoint = m_pCurrentTrack->Points[i];
    pPoint->GetdProj(&x0, &y0);
    nMaxDist = DistPS(x1, y1, x2, y2, x0, y0);

    pPoint = m_pCurrentTrack->Points[i];
    MovePoint(pPoint, x1, y1, x2, y2, nMaxDist);

    if(m_hParentWnd)
    {
      SendMessage(m_hParentWnd, DPHFILTER_POINT, 0, 0);
      Application->ProcessMessages();
    }
  }

  if(m_bNeedReduce) m_pCurrentTrack->RemoveInactivePoints();
  m_pCurrentTrack->RecalcDist();

  delete m_pOriginalTrack;
}

void __fastcall DP_HDOP_Smooth::ProcessSegment(int nFirst, int nLast)
{
  if(abs(nLast - nFirst) > 1)
  {
    PGPXTrackPoint pFirstPoint = m_pOriginalTrack->Points[nFirst];
    PGPXTrackPoint pLastPoint = m_pOriginalTrack->Points[nLast];

    double x1, y1, x2, y2;
    pFirstPoint->GetdProj(&x1, &y1);
    pLastPoint->GetdProj(&x2, &y2);

    double nMaxDist = 0.0;
    int nIndex;

    for(int i = nFirst + 1; i < nLast; i++)
    {
      PGPXTrackPoint pPoint = m_pOriginalTrack->Points[i];

      double x0, y0;
      pPoint->GetdProj(&x0, &y0);

      double dist = DistPS(x1, y1, x2, y2, x0, y0);

      if(dist > nMaxDist)
      {
        nMaxDist = dist;
        nIndex = i;
      }

      if(m_hParentWnd)
      {
        SendMessage(m_hParentWnd, DPHFILTER_POINT, 0, 0);
        Application->ProcessMessages();
      }
    }

    PGPXTrackPoint pPoint = m_pCurrentTrack->Points[nIndex];
    MovePoint(pPoint, x1, y1, x2, y2, nMaxDist);
    ProcessSegment(nFirst, nIndex);
    ProcessSegment(nIndex, nLast);
  }
}

double __fastcall DP_HDOP_Smooth::DistPP(double x1, double y1,
                                         double x2, double y2)
{
  double dx = x1 - x2, dy = y1 - y2;
  return dx * dx + dy * dy;
}

double __fastcall DP_HDOP_Smooth::DistPS(double x1, double y1,
                                         double x2, double y2,
                                         double x0, double y0)
{
  double a = DistPP(x0, y0, x1, y1);
  double b = DistPP(x0, y0, x2, y2);
  double c = DistPP(x1, y1, x2, y2);

  double dist;

  if(a >= b + c) dist = sqrt(b);
  else if( b >= a + c) dist = sqrt(a);
  else
  {
    a = sqrt(a); b = sqrt(b); c = sqrt(c);
    double p = (a + b + c) / 2;
    double s = sqrt((p - a) * (p - b) * (p - c) * p);
    dist = s * 2.0 / c;
  }

  return dist;
}

void __fastcall DP_HDOP_Smooth::SetCurrentTrack(PGPXTrack value)
{
  if(m_pCurrentTrack != value)
    m_pCurrentTrack = value;
}

PGPXTrack __fastcall DP_HDOP_Smooth::GetCurrentTrack()
{
  return m_pCurrentTrack;
}

void __fastcall DP_HDOP_Smooth::MovePoint(PGPXTrackPoint pPoint,
                                          double x1, double y1,
                                          double x2, double y2,
                                          double nMaxDist)
{
  double x0, y0, _x, _y, offs = pPoint->m_dHDOP * HDOP_FACTOR;
  pPoint->GetdProj(&x0, &y0);
  bool bBorder = InterseptPoint(x1, y1, x2, y2, x0, y0, _x, _y);

  if(offs > nMaxDist)
  {
    if(bBorder)
    {
      pPoint->m_bActive = false;
      m_bNeedReduce = true;
    }
    else pPoint->SetdProj(_x, _y);
  }
  else
  {
    double factor = offs / nMaxDist;
    double dx = _x - x0, dy = _y- y0;
    x0 += (dx * factor);
    y0 += (dy * factor);
    pPoint->SetdProj(x0, y0);
  }
}

bool __fastcall DP_HDOP_Smooth::InterseptPoint(double x1, double y1,
                                               double x2, double y2,
                                               double x0, double y0,
                                               double & _x, double & _y)
{
  bool bBorder = true;

  double a = DistPP(x0, y0, x1, y1);
  double b = DistPP(x0, y0, x2, y2);
  double c = DistPP(x1, y1, x2, y2);

  if(a >= b + c)
  {
    _x = x2;
    _y = y2;
  }
  else if(b >= a + c)
  {
    _x = x1;
    _y = y1;
  }
  else
  {
    bBorder = false;
    double x21 = x2 - x1;
    double y21 = y2 - y1;
    double x21_2 = x21 * x21;
    double y21_2 = y21 * y21;
    _x = (x21 * y21 * (y0 - y1) + x1 * y21_2 + x0 * x21_2)/(y21_2 + x21_2);
    _y = y21 * (_x - x1) / x21 + y1;
  }

  return bBorder;
}

void __fastcall DP_HDOP_Smooth::SetAperture(int value)
{
  if(m_nAperture != value) {
    m_nAperture = value;
  }
}
int __fastcall DP_HDOP_Smooth::GetAperture()
{
  return m_nAperture;
}
