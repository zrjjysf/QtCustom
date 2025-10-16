#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QGroupBox>
#include <QTimer>
#include <QRandomGenerator>
#include <QFormLayout>

#include "temperatureprobewidget.h"

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    TestWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setupUI();
        setupConnections();
        
        // 启动定时器模拟实时温度变化
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &TestWindow::updateRandomTemperature);
        timer->start(1500);
    }

private slots:
    void updateTemperature()
    {
        int temp = m_temperatureSpinBox->value();
        m_probeWidget->setTemperature(temp);
        updateStatus();
    }

    void updateNumber()
    {
        int number = m_numberSpinBox->value();
        m_probeWidget->setNumber(number);
        updateStatus();
    }

    void updateThresholds()
    {
        int low = m_lowThresholdSpinBox->value();
        int high = m_highThresholdSpinBox->value();
        m_probeWidget->setTemperatureThresholds(low, high);
        updateStatus();
    }

    void toggleHighlight()
    {
        bool highlighted = m_highlightCheckBox->isChecked();
        m_probeWidget->setHighlighted(highlighted);
        updateStatus();
    }

    void updateRandomTemperature()
    {
        if (m_autoUpdateCheckBox->isChecked()) {
            int temp = QRandomGenerator::global()->bounded(-10, 50);
            m_temperatureSpinBox->setValue(temp);
            updateTemperature();
        }
    }

    void resetProbe()
    {
        m_numberSpinBox->setValue(1);
        m_temperatureSpinBox->setValue(20);
        m_lowThresholdSpinBox->setValue(15);
        m_highThresholdSpinBox->setValue(30);
        m_highlightCheckBox->setChecked(false);
        
        updateNumber();
        updateTemperature();
        updateThresholds();
        toggleHighlight();
    }

private:
    void setupUI()
    {
        QWidget *centralWidget = new QWidget(this);
        QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

        // 左侧：探头显示
        QWidget *displayWidget = new QWidget(this);
        QVBoxLayout *displayLayout = new QVBoxLayout(displayWidget);
        
        m_probeWidget = new TemperatureProbeWidget(this);
        m_probeWidget->setFixedSize(200, 150); // 放大显示以便观察细节
        
        displayLayout->addWidget(m_probeWidget);
        displayLayout->addStretch();

        // 右侧：控制面板
        QWidget *controlWidget = new QWidget(this);
        QVBoxLayout *controlLayout = new QVBoxLayout(controlWidget);

        // 基本信息设置
        QGroupBox *basicGroup = new QGroupBox("基本信息", this);
        QFormLayout *basicLayout = new QFormLayout(basicGroup);
        
        m_numberSpinBox = new QSpinBox(this);
        m_numberSpinBox->setRange(1, 99);
        m_numberSpinBox->setValue(1);
        
        m_temperatureSpinBox = new QSpinBox(this);
        m_temperatureSpinBox->setRange(-999, 999);
        m_temperatureSpinBox->setValue(20);
        
        basicLayout->addRow("探头编号:", m_numberSpinBox);
        basicLayout->addRow("温度值 (°C):", m_temperatureSpinBox);

        // 阈值设置
        QGroupBox *thresholdGroup = new QGroupBox("温度阈值", this);
        QFormLayout *thresholdLayout = new QFormLayout(thresholdGroup);
        
        m_lowThresholdSpinBox = new QSpinBox(this);
        m_lowThresholdSpinBox->setRange(-50, 50);
        m_lowThresholdSpinBox->setValue(15);
        
        m_highThresholdSpinBox = new QSpinBox(this);
        m_highThresholdSpinBox->setRange(0, 100);
        m_highThresholdSpinBox->setValue(30);
        
        thresholdLayout->addRow("低温阈值:", m_lowThresholdSpinBox);
        thresholdLayout->addRow("高温阈值:", m_highThresholdSpinBox);

        // 显示设置
        QGroupBox *displayGroup = new QGroupBox("显示设置", this);
        QVBoxLayout *displayGroupLayout = new QVBoxLayout(displayGroup);
        
        m_highlightCheckBox = new QCheckBox("高亮显示", this);
        m_autoUpdateCheckBox = new QCheckBox("自动更新温度", this);
        m_autoUpdateCheckBox->setChecked(true);
        
        displayGroupLayout->addWidget(m_highlightCheckBox);
        displayGroupLayout->addWidget(m_autoUpdateCheckBox);

        // 操作按钮
        QGroupBox *actionGroup = new QGroupBox("操作", this);
        QVBoxLayout *actionLayout = new QVBoxLayout(actionGroup);
        
        QPushButton *updateBtn = new QPushButton("更新设置", this);
        QPushButton *resetBtn = new QPushButton("重置探头", this);
        QPushButton *randomBtn = new QPushButton("随机温度", this);
        
        actionLayout->addWidget(updateBtn);
        actionLayout->addWidget(resetBtn);
        actionLayout->addWidget(randomBtn);

        // 状态显示
        m_statusLabel = new QLabel(this);
        m_statusLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border: 1px solid #ccc; border-radius: 4px;");
        m_statusLabel->setMinimumHeight(40);

        // 组装控制面板
        controlLayout->addWidget(basicGroup);
        controlLayout->addWidget(thresholdGroup);
        controlLayout->addWidget(displayGroup);
        controlLayout->addWidget(actionGroup);
        controlLayout->addWidget(m_statusLabel);
        controlLayout->addStretch();

        // 组装主界面
        mainLayout->addWidget(displayWidget, 1);
        mainLayout->addWidget(controlWidget, 1);

        setCentralWidget(centralWidget);
        setWindowTitle("TemperatureProbeWidget 单元测试");
        resize(700, 500);

        // 初始化状态
        resetProbe();
    }

    void setupConnections()
    {
        // 连接所有控制信号
        connect(m_numberSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TestWindow::updateNumber);
        connect(m_temperatureSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TestWindow::updateTemperature);
        connect(m_lowThresholdSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TestWindow::updateThresholds);
        connect(m_highThresholdSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TestWindow::updateThresholds);
        connect(m_highlightCheckBox, &QCheckBox::toggled, this, &TestWindow::toggleHighlight);
        
        // 连接按钮
        QList<QPushButton*> buttons = findChildren<QPushButton*>();
        for (QPushButton* btn : buttons) {
            if (btn->text() == "更新设置") {
                connect(btn, &QPushButton::clicked, this, [this]() {
                    updateNumber();
                    updateTemperature();
                    updateThresholds();
                    toggleHighlight();
                });
            } else if (btn->text() == "重置探头") {
                connect(btn, &QPushButton::clicked, this, &TestWindow::resetProbe);
            } else if (btn->text() == "随机温度") {
                connect(btn, &QPushButton::clicked, this, [this]() {
                    int temp = QRandomGenerator::global()->bounded(-10, 50);
                    m_temperatureSpinBox->setValue(temp);
                    updateTemperature();
                });
            }
        }
    }

    void updateStatus()
    {
        QString status = QString("探头状态: No.%1 | 温度: %2°C | 阈值: %3°C ~ %4°C | 高亮: %5")
                            .arg(m_numberSpinBox->value())
                            .arg(m_temperatureSpinBox->value())
                            .arg(m_lowThresholdSpinBox->value())
                            .arg(m_highThresholdSpinBox->value())
                            .arg(m_highlightCheckBox->isChecked() ? "是" : "否");
        m_statusLabel->setText(status);
    }

    TemperatureProbeWidget *m_probeWidget;
    QSpinBox *m_numberSpinBox;
    QSpinBox *m_temperatureSpinBox;
    QSpinBox *m_lowThresholdSpinBox;
    QSpinBox *m_highThresholdSpinBox;
    QCheckBox *m_highlightCheckBox;
    QCheckBox *m_autoUpdateCheckBox;
    QLabel *m_statusLabel;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TestWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"