#include <math.h>
#include <stdio.h>
#include "smoothedspline.h"

extern "C" int is01d_c(double *, double *, double *, int *, double *,
                       double *, double *, double *, int *);
#define coeff_ref(a_1,a_2) m_pCoeff[(a_2)*3 + a_1]


SmoothedSpline::SmoothedSpline()
{
  m_nCount = 0;
  m_nError = 0;
  m_dSm = 0.0;
  m_pWeights = NULL;
  m_pX = NULL;
  m_pOriginalY = NULL;
  m_pSmoothedY = NULL;
  m_pCoeff = NULL;
  m_pTemp = NULL;
}

SmoothedSpline::~SmoothedSpline()
{
  Clear();
}

void SmoothedSpline::Clear()
{
  m_nCount = 0;
  m_nError = 0;
  m_dSm = 0.0;

  if(m_pWeights) delete[] m_pWeights;
  if(m_pX) delete[] m_pX;
  if(m_pOriginalY) delete[] m_pOriginalY;
  if(m_pSmoothedY) delete[] m_pSmoothedY;
  if(m_pCoeff) delete[] m_pCoeff;
  if(m_pTemp) delete[] m_pTemp;
}

void SmoothedSpline::Create(int count, double *x, double *y, double *w)
{
  Clear();

  m_nCount = count;
  m_pWeights = new double[m_nCount];
  m_pX = new double[m_nCount];
  m_pOriginalY = new double[m_nCount];
  m_pSmoothedY = new double[m_nCount];
  m_pCoeff = new double[3* (m_nCount - 1)];
  m_pTemp = new double[7* (m_nCount + 2)];

  for(int i = 0; i < m_nCount; i++)
  {
    m_pWeights[i] = w[i];
    m_pX[i] = x[i];
    m_pOriginalY[i] = y[i];
  }

  m_dSm = m_nCount - 1.0 - sqrt((double)(m_nCount * 2));
  is01d_c(m_pX, m_pOriginalY, m_pWeights, &m_nCount, &m_dSm, m_pSmoothedY, m_pCoeff, m_pTemp, &m_nError);

  /*
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < m_nCount - 1; j++)
      printf("%10.6lf ", coeff_ref(i, j));

    printf("\n");
  }
  */
}

double SmoothedSpline::Calculate(double x)
{
  double y = 0.0;

  if(m_pX && m_pOriginalY && m_pCoeff)
  {
    int n = -1;

    for(int i = 0; i < m_nCount - 1; i++)
    {
      if(x >= m_pX[i] && x <= m_pX[i + 1])
      {
        n = i;
        break;
      }
    }

    if(n > -1)
    {
      double h = x - m_pX[n];
      double temp = 0.0;

      for(int i = 2; i >= 0; i--)
      {
        temp += coeff_ref(i, n);
        temp *= h;
      }

      y = temp + m_pOriginalY[n];
      m_nError = 0;
    }
    else
      m_nError = 1;
  }
  else
    m_nError = 2;

  return y;
}




