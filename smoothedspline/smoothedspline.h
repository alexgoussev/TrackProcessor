#ifndef __SMOOTHED_SPLINE_H__

class SmoothedSpline
{
  public:
    SmoothedSpline();
    ~SmoothedSpline();

    void Create(int count, double *x, double *y, double *w);
    double Calculate(double x);
    double *SmoothedY() {return m_pSmoothedY;}
    int Error() {return m_nError;}

  protected:
    int m_nCount;
    int m_nError;
    double m_dSm;
    double *m_pWeights;
    double *m_pX;
    double *m_pOriginalY;
    double *m_pSmoothedY;
    double *m_pCoeff;
    double *m_pTemp;

    void Clear();
};

#endif
