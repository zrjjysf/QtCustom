#include "batterymodulewidget.h"
#include "customscrollbar.h"
#include <QFontMetrics>
#include <QPainter>
#include <QScrollBar>
#include <limits> // 用于数值极限
#include <QDebug>

// -------------------- ContentWidget 实现 --------------------
class BatteryModuleWidget::ContentWidget : public QWidget {
public:
  explicit ContentWidget(QWidget *parent = nullptr)
      : QWidget(parent), m_number(0) {}

  void setNumber(int num) {
    m_number = num;
    update();
  }

protected:
  void paintEvent(QPaintEvent *event) override {
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

      painter.drawText(numberRect, Qt::AlignVCenter | Qt::AlignLeft,
                       QString::number(m_number));
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
BatteryModuleWidget::BatteryModuleWidget(QWidget *parent)
    : QScrollArea(parent), 
      m_maxVoltage(0.0f),
      m_minVoltage(0.0f),
      m_maxVoltageCellIndex(-1),
      m_minVoltageCellIndex(-1) {
  // 设置承载内容的内部控件
  m_contentWidget = new ContentWidget;
  setWidget(m_contentWidget);

  // 使用 CustomScrollBar
  CustomScrollBar *vScroll = new CustomScrollBar(Qt::Vertical, this);
  vScroll->setStyleSheet(R"(CustomScrollBar
    {
        min-width:25px; 
        min-height:25px; 
    })");
  setVerticalScrollBar(vScroll);
  setViewportMargins(0, 0, 0, 0);
}

void BatteryModuleWidget::addBatteryCell(BatteryCellWidget *cell) {
  cell->setParent(m_contentWidget);
  cell->setVoltageThresholds(m_lowVoltageThreshold, m_highVoltageThreshold);
  m_cells.append(cell);
  cell->show();
  updateLayout();
}

void BatteryModuleWidget::clearCells() {
  for (auto cell : m_cells) {
    cell->setParent(nullptr);
    cell->deleteLater();
  }
  m_cells.clear();
  
  // 重置最高最低电压数据
  m_maxVoltage = 0.0f;
  m_minVoltage = 0.0f;
  m_maxVoltageCellIndex = -1;
  m_minVoltageCellIndex = -1;
  
  updateLayout();
}

void BatteryModuleWidget::updateCellData(const QVector<float>& voltages) {
    // 处理电芯数量不匹配的情况
    int voltageCount = voltages.size();
    int currentCellCount = m_cells.size();
    
    // 如果传入的电压数量比当前电芯多，创建新的电芯
    if (voltageCount > currentCellCount) {
        for (int i = currentCellCount; i < voltageCount; ++i) {
            BatteryCellWidget* newCell = new BatteryCellWidget(m_contentWidget);
            newCell->setNumber(i + 1); // 电芯编号从1开始
            newCell->setVoltageThresholds(m_lowVoltageThreshold, m_highVoltageThreshold);
            m_cells.append(newCell);
            newCell->show();
        }
    }
    // 如果传入的电压数量比当前电芯少，删除多余的电芯
    else if (voltageCount < currentCellCount) {
        for (int i = currentCellCount - 1; i >= voltageCount; --i) {
            BatteryCellWidget* cell = m_cells.takeAt(i);
            cell->setParent(nullptr);
            cell->deleteLater();
        }
    }
    
    // 更新所有电芯的电压数据,顺便判断状态
    Status currentStatus = Status::NORMAL;
    for (int i = 0; i < voltages.size(); ++i) {
        if (i < m_cells.size()) {
            m_cells[i]->setVoltage(voltages[i]);
            if(m_cells[i]->currentStatus() != BatteryCellWidget::Status::NORMAL)
            {
                currentStatus = Status::ALARM;
            }
        }
    }
    if(m_currentStatus!=currentStatus)
    {
        m_currentStatus = currentStatus;
        emit statusChanged(m_currentStatus);
    }
    
    // 计算最高最低电压并高亮对应电芯
    calculateAndHighlightExtremeVoltages(voltages);
    
    // 更新布局
    updateLayout();
}

void BatteryModuleWidget::calculateAndHighlightExtremeVoltages(const QVector<float>& voltages) {
    if (voltages.isEmpty()) {
        m_maxVoltage = 0.0f;
        m_minVoltage = 0.0f;
        m_maxVoltageCellIndex = -1;
        m_minVoltageCellIndex = -1;
        return;
    }
    
    // 首先取消所有电芯的高亮状态
    for (auto cell : m_cells) {
        cell->setHighlighted(false);
    }
    
    // 查找最高电压和最低电压
    m_maxVoltage = voltages[0];
    m_minVoltage = voltages[0];
    m_maxVoltageCellIndex = 0;
    m_minVoltageCellIndex = 0;
    
    for (int i = 1; i < voltages.size(); ++i) {
        if (voltages[i] > m_maxVoltage) {
            m_maxVoltage = voltages[i];
            m_maxVoltageCellIndex = i;
        }
        if (voltages[i] < m_minVoltage) {
            m_minVoltage = voltages[i];
            m_minVoltageCellIndex = i;
        }
    }
    
    // 高亮最高电压和最低电压的电芯
    if (m_maxVoltageCellIndex >= 0 && m_maxVoltageCellIndex < m_cells.size()) {
        m_cells[m_maxVoltageCellIndex]->setHighlighted(true);
    }
    if (m_minVoltageCellIndex >= 0 && m_minVoltageCellIndex < m_cells.size()) {
        m_cells[m_minVoltageCellIndex]->setHighlighted(true);
    }
    
    // 如果最高电压和最低电压是同一个电芯，确保它只被设置一次（虽然重复设置也没问题）
}

void BatteryModuleWidget::setVoltageThresholds(float lowThreshold, float highThreshold) {
    m_lowVoltageThreshold = lowThreshold;
    m_highVoltageThreshold = highThreshold;
    
    // 更新所有现有电芯的阈值
    for (auto cell : m_cells) {
        cell->setVoltageThresholds(lowThreshold, highThreshold);
    }
}

void BatteryModuleWidget::resizeEvent(QResizeEvent *event) {
  QScrollArea::resizeEvent(event);
  updateLayout();
}

void BatteryModuleWidget::updateLayout()
{
    if (m_cells.isEmpty()) {
        m_contentWidget->resize(viewport()->width(), 0);
        return;
    }

    // ---- 电芯尺寸 ----
    const int cellWidth  = BatteryCellWidget::leftRectWidth +
                           BatteryCellWidget::rightRectWidth +
                           BatteryCellWidget::margin * 2;
    const int cellHeight = BatteryCellWidget::rectHeight +
                           BatteryCellWidget::margin * 2;

    // ---- 间距定义 ----
    const int minHSpacing = 1;   // 最小水平间距
    const int vSpacing     = 1;  // 更紧凑的垂直间距（原来是5）
    const int topBarHeight = 14;
    const int bottomBarHeight = 14;

    int availableWidth = viewport()->width();

    // ---- 计算最大列数 ----
    int cols = 1;
    for (;; ++cols) {
        int totalWidth = cols * cellWidth + (cols + 1) * minHSpacing;
        if (totalWidth > availableWidth) {
            cols = qMax(1, cols - 1);
            break;
        }
    }

    // ---- 平均分配横向多余空间 ----
    int totalCellsWidth = cols * cellWidth;
    int totalExtraSpace = availableWidth - totalCellsWidth;
    double hSpacing = totalExtraSpace / double(cols + 1);

    // ---- 布局电芯 ----
    int y0 = topBarHeight;

    for (int i = 0; i < m_cells.size(); ++i) {
        int col = i % cols;
        int row = i / cols;

        int posX = static_cast<int>(hSpacing * (col + 1) + cellWidth * col);
        int posY = static_cast<int>(y0 + row * (cellHeight + vSpacing));

        m_cells[i]->setGeometry(posX, posY, cellWidth, cellHeight);
    }

    // ---- 更新内容高度 ----
    int rows = (m_cells.size() + cols - 1) / cols;
    int contentHeight = y0 + rows * (cellHeight + vSpacing) + bottomBarHeight;
    m_contentWidget->resize(viewport()->width(), contentHeight);
}

void BatteryModuleWidget::setNumber(int num) {
  if (m_contentWidget)
    m_contentWidget->setNumber(num);
}