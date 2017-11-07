//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Statistics.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall Statistics::Statistics(double * X, double * Y, int n)
{
  m_dSumX = m_dSumY = m_dSumX2 = m_dSumX3 = m_dSumX4 = m_dSumY2 = m_dSum2X = m_dSumXY = m_dSumX2Y = 0.0;
  m_nN = n;

  m_pX = new long double[n];
  m_pY = new long double[n];

  for(int i = 0; i < n; i++)
  {
    long double x2 = X[i] * X[i];

    m_pX[i] = (long double)X[i];
    m_pY[i] = (long double)Y[i];
    m_dSumX += (long double)X[i];
    m_dSumY += (long double)Y[i];
    m_dSumX2 += x2;
    m_dSumX3 += ((long double)X[i] * x2);
    m_dSumX4 += (x2 * x2);
    m_dSumY2 += ((long double)Y[i] * (long double)Y[i]);
    m_dSumXY += ((long double)X[i] * (long double)Y[i]);
    m_dSumX2Y += (x2 * Y[i]);
  }

  m_dSum2X = m_dSumX * m_dSumX;

  long double avX = (long double)AverageX;
  long double avY = (long double)AverageY;

  m_dSumVarX = m_dSumVarY = m_dSumVarXY = m_dSumVarX2 = m_dSumVarY2 = 0.0;

  for(int i = 0; i < n; i++)
  {
    long double dX = (long double)X[i] - avX;
    long double dY = (long double)Y[i] - avY;

    m_dSumVarX += dX;
    m_dSumVarY += dY;
    m_dSumVarXY += (dX * dY);
    m_dSumVarX2 += (dX * dX);
    m_dSumVarY2 += (dY * dY);
  }
}

__fastcall Statistics::~Statistics()
{
  delete [] m_pX;
  delete [] m_pY;
}

double __fastcall Statistics::GetCorrelation()
{
  return (m_dSumVarX2 != 0.0) ? m_dSumVarXY / sqrtl(m_dSumVarX2 * m_dSumVarY2) : 0.0;
}

double __fastcall Statistics::GetLinearR2()
{
  long double ldSSE = LinearSSE / (long double)(m_nN - 1);
  long double ldSST = SST / (long double)(m_nN - 2);

  return (ldSST != 0.0) ? (1.0 - ldSSE / ldSST) : 0.0;
}

double __fastcall Statistics::GetSquareR2()
{
  long double ldSSE = SquareSSE / (long double)(m_nN - 1);
  long double ldSST = SST / (long double)(m_nN - 3);

  return (ldSST != 0.0) ? (1.0 - ldSSE / ldSST) : 0.0;
}

double __fastcall Statistics::GetLinearA()
{
  long double D = (long double)m_nN * m_dSumX2 - m_dSumX * m_dSumX;
  long double D1 = (long double)m_nN * m_dSumXY - m_dSumX * m_dSumY;

  return (D != 0.0) ? (double)(D1 / D) : 0.0;
}

double __fastcall Statistics::GetLinearB()
{
  long double D = (long double)m_nN * m_dSumX2 - m_dSumX * m_dSumX;
  long double D0 = m_dSumX2 * m_dSumY - m_dSumX * m_dSumXY;

  return (D != 0.0) ? (double)(D0 / D) : 0.0;
}

double __fastcall Statistics::GetSquareA()
{
  long double D = (long double)m_nN * m_dSumX2 * m_dSumX4 +
                  2.0 * m_dSumX * m_dSumX3 * m_dSumX2 -
                  m_dSumX2 * m_dSumX2 * m_dSumX2 -
                  m_dSumX * m_dSumX * m_dSumX4 -
                 (long double)m_nN * m_dSumX3 * m_dSumX3;
  long double D2 = (long double)m_nN * m_dSumX2 * m_dSumX2Y +
                   m_dSumX * m_dSumX3 * m_dSumY +
                   m_dSumX * m_dSumX2 * m_dSumXY -
                   m_dSumX2 * m_dSumX2 * m_dSumY -
                   m_dSumX * m_dSumX * m_dSumX2Y -
                  (long double)m_nN * m_dSumX3 * m_dSumXY;
  return (D != 0.0) ? (double)(D2 / D) : 0.0;
}

double __fastcall Statistics::GetSquareB()
{
  long double D = (long double)m_nN * m_dSumX2 * m_dSumX4 +
                  2.0 * m_dSumX * m_dSumX3 * m_dSumX2 -
                  m_dSumX2 * m_dSumX2 * m_dSumX2 -
                  m_dSumX * m_dSumX * m_dSumX4 -
                  m_nN * m_dSumX3 * m_dSumX3;
  long double D1 = (long double)m_nN * m_dSumX4 * m_dSumXY +
                   m_dSumX * m_dSumX2 * m_dSumX2Y +
                   m_dSumX3 * m_dSumX2 * m_dSumY -
                   m_dSumX2 * m_dSumX2 * m_dSumXY -
                   m_dSumX * m_dSumX4 * m_dSumY -
                  (double)m_nN * m_dSumX3 * m_dSumX2Y;
  return (D != 0.0) ? (double)(D1 / D) : 0.0;
}

double __fastcall Statistics::GetSquareC()
{
  long double D = (long double)m_nN * m_dSumX2 * m_dSumX4 +
                  2.0 * m_dSumX * m_dSumX3 * m_dSumX2 -
                  m_dSumX2 * m_dSumX2 * m_dSumX2 -
                  m_dSumX * m_dSumX * m_dSumX4 -
                  (long double)m_nN * m_dSumX3 * m_dSumX3;
  long double D0 = m_dSumX2 * m_dSumX4 * m_dSumY +
                   m_dSumX3 * m_dSumX2 * m_dSumXY +
                   m_dSumX * m_dSumX3 * m_dSumX2Y -
                   m_dSumX2 * m_dSumX2 * m_dSumX2Y -
                   m_dSumX * m_dSumX4 * m_dSumXY -
                   m_dSumX3 * m_dSumX3 * m_dSumY;
  return (D != 0.0) ? (double)(D0 / D) : 0.0;
}

double __fastcall Statistics::GetAverageX()
{
  return (double)(m_dSumX / (long double)m_nN);
}

double __fastcall Statistics::GetAverageY()
{
  return (double)(m_dSumY / (long double)m_nN);
}

double __fastcall Statistics::GetCovariation()
{
  return (double)(m_dSumVarXY / (long double)m_nN);
}

double __fastcall Statistics::GetSimpleDispersionX()
{
  long double avX = (long double)AverageX;
  return (double)(m_dSumX2 / (long double)m_nN - avX * avX);
}

double __fastcall Statistics::GetSimpleDispersionY()
{
  long double avY = (long double)AverageY;
  return (double)(m_dSumY2 / (long double)m_nN - avY * avY);
}

double __fastcall Statistics::GetLinearSSE()
{
  long double ldSSE = 0.0;
  long double A = LinearA, B = LinearB;

  for(int i = 0; i < m_nN; i++)
  {
    long double d = m_pY[i] - (A * m_pX[i] + B);
    ldSSE += (d * d);
  }

  return ldSSE;
}

double __fastcall Statistics::GetSquareSSE()
{
  long double ldSSE = 0.0;
  long double A = SquareA, B = SquareB, C = SquareC;

  for(int i = 0; i < m_nN; i++)
  {
    long double d = m_pY[i] - ((A * m_pX[i] + B) * m_pX[i] + C);
    ldSSE += (d * d);
  }

  return ldSSE;
}

double __fastcall Statistics::GetSST()
{
  long double ldSST = 0.0;
  long double lpAvY = AverageY;

  for(int i = 0; i < m_nN; i++)
  {
    long double d = m_pY[i] - lpAvY;
    ldSST += (d * d);
  }

  return ldSST;
}

double __fastcall Statistics::GetLinearSSR()
{
  long double ldSSR = 0.0;
  long double A = LinearA, B = LinearB;
  long double lpAvY = AverageY;

  for(int i = 0; i < m_nN; i++)
  {
    long double d = (A * m_pX[i] + B) - lpAvY;
    ldSSR += (d * d);
  }

  return ldSSR;
}

double __fastcall Statistics::GetSquareSSR()
{
  long double ldSSR = 0.0;
  long double A = SquareA, B = SquareB, C = SquareC;
  long double lpAvY = AverageY;

  for(int i = 0; i < m_nN; i++)
  {
    long double d = ((A * m_pX[i] + B) * m_pX[i] + C) - lpAvY;
    ldSSR += (d * d);
  }

  return ldSSR;
}

double __fastcall Statistics::GetLinearSME()
{
  long double ldSSE = LinearSSE;
  return ldSSE / ((long double)m_nN - 2.0);
}

double __fastcall Statistics::GetSquareSME()
{
  long double ldSSE = SquareSSE;
  return ldSSE / ((long double)m_nN - 3.0);
}
