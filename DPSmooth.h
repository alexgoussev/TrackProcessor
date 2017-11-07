//---------------------------------------------------------------------------

#ifndef DPSmoothH
#define DPSmoothH

#include "GPXData.h"

#define HDOP_FACTOR             (5.0)
#define DPHFILTER_POINT    (WM_APP + 901)

//---------------------------------------------------------------------------

class DP_HDOP_Smooth
{

public:
  __fastcall DP_HDOP_Smooth(HWND hParentWnd = NULL);
  __fastcall ~DP_HDOP_Smooth();
  void __fastcall Process();
  __property PGPXTrack CurrentTrack  = { read=GetCurrentTrack, write=SetCurrentTrack };
  __property int Aperture  = { read=GetAperture, write=SetAperture };
protected:
  HWND m_hParentWnd;
  PGPXTrack m_pOriginalTrack;
  bool m_bNeedReduce;
  void __fastcall ProcessSegment(int nFirst, int nLast);
  double __fastcall DistPP(double x1, double y1, double x2, double y2);
  double __fastcall DistPS(double x1, double y1, double x2, double y2, double x0, double y0);
  void __fastcall MovePoint(PGPXTrackPoint pPoint, double x1, double y1, double x2, double y2, double nMaxDist);
  bool __fastcall InterseptPoint(double x1, double y1, double x2, double y2, double x0, double y0, double & _x, double & _y);
private:
  PGPXTrack m_pCurrentTrack;
  int m_nAperture;
  void __fastcall SetCurrentTrack(PGPXTrack value);
  PGPXTrack __fastcall GetCurrentTrack();
  void __fastcall SetAperture(int value);
  int __fastcall GetAperture();

};

#endif
