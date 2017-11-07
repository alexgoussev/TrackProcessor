#include <math.h>

#include "SK42WGS84.h"

static const double Pi = 3.14159265358979;                                         /* „исло ѕи */
static const double ro = 206264.8062;                                              /* „исло угловых секунд в радиане */

/* Ёллипсоид GRS80 (WGS84) */
static const double aW = 6378137.0;                                                 /* Ёкваториальный радиус */
static const double bW = 6356752.3142;                                             /* ѕол€рный радиус */
static const double bWaW = bW / aW;
static const double eW = sqrt(1.0  - bWaW * bWaW);
static const double e2W = eW * eW;
static const double e4W = e2W * e2W;
static const double e6W = e4W * e2W;
static const double e1W = (1.0 - sqrt(1.0 - e2W)) / (1 + sqrt(1.0 - e2W));
static const double e1W2 = e1W * e1W;
static const double e1W3 = e1W2 * e1W;
static const double e1W4 = e1W2 * e1W2;
static const double e_2W = eW * eW / (1.0 - eW * eW);
static const double nW = (aW - bW) / (aW + bW);
static const double n2W = nW * nW;
static const double n3W = n2W * nW;
static const double n4W = n2W * n2W;
static const double n23W = n2W * (1.0 - nW);
static const double n45W = n4W * (1.0 - nW);
static const double k0W = 0.9996;
static const double aWnW = aW * nW;
static const double aWn2W = aWnW * nW;
static const double aWn3W = aWn2W * nW;
static const double aWn4W = aWn3W * nW;
static const double A_ = aW * (1.0 - nW) * (1.0 + 5.0 * n2W / 4.0 + 81.0 * n4W / 64.0);
static const double B_ = (3.0 * aWnW / 2.0) * (1 - nW + (7.0 / 8.0) * n23W + (55.0 / 64.0) * n45W);
static const double C_ = (15.0 * aWn2W / 16.0) * (1.0 - nW + (3.0 / 4.0) * n23W);
static const double D_ = (35.0 * aWn3W / 48.0) * (1.0 - nW + (11.0 / 16.0) * n23W);
static const double E_ = (315.0 * aWn4W / 512.0) * (1.0 - nW);
static const double J1 = (3.0 * e1W / 2.0 - 27.0 * e1W3 / 32.0);
static const double J2 = (21.0 * e1W2 / 16.0 - 55.0 * e1W4 / 32.0);
static const double J3 = (151.0 * e1W3 / 96.0);
static const double J4 = (1097.0 * e1W4 / 512.0);

double rho(double Lat)
{
  double LatR = Lat * Pi / 180.0;
  double SinLat = sin(LatR);
  double tmp = 1 - e2W * SinLat * SinLat;
  return aW * (1 - e2W) / sqrt(tmp * tmp * tmp);
}

double nu(double LatR)
{
  double SinLat = sin(LatR);
  double tmp = 1 - e2W * SinLat * SinLat;
  return aW / sqrt(tmp);
}

double S(double LatR)
{
  return A_ * LatR - B_ * sin(2.0 * LatR) + C_ * sin(4.0 * LatR) - D_ * sin(6.0 * LatR) + E_ * sin(8.0 * LatR);
}

double K1(double LatR)
{
  return S(LatR) * k0W;
}

double K2(double LatR)
{
  return k0W * nu(LatR) * sin(2.0 * LatR) / 4.0;
}

double K3(double LatR)
{
  double CosLat = cos(LatR);
  double Cos2Lat = CosLat * CosLat;
  double TanLat = tan(LatR);
  return (k0W * nu(LatR) * sin(LatR) * Cos2Lat * CosLat / 24.0) * (5.0 - TanLat * TanLat + 9.0 * e_2W * Cos2Lat + 4.0 * e_2W * Cos2Lat * Cos2Lat);
}

double K4(double LatR)
{
  return k0W * nu(LatR) * cos(LatR);
}

double K5(double LatR)
{
  double CosLat = cos(LatR);
  double Cos2Lat = CosLat * CosLat;
  double TanLat = tan(LatR);
  return (k0W * nu(LatR) * Cos2Lat * CosLat / 6.0) * (1.0 - TanLat * TanLat + e_2W * Cos2Lat);
}

void WGS84ToUTM(double Lat, double Lon, double Lon0, double *X, double *Y)
{
  double LatR = Lat * Pi / 180.0;
  double LonR = Lon * Pi / 180.0;
  double Lon0R = Lon0 * Pi / 180.0;
  double p = LonR - Lon0R;
  double p2 = p * p;
  double p3 = p2 * p;
  double p4 = p2 * p2;

  *Y = K4(LatR) * p + K5(LatR) * p3 + 500000;
  *X = K1(LatR) + K2(LatR) * p2 + K3(LatR) * p4;
}

double mu(double X)
{
  return (X / k0W) / (aW * (1.0 - e2W / 4.0 - 3.0 * e4W / 64.0 - 5.0 * e6W / 256.0));
}

double fp(double X)
{
  double _mu = mu(X);
  return _mu + J1 * sin(2.0 * _mu) + J2 * sin(4 * _mu) + J3 * sin(6.0 * _mu) + J4 * sin(8.0 * _mu);
}

void UTMToWGS84(double X, double Y, double Lon0, double *Lat, double *Lon)
{
  Y -= 500000;
  double Lon0R = Lon0 * Pi / 180.0;
  double _fp = fp(X);
  double TanFP = tan(_fp);
  double CosFP = cos(_fp);
  double SinFP = sin(_fp);
  double tmp = (1.0 - e2W * SinFP * SinFP);
  double C1 = e_2W * CosFP * CosFP;
  double C1_2 = C1 * C1;
  double T1 = TanFP * TanFP;
  double T1_2 = T1 * T1;
  double R1 = aW * (1.0 - e2W) / sqrt(tmp * tmp * tmp);
  double N1 = aW / sqrt(tmp);
  double D = Y / (N1 * k0W);
  double D2 = D * D;
  double D3 = D2 * D;
  double D4 = D3 * D;
  double D5 = D4 * D;
  double D6 = D5 * D;
  double Q1 = N1 * tan(_fp) / R1;
  double Q2 = D2 / 2.0;
  double Q3 = (5.0 + 3.0 * T1 + 10.0 * C1_2 - 9.0 * e_2W) * D4 / 24.0;
  double Q4 = (61.0 + 90.0 * T1 + 298.0 * C1 + 45.0 * T1_2 - 3.0 * C1_2 - 252.0 * e_2W) * D6 / 720.0;
  double Q5 = D;
  double Q6 = (1.0 + 2.0 * T1 + C1) * D3 / 6.0;
  double Q7 = (5.0 - 2.0 * C1 + 28.0 * T1 - 3.0 * C1_2 + 8.0 * e_2W + 24.0 * T1_2) * D5 / 120.0;

  double LatR = _fp - Q1 * (Q2 - Q3 + Q4);
  double LonR = Lon0R + (Q5 - Q6 + Q7) / CosFP;

  *Lat = LatR * 180.0 / Pi;
  *Lon = LonR * 180.0 / Pi;
}

int UTMZone(double Lon)
{
  return 31 + (int)(Lon / 6.0);
}

double UTMZoneCM(int Zone)
{
  return (double)(6 * Zone - 183);
}

double UTMCM(double Lon)
{
  return (double)(6 * UTMZone(Lon) - 183);
}








