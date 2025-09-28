#include <QApplication>
#include <QFile>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include "themesvgwgt.h"
#include "tool.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(getQSS());
    QWidget window;
    window.setWindowTitle("ThemeSvg Demo");
    QFile f(":/img/tip_charge.svg");
    qDebug() << "exists?" << f.exists();

    QVBoxLayout *layout = new QVBoxLayout(&window);

    ThemeSvgWgt *svg1 = new ThemeSvgWgt;
    layout->addWidget(svg1);

    QPushButton *appQSS = new QPushButton("applicationQSS");
    layout->addWidget(appQSS);
    QObject::connect(appQSS, &QPushButton::pressed,[&](){cleanQSS();app.setStyleSheet(getQSS());});
    window.show();
    return app.exec();
}
