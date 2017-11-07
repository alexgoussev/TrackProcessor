//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "Median.h"
#include "MedianFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TMedianForm *MedianForm;
//---------------------------------------------------------------------------
__fastcall TMedianForm::TMedianForm(TComponent* Owner)
  : TForm(Owner)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  Altitude->Checked = pini->ReadBool("Median", "Altitude", false);
  Speed->Checked = pini->ReadBool("Median", "Speed", false);
  Coordinates->Checked = pini->ReadBool("Median", "Coordinates", true);
  UseRank->Checked = pini->ReadBool("Median", "Rank", false);
  double dRank = pini->ReadFloat("Median", "RankCoeff", 0.65);
  int nAperture = pini->ReadInteger("Median", "Aperture", 3);
  delete pini;

  RankCoeff->Text = AnsiString(dRank);
  Aperture->Value = nAperture;
}
//---------------------------------------------------------------------------
void __fastcall TMedianForm::UseRankClick(TObject *Sender)
{
  if(UseRank->Checked) RankCoeff->Enabled = true;
  else RankCoeff->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMedianForm::GoClick(TObject *Sender)
{
  if(!Coordinates->Checked &&
     !Altitude->Checked    &&
     !Speed->Checked)
    MessageBox(Handle, TEXT("Nothing to do! Check at least one of Coordinates, Altitude or Speed"), TEXT("Warning"), MB_OK | MB_ICONSTOP);
  else if(m_pCurrentTrack)
  {
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;

    Median *pMedian = new Median;
    PGPXTrack pOriginalTrack = new GPXTrack(m_pCurrentTrack);
    int nAperture = Aperture->Value;
    int nAperture_2 = nAperture / 2;
    bool bAltitude = Altitude->Checked;
    bool bSpeed = Speed->Checked;
    bool bCoordinates = Coordinates->Checked;

    pMedian->Average = UseRank->Checked;
    if(pMedian->Average) pMedian->AverageFactor = RankCoeff->Text.ToDouble();

    TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    if(Altitude->Enabled) pini->WriteBool("Median", "Altitude", bAltitude);
    if(Speed->Enabled) pini->WriteBool("Median", "Speed", bSpeed);
    pini->WriteBool("Median", "Coordinates", bCoordinates);
    pini->WriteBool("Median", "Rank", UseRank->Checked);
    pini->WriteFloat("Median", "RankCoeff", RankCoeff->Text.ToDouble());
    pini->WriteInteger("Median", "Aperture", nAperture);
    delete pini;

    double *X = new double[nAperture];
    double *Y = new double[nAperture];
    double *A = new double[nAperture];
    double *V = new double[nAperture];
    double *T = new double[nAperture];

    ProgressBar->Max = m_pCurrentTrack->PointCount;
    ProgressBar->Position = 0;

    Go->Enabled = false;
    Exit->Enabled = false;

    PGPXTrackPoint pFirstPoint = pOriginalTrack->Points[0];
    PGPXTrackPoint pLastPoint = pOriginalTrack->Points[pOriginalTrack->PointCount - 1];

    for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
    {
      for(int j = 0; j < nAperture; j++)
      {
        int k = i - nAperture_2 + j;

        if(k <= 0)
        {
          pFirstPoint->GetdProj(&X[j], &Y[j]);
          A[j] = pFirstPoint->m_dAlt;
          V[j] = pFirstPoint->m_dSpeed;
          T[j] = pFirstPoint->m_dCourse;
        }
        else if(k >= pOriginalTrack->PointCount - 1)
        {
          pLastPoint->GetdProj(&X[j], &Y[j]);
          A[j] = pLastPoint->m_dAlt;
          V[j] = pLastPoint->m_dSpeed;
          T[j] = pLastPoint->m_dCourse;
        }
        else
        {
          PGPXTrackPoint pPoint = pOriginalTrack->Points[k];
          pPoint->GetdProj(&X[j], &Y[j]);
          A[j] = pPoint->m_dAlt;
          V[j] = pPoint->m_dSpeed;
          T[j] = pPoint->m_dCourse;
        }
      }

      PGPXTrackPoint pPoint = m_pCurrentTrack->Points[i];

      if(bCoordinates)
        pPoint->SetdProj(pMedian->GetMedian(X, nAperture), pMedian->GetMedian(Y, nAperture));

      if(bAltitude) pPoint->m_dAlt = pMedian->GetMedian(A, nAperture);
      if(bSpeed)
      {
        pPoint->m_dSpeed = pMedian->GetMedian(V, nAperture);
        pPoint->m_dCourse = pMedian->GetMedian(T, nAperture);
      }

      ProgressBar->Position = i;
      Application->ProcessMessages();
    }

    m_pCurrentTrack->RecalcDistClimb();

    Go->Enabled = true;
    Exit->Enabled = true;
    ProgressBar->Position = 0;

    delete [] T;
    delete [] V;
    delete [] A;
    delete [] Y;
    delete [] X;
    delete pOriginalTrack;
    delete pMedian;

    Screen->Cursor = Save_Cursor;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMedianForm::FormShow(TObject *Sender)
{
  if(m_pCurrentTrack && !m_pCurrentTrack->m_bNoAlt)
    Altitude->Enabled = true;
  else
  {
    Altitude->Checked = false;
    Altitude->Enabled = false;
  }

  if(m_pCurrentTrack && !m_pCurrentTrack->m_bNoTime)
    Speed->Enabled = true;
  else
  {
    Speed->Checked = false;
    Speed->Enabled = false;
  }
}
//---------------------------------------------------------------------------

