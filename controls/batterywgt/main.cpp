#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include "batterywgt.h"
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QDebug>
#include <QWidget>
#include <QFrame>
#include <QStyleOption>
#include <QPainter>
#include "tool.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 加载 QSS 样式表
    app.setStyleSheet(getQSS());
    // 创建主窗口
    QWidget window;
    window.resize(1024, 600);
    QVBoxLayout *winLayout = new QVBoxLayout(&window);
    winLayout->setContentsMargins(0,0,0,0);
    winLayout->setSpacing(0);

    //状态栏
    QWidget *statusBar = new QWidget;
    statusBar->setAutoFillBackground(true);
    statusBar->setObjectName("statusBar");
    statusBar->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
    winLayout->addWidget(statusBar);
    winLayout->addStretch(1);
    qDebug()<<"window size:"<<window.size();
    qDebug()<<"statusBar size:"<<statusBar->size();

    QHBoxLayout *statusBarLayout = new QHBoxLayout(statusBar);
    statusBarLayout->setContentsMargins(0,0,0,0);
    statusBarLayout->setSpacing(0);
    // 创建电池控件
    BatteryWgt *battery = new BatteryWgt(true);
    BatteryWgt *battery1 = new BatteryWgt(true);
    battery->setObjectName("battery");
    battery1->setObjectName("battery");
    battery->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
    battery1->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);

    statusBarLayout->addWidget(battery,1);
    statusBarLayout->addWidget(battery1,1);
    statusBarLayout->addStretch(1);
    QTimer* click=new QTimer;
    QObject::connect(click,&QTimer::timeout,[battery](){static int per=10;battery->setBatteryPercent(per);per=per>=100?0:per+10;});
    click->start(1000);

    QPushButton *appQSS = new QPushButton("applicationQSS");
    statusBarLayout->addWidget(appQSS);
    QObject::connect(appQSS, &QPushButton::pressed,[&](){cleanQSS();app.setStyleSheet(getQSS());});
    // QObject::connect(appQSS, &QPushButton::released,[&](){app.setStyleSheet(getQSS());});
    QTimer::singleShot(0, [&](){
        qDebug()<<"after show - window size:"<<window.size();
        qDebug()<<"after show - statusBar size:"<<statusBar->size();
        qDebug() << "after show - statusBar contentsRect:" << statusBar->contentsRect();
        qDebug() << "after show - statusBar sizeHint:" << statusBar->sizeHint();
        qDebug()<<"after show - battery size:"<<battery->size();
        qDebug()<<"after show - battery contentsRect:"<<battery->contentsRect();
        qDebug() << "after show - battery geometry:" << battery->geometry();
    });
    window.show();

    return app.exec();
}
