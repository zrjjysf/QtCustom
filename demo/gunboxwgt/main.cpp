#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "gunboxwgt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);

    GunBoxWgt *gun = new GunBoxWgt;
    gun->setTitle("枪 A");
    gun->setIconText("故障");
    gun->setErrorText("故障：充电枪连接异常");

    QPushButton *btnNext = new QPushButton("切换状态");
    layout->addWidget(gun);
    layout->addWidget(btnNext);

    QObject::connect(btnNext, &QPushButton::clicked, [&]() {
        GunBoxWgt::Status st = gun->status();
        GunBoxWgt::Status next = static_cast<GunBoxWgt::Status>((st + 1) % 5);
        gun->setStatus(next);
        if (next == GunBoxWgt::Fault)
            gun->setErrorText("故障：充电枪连接异常");
        else
            gun->setErrorText("");
    });

    window.show();
    return a.exec();
}
