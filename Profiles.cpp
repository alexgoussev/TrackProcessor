//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.IniFiles.hpp>
#include <Vcl.Imaging.GIFImg.hpp>
#pragma hdrstop

#include "Spline.h"
#include "ImgSizeForm.h"
#include "Profiles.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProfile *Profile;
//---------------------------------------------------------------------------
__fastcall TProfile::TProfile(TComponent* Owner)
  : TForm(Owner)
{
  m_pAltMetafile = NULL;
  m_pSpdMetafile = NULL;
}

__fastcall TProfile::~TProfile()
{
  if(m_pAltMetafile) delete m_pAltMetafile;
  if(m_pSpdMetafile) delete m_pSpdMetafile;
}
//---------------------------------------------------------------------------
void __fastcall TProfile::FormResize(TObject *Sender)
{
  int ProfileHeight = ClientHeight / 2;
  AltPanel->Height = ProfileHeight;
  SpdPanel->Height = ProfileHeight;

  DrawMetafile(SpdImage->ClientWidth, SpdImage->ClientHeight,
               AltImage->ClientWidth, AltImage->ClientHeight);
  AltImage->Repaint();
  SpdImage->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TProfile::SetCurrentTrack(PGPXTrack value)
{
  m_pCurrentTrack = value;
  DrawMetafile(SpdImage->ClientWidth, SpdImage->ClientHeight,
               AltImage->ClientWidth, AltImage->ClientHeight);
  AltImage->Repaint();
  SpdImage->Repaint();
}
PGPXTrack __fastcall TProfile::GetCurrentTrack()
{
  return m_pCurrentTrack;
}

void __fastcall TProfile::DrawMetafile(int SpdWidth, int SpdHeight, int AltWidth, int AltHeight)
{
  if(m_pAltMetafile) delete m_pAltMetafile;
  if(m_pSpdMetafile) delete m_pSpdMetafile;

  m_pSpdMetafile = new Graphics::TMetafile;
  m_pAltMetafile = new Graphics::TMetafile;
  
  DrawMetafile(m_pAltMetafile, m_pSpdMetafile, SpdWidth, SpdHeight, AltWidth, AltHeight);
}

void __fastcall TProfile::DrawMetafile(TMetafile *pAltMetafile, TMetafile *pSpdMetafile,
                                       int SpdWidth, int SpdHeight, int AltWidth, int AltHeight)
{
  pSpdMetafile->Width = SpdWidth; pSpdMetafile->Height = SpdHeight;
  pAltMetafile->Width = AltWidth; pAltMetafile->Height = AltHeight;

  TMetafileCanvas *pSpdCanvas = new TMetafileCanvas(pSpdMetafile, 0);
  TMetafileCanvas *pAltCanvas = new TMetafileCanvas(pAltMetafile, 0);

  pSpdCanvas->Pen->Color = clWhite;
  pSpdCanvas->Pen->Style = psSolid;
  pSpdCanvas->Brush->Style = bsSolid;
  pSpdCanvas->Brush->Color = clWhite;
  pSpdCanvas->Rectangle(0, 0, SpdWidth, SpdHeight);

  pSpdCanvas->Pen->Color = clBlack;
  pSpdCanvas->Pen->Width = 2;
  pSpdCanvas->MoveTo(LEFT_SPACE, TOP_SPACE);
  pSpdCanvas->LineTo(LEFT_SPACE, SpdHeight - BOTTOM_SPACE);
  pSpdCanvas->LineTo(SpdWidth - RIGHT_SPACE, SpdHeight - BOTTOM_SPACE);

  pAltCanvas->Pen->Color = clWhite;
  pAltCanvas->Pen->Style = psSolid;
  pAltCanvas->Brush->Style = bsSolid;
  pAltCanvas->Brush->Color = clWhite;
  pAltCanvas->Rectangle(0, 0, AltWidth, AltHeight);

  pAltCanvas->Pen->Color = clBlack;
  pAltCanvas->Pen->Width = 2;
  pAltCanvas->MoveTo(LEFT_SPACE, TOP_SPACE);
  pAltCanvas->LineTo(LEFT_SPACE, AltHeight - BOTTOM_SPACE);
  pAltCanvas->LineTo(AltWidth - RIGHT_SPACE, AltHeight - BOTTOM_SPACE);

  if(m_pCurrentTrack)
  {
	double *pD, *pS, *pA = NULL;

    pD = new double [m_pCurrentTrack->PointCount];
    pS = new double [m_pCurrentTrack->PointCount];

    if(!m_pCurrentTrack->m_bNoAlt)
      pA = new double [m_pCurrentTrack->PointCount];

    double MinAlt = 99999.0, MaxAlt = -99999.0, MinSpeed = 99999.0, MaxSpeed = -99999.0;

    for(int i = 0; i < m_pCurrentTrack->PointCount; i++)
    {
      PGPXTrackPoint p = m_pCurrentTrack->Points[i];

      pD[i] = p->m_dDistance / 1000.0;
      pS[i] = p->m_dSpeed * 3.6;

      if(pS[i] < MinSpeed) MinSpeed = pS[i];
      if(pS[i] > MaxSpeed) MaxSpeed = pS[i];

      if(pA)
      {
        pA[i] = p->m_dAlt;

        if(pA[i] < MinAlt) MinAlt = pA[i];
        if(pA[i] > MaxAlt) MaxAlt = pA[i];
      }
    }

    int FieldWidth = SpdWidth - LEFT_SPACE - RIGHT_SPACE;
    int nTextBottom = SpdHeight - BOTTOM_SPACE + 3;
    pSpdCanvas->Font->Size = 12;
    pSpdCanvas->Font->Style = TFontStyles() << fsBold;
    pSpdCanvas->Font->Color = clBlack;

    AnsiString as = AnsiString("Speed vs Distance");
    pSpdCanvas->TextOut(LEFT_SPACE + (FieldWidth - pSpdCanvas->TextWidth(as)) / 2, TOP_SPACE - 15, as);

    //Horizontal
    pSpdCanvas->Font->Size = 10;
    pSpdCanvas->Font->Style = TFontStyles();

    as = FormatFloat("0.00", pD[0]);
    pSpdCanvas->TextOut(LEFT_SPACE, nTextBottom, as);

    as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1]);
    pSpdCanvas->TextOut(LEFT_SPACE + FieldWidth - pSpdCanvas->TextWidth(as) - 1, nTextBottom, as);

    as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] / 2.0);
    pSpdCanvas->TextOut(LEFT_SPACE + (FieldWidth - pSpdCanvas->TextWidth(as)) / 2, nTextBottom, as);

    as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] / 4.0);
    pSpdCanvas->TextOut(LEFT_SPACE + (FieldWidth / 2 - pSpdCanvas->TextWidth(as)) / 2, nTextBottom, as);

    as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] * 3.0 / 4.0);
    pSpdCanvas->TextOut(LEFT_SPACE + (FieldWidth * 3 / 2 - pSpdCanvas->TextWidth(as)) / 2, nTextBottom, as);

    as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] / 8.0);
    pSpdCanvas->TextOut(LEFT_SPACE + (FieldWidth / 4 - pSpdCanvas->TextWidth(as)) / 2, nTextBottom, as);

    as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] * 3.0 / 8.0);
    pSpdCanvas->TextOut(LEFT_SPACE + (FieldWidth * 3 / 4 - pSpdCanvas->TextWidth(as)) / 2, nTextBottom, as);

    as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] * 5.0 / 8.0);
    pSpdCanvas->TextOut(LEFT_SPACE + (FieldWidth * 5 / 4 - pSpdCanvas->TextWidth(as)) / 2, nTextBottom, as);

    as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] * 7.0 / 8.0);
    pSpdCanvas->TextOut(LEFT_SPACE + (FieldWidth * 7 / 4 - pSpdCanvas->TextWidth(as)) / 2, nTextBottom, as);

    //Vertical
    nTextBottom = SpdHeight - BOTTOM_SPACE - 15;

    as = FormatFloat("0.00", MinSpeed);
    pSpdCanvas->TextOut(LEFT_SPACE - pSpdCanvas->TextWidth(as) - 5, nTextBottom, as);

    as = FormatFloat("0.00", MaxSpeed);
    pSpdCanvas->TextOut(LEFT_SPACE - pSpdCanvas->TextWidth(as) - 5, TOP_SPACE, as);

    as = FormatFloat("0.00", MinSpeed + (MaxSpeed - MinSpeed) / 2.0);
    pSpdCanvas->TextOut(LEFT_SPACE - pSpdCanvas->TextWidth(as) - 5, TOP_SPACE + (nTextBottom - TOP_SPACE) / 2, as);

    as = FormatFloat("0.00", MinSpeed + (MaxSpeed - MinSpeed) / 4.0);
    pSpdCanvas->TextOut(LEFT_SPACE - pSpdCanvas->TextWidth(as) - 5, TOP_SPACE + (nTextBottom - TOP_SPACE) * 3 / 4, as);

    as = FormatFloat("0.00", MinSpeed + (MaxSpeed - MinSpeed) * 3.0 / 4.0);
    pSpdCanvas->TextOut(LEFT_SPACE - pSpdCanvas->TextWidth(as) - 5, TOP_SPACE + (nTextBottom - TOP_SPACE) / 4, as);

    if(pA)
    {
      FieldWidth = SpdWidth - LEFT_SPACE - RIGHT_SPACE;
      nTextBottom = SpdHeight - BOTTOM_SPACE + 3;
      pAltCanvas->Font->Size = 12;
      pAltCanvas->Font->Style = TFontStyles() << fsBold;
      pAltCanvas->Font->Color = clBlack;

      as = AnsiString("Altitude vs Distance");
      pAltCanvas->TextOut(LEFT_SPACE + (FieldWidth - pAltCanvas->TextWidth(as)) / 2, TOP_SPACE - 15, as);

      //Horizontal
      pAltCanvas->Font->Size = 10;
      pAltCanvas->Font->Style = TFontStyles();

      as = FormatFloat("0.00", pD[0]);
      pAltCanvas->TextOut(LEFT_SPACE, nTextBottom, as);

      as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1]);
      pAltCanvas->TextOut(LEFT_SPACE + FieldWidth - pAltCanvas->TextWidth(as) - 1, nTextBottom, as);

      as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] / 2.0);
      pAltCanvas->TextOut(LEFT_SPACE + (FieldWidth - pAltCanvas->TextWidth(as)) / 2, nTextBottom, as);

      as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] / 4.0);
      pAltCanvas->TextOut(LEFT_SPACE + (FieldWidth / 2 - pAltCanvas->TextWidth(as)) / 2, nTextBottom, as);

      as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] * 3.0 / 4.0);
      pAltCanvas->TextOut(LEFT_SPACE + (FieldWidth * 3 / 2 - pAltCanvas->TextWidth(as)) / 2, nTextBottom, as);

      as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] / 8.0);
      pAltCanvas->TextOut(LEFT_SPACE + (FieldWidth / 4 - pAltCanvas->TextWidth(as)) / 2, nTextBottom, as);

      as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] * 3.0 / 8.0);
      pAltCanvas->TextOut(LEFT_SPACE + (FieldWidth * 3 / 4 - pAltCanvas->TextWidth(as)) / 2, nTextBottom, as);

      as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] * 5.0 / 8.0);
      pAltCanvas->TextOut(LEFT_SPACE + (FieldWidth * 5 / 4 - pAltCanvas->TextWidth(as)) / 2, nTextBottom, as);

      as = FormatFloat("0.00", pD[m_pCurrentTrack->PointCount - 1] * 7.0 / 8.0);
      pAltCanvas->TextOut(LEFT_SPACE + (FieldWidth * 7 / 4 - pAltCanvas->TextWidth(as)) / 2, nTextBottom, as);

      //Vertical
      nTextBottom = AltHeight - BOTTOM_SPACE - 15;

      as = FormatFloat("0.0", MinAlt);
      pAltCanvas->TextOut(LEFT_SPACE - pAltCanvas->TextWidth(as) - 5, nTextBottom, as);

      as = FormatFloat("0.0", MaxAlt);
      pAltCanvas->TextOut(LEFT_SPACE - pAltCanvas->TextWidth(as) - 5, TOP_SPACE, as);

      as = FormatFloat("0.0", MinAlt + (MaxAlt - MinAlt) / 2.0);
      pAltCanvas->TextOut(LEFT_SPACE - pAltCanvas->TextWidth(as) - 5, TOP_SPACE + (nTextBottom - TOP_SPACE) / 2, as);

      as = FormatFloat("0.0", MinAlt + (MaxAlt - MinAlt) / 4.0);
      pAltCanvas->TextOut(LEFT_SPACE - pAltCanvas->TextWidth(as) - 5, TOP_SPACE + (nTextBottom - TOP_SPACE) * 3 / 4, as);

      as = FormatFloat("0.0", MinAlt + (MaxAlt - MinAlt) * 3.0 / 4.0);
      pAltCanvas->TextOut(LEFT_SPACE - pAltCanvas->TextWidth(as) - 5, TOP_SPACE + (nTextBottom - TOP_SPACE) / 4, as);
    }

    AkimaSpline *pSpeedSpl = NULL, *pAltSpl = NULL;

    pSpeedSpl = new AkimaSpline(pD, pS, m_pCurrentTrack->PointCount);
    if(pA) pAltSpl = new AkimaSpline(pD, pA, m_pCurrentTrack->PointCount);
    
    int MinPixX = LEFT_SPACE + 1, MaxPixX = SpdWidth - RIGHT_SPACE - 1;
    int MinPixYs = BOTTOM_SPACE + 1, MaxPixYs = SpdHeight - TOP_SPACE - 1;
    int MinPixYa = BOTTOM_SPACE + 1, MaxPixYa = AltHeight - TOP_SPACE - 1;

    pSpdCanvas->Pen->Color = clBlue; pSpdCanvas->Pen->Width = 1;
    pAltCanvas->Pen->Color = clRed; pAltCanvas->Pen->Width = 1;

    for(int i = MinPixX; i <= MaxPixX; i++)
    {
      double Val = PixToVal(i, MaxPixX, MinPixX, pD[m_pCurrentTrack->PointCount - 1], pD[0]);
      double Speed;

      pSpeedSpl->Interpolate(Val, Speed);
      if(Speed < MinSpeed) Speed = MinSpeed;
	  else if(Speed > MaxSpeed) Speed = MaxSpeed;

	  int PixY = 0;
	  if (0 != MaxSpeed) {
		PixY = ValToPix(Speed, MaxSpeed, MinSpeed, MaxPixYs, MinPixYs);
		PixY = SpdHeight - PixY;
	  }

      if(i == MinPixX)
        pSpdCanvas->MoveTo(i, PixY);
      else
        pSpdCanvas->LineTo(i, PixY);

      if(pAltSpl)
      {
        double Alt;

        pAltSpl->Interpolate(Val, Alt);
        if(Alt < MinAlt) Alt = MinAlt;
		else if(Alt > MaxAlt) Alt = MaxAlt;

		PixY = 0;
		if (0 != MaxAlt || 0 != MinAlt) {
		  PixY = ValToPix(Alt, MaxAlt, MinAlt, MaxPixYa, MinPixYa);
		  PixY = AltHeight - PixY;
		}

		if(i == MinPixX)
		  pAltCanvas->MoveTo(i, PixY);
        else
          pAltCanvas->LineTo(i, PixY);
      }
    }

    if(pA)
    {
      delete pAltSpl;
      delete [] pA;
    }

    delete pSpeedSpl;
    delete [] pD;
    delete [] pS;
  }

  delete pSpdCanvas;
  delete pAltCanvas;
}

int __fastcall TProfile::ValToPix(double Val, double MaxVal, double MinVal, int MaxPix, int MinPix)
{
  return (int)((double)MinPix + ((double)MaxPix - (double)MinPix) * (Val - MinVal) / (MaxVal - MinVal));
}

double __fastcall TProfile::PixToVal(int Pix, int MaxPix, int MinPix, double MaxVal, double MinVal)
{
  return MinVal + (MaxVal - MinVal) * ((double)Pix - (double)MinPix) / ((double)MaxPix - (double)MinPix);
}

void __fastcall TProfile::SpdImagePaint(TObject *Sender)
{
  SpdImage->Canvas->Draw(0,0, m_pSpdMetafile);
}
//---------------------------------------------------------------------------

void __fastcall TProfile::AltImagePaint(TObject *Sender)
{
  AltImage->Canvas->Draw(0,0, m_pAltMetafile);
}
//---------------------------------------------------------------------------

void __fastcall TProfile::SaveAltActionExecute(TObject *Sender)
{
  SaveImage(eSaveAlt);  
}
//---------------------------------------------------------------------------

void __fastcall TProfile::SaveAltActionUpdate(TObject *Sender)
{
  if(m_pCurrentTrack && !m_pCurrentTrack->m_bNoAlt)
    SaveAltAction->Enabled = true;
  else
    SaveAltAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TProfile::SaveSpdActionExecute(TObject *Sender)
{
  SaveImage(eSaveSpd);
}
//---------------------------------------------------------------------------


void __fastcall TProfile::SaveBothActionExecute(TObject *Sender)
{
  SaveImage(eSaveBoth);
}
//---------------------------------------------------------------------------

void __fastcall TProfile::SaveBothActionUpdate(TObject *Sender)
{
  if(m_pCurrentTrack && !m_pCurrentTrack->m_bNoAlt)
    SaveBothAction->Enabled = true;
  else
    SaveBothAction->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TProfile::SaveSpdActionUpdate(TObject *Sender)
{
  if(m_pCurrentTrack)
    SaveSpdAction->Enabled = true;
  else
    SaveSpdAction->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TProfile::SaveImage(SAVEIMG eSaveType)
{
  TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
  int nImgWidth = pini->ReadInteger("Profiles", "ImgWidth", 800);
  int nImgHeight = pini->ReadInteger("Profiles", "ImgHeight", 300);
  AnsiString asImgPath = pini->ReadString("Path", "ImgPath", "");
  delete pini;

  ImgSize->Width->Text = AnsiString(nImgWidth);
  ImgSize->Height->Text = AnsiString(nImgHeight);

  if(ImgSize->ShowModal() == mrOk)
  {
    SaveProfile->InitialDir = asImgPath;
    
    if(SaveProfile->Execute())
    {
      TIniFile *pini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
      pini->WriteInteger("Profiles", "ImgWidth", ImgSize->m_nWidth);
      pini->WriteInteger("Profiles", "ImgHeight", ImgSize->m_nHeight);
      pini->WriteString("Path", "ImgPath", ExtractFileDir(SaveProfile->FileName));
      delete pini;

      TCursor Save_Cursor = Screen->Cursor;
      Screen->Cursor = crHourGlass;

      TMetafile *pAltMetafile = new Graphics::TMetafile;
      TMetafile *pSpdMetafile = new Graphics::TMetafile;

      DrawMetafile(pAltMetafile, pSpdMetafile,
                   ImgSize->m_nWidth, ImgSize->m_nHeight,
                   ImgSize->m_nWidth, ImgSize->m_nHeight);

      Graphics::TBitmap *pBitmap = new Graphics::TBitmap();

      pBitmap->HandleType = bmDIB;
      pBitmap->Width = ImgSize->m_nWidth;

      if(eSaveType == eSaveBoth)
      {
        pBitmap->Height = 2 * ImgSize->m_nHeight;
        pBitmap->Canvas->Draw(0, 0, pAltMetafile);
        pBitmap->Canvas->Draw(0, ImgSize->m_nHeight, pSpdMetafile);
      }
      else
      {
        pBitmap->Height = ImgSize->m_nHeight;
        if(eSaveType == eSaveAlt) pBitmap->Canvas->Draw(0, 0, pAltMetafile);
        else if(eSaveType == eSaveSpd) pBitmap->Canvas->Draw(0, 0, pSpdMetafile);
      }

      TGIFImage *pGIFImage = new TGIFImage;

      pGIFImage->Assign(pBitmap);
      pGIFImage->SaveToFile(SaveProfile->FileName);

      delete pBitmap;
      delete pGIFImage;
      delete pAltMetafile;
      delete pSpdMetafile;

      Screen->Cursor = Save_Cursor;
    }
  }
}
