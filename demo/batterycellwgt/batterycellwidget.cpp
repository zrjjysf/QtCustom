#include "batterycellwidget.h"
#include <QPainter>
#include <QFontMetrics>

BatteryCellWidget::BatteryCellWidget(QWidget *parent)
    : QWidget(parent), m_number(0)
{
    setMinimumSize(20, 26);
}

void BatteryCellWidget::setNumber(int number)
{
    m_number = number;
    update(); // 触发重绘
}

void BatteryCellWidget::setCenterText(const QString &text)
{
    m_centerText = text;
    update(); // 触发重绘
}

void BatteryCellWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 缩放比例，保持SVG比例适应控件大小
    const double scaleX = width() / 200.0;
    const double scaleY = height() / 260.0;
    painter.scale(scaleX, scaleY);

    // 绘制顶部深棕色盖子
    QRectF topRect(30, 20, 140, 25);
    painter.setBrush(QColor("#2E1E10"));
    painter.setPen(Qt::NoPen);
    painter.drawRect(topRect);

    // 绘制主体橙色部分
    QRectF bodyRect(30, 45, 140, 195);
    painter.setBrush(QColor("#F7931A"));
    painter.drawRect(bodyRect);

    // 绘制编号（靠左）
    painter.setPen(Qt::white);
    QFont numberFont = painter.font();
    numberFont.setBold(true);
    numberFont.setPointSize(12);
    painter.setFont(numberFont);

    QString numberText = QString::number(m_number);
    painter.drawText(QRectF(30, 20, 140, 25), Qt::AlignLeft | Qt::AlignVCenter, numberText);

    // 绘制中间文本
    QFont centerFont = painter.font();
    centerFont.setPointSize(16);
    painter.setFont(centerFont);
    painter.drawText(bodyRect, Qt::AlignCenter, m_centerText);
}
