#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QStyleFactory>
#include <QFile>
#include <QDebug>
#include "batterypackwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序样式（可选）
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // 创建主窗口
    QMainWindow window;
    window.setWindowTitle("电池包控件演示 - BatteryPackWidget Demo");
    window.resize(400, 400);
    
    // 创建中央部件
    QWidget *centralWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // 左侧：电池包显示区域
    QVBoxLayout *displayLayout = new QVBoxLayout();
    QGroupBox *displayGroup = new QGroupBox("电池包显示");
    displayGroup->setLayout(displayLayout);
    
    BatteryPackWidget *battery = new BatteryPackWidget();
    displayLayout->addWidget(battery, 0, Qt::AlignCenter);
    
    // 右侧：控制面板
    QVBoxLayout *controlLayout = new QVBoxLayout();
    QGroupBox *controlGroup = new QGroupBox("控制面板");
    controlGroup->setLayout(controlLayout);
    
    // 开关状态控制
    QHBoxLayout *switchLayout = new QHBoxLayout();
    QLabel *switchLabel = new QLabel("开关状态:");
    QPushButton *toggleSwitchBtn = new QPushButton("切换开关");
    QCheckBox *switchCheckbox = new QCheckBox("打开状态");
    switchCheckbox->setChecked(true);
    
    switchLayout->addWidget(switchLabel);
    switchLayout->addWidget(switchCheckbox);
    switchLayout->addWidget(toggleSwitchBtn);
    switchLayout->addStretch();
    
    // 告警状态控制
    QHBoxLayout *alarmLayout = new QHBoxLayout();
    QLabel *alarmLabel = new QLabel("告警状态:");
    QPushButton *toggleAlarmBtn = new QPushButton("切换告警");
    QCheckBox *alarmCheckbox = new QCheckBox("告警状态");
    alarmCheckbox->setChecked(false);
    
    alarmLayout->addWidget(alarmLabel);
    alarmLayout->addWidget(alarmCheckbox);
    alarmLayout->addWidget(toggleAlarmBtn);
    alarmLayout->addStretch();
    
    // ID设置控制
    QHBoxLayout *idLayout = new QHBoxLayout();
    QLabel *idLabel = new QLabel("电池包ID:");
    QSpinBox *idSpinBox = new QSpinBox();
    idSpinBox->setRange(0, 999);
    idSpinBox->setValue(123);
    QPushButton *setIdBtn = new QPushButton("设置ID");
    
    idLayout->addWidget(idLabel);
    idLayout->addWidget(idSpinBox);
    idLayout->addWidget(setIdBtn);
    idLayout->addStretch();
    
    // 状态信息显示
    QGroupBox *statusGroup = new QGroupBox("当前状态");
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    QLabel *currentStatus = new QLabel();
    currentStatus->setText("开关: 打开 | 告警: 正常 | ID: 123");
    currentStatus->setStyleSheet("QLabel { padding: 10px; background: #f0f0f0; border-radius: 5px; }");
    statusLayout->addWidget(currentStatus);
    
    // 将所有控件添加到控制面板
    controlLayout->addLayout(switchLayout);
    controlLayout->addLayout(alarmLayout);
    controlLayout->addLayout(idLayout);
    controlLayout->addWidget(statusGroup);
    controlLayout->addStretch();
    
    // 预设样式按钮
    QGroupBox *styleGroup = new QGroupBox("预设样式");
    QVBoxLayout *styleLayout = new QVBoxLayout(styleGroup);
    
    QPushButton *normalStyleBtn = new QPushButton("正常样式");
    QPushButton *alarmStyleBtn = new QPushButton("告警样式");
    QPushButton *darkStyleBtn = new QPushButton("暗色主题");
    
    styleLayout->addWidget(normalStyleBtn);
    styleLayout->addWidget(alarmStyleBtn);
    styleLayout->addWidget(darkStyleBtn);
    
    controlLayout->addWidget(styleGroup);
    
    // 将显示区域和控制面板添加到主布局
    mainLayout->addWidget(displayGroup, 2);
    mainLayout->addWidget(controlGroup, 1);
    
    window.setCentralWidget(centralWidget);
    
    // 连接信号槽 - 开关状态
    QObject::connect(toggleSwitchBtn, &QPushButton::clicked, [battery, switchCheckbox]() {
        bool newState = !battery->isOn();
        battery->setSwitchState(newState);
        switchCheckbox->setChecked(newState);
    });
    
    QObject::connect(switchCheckbox, &QCheckBox::toggled, [battery](bool checked) {
        battery->setSwitchState(checked);
    });
    
    // 连接信号槽 - 告警状态
    QObject::connect(toggleAlarmBtn, &QPushButton::clicked, [battery, alarmCheckbox]() {
        bool newState = !battery->isAlarm();
        battery->setStatus(newState);
        alarmCheckbox->setChecked(newState);
    });
    
    QObject::connect(alarmCheckbox, &QCheckBox::toggled, [battery](bool checked) {
        battery->setStatus(checked);
    });
    
    // 连接信号槽 - ID设置
    QObject::connect(setIdBtn, &QPushButton::clicked, [battery, idSpinBox]() {
        battery->setId(idSpinBox->value());
    });
    
    QObject::connect(idSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [battery](int value) {
        battery->setId(value);
    });
    
    // 连接信号槽 - 状态更新显示
    QObject::connect(battery, &BatteryPackWidget::switchStateChanged, [currentStatus, battery](bool isOn) {
        QString status = QString("开关: %1 | 告警: %2 | ID: %3")
                            .arg(isOn ? "打开" : "关闭")
                            .arg(battery->isAlarm() ? "告警" : "正常")
                            .arg(battery->packId());
        currentStatus->setText(status);
    });
    
    QObject::connect(battery, &BatteryPackWidget::statusChanged, [currentStatus, battery](bool isAlarm) {
        QString status = QString("开关: %1 | 告警: %2 | ID: %3")
                            .arg(battery->isOn() ? "打开" : "关闭")
                            .arg(isAlarm ? "告警" : "正常")
                            .arg(battery->packId());
        currentStatus->setText(status);
    });
    
    QObject::connect(battery, &BatteryPackWidget::idChanged, [currentStatus, battery](int id) {
        QString status = QString("开关: %1 | 告警: %2 | ID: %3")
                            .arg(battery->isOn() ? "打开" : "关闭")
                            .arg(battery->isAlarm() ? "告警" : "正常")
                            .arg(id);
        currentStatus->setText(status);
    });
    
    // 连接信号槽 - 预设样式
    QObject::connect(normalStyleBtn, &QPushButton::clicked, [battery]() {
        battery->setStatus(false);
        battery->setSwitchState(true);
        battery->setId(100);
    });
    
    QObject::connect(alarmStyleBtn, &QPushButton::clicked, [battery]() {
        battery->setStatus(true);
        battery->setSwitchState(false);
        battery->setId(999);
    });
    
    QObject::connect(darkStyleBtn, &QPushButton::clicked, [centralWidget]() {
        // 简单的暗色主题示例
        centralWidget->setStyleSheet(
            "QMainWindow, QWidget { background: #2b2b2b; color: #ffffff; }"
            "QGroupBox { border: 1px solid #555; border-radius: 5px; margin-top: 1ex; padding-top: 10px; }"
            "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; color: #88c070; }"
            "QPushButton { background: #505050; color: white; border: 1px solid #666; padding: 5px 10px; border-radius: 3px; }"
            "QPushButton:hover { background: #606060; }"
            "QPushButton:pressed { background: #404040; }"
            "QSpinBox { background: #353535; color: white; border: 1px solid #555; padding: 2px; }"
            "QCheckBox { color: #cccccc; }"
            "QCheckBox::indicator { width: 13px; height: 13px; }"
            "QCheckBox::indicator:unchecked { background: #353535; border: 1px solid #555; }"
            "QCheckBox::indicator:checked { background: #5070a0; border: 1px solid #6080b0; }"
        );
    });
    
    // 初始设置
    battery->setId(123);
    battery->setSwitchState(true);
    battery->setStatus(false);
    
    // 应用基础样式
    centralWidget->setStyleSheet(
        "QGroupBox { border: 1px solid #ccc; border-radius: 5px; margin-top: 1ex; padding-top: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }"
        "QPushButton { background: #f0f0f0; border: 1px solid #aaa; padding: 5px 10px; border-radius: 3px; }"
        "QPushButton:hover { background: #e8e8e8; }"
        "QPushButton:pressed { background: #d8d8d8; }"
    );
    
    window.show();
    
    return app.exec();
}