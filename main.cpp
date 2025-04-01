#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QWidget>

#define Z0 10    // тунельынй зазор [5, 7, 15]
#define UT 0.01  // Ut тунельное напряжение [0.1]
#define FI0 4.5  // Fi0 локальная работа элетронов
#define EF 5.71  // Ef уровень Ферми

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QWidget* mainWidget = new QWidget();
  mainWidget->setWindowTitle("Модель СТМ");
  mainWidget->setMinimumHeight(1050);
  mainWidget->setMinimumWidth(1920);
  QHBoxLayout* mainLaout = new QHBoxLayout();
  mainWidget->setLayout(mainLaout);
  QVBoxLayout* leftPanel = new QVBoxLayout();
  mainLaout->addLayout(leftPanel);

  QLabel* label = new QLabel();
  leftPanel->addWidget(label);
  leftPanel->addWidget(new QPushButton());
  label->setText("Вариант 20");

  QWidget* canva = new QWidget();
  mainLaout->addWidget(canva);
  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::white);

  canva->setAutoFillBackground(true);
  canva->setPalette(pal);

  mainWidget->show();  // отображаем виджет
  return app.exec();
}
