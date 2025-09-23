#include <QApplication>
#include "customtablewidget.h"
#include "tool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(getQSS());

    CustomTableWidget table;
    table.setColumnCount(2);
    table.setRowCount(9);

    // 设置表头
    QStringList headers;
    headers << "设备名称" << "状态";
    table.setHorizontalHeaderLabels(headers);

    // 设置行表头（1,2,3,...）
    QStringList rowHeaders;
    for (int i = 1; i <= table.rowCount(); ++i) {
        rowHeaders << QString::number(i);
    }
    table.setVerticalHeaderLabels(rowHeaders);

    // 示例数据
    QStringList names = {"交流接触器","门禁","急停","交流枪锁","交流插枪",
                         "K9","K10","K11","K12"};
    for (int i = 0; i < names.size(); ++i) {
        table.setItem(i, 0, new QTableWidgetItem(names.at(i)));
        table.setItem(i, 1, new QTableWidgetItem("错误"));
    }

    table.resize(500, 400);
    table.setWindowTitle("设备状态表");
    table.show();

    return a.exec();
}
