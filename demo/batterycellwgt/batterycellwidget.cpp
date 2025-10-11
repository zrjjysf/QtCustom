#include "batterycellwidget.h"
#include <QPainter>
#include <QFontMetrics>

const int margin = 10;
const int leftRectWidth = 40;
const int rightRectWidth = 120;
const int rectHeight = 40;

BatteryCellWidget::BatteryCellWidget(QWidget *parent)
    : QWidget(parent), m_number(0)
{
    setMinimumSize(18, 10);
}

void BatteryCellWidget::setNumber(int number)
{
    m_number = number;
    update();
}

void BatteryCellWidget::setVoltage(double voltage)
{
    m_voltage = voltage;
    update();
}

void BatteryCellWidget::setVoltageThresholds(double low, double high)
{
    m_lowVoltage = low;
    m_highVoltage = high;
    update();
}

void BatteryCellWidget::setHighlighted(bool highlight)
{
    m_highlighted = highlight;
    update();
}

// ---------------- paintEvent ----------------
void BatteryCellWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const double scaleX = width() / (margin*2 + leftRectWidth + rightRectWidth);
    const double scaleY = height() / (rectHeight + margin*2);
    painter.scale(scaleX, scaleY);

    // ---------------- 左侧深棕色盖子 ----------------
    QRectF leftRect(margin, margin, leftRectWidth, rectHeight);
    painter.setBrush(QColor("#2E1E10"));
    painter.setPen(Qt::NoPen);
    painter.drawRect(leftRect);

    // ---------------- 右侧主体颜色 ----------------
    QRectF bodyRect(margin + leftRectWidth, margin, rightRectWidth, rectHeight);
    QColor bodyColor = QColor("#388E3C"); // 默认绿色
    if (m_lowVoltage < m_highVoltage) {
        if (m_voltage < m_lowVoltage) bodyColor = QColor("#D32F2F");      // 红色
        else if (m_voltage > m_highVoltage) bodyColor = QColor("#4CAF50"); // 绿色
    }
    painter.setBrush(bodyColor);
    painter.drawRect(bodyRect);

    painter.setPen(Qt::white);

    // ---------------- 编号 ----------------
    {
        QString numberText = QString::number(m_number);
        QFont numberFont = painter.font();
        numberFont.setBold(true);
        numberFont.setPointSize(rectHeight / 3);
        painter.setFont(numberFont);
        painter.drawText(leftRect, Qt::AlignCenter, numberText);
    }

    // ---------------- 电压文本 ----------------
    {
        QString voltageText = QString::number(m_voltage, 'f', 1) + "V";
        QFont voltageFont = painter.font();
        voltageFont.setBold(true);
        voltageFont.setPointSize(rectHeight / 3);
        painter.setFont(voltageFont);
        painter.drawText(bodyRect, Qt::AlignCenter, voltageText);
    }

    // ---------------- 高亮边框 ----------------
    if (m_highlighted) {
        QPen pen(QColor("#FFD700")); // 黄色
        pen.setWidth(3);             // 粗边框
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(margin/2, margin/2,
                         leftRectWidth + rightRectWidth + margin,
                         rectHeight + margin);
    }
}
