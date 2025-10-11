#include <QApplication>
#include "batterycellwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BatteryCellWidget w;
    w.setNumber(3);               // 顶部编号
    w.setCenterText("3.7V");      // 中间文本
    w.show();

    return a.exec();
}
