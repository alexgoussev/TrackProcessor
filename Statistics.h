//---------------------------------------------------------------------------

#ifndef StatisticsH
#define StatisticsH
//---------------------------------------------------------------------------

class Statistics
{

public:
  __fastcall Statistics(double * X, double * Y, int n);
  __fastcall ~Statistics();
  __property double Correlation  = { read=GetCorrelation };
  __property double LinearR2  = { read=GetLinearR2 };
  __property double SquareR2  = { read=GetSquareR2 };
  __property double LinearA  = { read=GetLinearA };
  __property double LinearB  = { read=GetLinearB };
  __property double SquareA  = { read=GetSquareA };
  __property double SquareB  = { read=GetSquareB };
  __property double SquareC  = { read=GetSquareC };
  __property double AverageX  = { read=GetAverageX };
  __property double AverageY  = { read=GetAverageY };
  __property double Covariation  = { read=GetCovariation };
  __property double SimpleDispersionX  = { read=GetSimpleDispersionX };
  __property double SimpleDispersionY  = { read=GetSimpleDispersionY };
  __property double LinearSSE  = { read=GetLinearSSE };
  __property double SquareSSE  = { read=GetSquareSSE };
  __property double SST  = { read=GetSST };
  __property double LinearSSR  = { read=GetLinearSSR };
  __property double SquareSSR  = { read=GetSquareSSR };
  __property double LinearSME  = { read=GetLinearSME };
  __property double SquareSME  = { read=GetSquareSME };
private:
  double __fastcall GetCorrelation();
  double __fastcall GetLinearR2();
  double __fastcall GetSquareR2();
  double __fastcall GetLinearA();
  double __fastcall GetLinearB();
  double __fastcall GetSquareA();
  double __fastcall GetSquareB();
  double __fastcall GetSquareC();
  double __fastcall GetAverageX();
  double __fastcall GetAverageY();
  double __fastcall GetCovariation();
  double __fastcall GetSimpleDispersionX();
  double __fastcall GetSimpleDispersionY();
  double __fastcall GetLinearSSE();
  double __fastcall GetSquareSSE();
  double __fastcall GetSST();
  double __fastcall GetLinearSSR();
  double __fastcall GetSquareSSR();
  double __fastcall GetLinearSME();
  double __fastcall GetSquareSME();
protected:
  int m_nN;
  long double m_dSumX;
  long double m_dSumY;
  long double m_dSumX2;
  long double m_dSumX3;
  long double m_dSumX4;
  long double m_dSumY2;
  long double m_dSum2X;
  long double m_dSumXY;
  long double m_dSumX2Y;
  long double m_dSumVarX;
  long double m_dSumVarY;
  long double m_dSumVarXY;
  long double m_dSumVarX2;
  long double m_dSumVarY2;
  long double *m_pX;
  long double *m_pY;
};

#endif
