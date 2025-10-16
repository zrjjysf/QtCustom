#ifndef BATTERYPACKWIDGET_H
#define BATTERYPACKWIDGET_H

#include <QWidget>
#include <QLabel>
#include "themesvgwgt.h"

class BatteryPackWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isOn READ isOn WRITE setSwitchState NOTIFY switchStateChanged)
    Q_PROPERTY(bool isAlarm READ isAlarm WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(int packId READ packId WRITE setId NOTIFY idChanged)

public:
    explicit BatteryPackWidget(QWidget *parent = nullptr);

    // 属性读取
    bool isOn() const { return m_isOn; }
    bool isAlarm() const { return m_isAlarm; }
    int packId() const { return m_packId; }

    QSize sizeHint() const override;

    // bool hasHeightForWidth() const override {return true;}
    // int heightForWidth(int) const override;
public slots:
    // 设置状态：正常状态或告警状态
    void setStatus(bool isAlarm);
    
    // 设置开关状态
    void setSwitchState(bool isOn);
    
    // 设置电池包ID
    void setId(int packId);

signals:
    void switchStateChanged(bool isOn);
    void statusChanged(bool isAlarm);
    void idChanged(int packId);

protected:
    // void resizeEvent(QResizeEvent *event) override;
    void updateAppearance();

private:
    bool m_isOn;
    bool m_isAlarm;
    int m_packId;
    
    ThemeSvgWgt *m_closedSvg;
    ThemeSvgWgt *m_openSvg;
    QLabel *m_idLabel;
};

#endif // BATTERYPACKWIDGET_H