//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "DouglasPeucker.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall DouglasPeucker::DouglasPeucker(HWND hParentWnd)
{
  m_hParentWnd = hParentWnd;
}

__fastcall DouglasPeucker::~DouglasPeucker()
{
  //TODO: Add your source code here
}

double __fastcall DouglasPeucker::GetDistance(double x1, double y1,
                                              double x2, double y2,
                                              double x0, double y0)
{
  //Line: (Y1 - Y2) * x + (X2 - X1) * y + (X2 * Y1 - X1 * Y2) = 0
  double A = y1 - y2;
  double B = x2 - x1;
  double C = x2 * y1 - x1 * y2;

  return fabs(A * x0 + B * y0 + C) / sqrt(A * A + B * B);
}

void __fastcall DouglasPeucker::SetCurrentTrack(PGPXTrack value)
{
  if(m_pCurrentTrack != value)
    m_pCurrentTrack = value;
}

PGPXTrack __fastcall DouglasPeucker::GetCurrentTrack()
{
  return m_pCurrentTrack;
}

void __fastcall DouglasPeucker::SetEpsilon(double value)
{
  if(m_dEpsilon != value)
    m_dEpsilon = value;
}

double __fastcall DouglasPeucker::GetEpsilon()
{
  return m_dEpsilon;
}

void __fastcall DouglasPeucker::Simplify()
{
  m_pCurrentTrack->SetPointsActive(false);
  SimplifySegment(0, m_pCurrentTrack->PointCount - 1);
  m_pCurrentTrack->RemoveInactivePoints();
  m_pCurrentTrack->RecalcDistClimb();
}

void __fastcall DouglasPeucker::SimplifySegment(int nFirst, int nLast)
{
  if(abs(nLast - nFirst) > 1)
  {
    PGPXTrackPoint pFirstPoint = m_pCurrentTrack->Points[nFirst];
    PGPXTrackPoint pLastPoint = m_pCurrentTrack->Points[nLast];

    pFirstPoint->m_bActive = true;
    pLastPoint->m_bActive = true;

    double x1, y1, x2, y2;
    pFirstPoint->GetdProj(&x1, &y1);
    pLastPoint->GetdProj(&x2, &y2);

    double nMaxDist = 0.0;
    int nIndex;

    for(int i = nFirst + 1; i < nLast; i++)
    {
      PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];

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
        SendMessage(m_hParentWnd, DPFILTER_POINT, 0, 0);
        Application->ProcessMessages();
      }
    }

    if(nMaxDist > m_dEpsilon)
    {
      PGPXTrackPoint pPoint = m_pCurrentTrack->Points[nIndex];
      pPoint->m_bActive = true;

      SimplifySegment(nFirst, nIndex);
      SimplifySegment(nIndex, nLast);
    }
  }
}

double __fastcall DouglasPeucker::DistPP(double x1, double y1, double x2, double y2)
{
  double dx = x1 - x2, dy = y1 - y2;
  return dx * dx + dy * dy;
}

double __fastcall DouglasPeucker::DistPS(double x1, double y1,
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