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
  float prof_[50 * 10][2];
  void paintEvent(QPaintEvent*) {
    QPainter painter;
    QPainterPath path;
    float surfaceScale = 20;
    path.moveTo(-10 * surfaceScale, 0);
    for (int i = 0; i < 10; ++i)
      path.lineTo(surfaceNodes[i][0] * surfaceScale,
                  surfaceNodes[i][1] * surfaceScale);
    path.lineTo((surfaceNodes[9][0] + 10) * surfaceScale, 0);
    painter.begin(this);
    painter.drawRect(0, 0, width() - 1, height() - 1);
    painter.translate(200,
                      height() * 0.9);  // утсновка начала координат
    painter.scale(1, -1);
    painter.strokePath(path, QPen(Qt::black));

    for (int i = 0; i < 50 * 10; ++i) {
      int x = prof_[i][0] * surfaceScale;
      int y = prof_[i][1] * surfaceScale;
      painter.drawRect(x, y, 1, 1);
    }

    painter.end();
  }

 public:
  SurfaceCanva() {}
 public slots:
  void redraw(float prof[50 * 10][2]) {
    for (int i = 0; i < 50 * 10; ++i) {
      prof_[i][0] = prof[i][0];
      prof_[i][1] = prof[i][1];
    }
    update();
  }
};

void onClickPlus() {}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  // mainwindow parametrs
  QWidget* mainWidget = new QWidget();
  mainWidget->setWindowTitle("Модель СТМ");
  mainWidget->setMinimumHeight(500);
  mainWidget->setMinimumWidth(1200);
  // layout
  QHBoxLayout* mainLayout = new QHBoxLayout();
  mainWidget->setLayout(mainLayout);
  QVBoxLayout* leftPanel = new QVBoxLayout();
  mainLayout->addLayout(leftPanel);

  QLabel* labelVar = new QLabel();
  leftPanel->addWidget(labelVar);
  labelVar->setText("Вариант 20");
  labelVar->setMaximumWidth(100);

  DataModel* model = new DataModel();

  QLabel* labelCur = new QLabel();
  leftPanel->addWidget(labelCur);
  QString CurValue = "Высота: ";
  CurValue.append(QString::number(model->z0()));
  labelCur->setText(CurValue);
  labelCur->setMaximumWidth(200);

  QHBoxLayout* z0BtnLayout = new QHBoxLayout();
  leftPanel->addLayout(z0BtnLayout);

  QPushButton* z0plus = new QPushButton("+");
  QPushButton* z0minus = new QPushButton("-");
  z0plus->setMaximumWidth(30);
  z0minus->setMaximumWidth(30);
  z0BtnLayout->addWidget(z0minus);
  z0BtnLayout->addWidget(z0plus);

  leftPanel->addStretch();

  SurfaceCanva* canva = new SurfaceCanva();
  canva->redraw(model->prof);
  mainLayout->addWidget(canva);
  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::white);
  canva->setAutoFillBackground(true);
  canva->setPalette(pal);

  QObject::connect(z0plus, &QPushButton::clicked, model, &DataModel::z0plus);
  QObject::connect(z0plus, &QPushButton::clicked, labelCur,
                   [=]() { labelCur->setText(QString::number(model->z0())); });
  QObject::connect(z0plus, &QPushButton::clicked, canva,
                   [=]() { canva->redraw(model->prof); });

  QObject::connect(z0minus, &QPushButton::clicked, model, &DataModel::z0minus);
  QObject::connect(z0minus, &QPushButton::clicked, labelCur,
                   [=]() { labelCur->setText(QString::number(model->z0())); });
  QObject::connect(z0minus, &QPushButton::clicked, canva,
                   [=]() { canva->redraw(model->prof); });

  mainWidget->show();
  return app.exec();
}
