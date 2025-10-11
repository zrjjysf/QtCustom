#include <QApplication>
#include <QHBoxLayout>
#include "batterycellwidget.h"
#include "batterymodulewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget* mainWindows = new QWidget;
    mainWindows->resize(1024, 540);
    QHBoxLayout* mainLayout = new QHBoxLayout(mainWindows);
    for(int i=1;i<3;++i)
    {
        BatteryModuleWidget* module = new BatteryModuleWidget;
        mainLayout->addWidget(module);
        module->setNumber(i);

        // 添加电芯
        for (int i = 0; i < 192; ++i) {
            BatteryCellWidget* cell = new BatteryCellWidget;
            cell->setNumber(i+1);
            cell->setVoltage(double(i));
            if(i<2||i>190)
            {
                cell->setHighlighted(true);
            }
            module->addBatteryCell(cell);
        }

    }

    mainWindows->show();
    return a.exec();
}
