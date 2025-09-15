#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include "batterywgt.h"
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 加载 QSS 样式表
    QFile qssFile(":/style.qss"); // 如果你放在资源文件中
    if (qssFile.open(QFile::ReadOnly)) {
        QString styleSheet = qssFile.readAll();
        app.setStyleSheet(styleSheet);
        qDebug()<<styleSheet;
    }
    // 创建主窗口
    QWidget window;
    window.resize(1024, 600);
    QVBoxLayout *winLayout = new QVBoxLayout(&window);

    //状态栏
    QWidget *statusBar=new QWidget;
    statusBar->setAutoFillBackground(true);
    statusBar->setObjectName("statusBar");
    winLayout->addWidget(statusBar);
    winLayout->addStretch(1);

    QHBoxLayout *statusBarLayout = new QHBoxLayout(statusBar);

    // 创建电池控件
    BatteryWgt *battery = new BatteryWgt(true);

    statusBarLayout->addWidget(battery,Qt::AlignCenter);
    qDebug()<<battery->sizeHint();
    qDebug()<<battery->width()<<","<<battery->height();

    QTimer* click=new QTimer;
    QObject::connect(click,&QTimer::timeout,[battery](){static int per=10;battery->setBatteryPercent(per);per=per>=100?0:per+10;});
    click->start(1000);
    window.show();

    return app.exec();
}
