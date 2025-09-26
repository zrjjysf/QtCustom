#include "tool.h"
#include "verticalinfowidget.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QTextDocument>
#include <QTextOption>
#include <QVBoxLayout>
#include "ccapplication.h"

int main(int argc, char *argv[]) {
  CCApplication a(argc, argv);
  a.setQSS(getQSS());

  QWidget *mainW = new QWidget;
  mainW->resize(400, 300);
  QVBoxLayout *mainL = new QVBoxLayout(mainW);
  VerticalInfoWidget w;
  w.setTopText("标题信息");
  w.setBottomText("详细信息");
  mainL->addWidget(&w);
  QLabel *testL = new QLabel;
  mainL->addWidget(testL);
  testL->setText("新华日报");
  QPushButton *appQSS = new QPushButton("applicationQSS");
  mainL->addWidget(appQSS);
  QObject::connect(appQSS, &QPushButton::pressed, [&]() {
    // cleanQSS();
    a.setQSS(getQSS());
  });
  mainW->show();
  return a.exec();
}
