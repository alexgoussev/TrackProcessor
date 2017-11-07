#ifndef __DATUM_CONVERSION_H__
#define __DATUM_CONVERSION_H__

/*
 * ����� ������� ��� �������������� ������������� ��������� �� WGS84 � ��42
 * � �������, � ����� ��������������� ��������� ��42 �� ������������� �
 * ������������ � �������.
 * �� ������ ����� �������: http://gis-lab.info/qa/wgs84-sk42-wgs84-formula.html
 * ��������� ������� �������������� ����� �������������� � �������������� ������������
 * � ����������� � ���� � 51794-2001 (http://gis-lab.info/docs/law/gost51794-2001.zip)
 *
 * (c) GIS-Lab � ������, 2002-2008
 * (c) Victor Pomortseff 2008
 * /

/*
 * ����������� �������������� WGS84 -> ��42 �� ���� � 51794-2001
 * (http://gis-lab.info/docs/law/gost51794-2001.zip)
 *
 * �������� �� ���������:
 *
 * datum_trans_dx = -23.92;
 * datum_trans_dy = 141.27;
 * datum_trans_dz = 80.9;
 *
 * ������� �������� �����������������, � ��������
 * (����� �� ��������� GeoCalc Open 0.97 - http://gis-lab.info/qa/geocalc.html)
 *
 * datum_trans_wx = 0;
 * datum_trans_wy = 0.35;
 * datum_trans_wz = 0.82;
 *
 * ���������������� �������� ��������� *
 * datum_trans_ms = 0.12 * 0.000001;
 *
 */

/*
 * �������������� ��������� �� WGS84 � ��42
 * Bd   - ������ WGS84 � ��������, ������ dd.dddddd
 * Ld   - ������� WGS84 � ��������, ������ dd.dddddd
 * H    - ������ WGS84 � ������
 * Bd42 - ������ ��42 � ��������, ������ dd.dddddd
 * Ld42 - ������� ��42 � ��������, ������ dd.dddddd
 * H42  - ������ ��42 � ������
 */
void WGS84ToSK42(double Bd, double Ld, double H,
                                            double *Bd42, double *Ld42, double *H42);

/*
 * �������������� ��������� �� ��42 � WGS84
 * Bd   - ������ ��42 � ��������, ������ dd.dddddd
 * Ld   - ������� ��42 � ��������, ������ dd.dddddd
 * H    - ������ ��42 � ������
 * Bd84 - ������ WGS84 � ��������, ������ dd.dddddd
 * Ld84 - ������� WGS84 � ��������, ������ dd.dddddd
 * H84  - ������ WGS84 � ������
 */
void SK42ToWGS84(double Bd, double Ld, double H,
                                            double *Bd84, double *Ld84, double *H84);

/*
 * ���������� ����� 6-��������� ���� ��42
 * Ld   - ������� ��42 � ��������, ������ dd.dddddd
 */
int SK42ZoneNo(double Ld);

/*
 * �������������� ������������� ��������� ��42 � �������������
 * Bd    - ������ ��42 � ��������, ������ dd.dddddd
 * Ld    - ������� ��42 � ��������, ������ dd.dddddd
 * nZone - ����� 6-��������� ���� ��42
 * X     - ���������� X
 * Y     - ���������� Y (�������� ������� ����)
 */
void SK42ToPlain(double Bd, double Ld, int nZone, double *X, double *Y);

/*
 * �������������� ������������� ��������� ��42 � �������������
 * X     - ���������� X
 * Y     - ���������� Y (������ ��������� ������� ����)
 * Bd    - ������ ��42 � ��������, ������ dd.dddddd
 * Ld    - ������� ��42 � ��������, ������ dd.dddddd
 */
void PlainToSK42(double X, double Y, double *Bd, double *Ld);

/*
 * �������� �������� ���� �� Y ���������� ��42
 * Y     - ���������� Y (���������� ������� ����)
 * nZone - ����� 6-��������� ���� ��42 (������� ������ �� Y ����������)
 */
double RemoveZone(double Y, int *nZone);

/*
 * ���������� �������� ���� � Y ���������� ��42
 * Y     - ���������� Y (�� ���������� ������� ����)
 * nZone - ����� 6-��������� ���� ��42
 */
double AddZone(double Y, int nZone);

/*
 * ������� �������������� WGS84 � ����� ��42 � �������
 */
void LL84ToPlain42(double dLat84, double dLon84, double *dX42, double *dY42, int *nZone42);
void Plain42ToLL84(double dX42, double dY42, int nZone42, double *pLat84, double *pLon84);

/*
 * �������������� WGS84 - UTM � ������� ������������ ��������� ������������ ��������� ����
 */
void WGS84ToUTM(double Lat, double Lon, double Lon0, double *X, double *Y);
void UTMToWGS84(double X, double Y, double Lon0, double *Lat, double *Lon);

/*
 * ������ ������������ ������ ���� UTM � �� ������������ ���������
 */
int UTMZone(double Lon);
double UTMZoneCM(int Zone);
double UTMCM(double Lon);

/*
 * ������ ���������� � ������� �� ����������� ��� WGS84.
 */
double AzimuthLL(double LatA, double LonA, double LatB, double LonB);
double DistanceLL(double LatA, double LonA, double LatB, double LonB);

/*
 * �������������� Lat/Lon � �������� ��������� � �������
 */
double Lon2MercatorX(double lon);
double Lat2MercatorY(double lat);
double MercatorX2Lon(double x);
double MercatorY2Lat(double y);

/*
 * ������������ � ������������ ��������
 */
double NormalizeCourse(double dCourse);
void ArrangeCourses(double *pdCourse1, double *pdCourse2);

/*
 * ���������� ������ ����� OSM ����� �� �����������
 */
int OSMLong2TileX(double Lon, int Z);
int OSMLat2TileY(double Lat, int Z);
double OSMTileX2Long(int X, int Z);
double OSMTileY2Lat(int Y, int Z);
int OSMCalcZoomLevel(double Lat1, double Long1, double Lat2, double Long2, int Width, int Height);
double OSMResolution(int nZoom);

/*
 * ���������� �������� �� ����
 */

void DecomposeSpeed(double  dSpeed,  double  dCourse,
                                               double *dSpeedX, double *dSpeedY);
void ComposeSpeed(double  dSpeedX, double  dSpeedY,
                                             double *dSpeed,  double *dCourse);


/*
 * ������ �������������
 *
 * #include <stdio.h>
 * #include "datum.h"
 *
 * int main()
 * {
 *   double Lat84 = 56.850340;
 *   double Long84 = 60.384130;
 *   double Alt84 = 264.0;
 *   double Lat42;
 *   double Long42;
 *   double Alt42;
 *   double X42;
 *   double Y42;
 *   int nZone;
 *
 *   WGS84ToSK42(Lat84, Long84, Alt84, &Lat42, &Long42, &Alt42);
 *   nZone = SK42ZoneNo(Long42);
 *   SK42ToPlain(Lat42, Long42, nZone, &X42, &Y42);
 *   Y42 = RemoveZone(Y42, &nZone);
 *
 *   printf("Lattitude WGS84  : %Lf\n", Lat84);
 *   printf("Longitude WGS84  : %Lf\n", Long84);
 *   printf("Altitude WGS84   : %Lf\n\n", Alt84);
 *
 *   printf("Lattitude SK42   : %Lf\n", Lat42);
 *   printf("Longitude SK42   : %Lf\n", Long42);
 *   printf("Altitude SK42    : %Lf\n\n", Alt42);
 *
 *   printf("Zone number SK42 : %d\n", nZone);
 *   printf("Position X SK42  : %Lf\n", X42);
 *   printf("Position Y SK42  : %Lf\n\n", Y42);
 *
 *   Y42 = AddZone(Y42, nZone);
 *   PlainToSK42(X42, Y42, &Lat42, &Long42);
 *   SK42ToWGS84(Lat42, Long42, Alt42, &Lat84, &Long84, &Alt84);
 *
 *   printf("Lattitude SK42   : %Lf\n", Lat42);
 *   printf("Longitude SK42   : %Lf\n", Long42);
 *   printf("Altitude SK42    : %Lf\n\n", Alt42);
 *
 *   printf("Lattitude WGS84  : %Lf\n", Lat84);
 *   printf("Longitude WGS84  : %Lf\n", Long84);
 *   printf("Altitude WGS84   : %Lf\n\n", Alt84);
 *
 *   return 0;
 * }
 *
 */
#endif