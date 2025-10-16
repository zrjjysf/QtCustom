#include "bmscellvoltagewgt.h"
// #include "core/coremanager.h"
// #include "gui/component/batterymodulewidget.h"
// #include "gui/component/batterypackwidget.h"
#include "batterypackwidget.h"
#include "batterymodulewidget.h"
#include "absbmsstruct.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QTimer>
#include <QDebug>

const int margin = 10;

BMSCellVoltageWgt::BMSCellVoltageWgt(QWidget *parent)
    : QWidget(parent),
      m_refreshTimer(new QTimer(this)),
      m_mainLayout(new QHBoxLayout(this)),
      m_scrollArea(new QScrollArea(this)),
      m_scrollContent(new QWidget),
      m_leftLayout(new QVBoxLayout(m_scrollContent)),
      m_stackLayout(new QStackedLayout)
{
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFixedWidth(BatteryPackWidget::staticSize.width()+margin*2);
    m_scrollArea->setWidget(m_scrollContent);
    QVBoxLayout* scrollContentLayout = new QVBoxLayout(m_scrollContent);
    scrollContentLayout->addLayout(m_leftLayout);
    scrollContentLayout->addStretch(1);
    m_mainLayout->addWidget(m_scrollArea);
    QWidget *stackContainer = new QWidget;
    stackContainer->setLayout(m_stackLayout);
    m_mainLayout->addWidget(stackContainer, 1);
    initMyConnect();

    m_refreshTimer->setInterval(1000);
    m_refreshTimer->start();
}


void BMSCellVoltageWgt::initMyConnect()
{
    connect(m_refreshTimer, &QTimer::timeout, this, &BMSCellVoltageWgt::updateData);
}

void BMSCellVoltageWgt::updateData()
{
    ABSBatteryClusterInfo data;
    for(float vol=1.0;vol<60.0;++vol)
        data.addCellVoltage(1,4,int(vol),vol);
    for(float vol=1.0;vol<100.0;++vol)
        data.addCellVoltage(1,2,int(vol),vol);
    for(float vol=1.0;vol<100.0;++vol)
        data.addCellVoltage(1,3,int(vol),vol);

    if (data.racks.empty())
        return;

    const ABSBatteryRack &rack = data.racks.constBegin().value();
    auto modules_it = rack.modules.constBegin();

    while (modules_it != rack.modules.constEnd()) {
        int moduleId = modules_it.key();
        QVector<float> voltages(modules_it.value().cells.begin(), modules_it.value().cells.end());

        // 创建或更新 BatteryModuleWidget
        BatteryModuleWidget *moduleWidget = nullptr;
        if (m_moduleWidgets.contains(moduleId)) {
            moduleWidget = m_moduleWidgets[moduleId];
            moduleWidget->updateCellData(voltages);
        } else {
            moduleWidget = new BatteryModuleWidget;
            moduleWidget->setNumber(moduleId);
            moduleWidget->updateCellData(voltages);
            m_moduleWidgets[moduleId] = moduleWidget;
            m_stackLayout->addWidget(moduleWidget);
        }

        // 创建或更新 BatteryPackWidget
        if (!m_packWidgets.contains(moduleId)) {
            BatteryPackWidget *packWidget = new BatteryPackWidget;
            packWidget->setPackID(moduleId);
            m_packWidgets[moduleId] = packWidget;
            m_leftLayout->addWidget(packWidget);

            // 点击左侧 packWidget → 切换右侧页面
            connect(packWidget, &BatteryPackWidget::opened,this,&BMSCellVoltageWgt::handleSwitch);
        }

        ++modules_it;
    }
}

void BMSCellVoltageWgt::handleSwitch(int packID)
{
    if (!m_moduleWidgets.contains(packID))
        return;
    m_stackLayout->setCurrentWidget(m_moduleWidgets[packID]);
    auto packs_it = m_packWidgets.constBegin();
    while(packs_it!=m_packWidgets.constEnd())
    {
        if(packs_it.key()!=packID)
            packs_it.value()->setOpenMode(BatteryPackWidget::CLOSE);
        packs_it++;
    }
}
