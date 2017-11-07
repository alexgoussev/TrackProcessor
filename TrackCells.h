//---------------------------------------------------------------------------

#ifndef TrackCellsH
#define TrackCellsH

#include "GPXData.h"
//---------------------------------------------------------------------------

typedef TList* PList;

class TTrackCells {

public:
  __fastcall TTrackCells(PGPXTrack pTrack, double dCellSize);
  __fastcall ~TTrackCells();
  __property PList Cell[int i][int j] = { read=GetCell, write=SetCell };
  __property PList CellXY[double x][double y] = { read=GetCellXY, write=SetCellXY };
  __property int PointCount[int i][int j] = { read=GetPointCount };
  __property PGPXTrackPoint Point[int i][int j][int k] = { read=GetPoint };
  __property int PointIndex[int i][int j][int k] = { read=GetPointIndex };
  __property PGPXTrack Track  = { read=GetTrack };
  __property int CellsX  = { read=GetCellsX };
  __property int CellsY  = { read=GetCellsY };
protected:
  PGPXTrack m_pTrack;
  double m_dCellSize;
  double m_dXmin;
  double m_dYmin;
  double m_dXmax;
  double m_dYmax;
  int m_nCellsX;
  int m_nCellsY;
  PList * m_pCellsList;
  void __fastcall AddPoint(int n);
private:
  PList __fastcall GetCell(int i, int j);
  void __fastcall SetCell(int i, int j, PList pList);
  PList __fastcall GetCellXY(double x, double y);
  void __fastcall SetCellXY(double x, double y, PList pList);
  int __fastcall GetPointCount(int i, int j);
  PGPXTrackPoint __fastcall GetPoint(int i, int j, int k);
  int __fastcall GetPointIndex(int i, int j, int k);
  PGPXTrack __fastcall GetTrack();
  int __fastcall GetCellsX();
  int __fastcall GetCellsY();
};

#endif
