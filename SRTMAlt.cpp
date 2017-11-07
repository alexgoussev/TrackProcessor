//---------------------------------------------------------------------------

#include <vcl.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "SRTMAlt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma resource "srtm.res"

TSTRMAltitude *STRMAltitude;

//---------------------------------------------------------------------------
__fastcall TSTRMAltitude::TSTRMAltitude(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_asHGTPath = pini->ReadString("Path", "HGTPath", "");
  m_asSRTMBaseURL = pini->ReadString("SRTM", "SRTMBaseURL", "http://dds.cr.usgs.gov/srtm/version2_1/SRTM3");
  Download->Checked = pini->ReadBool("SRTM", "AutoDownload", true);
  m_dBeta = pini->ReadFloat("SRTM", "AltSmoothFactor", 0.3);
  delete pini;

  AltSmoothFactor->Text = AnsiString(m_dBeta);

  if(m_asHGTPath.IsEmpty())
  {
    m_asHGTPath = ExcludeTrailingPathDelimiter(ExtractFileDir(Application->ExeName)) + AnsiString("\\hgt\\");
    if(access(m_asHGTPath.c_str(), 0)) mkdir(m_asHGTPath.c_str());
  }
}
//---------------------------------------------------------------------------


//void __fastcall TSTRMAltitude::ZipProgress(TObject *Sender, String FileName, const TZipHeader &Header, __int64 Position)
//{
//  ProgressBar->Position = ProgressBar->Max * Position / Header.UncompressedSize;
//}
//---------------------------------------------------------------------------

void __fastcall TSTRMAltitude::HTTPGetSRTMWorkBegin(TObject *ASender, TWorkMode AWorkMode,
		  __int64 AWorkCountMax)
{
  ProgressBar->Max = (AWorkCountMax) ? AWorkCountMax : 100;
  ProgressBar->Position = 0;
}

//---------------------------------------------------------------------------

void __fastcall TSTRMAltitude::HTTPGetSRTMWork(TObject *ASender, TWorkMode AWorkMode,
		  __int64 AWorkCount)
{
  ProgressBar->Position = AWorkCount % ProgressBar->Max;
}

//---------------------------------------------------------------------------

void __fastcall TSTRMAltitude::m_OkClick(TObject *Sender)
{
  if(m_pTrack && m_pTrack->PointCount)
  {
	m_dBeta = AltSmoothFactor->Text.ToDouble();

	TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	pini->WriteBool("SRTM", "AutoDownload", Download->Checked);
	pini->WriteFloat("SRTM", "AltSmoothFactor", m_dBeta);
	delete pini;

	bool bIncomplete = false, bAltSmoothInit = false, bDEMAltSet;
	double dSAltprev, dS2Altprev;

	TCursor Save_Cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;

	m_Ok->Enabled = false;
	m_Cancel->Enabled = false;
	Download->Enabled = false;

	ProgressBar->Max = m_pTrack->PointCount;
	ProgressBar->Position = 0;
	Status->Caption = AnsiString("Processing points...");
	Status->Update();
	Application->ProcessMessages();

	for(m_nCurPoint = 0; m_nCurPoint < m_pTrack->PointCount; m_nCurPoint++)
	{
	  ProgressBar->Position = m_nCurPoint;
	  bDEMAltSet = false;

	  PGPXTrackPoint pPoint = m_pTrack->Points[m_nCurPoint];
	  char chLat = 'N', chLon = 'E';
	  int nLat = (int)pPoint->m_dLat, nLon = (int)pPoint->m_dLon;
	  double dLat = pPoint->m_dLat, dLon = pPoint->m_dLon;

	  if(nLat < 0) {chLat = 'S'; nLat = -nLat; dLat = -dLat;}
	  if(nLon < 0) {chLon = 'W'; nLon = -nLon; dLon = -dLon;}

      char sFileName[512];
      sprintf(sFileName, "%s%c%02d%c%03d.hgt", m_asHGTPath.c_str(), chLat, nLat, chLon, nLon);

      if(access(sFileName, 0))
      {
        if(Download->Checked)
        {
          if(DownloadFile(ChangeFileExt(ExtractFileName(AnsiString(sFileName)), "")))
          {
            double dDEMAlt = CalcAlt(AnsiString(sFileName), dLat, dLon);
            if(dDEMAlt != GPS_PT_NO_VALUE)
            {
              pPoint->m_dAlt = dDEMAlt;
              bDEMAltSet = true;
            }
          }
          else bIncomplete = true;

          Status->Caption = AnsiString("Processing points...");
          Status->Update();
          ProgressBar->Max = m_pTrack->PointCount;
          ProgressBar->Position = m_nCurPoint;
          Application->ProcessMessages();
        }
        else
        {
          bIncomplete = true;
          continue;
        }
      }
      else
      {
        double dDEMAlt = CalcAlt(AnsiString(sFileName), dLat, dLon);
        if(dDEMAlt != GPS_PT_NO_VALUE)
        {
          pPoint->m_dAlt = dDEMAlt;
          bDEMAltSet = true;
        }
      }

      if(bDEMAltSet)
      {
        if(!bAltSmoothInit)
        {
          dSAltprev = dS2Altprev = pPoint->m_dAlt;
          bAltSmoothInit = true;
        }
        else
        {
          double dSAlt = m_dBeta * pPoint->m_dAlt + (1.0 - m_dBeta) * dSAltprev;
          double dS2Alt = m_dBeta * dSAlt + (1.0 - m_dBeta) * dS2Altprev;
          pPoint->m_dAlt = 2.0 * dSAlt - dS2Alt;
          dSAltprev = dSAlt;
          dS2Altprev = dS2Alt;
        }
      }
    } 

    m_pTrack->RecalcDistClimb();
    
    Status->Caption = AnsiString("");
    Status->Update();
    m_Ok->Enabled = true;
    m_Cancel->Enabled = true;
    Download->Enabled = true;

    Screen->Cursor = Save_Cursor;
    ModalResult = (bIncomplete) ? mrRetry	: mrOk;
  }
  else ModalResult = mrAbort;
}
//---------------------------------------------------------------------------


double __fastcall TSTRMAltitude::CalcAlt(AnsiString asFileName, double dLat, double dLon)
{
  double dDEMAlt = GPS_PT_NO_VALUE;
  int h = open(asFileName.c_str(), O_RDONLY | O_BINARY, S_IREAD);

  if(h > -1)
  {
    //—читаем опорные точки дл€ аппроксимации
    dLat = Degrees2Seconds(dLat);
    dLon = Degrees2Seconds(dLon);

    int i0 = (int)(dLat / 3.0);
    int j0 = (int)(dLon / 3.0);

    /* по 4-м точкам */
    double df[2][2];
    int i, j, k, l;

    for(i = i0, k = 0; i < i0 + 2; i++, k++)
    {
      for(j= j0, l = 0; j < j0 + 2; j++, l++)
      {
        df[k][l] = ReadDEMFile(h, i, j);

        if(df[k][l] == 32768.0)
        {
          df[k][l] = 0;

          double d[4] = {32768.0, 32768.0, 32768.0, 32768.0};
          int n = 0;

          if(i > 0) d[0] = ReadDEMFile(h, i - 1, j);
          if(j > 0) d[1] = ReadDEMFile(h, i, j - 1);
          if(i < 1201) d[2] = ReadDEMFile(h, i + 1, j);
          if(j < 1201) d[3] = ReadDEMFile(h, i, j + 1);

          for(int m = 0; m < 4; m++)
          {
            if(d[m] != 32768.0)
            {
              df[k][l] += d[m];
              n++;
            }
          }

          df[k][l] /= (double)n;
        }
      }
    }

    close(h);

    /* по 4-м точкам */
    double dp = (dLat - (double)(i0 * 3)) / 3.0;
    double dq = (dLon - (double)(j0 * 3)) / 3.0;
    double dp_ = 1.0 - dp;
    double dq_ = 1.0 - dq;

    dDEMAlt = dp_ * dq_ * df[0][0] + dp * dq_ * df[1][0] + dp_ * dq * df[0][1] + dp * dq * df[1][1];
  }

  return dDEMAlt;
}

bool __fastcall TSTRMAltitude::DownloadFile(AnsiString asFileName)
{
  bool bResult = false;
  TResourceStream *pSRTMData = new TResourceStream((int)HInstance, AnsiString("SRTM_FILE_TABLE"), RT_RCDATA);
  int nDirs, nItems;

  pSRTMData->Read(&nDirs, sizeof(int));
  pSRTMData->Read(&nItems, sizeof(int));

  StringMap HGTFiles;
  AnsiString *pSRTMDirs = new AnsiString[nDirs];

  for(int i = 0; i < nDirs; i++)
  {
    DIRITEM sDirItem;
    pSRTMData->Read(&sDirItem, sizeof(DIRITEM));
    pSRTMDirs[sDirItem.nDirNo] = AnsiString("/") + AnsiString(sDirItem.szDir) + AnsiString("/");
  }

  for(int i = 0; i < nItems; i++)
  {
    HGTDATA sHGTItem;
    pSRTMData->Read(&sHGTItem, sizeof(HGTDATA));
    HGTFiles[sHGTItem.szName] = sHGTItem.nIndex;
  }

  delete pSRTMData;

  AnsiString asDlFileName = asFileName + AnsiString(".hgt.zip");
  AnsiString asZipFileName = m_asHGTPath + asFileName + AnsiString(".zip");
  AnsiString asURL = m_asSRTMBaseURL + pSRTMDirs[HGTFiles[asFileName.UpperCase().c_str()]] + asDlFileName;

  HGTFiles.clear();
  delete [] pSRTMDirs;

  Status->Caption = AnsiString("File download...");
  Status->Update();
  Application->ProcessMessages();

  TMemoryStream *pStream = new TMemoryStream;
  IdHTTP->Get(UnicodeString(asURL), pStream);
  pStream->SaveToFile(UnicodeString(asZipFileName));
  delete pStream;

  if(!access(asZipFileName.c_str(), 0))
  {
    Status->Caption = AnsiString("Unzip file...");
    Status->Update();
    Application->ProcessMessages();
    ProgressBar->Max = 100;
	ProgressBar->Position = 0;
    TZipFile *ZipFile;
	//ZipFile->ExtractZipFile(asZipFileName, m_asHGTPath, ZipProgress);
	ZipFile->ExtractZipFile(asZipFileName, m_asHGTPath);
    _unlink(asZipFileName.c_str());

    if(!access(AnsiString(ChangeFileExt(asZipFileName, ".hgt")).c_str(), 0)) bResult = true;
  }

  return bResult;
}

double __fastcall TSTRMAltitude::Degrees2Seconds(double dDegrees)
{
  int nDegrees = (int)dDegrees;
  dDegrees -= (double)nDegrees;
  dDegrees *= 60.0;
  int nMinutes = (int)dDegrees;
  dDegrees -= (double)nMinutes;
  double dSeconds = dDegrees * 60.0;
  dSeconds += (double)nMinutes * 60.0;

  return dSeconds;
}

double __fastcall TSTRMAltitude::ReadDEMFile(int handle, int i, int j)
{
  int nFOffset = GetHGTFileOffset(i, j);
  unsigned char val[2];

  lseek(handle, nFOffset, SEEK_SET);
  read(handle, val, 2);

  return (double)(val[0]) * 256.0 + (double)(val[1]);
}

int __fastcall TSTRMAltitude::GetHGTFileOffset(int nLatI, int nLonJ)
{
  int nRowPos = nLonJ * 2;
  int nRowNum = 1200 - nLatI;

  return nRowNum * 2402 + nRowPos;
}

//---------------------------------------------------------------------------

