#ifndef __DATUM_CONVERSION_H__
#define __DATUM_CONVERSION_H__

/*
 * Набор функций для преобразования геодезических координат из WGS84 в СК42
 * и обратно, а также преобразованиия координат СК42 из геодезических в
 * прямоугоьные и обратно.
 * За основу взяты формулы: http://gis-lab.info/qa/wgs84-sk42-wgs84-formula.html
 * Добавлены функции преобразования между геодезическими и прямоугольными координатами
 * в соответсвии с ГОСТ Р 51794-2001 (http://gis-lab.info/docs/law/gost51794-2001.zip)
 *
 * (c) GIS-Lab и авторы, 2002-2008
 * (c) Victor Pomortseff 2008
 * /

/*
 * Коффициенты преобразования WGS84 -> СК42 по ГОСТ Р 51794-2001
 * (http://gis-lab.info/docs/law/gost51794-2001.zip)
 *
 * Значения по умолчанию:
 *
 * datum_trans_dx = -23.92;
 * datum_trans_dy = 141.27;
 * datum_trans_dz = 80.9;
 *
 * Угловые элементы трансформирования, в секундах
 * (взяты из программы GeoCalc Open 0.97 - http://gis-lab.info/qa/geocalc.html)
 *
 * datum_trans_wx = 0;
 * datum_trans_wy = 0.35;
 * datum_trans_wz = 0.82;
 *
 * Дифференциальное различие масштабов *
 * datum_trans_ms = 0.12 * 0.000001;
 *
 */

/*
 * Преобразование координат из WGS84 в СК42
 * Bd   - широта WGS84 в градусах, формат dd.dddddd
 * Ld   - долгота WGS84 в градусах, формат dd.dddddd
 * H    - высота WGS84 в метрах
 * Bd42 - широта СК42 в градусах, формат dd.dddddd
 * Ld42 - долгота СК42 в градусах, формат dd.dddddd
 * H42  - высота СК42 в метрах
 */
void WGS84ToSK42(double Bd, double Ld, double H,
                                            double *Bd42, double *Ld42, double *H42);

/*
 * Преобразование координат из СК42 в WGS84
 * Bd   - широта СК42 в градусах, формат dd.dddddd
 * Ld   - долгота СК42 в градусах, формат dd.dddddd
 * H    - высота СК42 в метрах
 * Bd84 - широта WGS84 в градусах, формат dd.dddddd
 * Ld84 - долгота WGS84 в градусах, формат dd.dddddd
 * H84  - высота WGS84 в метрах
 */
void SK42ToWGS84(double Bd, double Ld, double H,
                                            double *Bd84, double *Ld84, double *H84);

/*
 * Возвращает номер 6-градусной зоны СК42
 * Ld   - долгота СК42 в градусах, формат dd.dddddd
 */
int SK42ZoneNo(double Ld);

/*
 * Преобразование геодезических координат СК42 в прямоугольные
 * Bd    - широта СК42 в градусах, формат dd.dddddd
 * Ld    - долгота СК42 в градусах, формат dd.dddddd
 * nZone - номер 6-градусной зоны СК42
 * X     - координата X
 * Y     - координата Y (содержит префикс зоны)
 */
void SK42ToPlain(double Bd, double Ld, int nZone, double *X, double *Y);

/*
 * Преобразование прямоугольных координат СК42 в геодезические
 * X     - координата X
 * Y     - координата Y (должна содержать префикс зоны)
 * Bd    - широта СК42 в градусах, формат dd.dddddd
 * Ld    - долгота СК42 в градусах, формат dd.dddddd
 */
void PlainToSK42(double X, double Y, double *Bd, double *Ld);

/*
 * Удаление префикса зоны из Y координаты СК42
 * Y     - координата Y (содержащая префикс зоны)
 * nZone - номер 6-градусной зоны СК42 (который удален из Y координаты)
 */
double RemoveZone(double Y, int *nZone);

/*
 * Добавление префикса зоны в Y координату СК42
 * Y     - координата Y (не содержащая префикс зоны)
 * nZone - номер 6-градусной зоны СК42
 */
double AddZone(double Y, int nZone);

/*
 * Быстрое перобразование WGS84 в метры СК42 и обратно
 */
void LL84ToPlain42(double dLat84, double dLon84, double *dX42, double *dY42, int *nZone42);
void Plain42ToLL84(double dX42, double dY42, int nZone42, double *pLat84, double *pLon84);

/*
 * Преобразование WGS84 - UTM и обратно относительно заданного центрального мередиана зоны
 */
void WGS84ToUTM(double Lat, double Lon, double Lon0, double *X, double *Y);
void UTMToWGS84(double X, double Y, double Lon0, double *Lat, double *Lon);

/*
 * Расчет стандартного номера зоны UTM и ее центрального мередиана
 */
int UTMZone(double Lon);
double UTMZoneCM(int Zone);
double UTMCM(double Lon);

/*
 * Расчет расстояния и азимута по координатам для WGS84.
 */
double AzimuthLL(double LatA, double LonA, double LatB, double LonB);
double DistanceLL(double LatA, double LonA, double LatB, double LonB);

/*
 * Преобразование Lat/Lon в проекцию Меркатора и обратно
 */
double Lon2MercatorX(double lon);
double Lat2MercatorY(double lat);
double MercatorX2Lon(double x);
double MercatorY2Lat(double y);

/*
 * Выравнивание и нормализация азимутов
 */
double NormalizeCourse(double dCourse);
void ArrangeCourses(double *pdCourse1, double *pdCourse2);

/*
 * Вычисление номера тейла OSM карты по координатам
 */
int OSMLong2TileX(double Lon, int Z);
int OSMLat2TileY(double Lat, int Z);
double OSMTileX2Long(int X, int Z);
double OSMTileY2Lat(int Y, int Z);
int OSMCalcZoomLevel(double Lat1, double Long1, double Lat2, double Long2, int Width, int Height);
double OSMResolution(int nZoom);

/*
 * Разложение скорости по осям
 */

void DecomposeSpeed(double  dSpeed,  double  dCourse,
                                               double *dSpeedX, double *dSpeedY);
void ComposeSpeed(double  dSpeedX, double  dSpeedY,
                                             double *dSpeed,  double *dCourse);


/*
 * Пример использования
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