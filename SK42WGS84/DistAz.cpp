//---------------------------------------------------------------------------
#include <math.h> 

#include "SK42WGS84.h"

static const double Pi = 3.14159265358979;
static const double dEarthRadius = 6378.135;
static const double dFlat = 0.993291;

double AzimuthLL(double LatA, double LonA, double LatB, double LonB)
{
  double TanA, TanB, GeoA, GeoB, ColA, ColB;
  double Sin_ColA, Cos_ColA;
  double DLon, xtop, xden;
  double AzimutRad, AzimutDEG;

  // Conversation to radiants
  LatA = LatA * Pi / 180.0;
  LonA = LonA * Pi / 180.0;
  LatB = LatB * Pi / 180.0;
  LonB = LonB * Pi / 180.0;

  /* Determning latitutes in co latitudes Point A and Sine & Cosine values */
  TanA = tan(LatA) * dFlat;
  GeoA = atan(TanA);
  ColA = M_PI_2 - GeoA;
  Sin_ColA = sin(ColA);
  Cos_ColA = cos(ColA);

  /* Determning latitutes in co latitudes Point A and Sine & Cosine values */
  TanB = tan(LatB) * dFlat;
  GeoB = atan(TanB);
  ColB = M_PI_2 - GeoB;

  // Determening Distance  between A and B
  DLon = LonB - LonA;

  // Determing azimut from A to B
  xtop = sin(DLon);
  xden = Sin_ColA / tan(ColB) - cos(DLon) * Cos_ColA;

  if(xtop != 0.0 && xden != 0.0) AzimutRad = atan2(xtop,xden);
  else AzimutRad = 0.0;

  AzimutDEG = AzimutRad * 180.0 / Pi;

  if(AzimutRad < 0.0)
    AzimutDEG = AzimutDEG + 360.0;

  return AzimutDEG;
}

double DistanceLL(double LatA, double LonA, double LatB, double LonB)
{
  double TanA, TanB, GeoA, GeoB, ColA, ColB;
  double Sin_ColA, Cos_ColA, Sin_ColB, Cos_ColB;
  double DLon, CosDelta, DeltaRad;
  double Cos_DLon, CoLat, Cos_CoLat, DeltaM;

  // Conversation to radiants
  LatA = LatA * Pi / 180.0;
  LonA = LonA * Pi / 180.0;
  LatB = LatB * Pi / 180.0;
  LonB = LonB * Pi / 180.0;

  /* Determning latitutes in co latitudes Point A and Sine & Cosine values */
  TanA = tan(LatA) * dFlat;
  GeoA = atan(TanA);
  ColA = M_PI_2 - GeoA;
  Sin_ColA = sin(ColA);
  Cos_ColA = cos(ColA);

  /* Determning latitutes in co latitudes Point A and Sine & Cosine values */
  TanB = tan(LatB) * dFlat;
  GeoB = atan(TanB);
  ColB = M_PI_2 - GeoB;
  Sin_ColB = sin(ColB);
  Cos_ColB = cos(ColB);

  // Determening Distance  between A and B
  DLon = LonB - LonA;
  Cos_DLon = cos(DLon);
  CosDelta = Sin_ColA * Sin_ColB * Cos_DLon + Cos_ColA * Cos_ColB;

  if(CosDelta > 1.0)
    CosDelta = 1.0;
  else if(CosDelta < -1.0)
    CosDelta = -1.0;

  DeltaRad=acos(CosDelta);

  // Determening distance in meter
  CoLat = M_PI_2 - (LatA + LatB) / 2.0;
  Cos_CoLat = cos(CoLat);
  DeltaM = DeltaRad * ((1.0/3.0 - Cos_CoLat * Cos_CoLat) * 0.00335278 + 1.0) * dEarthRadius * 1000;

  return DeltaM;
}
