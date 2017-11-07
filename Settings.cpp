//---------------------------------------------------------------------------

#include <vcl.h>
#include <Vcl.FileCtrl.hpp>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "Settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingsForm *SettingsForm;
//---------------------------------------------------------------------------
__fastcall TSettingsForm::TSettingsForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_asHGTPath = pini->ReadString("Path", "HGTPath", "");

  if(m_asHGTPath.IsEmpty())
  {
    m_asHGTPath = ExcludeTrailingPathDelimiter(ExtractFileDir(Application->ExeName)) + AnsiString("\\hgt\\");
    if(!DirectoryExists(m_asHGTPath)) CreateDir(m_asHGTPath);

    pini->WriteString("Path", "HGTPath", m_asHGTPath);
  }

  m_asMapCache = pini->ReadString("Path", "MapCachePath", "");

  if(m_asMapCache.IsEmpty())
  {
    m_asMapCache = ExcludeTrailingPathDelimiter(ExtractFileDir(Application->ExeName)) + AnsiString("\\cache\\");
    if(!DirectoryExists(m_asMapCache)) CreateDir(m_asMapCache);

    pini->WriteString("Path", "MapCachePath", m_asMapCache);
  }

  m_nFileAge = pini->ReadInteger("Map", "TileAge", 30);
  m_dAvgTime = pini->ReadFloat("GPXFile", "AvgTime", 3.0);
  m_dMinClimbDAlt = pini->ReadFloat("GPXFile", "MinClimbAltDiff", 1.0);
  m_dMinClimbDist = pini->ReadFloat("GPXFile", "MinClimbDist", 5.0);
  m_dMinClimbSpeed = pini->ReadFloat("GPXFile", "MinClimbSpeed", 1.0);
  m_dHDOPFactor = pini->ReadFloat("ColumbusCSV", "HDOPFactor", 10.0);
  m_dAVGAlpha = pini->ReadFloat("ColumbusCSV", "ComplementaryFactor", 0.9);
  m_bRestoreSpeed = pini->ReadBool("ColumbusCSV", "RestoreSpeed", true);

  delete pini;

  HGTPath->Text = m_asHGTPath;
  CachePath->Text = m_asMapCache;
  FileAgeUpDown->Position = m_nFileAge;
  AvgTimeUpDown->Position = (int)m_dAvgTime;
  ClimbAltDiffUpDown->Position = (int)m_dMinClimbDAlt;
  ClimbDistUpDown->Position = (int)m_dMinClimbDist;
  ClimbSpeedUpDown->Position = (int)m_dMinClimbSpeed;
  HDOPFactprUpDown->Position = (int)m_dHDOPFactor;
  ComplimentaryFactor->Text = AnsiString(m_dAVGAlpha);
  ResoreSpeed->Checked = m_bRestoreSpeed;
}

//---------------------------------------------------------------------------
void __fastcall TSettingsForm::ComplimentaryFactorKeyPress(TObject *Sender,
      char &Key)
{
  if((Key >= '0' && Key <= '9') ||
     Key == VK_BACK             ||
     Key == VK_RETURN           ||
     Key == '.') {}
  else Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::HGTPathButtonClick(TObject *Sender)
{
  String Directory = String(m_asHGTPath);
  if(SelectDirectory(Directory, TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt, -1))
	HGTPath->Text = m_asHGTPath;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::CachePathButtonClick(TObject *Sender)
{
  String Directory = String(m_asMapCache);
  if(SelectDirectory(Directory, TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt, -1))
    CachePath->Text = m_asMapCache;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::SaveActionExecute(TObject *Sender)
{
  m_asHGTPath = HGTPath->Text;
  m_asMapCache = CachePath->Text;
  m_nFileAge = FileAgeUpDown->Position;
  m_dAvgTime = (double)AvgTimeUpDown->Position;
  m_dMinClimbDAlt = (double)ClimbAltDiffUpDown->Position;
  m_dMinClimbDist = (double)ClimbDistUpDown->Position;
  m_dMinClimbSpeed = (double)ClimbSpeedUpDown->Position;
  m_dHDOPFactor = (double)HDOPFactprUpDown->Position;
  m_dAVGAlpha = ComplimentaryFactor->Text.ToDouble();
  m_bRestoreSpeed = ResoreSpeed->Checked;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  pini->WriteString("Path", "HGTPath", m_asHGTPath);
  pini->WriteString("Path", "MapCachePath", m_asMapCache);
  pini->WriteInteger("Map", "TileAge", m_nFileAge);
  pini->WriteFloat("GPXFile", "AvgTime", m_dAvgTime);
  pini->WriteFloat("GPXFile", "MinClimbAltDiff", m_dMinClimbDAlt);
  pini->WriteFloat("GPXFile", "MinClimbDist", m_dMinClimbDist);
  pini->WriteFloat("GPXFile", "MinClimbSpeed", m_dMinClimbSpeed);
  pini->WriteFloat("ColumbusCSV", "HDOPFactor", m_dHDOPFactor);
  pini->WriteFloat("ColumbusCSV", "ComplementaryFactor", m_dAVGAlpha);
  pini->WriteBool("ColumbusCSV", "RestoreSpeed", m_bRestoreSpeed);
  delete pini;

  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::SaveActionUpdate(TObject *Sender)
{
  m_dAVGAlpha = ComplimentaryFactor->Text.ToDouble();
  m_asHGTPath = HGTPath->Text;
  m_asMapCache = CachePath->Text;

  if(m_dAVGAlpha >= 0.0 && m_dAVGAlpha <= 1.0 && !m_asHGTPath.IsEmpty() && !m_asMapCache.IsEmpty())
    SaveAction->Enabled = true;
  else
    SaveAction->Enabled = false;
}
//---------------------------------------------------------------------------

