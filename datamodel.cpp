#include "datamodel.h"

#include <QtMath>

DataModel::DataModel() {  // default value for variant 20
  z0 = 10.0;              // тунельынй зазор [5, 7, 15]
  // Ut = 0.01;              // Ut тунельное напряжение [0.1]
  // Fi0 = 4.5;              // Fi0 локальная работа элетронов
  // Ef = 5.71;              // Ef уровень Ферми
}

// const float z0 = 10.0;
const float U_t = 0.01;
const float Fi0 = 4.5;
const float E_f = 5.71;
const float k = 1;

double fi(float Z) {  //
  float S1 = 3 / (k * Fi0);
  float S2 = Z * (1 - 23 / (3 * Fi0 * k * Z + 10 - 2 * U_t * k * Z)) + S1;
  float part1 = U_t * (S1 + S2) / (2 * Z);
  float part2 = 2.86 / (k * (S2 - S1)) * qLn((S2 * (Z - S1)) / (S1 * (Z - S2)));
  double f = Fi0 - part1 - part2;
  return f;
}

double DataModel::getI(float Z) {
  // сразу приведено к наноамперам и аргстремам
  float part1 = qPow(M_E, (-1.025 * Z * qSqrt(fi(Z))));
  double I = 1620 * U_t * E_f * part1;
  return I;
}
