#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include "batterypackwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 主窗口
    QMainWindow window;
    window.setWindowTitle("BatteryPackWidget 测试");
    QWidget *central = new QWidget;
    QGridLayout *grid = new QGridLayout(central);

    // 创建多个电池包控件
    const int rows = 2;
    const int cols = 3;
    BatteryPackWidget* packs[rows][cols];

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            BatteryPackWidget* pack = new BatteryPackWidget;
            pack->setPackID(r * cols + c + 1);
            pack->setOpenMode((r + c) % 2 == 0 ? BatteryPackWidget::OPEN : BatteryPackWidget::CLOSE);
            pack->setStatus((r + c) % 2 == 0 ? BatteryPackWidget::NORMAL : BatteryPackWidget::ALARM);
            packs[r][c] = pack;
            grid->addWidget(pack, r, c);
        }
    }

    // 控制按钮
    QPushButton *toggleAll = new QPushButton("切换全部状态");
    grid->addWidget(toggleAll, rows, 0, 1, cols);

    QObject::connect(toggleAll, &QPushButton::clicked, [=]() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                BatteryPackWidget *pack = packs[r][c];
                auto newMode = pack->openMode() == BatteryPackWidget::OPEN
                               ? BatteryPackWidget::CLOSE
                               : BatteryPackWidget::OPEN;
                auto newStatus = pack->status() == BatteryPackWidget::NORMAL
                                 ? BatteryPackWidget::ALARM
                                 : BatteryPackWidget::NORMAL;
                pack->setOpenMode(newMode);
                pack->setStatus(newStatus);
            }
        }
    });

    window.setCentralWidget(central);
    window.resize(600, 400);
    window.show();

    return app.exec();
}
