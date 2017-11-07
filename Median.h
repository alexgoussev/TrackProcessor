//---------------------------------------------------------------------------

#ifndef MedianH
#define MedianH
//---------------------------------------------------------------------------

class Median
{
  public:
    __fastcall Median();
    __fastcall ~Median();
    double __fastcall GetMedian(double * arr, int n);
    __property bool Average  = { read=GetAverage, write=SetAverage };
    __property double AverageFactor  = { read=GetAverageFactor, write=SetAverageFactor };
  protected:
    void __fastcall Sort(double * arr, int n);
  private:
    bool m_bAverage;
    double m_dAverageFactor;
    void __fastcall SetAverage(bool value);
    bool __fastcall GetAverage();
    void __fastcall SetAverageFactor(double value);
    double __fastcall GetAverageFactor();
};

#endif

