#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "aspectratiowidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget window;
    window.setWindowTitle("Aspect Ratio Widget Demo");

    AspectRatioWidget *ratioWidget = new AspectRatioWidget(2.0/1.0);

    QPushButton *btnH = new QPushButton("水平布局");
    QPushButton *btnV = new QPushButton("垂直布局");

    QHBoxLayout *mainLayout = new QHBoxLayout(&window);
    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlLayout->addWidget(btnH);
    controlLayout->addWidget(btnV);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(ratioWidget, 1);

    // 切换布局
    QObject::connect(btnH, &QPushButton::clicked, [&]() {
        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(ratioWidget);
        mainLayout->addLayout(h, 1);
    });

    QObject::connect(btnV, &QPushButton::clicked, [&]() {
        QVBoxLayout *v = new QVBoxLayout;
        v->addWidget(ratioWidget);
        mainLayout->addLayout(v, 1);
    });

    window.resize(800, 600);
    window.show();

    return a.exec();
}
