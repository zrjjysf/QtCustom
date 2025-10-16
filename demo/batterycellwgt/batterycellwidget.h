#ifndef BATTERYCELLWIDGET_H
#define BATTERYCELLWIDGET_H

#include <QWidget>

class BatteryCellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BatteryCellWidget(QWidget *parent = nullptr);

    // 设置编号
    void setNumber(int number);

    // 设置电压值（中间显示）
    void setVoltage(double voltage);

    // 设置电压阈值
    void setVoltageThresholds(double low, double high);

    // 设置是否高亮显示
    void setHighlighted(bool highlight);

    static int margin;
    static int leftRectWidth;
    static int rightRectWidth;
    static int rectHeight;
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_number;
    double m_voltage = 0.0;      // 当前电压

    double m_lowVoltage = 0.0;   // 低压阈值
    double m_highVoltage = 0.0;  // 高压阈值

    bool m_highlighted = false;  // 是否高亮
};

#endif // BATTERYCELLWIDGET_H
