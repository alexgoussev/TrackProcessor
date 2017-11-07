//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Median.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall Median::Median()
{
  m_bAverage = false;
  m_dAverageFactor = 0.65;
}

__fastcall Median::~Median()
{
  //TODO: Add your source code here
}

void __fastcall Median::Sort(double * arr, int n)
{
  int i, j;
  double tmp;

  for(i = 1; i < n; ++i)                         // цикл проходов, i - номер прохода
  {
    tmp = arr[i];

    for (j = i - 1; j >= 0 && arr[j] > tmp; --j) // поиск места элемента в готовой последовательности
      arr[j + 1] = arr[j];                       // сдвигаем элемент направо, пока не дошли

    arr[j + 1] = tmp;                            // место найдено, вставить элемент
  }
}


void __fastcall Median::SetAverage(bool value)
{
  if(m_bAverage != value) m_bAverage = value;
}

bool __fastcall Median::GetAverage()
{
  return m_bAverage;
}

void __fastcall Median::SetAverageFactor(double value)
{
  if(m_dAverageFactor != value) m_dAverageFactor = value;
}

double __fastcall Median::GetAverageFactor()
{
  return m_dAverageFactor;
}

double __fastcall Median::GetMedian(double * arr, int n)
{
  double *sort_arr = new double[n];
  double dMedian;
  int nMedIndex = n / 2;
  bool isEven = ((n % 2) == 0);

  for(int i = 0; i < n; i++) sort_arr[i] = arr[i];
  Sort(sort_arr, n);

  if(m_bAverage)
    dMedian = m_dAverageFactor * sort_arr[nMedIndex] +
              (1.0 - m_dAverageFactor) * (sort_arr[nMedIndex - 1] + sort_arr[nMedIndex + 1]) / 2.0;
  else
  {
    if(isEven) dMedian = (sort_arr[nMedIndex - 1] + sort_arr[nMedIndex]) / 2.0; 
    else dMedian = sort_arr[nMedIndex];
  }

  delete [] sort_arr;

  return dMedian;
}