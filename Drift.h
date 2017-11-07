//---------------------------------------------------------------------------

#ifndef DriftH
#define DriftH
//---------------------------------------------------------------------------
#include "GPXData.h"
#include "Median.h"

#define DRIFTFILTER_POINT    (WM_APP + 501)

enum eStrength {eExtremely = 1, eHigh = 2, eAverage = 4, eLow = 8};

struct DriftInterval
{
  int nStart;
  int nEnd;
  int nPoints;
};

typedef DriftInterval* PDriftInterval;

class Drift
{

public:
  PGPXTrack m_pCurrentTrack;
  __fastcall Drift(HWND hParent = NULL);
  __fastcall ~Drift();
  void __fastcall AddInterval(DriftInterval * pInterval);
  void __fastcall DeleteInterval(int i);
  int __fastcall FindIntervals(int nAperture, double dThereshould);
  int __fastcall MergeIntervals();
  void __fastcall ProcessIntervals(eStrength FilterStrenght);
  __property PDriftInterval Intervals[int i] = { read=GetInterval };
  __property int IntervalCount  = { read=GetIntervalCount };
protected:
  TList * m_pIntervals;
  HWND m_hParent;
private:
  PDriftInterval __fastcall GetInterval(int i);
  int __fastcall GetIntervalCount();

};

#endif
