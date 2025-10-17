#ifndef BMSTEMPERATUREPROBEWGT_H
#define BMSTEMPERATUREPROBEWGT_H

#include <QWidget>
#include <QMap>

class QLabel;
class QTimer;
class QHBoxLayout;
class QScrollArea;
class QStackedLayout;
class BatteryPackWidget;
class TemperatureModuleWidget;
class FlowLayout;

class BMSTemperatureProbeWgt : public QWidget {
    Q_OBJECT
public:
    explicit BMSTemperatureProbeWgt(QWidget *parent = nullptr);
    void updateData();

private:
    void initMyConnect();
    void handleSwitch(int id);

    QTimer *m_refreshTimer;

    QHBoxLayout *m_mainLayout;
    FlowLayout *m_leftLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;

    QStackedLayout *m_stackLayout;

    QMap<int, BatteryPackWidget*> m_packWidgets;
    QMap<int, TemperatureModuleWidget*> m_moduleWidgets;
    QVector<int> m_index;//id存放的位置
    QWidget* m_stackContainer;
    int m_currentID = -1;
};

#endif // BMSTEMPERATUREPROBEWGT_H
