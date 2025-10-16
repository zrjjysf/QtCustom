#include <QApplication>
#include <QFile>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include "themesvgwgt.h"
#include "tool.h"
#include <QDebug>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(getQSS());
    QWidget* window = new QWidget;
    window->setWindowTitle("ThemeSvg Demo");
    QFile f(":/img/tip_charge.svg");
    qDebug() << "exists?" << f.exists();

    QVBoxLayout *layout = new QVBoxLayout(window);

    ThemeSvgWgt *svg1 = new ThemeSvgWgt;
    layout->addWidget(svg1);

    QPushButton *changeStatus = new QPushButton("changeStatus");
    QObject::connect(changeStatus, &QPushButton::pressed,[&](){
        QVariant current = window->property("status");
        qDebug()<<current;
        QString nextStatus;
        nextStatus = current.toString()=="Fault"?"Charge":"Fault";
        window->setProperty("status", nextStatus);

        for (auto child : window->findChildren<ThemeSvgWgt*>()) {
            child->style()->unpolish(child);
            child->style()->polish(child);
            child->update();
        }
    });
    layout->addWidget(changeStatus);

    QPushButton *appQSS = new QPushButton("applicationQSS");
    layout->addWidget(appQSS);
    QObject::connect(appQSS, &QPushButton::pressed,[&](){cleanQSS();app.setStyleSheet(getQSS());});
    window->show();
    return app.exec();
}
