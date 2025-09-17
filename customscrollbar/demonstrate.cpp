#include <QApplication>
#include <QTextEdit>
#include "customscrollbar.h"

int customscrollbar_demonstrate(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextEdit edit;
    edit.setVerticalScrollBar(new CustomScrollBar(&edit));

    // 填充测试内容
    for (int i = 1; i <= 100; ++i) {
        edit.append(QString("这是第 %1 行内容").arg(i));
    }

    edit.resize(400, 300);
    edit.show();

    return a.exec();
}
