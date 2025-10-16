#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QTimer>
#include <QSlider>
#include <QCheckBox>
#include <QRandomGenerator>
#include <QGroupBox>

#include "temperaturemodulewidget.h"

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    TestWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setupUI();
        setupConnections();
        
        // 启动定时器模拟实时数据更新
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &TestWindow::updateRandomData);
        timer->start(2000); // 每2秒更新一次
    }

private slots:
    void addProbe()
    {
        static int probeNumber = m_moduleWidget->getProbeCount() + 1;
        TemperatureProbeWidget *probe = new TemperatureProbeWidget();
        probe->setNumber(probeNumber++);
        probe->setTemperature(QRandomGenerator::global()->bounded(10, 40));
        m_moduleWidget->addTemperatureProbe(probe);
        updateStatus();
    }

    void clearProbes()
    {
        m_moduleWidget->clearProbes();
        updateStatus();
    }

    void updateRandomData()
    {
        if (m_moduleWidget->getProbeCount() == 0) return;

        QVector<int> temperatures;
        int probeCount = m_moduleWidget->getProbeCount();
        
        for (int i = 0; i < probeCount; ++i) {
            int temp = QRandomGenerator::global()->bounded(10, 40);
            temperatures.append(temp);
        }
        
        m_moduleWidget->updateProbeData(temperatures);
        updateStatus();
    }

    void setCustomData()
    {
        int probeCount = m_probeCountSpinBox->value();
        QVector<int> temperatures;
        
        for (int i = 0; i < probeCount; ++i) {
            temperatures.append(QRandomGenerator::global()->bounded(10, 40));
        }
        
        m_moduleWidget->updateProbeData(temperatures);
        updateStatus();
    }

    void updateThresholds()
    {
        int low = m_lowThresholdSpinBox->value();
        int high = m_highThresholdSpinBox->value();
        m_moduleWidget->setTemperatureThresholds(low, high);
    }

    void updateStatus()
    {
        QString status = QString("探头数量: %1 | 最高温度: %2°C (探头%3) | 最低温度: %4°C (探头%5)")
                            .arg(m_moduleWidget->getProbeCount())
                            .arg(m_moduleWidget->getMaxTemperature())
                            .arg(m_moduleWidget->getMaxTemperatureProbeIndex() + 1)
                            .arg(m_moduleWidget->getMinTemperature())
                            .arg(m_moduleWidget->getMinTemperatureProbeIndex() + 1);
        m_statusLabel->setText(status);
    }

private:
    void setupUI()
    {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // 控制面板
        QGroupBox *controlGroup = new QGroupBox("控制面板", this);
        QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);

        // 探头管理
        QPushButton *addBtn = new QPushButton("添加探头", this);
        QPushButton *clearBtn = new QPushButton("清空探头", this);
        QPushButton *randomBtn = new QPushButton("随机数据", this);
        
        m_probeCountSpinBox = new QSpinBox(this);
        m_probeCountSpinBox->setRange(1, 50);
        m_probeCountSpinBox->setValue(8);
        QPushButton *setDataBtn = new QPushButton("设置数据", this);

        controlLayout->addWidget(addBtn);
        controlLayout->addWidget(clearBtn);
        controlLayout->addWidget(randomBtn);
        controlLayout->addWidget(new QLabel("探头数量:", this));
        controlLayout->addWidget(m_probeCountSpinBox);
        controlLayout->addWidget(setDataBtn);
        controlLayout->addStretch();

        // 阈值设置
        QGroupBox *thresholdGroup = new QGroupBox("温度阈值设置", this);
        QHBoxLayout *thresholdLayout = new QHBoxLayout(thresholdGroup);

        m_lowThresholdSpinBox = new QSpinBox(this);
        m_lowThresholdSpinBox->setRange(-50, 50);
        m_lowThresholdSpinBox->setValue(15);
        
        m_highThresholdSpinBox = new QSpinBox(this);
        m_highThresholdSpinBox->setRange(0, 100);
        m_highThresholdSpinBox->setValue(30);
        
        QPushButton *setThresholdBtn = new QPushButton("设置阈值", this);

        thresholdLayout->addWidget(new QLabel("低温阈值:", this));
        thresholdLayout->addWidget(m_lowThresholdSpinBox);
        thresholdLayout->addWidget(new QLabel("°C"));
        thresholdLayout->addWidget(new QLabel("高温阈值:", this));
        thresholdLayout->addWidget(m_highThresholdSpinBox);
        thresholdLayout->addWidget(new QLabel("°C"));
        thresholdLayout->addWidget(setThresholdBtn);
        thresholdLayout->addStretch();

        // 状态显示
        m_statusLabel = new QLabel("就绪", this);
        m_statusLabel->setStyleSheet("background-color: #f0f0f0; padding: 5px; border: 1px solid #ccc;");

        // 温度模块
        m_moduleWidget = new TemperatureModuleWidget(this);
        m_moduleWidget->setNumber(1); // 设置模块编号

        // 组装主界面
        mainLayout->addWidget(controlGroup);
        mainLayout->addWidget(thresholdGroup);
        mainLayout->addWidget(m_statusLabel);
        mainLayout->addWidget(m_moduleWidget, 1);

        setCentralWidget(centralWidget);
        setWindowTitle("TemperatureModuleWidget 单元测试");
        resize(800, 600);

        // 初始化一些测试数据
        initializeTestData();
    }

    void setupConnections()
    {
        connect(findChild<QPushButton*>("添加探头"), &QPushButton::clicked, this, &TestWindow::addProbe);
        connect(findChild<QPushButton*>("清空探头"), &QPushButton::clicked, this, &TestWindow::clearProbes);
        connect(findChild<QPushButton*>("随机数据"), &QPushButton::clicked, this, &TestWindow::updateRandomData);
        connect(findChild<QPushButton*>("设置数据"), &QPushButton::clicked, this, &TestWindow::setCustomData);
        connect(findChild<QPushButton*>("设置阈值"), &QPushButton::clicked, this, &TestWindow::updateThresholds);
    }

    void initializeTestData()
    {
        // 添加初始测试探头
        for (int i = 1; i <= 8; ++i) {
            TemperatureProbeWidget *probe = new TemperatureProbeWidget();
            probe->setNumber(i);
            probe->setTemperature(QRandomGenerator::global()->bounded(15, 35));
            m_moduleWidget->addTemperatureProbe(probe);
        }
        
        // 设置初始阈值
        updateThresholds();
        updateStatus();
    }

    TemperatureModuleWidget *m_moduleWidget;
    QLabel *m_statusLabel;
    QSpinBox *m_probeCountSpinBox;
    QSpinBox *m_lowThresholdSpinBox;
    QSpinBox *m_highThresholdSpinBox;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TestWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"