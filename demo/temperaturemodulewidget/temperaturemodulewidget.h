#ifndef TEMPERATUREMODULEWIDGET_H
#define TEMPERATUREMODULEWIDGET_H

#include <QScrollArea>
#include <QVector>
#include "temperatureprobewidget.h"

class TemperatureModuleWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit TemperatureModuleWidget(QWidget *parent = nullptr);

    // 添加一个温度探头
    void addTemperatureProbe(TemperatureProbeWidget* probe);

    // 清空所有探头
    void clearProbes();

    // 设置模块编号
    void setNumber(int num);

    // 更新探头数据
    void updateProbeData(const QVector<int>& temperatures);

    // 设置温度阈值
    void setTemperatureThresholds(int lowThreshold, int highThreshold);

    int getProbeCount(){return m_probes.count();}

    // 获取最高温度和最低温度
    int getMaxTemperature() const { return m_maxTemperature; }
    int getMinTemperature() const { return m_minTemperature; }

    // 获取最高温度和最低温度对应的探头索引
    int getMaxTemperatureProbeIndex() const { return m_maxTemperatureProbeIndex; }
    int getMinTemperatureProbeIndex() const { return m_minTemperatureProbeIndex; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    class ContentWidget;             // 前向声明
    ContentWidget* m_contentWidget;  // 承载所有探头的容器

    QVector<TemperatureProbeWidget*> m_probes;
    void updateLayout();             // 布局探头
    
    int m_lowTemperatureThreshold;     // 低温阈值
    int m_highTemperatureThreshold;    // 高温阈值
    
    // 新增：最高最低温度相关变量
    int m_maxTemperature;              // 当前最高温度
    int m_minTemperature;              // 当前最低温度
    int m_maxTemperatureProbeIndex;       // 最高温度探头索引
    int m_minTemperatureProbeIndex;       // 最低温度探头索引
    
    // 新增：计算最高最低温度并高亮对应探头
    void calculateAndHighlightExtremeTemperatures(const QVector<int>& temperatures);
};

#endif // TEMPERATUREMODULEWIDGET_H