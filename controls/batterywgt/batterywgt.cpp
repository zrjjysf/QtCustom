#include "batterywgt.h"
#include <QPainter>
#include <QFont>
#include <algorithm>
#include <QStyle>
#include <QDebug>
#include <QStyleOptionFrame>
#include <QResizeEvent>

BatteryWgt::BatteryWgt(bool showText, QWidget *parent)
    : QFrame(parent), m_needShowText(showText),m_svgRect(96, 224, 896, 576)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setTextVisible(showText);
    style()->unpolish(this);
    style()->polish(this);
}

QSize BatteryWgt::sizeHint() const
{
    // 默认尺寸为 128x128，适合显示电池图形和文本
    return QSize(12, 12);
}
void BatteryWgt::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    int w = newSize.width();
    int h = newSize.height();
    double aspectRatio = m_svgRect.width() / m_svgRect.height();

    // 按宽度算高度
    int expectedH = int(w / aspectRatio);

    if (expectedH <= h) {
        // 宽度主导
        resize(w, expectedH);
    } else {
        // 高度主导
        int expectedW = int(h * aspectRatio);
        resize(expectedW, h);
    }
    qDebug()<<rect();
}

void BatteryWgt::addConfig(int threshold, QColor col)
{
    m_configMap[threshold] = col;
    update();
}

void BatteryWgt::setBatteryPercent(int percent)
{
    m_iBatteryPercent = percent;
    update();
}

void BatteryWgt::setTextVisible(bool show)
{
    m_needShowText = show;
    update();
}

void BatteryWgt::setBatteryColorConfig(const QString &configStr)
{
    m_configMap.clear();
    const QStringList entries = configStr.split(';');
    for (const QString &entry : entries) {
        QStringList pair = entry.split(':');
        if (pair.size() == 2) {
            bool ok = false;
            int threshold = pair[0].toInt(&ok);
            QColor color(pair[1]);
            if (ok && color.isValid()) {
                addConfig(threshold, color);
            }
        }
    }
    qDebug()<<m_configMap;
}

QString BatteryWgt::batteryColorConfig() const
{
    QStringList result;
    for (auto it = m_configMap.constBegin(); it != m_configMap.constEnd(); ++it) {
        result << QString("%1:%2").arg(it.key()).arg(it.value().name());
    }
    return result.join(";");
}

void BatteryWgt::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 电池图形原始尺寸（SVG参考）
    double aspectRatio = m_svgRect.width() / m_svgRect.height();


    {
        // 留出边距以防边框被裁剪
        // const qreal margin = 4.0;
        QRectF availableRect = contentsRect();
        // QRectF availableRect = rect().adjusted(margin, margin, -margin, -margin);

        // 计算缩放后的目标区域
        QRectF targetRect;
        if (availableRect.width() / aspectRatio <= availableRect.height()) {
            double th = availableRect.width() / aspectRatio;
            targetRect = QRectF(availableRect.left(), availableRect.top() + (availableRect.height() - th) / 2,
                                availableRect.width(), th);
        } else {
            double tw = availableRect.height() * aspectRatio;
            targetRect = QRectF(availableRect.left() + (availableRect.width() - tw) / 2, availableRect.top(),
                                tw, availableRect.height());
        }

        // 坐标映射函数
        auto mapX = [&](qreal x) { return targetRect.left() + (x - m_svgRect.left()) / m_svgRect.width() * targetRect.width(); };
        auto mapY = [&](qreal y) { return targetRect.top() + (y - m_svgRect.top()) / m_svgRect.height() * targetRect.height(); };
        auto mapW = [&](qreal w) { return w / m_svgRect.width() * targetRect.width(); };
        auto mapH = [&](qreal h) { return h / m_svgRect.height() * targetRect.height(); };

        // 外壳
        qreal shellPenWidth = mapW(32);
        QRectF outer(mapX(96) + shellPenWidth / 2, mapY(224) + shellPenWidth / 2,
                    mapW(832) - shellPenWidth, mapH(576) - shellPenWidth);
        painter.setPen(QPen(QColor("#111"), shellPenWidth));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(outer, mapW(80), mapH(80));

        // 正极端子
        QRectF terminal(mapX(912), mapY(400), mapW(64), mapH(224));
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#111"));
        painter.drawRoundedRect(terminal, mapW(20), mapH(20));

        // 内部背景
        QRectF inner(mapX(136), mapY(264), mapW(752), mapH(496));
        painter.setBrush(QColor("#eee"));
        painter.drawRoundedRect(inner, mapW(56), mapH(56));

        // 电量颜色选择
        QColor chargeColor = QColor("#76c043");
        if (m_iBatteryPercent == -1) {
            chargeColor = Qt::gray;
        } else {
            QList<int> keys = m_configMap.keys();
            std::sort(keys.begin(), keys.end());
            for (int key : keys) {
                if (m_iBatteryPercent <= key) {
                    chargeColor = m_configMap.value(key);
                    break;
                }
            }
        }

        // 电量条
        if (m_iBatteryPercent >= 0) {
            qreal chargeWidth = mapW(752) * m_iBatteryPercent / 100.0;
            QRectF charge(inner.left(), inner.top(), chargeWidth, inner.height());
            painter.setBrush(chargeColor);
            painter.drawRoundedRect(charge, mapW(56), mapH(56));
        }

        // 内部边框
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor("#bbb"), mapW(8)));
        painter.drawRoundedRect(inner, mapW(56), mapH(56));

        // 电量文本
        if (m_needShowText) {
            QString text = m_iBatteryPercent == -1 ? "?" : QString("%1%").arg(m_iBatteryPercent);
            QFont font = painter.font();
            font.setPixelSize(inner.height() * 0.4);
            painter.setFont(font);
            painter.setPen(Qt::black);
            painter.drawText(inner, Qt::AlignCenter, text);
        }
    }

}
