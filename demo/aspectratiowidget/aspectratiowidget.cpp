#include "aspectratiowidget.h"
#include <QDebug>

AspectRatioWidget::AspectRatioWidget(double ratio, QWidget *parent)
    : QWidget(parent), m_ratio(ratio)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void AspectRatioWidget::setRatio(const QString &ratioStr)
{
    bool ok;
    double a = ratioStr.split(':')[0].toDouble(&ok);
    if(!ok) return;
    double b = ratioStr.split(':')[1].toDouble(&ok);
    if(!ok) return;
    m_ratio=a/b;
    qDebug()<<m_ratio;
}

QString AspectRatioWidget::ratio() const
{
    return QString::number(m_ratio)+"/1";
}

void AspectRatioWidget::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    int w = newSize.width();
    int h = newSize.height();

    // 按宽度算高度
    int expectedH = int(w / m_ratio);

    if (expectedH <= h) {
        // 宽度主导
        resize(w, expectedH);
    } else {
        // 高度主导
        int expectedW = int(h * m_ratio);
        resize(expectedW, h);
    }
    qDebug()<<rect();
}

void AspectRatioWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 背景
    p.fillRect(rect(), QColor(200, 220, 250));

    // 画个边框看看
    p.setPen(QPen(Qt::blue, 4));
    p.drawRect(rect().adjusted(2, 2, -2, -2));

    // 画比例文字
    p.setPen(Qt::black);
    p.drawText(rect(), Qt::AlignCenter,
               QString("Aspect Ratio %1:%2")
               .arg(m_ratio, 0, 'f', 2)
               .arg(1.0));
}
