//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <stdio.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "TrackStat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTrackStatForm *TrackStatForm;
//---------------------------------------------------------------------------
__fastcall TTrackStatForm::TTrackStatForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_dMinSpeed = pini->ReadFloat("Statistics", "MinSpeed", 0.1) / 3.6;
  m_dMinClimb = (double)pini->ReadInteger("Statistics", "MinClimb", 1);
  m_dMinAccend = pini->ReadFloat("Statistics", "MinAccend", 1.0);
  delete pini;

  //Time
  m_pLabels[0] = Label1; m_pLabels[1] = TimeTotal;
  m_pLabels[2] = Label2; m_pLabels[3] = TimeMovement;
  m_pLabels[4] = Label3; m_pLabels[5] = TimeStop;
  m_pLabels[6] = Label4; m_pLabels[7] = TimeUphill;
  m_pLabels[8] = Label5; m_pLabels[9] = TimeDownhill;
  m_pLabels[10] = Label6; m_pLabels[11] = TimeFlat;

  //Distance
  m_pLabels[12] = Label7; m_pLabels[13] = DistTotal;
  m_pLabels[14] = Label8; m_pLabels[15] = DistUphill;
  m_pLabels[16] = Label9; m_pLabels[17] = DistDownhill;
  m_pLabels[18] = Label10; m_pLabels[19] = DistFlat;

  //Altitude
  m_pLabels[20] = Label11; m_pLabels[21] = AltMax;
  m_pLabels[22] = Label12; m_pLabels[23] = AltMin;
  m_pLabels[24] = Label13; m_pLabels[25] = AltDiff;
  m_pLabels[26] = Label14; m_pLabels[27] = AltAccent;
  m_pLabels[28] = Label15; m_pLabels[29] = AltDescent;
  m_pLabels[30] = Label16; m_pLabels[31] = AltRelAcc;
  m_pLabels[32] = Label17; m_pLabels[33] = AltRelDesc;

  //Speed
  m_pLabels[34] = Label18; m_pLabels[35] = SpdMax;
  m_pLabels[36] = Label19; m_pLabels[37] = SpdAvg;
  m_pLabels[38] = Label20; m_pLabels[39] = SpdMovAvg;
  m_pLabels[40] = Label21; m_pLabels[41] = SpdAvgUp;
  m_pLabels[42] = Label22; m_pLabels[43] = SpdAvgDn;
  m_pLabels[44] = Label31; m_pLabels[45] = SpdAvgFlat;

  //Climb
  m_pLabels[46] = Label23; m_pLabels[47] = ClimbUphill;
  m_pLabels[48] = Label24; m_pLabels[49] = ClimbDownhill;

  //Indices
  m_pLabels[50] = Label25; m_pLabels[51] = IndexGPSies;
  m_pLabels[52] = Label26; m_pLabels[53] = IndexClimbByBike;
  m_pLabels[54] = Label27; m_pLabels[55] = IndexFIETS;
}
//---------------------------------------------------------------------------

void __fastcall TTrackStatForm::SaveClick(TObject *Sender)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  AnsiString asPath = pini->ReadString("Path", "StatTextPath", "");
  delete pini;

  SaveStatDialog->InitialDir = asPath;

  if(SaveStatDialog->Execute())
  {
    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    pini->WriteString("Path", "StatTextPath", ExtractFileDir(SaveStatDialog->FileName));
    delete pini;

    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

	FILE *pOut = fopen(AnsiString(SaveStatDialog->FileName).c_str(), "w+t");
    if(pOut)
    {
      fprintf(pOut, " Time \n");
      fprintf(pOut, "---------------------------------------------------\n");

      for(int i = 0; i < 12; i += 2)
        fprintf(pOut, "%-35s %15s\n", m_pLabels[i]->Caption.c_str(), m_pLabels[i + 1]->Caption.c_str());

      fprintf(pOut, "\n Distance \n");
      fprintf(pOut, "---------------------------------------------------\n");

      for(int i = 12; i < 20; i += 2)
        fprintf(pOut, "%-35s %15s\n", m_pLabels[i]->Caption.c_str(), m_pLabels[i + 1]->Caption.c_str());

      fprintf(pOut, "\n Altitude \n");
      fprintf(pOut, "---------------------------------------------------\n");

      for(int i = 20; i < 34; i += 2)
        fprintf(pOut, "%-35s %15s\n", m_pLabels[i]->Caption.c_str(), m_pLabels[i + 1]->Caption.c_str());

      fprintf(pOut, "\n Speed \n");
      fprintf(pOut, "---------------------------------------------------\n");

      for(int i = 34; i < 46; i += 2)
        fprintf(pOut, "%-35s %15s\n", m_pLabels[i]->Caption.c_str(), m_pLabels[i + 1]->Caption.c_str());

      fprintf(pOut, "\n Climb \n");
      fprintf(pOut, "---------------------------------------------------\n");

      for(int i = 46; i < 50; i += 2)
        fprintf(pOut, "%-35s %15s\n", m_pLabels[i]->Caption.c_str(), m_pLabels[i + 1]->Caption.c_str());

      fprintf(pOut, "\n Indices \n");
      fprintf(pOut, "---------------------------------------------------\n");

      for(int i = 50; i < 56; i += 2)
        fprintf(pOut, "%-35s %15s\n", m_pLabels[i]->Caption.c_str(), m_pLabels[i + 1]->Caption.c_str());

      fprintf(pOut, "\n Settings \n");
      fprintf(pOut, "---------------------------------------------------\n");
      fprintf(pOut, "%-35s %15s\n", Label28->Caption.c_str(), SetMinSpeed->Text.c_str());
      fprintf(pOut, "%-35s %15s\n", Label29->Caption.c_str(), SetMinClimb->Text.c_str());
      fprintf(pOut, "%-35s %15s\n", Label30->Caption.c_str(), SetMinAccend->Text.c_str());

      fclose(pOut);
    }

    Screen->Cursor = Save_Cursor;
  }
}
//---------------------------------------------------------------------------

void __fastcall TTrackStatForm::CancelClick(TObject *Sender)
{
//  
}
//---------------------------------------------------------------------------

void __fastcall TTrackStatForm::SetCurrentTrack(PGPXTrack value)
{
  m_pCurrentTrack = value;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  SetMinSpeed->Text = AnsiString(pini->ReadFloat("Statistics", "MinSpeed", 0.1));
  SetMinClimb->Text = AnsiString(pini->ReadInteger("Statistics", "MinClimb", 1));
  SetMinAccend->Text = AnsiString(pini->ReadInteger("Statistics", "MinAccend", 1));
  delete pini;
  
  ClearFields();
  if(m_pCurrentTrack != NULL) CalcStat();
}

PGPXTrack __fastcall TTrackStatForm::GetCurrentTrack()
{
  return m_pCurrentTrack;
}


void __fastcall TTrackStatForm::ClearFields()
{
  TimeTotal->Caption = AnsiString("");
  TimeMovement->Caption = AnsiString("");
  TimeStop->Caption = AnsiString("");
  TimeUphill->Caption = AnsiString("");
  TimeDownhill->Caption = AnsiString("");
  TimeFlat->Caption = AnsiString("");

  DistTotal->Caption = AnsiString("");
  DistUphill->Caption = AnsiString("");
  DistDownhill->Caption = AnsiString("");
  DistFlat->Caption = AnsiString("");

  ClimbUphill->Caption = AnsiString("");
  ClimbDownhill->Caption = AnsiString("");

  AltMax->Caption = AnsiString("");
  AltMin->Caption = AnsiString("");
  AltDiff->Caption = AnsiString("");
  AltAccent->Caption = AnsiString("");
  AltDescent->Caption = AnsiString("");
  AltRelAcc->Caption = AnsiString("");
  AltRelDesc->Caption = AnsiString("");

  SpdMax->Caption = AnsiString("");
  SpdAvg->Caption = AnsiString("");
  SpdMovAvg->Caption = AnsiString("");
  SpdAvgUp->Caption = AnsiString("");
  SpdAvgDn->Caption = AnsiString("");

  IndexClimbByBike->Caption = AnsiString("");
  IndexFIETS->Caption = AnsiString("");
  IndexGPSies->Caption = AnsiString("");

  SetMinSpeed->Text = AnsiString(m_dMinSpeed * 3.6);
  SetMinClimb->Text = AnsiString((int)m_dMinClimb);
  SetMinAccend->Text = AnsiString(m_dMinAccend);
}

void __fastcall TTrackStatForm::CalcStat()
{
  double dTimeTotal = 0.0, dTimeStop = 0.0, dTimeMovement = 0.0, dTimeUphill = 0.0, dTimeDownhill = 0.0, dTimeFlat = 0.0;
  double dDistTotal = 0.0, dDistUphill = 0.0, dDistDownhill = 0.0, dDistFlat = 0.0;
  double dMinAlt = 99999.0, dMaxAlt = -99999.0, dDiffAlt = 0.0;
  double dMaxUp = 0.0, dMaxDn = 0.0;
  double dTotalAcc = 0.0, dTotalDesc = 0.0, dRelAcc = 0.0, dRelDesc = 0.0;
  double dAvgSpeed = 0.0, dAvgMoveSpeed = 0.0, dAvgUpSpeed = 0.0, dAvgDnSpeed = 0.0, dAvgFlatSpeed = 0.0, dMaxSpeed = -1.0;

  eSegType eSegment;
  PGPXTrack pSegment;

  m_pStopSegs = new TList;
  m_pMovingUpSegs = new TList;
  m_pMovingDownSegs = new TList;

  //Делим трек на сегменты
  dMaxSpeed = m_pCurrentTrack->Points[0]->m_dSpeed;
  dMinAlt = m_pCurrentTrack->Points[0]->m_dAlt;
  dMaxAlt = m_pCurrentTrack->Points[0]->m_dAlt;

  for(int i = 1; i < m_pCurrentTrack->PointCount; i++)
  {
    if(dMaxSpeed < m_pCurrentTrack->Points[i]->m_dSpeed)
      dMaxSpeed = m_pCurrentTrack->Points[i]->m_dSpeed;

    if(dMaxAlt < m_pCurrentTrack->Points[i]->m_dAlt)
      dMaxAlt = m_pCurrentTrack->Points[i]->m_dAlt;

    if(dMinAlt > m_pCurrentTrack->Points[i]->m_dAlt)
      dMinAlt = m_pCurrentTrack->Points[i]->m_dAlt;

    eSegType eNewSegment = SegmentType(i);

    if(i == 1 || eNewSegment != eSegment)
    {
      eSegment = eNewSegment;
      pSegment = NewSegment(i, eSegment);
    }
    else if(pSegment) AddPointToSegment(pSegment, i);
  }

  SpdMax->Caption = SpeedString(dMaxSpeed);
  AltMax->Caption = AltString(dMaxAlt);
  AltMin->Caption = AltString(dMinAlt);
  dDiffAlt = dMaxAlt - dMinAlt;
  AltDiff->Caption = AltString(dDiffAlt);

  //Анализируем сегменты
  int i = 0;
  while(i < m_pMovingDownSegs->Count)
  {
    pSegment = (PGPXTrack)m_pMovingDownSegs->Items[i];

    if((pSegment->Points[0]->m_dAlt - pSegment->Points[pSegment->PointCount - 1]->m_dAlt) < m_dMinAccend)
    {
      m_pMovingDownSegs->Delete(i);
      delete pSegment;
    }
    else i++;
  }

  i = 0;
  while(i < m_pMovingUpSegs->Count)
  {
    pSegment = (PGPXTrack)m_pMovingUpSegs->Items[i];

    if((pSegment->Points[pSegment->PointCount - 1]->m_dAlt - pSegment->Points[0]->m_dAlt) < m_dMinAccend)
    {
      m_pMovingUpSegs->Delete(i);
      delete pSegment;
    }
    else i++;
  }

  //Считаем статистику
  dTimeTotal = m_pCurrentTrack->Points[m_pCurrentTrack->PointCount - 1]->m_dTime - m_pCurrentTrack->Points[0]->m_dTime;
  TimeTotal->Caption = TimeString(dTimeTotal);

  dDistTotal = m_pCurrentTrack->Points[m_pCurrentTrack->PointCount - 1]->m_dDistance;
  DistTotal->Caption = DistanceString(dDistTotal);

  if(dTimeTotal > 0.0)
  {
    dAvgSpeed = dDistTotal / dTimeTotal;
    SpdAvg->Caption = SpeedString(dAvgSpeed);
  }

  while(m_pStopSegs->Count)
  {
    pSegment = (PGPXTrack)m_pStopSegs->Items[0];
    dTimeStop += (pSegment->Points[pSegment->PointCount - 1]->m_dTime - pSegment->Points[0]->m_dTime);

    m_pStopSegs->Delete(0);
    delete pSegment;
  }

  TimeStop->Caption = TimeString(dTimeStop);
  dTimeMovement = dTimeTotal - dTimeStop;
  TimeMovement->Caption = TimeString(dTimeMovement);

  if(dTimeMovement > 0.0)
  {
    dAvgMoveSpeed = dDistTotal / dTimeMovement;
    SpdMovAvg->Caption = SpeedString(dAvgMoveSpeed);
  }

  while(m_pMovingDownSegs->Count)
  {
    pSegment = (PGPXTrack)m_pMovingDownSegs->Items[0];
    dTimeDownhill += (pSegment->Points[pSegment->PointCount - 1]->m_dTime - pSegment->Points[0]->m_dTime);
    dDistDownhill += pSegment->Points[pSegment->PointCount - 1]->m_dDistance;
    dTotalDesc += (pSegment->Points[0]->m_dAlt - pSegment->Points[pSegment->PointCount - 1]->m_dAlt);

    for(int j = 0; j < pSegment->PointCount; j++)
    {
      if(dMaxDn > pSegment->Points[j]->m_dClimb)
        dMaxDn = pSegment->Points[j]->m_dClimb;
    }

    m_pMovingDownSegs->Delete(0);
    delete pSegment;
  }

  TimeDownhill->Caption = TimeString(dTimeDownhill);
  DistDownhill->Caption = DistanceString(dDistDownhill);

  AltDescent->Caption = AltString(dTotalDesc);
  dRelDesc = dTotalDesc / (dDistTotal / 1000.0);
  AltRelDesc->Caption = AltString(dRelDesc);

  if(dTimeDownhill > 0.0)
  {
    dAvgDnSpeed = dDistDownhill / dTimeDownhill;
    SpdAvgDn->Caption = SpeedString(dAvgDnSpeed);
  }

  ClimbDownhill->Caption = ClimbString(dMaxDn);

  double dMaxAltUp = -99999.9;

  while(m_pMovingUpSegs->Count)
  {
    pSegment = (PGPXTrack)m_pMovingUpSegs->Items[0];

    dTimeUphill += (pSegment->Points[pSegment->PointCount - 1]->m_dTime - pSegment->Points[0]->m_dTime);
    dDistUphill += pSegment->Points[pSegment->PointCount - 1]->m_dDistance;
    dTotalAcc += (pSegment->Points[pSegment->PointCount - 1]->m_dAlt - pSegment->Points[0]->m_dAlt);

    if(dMaxAltUp < pSegment->Points[pSegment->PointCount - 1]->m_dAlt)
      dMaxAltUp = pSegment->Points[pSegment->PointCount - 1]->m_dAlt;

    for(int j = 0; j < pSegment->PointCount; j++)
    {
      if(dMaxUp < pSegment->Points[j]->m_dClimb)
        dMaxUp = pSegment->Points[j]->m_dClimb;
    }

    m_pMovingUpSegs->Delete(0);
    delete pSegment;
  }

  TimeUphill->Caption = TimeString(dTimeUphill);
  dTimeFlat = dTimeMovement - (dTimeDownhill + dTimeUphill);
  TimeFlat->Caption = TimeString(dTimeFlat);

  DistUphill->Caption = DistanceString(dDistUphill);
  dDistFlat = dDistTotal - (dDistDownhill + dDistUphill);
  DistFlat->Caption = DistanceString(dDistFlat);

  AltAccent->Caption = AltString(dTotalAcc);
  dRelAcc = dTotalAcc / (dDistTotal / 1000.0);
  AltRelAcc->Caption = AltString(dRelAcc);

  if(dTimeUphill > 0.0)
  {
    dAvgUpSpeed = dDistUphill / dTimeUphill;
    SpdAvgUp->Caption = SpeedString(dAvgUpSpeed);
  }

  if(dTimeFlat > 0.0)
  {
    dAvgFlatSpeed = dDistFlat / dTimeFlat;
    SpdAvgFlat->Caption = SpeedString(dAvgFlatSpeed);
  }

  ClimbUphill->Caption = ClimbString(dMaxUp);

  double dGPSIESIndex = GPSIESIndex(dTotalAcc, dDistUphill, dMaxAltUp);
  double dClimbByBikeIndex = ClimbByBikeIndex(dTotalAcc, dDistUphill, dMaxAltUp);
  double dFIETSIndex = FIETSIndex(dTotalAcc, dDistUphill, dMaxAltUp);

  IndexGPSies->Caption = IndexString(dGPSIESIndex);
  IndexClimbByBike->Caption = IndexString(dClimbByBikeIndex);
  IndexFIETS->Caption = IndexString(dFIETSIndex);

  delete m_pStopSegs;
  delete m_pMovingUpSegs;
  delete m_pMovingDownSegs;
}

void __fastcall TTrackStatForm::SetClick(TObject *Sender)
{
  m_dMinSpeed = SetMinSpeed->Text.ToDouble() / 3.6;
  m_dMinClimb = (double)SetMinClimb->Text.ToInt();
  m_dMinAccend = SetMinAccend->Text.ToDouble();

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  pini->WriteFloat("Statistics", "MinSpeed", m_dMinSpeed * 3.6);
  pini->WriteInteger("Statistics", "MinClimb", (int)m_dMinClimb);
  pini->WriteFloat("Statistics", "MinAccend", m_dMinAccend);
  delete pini;

  ClearFields();
  if(m_pCurrentTrack != NULL) CalcStat();
}
//---------------------------------------------------------------------------

void __fastcall TTrackStatForm::SetMinSpeedKeyPress(TObject *Sender,
      char &Key)
{
  if((Key >= '0' && Key <= '9') ||
     Key == VK_BACK             ||
     Key == VK_RETURN           ||
     Key == '.') {}
  else Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TTrackStatForm::SetMinClimbKeyPress(TObject *Sender,
      char &Key)
{
  if((Key >= '0' && Key <= '9') ||
     Key == VK_BACK             ||
     Key == VK_RETURN) {}
  else Key = 0;
}
//---------------------------------------------------------------------------


AnsiString __fastcall TTrackStatForm::TimeString(double dTime)
{
  static char str[128];
  DWORD dwTime = (DWORD)dTime;
  int nmSec = (int)((dTime - (double)dwTime) * 1000.0);

  int nHours = dwTime / 3600;
  dwTime %= 3600;
  int nMinutes = dwTime / 60;
  int nSeconds = dwTime % 60;

  sprintf(str, "%d:%02d:%02d.%03d", nHours, nMinutes, nSeconds, nmSec);

  return AnsiString(str);
}

AnsiString __fastcall TTrackStatForm::DistanceString(double dDist)
{
  char str[64];
  dDist = dDist / 10.0 + 0.5;
  dDist = ((double)((int)dDist)) / 100.0;
  sprintf(str, "%.2lf", dDist);
  return AnsiString(str);
}

AnsiString __fastcall TTrackStatForm::ClimbString(double dClimb)
{
  char str[64];

  double climb = ((double)((int)((fabs(dClimb) * 10.0) + 0.5))) / 10.0;
  if(dClimb < 0.0) climb = -climb;
  sprintf(str, "%.1lf", climb);

  return AnsiString(str);
}

AnsiString __fastcall TTrackStatForm::AltString(double dAlt)
{
  char str[64];

  dAlt = (dAlt > 0.0) ? ((double)((int)((dAlt * 10.0) + 0.5))) / 10.0 :
                        ((double)((int)((dAlt * 10.0) - 0.5))) / 10.0;

  sprintf(str, "%.1lf", dAlt);

  return AnsiString(str);
}

AnsiString __fastcall TTrackStatForm::SpeedString(double dSpeed)
{
  char str[64];

  dSpeed = ((double)((int)((dSpeed * 36.0) + 0.5))) / 10.0;
  sprintf(str, "%.1lf", dSpeed);

  return AnsiString(str);
}

AnsiString __fastcall TTrackStatForm::IndexString(double dIndex)
{
  char str[64];

  dIndex = ((double)((int)((dIndex * 100.0) + 0.5))) / 100.0;
  sprintf(str, "%.2lf", dIndex);

  return AnsiString(str);
}

double __fastcall TTrackStatForm::GPSIESIndex(double dAccent, double dDistance, double dMaxAlt)
{
  if (0 == dDistance)
    return 0;
  return dAccent * (0.1 * dAccent + 40.0) / dDistance + (0.1 * dDistance + dMaxAlt) / 1000.0;
}

double __fastcall TTrackStatForm::ClimbByBikeIndex(double dAccent, double dDistance, double dMaxAlt)
{
  if (0 == dDistance)
	return 0;
  double dIndex = (dAccent * 100.0 / dDistance) * 2.0 + dAccent * dAccent / dDistance + dDistance / 1000.0;
  if(dMaxAlt > 1000.0) dIndex += (dMaxAlt - 1000.0) / 100.0;

  return dIndex;
}

double __fastcall TTrackStatForm::FIETSIndex(double dAccent, double dDistance, double dMaxAlt)
{
  if (0 == dDistance)
    return 0;
  double dIndex = (dAccent * dAccent / dDistance) * 10.0;
  if(dMaxAlt > 1000.0) dIndex += (dMaxAlt - 1000.0) / 1000.0;

  return dIndex;
}

PGPXTrack __fastcall TTrackStatForm::NewSegment(int i, eSegType eSegment)
{
  PGPXTrack pSegment = NULL;

  if(eSegment != stFlat)
  {
    pSegment = new GPXTrack;
    AddPointToSegment(pSegment, i - 1);
    AddPointToSegment(pSegment, i);

    switch(eSegment)
    {
      case stStop:
        m_pStopSegs->Add(pSegment);
        break;

      case stDown:
        m_pMovingDownSegs->Add(pSegment);
        break;

      case stUp:
        m_pMovingUpSegs->Add(pSegment);
        break;
    }
  }

  return pSegment;
}

eSegType __fastcall TTrackStatForm::SegmentType(int i)
{
  eSegType eSegment;
  PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];

  if(pPoint->m_dSpeed < m_dMinSpeed) eSegment = stStop;
  else
  {
    if(m_pCurrentTrack->m_bNoAlt) eSegment = stFlat;
    else if(fabs(pPoint->m_dClimb) < m_dMinClimb) eSegment = stFlat;
    else if(pPoint->m_dClimb > 0.0) eSegment = stUp;
    else eSegment = stDown;
  }

  return eSegment;
}

void __fastcall TTrackStatForm::AddPointToSegment(PGPXTrack pSegment, int i)
{
  PGPXTrackPoint pSegPoint = pSegment->NewPoint;
  pSegPoint->SetPoint(m_pCurrentTrack->Points[i]);
  pSegment->AddPoint(pSegPoint);
}




