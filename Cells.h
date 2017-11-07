//---------------------------------------------------------------------------

#ifndef CellsH
#define CellsH
//---------------------------------------------------------------------------

#define CELL_SIZE   16

struct tagCellPoint {
  int nPoint;
  int nX;
  int nY;
};

typedef tagCellPoint* PCellPoint;
typedef TList* PList;

class TCells {
public:
  __fastcall TCells(int nWidth, int nHeight);
  __fastcall ~TCells();
  __property PList Cell[int i][int j] = { read=GetCell, write=SetCell };
  __property PList CellXY[int x][int y] = { read=GetCellXY, write=SetCellXY };
  __property int Point[int x][int y] = { write=SetPoint };
  __property int NearestPoint[int x][int y] = { read=GetNearestPoint };
protected:
  int m_nCellsX;
  int m_nCellsY;
  PList * m_pCellsList;
  static double m_dDistances[CELL_SIZE][CELL_SIZE];
  static bool m_bDistCalc;
private:
  PList __fastcall GetCell(int i, int j);
  void __fastcall SetCell(int i, int j, PList pList);
  PList __fastcall GetCellXY(int x, int y);
  void __fastcall SetCellXY(int x, int y, PList pList);
  void __fastcall SetPoint(int x, int y, int n);
  int __fastcall GetNearestPoint(int x, int y);
};

#endif
