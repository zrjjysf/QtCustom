#include <QApplication>
#include <QWidget>
#include <QLabel>
#include "aspectratiolayout.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget window;
    window.setWindowTitle("AspectRatioLayout Demo");

    // 这里随便放个控件，比如 QLabel
    QLabel *label = new QLabel("Hello Aspect Ratio!");
    label->setStyleSheet("background: lightblue; border: 2px solid blue;");

    // 使用自定义布局，保持 16:9
    AspectRatioLayout *layout = new AspectRatioLayout(16.0/9.0, &window);
    layout->addWidget(label);

    window.setLayout(layout);
    window.resize(800, 600);
    window.show();

    return a.exec();
}
