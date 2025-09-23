#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "heightforwidthwidget.h"
#include "resizeeventwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget window;
    window.setWindowTitle("Aspect Ratio Widget: 两种实现方式对比");

    auto *hfw = new HeightForWidthWidget(16.0/9.0);
    auto *rew = new ResizeEventWidget(16.0/9.0);

    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(new QLabel("HeightForWidth 实现 (单向 width->height)："));
    layout->addWidget(hfw, 1);
    layout->addSpacing(20);
    layout->addWidget(new QLabel("ResizeEvent Hack 实现 (双向强制调整)："));
    layout->addWidget(rew, 1);

    window.resize(800, 600);
    window.show();

    return a.exec();
}
