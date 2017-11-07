//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "HDOPFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
THDOPForm *HDOPForm;
//---------------------------------------------------------------------------
__fastcall THDOPForm::THDOPForm(TComponent* Owner)
  : TForm(Owner)
{
  m_pCurrentTrack = NULL;
  m_pBitmap = new Graphics::TBitmap();
  m_pSpline = NULL;
  m_dMaxHDOP = 0.0;
}

__fastcall THDOPForm::~THDOPForm()
{
  delete m_pBitmap;
  if(m_pSpline) delete m_pSpline;
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::UpperSpinDownClick(TObject *Sender)
{
  if(m_dUpperTh > 0.0)
  {
    m_dUpperTh -= 0.1;
    UpperTh->Text = RoundStr(m_dUpperTh);
    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::UpperSpinUpClick(TObject *Sender)
{
  if(m_dUpperTh < m_dMaxHDOP)
  {
    m_dUpperTh += 0.1;
    UpperTh->Text = RoundStr(m_dUpperTh);
    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::GoClick(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    if(m_dUpperTh > 0.0 && m_dUpperTh < m_dMaxHDOP)
    {
      ProgressBar->Max = m_pCurrentTrack->PointCount;
      ProgressBar->Position = 0;

      int i = 0;
      while(i < m_pCurrentTrack->PointCount)
      {
        PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];

        if(pPoint->m_dHDOP > m_dUpperTh) m_pCurrentTrack->DeletePoint(i);
        else i++;

        ProgressBar->Position = ProgressBar->Position + 1;
        Application->ProcessMessages();
      }

      ProgressBar->Position = 0;
      m_pCurrentTrack->RecalcDistClimb();
    }

    Screen->Cursor = Save_Cursor;
  }

  if(m_pSpline)
  {
    delete m_pSpline;
    m_pSpline = NULL;
  }

  m_dMaxHDOP = 0.0;
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::UpperThKeyPress(TObject *Sender, char &Key)
{
  if((Key < '0' || Key > '9') && Key != '.' && Key != '\b' && Key != '\r')
    Key = '\0';

  if(Key == '\r')
  {
    m_dUpperTh = UpperTh->Text.ToDouble();

    if(m_dUpperTh > m_dMaxHDOP) m_dUpperTh = m_dMaxHDOP;
    else if(m_dUpperTh < 0.0) m_dUpperTh = 0.0;

    UpperTh->Text = RoundStr(m_dUpperTh);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::UpperThExit(TObject *Sender)
{
  m_dUpperTh = UpperTh->Text.ToDouble();

  if(m_dUpperTh > m_dMaxHDOP) m_dUpperTh = m_dMaxHDOP;
  else if(m_dUpperTh < 0.0) m_dUpperTh = 0.0;

  UpperTh->Text = RoundStr(m_dUpperTh);

  SetLimits();
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::FormShow(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    CalcStat();
    
    if(m_dMaxHDOP > 0.0)
    {
      int *pCounts = new int[INTERVALS];
      double *dPersentage = new double[INTERVALS];
      double *dX = new double[INTERVALS + 2];
      double *dY = new double[INTERVALS + 2];

      if(m_pSpline) delete m_pSpline;

      double nMaxPersentage = 0.0;
      double dInterval = m_dMaxHDOP / (double)(INTERVALS - 1);

      for(int i = 0; i < INTERVALS; i++) pCounts[i] = 0;

      for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
      {
        PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];

        int nInterval = (int)(pPoint->m_dHDOP / dInterval);
        pCounts[nInterval]++;
      }

      for(int i = 0; i < INTERVALS; i++)
      {
        dPersentage[i] = (double)pCounts[i] * 100.0 / (double)m_pCurrentTrack->PointCount;
        if(nMaxPersentage < dPersentage[i]) nMaxPersentage = dPersentage[i];
      }

      double dBarWidth = (double)HDOPDistribution->Width / (double)INTERVALS;
      double dPersUnit = (double)(HDOPDistribution->Height - 20) / (nMaxPersentage * 1.1);

      dX[0] = 0.0; dX[INTERVALS + 1] = (double)HDOPDistribution->Width;
      dY[0] = dY[INTERVALS + 1] = 0.0;

      for(int i = 0; i < INTERVALS; i++)
      {
        dX[i + 1] = ((double)i + 0.5) * dBarWidth;
        dY[i + 1] = dPersentage[i] * dPersUnit;
      }

      m_pSpline = new AkimaSpline(dX, dY, INTERVALS + 2);

      m_pBitmap->Width = HDOPDistribution->Width;
      m_pBitmap->Height = HDOPDistribution->Height;

      TCanvas *pCanvas = m_pBitmap->Canvas;

      int nBottom = HDOPDistribution->Height - BOTTOM_SPACE;

      pCanvas->Pen->Color = clWhite;
      pCanvas->Pen->Style = psSolid;
      pCanvas->Brush->Style = bsSolid;
      pCanvas->Brush->Color = clWhite;
      pCanvas->Rectangle(0, 0, HDOPDistribution->Width, HDOPDistribution->Height);
      pCanvas->Pen->Color = clBlack;
      pCanvas->MoveTo(0, nBottom + 1);
      pCanvas->LineTo(HDOPDistribution->Width, nBottom + 1);
      pCanvas->Pen->Color = clRed;

      for(int i = 0; i < HDOPDistribution->Width; i++)
      {
        double X = (double)i;
        double Y;

        m_pSpline->Interpolate(X, Y);
        if(Y < 0.0) Y = 0.0;

        pCanvas->MoveTo((int)X, nBottom);
        pCanvas->LineTo((int)X, nBottom - (int)Y);
      }

      AnsiString as = AnsiString("0.00");

      pCanvas->Font->Size = 10;
      pCanvas->Font->Color = clBlack;
	  pCanvas->TextOut(1, nBottom + 3, as);

      as = FormatFloat("0.00", m_dMaxHDOP);
	  pCanvas->TextOut(HDOPDistribution->Width - pCanvas->TextWidth(as) - 1, nBottom + 3, as);

      as = FormatFloat("0.00", m_dMaxHDOP / 2.0);
	  pCanvas->TextOut((HDOPDistribution->Width - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.00", m_dMaxHDOP / 4.0);
	  pCanvas->TextOut((HDOPDistribution->Width / 2 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.00", m_dMaxHDOP * 3.0 / 4.0);
	  pCanvas->TextOut((HDOPDistribution->Width * 3 / 2 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.00", m_dMaxHDOP / 8.0);
	  pCanvas->TextOut((HDOPDistribution->Width / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.00", m_dMaxHDOP * 3.0 / 8.0);
	  pCanvas->TextOut((HDOPDistribution->Width * 3 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.00", m_dMaxHDOP * 5.0 / 8.0);
	  pCanvas->TextOut((HDOPDistribution->Width * 5 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.00", m_dMaxHDOP * 7.0 / 8.0);
	  pCanvas->TextOut((HDOPDistribution->Width * 7 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      pCanvas = HDOPDistribution->Canvas;
      pCanvas->Draw(0, 0, m_pBitmap);

      delete [] dX;
      delete [] dY;
      delete [] dPersentage;
      delete [] pCounts;

      UpperSpin->Enabled = true;
      UpperTh->Enabled = true;
      Go->Enabled = true;
    }
  }

  if(m_dMaxHDOP == 0.0)
  {
    UpperSpin->Enabled = false;
    UpperTh->Enabled = false;
    Go->Enabled = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::SetLimits()
{
  TCanvas *pCanvas = HDOPDistribution->Canvas;
  pCanvas->Draw(0, 0, m_pBitmap);

  int nBottom = HDOPDistribution->Height - BOTTOM_SPACE;
  double Unit = (double)HDOPDistribution->Width / m_dMaxHDOP;

  int rBorder = (m_dUpperTh < m_dMaxHDOP) ? (int)(m_dUpperTh * Unit) : 0;

  pCanvas->Pen->Color = clBlack;
  pCanvas->Pen->Style = psSolid;

  if(rBorder)
  {
    pCanvas->MoveTo(rBorder, nBottom);
    pCanvas->LineTo(rBorder, 0);

    for(int i = rBorder + 1; i < HDOPDistribution->Width; i++)
    {
      double X = (double)i;
      double Y;

      m_pSpline->Interpolate(X, Y);
      if(Y < 0.0) Y = 0.0;

      pCanvas->MoveTo((int)X, nBottom);
      pCanvas->LineTo((int)X, nBottom - (int)Y);
    }
  }
}

void __fastcall THDOPForm::m_p2SClick(TObject *Sender)
{
  m_dUpperTh = m_dAverageHDOP + 2 * m_dSigma;
  UpperTh->Text = RoundStr(m_dUpperTh);

  SetLimits();
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::m_p3SClick(TObject *Sender)
{
  m_dUpperTh = m_dAverageHDOP + 3 * m_dSigma;
  UpperTh->Text = RoundStr(m_dUpperTh);

  SetLimits();
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::m_pCIClick(TObject *Sender)
{
  TCursor Save_Cursor = Screen->Cursor;
  Screen->Cursor = crHourGlass;

  int nPoints = (int)((double)(m_pCurrentTrack->PointCount * (100 - m_pCIPers->Value)) / 100.0);
  double *pArray = new double[m_pCurrentTrack->PointCount];

  for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
  {
    PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];
    pArray[i] = pPoint->m_dHDOP;
  }
  for(int i = 0; i < nPoints; i++) HighestFirst(&pArray[i], m_pCurrentTrack->PointCount - i);

  m_dUpperTh = pArray[nPoints - 1];
  UpperTh->Text = RoundStr(m_dUpperTh);
  SetLimits();

  delete [] pArray;

  Screen->Cursor = Save_Cursor;
}
//---------------------------------------------------------------------------

void __fastcall THDOPForm::CalcStat()
{
  m_dAverageHDOP = m_dMaxHDOP = m_dSigma = 0.0;

  for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
  {
    PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];

    if(pPoint->m_dHDOP > m_dMaxHDOP)
      m_dMaxHDOP = pPoint->m_dHDOP;

    m_dAverageHDOP += pPoint->m_dHDOP;
  }

  m_dAverageHDOP /= (double)m_pCurrentTrack->PointCount;

  for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
  {
    PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];

    double dD = pPoint->m_dHDOP - m_dAverageHDOP;
    m_dSigma += dD * dD;
  }

  m_dSigma = sqrt(m_dSigma / (double)m_pCurrentTrack->PointCount);
  m_dUpperTh = m_dMaxHDOP;

  m_pMax->Text = RoundStr(m_dMaxHDOP);
  m_pAvg->Text = RoundStr(m_dAverageHDOP);
  m_pS->Text = RoundStr(m_dSigma);
  UpperTh->Text = RoundStr(m_dUpperTh);
}

AnsiString __fastcall THDOPForm::RoundStr(double dValue)
{
  double dRound = ((double)((int)((dValue * 10.0) + 0.5))) / 10.0;
  return FormatFloat("0.0", dRound);
}

void __fastcall THDOPForm::HighestFirst(double *pArray, int n)
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
