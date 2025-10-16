#ifndef BATTERYMODULEWIDGET_H
#define BATTERYMODULEWIDGET_H

#include <QScrollArea>
#include <QVector>
#include "batterycellwidget.h"

class BatteryModuleWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit BatteryModuleWidget(QWidget *parent = nullptr);

    // 添加一个电芯
    void addBatteryCell(BatteryCellWidget* cell);

    // 清空所有电芯
    void clearCells();

    // 设置模块编号
    void setNumber(int num);

    // 更新电芯数据
    void updateCellData(const QVector<float>& voltages);

    // 设置电压阈值
    void setVoltageThresholds(float lowThreshold, float highThreshold);

    int getCellCount(){return m_cells.count();}

    // 获取最高电压和最低电压
    float getMaxVoltage() const { return m_maxVoltage; }
    float getMinVoltage() const { return m_minVoltage; }

    // 获取最高电压和最低电压对应的电芯索引
    int getMaxVoltageCellIndex() const { return m_maxVoltageCellIndex; }
    int getMinVoltageCellIndex() const { return m_minVoltageCellIndex; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    class ContentWidget;             // 前向声明
    ContentWidget* m_contentWidget;  // 承载所有电芯的容器

    QVector<BatteryCellWidget*> m_cells;
    void updateLayout();             // 布局电芯
    
    float m_lowVoltageThreshold;     // 低压阈值
    float m_highVoltageThreshold;    // 高压阈值
    
    // 新增：最高最低电压相关变量
    float m_maxVoltage;              // 当前最高电压
    float m_minVoltage;              // 当前最低电压
    int m_maxVoltageCellIndex;       // 最高电压电芯索引
    int m_minVoltageCellIndex;       // 最低电压电芯索引
    
    // 新增：计算最高最低电压并高亮对应电芯
    void calculateAndHighlightExtremeVoltages(const QVector<float>& voltages);
};

#endif // BATTERYMODULEWIDGET_H