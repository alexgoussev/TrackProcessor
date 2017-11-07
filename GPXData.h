//---------------------------------------------------------------------------

#ifndef GPXDataH
#define GPXDataH
//---------------------------------------------------------------------------

#define GPS_PT_NO_VALUE  (double)(-999999.0)

typedef class GPXTrack GPXTrack;
typedef class GPXTrack* PGPXTrack;
typedef class GPXTrackPoint GPXTrackPoint;
typedef class GPXTrackPoint* PGPXTrackPoint;

enum GEOCOORD {eLat, eLon};
enum GEOCOORDFMT {eD_D, eDMS};

class GPXTrackPoint
{
  public:
    double m_dLat;
    double m_dLon;
    double m_dTime;
    double m_dCourse;
    double m_dSpeed;
    double m_dDistance;
    double m_dSpacing;
    double m_dAlt;
    double m_dHDOP;
    double m_dClimb;
    bool m_bActive;
    AnsiString m_asDesc;
    double m_dInterval;
    double m_dElapsed;
    double m_dPDOP;
    double m_dVDOP;
    int m_nSats;
    double m_dMagvar;
    int m_nFix;
    double m_dGeoidHeigtht;
    __fastcall GPXTrackPoint(PGPXTrack pTrack);
    __fastcall ~GPXTrackPoint();
    void __fastcall SetdProj(double x, double y);
    void __fastcall GetdProj(double *px, double *py);
    void __fastcall SetPoint(PGPXTrackPoint pPoint);
    __property AnsiString TimeString  = { read=GetTimeString };
    __property AnsiString GPXTimeString  = { read=GetGPXTimeString };
    __property AnsiString GPXEleString  = { read=GetGPXEleString };
    __property AnsiString GPXLatString  = { read=GetGPXLatString };
    __property AnsiString GPXLonString  = { read=GetGPXLonString };
    __property AnsiString GPXHDOPString  = { read=GetGPXHDOPString };
    __property AnsiString GPXPDOPString  = { read=GetGPXPDOPString };
    __property AnsiString GPXVDOPString  = { read=GetGPXVDOPString };
    __property AnsiString GPXSpeedString  = { read=GetGPXSpeedString };
    __property AnsiString GPXCourseString  = { read=GetGPXCourseString };
    __property AnsiString GPXDescString  = { read=GetGPXDescString };
    __property AnsiString GPXClimbString  = { read=GetGPXClimbString };
  __property AnsiString LatDMSString  = { read=GetLatDMSString };
  __property AnsiString LonDMSString  = { read=GetLonDMSString };
  __property AnsiString IntervalString  = { read=GetIntervalString };
  __property AnsiString SpacingString  = { read=GetSpacingString };
  __property AnsiString DistanceString  = { read=GetDistanceString };
  __property AnsiString ElapsedString  = { read=GetElapsedString };
  __property AnsiString LocalTimeString  = { read=GetLocalTimeString };
  __property AnsiString LatD_DString  = { read=GetLatD_DString };
  __property AnsiString LonD_DString  = { read=GetLonD_DString };
  __property AnsiString GPXSatNumString  = { read=GetGPXSatNumString };
  __property AnsiString GPXMagvarString  = { read=GetGPXMagvarString };
  __property AnsiString GPXFixString  = { read=GetGPXFixString };
  __property AnsiString GPXGeoidHeightString  = { read=GetGPXGeoidHeightString };
  protected:
    PGPXTrack m_pTrack;
    private:
    AnsiString __fastcall GetTimeString();
    AnsiString __fastcall GetGPXTimeString();
    AnsiString __fastcall GetGPXEleString();
    AnsiString __fastcall GetGPXLatString();
    AnsiString __fastcall GetGPXLonString();
    AnsiString __fastcall GetGPXHDOPString();
    AnsiString __fastcall GetGPXPDOPString();
    AnsiString __fastcall GetGPXVDOPString();
    AnsiString __fastcall GetGPXSpeedString();
    AnsiString __fastcall GetGPXCourseString();
    AnsiString __fastcall GetGPXDescString();
    AnsiString __fastcall GetGPXClimbString();
  AnsiString __fastcall GetLatDMSString();
  AnsiString __fastcall GetLonDMSString();
  AnsiString __fastcall GetIntervalString();
  AnsiString __fastcall GetSpacingString();
  AnsiString __fastcall GetDistanceString();
  AnsiString __fastcall GetElapsedString();
  AnsiString __fastcall GetLocalTimeString();
  AnsiString __fastcall GetLatD_DString();
  AnsiString __fastcall GetLonD_DString();
  AnsiString __fastcall GetGPXSatNumString();
  AnsiString __fastcall GetGPXMagvarString();
  AnsiString __fastcall GetGPXFixString();
  AnsiString __fastcall GetGPXGeoidHeightString();
};

class GPXTrack
{
  public:
    double m_dMinLat;
    double m_dMinLon;
    double m_dMaxLat;
    double m_dMaxLon;
    double m_dSpeedAveragingTime;
    double m_dMinClimbDAlt;
    double m_dMinClimbDist;
    double m_dMinClimbSpeed;
    AnsiString m_asName;
    AnsiString m_asDesc;
    AnsiString m_asSrc;
    AnsiString m_asCmt;
    bool m_bCourseCalculated;
    bool m_bSpeedCalculated;
    bool m_bNoTime;
    bool m_bNoAlt;
    bool m_bNoHDOP;
    bool m_bNoPDOP;
    bool m_bNoVDOP;
    AnsiString m_asFileName;
    __fastcall GPXTrack();
    __fastcall ~GPXTrack();
    void __fastcall AddPoint(PGPXTrackPoint pPoint);
    //void __fastcall Recalc();
    void __fastcall DeletePoint(int i);
    double __fastcall GetDistance(double dLatA, double dLonA, double dLatB, double dLonB);
    double __fastcall GetCource(double dLatA, double dLonA, double dLatB, double dLonB);
    double __fastcall GetDistance(int p1, int p2);
    double __fastcall GetCource(int p1, int p2);
    void __fastcall SetProj(double x, double y, double * plat, double * plon);
    void __fastcall GetProj(double *x, double *y, double lat, double lon);
    AnsiString __fastcall FormatCoordString(double dCoord, GEOCOORD eType, GEOCOORDFMT eFmt);
    AnsiString __fastcall FormatAltString(double dAlt);
    AnsiString __fastcall FormatSpeedString(double dSpeed);
    AnsiString __fastcall FormatCourseString(double dCourse);
    AnsiString __fastcall FormatDistanceString(double dDist);
    AnsiString __fastcall FormatClimbString(double dClimb);
    __fastcall GPXTrack(PGPXTrack pTrack);
    void __fastcall RecalcDist();
    void __fastcall RecalcDistClimb();
    void __fastcall SetPointsActive(bool bActive);
    void __fastcall RemoveInactivePoints();
  AnsiString __fastcall FormatElapsedSring(DWORD Elapsed);
    __property PGPXTrackPoint NewPoint  = { read=GetNewPoint };
    __property PGPXTrackPoint Points[int i] = { read=GetPoint };
    __property int PointCount  = { read=GetPointCount };
  protected:
    TList * m_pPoints;
  private:
    PGPXTrackPoint __fastcall GetNewPoint();
    PGPXTrackPoint __fastcall GetPoint(int i);
    int __fastcall GetPointCount();
};

#endif

