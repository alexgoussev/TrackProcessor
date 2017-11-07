//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <mem.h>
#include <float.h>
#pragma hdrstop

#include "Spline.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall AkimaSpline::AkimaSpline(double * X, double * Y, int n)
{
  m_pXM = new long double[n + 3];
  m_pZ = new long double[n];
  m_pX = new long double[n];
  m_pY = new long double[n];
  m_nN = 1;

  m_pX[0] = (long double)X[0]; m_pY[0] = (long double)Y[0];

  for(int i = 1; i < n; i++)
  {
    if(fabsl((long double)X[i] - m_pX[m_nN - 1]) > LDBL_EPSILON)
    {
      m_pX[m_nN] = (long double)X[i];
      m_pY[m_nN] = (long double)Y[i];
      m_nN++;
    }
  }

  m_dMaxX = (long double)X[n - 3];
  m_dMinX = (long double)X[0];

  for(int i = 0; i < n - 1; i++)
  {
    long double dx = (long double)X[i + 1] - (long double)X[i];
    if(dx != 0.0) m_pXM[i + 2] = ((long double)Y[i + 1] - (long double)Y[i]) / dx;
    else m_pXM[i + 2] = 0.0;
  }

  m_pXM[n + 1] = 2.0 * m_pXM[n] - m_pXM[n - 1];
  m_pXM[n + 2] = 2.0 * m_pXM[n + 1] - m_pXM[n];
  m_pXM[1] = 2.0 * m_pXM[2] - m_pXM[3];
  m_pXM[0] = 2.0 * m_pXM[1] - m_pXM[2];

  for(int i=0; i < n; i++)
  {
    long double a=fabsl(m_pXM[i + 3] - m_pXM[i + 2]);
    long double b=fabsl(m_pXM[i + 1] - m_pXM[i]);

    if(a + b != 0.0) m_pZ[i] = (a * m_pXM[i + 1] + b * m_pXM[i + 2]) / (a + b);
    else m_pZ[i] = (m_pXM[i + 2] + m_pXM[i + 1]) / 2.0;
  }
}

__fastcall AkimaSpline::~AkimaSpline()
{
  delete [] m_pXM;
  delete [] m_pZ;
  delete [] m_pX;
  delete [] m_pY;
}

int __fastcall AkimaSpline::Interpolate(double X, double & Y)
{
  int res = 0;

  if(fabsl((long double)X - m_dMaxX) <= LDBL_EPSILON) Y = (double)m_pY[m_nN - 3];
  else if((long double)X >= m_dMinX && (long double)X < m_dMaxX)
  {
    res = 1;

    int i = 0;

    while((long double)X >= m_pX[i]) i++;
    i--;

    if(fabsl((long double)X - m_pX[i]) <= LDBL_EPSILON ||
       fabsl(m_pY[i] - m_pY[i+1]) <= LDBL_EPSILON)
      Y = (double)m_pY[i];
    else
    {
      //Begin interpolation
      long double b = m_pX[i + 1] - m_pX[i];
      long double a = (long double)X - m_pX[i];
      long double _y = m_pY[i] + m_pZ[i] * a + (3.0 * m_pXM[i+2] - 2.0 * m_pZ[i] - m_pZ[i + 1]) * a * a / b;
      Y = (double)(_y + (m_pZ[i] + m_pZ[i + 1] - 2.0 * m_pXM[i + 2]) * a * a * a / (b * b));
    }
  }

  return res;
}

//---------------------------------------------------------------------------

