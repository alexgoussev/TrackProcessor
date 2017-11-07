//---------------------------------------------------------------------------

#ifndef DouglasPeuckerH
#define DouglasPeuckerH

#include "GPXData.h"

#define DPFILTER_POINT    (WM_APP + 701)
//---------------------------------------------------------------------------

class DouglasPeucker
{
public:
  __fastcall DouglasPeucker(HWND hParentWnd = NULL);
  __fastcall ~DouglasPeucker();
  void __fastcall Simplify();
  __property PGPXTrack CurrentTrack  = { read=GetCurrentTrack, write=SetCurrentTrack };
  __property double Epsilon  = { read=GetEpsilon, write=SetEpsilon };
protected:
  HWND m_hParentWnd;
  double __fastcall GetDistance(double x1, double y1, double x2, double y2, double x0, double y0);
  void __fastcall SimplifySegment(int nFirst, int nLast);
  double __fastcall DistPP(double x1, double y1, double x2, double y2);
  double __fastcall DistPS(double x1, double y1, double x2, double y2, double x0, double y0);
private:
  PGPXTrack m_pCurrentTrack;
  double m_dEpsilon;
  void __fastcall SetCurrentTrack(PGPXTrack value);
  PGPXTrack __fastcall GetCurrentTrack();
  void __fastcall SetEpsilon(double value);
  double __fastcall GetEpsilon();

};

#endif

