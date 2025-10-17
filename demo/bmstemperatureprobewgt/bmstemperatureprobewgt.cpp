#include "bmstemperatureprobewgt.h"
// #include "core/coremanager.h"
// #include "gui/component/batterymodulewidget.h"
// #include "gui/component/batterypackwidget.h"
#include "batterypackwidget.h"
#include "temperaturemodulewidget.h"
#include "customscrollbar.h"
#include "flowlayout.h"
#include "absbmsstruct.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QTimer>
#include <QDebug>

const int margin = 18;
const int lowTemperatureThreshold = -1000;
const int highTemperatureThreshold = 53.0;

BMSTemperatureProbeWgt::BMSTemperatureProbeWgt(QWidget *parent)
    : QWidget(parent),
      m_refreshTimer(new QTimer(this)),
      m_mainLayout(new QHBoxLayout(this)),
      m_leftLayout(new FlowLayout),
      m_scrollArea(new QScrollArea(this)),
      m_scrollContent(new QWidget),
      m_stackLayout(new QStackedLayout),
      m_stackContainer(new QWidget)
{
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    
    m_scrollArea->setWidgetResizable(true);
    CustomScrollBar *vScroll = new CustomScrollBar(Qt::Vertical, m_scrollArea);
    vScroll->setStyleSheet(R"(CustomScrollBar
        {
            min-width:25px; 
            min-height:25px; 
        })");
    m_scrollArea->setVerticalScrollBar(vScroll);
    m_scrollArea->setWidget(m_scrollContent);
    m_scrollContent->setLayout(m_leftLayout);
    m_mainLayout->addWidget(m_scrollArea);
    m_stackContainer->setLayout(m_stackLayout);
    m_mainLayout->addWidget(m_stackContainer, 1);
    m_stackContainer->setVisible(false);
    initMyConnect();

    m_refreshTimer->setInterval(1000);
    m_refreshTimer->start();
}


void BMSTemperatureProbeWgt::initMyConnect()
{
    connect(m_refreshTimer, &QTimer::timeout, this, &BMSTemperatureProbeWgt::updateData);
}

void BMSTemperatureProbeWgt::updateData()
{
    ABSBatteryClusterInfo data;
    for(int i=0;i<12;++i) // 包号
    {
        for(int cell_id = 0;cell_id<i*5;++cell_id)//电芯编号
        {
            data.addTempSensor(1,i,cell_id,cell_id+i);
        }
    }

    if (data.racks.empty())
        return;

    const ABSBatteryRack &rack = data.racks.constBegin().value();
    auto modules_it = rack.modules.constBegin();

    while (modules_it != rack.modules.constEnd()) {
        int moduleId = modules_it.key();
        QVector<int> cell_values(modules_it.value().temps.begin(), modules_it.value().temps.end());

        TemperatureModuleWidget *moduleWidget = nullptr;
        if (m_moduleWidgets.contains(moduleId)) {
            moduleWidget = m_moduleWidgets[moduleId];
        } else {
            moduleWidget = new TemperatureModuleWidget;
            moduleWidget->setNumber(moduleId);
            moduleWidget->setTemperatureThresholds(lowTemperatureThreshold, highTemperatureThreshold);
            m_moduleWidgets[moduleId] = moduleWidget;
            m_stackLayout->addWidget(moduleWidget);
        }

        // 创建或更新 BatteryPackWidget
        if (!m_packWidgets.contains(moduleId)) {
            BatteryPackWidget *packWidget = new BatteryPackWidget;
            packWidget->setPackID(moduleId);
            m_packWidgets[moduleId] = packWidget;
            // 计算插入位置
            auto it = std::lower_bound(m_index.begin(), m_index.end(), moduleId);
            int insertPos = std::distance(m_index.begin(), it);
            qDebug()<<moduleId;
            qDebug()<<insertPos;
            m_index.insert(insertPos, moduleId);
            m_leftLayout->insertWidget(insertPos, packWidget);

            // 点击左侧 packWidget → 切换右侧页面
            connect(packWidget, &BatteryPackWidget::opened,this,&BMSTemperatureProbeWgt::handleSwitch);
            connect(moduleWidget,&TemperatureModuleWidget::statusChanged,packWidget,&BatteryPackWidget::slotStatusChanged);
        }

        // 更新数据，这时会触发信号
        moduleWidget->updateProbeData(cell_values);
        ++modules_it;
    }
}

void BMSTemperatureProbeWgt::handleSwitch(int packID)
{
    if (!m_moduleWidgets.contains(packID))
        return;
    if(m_currentID==-1)
    {
        m_scrollArea->setFixedWidth(BatteryPackWidget::staticSize.width()+margin*3);
        m_stackContainer->setVisible(true);
    }
    m_currentID = packID;
    m_stackLayout->setCurrentWidget(m_moduleWidgets[packID]);
    auto packs_it = m_packWidgets.constBegin();
    while(packs_it!=m_packWidgets.constEnd())
    {
        if(packs_it.key()!=packID)
            packs_it.value()->setOpenMode(BatteryPackWidget::CLOSE);
        packs_it++;
    }
}
