//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <float.h>
#pragma hdrstop

#include "Drift.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall Drift::Drift(HWND hParent)
{
  m_pIntervals = new TList;
  m_hParent = hParent;
}

__fastcall Drift::~Drift()
{
  while(IntervalCount) DeleteInterval(0);
  delete m_pIntervals;
}

void __fastcall Drift::AddInterval(DriftInterval * pInterval)
{
  m_pIntervals->Add(pInterval);
}

void __fastcall Drift::DeleteInterval(int i)
{
  PDriftInterval pInterval = Intervals[i];
  m_pIntervals->Delete(i);
  delete pInterval;
}

PDriftInterval __fastcall Drift::GetInterval(int i)
{
  PDriftInterval pInterval = (PDriftInterval)m_pIntervals->Items[i];
  return pInterval;
}

int __fastcall Drift::GetIntervalCount()
{
  return m_pIntervals->Count;
}

int __fastcall Drift::FindIntervals(int nAperture, double dThereshould)
{
  if(m_pCurrentTrack)
  {
    double sth = (double)nAperture, d;

    for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
    {
      int k;

      for(int j = i + 1; j < m_pCurrentTrack->PointCount; j++)
      {
        d = m_pCurrentTrack->GetDistance(i, j);
        k = j;

        if(d >= sth) break;
      }

      if(k > i)
      {
        PGPXTrackPoint pi = m_pCurrentTrack->Points[i];
        PGPXTrackPoint pj = m_pCurrentTrack->Points[k];
        double vd = 3.6 * d / (pj->m_dTime - pi->m_dTime);

        if(vd < dThereshould)
        {
          DriftInterval *pInterval = new DriftInterval;
          pInterval->nStart = i;
          pInterval->nEnd = k;
          m_pIntervals->Add(pInterval);
          pInterval = NULL;
        }
      }

      if(m_hParent)
      {
        SendMessage(m_hParent, DRIFTFILTER_POINT, 0, 0);
        Application->ProcessMessages();
      }
    }
  }

  return IntervalCount;
}

int __fastcall Drift::MergeIntervals()
{
  int i = 1;

  while(i < IntervalCount)
  {
    DriftInterval *pInt_ = Intervals[i - 1];
    DriftInterval *pInt = Intervals[i];

    if(pInt_->nEnd + 1 >= pInt->nStart)
    {
      pInt_->nEnd = pInt->nEnd;
      DeleteInterval(i);
    }
    else i++;

    if(m_hParent)
    {
      SendMessage(m_hParent, DRIFTFILTER_POINT, 0, 0);
      Application->ProcessMessages();
    }
  }

  return IntervalCount;
}

void __fastcall Drift::ProcessIntervals(eStrength FilterStrenght)
{
  int nDivider = (int)FilterStrenght;
  Median *pMedian = new Median;

  while(IntervalCount)
  {
    DriftInterval *pInt = Intervals[0];

    int n = (pInt->nEnd - pInt->nStart + 1) / nDivider;
    if(n % 2 == 0) n++;

    int n2 = n / 2;
    double *X = new double[n];
    double *Y = new double[n];
    PGPXTrackPoint p;

    for(int i = pInt->nStart; i <= pInt->nEnd; i++)
    {
      for(int j = 0; j < n; j++)
      {
        int k = i - n2 + j;

        if(k < pInt->nStart) k = pInt->nStart;
        else if(k > pInt->nEnd) k = pInt->nEnd;

        p = m_pCurrentTrack->Points[k];
        p->GetdProj(&X[j], &Y[j]);
      }

      p = m_pCurrentTrack->Points[i];
      p->SetdProj(pMedian->GetMedian(X, n), pMedian->GetMedian(Y, n));
    }

    delete [] X;
    delete [] Y;

    DeleteInterval(0);

    if(m_hParent)
    {
      SendMessage(m_hParent, DRIFTFILTER_POINT, 0, 0);
      Application->ProcessMessages();
    }
  }

  delete pMedian;
}
