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
    QLabel *label1 = new QLabel("Hello Aspect Ratio!");
    label1->setStyleSheet("background: lightblue; border: 2px solid blue;");

    // 使用自定义布局，保持 16:9
    qreal ratio = 2/1;
    AspectRatioLayout *layout = new AspectRatioLayout(ratio, &window);
    layout->addWidget(label);
    layout->addWidget(label1);
    label->setText(QString::number(ratio));

    window.setLayout(layout);
    window.resize(800, 600);
    window.show();

    return a.exec();
}
