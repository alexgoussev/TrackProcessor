//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "Confidence.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"

TConfidenceForm *ConfidenceForm;
//---------------------------------------------------------------------------
__fastcall TConfidenceForm::TConfidenceForm(TComponent* Owner)
  : TForm(Owner)
{
  m_pBaseTrack = NULL;
  m_pTrendTrack = NULL;
  m_pResultTrack = NULL;
  m_pDeviations = NULL;
  m_pSplineX = NULL;
  m_pSplineY = NULL;
  m_pSplineD = NULL;
  m_pBitmap = new Graphics::TBitmap();
  m_bExhanged = false;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_pReplace->Checked = pini->ReadBool("Confidence", "Replace", false);
  delete pini;
}

__fastcall TConfidenceForm::~TConfidenceForm()
{
  delete m_pBitmap;
  if(m_pDeviations) delete [] m_pDeviations;
  if(m_pSplineX) delete m_pSplineX;
  if(m_pSplineY) delete m_pSplineY;
  if(m_pSplineD) delete m_pSplineD;
}

//---------------------------------------------------------------------------
void __fastcall TConfidenceForm::ExchangeClick(TObject *Sender)
{
  SetTracks(m_pTrendTrack, m_pBaseTrack);
  m_bExhanged = !m_bExhanged;
}

//---------------------------------------------------------------------------
void __fastcall TConfidenceForm::UpperThereshouldKeyPress(
      TObject *Sender, char &Key)
{
  if((Key < '0' || Key > '9') && Key != '.' && Key != '\b' && Key != '\r')
    Key = '\0';

  if(Key == '\r')
  {
    m_dUpTh = m_pUpperThereshould->Text.ToDouble();

    if(m_dUpTh > m_dMaxDev)
      m_dUpTh = m_dMaxDev;
    else if(m_dUpTh < 0.0)
      m_dUpTh = 0.0;

    m_pUpperThereshould->Text = RoundStr(m_dUpTh);// = AnsiString(upth);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall TConfidenceForm::UpperThDownClick(TObject *Sender)
{
  m_dUpTh = m_pUpperThereshould->Text.ToDouble();

  if(m_dUpTh > 0.0)
  {
    m_dUpTh -= 0.1;
    m_pUpperThereshould->Text = RoundStr(m_dUpTh);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall TConfidenceForm::UpperThUpClick(TObject *Sender)
{
  m_dUpTh = m_pUpperThereshould->Text.ToDouble();

  if(m_dUpTh < m_dMaxDev)
  {
    m_dUpTh += 0.1;
    m_pUpperThereshould->Text = RoundStr(m_dUpTh);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall TConfidenceForm::UpperThereshouldExit(TObject *Sender)
{
  m_dUpTh = m_pUpperThereshould->Text.ToDouble();

  if(m_dUpTh > m_dMaxDev)
    m_dUpTh = m_dMaxDev;
  else if(m_dUpTh < 0.0)
    m_dUpTh = 0.0;

  m_pUpperThereshould->Text = RoundStr(m_dUpTh);

  SetLimits();
}
//---------------------------------------------------------------------------

void __fastcall TConfidenceForm::S2Click(TObject *Sender)
{
  m_dUpTh = m_dAvgDev + 2.0 * m_dSigma;

  if(m_dUpTh > m_dMaxDev) m_dUpTh = m_dMaxDev;

  m_pUpperThereshould->Text = RoundStr(m_dUpTh);
  SetLimits();
}
//---------------------------------------------------------------------------

void __fastcall TConfidenceForm::S3Click(TObject *Sender)
{
  m_dUpTh = m_dAvgDev + 3.0 * m_dSigma;

  if(m_dUpTh > m_dMaxDev) m_dUpTh = m_dMaxDev;

  m_pUpperThereshould->Text = RoundStr(m_dUpTh);
  SetLimits();
}
//---------------------------------------------------------------------------

void __fastcall TConfidenceForm::ConfidenceClick(TObject *Sender)
{
  TCursor Save_Cursor = Screen->Cursor;
  Screen->Cursor = crHourGlass;

  int nPoints = (int)((double)(m_nPoints * (100 - m_pConfIntVal->Value)) / 100.0);
  double *pArray = new double[m_nPoints];

  for(int i = 0; i < m_nPoints; i++) pArray[i] = m_pDeviations[i];
  for(int i = 0; i < nPoints; i++) HighestFirst(&pArray[i], m_nPoints - i);

  m_dUpTh = pArray[nPoints - 1];
  m_pUpperThereshould->Text = RoundStr(m_dUpTh);
  SetLimits();

  delete [] pArray;

  Screen->Cursor = Save_Cursor;
}
//---------------------------------------------------------------------------

void __fastcall TConfidenceForm::GoClick(TObject *Sender)
{
  m_pProgress->Max = m_nPoints;
  m_pProgress->Position = 0;

  TCursor Save_Cursor = Screen->Cursor;
  Screen->Cursor = crHourGlass;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  pini->WriteBool("Confidence", "Replace", m_pReplace->Checked);
  delete pini;

  m_pResultTrack = new GPXTrack;

  for(int i = 0; i < m_nPoints; i++)
  {
    int j = m_nStartPoint + i;
    PGPXTrackPoint pPoint = m_pBaseTrack->Points[j];

    if(m_pDeviations[i] <= m_dUpTh)
    {
      PGPXTrackPoint pNewPoint = m_pResultTrack->NewPoint;
      pNewPoint->SetPoint(pPoint);
      m_pResultTrack->AddPoint(pNewPoint);
    }
    else if(m_pReplace->Checked)
    {
      PGPXTrackPoint pNewPoint = m_pResultTrack->NewPoint;
      pNewPoint->SetPoint(pPoint);

      double dXt, dYt;

      m_pSplineX->Interpolate(pPoint->m_dTime, dXt);
      m_pSplineY->Interpolate(pPoint->m_dTime, dYt);
      pNewPoint->SetdProj(dXt, dYt);

      m_pResultTrack->AddPoint(pNewPoint);
    }

    m_pProgress->Position = i;
    Application->ProcessMessages();
  }

  m_pProgress->Position = 0;
  Screen->Cursor = Save_Cursor;
}
//---------------------------------------------------------------------------

PGPXTrack __fastcall TConfidenceForm::GetResultTrack()
{
  return m_pResultTrack;
}

void __fastcall TConfidenceForm::SetTracks(PGPXTrack pBase, PGPXTrack pTrend)
{
  if(pBase && pTrend)
  {
    m_bExhanged = false;
    m_pBaseTrack = pBase;
    m_pTrendTrack = pTrend;
    m_pBase->Text = pBase->m_asName;
    m_pTrend->Text = pTrend->m_asName;

    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    CalcStat();
    DrawDistribution();

    Screen->Cursor = Save_Cursor;
  }
}

void __fastcall TConfidenceForm::CalcStat()
{
  //перва€ точка
  m_nStartPoint = 0;
  while(m_pBaseTrack->Points[m_nStartPoint]->m_dTime < m_pTrendTrack->Points[0]->m_dTime &&
        m_nStartPoint < m_pBaseTrack->PointCount && m_nStartPoint < m_pTrendTrack->PointCount)
    m_nStartPoint++;

  //последн€€ точка
  m_nEndPoint = m_pBaseTrack->PointCount - 1;
  while(m_pBaseTrack->Points[m_nEndPoint]->m_dTime > m_pTrendTrack->Points[m_pTrendTrack->PointCount - 3]->m_dTime &&
        m_nEndPoint >= 0)
    m_nEndPoint--;

  m_nPoints = m_nEndPoint - m_nStartPoint + 1;
  m_dAvgDev = m_dMaxDev = m_dSigma = 0.0;

  if(m_nPoints)
  {
    if(m_pDeviations) delete [] m_pDeviations;
    if(m_pSplineX) delete m_pSplineX;
    if(m_pSplineY) delete m_pSplineY;

    m_pDeviations = new double[m_nPoints];

    double *pT = new double[m_pTrendTrack->PointCount];
    double *pX = new double[m_pTrendTrack->PointCount];
    double *pY = new double[m_pTrendTrack->PointCount];

    for(int i = 0; i < m_pTrendTrack->PointCount; i++)
    {
      pT[i] = m_pTrendTrack->Points[i]->m_dTime;
      m_pTrendTrack->Points[i]->GetdProj(&pX[i], &pY[i]);
    }

    m_pSplineX = new AkimaSpline(pT, pX, m_pTrendTrack->PointCount);
    m_pSplineY = new AkimaSpline(pT, pY, m_pTrendTrack->PointCount);

    for(int i = m_nStartPoint; i <= m_nEndPoint; i++)
    {
      double dXb, dYb, dXt, dYt;

      m_pBaseTrack->Points[i]->GetdProj(&dXb, &dYb);
      m_pSplineX->Interpolate(m_pBaseTrack->Points[i]->m_dTime, dXt);
      m_pSplineY->Interpolate(m_pBaseTrack->Points[i]->m_dTime, dYt);

      double dX = (dXb - dXt), dY = (dYb - dYt);
      double dR = sqrt(dX * dX + dY * dY);

      m_pDeviations[i - m_nStartPoint] = dR;
      if(dR > m_dMaxDev) m_dMaxDev = dR;
      m_dAvgDev += dR;
    }

    m_dAvgDev /= (double)m_nPoints;

    for(int i = 0; i < m_nPoints; i++)
    {
      double dD = m_pDeviations[i] - m_dAvgDev;
      m_dSigma += dD * dD;
    }

    m_dSigma = sqrt(m_dSigma / (double)m_nPoints);
  }

  m_dUpTh = m_dMaxDev;
  m_pAvg->Text = RoundStr(m_dAvgDev);
  m_pMax->Text = RoundStr(m_dMaxDev);
  m_pS->Text = RoundStr(m_dSigma);
  m_pUpperThereshould->Text = RoundStr(m_dUpTh);
}

AnsiString __fastcall TConfidenceForm::RoundStr(double dValue)
{
  double dRound = ((double)((int)((dValue * 10.0) + 0.5))) / 10.0;
  return FormatFloat("0.0", dRound);
}


void __fastcall TConfidenceForm::DrawDistribution()
{
  int *pCounts = new int[INTERVALS];
  double *dPersentage = new double[INTERVALS];
  double *dX = new double[INTERVALS + 2];
  double *dY = new double[INTERVALS + 2];

  if(m_pSplineD) delete m_pSplineD;

  double nMaxPersentage = 0.0;
  double dInterval = m_dMaxDev / (double)(INTERVALS - 1);

  for(int i = 0; i < INTERVALS; i++) pCounts[i] = 0;

  for(int i = 0; i < m_nPoints; i++)
  {
    int nInterval = (int)(m_pDeviations[i] / dInterval);
    pCounts[nInterval]++;
  }

  for(int i = 0; i < INTERVALS; i++)
  {
    dPersentage[i] = (double)pCounts[i] * 100.0 / (double)m_nPoints;
    if(nMaxPersentage < dPersentage[i]) nMaxPersentage = dPersentage[i];
  }

  double dBarWidth = (double)m_pResDistr->Width / (double)INTERVALS;
  double dPersUnit = (double)(m_pResDistr->Height - BOTTOM_SPACE) / (nMaxPersentage * 1.1);

  dX[0] = 0.0; dX[INTERVALS + 1] = (double)m_pResDistr->Width;
  dY[0] = dY[INTERVALS + 1] = 0.0;

  for(int i = 0; i < INTERVALS; i++)
  {
    dX[i + 1] = ((double)i + 0.5) * dBarWidth;
    dY[i + 1] = dPersentage[i] * dPersUnit;
  }

  m_pSplineD = new AkimaSpline(dX, dY, INTERVALS + 2);

  m_pBitmap->Width = m_pResDistr->Width;
  m_pBitmap->Height = m_pResDistr->Height;

  TCanvas *pCanvas = m_pBitmap->Canvas;

  int nBottom = m_pResDistr->Height - BOTTOM_SPACE;

  pCanvas->Pen->Color = clWhite;
  pCanvas->Pen->Style = psSolid;
  pCanvas->Brush->Style = bsSolid;
  pCanvas->Brush->Color = clWhite;
  pCanvas->Rectangle(0, 0, m_pResDistr->Width, m_pResDistr->Height);
  pCanvas->Pen->Color = clBlack;
  pCanvas->MoveTo(0, nBottom + 1);
  pCanvas->LineTo(m_pResDistr->Width, nBottom + 1);
  pCanvas->Pen->Color = clRed;

  for(int i = 0; i < m_pResDistr->Width; i++)
  {
    double X = (double)i;
    double Y;

    m_pSplineD->Interpolate(X, Y);
    if(Y < 0.0) Y = 0.0;

    pCanvas->MoveTo((int)X, nBottom);
    pCanvas->LineTo((int)X, nBottom - (int)Y);
  }

  AnsiString as = AnsiString("0.0");

  pCanvas->Font->Size = 10;
  pCanvas->Font->Color = clBlack;
  pCanvas->TextOut(1, nBottom + 3, as);

  as = FormatFloat("0.0", m_dMaxDev);
  pCanvas->TextOut(m_pResDistr->Width - pCanvas->TextWidth(as) - 1, nBottom + 3, as);

  as = FormatFloat("0.0", m_dMaxDev / 2.0);
  pCanvas->TextOut((m_pResDistr->Width - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

  as = FormatFloat("0.0", m_dMaxDev / 4.0);
  pCanvas->TextOut((m_pResDistr->Width / 2 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

  as = FormatFloat("0.0", m_dMaxDev * 3.0 / 4.0);
  pCanvas->TextOut((m_pResDistr->Width * 3 / 2 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

  as = FormatFloat("0.0", m_dMaxDev / 8.0);
  pCanvas->TextOut((m_pResDistr->Width / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

  as = FormatFloat("0.0", m_dMaxDev * 3.0 / 8.0);
  pCanvas->TextOut((m_pResDistr->Width * 3 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

  as = FormatFloat("0.0", m_dMaxDev * 5.0 / 8.0);
  pCanvas->TextOut((m_pResDistr->Width * 5 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

  as = FormatFloat("0.0", m_dMaxDev * 7.0 / 8.0);
  pCanvas->TextOut((m_pResDistr->Width * 7 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

  pCanvas = m_pResDistr->Canvas;
  pCanvas->Draw(0, 0, m_pBitmap);

  delete [] dX;
  delete [] dY;
  delete [] dPersentage;
  delete [] pCounts;
}

void __fastcall TConfidenceForm::SetLimits()
{
  TCanvas *pCanvas = m_pResDistr->Canvas;
  pCanvas->Draw(0, 0, m_pBitmap);

  int nBottom = m_pResDistr->Height - BOTTOM_SPACE;
  double Unit = (double)m_pResDistr->Width / m_dMaxDev;

  int rBorder = (m_dUpTh < m_dMaxDev) ? (int)(m_dUpTh * Unit) : 0;

  pCanvas->Pen->Color = clBlack;
  pCanvas->Pen->Style = psSolid;

  if(rBorder)
  {
    pCanvas->MoveTo(rBorder, nBottom);
    pCanvas->LineTo(rBorder, 0);

    for(int i = rBorder + 1; i < m_pResDistr->Width; i++)
    {
      double X = (double)i;
      double Y;

      m_pSplineD->Interpolate(X, Y);
      if(Y < 0.0) Y = 0.0;

      pCanvas->MoveTo((int)X, nBottom);
      pCanvas->LineTo((int)X, nBottom - (int)Y);
    }
  }
}

void __fastcall TConfidenceForm::HighestFirst(double *pArray, int n)
{
  for(int i = 1; i < n; i++)
  {
    if(pArray[i] > pArray[0])
    {
      double tmp = pArray[0];
      pArray[0] = pArray[i];
      pArray[i] = tmp;
    }
  }
}

bool __fastcall TConfidenceForm::GetExchanged()
{
  return m_bExhanged;
}
