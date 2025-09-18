#include "resizeeventwidget.h"

ResizeEventWidget::ResizeEventWidget(double ratio, QWidget *parent)
    : QWidget(parent), m_ratio(ratio)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ResizeEventWidget::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    int w = newSize.width();
    int h = newSize.height();

    int expectedH = int(w / m_ratio);

    if (expectedH <= h) {
        resize(w, expectedH);
    } else {
        int expectedW = int(h * m_ratio);
        resize(expectedW, h);
    }
}

void ResizeEventWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), QColor(250, 220, 220));
    p.setPen(QPen(Qt::red, 3));
    p.drawRect(rect().adjusted(2, 2, -2, -2));
    p.drawText(rect(), Qt::AlignCenter, "ResizeEvent Hack");
}
