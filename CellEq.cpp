//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <System.IniFiles.hpp>
#pragma hdrstop

#include "SK42WGS84\\SK42WGS84.h"
#include "CellEq.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCellEqualizer *CellEqualizer;
//---------------------------------------------------------------------------
__fastcall TCellEqualizer::TCellEqualizer(TComponent* Owner)
  : TForm(Owner)
{
  m_pResultTrack = m_pTrack = NULL;
  m_dCellSize = 0.0;

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  m_pAltitude->Checked = pini->ReadBool("CellEq", "Altitude", false);
  m_pCoordinates->Checked = pini->ReadBool("CellEq", "Coordinates", true);
  m_pMovDir->Checked = pini->ReadBool("CellEq", "MovementDir", true);
  m_pCellSize->Text = AnsiString(pini->ReadFloat("CellEq", "CellSize", 15));
  m_pMaxCourseDiff->Text = AnsiString(pini->ReadInteger("CellEq", "MaxCourseDiff", 60));
  m_pKType->ItemIndex = pini->ReadInteger("CellEq", "AvgMethod", 2);
  delete pini;

  m_pMaxCourseDiff->Enabled = m_pMovDir->Checked;
}
//---------------------------------------------------------------------------

__fastcall TCellEqualizer::~TCellEqualizer()
{
}

void __fastcall TCellEqualizer::SetTrack(PGPXTrack value)
{
  if(m_pTrack != value) {
    m_pTrack = value;
  }
}

PGPXTrack __fastcall TCellEqualizer::GetTrack()
{
  return m_pResultTrack;
}

double __fastcall TCellEqualizer::FSquare(double x)
{
  return x * x;
}

double __fastcall TCellEqualizer::FCube(double x)
{
  return x * x * x; 
}

double __fastcall TCellEqualizer::KernelBiQuadratic(double r)
{
  return FSquare(1.0 - FSquare(r));
}

double __fastcall TCellEqualizer::KernelTriCube(double r)
{
  return FCube(1.0 - FCube(r));
}

double __fastcall TCellEqualizer::KernelGauss(double r)
{
  return exp(-0.5 * FSquare(r));
}

void __fastcall TCellEqualizer::Process()
{
  if(m_pTrack && m_dCellSize > 0.0)
  {
    PGPXTrackPoint *pPointsList = new PGPXTrackPoint[m_pTrack->PointCount];

    TTrackCells *pTrackCells = new TTrackCells(m_pTrack, m_dCellSize);
    m_pResultTrack = new GPXTrack;

    m_pProgressBar->Max = pTrackCells->CellsX * pTrackCells->CellsY + m_pTrack->PointCount;
    m_pProgressBar->Position = 0;
    int nPos = 0;

    for(int i = 0; i < pTrackCells->CellsX; i++)
    {
      for(int j = 0; j < pTrackCells->CellsY; j++)
      {
        for(int k = 0; k < pTrackCells->PointCount[i][j]; k++)
        {
          PGPXTrackPoint pPoint = pTrackCells->Point[i][j][k];
          int nPointIndex = pTrackCells->PointIndex[i][j][k];
          pPointsList[nPointIndex] = m_pResultTrack->NewPoint;
          pPointsList[nPointIndex]->SetPoint(pPoint);

          double x, y, c = pPoint->m_dCourse, avgx = 0.0, avgy = 0.0, avga = 0.0, sumw = 0.0;
          pPoint->GetdProj(&x, &y);

          int iFrom = i - 1, iTo = i + 2;
          int jFrom = j - 1, jTo = j + 2;

          if(iFrom < 0) iFrom = 0;
          if(iTo > pTrackCells->CellsX) iTo = pTrackCells->CellsX;
          if(jFrom < 0) jFrom = 0;
          if(jTo > pTrackCells->CellsY) jTo = pTrackCells->CellsY;

          for(int _i = iFrom; _i < iTo; _i++)
          {
            for(int _j = jFrom; _j < jTo; _j++)
            {
              for(int _k = 0; _k < pTrackCells->PointCount[_i][_j]; _k++)
              {
                PGPXTrackPoint _pPoint = pTrackCells->Point[_i][_j][_k];
                double _x, _y, _c = _pPoint->m_dCourse;
                bool bDirection = true;

                if(m_bMoveDir)
                {
                  ArrangeCourses(&c, &_c);
                  bDirection = (fabs(c - _c) >= (double)m_nMaxCourseDiff);
                }

                if(bDirection)
                {
                  _pPoint->GetdProj(&_x, &_y);

                  double r = sqrt(FSquare(x - _x) + FSquare(y - _y));

                  if(r < m_dCellSize)
                  {
                    double w = Weight(r);

                    avgx += _x * w;
                    avgy += _y * w;
                    avga += _pPoint->m_dAlt * w;
                    sumw += w;
                  }
                }
              }
            }
          }

          if(sumw > 0.0)
          {
            avgx /= sumw;
            avgy /= sumw;
            avga /= sumw;

            if(m_bCoordinates)
              pPointsList[nPointIndex]->SetdProj(avgx, avgy);

            if(m_bAltitude)
              pPointsList[nPointIndex]->m_dAlt = avga;
          }
        }

        nPos++;
        m_pProgressBar->Position = nPos;
        Application->ProcessMessages();
      }
    }

    for(int i = 0; i < m_pTrack->PointCount; i++)
    {
      m_pResultTrack->AddPoint(pPointsList[i]);

      nPos++;
      m_pProgressBar->Position = nPos;
      Application->ProcessMessages();
    }

    delete pTrackCells;
    delete [] pPointsList;
    m_pProgressBar->Position = 0;
  }
}

double __fastcall TCellEqualizer::Weight(double r)
{
  double w = 1.0;

  if(m_eKType == ktBiQuadratic)
    w = FSquare(1.0 - FSquare(r / m_dCellSize));
  else if(m_eKType == ktTriCube)
    w = FCube(1.0 - FCube(r / m_dCellSize));
  else if(m_eKType == ktGauss)
    w = exp(-0.5 * FSquare(r / m_dCellSize));

  return w;
}

void __fastcall TCellEqualizer::GoClick(TObject *Sender)
{
  m_dCellSize = m_pCellSize->Text.ToDouble();
  m_bCoordinates = m_pCoordinates->Checked;
  m_bAltitude = m_pAltitude->Checked;
  m_bMoveDir = m_pMovDir->Checked;
  m_eKType = (eKernelType)m_pKType->ItemIndex;
  m_nMaxCourseDiff = m_pMaxCourseDiff->Text.ToInt();

  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  pini->WriteBool("CellEq", "Altitude", m_bAltitude);
  pini->WriteBool("CellEq", "Coordinates", m_bCoordinates);
  pini->WriteBool("CellEq", "MovementDir", m_bMoveDir);
  pini->WriteFloat("CellEq", "CellSize", m_dCellSize);
  pini->WriteInteger("CellEq", "AvgMethod", (int)m_eKType);
  pini->WriteInteger("CellEq", "MaxCourseDiff", m_nMaxCourseDiff);
  delete pini;

  TCursor Save_Cursor = Screen->Cursor;
  Screen->Cursor = crHourGlass;
  
  Process();

  Screen->Cursor = Save_Cursor;
}
//---------------------------------------------------------------------------

void __fastcall TCellEqualizer::CellSizeKeyPress(TObject *Sender,
      char &Key)
{
  if((Key >= '0' && Key <= '9') ||
     Key == VK_BACK             ||
     Key == VK_RETURN           ||
     Key == '.') {}
  else Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCellEqualizer::MovDirClick(TObject *Sender)
{
  m_pMaxCourseDiff->Enabled = m_pMovDir->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TCellEqualizer::MaxCourseDiffKeyPress(TObject *Sender,
      char &Key)
{
  if((Key >= '0' && Key <= '9') ||
     Key == VK_BACK             ||
     Key == VK_RETURN) {}
  else Key = 0;
}
//---------------------------------------------------------------------------

