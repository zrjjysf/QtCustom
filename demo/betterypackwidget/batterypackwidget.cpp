#include "batterypackwidget.h"
#include <QApplication>
#include <QStyle>
#include "themesvgwgt.h"
#include <QGridLayout>
#include <QDebug>

BatteryPackWidget::BatteryPackWidget(QWidget *parent)
    : QWidget(parent)
    , m_isOn(true)
    , m_isAlarm(false)
    , m_packId(0)
{
    // 创建SVG控件
    m_closedSvg = new ThemeSvgWgt(this);
    m_openSvg = new ThemeSvgWgt(this);
    
    // 创建ID标签 - 叠加在SVG之上
    m_idLabel = new QLabel(this);
    m_idLabel->setAlignment(Qt::AlignCenter);
    m_idLabel->setStyleSheet("QLabel {"
                             "color: white;"
                             "font: bold 100px;"
                             "background: transparent;"
                             "border: none;"
                             "}");
    
    // 使用网格布局让SVG控件重叠
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
    
    // 将两个SVG控件放在同一位置
    gridLayout->addWidget(m_closedSvg, 0, 0);
    gridLayout->addWidget(m_openSvg, 0, 0);
    
    // 标签不使用布局，手动定位
    m_idLabel->setParent(this);
    
    // 设置初始大小建议
    setMinimumSize(20, 20); // 最小尺寸
    
    // 初始更新外观
    updateAppearance();
}

QSize BatteryPackWidget::sizeHint() const
{
    // 返回一个合适的默认大小，保持1:1比例
    return QSize(400, 400);
}

// int BatteryPackWidget::heightForWidth(int width) const
// {
//     return width;
//     qDebug()<<"heightForWidth"<<width;
// }

// void BatteryPackWidget::resizeEvent(QResizeEvent *event)
// {
//     QWidget::resizeEvent(event);
    
//     // 计算1:1的正方形区域
//     // int side = qMin(width(), height());
//     // int x = (width() - side) / 2;
//     // int y = (height() - side) / 2;
    
//     // // 设置SVG控件为正方形
//     // m_closedSvg->setGeometry(x, y, side, side);
//     // m_openSvg->setGeometry(x, y, side, side);
    
//     // // 手动设置标签位置，使其显示在下半部分区域
//     // // 在正方形区域内定位
//     // int labelHeight = side / 8; // 根据正方形大小调整标签高度
//     // int labelY = y + side * 2 / 3 - labelHeight / 2; // 在下半部分的中央
    
//     m_idLabel->setGeometry(m_closedSvg->x(), m_closedSvg->y(), m_closedSvg->width(), m_closedSvg->height());
    
//     // 根据正方形大小动态调整字体大小
//     int fontSize = side / 10; // 字体大小与正方形大小成比例
//     m_idLabel->setStyleSheet(QString("QLabel {"
//                                      "color: white;"
//                                      "font: bold %1px;"
//                                      "background: transparent;"
//                                      "border: none;"
//                                      "}").arg(fontSize));
//     qDebug()<<size();
// }

void BatteryPackWidget::setStatus(bool isAlarm)
{
    if (m_isAlarm != isAlarm) {
        m_isAlarm = isAlarm;
        updateAppearance();
        emit statusChanged(m_isAlarm);
    }
}

void BatteryPackWidget::setSwitchState(bool isOn)
{
    if (m_isOn != isOn) {
        m_isOn = isOn;
        updateAppearance();
        emit switchStateChanged(m_isOn);
    }
}

void BatteryPackWidget::setId(int packId)
{
    if (m_packId != packId) {
        m_packId = packId;
        m_idLabel->setText(QString("ID: %1").arg(m_packId));
        emit idChanged(m_packId);
    }
}

void BatteryPackWidget::updateAppearance()
{
    // 根据状态设置颜色
    QString color = m_isAlarm ? "red" : "#f49c39";
    
    // 设置样式表
    QString style = QString("ThemeSvgWgt {"
                           "color: %1;"
                           "background: transparent;"
                           "}").arg(color);
    
    setStyleSheet(style);
    
    // 设置SVG路径
    if (m_isOn) {
        // 关闭状态的SVG路径
        m_closedSvg->setSvgPath("./package.svg");
        m_closedSvg->show();
        m_openSvg->hide();
    } else {
        // 打开状态的SVG路径
        m_openSvg->setSvgPath("./package_open.svg");
        m_openSvg->show();
        m_closedSvg->hide();
    }
    
    // 更新ID显示
    m_idLabel->setText(QString("ID: %1").arg(m_packId));
    
    // 确保标签在最上层
    m_idLabel->raise();
}