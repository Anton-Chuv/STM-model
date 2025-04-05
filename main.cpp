#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QWidget>

class Data {
  float z0 = 10.0;  // тунельынй зазор [5, 7, 15]
  float Ut = 0.01;  // Ut тунельное напряжение [0.1]
  float Fi0 = 4.5;  // Fi0 локальная работа элетронов
  float Ef = 5.71;  // Ef уровень Ферми
                    //
                    // QPoint surfaceNodes[10] = {QPoint(0, 0), QPoint(0, 4)};

 public:
  double getI(int Z) {
    // сразу приведено к наноамперам и аргстремам
    // return 1620*U*E*math.e**(-1.025*Z*(fi(Z))**0.5)
    double I = 0;
    return I;
  }
};

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

  QLabel* labelCur = new QLabel();
  leftPanel->addWidget(labelCur);
  labelCur->setText("Текущий ток: чч");
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
