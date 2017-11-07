//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "SK42WGS84\\SK42WGS84.h"
#include "HP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
THPForm *HPForm;

//---------------------------------------------------------------------------
__fastcall THPForm::THPForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  
  m_dLambda = pini->ReadFloat("HP", "Lambda", 500);
  m_pCoordinates->Checked = pini->ReadBool("HP", "Coordinates", true);
  m_pAltitude->Checked = pini->ReadBool("HP", "Altitude", false);
  m_pSpeed->Checked = pini->ReadBool("HP", "Speed", false);

  delete pini;

  m_pLambda->Text = AnsiString(m_dLambda);
}
//---------------------------------------------------------------------------

__fastcall THPForm::~THPForm()
{
  //TODO: Add your source code here
}

void __fastcall THPForm::Button1Click(TObject *Sender)
{
  ProgressBar->Max = m_pTrack->PointCount * 6 - 4;
  m_nProgress = 0;
  ProgressBar->Position = m_nProgress;
  TCursor Save_Cursor = Screen->Cursor;
  Screen->Cursor = crHourGlass;

  m_dLambda = m_pLambda->Text.ToDouble();

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  pini->WriteFloat("HP", "Lambda", m_dLambda);
  pini->WriteBool("HP", "Coordinates", m_pCoordinates->Checked);
  pini->WriteBool("HP", "Altitude", m_pAltitude->Checked);
  pini->WriteBool("HP", "Speed", m_pSpeed->Checked);
  delete pini;

  ProcessHP();

  Screen->Cursor = Save_Cursor;
  m_nProgress = 0;
  ProgressBar->Position = m_nProgress;
}
//---------------------------------------------------------------------------

void __fastcall THPForm::SetTrack(PGPXTrack value)
{
  if(m_pTrack != value) {
    m_pTrack = value;

    if(m_pTrack)
    {
      Button1->Enabled = true;
      m_pLambda->Enabled = true;
      m_pCoordinates->Enabled = true;
      m_pSpeed->Enabled = true;

      if(m_pTrack->m_bNoAlt)
      {
        m_pAltitude->Enabled = false;
        m_pAltitude->Checked = false;
      }
    }
    else
    {
      Button1->Enabled = false;
      m_pLambda->Enabled = false;
      m_pCoordinates->Enabled = false;
      m_pAltitude->Enabled = false;
      m_pSpeed->Enabled = false;
    }

  }
}

PGPXTrack __fastcall THPForm::GetTrack()
{
  return m_pTrack;
}

void __fastcall THPForm::ProcessHP()
{
  tagDataSet sInput, sOutput;

  if(m_pCoordinates->Checked)
  {
    sInput.pX = new double[m_pTrack->PointCount];
    sInput.pY = new double[m_pTrack->PointCount];
    sOutput.pX = new double[m_pTrack->PointCount];
    sOutput.pY = new double[m_pTrack->PointCount];
  }
  else
    sInput.pX = sInput.pY = sOutput.pX = sOutput.pY = NULL;

  if(m_pAltitude->Checked)
  {
    sInput.pA = new double[m_pTrack->PointCount];
    sOutput.pA = new double[m_pTrack->PointCount];
  }
  else
    sInput.pA = sOutput.pA = NULL;

  if(m_pSpeed->Checked)
  {
    //sInput.pS = new double[m_pTrack->PointCount];
    //sOutput.pS = new double[m_pTrack->PointCount];
    sInput.pVx = new double[m_pTrack->PointCount];
    sOutput.pVx = new double[m_pTrack->PointCount];
    sInput.pVy = new double[m_pTrack->PointCount];
    sOutput.pVy = new double[m_pTrack->PointCount];
  }
  else
    sInput.pVx = sOutput.pVx = sInput.pVy = sOutput.pVy = NULL;
    //sInput.pS = sOutput.pS = NULL;

  for(int i = 0; i < m_pTrack->PointCount; i++)
  {
    PGPXTrackPoint pPoint = m_pTrack->Points[i];

    if(m_pCoordinates->Checked)
    {
      double x, y;
      pPoint->GetdProj(&x, &y);

      sInput.pX[i] = x; sInput.pY[i] = y;
    }

    if(m_pAltitude->Checked)
      sInput.pA[i] = pPoint->m_dAlt;

    if(m_pSpeed->Checked)
    {
      DecomposeSpeed(pPoint->m_dSpeed, pPoint->m_dCourse, &sInput.pVx[i], &sInput.pVy[i]);
      //sInput.pS[i] = pPoint->m_dSpeed;
    }

    m_nProgress++;
    ProgressBar->Position = m_nProgress;
    Application->ProcessMessages();
  }

  if(!HPFilterProc(&sInput, m_pTrack->PointCount, m_dLambda, &sOutput))
  {
    PGPXTrack pTrack = new GPXTrack;

    for(int i = 0; i < m_pTrack->PointCount; i++)
    {
      PGPXTrackPoint pNewPoint = pTrack->NewPoint;
      pNewPoint->SetPoint(m_pTrack->Points[i]);

      if(m_pCoordinates->Checked)
        pNewPoint->SetdProj(sOutput.pX[i], sOutput.pY[i]);

      if(m_pAltitude->Checked)
        pNewPoint->m_dAlt = sOutput.pA[i];

      if(m_pSpeed->Checked)
      {
        double dV, dC;

        ComposeSpeed(sOutput.pVx[i], sOutput.pVy[i], &dV, &dC);
        pNewPoint->m_dSpeed = dV;
        pNewPoint->m_dCourse = dC;  
        //pNewPoint->m_dSpeed = sOutput.pS[i];
      }

      pTrack->AddPoint(pNewPoint);

      m_nProgress++;
      ProgressBar->Position = m_nProgress;
      Application->ProcessMessages();
    }

    m_pTrack = pTrack;
  }

  if(sInput.pX) delete [] sInput.pX;
  if(sInput.pY) delete [] sInput.pY;
  if(sInput.pA) delete [] sInput.pA;
  //if(sInput.pS) delete [] sInput.pS;
  if(sInput.pVx) delete [] sInput.pVx;
  if(sInput.pVy) delete [] sInput.pVy;
  if(sOutput.pX) delete [] sOutput.pX;
  if(sOutput.pY) delete [] sOutput.pY;
  if(sOutput.pA) delete [] sOutput.pA;
  //if(sOutput.pS) delete [] sOutput.pS;
  if(sOutput.pVx) delete [] sOutput.pVx;
  if(sOutput.pVy) delete [] sOutput.pVy;
}

int __fastcall THPForm::HPFilterProc(PDataSet pInput, int nobs, double lambda, PDataSet pOutput)
{
/*
     *  Schneller Algorithmus zur Lösung des LGS BxX=Y
     * unter Berücksichtigung, dass B eine pentadiagonale Matrix ist
     * siehe z.B. Helmut Späth "Numerik, Eine Einführung für
     * Mathematiker und Informatiker", S. 110 ff, Braunschweig &
     * Wiesbaden, 1994
     *
*/
  int rc = 0;

  /* Testen, ob nobs >5 */
  if (nobs <= 5) rc = 1;
  else
  {
    /* Speicher in den HEAP */
    tagHPData sX, sY, sA, /*sS*/sVx, sVy;
    
    if(m_pCoordinates->Checked)
    {
      sX.a = new double[nobs];
      sX.b = new double[nobs];
      sX.c = new double[nobs];

      sY.a = new double[nobs];
      sY.b = new double[nobs];
      sY.c = new double[nobs];

      if(sX.a == NULL || sX.b == NULL || sX.c == NULL ||
         sY.a == NULL || sY.b == NULL || sY.c == NULL) rc = 2;

      sX.H1 = sX.H2 = sX.H3 = sX.H4 = sX.H5 = 0.0;
      sX.HH1 = sX.HH2 = sX.HH3 = sX.HH4 = sX.HH5 = 0.0;

      sY.H1 = sY.H2 = sY.H3 = sY.H4 = sY.H5 = 0.0;
      sY.HH1 = sY.HH2 = sY.HH3 = sY.HH4 = sY.HH5 = 0.0;
    }
    else
    {
      sX.a = sX.b = sX.c = NULL;
      sY.a = sY.b = sY.c = NULL;
    }

    if(m_pAltitude->Checked && rc == 0)
    {
      sA.a = new double[nobs];
      sA.b = new double[nobs];
      sA.c = new double[nobs];

      if(sA.a == NULL || sA.b == NULL || sA.c == NULL) rc = 2;

      sA.H1 = sA.H2 = sA.H3 = sA.H4 = sA.H5 = 0.0;
      sA.HH1 = sA.HH2 = sA.HH3 = sA.HH4 = sA.HH5 = 0.0;
    }
    else
      sA.a = sA.b = sA.c = NULL;

    if(m_pSpeed->Checked && rc == 0)
    {
      sVx.a = new double[nobs];
      sVx.b = new double[nobs];
      sVx.c = new double[nobs];

      if(sVx.a == NULL || sVx.b == NULL || sVx.c == NULL) rc = 2;

      sVx.H1 = sVx.H2 = sVx.H3 = sVx.H4 = sVx.H5 = 0.0;
      sVx.HH1 = sVx.HH2 = sVx.HH3 = sVx.HH4 = sVx.HH5 = 0.0;

      sVy.a = new double[nobs];
      sVy.b = new double[nobs];
      sVy.c = new double[nobs];

      if(sVy.a == NULL || sVy.b == NULL || sVy.c == NULL) rc = 2;

      sVy.H1 = sVy.H2 = sVy.H3 = sVy.H4 = sVy.H5 = 0.0;
      sVy.HH1 = sVy.HH2 = sVy.HH3 = sVy.HH4 = sVy.HH5 = 0.0;

      /*
      sS.a = new double[nobs];
      sS.b = new double[nobs];
      sS.c = new double[nobs];

      if(sS.a == NULL || sS.b == NULL || sS.c == NULL) rc = 2;

      sS.H1 = sS.H2 = sS.H3 = sS.H4 = sS.H5 = 0.0;
      sS.HH1 = sS.HH2 = sS.HH3 = sS.HH4 = sS.HH5 = 0.0;
      */
    }
    else
      sVx.a = sVx.b = sVx.c = sVy.a = sVy.b = sVy.c = NULL;
      //sS.a = sS.b = sS.c = NULL;

    /* Testen auf Speicherproblem */
    if(rc == 0)
    {
      /* Da bei diesem Algorithmus, die Daten überschrieben wrden, müssen wir
        ein neuen Array füllem.
      */
      for(int i = 0; i < nobs; i++)
      {
        if(m_pCoordinates->Checked)
        {
          pOutput->pX[i] = pInput->pX[i];
          pOutput->pY[i] = pInput->pY[i];
        }

        if(m_pAltitude->Checked)
          pOutput->pA[i] = pInput->pA[i];

        if(m_pSpeed->Checked)
        {
          pOutput->pVx[i] = pInput->pVx[i];
          pOutput->pVy[i] = pInput->pVy[i];
          //pOutput->pS[i] = pInput->pS[i];
        }

        m_nProgress++;
        ProgressBar->Position = m_nProgress;
        Application->ProcessMessages();
      }

      /* Koeffizientenmatrix erstellen */
      if(m_pCoordinates->Checked)
      {
        sX.a[0] = sY.a[0] = 1.0 + lambda;
        sX.b[0] = sY.b[0] = -2.0 * lambda;
        sX.c[0] = sY.c[0] = lambda;
      }

      if(m_pAltitude->Checked)
      {
        sA.a[0] = 1.0 + lambda;
        sA.b[0] = -2.0 * lambda;
        sA.c[0] = lambda;
      }

      if(m_pSpeed->Checked)
      {
        sVx.a[0] = 1.0 + lambda;
        sVx.b[0] = -2.0 * lambda;
        sVx.c[0] = lambda;

        sVy.a[0] = 1.0 + lambda;
        sVy.b[0] = -2.0 * lambda;
        sVy.c[0] = lambda;

        /*
        sS.a[0] = 1.0 + lambda;
        sS.b[0] = -2.0 * lambda;
        sS.c[0] = lambda;
        */
      }

      for(int i = 1; i < nobs - 2; i++)
      {
        if(m_pCoordinates->Checked)
        {
          sX.a[i] = sY.a[i] = 6.0 * lambda + 1.0;
          sX.b[i] = sY.b[i] = -4.0 * lambda;
          sX.c[i] = sY.c[i] = lambda;
        }

        if(m_pAltitude->Checked)
        {
          sA.a[i] = 6.0 * lambda + 1.0;
          sA.b[i] = -4.0 * lambda;
          sA.c[i] = lambda;
        }

        if(m_pSpeed->Checked)
        {
          sVx.a[i] = 6.0 * lambda + 1.0;
          sVx.b[i] = -4.0 * lambda;
          sVx.c[i] = lambda;

          sVy.a[i] = 6.0 * lambda + 1.0;
          sVy.b[i] = -4.0 * lambda;
          sVy.c[i] = lambda;

          /*
          sS.a[i] = 6.0 * lambda + 1.0;
          sS.b[i] = -4.0 * lambda;
          sS.c[i] = lambda;
          */
        }

        m_nProgress++;
        ProgressBar->Position = m_nProgress;
        Application->ProcessMessages();
      }

      if(m_pCoordinates->Checked)
      {
        sX.a[1] = sY.a[1] = 5.0 * lambda + 1.0;
        sX.a[nobs-1] = sY.a[nobs-1] = 1.0 + lambda;
        sX.a[nobs-2] = sY.a[nobs-2] = 5.0 * lambda + 1.0;

        sX.b[nobs-2] = sY.b[nobs-2] = -2.0 * lambda;
        sX.b[nobs-1] = sY.b[nobs-1] = 0.0;

        sX.c[nobs-2] = sY.c[nobs-2] = 0.0;
        sX.c[nobs-1] = sY.c[nobs-1] = 0.0;
      }

      if(m_pAltitude->Checked)
      {
        sA.a[1] = 5.0 * lambda + 1.0;
        sA.a[nobs-1] = 1.0 + lambda;
        sA.a[nobs-2] = 5.0 * lambda + 1.0;

        sA.b[nobs-2] = -2.0 * lambda;
        sA.b[nobs-1] = 0.0;

        sA.c[nobs-2] = 0.0;
        sA.c[nobs-1] = 0.0;
      }

      if(m_pSpeed->Checked)
      {
        sVx.a[1] = 5.0 * lambda + 1.0;
        sVx.a[nobs-1] = 1.0 + lambda;
        sVx.a[nobs-2] = 5.0 * lambda + 1.0;

        sVx.b[nobs-2] = -2.0 * lambda;
        sVx.b[nobs-1] = 0.0;

        sVx.c[nobs-2] = 0.0;
        sVx.c[nobs-1] = 0.0;

        sVy.a[1] = 5.0 * lambda + 1.0;
        sVy.a[nobs-1] = 1.0 + lambda;
        sVy.a[nobs-2] = 5.0 * lambda + 1.0;

        sVy.b[nobs-2] = -2.0 * lambda;
        sVy.b[nobs-1] = 0.0;

        sVy.c[nobs-2] = 0.0;
        sVy.c[nobs-1] = 0.0;

        /*
        sS.a[1] = 5.0 * lambda + 1.0;
        sS.a[nobs-1] = 1.0 + lambda;
        sS.a[nobs-2] = 5.0 * lambda + 1.0;

        sS.b[nobs-2] = -2.0 * lambda;
        sS.b[nobs-1] = 0.0;

        sS.c[nobs-2] = 0.0;
        sS.c[nobs-1] = 0.0;
        */
      }

      /* Vorwärts du junge Garde */
      for(int i = 0; i < nobs; i++)
      {
        if(m_pCoordinates->Checked)
        {
          sX.Z = sX.a[i] - sX.H4 * sX.H1 - sX.HH5 * sX.HH2;
          sY.Z = sY.a[i] - sY.H4 * sY.H1 - sY.HH5 * sY.HH2;
        }

        if(m_pAltitude->Checked)
          sA.Z = sA.a[i] - sA.H4 * sA.H1 - sA.HH5 * sA.HH2;

        if(m_pSpeed->Checked)
        {
          //sS.Z = sS.a[i] - sS.H4 * sS.H1 - sS.HH5 * sS.HH2;
          sVx.Z = sVx.a[i] - sVx.H4 * sVx.H1 - sVx.HH5 * sVx.HH2;
          sVy.Z = sVy.a[i] - sVy.H4 * sVy.H1 - sVy.HH5 * sVy.HH2;
        }

        if((m_pCoordinates->Checked && (sX.Z == 0.0 || sY.Z == 0.0)) ||
           (m_pAltitude->Checked && sA.Z == 0.0)                     ||
           (m_pSpeed->Checked && (/*sS.Z == 0.0*/ sVx.Z == 0.0 || sVy.Z == 0.0)))
        {
          rc = 3;
          break;
        }
        else
        {
          if(m_pCoordinates->Checked)
          {
            sX.HB = sX.b[i];
            sX.HH1 = sX.H1;
            sX.H1 = (sX.HB - sX.H4 * sX.H2) / sX.Z;

            sX.b[i] = sX.H1;

            sX.HC = sX.c[i];
            sX.HH2 = sX.H2;
            sX.H2 = sX.HC / sX.Z;
            sX.c[i] = sX.H2;

            sX.a[i] = (pOutput->pX[i] - sX.HH3 * sX.HH5 - sX.H3 * sX.H4) / sX.Z;
            sX.HH3 = sX.H3;
            sX.H3 = sX.a[i];

            sX.H4 = sX.HB - sX.H5 * sX.HH1;
            sX.HH5 = sX.H5;
            sX.H5 = sX.HC;

            sY.HB = sY.b[i];
            sY.HH1 = sY.H1;
            sY.H1 = (sY.HB - sY.H4 * sY.H2) / sY.Z;

            sY.b[i] = sY.H1;

            sY.HC = sY.c[i];
            sY.HH2 = sY.H2;
            sY.H2 = sY.HC / sY.Z;
            sY.c[i] = sY.H2;

            sY.a[i] = (pOutput->pY[i] - sY.HH3 * sY.HH5 - sY.H3 * sY.H4) / sY.Z;
            sY.HH3 = sY.H3;
            sY.H3 = sY.a[i];

            sY.H4 = sY.HB - sY.H5 * sY.HH1;
            sY.HH5 = sY.H5;
            sY.H5 = sY.HC;
          }

          if(m_pAltitude->Checked)
          {
            sA.HB = sA.b[i];
            sA.HH1 = sA.H1;
            sA.H1 = (sA.HB - sA.H4 * sA.H2) / sA.Z;

            sA.b[i] = sA.H1;

            sA.HC = sA.c[i];
            sA.HH2 = sA.H2;
            sA.H2 = sA.HC / sA.Z;
            sA.c[i] = sA.H2;

            sA.a[i] = (pOutput->pA[i] - sA.HH3 * sA.HH5 - sA.H3 * sA.H4) / sA.Z;
            sA.HH3 = sA.H3;
            sA.H3 = sA.a[i];

            sA.H4 = sA.HB - sA.H5 * sA.HH1;
            sA.HH5 = sA.H5;
            sA.H5 = sA.HC;
          }

          if(m_pSpeed->Checked)
          {
            sVx.HB = sVx.b[i];
            sVx.HH1 = sVx.H1;
            sVx.H1 = (sVx.HB - sVx.H4 * sVx.H2) / sVx.Z;

            sVx.b[i] = sVx.H1;

            sVx.HC = sVx.c[i];
            sVx.HH2 = sVx.H2;
            sVx.H2 = sVx.HC / sVx.Z;
            sVx.c[i] = sVx.H2;

            sVx.a[i] = (pOutput->pVx[i] - sVx.HH3 * sVx.HH5 - sVx.H3 * sVx.H4) / sVx.Z;
            sVx.HH3 = sVx.H3;
            sVx.H3 = sVx.a[i];

            sVx.H4 = sVx.HB - sVx.H5 * sVx.HH1;
            sVx.HH5 = sVx.H5;
            sVx.H5 = sVx.HC;

            //

            sVy.HB = sVy.b[i];
            sVy.HH1 = sVy.H1;
            sVy.H1 = (sVy.HB - sVy.H4 * sVy.H2) / sVy.Z;

            sVy.b[i] = sVy.H1;

            sVy.HC = sVy.c[i];
            sVy.HH2 = sVy.H2;
            sVy.H2 = sVy.HC / sVy.Z;
            sVy.c[i] = sVy.H2;

            sVy.a[i] = (pOutput->pVy[i] - sVy.HH3 * sVy.HH5 - sVy.H3 * sVy.H4) / sVy.Z;
            sVy.HH3 = sVy.H3;
            sVy.H3 = sVy.a[i];

            sVy.H4 = sVy.HB - sVy.H5 * sVy.HH1;
            sVy.HH5 = sVy.H5;
            sVy.H5 = sVy.HC;

            /*
            sS.HB = sS.b[i];
            sS.HH1 = sS.H1;
            sS.H1 = (sS.HB - sS.H4 * sS.H2) / sS.Z;

            sS.b[i] = sS.H1;

            sS.HC = sS.c[i];
            sS.HH2 = sS.H2;
            sS.H2 = sS.HC / sS.Z;
            sS.c[i] = sS.H2;

            sS.a[i] = (pOutput->pS[i] - sS.HH3 * sS.HH5 - sS.H3 * sS.H4) / sS.Z;
            sS.HH3 = sS.H3;
            sS.H3 = sS.a[i];

            sS.H4 = sS.HB - sS.H5 * sS.HH1;
            sS.HH5 = sS.H5;
            sS.H5 = sS.HC;
            */
          }
        }

        m_nProgress++;
        ProgressBar->Position = m_nProgress;
        Application->ProcessMessages();
      }

      if(rc == 0)
      {
        /* Rückwärts muss die junge Garde auch */
        if(m_pCoordinates->Checked)
        {
          sX.H2 = 0;
          sX.H1 = sX.a[nobs - 1];
          pOutput->pX[nobs - 1] = sX.H1;

          sY.H2 = 0;
          sY.H1 = sY.a[nobs - 1];
          pOutput->pY[nobs - 1] = sY.H1;
        }

        if(m_pAltitude->Checked)
        {
          sA.H2 = 0;
          sA.H1 = sA.a[nobs - 1];
          pOutput->pA[nobs - 1] = sA.H1;
        }

        if(m_pSpeed->Checked)
        {
          sVx.H2 = 0;
          sVx.H1 = sVx.a[nobs - 1];
          pOutput->pVx[nobs - 1] = sVx.H1;

          sVy.H2 = 0;
          sVy.H1 = sVy.a[nobs - 1];
          pOutput->pVy[nobs - 1] = sVy.H1;

          /*
          sS.H2 = 0;
          sS.H1 = sS.a[nobs - 1];
          pOutput->pS[nobs - 1] = sS.H1;
          */
        }

        for(int i = nobs-1; i > 0; i--)
        {
          if(m_pCoordinates->Checked)
          {
            pOutput->pX[i - 1] = sX.a[i - 1] - sX.b[i - 1] * sX.H1 - sX.c[i - 1] * sX.H2;
            sX.H2 = sX.H1;
            sX.H1 = pOutput->pX[i - 1];

            pOutput->pY[i - 1] = sY.a[i - 1] - sY.b[i - 1] * sY.H1 - sY.c[i - 1] * sY.H2;
            sY.H2 = sY.H1;
            sY.H1 = pOutput->pY[i - 1];
          }

          if(m_pAltitude->Checked)
          {
            pOutput->pA[i - 1] = sA.a[i - 1] - sA.b[i - 1] * sA.H1 - sA.c[i - 1] * sA.H2;
            sA.H2 = sA.H1;
            sA.H1 = pOutput->pA[i - 1];
          }

          if(m_pSpeed->Checked)
          {
            pOutput->pVx[i - 1] = sVx.a[i - 1] - sVx.b[i - 1] * sVx.H1 - sVx.c[i - 1] * sVx.H2;
            sVx.H2 = sVx.H1;
            sVx.H1 = pOutput->pVx[i - 1];

            pOutput->pVy[i - 1] = sVy.a[i - 1] - sVy.b[i - 1] * sVy.H1 - sVy.c[i - 1] * sVy.H2;
            sVy.H2 = sVy.H1;
            sVy.H1 = pOutput->pVy[i - 1];

            /*
            pOutput->pS[i - 1] = sS.a[i - 1] - sS.b[i - 1] * sS.H1 - sS.c[i - 1] * sS.H2;
            sS.H2 = sS.H1;
            sS.H1 = pOutput->pS[i - 1];
            */
          }

          m_nProgress++;
          ProgressBar->Position = m_nProgress;
          Application->ProcessMessages();
        }
      }

      /* Wer will schon Speicherlücken? */
      if(sX.a) delete [] sX.a;
      if(sX.b) delete [] sX.b;
      if(sX.c) delete [] sX.c;

      if(sY.a) delete [] sY.a;
      if(sY.b) delete [] sY.b;
      if(sY.c) delete [] sY.c;

      if(sA.a) delete [] sA.a;
      if(sA.b) delete [] sA.b;
      if(sA.c) delete [] sA.c;

      if(sVx.a) delete [] sVx.a;
      if(sVx.b) delete [] sVx.b;
      if(sVx.c) delete [] sVx.c;

      if(sVy.a) delete [] sVy.a;
      if(sVy.b) delete [] sVy.b;
      if(sVy.c) delete [] sVy.c;

      /*
      if(sS.a) delete [] sS.a;
      if(sS.b) delete [] sS.b;
      if(sS.c) delete [] sS.c;
      */
    }
  }

  return rc;
}

void __fastcall THPForm::OnLambdaKeyPress(TObject *Sender, char &Key)
{
  if((Key >= '0' && Key <= '9') ||
     Key == VK_BACK             ||
     Key == VK_RETURN           ||
     Key == '.') {}
  else Key = 0;
}
//---------------------------------------------------------------------------

