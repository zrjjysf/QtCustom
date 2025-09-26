#include <QApplication>
#include <QFile>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include "iconlabel.h"
#include "tool.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(getQSS());
    QWidget window;
    window.setWindowTitle("ThemeSvg Demo");
    window.resize(200,100);
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QHBoxLayout *hlayout = new QHBoxLayout;
    layout->addLayout(hlayout);
    IconLabel* label = new IconLabel;
    hlayout->addWidget(label);
    hlayout->addStretch(1);
    label->setText("充电中");
    QPushButton *appQSS = new QPushButton("applicationQSS");
    layout->addWidget(appQSS);
    QObject::connect(appQSS, &QPushButton::pressed,[&](){cleanQSS();app.setStyleSheet(getQSS());});
    window.show();
    return app.exec();
}
