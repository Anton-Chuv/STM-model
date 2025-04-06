#include "datamodel.h"

DataModel::DataModel() {  // default value for variant 20
  z0 = 10.0;              // тунельынй зазор [5, 7, 15]
  Ut = 0.01;              // Ut тунельное напряжение [0.1]
  Fi0 = 4.5;              // Fi0 локальная работа элетронов
  Ef = 5.71;              // Ef уровень Ферми
}

double DataModel::getI(int Z) {
  // сразу приведено к наноамперам и аргстремам
  // return 1620*U*E*math.e**(-1.025*Z*(fi(Z))**0.5)
  double I = 0;
  return I;
}
