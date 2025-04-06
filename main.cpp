#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QWidget>

#include "datamodel.h"

class SurfaceCanva : public QWidget {
  // Точки исходной поверхности Варианта 20
  int surfaceNodes[10][2] = {{0, 0},  {0, 4},  {7, 4},   {7, 7},   {10, 7},
                             {10, 0}, {15, 0}, {15, 10}, {20, 10}, {30, 0}};

  void paintEvent(QPaintEvent*) {
    QPainter painter;
    QPainterPath path;
    float surfaceScale = 20;
    path.moveTo(-10 * surfaceScale, 0);
    for (int i = 0; i < 10; ++i)
      path.lineTo(surfaceNodes[i][0] * surfaceScale,
                  -surfaceNodes[i][1] * surfaceScale);
    path.lineTo((surfaceNodes[10][0] + 10) * surfaceScale, 0);
    painter.begin(this);
    painter.drawRect(0, 0, width() - 1, height() - 1);
    painter.translate(width() * 0.1,
                      height() * 0.9);  // утсновка начала координат
    // painter.scale(1, 1);
    painter.strokePath(path, QPen(Qt::black));

    painter.end();
  }
};

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  // mainwindow parametrs
  QWidget* mainWidget = new QWidget();
  mainWidget->setWindowTitle("Модель СТМ");
  mainWidget->setMinimumHeight(500);
  mainWidget->setMinimumWidth(1000);
  // layout
  QHBoxLayout* mainLaout = new QHBoxLayout();
  mainWidget->setLayout(mainLaout);
  QVBoxLayout* leftPanel = new QVBoxLayout();
  mainLaout->addLayout(leftPanel);

  QLabel* labelVar = new QLabel();
  leftPanel->addWidget(labelVar);
  labelVar->setText("Вариант 20");
  labelVar->setMaximumWidth(150);

  DataModel* model = new DataModel();

  QLabel* labelCur = new QLabel();
  leftPanel->addWidget(labelCur);
  QString CurValue = "Текущий ток: ";
  CurValue.append(QString::number(model->getI(-1)));
  labelCur->setText(CurValue);
  labelCur->setMaximumWidth(150);

  leftPanel->addStretch();

  SurfaceCanva* canva = new SurfaceCanva();
  mainLaout->addWidget(canva);
  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::white);
  canva->setAutoFillBackground(true);
  canva->setPalette(pal);

  mainWidget->show();
  return app.exec();
}
