#include <QApplication>
#include "batterycellwidget.h"
#include "batterymodulewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BatteryModuleWidget module;
    module.resize(400, 300);

    // 添加电芯
    for (int i = 0; i < 10; ++i) {
        BatteryCellWidget* cell = new BatteryCellWidget;
        cell->setNumber(i+1);
        cell->setCenterText(QString("%1.%1V").arg(i+3));
        module.addBatteryCell(cell);
    }

    module.show();
    return a.exec();
}
