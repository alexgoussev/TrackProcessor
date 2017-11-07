/*
 * Функции рабочей книги для преобразования
 * геодезических координат из координатной системы Пулково 1942
 * в координатную систему WGS84
 */

/*
 * Все угловые значения передаются и возвращаются в градусах,
 * высоты передаются и возвращаются в метрах
 */

/*
 * Формулы и значения элементов трансформирования: ГОСТ Р 51794-2001 Российской Федерации
 * http://gis-lab.info/qa/wgs84-sk42-wgs84-formula.html
 */

#include <math.h>
#include <float.h>

#include "SK42WGS84.h"

static const double Pi = 3.14159265358979;                                         /* Число Пи */
static const double ro = 206264.8062;                                              /* Число угловых секунд в радиане */

/* Эллипсоид Красовского (Пулково 1942) */
static const double aP = 6378245.0;                                                /* Большая полуось */
static const double alP = 1.0 / 298.3;                                             /* Сжатие */
static const double e2P = (2.0  - 1.0 / 298.3) / 298.3;                            /* Квадрат эксцентриситета */

/* Эллипсоид GRS80 (WGS84) */
static const double aW = 6378137.0;                                                /* Большая полуось */
static const double alW = 1.0 / 298.257223563;                                     /* Сжатие */
static const double e2W = (2.0  - 1.0 / 298.257223563) / 298.257223563;            /* Квадрат эксцентриситета */

/* Вспомогательные значения для преобразования эллипсоидов */
static const double a = 6378191.0;
static const double e2 = (((2.0  - 1.0 / 298.3) / 298.3) + ((2.0  - 1.0 / 298.257223563) / 298.257223563)) / 2;
static const double da = 6378245.0 - 6378137.0;
static const double de2 = ((2.0  - 1.0 / 298.3) / 298.3) - ((2.0  - 1.0 / 298.257223563) / 298.257223563);

/* Линейные элементы трансформирования, в метрах */
double datum_trans_dx = -23.92;
double datum_trans_dy = 141.27;
double datum_trans_dz = 80.9;

/* Угловые элементы трансформирования, в секундах */
double datum_trans_wx = 0;
double datum_trans_wy = 0.35;
double datum_trans_wz = 0.82;

/* Дифференциальное различие масштабов */
double datum_trans_ms = 0.12 * 0.000001;

/* Коэфф. пересчета в plain  и обратно */
static const double kx[] = {6367558.4968, 16002.89, 66.9607, 0.3515, 1594561.25, 5336.535, 26.79,
                                 0.149, 672483.4, 811219.9, 5420.0, 10.6, 278194.0, 830174.0, 572434.0,
                                 16010.0, 109500.0, 574700.0, 863700.0, 398600.0};
static const double ky[] = {5.0, 10.0, 100000.0, 6378245.0, 21346.1415, 107.159, 0.5977, 1070204.16,
                                 2136826.66, 17.98, 11.99, 2700806.0, 1523417.0, 1327645.0, 21701,0, 79690.0,
                                 866190.0, 1730360.0, 945460.0};
static const double kb0[] = {0.00252588685, 0.0000149186, 0.00000011904};
static const double kdb[] = {0.251684631, 0.003369263, 0.000011276, 0.10500614, 0.04559916, 0.00228901,
                                  0.00002987, 0.042858, 0.025318, 0.014346, 0.001264, 0.01672, 0.0063, 0.01188,
                                  0.00328};
static const double kl[] = {1.0, 0.0033467108, 0.0000056002, 0.0000000187, 0.16778975, 0.16273586, 0.0005249,
                                 0.00000846, 0.0420025, 0.1487407, 0.005942, 0.000015, 0.01225, 0.09477, 0.03282,
                                 0.00034, 0.0038, 0.0524, 0.0482, 0.0032};

/* Вспомогательные функции */
void deltas(double Bd, double Ld, double H,
            double _da, double _de2,
            double dx, double dy, double dz,
            double wx, double wy, double wz, double ms,
            double *dB, double *dL, double *dH);
void values(double X, double Y, double *n, double *l, double *B0, double *dB);

void WGS84ToSK42(double Bd, double Ld, double H, double *Bd42, double *Ld42, double *H42)
{
  double dB;
  double dL;
  double dH;

  deltas(Bd, Ld, H, da, de2, datum_trans_dx, datum_trans_dy, datum_trans_dz,
         datum_trans_wx, datum_trans_wy, datum_trans_wz, datum_trans_ms, &dB, &dL, &dH);
  *Bd42 = (Bd + dB / 3600.0);
  *Ld42 = (Ld + dL / 3600.0);
  *H42 = (H + dH);
}

void SK42ToWGS84(double Bd, double Ld, double H, double *Bd84, double *Ld84, double *H84)
{
  double dB;
  double dL;
  double dH;

  deltas(Bd, Ld, H, -da, -de2, -datum_trans_dx, -datum_trans_dy, -datum_trans_dz,
         -datum_trans_wx, -datum_trans_wy, -datum_trans_wz, -datum_trans_ms, &dB, &dL, &dH);
  *Bd84 = (Bd + dB / 3600.0);
  *Ld84 = (Ld + dL / 3600.0);
  *H84 = (H + dH);
}

void deltas(double Bd, double Ld, double H,
            double _da, double _de2,
            double dx, double dy, double dz,
            double wx, double wy, double wz, double ms,
            double *dB, double *dL, double *dH)
{
  double B = Bd * Pi / 180.0;
  double L = Ld * Pi / 180.0;
  double sinl = sin(L);
  double cosl = cos(L);
  double sinb = sin(B);
  double cosb = cos(B);
  double sinb2 = sinb * sinb;
  double cos2b = cos(2.0 * B);
  double K = 1 - e2 * sinb2;
  double sqK = sqrt(K);
  double M = a * (1 - e2) / (K * sqK);
  double N = a / sqK;

  *dB = (ro / (M + H) * (N / a * e2 * sinb * cosb * _da + ((N * N) / (a * a) + 1) * N * sinb * cosb * _de2 / 2 -
         (dx * cosl + dy * sinl) * sinb + dz * cosb) - wx *
          sinl * (1 + e2 * cos2b) + wy * cosl * (1 + e2 * cos2b) -
          ro * ms * e2 * sinb * cosb);

  *dL = (ro / ((N + H) * cosb) * (-dx * sinl + dy * cosl) +
         sinb / cosb * (1 - e2) * (wx * cosl + wy * sinl) - wz);

  *dH = (-a / N * _da + N * sinb2 * _de2 / 2 + (dx * cosl + dy * sinl) * cosb +
         dz * sinb - N * e2 * sinb * cosb * (wx / ro * sinl - wy / ro * cosl) +
         ((a * a) / N + H) * ms);
}

int SK42ZoneNo(double Ld)
{
  return (int)((6.0 + Ld) / 6.0);
}

void SK42ToPlain(double Bd, double Ld, int nZone, double *X, double *Y)
{
  double B = Bd * Pi / 180.0;
  double n = (double)nZone;
  double l = (Ld - (3.0 + 6.0 * (n - 1.0))) / 57.29577951;
  double l2 = l * l;
  double cosb = cos(B);
  double sinb = sin(B);
  double sin2b = sin(2.0 * B);
  double sinb2 = sinb * sinb;
  double sinb4 = sinb2 * sinb2;
  double sinb6 = sinb4 * sinb2;

  *X = (kx[0] * B - sin2b * (kx[1] + kx[2] * sinb2 + kx[3] * sinb4 -
        l2 * (kx[4] + kx[5] * sinb2 + kx[6] * sinb4 + kx[7] * sinb6 +
        l2 * (kx[8] - kx[9] * sinb2 + kx[10] * sinb4 - kx[11] * sinb6 +
        l2 * (kx[12] - kx[13] * sinb2 + kx[14] * sinb4 - kx[15] * sinb6 +
        l2 * (kx[16] - kx[17] * sinb2 + kx[18] * sinb4 - kx[19] * sinb6))))));


  *Y = ((ky[0] + ky[1] * n) * ky[2] + l * cosb * (ky[3] + ky[4] * sinb2 + ky[5] * sinb4 + ky[6] * sinb6 +
        l2 * (ky[7] - ky[8] * sinb2 + ky[9] * sinb4 - ky[10] * sinb6 +
        l2 * (ky[11] - ky[12] * sinb2 + ky[13] * sinb4 - ky[14] * sinb6 +
        l2 * (ky[15] - ky[16] * sinb2 + ky[17] * sinb4 - ky[18] * sinb6)))));
}

void PlainToSK42(double X, double Y, double *Bd, double *Ld)
{
  double n;
  double B0;
  double dB;
  double l;
  double L;
  double B;

  values(X, Y, &n, &l, &B0, &dB);

  L = 6.0 * (n - 0.5) / 57.29577951 + l;
  B = B0 + dB;

  *Bd = (B * 180.0 / Pi);
  *Ld = (L * 180.0 / Pi);
}

void values(double X, double Y, double *n, double *l, double *B0, double *dB)
{
  double beta = X / 6367558.4968;
  double sinbeta = sin(beta);
  double sinbeta2 = sinbeta * sinbeta;
  double sinbeta4 = sinbeta2 * sinbeta2;
  double sin2beta = sin(2.0 * beta);
  double z0, z02;
  double cosb0;
  double sinb0, sinb02, sinb04, sinb06, sin2b0;

  *n = (double)((int)(Y / 1000000.0));
  *B0 = (beta + sin2beta * (kb0[0] - kb0[1] * sinbeta2 + kb0[2] * sinbeta4));

  cosb0 = cos(*B0);
  sinb0 = sin(*B0);
  sinb02 = sinb0 * sinb0;
  sinb04 = sinb02 * sinb02;
  sinb06 = sinb04 * sinb02;
  sin2b0 = sin(2.0 * *B0);

  z0 = ((Y - (10.0 * *n + 5.0) * 100000.0) / (6378245.0 * cosb0));
  z02 = z0 * z0;

  *dB = (-z02 * sin2b0 * (kdb[0] - kdb[1] * sinb02 + kdb[2] * sinb04 -
         z02 * (kdb[3] - kdb[4] * sinb02 + kdb[5] * sinb04 - kdb[6] * sinb06 -
         z02 * (kdb[7] - kdb[8] * sinb02 + kdb[9] * sinb04 - kdb[10] * sinb06 -
         z02 * (kdb[11] - kdb[12] * sinb02 + kdb[13] * sinb04 - kdb[14] * sinb06)))));

  *l = (z0 * (kl[0] - kl[1] * sinb02 - kl[2] * sinb04 - kl[3] * sinb06 -
        z02 * (kl[4] + kl[5] * sinb02 - kl[6] * sinb04 - kl[7] * sinb06 -
        z02 * (kl[8] + kl[9] * sinb02 + kl[10] * sinb04 - kl[11] * sinb06 -
        z02 * (kl[12] + kl[13] * sinb02 + kl[14] * sinb04 - kl[15] * sinb06 -
        z02 * (kl[16] + kl[17] * sinb02 + kl[18] * sinb04 + kl[19] * sinb06))))));
}

double RemoveZone(double Y, int *nZone)
{
  *nZone = ((int)(Y / 1000000.0));

  return (Y - ((double)*nZone) * 1000000.0);
}

double AddZone(double Y, int nZone)
{
  return (Y + ((double)nZone) * 1000000.0);
}

void LL84ToPlain42(double dLat84, double dLon84, double *pX42, double *pY42, int *pZone42)
{
  double dLat42, dLon42, dAlt42;

  WGS84ToSK42(dLat84, dLon84, 0,  &dLat42, &dLon42, &dAlt42);
  *pZone42 = SK42ZoneNo(dLon42);
  SK42ToPlain(dLat42, dLon42, *pZone42, pX42, pY42);
  *pY42 = RemoveZone(*pY42, pZone42);
}

void Plain42ToLL84(double dX42, double dY42, int nZone42, double *pLat84, double *pLon84)
{
  double dY = AddZone(dY42, nZone42);
  double dBd, dLd, dH84;

  PlainToSK42(dX42, dY, &dBd, &dLd);
  SK42ToWGS84(dBd, dLd, 0, pLat84, pLon84, &dH84);
}


/*
 * Mercator transformation
 * accounts for the fact that the earth is not a sphere, but a spheroid
 */
#define D_R (M_PI / 180.0)
#define R_D (180.0 / M_PI)
#define R_MAJOR 6378137.0
#define R_MINOR 6356752.3142
#define RATIO (R_MINOR/R_MAJOR)
#define ECCENT (sqrt(1.0 - (RATIO * RATIO)))
#define COM (0.5 * ECCENT)

double deg_rad(double ang) { return ang * D_R; }
double rad_deg(double ang) { return ang * R_D; }
double fmin(double f1, double f2) { return (f1 > f2) ? f2 : f1; }
double fmax(double f1, double f2) { return (f1 > f2) ? f1 : f2; }

double Lon2MercatorX(double lon)
{
  return R_MAJOR * deg_rad (lon);
}

double Lat2MercatorY(double lat)
{
  lat = fmin (89.5, fmax (lat, -89.5));
  double phi = deg_rad(lat);
  double sinphi = sin(phi);
  double con = ECCENT * sinphi;
  con = pow((1.0 - con) / (1.0 + con), COM);
  double ts = tan(0.5 * (M_PI * 0.5 - phi)) / con;

  return 0.0 - R_MAJOR * log(ts);
}

double MercatorX2Lon(double x)
{
  return rad_deg(x) / R_MAJOR;
}

double MercatorY2Lat(double y)
{
  double ts = exp(-y / R_MAJOR);
  double phi = M_PI_2 - 2.0 * atan(ts);
  double dphi = 1.0;

  for (int i = 0; (fabs(dphi) > 0.000000001 && i < 15); i++)
  {
    double con = ECCENT * sin (phi);
    dphi = M_PI_2 - 2.0 * atan (ts * pow((1.0 - con) / (1.0 + con), COM)) - phi;
    phi += dphi;
  }
  
  return rad_deg (phi);
}

double NormalizeCourse(double dCourse)
{
  if(dCourse >= 360.0) while(dCourse >= 360.0) dCourse -= 360.0;
  if(dCourse < 0.0) while(dCourse < 0.0) dCourse += 360.0;

  return dCourse;
}

void ArrangeCourses(double *pdCourse1, double *pdCourse2)
{
  if(*pdCourse1 != *pdCourse2)
  {
    if(fabs(*pdCourse2 - *pdCourse1) > 180.0)
    {
      if(*pdCourse2 >= 0.0 && *pdCourse2 <= 90.0 && *pdCourse2 < *pdCourse1)
        *pdCourse2 += 360.0;
      else if(*pdCourse1 >= 0.0 && *pdCourse1 <= 90.0 && *pdCourse1 < *pdCourse2)
        *pdCourse1 += 360.0;
      else if(*pdCourse2 >= 90.0 && *pdCourse2 <= 180.0 && *pdCourse2 < *pdCourse1)
        *pdCourse1 -= 360.0;
      else if(*pdCourse1 >= 90.0 && *pdCourse1 <= 180.0 && *pdCourse1 < *pdCourse2)
        *pdCourse2 -= 360.0;
    }
  }
}

int OSMLong2TileX(double Lon, int Z)
{
	return (int)(floor((Lon + 180.0) / 360.0 * pow(2.0, Z)));
}
 
int OSMLat2TileY(double Lat, int Z)
{ 
	return (int)(floor((1.0 - log(tan(Lat * M_PI / 180.0) + 1.0 / cos(Lat * M_PI / 180.0)) / M_PI) / 2.0 * pow(2.0, Z))); 
}
 
double OSMTileX2Long(int X, int Z)
{
	return (X / pow(2.0, Z) * 360.0 - 180);
}
 
double OSMTileY2Lat(int Y, int Z)
{
	double n = M_PI - 2.0 * M_PI * Y / pow(2.0, Z);
	return (180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n))));
}

const static double ResolutionMPP[21] = {156543.03, 78271.52, 39135.76, 19567.88, 9783.94,
                                         4891.97, 2445.98, 1222.99, 611.50, 305.75, 152.87,
                                         76.437, 38.219, 19.109, 9.5546, 4.7773, 2.3887, 
                                         1.1943, 0.5972, 0.2986, 0.1493};

double OSMResolution(int nZoom)
{
  double dRes = 0.0;

  if(nZoom >= 0 && nZoom <= 20) dRes = ResolutionMPP[nZoom];

  return dRes;
}

int OSMCalcZoomLevel(double Lat1, double Long1, double Lat2, double Long2, int Width, int Height)
{
  double CentralLat = (Lat1 + Lat2) / 2.0;
  CentralLat = fmin (89.5, fmax(CentralLat, -89.5));
  double LatFactor = cos(deg_rad(CentralLat));

  /*
  double ResolutionH = fabs(Lat2MercatorY(Lat1) - Lat2MercatorY(Lat2)) / (double)Height;
  double ResolutionW = fabs(Lon2MercatorX(Long1) - Lon2MercatorX(Long2)) / (double)Width;
  double Resolution = fmax(ResolutionH, ResolutionW) / LatFactor;

  int Zoom = 0;

  for(int i = 0; i < 21; i++)
  {
    if(Resolution > ResolutionMPP[i]) break;
    Zoom = i;
  }
  */

  /**/
  double SpaceH = fabs(Lat2MercatorY(Lat1) - Lat2MercatorY(Lat2));
  double SpaceW = fabs(Lon2MercatorX(Long1) - Lon2MercatorX(Long2));

  int Zoom = 0;
  int n = 1;

  for(int i = 0; i < 20; i++)
  {
    double Resolution = 156543.03 * LatFactor / (double)n;
    int OSMHeight = SpaceH / Resolution;
    int OSMWidth = SpaceW / Resolution;

    if(OSMHeight <= Height && OSMWidth <= Width)
    {
      Zoom = i + 1;
      n <<= 1;
    }
    else break;
  }

  /**/

  return Zoom;
}

void DecomposeSpeed(double  dSpeed,  double  dCourse,
                                               double *dSpeedX, double *dSpeedY)
{
  double dCourseR = dCourse * D_R;
  *dSpeedX = dSpeed * cos(dCourseR);
  *dSpeedY = dSpeed * sin(dCourseR);
}

void ComposeSpeed(double  dSpeedX, double  dSpeedY,
                                             double *dSpeed,  double *dCourse)
{
  *dSpeed = sqrt(dSpeedX * dSpeedX + dSpeedY * dSpeedY);

  if(fabs(dSpeedY) < DBL_EPSILON && fabs(dSpeedX) < DBL_EPSILON)
    *dCourse = 0.0;
  else
  {
    *dCourse = atan2(dSpeedY, dSpeedX) * R_D;
    if(*dCourse < 0.0) *dCourse += 360.0;
  }
}
