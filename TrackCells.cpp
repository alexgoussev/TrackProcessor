//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "TrackCells.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TTrackCells::TTrackCells(PGPXTrack pTrack, double dCellSize)
{
  m_pTrack = pTrack;
  m_dCellSize = dCellSize;

  m_pTrack->GetProj(&m_dXmin, &m_dYmin, m_pTrack->m_dMinLat, m_pTrack->m_dMinLon);
  m_pTrack->GetProj(&m_dXmax, &m_dYmax, m_pTrack->m_dMaxLat, m_pTrack->m_dMaxLon);

  m_nCellsX = m_nCellsY = 0;
  m_pCellsList = NULL;

  if(m_dXmin < m_dXmax && m_dYmin < m_dYmax)
  {
    m_dXmin -= m_dCellSize / 2.0; m_dXmax += m_dCellSize / 2.0;
    m_dYmin -= m_dCellSize / 2.0; m_dYmax += m_dCellSize / 2.0;

    m_nCellsX = (int)((m_dXmax - m_dXmin) / m_dCellSize) + 1;
    m_nCellsY = (int)((m_dYmax - m_dYmin) / m_dCellSize) + 1;

    int n = m_nCellsX * m_nCellsY;
    m_pCellsList = new PList[n];
    memset(m_pCellsList, 0, n * sizeof(PList));

    for(int i = 0; i < m_pTrack->PointCount; i++) AddPoint(i);
  }
}

__fastcall TTrackCells::~TTrackCells()
{
  int n = m_nCellsX * m_nCellsY;

  for(int i = 0; i < n; i++)
  {
    PList pList = m_pCellsList[i];
    if(pList) delete pList;
  }

  delete [] m_pCellsList;
}

PList __fastcall TTrackCells::GetCell(int i, int j)
{
  PList pList = NULL;

  if(m_pCellsList && m_nCellsX && m_nCellsY)
  {
    int n = i + m_nCellsX * j;
    if(n >= 0 && n < (m_nCellsX * m_nCellsY)) pList = m_pCellsList[n];
  }

  return pList;
}

void __fastcall TTrackCells::SetCell(int i, int j, PList pList)
{
  if(m_pCellsList && m_nCellsX && m_nCellsY)
  {
    int n = i + m_nCellsX * j;
    if(n >= 0 && n < (m_nCellsX * m_nCellsY) && m_pCellsList[n] == NULL) m_pCellsList[n] = pList;
  }
}

PList __fastcall TTrackCells::GetCellXY(double x, double y)
{
  int i = (int)((x - m_dXmin) / m_dCellSize), j = (int)((y - m_dYmin) / m_dCellSize);
  return Cell[i][j];
}

void __fastcall TTrackCells::SetCellXY(double x, double y, PList pList)
{
  int i = (int)((x - m_dXmin) / m_dCellSize), j = (int)((y - m_dYmin) / m_dCellSize);
  Cell[i][j] = pList;
}

void __fastcall TTrackCells::AddPoint(int n)
{
  double x, y;
  PGPXTrackPoint pPoint = m_pTrack->Points[n];

  pPoint->GetdProj(&x, &y);
  PList pList = CellXY[x][y];

  if(!pList)
  {
    pList = new TList;
    CellXY[x][y] = pList;
  }

  pList->Add((void*)n);
}

int __fastcall TTrackCells::GetPointCount(int i, int j)
{
  int nPoints = 0;

  PList pList = Cell[i][j];
  if(pList) nPoints = pList->Count;

  return nPoints;
}

PGPXTrackPoint __fastcall TTrackCells::GetPoint(int i, int j, int k)
{
  PGPXTrackPoint pPoint = NULL;
  int nPointIndex = PointIndex[i][j][k];

  if(nPointIndex > -1)
    pPoint = m_pTrack->Points[nPointIndex];

  return pPoint;
}

int __fastcall TTrackCells::GetPointIndex(int i, int j, int k)
{
  int nPointIndex = -1;

  PList pList = Cell[i][j];
  if(pList && k > -1 && k < pList->Count)
    nPointIndex = (int)pList->Items[k];

  return nPointIndex;
}

PGPXTrack __fastcall TTrackCells::GetTrack()
{
  return m_pTrack;
}

int __fastcall TTrackCells::GetCellsX()
{
  return m_nCellsX;
}

int __fastcall TTrackCells::GetCellsY()
{
  return m_nCellsY;
}
