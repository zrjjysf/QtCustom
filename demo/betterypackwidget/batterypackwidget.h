#ifndef BATTERYPACKWIDGET_H
#define BATTERYPACKWIDGET_H

#include <QWidget>
#include <QLabel>
class TemplateSvgWidget;
class BatteryPackWidget : public QWidget
{
    Q_OBJECT
public:
    enum OpenMode
    {
        OPEN,
        CLOSE
    };
    Q_ENUM(OpenMode)
    enum Status
    {
        NORMAL,
        ALARM
    };
    Q_ENUM(Status)
    explicit BatteryPackWidget(QWidget *parent = nullptr);
    int packID(){return m_packID;}
    OpenMode openMode(){return m_openMode;}
    Status status(){return m_status;}

    void setPackID(int packID);
    void setOpenMode(OpenMode mode);
    void setStatus(Status sta);
    static QSize staticSize;
public slots:
    void slotStatusChanged(int status);
signals:
    void opened(int packID);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    OpenMode m_openMode = OpenMode::CLOSE;
    Status m_status = Status::NORMAL;
    int m_packID = -1;
    TemplateSvgWidget *m_bg;
};

#endif // BATTERYPACKWIDGET_H