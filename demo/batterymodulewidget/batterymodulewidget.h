#ifndef BATTERYMODULEWIDGET_H
#define BATTERYMODULEWIDGET_H

#include <QScrollArea>
#include <QVector>
#include "batterycellwidget.h"

//有个低压阈值，有个最低最高值
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

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    class ContentWidget;             // 前向声明
    ContentWidget* m_contentWidget;  // 承载所有电芯的容器

    QVector<BatteryCellWidget*> m_cells;
    void updateLayout();             // 布局电芯
};

#endif // BATTERYMODULEWIDGET_H
