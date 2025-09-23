#include "heightforwidthwidget.h"

HeightForWidthWidget::HeightForWidthWidget(double ratio, QWidget *parent)
    : QWidget(parent), m_ratio(ratio)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void HeightForWidthWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), QColor(220, 250, 220));
    p.setPen(QPen(Qt::darkGreen, 3));
    p.drawRect(rect().adjusted(2, 2, -2, -2));
    p.drawText(rect(), Qt::AlignCenter, "HeightForWidth");
}
