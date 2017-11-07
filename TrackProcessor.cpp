//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include "Profiler.h"

#pragma link "wininet.lib"

//---------------------------------------------------------------------------
USEFORM("Profiles.cpp", Profile);
USEFORM("PointGridSettings.cpp", GridSettings);
USEFORM("Points.cpp", PointsForm);
USEFORM("SRTMAlt.cpp", STRMAltitude);
USEFORM("StaticNav.cpp", SNEForm);
USEFORM("TrackStat.cpp", TrackStatForm);
USEFORM("WAvgFilter.cpp", WAvgForm);
USEFORM("Settings.cpp", SettingsForm);
USEFORM("ShifTime.cpp", TimeShift);
USEFORM("SplineSmooth.cpp", SplineSmoothForm);
USEFORM("Split.cpp", SplitForm);
USEFORM("SpdFilter.cpp", SpeedFilter);
USEFORM("HDOPFilter.cpp", HDOPForm);
USEFORM("HP.cpp", HPForm);
USEFORM("GPSKalmanForm.cpp", GPSKalmanFilter);
USEFORM("MainFrame.cpp", MainForm);
USEFORM("MedianFilter.cpp", MedianForm);
USEFORM("ImgSizeForm.cpp", ImgSize);
USEFORM("LowPass.cpp", LowPassForm);
USEFORM("DES.cpp", DESForm);
USEFORM("CellEq.cpp", CellEqualizer);
USEFORM("Confidence.cpp", ConfidenceForm);
USEFORM("DriftFilter.cpp", DriftForm);
USEFORM("DPFilter.cpp", DPForm);
USEFORM("Metadata.cpp", EditMetadata);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
  try
  {
     #ifdef USE_PROFILER
       pProfiler = new TProfiler;
     #endif

	 if (FormatSettings.DecimalSeparator != '.') FormatSettings.DecimalSeparator = '.';

     AnsiString asCurrentPath = GetCurrentDir();
     SetCurrentDir(ExtractFilePath(Application->ExeName));

     Application->Initialize();
     Application->Title = "Track Processor";
     Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TMedianForm), &MedianForm);
		Application->CreateForm(__classid(TDPForm), &DPForm);
		Application->CreateForm(__classid(TSpeedFilter), &SpeedFilter);
		Application->CreateForm(__classid(TLowPassForm), &LowPassForm);
		Application->CreateForm(__classid(TDriftForm), &DriftForm);
		Application->CreateForm(__classid(THDOPForm), &HDOPForm);
		Application->CreateForm(__classid(TTimeShift), &TimeShift);
		Application->CreateForm(__classid(TProfile), &Profile);
		Application->CreateForm(__classid(TImgSize), &ImgSize);
		Application->CreateForm(__classid(TWAvgForm), &WAvgForm);
		Application->CreateForm(__classid(TPointsForm), &PointsForm);
		Application->CreateForm(__classid(TGridSettings), &GridSettings);
		Application->CreateForm(__classid(TSNEForm), &SNEForm);
		Application->CreateForm(__classid(TSTRMAltitude), &STRMAltitude);
		Application->CreateForm(__classid(TDESForm), &DESForm);
		Application->CreateForm(__classid(TSplitForm), &SplitForm);
		Application->CreateForm(__classid(TTrackStatForm), &TrackStatForm);
		Application->CreateForm(__classid(TEditMetadata), &EditMetadata);
		Application->CreateForm(__classid(TGPSKalmanFilter), &GPSKalmanFilter);
		Application->CreateForm(__classid(TSplineSmoothForm), &SplineSmoothForm);
		Application->CreateForm(__classid(THPForm), &HPForm);
		Application->CreateForm(__classid(TConfidenceForm), &ConfidenceForm);
		Application->CreateForm(__classid(TCellEqualizer), &CellEqualizer);
		Application->CreateForm(__classid(TSettingsForm), &SettingsForm);
		Application->CreateForm(__classid(TCellEqualizer), &CellEqualizer);
		Application->CreateForm(__classid(TConfidenceForm), &ConfidenceForm);
		Application->CreateForm(__classid(TDESForm), &DESForm);
		Application->CreateForm(__classid(TDPForm), &DPForm);
		Application->CreateForm(__classid(TDriftForm), &DriftForm);
		Application->CreateForm(__classid(TGPSKalmanFilter), &GPSKalmanFilter);
		Application->CreateForm(__classid(THDOPForm), &HDOPForm);
		Application->CreateForm(__classid(THPForm), &HPForm);
		Application->CreateForm(__classid(TImgSize), &ImgSize);
		Application->CreateForm(__classid(TLowPassForm), &LowPassForm);
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TMedianForm), &MedianForm);
		Application->CreateForm(__classid(TEditMetadata), &EditMetadata);
		Application->CreateForm(__classid(TGridSettings), &GridSettings);
		Application->CreateForm(__classid(TPointsForm), &PointsForm);
		Application->CreateForm(__classid(TProfile), &Profile);
		Application->CreateForm(__classid(TSettingsForm), &SettingsForm);
		Application->CreateForm(__classid(TTimeShift), &TimeShift);
		Application->CreateForm(__classid(TSpeedFilter), &SpeedFilter);
		Application->CreateForm(__classid(TSplineSmoothForm), &SplineSmoothForm);
		Application->CreateForm(__classid(TSplitForm), &SplitForm);
		Application->CreateForm(__classid(TSTRMAltitude), &STRMAltitude);
		Application->CreateForm(__classid(TSNEForm), &SNEForm);
		Application->CreateForm(__classid(TTrackStatForm), &TrackStatForm);
		Application->CreateForm(__classid(TWAvgForm), &WAvgForm);
		Application->Run();

     SetCurrentDir(asCurrentPath);

     #ifdef USE_PROFILER
       delete pProfiler;
     #endif
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  catch (...)
  {
     try
     {
       throw Exception("");
     }
     catch (Exception &exception)
     {
       Application->ShowException(&exception);
     }
  }
  return 0;
}
//---------------------------------------------------------------------------
