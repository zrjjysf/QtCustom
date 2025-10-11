#include "batterymodulewidget.h"
#include "customscrollbar.h"
#include <QPainter>
#include <QScrollBar>
#include <QFontMetrics>

// -------------------- ContentWidget 实现 --------------------
class BatteryModuleWidget::ContentWidget : public QWidget
{
public:
    explicit ContentWidget(QWidget* parent = nullptr)
        : QWidget(parent), m_number(0) {}

    void setNumber(int num) { m_number = num; update(); }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event)
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const int w = width();
        const int h = height();
        const int topBarHeight = 14;
        const int bottomBarHeight = 14;
        const int numberWidth = 50; // 左上角编号可用宽度

        // 上橙条
        painter.setBrush(QColor("#E97F1A"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, w, topBarHeight);

        // 绘制编号（左上角）
        if (m_number > 0) {
            QRectF numberRect(6, 0, numberWidth, topBarHeight);
            painter.setPen(Qt::white);

            QFont font = painter.font();
            font.setBold(true);
            font.setPointSize(topBarHeight * 0.8); // 高度占比
            painter.setFont(font);

            painter.drawText(numberRect, Qt::AlignVCenter | Qt::AlignLeft, QString::number(m_number));
        }

        // 中灰背景
        painter.setBrush(QColor("#4A5A64"));
        painter.drawRect(0, topBarHeight, w, h - topBarHeight - bottomBarHeight);

        // 下橙条
        painter.setBrush(QColor("#E97F1A"));
        painter.drawRect(0, h - bottomBarHeight, w, bottomBarHeight);
    }

private:
    int m_number;
};

// -------------------- BatteryModuleWidget 实现 --------------------
BatteryModuleWidget::BatteryModuleWidget(QWidget* parent)
    : QScrollArea(parent)
{
    // 设置承载内容的内部控件
    m_contentWidget = new ContentWidget;
    setWidget(m_contentWidget);
    // setWidgetResizable(true);

    // ---------------- 使用 CustomScrollBar --------------------
    CustomScrollBar* vScroll = new CustomScrollBar(Qt::Vertical, this);
    setVerticalScrollBar(vScroll);
    setViewportMargins(0, 0, 0, 0);
}

void BatteryModuleWidget::addBatteryCell(BatteryCellWidget* cell)
{
    cell->setParent(m_contentWidget);
    cell->setVoltageThresholds(2.8, 999); // 默认阈值，可在外部修改
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

void BatteryModuleWidget::resizeEvent(QResizeEvent* event)
{
    QScrollArea::resizeEvent(event);
    updateLayout();
}

void BatteryModuleWidget::updateLayout()
{
    if (m_cells.isEmpty()) {
        m_contentWidget->resize(viewport()->width(), 0);
        return;
    }

    // 电芯尺寸与 BatteryCellWidget 对应
    const int cellWidth = 180;  // margin*2 + leftRectWidth + rightRectWidth
    const int cellHeight = 60;  // rectHeight + margin*2
    const int hSpacing = 5;
    const int vSpacing = 5;

    int availableWidth = viewport()->width();
    int cols = qMax(1, availableWidth / (cellWidth + hSpacing));
    if (cols * (cellWidth + hSpacing) - hSpacing > availableWidth) {
        cols = qMax(1, cols - 1);
    }

    int x = 0;
    int y = 14; // 留出上橙条

    for (int i = 0; i < m_cells.size(); ++i) {
        int col = i % cols;
        int row = i / cols;
        int posX = x + col * (cellWidth + hSpacing);
        int posY = y + row * (cellHeight + vSpacing);
        m_cells[i]->setGeometry(posX, posY, cellWidth, cellHeight);
    }

    int rows = (m_cells.size() + cols - 1) / cols;
    int contentHeight = y + rows * (cellHeight + vSpacing) + 14; // 下橙条高度
    m_contentWidget->resize(viewport()->width(), contentHeight);
}

void BatteryModuleWidget::setNumber(int num)
{
    if (m_contentWidget)
        m_contentWidget->setNumber(num);
}
