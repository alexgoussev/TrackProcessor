//---------------------------------------------------------------------------

#ifndef WAvgH
#define WAvgH

#include "GPXData.h"
#include "Spline.h"

#define WAVGFILTER_POINT    (WM_APP + 401)

class WheitedAverage
{

public:
  __fastcall WheitedAverage(HWND hParentWnd = NULL);
  __fastcall ~WheitedAverage();
  int __fastcall BuildAvgTrack();
  void __fastcall Interpolate();
  void __fastcall BuildSplines();
  __property PGPXTrack AvgTrack  = { read=GetAvgTrack };
  __property int AvgTrackPoints  = { read=GetAvgTrackPoints };
  __property PGPXTrack CurrentTrack  = { write=SetCurrentTrack };
  __property double DistTh  = { read=GetDistTh, write=SetDistTh };
  __property double TimeTh  = { read=GetTimeTh, write=SetTimeTh };
  __property double SpeedTh  = { read=GetSpeedTh, write=SetSpeedTh };
  __property double CourceTh  = { read=GetCourceTh, write=SetCourceTh };
  __property double AvgTime  = { read=GetAvgTime, write=SetAvgTime };
  __property double SmoothFactor  = { read=GetSmoothFactor, write=SetSmoothFactor };
  __property bool AltSmooth  = { write=SetAltSmooth };
  __property bool SpeedSmooth  = { write=SetSpeedSmooth };
protected:
  PGPXTrack m_pAvgTrack;
  PGPXTrack m_pCurrentTrack;
  double m_dDistTh;
  double m_dAvgTimeTh;
  double m_dTimeTh;
  double m_dSpeedTh;
  double m_dCourceTh;
  double m_dSmooth;
  AkimaSpline * m_pSplineX;
  AkimaSpline * m_pSplineY;
  AkimaSpline * m_pSplineA;
  AkimaSpline * m_pSplineVx;
  AkimaSpline * m_pSplineVy;
  //AkimaSpline * m_pSplineV;
  //AkimaSpline * m_pSplineC;
  HWND m_hParentWnd;
  void __fastcall Add2Avg(PGPXTrackPoint pNewPoint, PGPXTrackPoint pi);
private:
  bool m_bAltSmooth;
  bool m_bSpeedSmooth;
  PGPXTrack __fastcall GetAvgTrack();
  int __fastcall GetAvgTrackPoints();
  void __fastcall SetCurrentTrack(PGPXTrack value);
  void __fastcall SetDistTh(double value);
  double __fastcall GetDistTh();
  void __fastcall SetTimeTh(double value);
  double __fastcall GetTimeTh();
  void __fastcall SetSpeedTh(double value);
  double __fastcall GetSpeedTh();
  void __fastcall SetCourceTh(double value);
  double __fastcall GetCourceTh();
  void __fastcall SetAvgTime(double value);
  double __fastcall GetAvgTime();
  void __fastcall SetSmoothFactor(double value);
  double __fastcall GetSmoothFactor();
  void __fastcall SetAltSmooth(bool value);
  void __fastcall SetSpeedSmooth(bool value);
};

//---------------------------------------------------------------------------
#endif
