#include "temperatureprobewidget.h"
#include <QPainter>
#include <QFontMetrics>
#include <QDebug>

int TemperatureProbeWidget::margin = 6;
int TemperatureProbeWidget::padding = 6;
int TemperatureProbeWidget::probeWidth = 100;
int TemperatureProbeWidget::probeHeight = 72;

TemperatureProbeWidget::TemperatureProbeWidget(QWidget *parent)
    : QWidget(parent), m_number(0)
{
    setMinimumSize(100, 80);
}

void TemperatureProbeWidget::setNumber(int number)
{
    m_number = number;
    update();
}

void TemperatureProbeWidget::setTemperature(int temperature)
{
    m_temperature = temperature;
    if(m_lowTemperature<m_highTemperature)//阈值正常
    {
        if(m_temperature<=m_lowTemperature)
        {
            m_status = Status::TOO_LOW;
        }
        else if (m_temperature>=m_highTemperature) {
            m_status = Status::TOO_HIGH;
        }
        else {
            m_status = Status::NORMAL;
        }
    }
    else {
        m_status = Status::NORMAL;
    }
    update();
}

void TemperatureProbeWidget::setTemperatureThresholds(int low, int high)
{
    m_lowTemperature = low;
    m_highTemperature = high;
    update();
}

void TemperatureProbeWidget::setHighlighted(bool highlight)
{
    m_highlighted = highlight;
    update();
}


void TemperatureProbeWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 缩放比例保持整体比例
    int scaleX = width() / static_cast<int>(probeWidth+margin*2);
    int scaleY = height() / static_cast<int>(probeHeight+margin*2);
    int scale = qMin(scaleX, scaleY);
    p.scale(scale, scale);

    // ---------------- 外框 ----------------
    QRectF outerRect(margin, margin, probeWidth, probeHeight);
    p.fillRect(outerRect, QColor("#C2BAA7"));

    QSize content(outerRect.width()-2*padding, outerRect.height()-2*padding);
    // ---------------- 显示屏外框 ----------------
    QRectF bodyRect(outerRect.left()+padding, outerRect.top()+padding, content.width(), content.height()*0.68);
    p.fillRect(bodyRect, QColor("#1E1A17"));

    // ---------------- 显示屏 ----------------
    QRectF screenRect(bodyRect.left()+padding, bodyRect.top()+padding, bodyRect.width()-2*padding, bodyRect.height()-2*padding);
    QColor screenColor = QColor("#98EAE8");
    if(m_lowTemperature<m_highTemperature)//阈值正常
    {
        if(m_temperature<=m_lowTemperature)
        {
            screenColor = QColor("blue");
        }
        else if (m_temperature>=m_highTemperature) {
            screenColor = QColor("red");
        }
    }
    p.fillRect(screenRect, screenColor);

    // ---------------- 下方区域 ----------------
    QRectF baseRect(bodyRect.left(), bodyRect.bottom(), content.width(), content.height()-bodyRect.height());
    p.fillRect(baseRect, QColor("#CFCFCF"));

    // ---------------- 编号 ----------------
    QRectF numberRect(baseRect.left() + 10, baseRect.top(), baseRect.width(), baseRect.height());
    QFont fontNum = p.font();
    fontNum.setBold(true);
    fontNum.setPixelSize(static_cast<int>(numberRect.height()*0.7));
    p.setFont(fontNum);
    p.setPen(Qt::black);
    p.drawText(numberRect, Qt::AlignLeft | Qt::AlignVCenter,
               QString("No.%1").arg(QString::number(m_number)));

    // ---------------- 温度 ----------------
    QRectF tempRect(screenRect.right()-screenRect.width(), screenRect.top(), screenRect.width(), screenRect.height());
    QFont fontTemp = p.font();
    fontTemp.setBold(true);
    fontTemp.setPixelSize(static_cast<int>(screenRect.height()*0.7));
    p.setFont(fontTemp);
    p.setPen(Qt::black);
    QString tempText = QString::number(m_temperature) + "°C";
    p.drawText(tempRect, Qt::AlignRight | Qt::AlignVCenter, tempText);

    // ---------------- 高亮边框 ----------------
    if (m_highlighted) {
        QPen pen(QColor("#FFD700"), 6 / scale);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);
        p.drawRect(margin/2, margin/2, probeWidth+margin, probeHeight+margin);
    }
}
