#include "gunboxwgt.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include "iconlabel.h"
#include "verticalinfowidget.h"

GunBoxWgt::GunBoxWgt(QWidget *parent)
    : QFrame(parent),
      m_titleLabel(new QLabel("枪 A")),
      m_statusIcon(new IconLabel),
      m_centerArea(new QWidget),
      m_centerLayout(new QVBoxLayout),
      m_errorLabel(new QLabel),
      m_faultPage(nullptr),
      m_waitInsertGunPage(nullptr),
      m_waitScanQRPage(nullptr),
      m_chargeStartingPage(nullptr),
      m_chargingPage(nullptr),
      m_status(Fault)
{
    // 总体布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(6);

    // 顶部（标题 + 状态图标）
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_statusIcon);
    mainLayout->addLayout(topLayout);

    // 中间
    m_centerArea->setLayout(m_centerLayout);
    m_centerLayout->setContentsMargins(0, 0, 0, 0);
    m_centerLayout->setSpacing(4);
    mainLayout->addWidget(m_centerArea, 1);

    // 底部
    m_errorLabel->setStyleSheet("color: red;");
    mainLayout->addWidget(m_errorLabel);

    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);

    // 预初始化所有页面
    initFaultWidgets();
    initWaitInsertGunWidgets();
    initWaitScanQRWidgets();
    initChargeStartingWidgets();
    initChargingWidgets();

    setStatus(Fault);
}

void GunBoxWgt::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void GunBoxWgt::setIconText(const QString &text)
{
    m_statusIcon->setText(text);
}

void GunBoxWgt::setErrorText(const QString &text)
{
    m_errorLabel->setText(text);
}

void GunBoxWgt::setStatus(Status st)
{
    if (m_status == st) return;
    m_status = st;
    updateUI();
}

void GunBoxWgt::updateUI()
{
    // 隐藏所有
    if (m_faultPage) m_faultPage->hide();
    if (m_waitInsertGunPage) m_waitInsertGunPage->hide();
    if (m_waitScanQRPage) m_waitScanQRPage->hide();
    if (m_chargeStartingPage) m_chargeStartingPage->hide();
    if (m_chargingPage) m_chargingPage->hide();

    // 清理布局
    while (m_centerLayout->count() > 0) {
        m_centerLayout->takeAt(0);
    }

    // 根据状态添加对应的页面
    switch (m_status) {
    case Fault:
        m_centerLayout->addWidget(m_faultPage);
        m_faultPage->show();
        break;
    case WaitInsertGun:
        m_centerLayout->addWidget(m_waitInsertGunPage);
        m_waitInsertGunPage->show();
        break;
    case WaitScanQR:
        m_centerLayout->addWidget(m_waitScanQRPage);
        m_waitScanQRPage->show();
        break;
    case ChargeStarting:
        m_centerLayout->addWidget(m_chargeStartingPage);
        m_chargeStartingPage->show();
        break;
    case Charging:
        m_centerLayout->addWidget(m_chargingPage);
        m_chargingPage->show();
        break;
    }
}

void GunBoxWgt::initFaultWidgets()
{
    m_faultPage = new QWidget(this);
    QGridLayout *grid = new QGridLayout(m_faultPage);
    grid->setContentsMargins(0,0,0,0);
    grid->setSpacing(6);

    auto v1 = new VerticalInfoWidget; v1->setTopText("电压"); v1->setBottomText("0 V");
    auto v2 = new VerticalInfoWidget; v2->setTopText("电流"); v2->setBottomText("0 A");
    auto v3 = new VerticalInfoWidget; v3->setTopText("功率"); v3->setBottomText("0 kW");
    auto v4 = new VerticalInfoWidget; v4->setTopText("SOC");   v4->setBottomText("--");

    grid->addWidget(v1, 0, 0);
    grid->addWidget(v2, 0, 1);
    grid->addWidget(v3, 1, 0);
    grid->addWidget(v4, 1, 1);
}

void GunBoxWgt::initWaitInsertGunWidgets()
{
    m_waitInsertGunPage = new QLabel("请插入充电枪", this);
    static_cast<QLabel*>(m_waitInsertGunPage)->setAlignment(Qt::AlignCenter);
}

void GunBoxWgt::initWaitScanQRWidgets()
{
    QLabel *qr = new QLabel(this);
    qr->setPixmap(QPixmap(":/images/qrcode.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    qr->setAlignment(Qt::AlignCenter);
    m_waitScanQRPage = qr;
}

void GunBoxWgt::initChargeStartingWidgets()
{
    QLabel *info = new QLabel("正在启动充电…", this);
    info->setAlignment(Qt::AlignCenter);
    m_chargeStartingPage = info;
}

void GunBoxWgt::initChargingWidgets()
{
    QLabel *info = new QLabel("充电中…", this);
    info->setAlignment(Qt::AlignCenter);
    m_chargingPage = info;
}
