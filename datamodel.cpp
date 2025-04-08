#include "datamodel.h"

#include <QRandomGenerator>
#include <QtMath>

DataModel::DataModel() {  // default value for variant 20
  z0_ = 4;                // тунельынй зазор [5, 7, 15]
  // Ut = 0.01;              // Ut тунельное напряжение [0.1]
  // Fi0 = 4.5;              // Fi0 локальная работа элетронов
  // Ef = 5.71;              // Ef уровень Ферми
  etalonI_ = etalonI();
  for (int i = 0; i < 50 * 10; ++i) {
    prof[i][0] = i * 0.1 - 10;
    prof[i][1] = z0_;
  }

  for (int i = 1; i < 50 * 10; ++i) {
    prof[i][1] = prof[i - 1][1];
    double I = getI(prof[i][0], prof[i][1]);
    while (etalonI_ * (1 + 0.01) < I || etalonI_ * (1 - 0.01) > I) {
      if (etalonI_ * (1 + 0.01) < I) prof[i][1] -= 0.01;
      if (etalonI_ * (1 - 0.01) > I) prof[i][1] += 0.01;

      I = getI(prof[i][0], prof[i][1]);
    }
  }
}
float DataModel::z0() { return z0_; }
double DataModel::etalonI() { return getI(-10, z0_); }

// const float z0 = 10.0;
int surfaceNodes[10][2] = {{0, 0},  {0, 4},  {7, 4},   {7, 7},   {10, 7},
                           {10, 0}, {15, 0}, {15, 10}, {20, 10}, {30, 0}};
double getXhiegth(double x) {
  if (x < 0) return 0;
  for (int i = 0; i < 10 - 1; ++i) {
    if (x <= surfaceNodes[i + 1][0]) {
      if (surfaceNodes[i][0] == surfaceNodes[i + 1][0]) i++;
      return surfaceNodes[i][1] +
             (surfaceNodes[i + 1][1] - surfaceNodes[i][1]) *
                 (x - surfaceNodes[i][0]) /
                 (surfaceNodes[i + 1][0] - surfaceNodes[i][0]);
    }
  }
  return 0;
}

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
  float d = z0_;  // +- x, +- y
  int n = 5000;
  int s = 0;

  double z = z0_ * 1;
  QRandomGenerator rnd;
  for (int i = 0; i < n; i++) {
    double x = X + rnd.generateDouble() * 2 * d - d;
    double y = rnd.generateDouble() * 2 * d - d;
    double j = rnd.generateDouble() * 1620 * U_t * E_f *
               qPow(M_E, (-1.025 * z * qSqrt(fi(z))));
    double H = getXhiegth(x);
    // if (H > Z) {
    // H = Z;
    // }

    double dist = qSqrt(qPow(X - x, 2) + qPow(y, 2) + qPow(Z - H, 2));
    double j0 = getJ(dist);
    if (j0 <= j) s++;
  }

  I = d * d * 2 * 2 * 1620 * U_t * E_f *
      qPow(M_E, (-1.025 * z * qSqrt(fi(z)))) / n * s;

  // тоже самое но для вериткальных поверхностей
  s = 0;
  int countVsurface = 0;
  for (int j = 0; j < 10 - 1; ++j) {
    if (surfaceNodes[j][0] == surfaceNodes[j + 1][0] &&
        ((surfaceNodes[j][1] < surfaceNodes[j + 1][1] &&
          surfaceNodes[j][0] > X) ||
         (surfaceNodes[j][1] > surfaceNodes[j + 1][1] &&
          surfaceNodes[j][0] < X))) {
      countVsurface++;
      for (int i = 0; i < n; i++) {
        int maxy = surfaceNodes[j][1] > surfaceNodes[j + 1][1]
                       ? surfaceNodes[j][1]
                       : surfaceNodes[j + 1][1];
        int miny = surfaceNodes[j][1] < surfaceNodes[j + 1][1]
                       ? surfaceNodes[j][1]
                       : surfaceNodes[j + 1][1];
        double x = Z + rnd.generateDouble() * 2 * d - d;
        double H = surfaceNodes[j][0] - X;
        if (x > maxy || x < miny) {
          x = -100000;
        }
        double y = rnd.generateDouble() * 2 * d - d;
        double j = rnd.generateDouble() * 1620 * U_t * E_f *
                   qPow(M_E, (-1.025 * z * qSqrt(fi(z))));
        double dist = qSqrt(qPow(y, 2) + qPow(H, 2) + qPow(Z - x, 2));
        double j0 = getJ(dist);
        if (j0 <= j) s++;
      }
    }
  }
  if (countVsurface)
    I += d * d * 2 * 2 * 1620 * U_t * E_f *
         qPow(M_E, (-1.025 * z * qSqrt(fi(z)))) / n * s / countVsurface;

  return I;
}
