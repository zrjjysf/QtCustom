#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <QScrollArea>
#include <QRandomGenerator>
#include <QLabel>
#include "batterycellwidget.h"
#include "batterymodulewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 创建主窗口
    QWidget* mainWindow = new QWidget;
    mainWindow->resize(1024, 740);
    mainWindow->setWindowTitle("电池模块模拟器");
    
    // 创建主布局 - 垂直布局
    QHBoxLayout* windowLayout = new QHBoxLayout(mainWindow);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    QScrollArea* scrollArea = new QScrollArea;
    // scrollArea->setWidgetResizable(true); // 内容随区域大小变化
    scrollArea->setFixedWidth(160);
    QWidget* content = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(content);

    // 添加内容控件
    for (int i = 0; i < 50; ++i) {
        layout->addWidget(new QPushButton(QString("Button %1").arg(i)));
    }
    scrollArea->setWidget(content); // 设置滚动区域的内容


    windowLayout->addWidget(scrollArea);


    windowLayout->addLayout(mainLayout);
    // 创建按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    
    // 创建更新按钮
    QPushButton* updateButton = new QPushButton("更新电芯数据");
    QPushButton* autoUpdateButton = new QPushButton("自动更新");
    QPushButton* stopAutoUpdateButton = new QPushButton("停止自动更新");
    QPushButton* addCellsButton = new QPushButton("增加电芯");
    QPushButton* removeCellsButton = new QPushButton("减少电芯");
    
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(autoUpdateButton);
    buttonLayout->addWidget(stopAutoUpdateButton);
    buttonLayout->addWidget(addCellsButton);
    buttonLayout->addWidget(removeCellsButton);
    buttonLayout->addStretch(); // 添加弹性空间
    
    // 创建状态标签
    QLabel* statusLabel = new QLabel("状态: 就绪");
    buttonLayout->addWidget(statusLabel);
    
    // 创建模块容器
    QHBoxLayout* modulesLayout = new QHBoxLayout;
    
    // 存储模块指针，以便后续更新
    QVector<BatteryModuleWidget*> modules;
    
    // 创建两个电池模块
    for(int i = 1; i < 3; ++i)
    {
        BatteryModuleWidget* module = new BatteryModuleWidget;
        modulesLayout->addWidget(module);
        module->setNumber(i);
        
        // 设置电压阈值
        module->setVoltageThresholds(2.5f, 4.2f);
        
        // 初始电芯数据
        QVector<float> initialVoltages;
        for (int j = 0; j < 96; ++j) {
            // 初始电压在3.6-3.8V之间
            float voltage = -1200.0f + (QRandomGenerator::global()->generateDouble() * 2400.0f);
            initialVoltages.append(voltage);
        }
        
        // 使用updateCellData初始化电芯
        module->updateCellData(initialVoltages);
        
        modules.append(module);
    }
    
    // 将按钮布局和模块布局添加到主布局
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(modulesLayout);
    
    // 创建定时器用于自动更新
    QTimer* autoUpdateTimer = new QTimer(mainWindow);
    
    // 存储每个模块的电芯数量
    QVector<int> moduleCellCounts;
    moduleCellCounts.append(96);
    moduleCellCounts.append(96);
    
    // 更新电芯数据的函数
    auto updateCellData = [&modules, &moduleCellCounts, statusLabel]() {
        for (int moduleIndex = 0; moduleIndex < modules.size(); ++moduleIndex) {
            BatteryModuleWidget* module = modules[moduleIndex];
            
            // 生成新的随机电压数据
            QVector<float> newVoltages;
            int cellCount = moduleCellCounts[moduleIndex];
            
            for (int i = 0; i < cellCount; ++i) {
                // 生成3.5-3.9V之间的随机电压
                float voltage = 3.5f + (QRandomGenerator::global()->generateDouble() * 0.4f);
                newVoltages.append(voltage);
            }
            
            // 随机改变电芯数量（模拟动态变化）
            if (QRandomGenerator::global()->bounded(5) == 0) { // 20%的概率
                int change = QRandomGenerator::global()->bounded(11) - 5; // -5 到 5 的变化
                int newCount = qMax(0, cellCount + change); // 最少10个电芯
                
                // 调整电压数据数量
                if (newCount > cellCount) {
                    for (int i = cellCount; i < newCount; ++i) {
                        float voltage = 3.5f + (QRandomGenerator::global()->generateDouble() * 0.4f);
                        newVoltages.append(voltage);
                    }
                    statusLabel->setText(QString("模块 %1 增加了 %2 个电芯").arg(moduleIndex+1).arg(newCount - cellCount));
                } else if (newCount < cellCount) {
                    newVoltages.resize(newCount);
                    statusLabel->setText(QString("模块 %1 减少了 %2 个电芯").arg(moduleIndex+1).arg(cellCount - newCount));
                }
                
                moduleCellCounts[moduleIndex] = newCount;
            }
            
            // 更新模块数据
            module->updateCellData(newVoltages);
        }
    };
    
    // 手动增加电芯
    QObject::connect(addCellsButton, &QPushButton::clicked, [&modules, &moduleCellCounts, statusLabel]() {
        for (int moduleIndex = 0; moduleIndex < modules.size(); ++moduleIndex) {
            int addCount = QRandomGenerator::global()->bounded(5) + 5; // 5-10个
            int newCount = moduleCellCounts[moduleIndex] + addCount;
            
            // 生成新的电压数据
            QVector<float> newVoltages;
            for (int i = 0; i < newCount; ++i) {
                float voltage = 3.5f + (QRandomGenerator::global()->generateDouble() * 0.4f);
                newVoltages.append(voltage);
            }
            
            modules[moduleIndex]->updateCellData(newVoltages);
            moduleCellCounts[moduleIndex] = newCount;
        }
        statusLabel->setText("手动增加了电芯");
    });
    
    // 手动减少电芯
    QObject::connect(removeCellsButton, &QPushButton::clicked, [&modules, &moduleCellCounts, statusLabel]() {
        for (int moduleIndex = 0; moduleIndex < modules.size(); ++moduleIndex) {
            int removeCount = QRandomGenerator::global()->bounded(5) + 5; // 5-10个
            int newCount = qMax(0, moduleCellCounts[moduleIndex] - removeCount); // 最少10个
            
            // 生成新的电压数据
            QVector<float> newVoltages;
            for (int i = 0; i < newCount; ++i) {
                float voltage = 3.5f + (QRandomGenerator::global()->generateDouble() * 0.4f);
                newVoltages.append(voltage);
            }
            
            modules[moduleIndex]->updateCellData(newVoltages);
            moduleCellCounts[moduleIndex] = newCount;
        }
        statusLabel->setText("手动减少了电芯");
    });
    
    // 连接按钮信号
    QObject::connect(updateButton, &QPushButton::clicked, [updateCellData, statusLabel]() {
        updateCellData();
        statusLabel->setText("手动更新了电芯数据");
    });
    
    QObject::connect(autoUpdateButton, &QPushButton::clicked, [autoUpdateTimer, statusLabel]() {
        autoUpdateTimer->start(1000); // 每1秒更新一次
        statusLabel->setText("开始自动更新");
    });
    
    QObject::connect(stopAutoUpdateButton, &QPushButton::clicked, [autoUpdateTimer, statusLabel]() {
        autoUpdateTimer->stop();
        statusLabel->setText("停止自动更新");
    });
    
    // 连接定时器信号
    QObject::connect(autoUpdateTimer, &QTimer::timeout, updateCellData);
    
    mainWindow->show();
    return a.exec();
}