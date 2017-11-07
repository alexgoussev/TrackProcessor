//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Median.h"
#include "SpdFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TSpeedFilter *SpeedFilter;
//---------------------------------------------------------------------------
__fastcall TSpeedFilter::TSpeedFilter(TComponent* Owner)
  : TForm(Owner)
{
  m_pCurrentTrack = NULL;
  m_pBitmap = new Graphics::TBitmap();
  m_pSpline = NULL;
  m_dMaxSpeed = 0.0;
}
//---------------------------------------------------------------------------
void __fastcall TSpeedFilter::FormShow(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    m_dMaxSpeed = 0.0;

    for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
    {
      PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];
      double dSpeed = pPoint->m_dSpeed * 3.6;

      if(pPoint->m_dSpeed != GPS_PT_NO_VALUE && dSpeed > m_dMaxSpeed)
        m_dMaxSpeed = dSpeed;
    }

    if(m_dMaxSpeed > 0.0)
    {
      int *pCounts = new int[INTERVALS];
      double *dPersentage = new double[INTERVALS];
      double *dX = new double[INTERVALS + 2];
      double *dY = new double[INTERVALS + 2];

      if(m_pSpline) delete m_pSpline;

      double nMaxPersentage = 0.0;
      double dInterval = m_dMaxSpeed / (double)(INTERVALS - 1);

      for(int i = 0; i < INTERVALS; i++) pCounts[i] = 0;

      for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
      {
        PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];
        double dSpeed = pPoint->m_dSpeed * 3.6;

        int nInterval = (int)(dSpeed / dInterval);
        pCounts[nInterval]++;
      }

      for(int i = 0; i < INTERVALS; i++)
      {
        dPersentage[i] = (double)pCounts[i] * 100.0 / (double)m_pCurrentTrack->PointCount;
        if(nMaxPersentage < dPersentage[i]) nMaxPersentage = dPersentage[i];
      }

      double dBarWidth = (double)SpeedDistribution->Width / (double)INTERVALS;
      double dPersUnit = (double)(SpeedDistribution->Height - 20) / (nMaxPersentage * 1.1);

      dX[0] = 0.0; dX[INTERVALS + 1] = (double)SpeedDistribution->Width;
      dY[0] = dY[INTERVALS + 1] = 0.0;

      for(int i = 0; i < INTERVALS; i++)
      {
        dX[i + 1] = ((double)i + 0.5) * dBarWidth;
        dY[i + 1] = dPersentage[i] * dPersUnit;
      }

      m_pSpline = new AkimaSpline(dX, dY, INTERVALS + 2);

      m_pBitmap->Width = SpeedDistribution->Width;
      m_pBitmap->Height = SpeedDistribution->Height;

      TCanvas *pCanvas = m_pBitmap->Canvas;

      int nBottom = SpeedDistribution->Height - BOTTOM_SPACE;

      pCanvas->Pen->Color = clWhite;
      pCanvas->Pen->Style = psSolid;
      pCanvas->Brush->Style = bsSolid;
      pCanvas->Brush->Color = clWhite;
      pCanvas->Rectangle(0, 0, SpeedDistribution->Width, SpeedDistribution->Height);
      pCanvas->Pen->Color = clBlack;
      pCanvas->MoveTo(0, nBottom + 1);
      pCanvas->LineTo(SpeedDistribution->Width, nBottom + 1);
      pCanvas->Pen->Color = clRed;

      for(int i = 0; i < SpeedDistribution->Width; i++)
      {
        double X = (double)i;
        double Y;

        m_pSpline->Interpolate(X, Y);
        if(Y < 0.0) Y = 0.0;

        pCanvas->MoveTo((int)X, nBottom);
        pCanvas->LineTo((int)X, nBottom - (int)Y);
      }

      AnsiString as = AnsiString("0.0");

      pCanvas->Font->Size = 10;
      pCanvas->Font->Color = clBlack;
      pCanvas->TextOut(1, nBottom + 3, as);

      as = FormatFloat("0.0", m_dMaxSpeed);
      pCanvas->TextOut(SpeedDistribution->Width - pCanvas->TextWidth(as) - 1, nBottom + 3, as);

      as = FormatFloat("0.0", m_dMaxSpeed / 2.0);
      pCanvas->TextOut((SpeedDistribution->Width - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.0", m_dMaxSpeed / 4.0);
      pCanvas->TextOut((SpeedDistribution->Width / 2 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.0", m_dMaxSpeed * 3.0 / 4.0);
      pCanvas->TextOut((SpeedDistribution->Width * 3 / 2 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.0", m_dMaxSpeed / 8.0);
      pCanvas->TextOut((SpeedDistribution->Width / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.0", m_dMaxSpeed * 3.0 / 8.0);
      pCanvas->TextOut((SpeedDistribution->Width * 3 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.0", m_dMaxSpeed * 5.0 / 8.0);
      pCanvas->TextOut((SpeedDistribution->Width * 5 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      as = FormatFloat("0.0", m_dMaxSpeed * 7.0 / 8.0);
      pCanvas->TextOut((SpeedDistribution->Width * 7 / 4 - pCanvas->TextWidth(as)) / 2, nBottom + 3, as);

      pCanvas = SpeedDistribution->Canvas;
      pCanvas->Draw(0, 0, m_pBitmap);

      LowerTh->Text = FormatFloat("0.0", 0.0);
      UpperTh->Text = FormatFloat("0.0", m_dMaxSpeed);

      delete [] dX;
      delete [] dY;
      delete [] dPersentage;
      delete [] pCounts;

      LowerSpin->Enabled = true;
      LowerTh->Enabled = true;
      UpperSpin->Enabled = true;
      UpperTh->Enabled = true;
      Go->Enabled = true;
    }
  }

  if(m_dMaxSpeed == 0.0)
  {
    LowerSpin->Enabled = false;
    LowerTh->Enabled = false;
    UpperSpin->Enabled = false;
    UpperTh->Enabled = false;
    Go->Enabled = false;
  }
}
//---------------------------------------------------------------------------

__fastcall TSpeedFilter::~TSpeedFilter()
{
  delete m_pBitmap;
  if(m_pSpline) delete m_pSpline;
}

void __fastcall TSpeedFilter::LowerSpinDownClick(TObject *Sender)
{
  double lwth = LowerTh->Text.ToDouble();
  if(lwth > 0.0)
  {
    lwth -= 0.1;
    LowerTh->Text = FormatFloat("0.0", lwth);// = AnsiString(lwth);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall TSpeedFilter::LowerSpinUpClick(TObject *Sender)
{
  double lwth = LowerTh->Text.ToDouble();
  if(lwth < m_dMaxSpeed)
  {
    lwth += 0.1;
    LowerTh->Text = FormatFloat("0.0", lwth);// = AnsiString(lwth);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall TSpeedFilter::UpperSpinDownClick(TObject *Sender)
{
  double upth = UpperTh->Text.ToDouble();
  if(upth > 0.0)
  {
    upth -= 0.1;
    UpperTh->Text = FormatFloat("0.0", upth);// = AnsiString(upth);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall TSpeedFilter::UpperSpinUpClick(TObject *Sender)
{
  double upth = UpperTh->Text.ToDouble();
  if(upth < m_dMaxSpeed)
  {
    upth += 0.1;
    UpperTh->Text = FormatFloat("0.0", upth);// = AnsiString(upth);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall TSpeedFilter::ThKeyPress(TObject *Sender, char &Key)
{
  if((Key < '0' || Key > '9') && Key != '.' && Key != '\b' && Key != '\r')
    Key = '\0';

  if(Key == '\r')
  {
    double th = UpperTh->Text.ToDouble();

    if(th > m_dMaxSpeed) th = m_dMaxSpeed;
    else if(th < 0.0) th = 0.0;

    UpperTh->Text = FormatFloat("0.0", th);

    th = LowerTh->Text.ToDouble();

    if(th > m_dMaxSpeed) th = m_dMaxSpeed;
    else if(th < 0.0) th = 0.0;

    LowerTh->Text = FormatFloat("0.0", th);// = AnsiString(upth);

    SetLimits();
  }
}
//---------------------------------------------------------------------------

void __fastcall TSpeedFilter::LowerThExit(TObject *Sender)
{
  double th = LowerTh->Text.ToDouble();

  if(th > m_dMaxSpeed)
    LowerTh->Text = FormatFloat("0.0", m_dMaxSpeed);// = AnsiString(upth);
  else if(th < 0.0)
    LowerTh->Text = FormatFloat("0.0", 0.0);// = AnsiString(upth);

  SetLimits();
}
//---------------------------------------------------------------------------

void __fastcall TSpeedFilter::UpperThExit(TObject *Sender)
{
  double th = UpperTh->Text.ToDouble();

  if(th > m_dMaxSpeed)
    UpperTh->Text = FormatFloat("0.0", m_dMaxSpeed);// = AnsiString(upth);
  else if(th < 0.0)
    UpperTh->Text = FormatFloat("0.0", 0.0);// = AnsiString(upth);

  SetLimits();
}
//---------------------------------------------------------------------------


void __fastcall TSpeedFilter::SetLimits()
{
  TCanvas *pCanvas = SpeedDistribution->Canvas;
  pCanvas->Draw(0, 0, m_pBitmap);

  int nBottom = SpeedDistribution->Height - BOTTOM_SPACE;
  double lwTh = LowerTh->Text.ToDouble();
  double upTh = UpperTh->Text.ToDouble();
  double Unit = (double)SpeedDistribution->Width / m_dMaxSpeed;

  int lBorder = (lwTh > 0.0) ? (int)(lwTh * Unit) : 0;
  int rBorder = (upTh < m_dMaxSpeed) ? (int)(upTh * Unit) : 0;

  pCanvas->Pen->Color = clBlack;
  pCanvas->Pen->Style = psSolid;

  if(lBorder)
  {
    pCanvas->MoveTo(lBorder, nBottom);
    pCanvas->LineTo(lBorder, 0);

    for(int i = 0; i < lBorder; i++)
    {
      double X = (double)i;
      double Y;

      m_pSpline->Interpolate(X, Y);
      if(Y < 0.0) Y = 0.0;

      pCanvas->MoveTo((int)X, nBottom);
      pCanvas->LineTo((int)X, nBottom - (int)Y);
    }
  }

  if(rBorder)
  {
    pCanvas->MoveTo(rBorder, nBottom);
    pCanvas->LineTo(rBorder, 0);

    for(int i = rBorder + 1; i < SpeedDistribution->Width; i++)
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

void __fastcall TSpeedFilter::GoClick(TObject *Sender)
{
  if(m_pCurrentTrack)
  {
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    double lTh = LowerTh->Text.ToDouble();
    double uTh = UpperTh->Text.ToDouble();

    if(lTh > 0.0 || uTh < m_dMaxSpeed)
    {
      ProgressBar->Max = m_pCurrentTrack->PointCount;
      ProgressBar->Position = 0;
      int i = 0;

      while(i < m_pCurrentTrack->PointCount)
      {
        PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];
        double Speed = pPoint->m_dSpeed * 3.6;

        if(Speed < lTh)
        {
          int iFirst = i, iLast = i;

          for(int j = i + 1; j < m_pCurrentTrack->PointCount; j++)
          {
            pPoint = m_pCurrentTrack->Points[j];

            if(pPoint->m_dSpeed * 3.6 >= lTh) break;
            else iLast = j;
          }

          int interval = iLast - iFirst;

          if(interval == 1)
          {
            PGPXTrackPoint p1 = m_pCurrentTrack->Points[iFirst];
            PGPXTrackPoint p2 = m_pCurrentTrack->Points[iLast];

            p1->m_dSpeed = p2->m_dSpeed = 0.0;
            p1->m_dAlt = p2->m_dAlt = (p1->m_dAlt + p2->m_dAlt) / 2.0;
            p1->m_dLat = p2->m_dLat = (p1->m_dLat + p2->m_dLat) / 2.0;
            p1->m_dLon = p2->m_dLon = (p1->m_dLon + p2->m_dLon) / 2.0;

            i += 2;
          }
          else if(interval > 1)
          {
            int j, k, n = interval + 1;
            double *lat = new double[n];
            double *lon = new double[n];
            double *alt = new double[n];

            for(j = iFirst, k = 0; j <= iLast; j++, k++)
            {
              pPoint = m_pCurrentTrack->Points[j];
              lat[k] = pPoint->m_dLat;
              lon[k] = pPoint->m_dLon;
              alt[k] = pPoint->m_dAlt;
            }

            Median *pMedian = new Median;
            PGPXTrackPoint p1 = m_pCurrentTrack->Points[iFirst];
            PGPXTrackPoint p2 = m_pCurrentTrack->Points[iLast];

            p1->m_dSpeed = p2->m_dSpeed = 0.0;
            p1->m_dLat = p2->m_dLat = pMedian->GetMedian(lat, n);
            p1->m_dLon = p2->m_dLon = pMedian->GetMedian(lon, n);
            p1->m_dAlt = p2->m_dAlt = pMedian->GetMedian(alt, n);

            delete pMedian;
            delete [] lat;
            delete [] lon;
            delete [] alt;

            k = iFirst + 1;

            for(j = k; j < iLast; j++)
              m_pCurrentTrack->DeletePoint(k);

            i += 2;
          }
          else
          {
            pPoint = m_pCurrentTrack->Points[iFirst];
            pPoint->m_dSpeed = 0.0;
            i++;
          }
        }
        else if(Speed > uTh)
          m_pCurrentTrack->DeletePoint(i);
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

  m_dMaxSpeed = 0.0;
}
//---------------------------------------------------------------------------

