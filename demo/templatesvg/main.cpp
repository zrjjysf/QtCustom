#include <QApplication>
#include <QFile>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include "templatesvgwidget.h"
#include "templatesvgrenderer.h"
#include "tool.h"
#include <QDebug>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(getQSS());
    QWidget* window = new QWidget;
    window->setWindowTitle("ThemeSvg Demo");
    // window->resize(200,200);

    QVBoxLayout *layout = new QVBoxLayout(window);

    TemplateSvgWidget *svg1 = new TemplateSvgWidget;
    svg1->setFixedSize(200,200);
    svg1->setProperty("status","close");
    layout->addWidget(svg1);
    TemplateSvgRenderer * svg1Renderer = svg1->renderer();
    svg1Renderer->setReplacement("currentColor", "#f49c39");
    svg1Renderer->setReplacement("PACKAGE_ID", "Pack 22");

    QPushButton *changeStatus = new QPushButton("changeStatus");
    QObject::connect(changeStatus, &QPushButton::clicked,[&](){
        QVariant current = svg1->property("status");
        qDebug()<<current;
        QString nextStatus;
        nextStatus = current.toString()=="close"?"open":"close";
        svg1->setProperty("status", nextStatus);
        svg1->style()->unpolish(svg1);
        svg1->style()->polish(svg1);
        svg1->update();
    });
    layout->addWidget(changeStatus);

    QPushButton *appQSS = new QPushButton("applicationQSS");
    layout->addWidget(appQSS);
    QObject::connect(appQSS, &QPushButton::pressed,[&](){cleanQSS();app.setStyleSheet(getQSS());});
    window->show();
    return app.exec();
}
