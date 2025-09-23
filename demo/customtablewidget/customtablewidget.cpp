#include "customtablewidget.h"
#include <QHeaderView>

CustomTableWidget::CustomTableWidget(QWidget *parent)
    : QTableWidget(parent)
{

}

void CustomTableWidget::paintEvent(QPaintEvent *event)
{
    // 先正常绘制表格
    QTableWidget::paintEvent(event);

    QPainter p(viewport());
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);

    int usedHeight = 0;
    for (int r = 0; r < rowCount(); ++r) {
        usedHeight += rowHeight(r);
    }

    // 覆盖整张表的空白区域
    QRect rect(0, usedHeight, viewport()->width(), viewport()->height() - usedHeight);
    if (rect.height() > 0) {
        p.drawRect(rect);
    }
}


