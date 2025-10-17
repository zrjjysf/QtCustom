#ifndef TEMPERATUREPROBEWIDGET_H
#define TEMPERATUREPROBEWIDGET_H

#include <QWidget>

class TemperatureProbeWidget : public QWidget
{
    Q_OBJECT
public:
    enum Status { NORMAL, TOO_LOW, TOO_HIGH };
    explicit TemperatureProbeWidget(QWidget *parent = nullptr);

    // 设置编号
    void setNumber(int number);

    // 设置温度值
    void setTemperature(int temperature);

    // 设置温度阈值
    void setTemperatureThresholds(int low, int high);

    // 设置是否高亮显示
    void setHighlighted(bool highlight);
    Status currentStatus() { return m_status; }

    // 静态尺寸定义
    static int margin;
    static int padding;
    static int probeWidth;
    static int probeHeight;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Status m_status = Status::NORMAL;
    int m_number;
    int m_temperature = 0;      // 当前温度
    int m_lowTemperature = 0;   // 低温阈值
    int m_highTemperature = 0;  // 高温阈值
    bool m_highlighted = false;      // 是否高亮
};

#endif // TEMPERATUREPROBEWIDGET_H