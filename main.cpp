#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include "batterywgt.h"
#include <QDebug>
#include <QTimer>
#include <QPushButton>

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
    window.setWindowTitle("Battery Widget with QSS");

    // 创建电池控件
    BatteryWgt *battery = new BatteryWgt(true);
//    battery->setBatteryPercent(35); // 设置当前电量

    // 布局
    QHBoxLayout *layout = new QHBoxLayout(&window);
//    QPushButton * a=new QPushButton();

    layout->setAlignment(Qt::AlignCenter);
//    layout->addWidget(a,Qt::AlignCenter);
    layout->addWidget(battery,Qt::AlignCenter);
    qDebug()<<battery->sizeHint();

    QTimer* click=new QTimer;
    QObject::connect(click,&QTimer::timeout,[battery](){static int per=10;battery->setBatteryPercent(per);per=per>=100?0:per+10;});
    click->start(1000);
    window.resize(300, 300);
    window.show();

    return app.exec();
}
