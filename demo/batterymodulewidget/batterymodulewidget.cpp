#include "batterymodulewidget.h"
#include <QPainter>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

BatteryModuleWidget::BatteryModuleWidget(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    // 内容容器
    m_contentWidget = new QWidget;
    m_contentWidget->setGeometry(0,0,viewport()->width(), viewport()->height());
    setViewport(m_contentWidget);

    // 启用滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void BatteryModuleWidget::addBatteryCell(BatteryCellWidget* cell)
{
    cell->setParent(m_contentWidget);
    m_cells.append(cell);
    cell->show();
    updateLayout();
}

void BatteryModuleWidget::clearCells()
{
    for (auto cell : m_cells) {
        cell->setParent(nullptr);
        cell->deleteLater();
    }
    m_cells.clear();
    updateLayout();
}

void BatteryModuleWidget::resizeEvent(QResizeEvent *event)
{
    QAbstractScrollArea::resizeEvent(event);
    updateLayout();
}

void BatteryModuleWidget::updateLayout()
{
    if (m_cells.isEmpty()) return;

    // 电芯尺寸
    const int cellWidth = 80;
    const int cellHeight = 104; // 200*0.5大约
    const int hSpacing = 10;
    const int vSpacing = 10;

    int cols = qMax(1, viewport()->width() / (cellWidth + hSpacing));
    int x = 6;
    int y = 16;

    for (int i = 0; i < m_cells.size(); ++i) {
        int col = i % cols;
        int row = i / cols;
        int posX = x + col * (cellWidth + hSpacing);
        int posY = y + row * (cellHeight + vSpacing);
        m_cells[i]->setGeometry(posX, posY, cellWidth, cellHeight);
    }

    // 更新内容区域高度
    int rows = (m_cells.size() + cols - 1) / cols;
    int contentHeight = y + rows * (cellHeight + vSpacing) + 16; // 下方预留
    m_contentWidget->resize(viewport()->width(), contentHeight);
}

void BatteryModuleWidget::paintEvent(QPaintEvent *event)
{
    QAbstractScrollArea::paintEvent(event);

    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);

    const int w = viewport()->width();
    const int h = viewport()->height();

    // 上方橙色条
    painter.setBrush(QColor("#E97F1A"));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, w, 14);

    // 中间深灰背景
    painter.setBrush(QColor("#4A5A64"));
    painter.drawRect(6, 16, w - 12, h - 32);

    // 下方橙色条
    painter.setBrush(QColor("#E97F1A"));
    painter.drawRect(0, h - 14, w, 14);
}
