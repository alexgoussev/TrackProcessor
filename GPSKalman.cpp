//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <stdio.h>
#pragma hdrstop

#include "SK42WGS84\\SK42WGS84.h"
#include "GPSKalman.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall GPSKalman2D::GPSKalman2D()
{
  m_mF.resize(4, 4);
  m_mB.resize(4, 2);
  m_mP.resize(4, 4);
  m_mR.resize(4, 4);
  m_mQ.resize(4, 4);
  m_mI.resize(4, 4);
  m_mH.resize(4, 4);
  m_vX.resize(4);

  m_mFs.resize(2, 2);
  m_mBs.resize(2, 2);
  m_mPs.resize(2, 2);
  m_mRs.resize(2, 2);
  m_mQs.resize(2, 2);
  m_mIs.resize(2, 2);
  m_mHs.resize(2, 2);
  m_vXs.resize(2);

  m_mFa.resize(2, 2);
  m_mBa.resize(2, 2);
  m_mPa.resize(2, 2);
  m_mRa.resize(2, 2);
  m_mQa.resize(2, 2);
  m_mIa.resize(2, 2);
  m_mHa.resize(2, 2);
  m_vXa.resize(2);

  m_mI.Row(1) << 1.0 << 0.0 << 0.0 << 0.0;
  m_mI.Row(2) << 0.0 << 1.0 << 0.0 << 0.0;
  m_mI.Row(3) << 0.0 << 0.0 << 1.0 << 0.0;
  m_mI.Row(4) << 0.0 << 0.0 << 0.0 << 1.0;

  m_mH.Row(1) << 1.0 << 0.0 << 0.0 << 0.0;
  m_mH.Row(2) << 0.0 << 1.0 << 0.0 << 0.0;
  m_mH.Row(3) << 0.0 << 0.0 << 1.0 << 0.0;
  m_mH.Row(4) << 0.0 << 0.0 << 0.0 << 1.0;

  m_mR.Row(1) << 10.0 <<  0.0 <<  0.0 <<  0.0;
  m_mR.Row(2) <<  0.0 << 10.0 <<  0.0 <<  0.0;
  m_mR.Row(3) <<  0.0 <<  0.0 << 10.0 <<  0.0;
  m_mR.Row(4) <<  0.0 <<  0.0 <<  0.0 << 10.0;

  m_mIs.Row(1) << 1.0 << 0.0;
  m_mIs.Row(2) << 0.0 << 1.0;

  m_mHs.Row(1) << 1.0 << 0.0;
  m_mHs.Row(2) << 0.0 << 1.0;

  m_mRs.Row(1) << 10.0 <<  0.0;
  m_mRs.Row(2) <<  0.0 << 10.0;

  m_mIa.Row(1) << 1.0 << 0.0;
  m_mIa.Row(2) << 0.0 << 1.0;

  m_mHa.Row(1) << 1.0 << 0.0;
  m_mHa.Row(2) << 0.0 << 1.0;

  m_mRa.Row(1) << 10.0 <<  0.0;
  m_mRa.Row(2) <<  0.0 << 10.0;

  Reset();
}

__fastcall GPSKalman2D::~GPSKalman2D()
{
}

void __fastcall GPSKalman2D::Reset()
{
  m_dX = m_dY = m_dA = m_dVx = m_dVy = m_dVa = m_dAx = m_dAy = m_dAa = m_dTime = 0.0;
  m_dSigma = m_dAltSigma = 1.0;

  m_mP.Row(1) << 50.0 <<  0.0 <<  0.0 <<  0.0;
  m_mP.Row(2) <<  0.0 << 50.0 <<  0.0 <<  0.0;
  m_mP.Row(3) <<  0.0 <<  0.0 << 50.0 <<  0.0;
  m_mP.Row(4) <<  0.0 <<  0.0 <<  0.0 << 50.0;

  m_mPs.Row(1) << 50.0 <<  0.0;
  m_mPs.Row(2) <<  0.0 << 50.0;

  m_mPa.Row(1) << 50.0 <<  0.0;
  m_mPa.Row(2) <<  0.0 << 50.0;

  m_bFirstPoint = m_bFilterAlt = true;
}

void __fastcall GPSKalman2D::Filter(double dTime, double & dLat, double & dLon,
                                    double & dSpeed, double & dCourse,
                                    double & dAlt, double & dAltSpeed)
{
  double dVx, dVy, dS, dC;
  double dY = Lon2MercatorX(dLon);
  double dX = Lat2MercatorY(dLat);
  DecomposeSpeed(dSpeed, dCourse, &dVx, &dVy);

  if(!m_bFirstPoint)
  {
    double dT = dTime - m_dTime;

    if(dT > 0.0)
    {
      m_dAx = (dVx - m_dVx) / dT;
      m_dAy = (dVy - m_dVy) / dT;

      if(m_bFilterAlt) m_dAa = (dAltSpeed - m_dVa) / dT;
    }
    else
      m_dAx = m_dAy = m_dAa =0.0;

    Predict(dTime);
    Update(dX, dY, dVx, dVy, dAlt, dAltSpeed);

    dX = m_vX(1);
    dY = m_vX(2);
    dVx = m_vXs(1);
    dVy = m_vXs(2);

    if(m_bFilterAlt)
    {
      dAlt = m_vXa(1);
      dAltSpeed = m_vXa(2);
    }

    dLon = MercatorX2Lon(dY);
    dLat = MercatorY2Lat(dX);

    ComposeSpeed(dVx, dVy, &dS, &dC);
    dSpeed = dS; dCourse = dC;
  }

  SetData(dTime, dX, dY, dVx, dVy, dAlt, dAltSpeed);
  m_bFirstPoint = false;
}


/*
¬ектор основных величин:

X
Y
Vx
Vy

¬ектор управл€ющих воздействий:

Ax
Ay

F - матрица

1 0 dT 0
0 1  0 dT
0 0  1 0
0 0  0 1

B - матрица

dT^2/2     0
    0  dT^2/2
   dT      0
    0     dT  

*/
void __fastcall GPSKalman2D::Predict(double dTime)
{
  double dT = dTime - m_dTime;
  double dTsq = dT * dT / 2.0;

  m_mF.Row(1) << 1.0 << 0.0 << dT  << 0.0;
  m_mF.Row(2) << 0.0 << 1.0 << 0.0 << dT;
  m_mF.Row(3) << 0.0 << 0.0 << 1.0 << 0.0;
  m_mF.Row(4) << 0.0 << 0.0 << 0.0 << 1.0;

  m_mFs.Row(1) << 1.0 << 0.0;
  m_mFs.Row(2) << 0.0 << 1.0;

  m_mFa.Row(1) << 1.0 <<  dT;
  m_mFa.Row(2) << 0.0 << 1.0;

  m_mB.Row(1) << dTsq <<  0.0;
  m_mB.Row(2) <<  0.0 << dTsq;
  m_mB.Row(3) <<   dT <<  0.0;
  m_mB.Row(4) <<  0.0 <<   dT;

  m_mBs.Row(1) <<  dT << 0.0;
  m_mBs.Row(2) << 0.0 <<  dT;

  m_mBa.Row(1) << dTsq << 0.0;
  m_mBa.Row(2) <<  0.0 <<  dT;

  ColumnVector T(4);
  T << m_dX << m_dY << m_dVx << m_dVy;

  ColumnVector Ts(2);
  Ts << m_dVx << m_dVy;

  ColumnVector U(2);
  U << m_dAx << m_dAy;

  m_vX = m_mF * T + m_mB * U;

  m_vXs = m_mFs * Ts + m_mBs * U;

  if(m_bFilterAlt)
  {
    ColumnVector Ta(2);
    Ta << m_dA << m_dVa;

    m_vXa = m_mFa * Ta;
  }

  m_mQ = m_mB * m_mB.t();
  m_mQ *= m_dSigma;

  m_mQs = m_mBs * m_mBs.t();
  m_mQs *= m_dSigma;

  if(m_bFilterAlt)
  {
    m_mQa = m_mBa * m_mBa.t();
    m_mQa *= m_dAltSigma;
  }

  //P = F * P * Ft + Q
  Matrix Temp(4, 4);

  Temp = m_mF * m_mP;
  m_mP = Temp * m_mF.t();
  m_mP += m_mQ;

  Matrix Temps(2, 2);

  Temps = m_mFs * m_mPs;
  m_mPs = Temps * m_mFs.t();
  m_mPs += m_mQs;

  if(m_bFilterAlt)
  {
    Matrix Tempa(2, 2);

    Tempa = m_mFa * m_mPa;
    m_mPa = Tempa * m_mFa.t();
    m_mPa += m_mQa;
  }
}

void __fastcall GPSKalman2D::SetData(double dTime, double dX, double dY, double dVx, double dVy,
                                     double dAlt, double dAltSpeed)
{
  m_dTime = dTime;
  m_dX = dX;
  m_dY = dY;
  m_dVx = dVx;
  m_dVy = dVy;

  if(m_bFilterAlt)
  {
    m_dA = dAlt;
    m_dVa = dAltSpeed;
  }
}

void __fastcall GPSKalman2D::SetR(double val)
{
  m_mR.Row(1) << val << 0.0 << 0.0 << 0.0;
  m_mR.Row(2) << 0.0 << val << 0.0 << 0.0;
  m_mR.Row(3) << 0.0 << 0.0 << val << 0.0;
  m_mR.Row(4) << 0.0 << 0.0 << 0.0 << val;

  m_mRs.Row(1) << val << 0.0;
  m_mRs.Row(2) << 0.0 << val;

  m_mRa.Row(1) << val << 0.0;
  m_mRa.Row(2) << 0.0 << val;
}

void __fastcall GPSKalman2D::Update(double dX, double dY, double dVx, double dVy,
                                    double dAlt, double dAltSpeed)
{
  Matrix Temp1(4, 4);
  Matrix Temp2(4, 4);
  Matrix K(4, 4);
  ColumnVector Z(4);
  ColumnVector T1(4);

  Matrix Temp1s(2, 2);
  Matrix Temp2s(2, 2);
  Matrix Ks(2, 2);
  ColumnVector Zs(2);
  ColumnVector T1s(2);

  Matrix Temp1a(2, 2);
  Matrix Temp2a(2, 2);
  Matrix Ka(2, 2);
  ColumnVector Za(2);
  ColumnVector T1a(2);

  //K = P * Ht / (H * P * Ht + R)
  Temp1 = m_mP * m_mH.t();
  Temp2 = m_mH * Temp1;
  Temp2 += m_mR;
  K = Temp1 * Temp2.i();

  Z << dX << dY << dVx << dVy;

  Temp1s = m_mPs * m_mHs.t();
  Temp2s = m_mHs * Temp1s;
  Temp2s += m_mRs;
  Ks = Temp1s * Temp2s.i();

  Zs << dVx << dVy;

  if(m_bFilterAlt)
  {
    Temp1a = m_mPa * m_mHa.t();
    Temp2a = m_mHa * Temp1a;
    Temp2a += m_mRa;
    Ka = Temp1a * Temp2a.i();

    Za << dAlt << dAltSpeed;
  }

  //X = X + K * (Z - H * X)
  T1 = m_mH * m_vX;
  Z -= T1;
  T1 = K * Z;
  m_vX += T1;

  T1s = m_mHs * m_vXs;
  Zs -= T1s;
  T1s = Ks * Zs;
  m_vXs += T1s;

  if(m_bFilterAlt)
  {
    T1a = m_mHa * m_vXa;
    Za -= T1a;
    T1a = Ka * Za;
    m_vXa += T1a;
  }

  //P = P * (I - K * H)
  Temp1 = K * m_mH;
  Temp2 = m_mI - Temp1;
  m_mP *= Temp2;

  Temp1s = Ks * m_mHs;
  Temp2s = m_mIs - Temp1s;
  m_mPs *= Temp2s;

  if(m_bFilterAlt)
  {
    Temp1a = Ka * m_mHa;
    Temp2a = m_mIa - Temp1a;
    m_mPa *= Temp2a;
  }
}

void __fastcall GPSKalman2D::SetSigma2(double value)
{
  if(m_dSigma != value) {
    m_dSigma = value;
  }
}

void __fastcall GPSKalman2D::SetAltSigma(double value)
{
  if(m_dAltSigma != value) {
    m_dAltSigma = value;
  }
}

void __fastcall GPSKalman2D::SetFilterAlt(bool value)
{
  if(m_bFilterAlt != value) {
    m_bFilterAlt = value;
  }
}
