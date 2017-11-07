//---------------------------------------------------------------------------

#ifndef GPSKalmanH
#define GPSKalmanH
//---------------------------------------------------------------------------

#include "newmat\\newmat.h"

// Q << R - сильная фильтрация и инерционность
// Q ~ R - слабая фильтрация, нет искажений АЧХ
class GPSKalman2D
{
public:
  __fastcall GPSKalman2D();
  __fastcall ~GPSKalman2D();
  void __fastcall Reset();
  void __fastcall Filter(double dTime, double & dLat, double & dLon,
                         double & dSpeed, double & dCourse, double & dAlt, double & dAltSpeed);
  __property double R  = { write=SetR };
  __property double Sigma2  = { write=SetSigma2 };
  __property double AltSigma  = { write=SetAltSigma };
  __property bool FilterAlt  = { write=SetFilterAlt };
protected:
  double m_dSigma;
  double m_dAltSigma;
  double m_dX;
  double m_dY;
  double m_dA;
  double m_dVx;
  double m_dVy;
  double m_dVa;
  double m_dAx;
  double m_dAy;
  double m_dAa;
  double m_dTime;
  Matrix m_mF;
  Matrix m_mB;
  Matrix m_mP;
  Matrix m_mR;
  Matrix m_mQ;
  Matrix m_mI;
  Matrix m_mH;
  ColumnVector m_vX;
  Matrix m_mFs;
  Matrix m_mBs;
  Matrix m_mPs;
  Matrix m_mRs;
  Matrix m_mQs;
  Matrix m_mIs;
  Matrix m_mHs;
  ColumnVector m_vXs;
  Matrix m_mFa;
  Matrix m_mBa;
  Matrix m_mPa;
  Matrix m_mRa;
  Matrix m_mQa;
  Matrix m_mIa;
  Matrix m_mHa;
  ColumnVector m_vXa;
  bool m_bFirstPoint;
  bool m_bFilterAlt;
  void __fastcall Predict(double dTime);
  void __fastcall SetData(double dTime, double dX, double dY, double dSpeed, double dCourse,
                          double dAlt, double dAltSpeed);
  void __fastcall Update(double dX, double dY, double dSpeed, double dCourse,
                         double dAlt, double dAltSpeed);
private:
  void __fastcall SetR(double val);
  void __fastcall SetSigma2(double value);
  void __fastcall SetAltSigma(double value);
  void __fastcall SetFilterAlt(bool value);
};

#endif
