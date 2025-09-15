#ifndef BATTERYWGT_H
#define BATTERYWGT_H

#include <QWidget>
#include <QColor>
#include <QMap>

class BatteryWgt : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString batteryColorConfig READ batteryColorConfig WRITE setBatteryColorConfig)

public:
    explicit BatteryWgt(bool showText = true, QWidget *parent = nullptr);

    void addConfig(int threshold, QColor col);
    void setBatteryPercent(int percent);
    void setTextVisible(bool show = true);

    // QSS支持的属性
    void setBatteryColorConfig(const QString &configStr);
    QString batteryColorConfig() const;
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_iBatteryPercent = -1;
    bool m_needShowText = true;
    QMap<int, QColor> m_configMap;
};

#endif // BATTERYWGT_H
