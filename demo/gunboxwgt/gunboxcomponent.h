#ifndef GUNBOX_H
#define GUNBOX_H

#include <QFrame>
#include <QLabel>

class IconLabel;
class GunBox : public QFrame
{
    Q_OBJECT
public:

    enum Status
    {
        Fault,
        WaitInsertGun,
        WaitScanQR,
        ChargeStarting,
        Charging
    };
    Q_ENUM(Status)

    explicit GunBox(QWidget *parent = nullptr);
private:
    QLabel* m_title;
    IconLabel* m_statusLabel;
    QLabel* m_gunTip;
    Status m_status=Status::Fault;
};

#endif // GUNBOX_H
