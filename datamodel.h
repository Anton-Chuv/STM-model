#ifndef DATAMODEL_H
#define DATAMODEL_H

class DataModel {
  float z0_;  // тунельынй зазор [5, 7, 15]
  float Ut;   // Ut тунельное напряжение [0.1]
  float Fi0;  // Fi0 локальная работа элетронов
  float Ef;   // Ef уровень Ферми
  double etalonI_;

 public:
  float prof[50 * 10][2];
  double etalonI();
  float z0();

  DataModel();
  double getI(float X, float Z);
};

#endif  // DATAMODEL_H
