#include "datamodel.h"

#include <QRandomGenerator>
#include <QtMath>

DataModel::DataModel() {  // default value for variant 20
  z0 = 10.0;              // тунельынй зазор [5, 7, 15]
  // Ut = 0.01;              // Ut тунельное напряжение [0.1]
  // Fi0 = 4.5;              // Fi0 локальная работа элетронов
  // Ef = 5.71;              // Ef уровень Ферми
}

// const float z0 = 10.0;
int surfaceNodes[10][2] = {{0, 0},  {0, 4},  {7, 4},   {7, 7},   {10, 7},
                           {10, 0}, {15, 0}, {15, 10}, {20, 10}, {30, 0}};

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

double getJ(double Z) {
  // сразу приведено к наноамперам и аргстремам
  double part1 = qPow(M_E, (-1.025 * Z * qSqrt(fi(Z))));
  double j = 1620 * U_t * E_f * part1;
  return j;
}

double DataModel::getI(float X, float Z) {
  float I = 0;
  float d = 10;  // +- x, +- y
  int n = 10000000;
  int s = 0;
  QRandomGenerator rnd;
  for (int i = 0; i < n; i++) {
    double x = X + rnd.generateDouble() * 2 * d - d;
    double y = rnd.generateDouble() * 2 * d - d;
    double z0 = 3;
    double j = rnd.generateDouble() * 1620 * U_t * E_f *
               qPow(M_E, (-1.025 * z0 * qSqrt(fi(z0))));
    double dist = qSqrt(qPow(X - x, 2) + qPow(y, 2) + qPow(Z - 0, 2));
    // TODO выше ноль поменять на высоту в точке x
    double j0 = getJ(dist);
    if (j0 <= j) s++;
  }
  I = d * d * 2 * 2 * 1620 * U_t * E_f *
      qPow(M_E, (-1.025 * z0 * qSqrt(fi(z0)))) / n * s;
  return I;
}
