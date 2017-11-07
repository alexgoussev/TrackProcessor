//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Cells.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

double TCells::m_dDistances[CELL_SIZE][CELL_SIZE];
bool TCells::m_bDistCalc = false;

__fastcall TCells::TCells(int nWidth, int nHeight)
{
  m_nCellsX = nWidth / CELL_SIZE;
  m_nCellsY = nHeight / CELL_SIZE;

  int n = m_nCellsX * m_nCellsY;
  m_pCellsList = new PList[n];
  memset(m_pCellsList, 0, n * sizeof(PList)); 

  if(!m_bDistCalc)
  {
    for(int i = 0; i < CELL_SIZE; i++)
    {
      for(int j = 0; j < CELL_SIZE; j++)
        m_dDistances[i][j] = sqrt((double)(i * i + j * j));
    }

    m_bDistCalc = true;
  }
}

__fastcall TCells::~TCells()
{
  int n = m_nCellsX * m_nCellsY;

  for(int i = 0; i < n; i++)
  {
    PList pList = m_pCellsList[i];

    if(pList)
    {
      while(pList->Count)
      {
        PCellPoint pPoint = (PCellPoint)pList->Items[0];
        delete pPoint;
        pList->Delete(0);
      }

      delete pList;
    }
  }

  delete [] m_pCellsList;
}

PList __fastcall TCells::GetCell(int i, int j)
{
  PList pList = NULL;

  if(m_pCellsList && m_nCellsX && m_nCellsY)
  {
    int n = i + m_nCellsX * j;
    if(n >= 0 && n < (m_nCellsX * m_nCellsY)) pList = m_pCellsList[n];
  }

  return pList;
}

void __fastcall TCells::SetCell(int i, int j, PList pList)
{
  if(m_pCellsList && m_nCellsX && m_nCellsY)
  {
    int n = i + m_nCellsX * j;
    if(n >= 0 && n < (m_nCellsX * m_nCellsY) && m_pCellsList[n] == NULL) m_pCellsList[n] = pList;
  }
}

PList __fastcall TCells::GetCellXY(int x, int y)
{
  return Cell[x / CELL_SIZE][y / CELL_SIZE];
}

void __fastcall TCells::SetCellXY(int x, int y, PList pList)
{
  Cell[x / CELL_SIZE][y / CELL_SIZE] = pList;
}

void __fastcall TCells::SetPoint(int x, int y, int n)
{
  PCellPoint pPoint = new tagCellPoint;
  pPoint->nPoint = n;
  pPoint->nX = x;
  pPoint->nY = y;
  PList pList = CellXY[x][y];

  if(pList == NULL)
  {
    pList = new TList;
    CellXY[x][y] = pList;
  }

  pList->Add(pPoint);
}

int __fastcall TCells::GetNearestPoint(int x, int y)
{
  int n = -1;
  int nFromX = x / CELL_SIZE - 1, nFromY = y / CELL_SIZE - 1;
  int nToX = nFromX + 3, nToY = nFromY + 3;
  double dmindist = (double)CELL_SIZE;

  for(int i = nFromX; i < nToX; i++)
  {
    for(int j = nFromY; j < nToY; j++)
    {
      PList pList = Cell[i][j];

      if(pList)
      {
        for(int k = 0; k < pList->Count; k++)
        {
          PCellPoint pPoint = (PCellPoint)pList->Items[k];
          int dx = abs(x - pPoint->nX);

          if(dx < CELL_SIZE)
          {
            int dy = abs(y - pPoint->nY);

            if(dy < CELL_SIZE && dmindist > m_dDistances[dx][dy])
            {
              dmindist = m_dDistances[dx][dy];
              n = pPoint->nPoint;
            }
          }
        }
      }
    }
  }

  return n;
}
