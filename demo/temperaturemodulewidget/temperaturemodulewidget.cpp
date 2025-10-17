#include "temperaturemodulewidget.h"
#include "customscrollbar.h"
#include <QFontMetrics>
#include <QPainter>
#include <QScrollBar>
#include <limits> // 用于数值极限

// -------------------- ContentWidget 实现 --------------------
class TemperatureModuleWidget::ContentWidget : public QWidget {
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

// -------------------- TemperatureModuleWidget 实现 --------------------
TemperatureModuleWidget::TemperatureModuleWidget(QWidget *parent)
    : QScrollArea(parent), 
      m_maxTemperature(0.0),
      m_minTemperature(0.0),
      m_maxTemperatureProbeIndex(-1),
      m_minTemperatureProbeIndex(-1) {
  // 设置承载内容的内部控件
  m_contentWidget = new ContentWidget;
  setWidget(m_contentWidget);

  // 使用 CustomScrollBar
  CustomScrollBar *vScroll = new CustomScrollBar(Qt::Vertical, this);
  setVerticalScrollBar(vScroll);
  setViewportMargins(0, 0, 0, 0);
}

void TemperatureModuleWidget::addTemperatureProbe(TemperatureProbeWidget *probe) {
  probe->setParent(m_contentWidget);
  probe->setTemperatureThresholds(m_lowTemperatureThreshold, m_highTemperatureThreshold);
  m_probes.append(probe);
  probe->show();
  updateLayout();
}

void TemperatureModuleWidget::clearProbes() {
  for (auto probe : m_probes) {
    probe->setParent(nullptr);
    probe->deleteLater();
  }
  m_probes.clear();
  
  // 重置最高最低温度数据
  m_maxTemperature = 0.0;
  m_minTemperature = 0.0;
  m_maxTemperatureProbeIndex = -1;
  m_minTemperatureProbeIndex = -1;
  
  updateLayout();
}

void TemperatureModuleWidget::updateProbeData(const QVector<int>& temperatures) {
    // 处理探头数量不匹配的情况
    int temperatureCount = temperatures.size();
    int currentProbeCount = m_probes.size();
    
    // 如果传入的温度数量比当前探头多，创建新的探头
    if (temperatureCount > currentProbeCount) {
        for (int i = currentProbeCount; i < temperatureCount; ++i) {
            TemperatureProbeWidget* newProbe = new TemperatureProbeWidget(m_contentWidget);
            newProbe->setNumber(i + 1); // 探头编号从1开始
            newProbe->setTemperatureThresholds(m_lowTemperatureThreshold, m_highTemperatureThreshold);
            m_probes.append(newProbe);
            newProbe->show();
        }
    }
    // 如果传入的温度数量比当前探头少，删除多余的探头
    else if (temperatureCount < currentProbeCount) {
        for (int i = currentProbeCount - 1; i >= temperatureCount; --i) {
            TemperatureProbeWidget* probe = m_probes.takeAt(i);
            probe->setParent(nullptr);
            probe->deleteLater();
        }
    }
    
    Status currentStatus = Status::NORMAL;
    // 更新所有探头的温度数据
    for (int i = 0; i < temperatures.size(); ++i) {
        if (i < m_probes.size()) {
            m_probes[i]->setTemperature(temperatures[i]);
            if(m_probes[i]->currentStatus() != TemperatureProbeWidget::Status::NORMAL)
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
    
    // 计算最高最低温度并高亮对应探头
    calculateAndHighlightExtremeTemperatures(temperatures);
    
    // 更新布局
    updateLayout();
}

void TemperatureModuleWidget::calculateAndHighlightExtremeTemperatures(const QVector<int>& temperatures) {
    if (temperatures.isEmpty()) {
        m_maxTemperature = 0.0;
        m_minTemperature = 0.0;
        m_maxTemperatureProbeIndex = -1;
        m_minTemperatureProbeIndex = -1;
        return;
    }
    
    // 首先取消所有探头的高亮状态
    for (auto probe : m_probes) {
        probe->setHighlighted(false);
    }
    
    // 查找最高温度和最低温度
    m_maxTemperature = temperatures[0];
    m_minTemperature = temperatures[0];
    m_maxTemperatureProbeIndex = 0;
    m_minTemperatureProbeIndex = 0;
    
    for (int i = 1; i < temperatures.size(); ++i) {
        if (temperatures[i] > m_maxTemperature) {
            m_maxTemperature = temperatures[i];
            m_maxTemperatureProbeIndex = i;
        }
        if (temperatures[i] < m_minTemperature) {
            m_minTemperature = temperatures[i];
            m_minTemperatureProbeIndex = i;
        }
    }
    
    // 高亮最高温度和最低温度的探头
    if (m_maxTemperatureProbeIndex >= 0 && m_maxTemperatureProbeIndex < m_probes.size()) {
        m_probes[m_maxTemperatureProbeIndex]->setHighlighted(true);
    }
    if (m_minTemperatureProbeIndex >= 0 && m_minTemperatureProbeIndex < m_probes.size()) {
        m_probes[m_minTemperatureProbeIndex]->setHighlighted(true);
    }
    
    // 如果最高温度和最低温度是同一个探头，确保它只被设置一次
}

void TemperatureModuleWidget::setTemperatureThresholds(int lowThreshold, int highThreshold) {
    m_lowTemperatureThreshold = lowThreshold;
    m_highTemperatureThreshold = highThreshold;
    
    // 更新所有现有探头的阈值
    for (auto probe : m_probes) {
        probe->setTemperatureThresholds(lowThreshold, highThreshold);
    }
}

void TemperatureModuleWidget::resizeEvent(QResizeEvent *event) {
  QScrollArea::resizeEvent(event);
  updateLayout();
}

void TemperatureModuleWidget::updateLayout()
{
    if (m_probes.isEmpty()) {
        m_contentWidget->resize(viewport()->width(), 0);
        return;
    }

    // ---- 探头尺寸 ----
    // 使用TemperatureProbeWidget的静态尺寸定义
    const int probeWidth = TemperatureProbeWidget::probeWidth + 
                          TemperatureProbeWidget::margin * 2;
    const int probeHeight = TemperatureProbeWidget::probeHeight + 
                           TemperatureProbeWidget::margin * 2;

    // ---- 间距定义 ----
    const int minHSpacing = 2;   // 最小水平间距
    const int vSpacing = 2;      // 垂直间距
    const int topBarHeight = 14;
    const int bottomBarHeight = 14;

    int availableWidth = viewport()->width();

    // ---- 计算最大列数 ----
    int cols = 1;
    for (;; ++cols) {
        int totalWidth = cols * probeWidth + (cols + 1) * minHSpacing;
        if (totalWidth > availableWidth) {
            cols = qMax(1, cols - 1);
            break;
        }
    }

    // ---- 平均分配横向多余空间 ----
    int totalProbesWidth = cols * probeWidth;
    int totalExtraSpace = availableWidth - totalProbesWidth;
    int hSpacing = totalExtraSpace / int(cols + 1);

    // ---- 布局探头 ----
    int y0 = topBarHeight;

    for (int i = 0; i < m_probes.size(); ++i) {
        int col = i % cols;
        int row = i / cols;

        int posX = static_cast<int>(hSpacing * (col + 1) + probeWidth * col);
        int posY = static_cast<int>(y0 + row * (probeHeight + vSpacing));

        m_probes[i]->setGeometry(posX, posY, probeWidth, probeHeight);
    }

    // ---- 更新内容高度 ----
    int rows = (m_probes.size() + cols - 1) / cols;
    int contentHeight = y0 + rows * (probeHeight + vSpacing) + bottomBarHeight;
    m_contentWidget->resize(viewport()->width(), contentHeight);
}

void TemperatureModuleWidget::setNumber(int num) {
  if (m_contentWidget)
    m_contentWidget->setNumber(num);
}