#ifndef BATTERYMODULEWIDGET_H
#define BATTERYMODULEWIDGET_H

#include <QAbstractScrollArea>
#include <QVector>
#include "batterycellwidget.h"

class BatteryModuleWidget : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit BatteryModuleWidget(QWidget *parent = nullptr);

    // 添加一个电芯
    void addBatteryCell(BatteryCellWidget* cell);

    // 清空所有电芯
    void clearCells();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QVector<BatteryCellWidget*> m_cells;
    QWidget* m_contentWidget;    // 承载所有电芯的容器
    void updateLayout();          // 布局电芯
};

#endif // BATTERYMODULEWIDGET_H
