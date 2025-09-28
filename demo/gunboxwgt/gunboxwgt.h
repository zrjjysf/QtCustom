#ifndef GUNBOXWGT_H
#define GUNBOXWGT_H

#include <QFrame>

class QLabel;
class QVBoxLayout;
class QGridLayout;
class IconLabel;
class VerticalInfoWidget;

class GunBoxWgt : public QFrame
{
    Q_OBJECT
public:
    enum Status {
        Fault,
        WaitInsertGun,
        WaitScanQR,
        ChargeStarting,
        Charging
    };
    Q_ENUM(Status)

    explicit GunBoxWgt(QWidget *parent = nullptr);

    void setTitle(const QString &title);
    void setIconText(const QString &text);
    void setErrorText(const QString &text);

    void setStatus(Status st);
    Status status() const { return m_status; }

private:
    // 各个 UI 状态初始化
    void initFaultWidgets();
    void initWaitInsertGunWidgets();
    void initWaitScanQRWidgets();
    void initChargeStartingWidgets();
    void initChargingWidgets();

    void updateUI();

private:
    QLabel *m_titleLabel;
    IconLabel *m_statusIcon;
    QWidget *m_centerArea;
    QVBoxLayout *m_centerLayout;
    QLabel *m_errorLabel;

    // 不同状态的控件（预创建，切换时 show/hide）
    QWidget *m_faultPage;
    QWidget *m_waitInsertGunPage;
    QWidget *m_waitScanQRPage;
    QWidget *m_chargeStartingPage;
    QWidget *m_chargingPage;

    Status m_status;
};

#endif // GUNBOXWGT_H
