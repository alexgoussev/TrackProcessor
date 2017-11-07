//---------------------------------------------------------------------------

#ifndef SplineH
#define SplineH

class AkimaSpline {
  public:
    __fastcall AkimaSpline(double * X, double * Y, int n);
    __fastcall ~AkimaSpline();
    int __fastcall Interpolate(double X, double & Y);
  protected:
    long double * m_pXM;
    long double * m_pZ;
    int m_nN;
    long double m_dMaxX;
    long double m_dMinX;
    long double * m_pX;
    long double * m_pY;
};
//---------------------------------------------------------------------------
#endif


