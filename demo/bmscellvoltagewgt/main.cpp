#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include "bmscellvoltagewgt.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 主窗口
    QMainWindow window;
    window.setWindowTitle("BatteryPackWidget 测试");
    BMSCellVoltageWgt * wgt = new BMSCellVoltageWgt;

    window.setCentralWidget(wgt);
    window.resize(1024, 740);
    window.show();

    return app.exec();
}
