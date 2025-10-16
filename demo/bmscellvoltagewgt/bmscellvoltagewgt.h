#ifndef BMSCELLVOLTAGEWGT_H
#define BMSCELLVOLTAGEWGT_H

#include <QWidget>
#include <QMap>

class QLabel;
class QTimer;
class QVBoxLayout;
class QHBoxLayout;
class QScrollArea;
class QStackedLayout;
class BatteryPackWidget;
class BatteryModuleWidget;

class BMSCellVoltageWgt : public QWidget {
    Q_OBJECT
public:
    explicit BMSCellVoltageWgt(QWidget *parent = nullptr);
    void updateData();

private:
    void initMyConnect();

    QTimer *m_refreshTimer;

    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_leftLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;

    QStackedLayout *m_stackLayout;

    QMap<int, BatteryPackWidget*> m_packWidgets;
    QMap<int, BatteryModuleWidget*> m_moduleWidgets;

    void handleSwitch(int id);
    int currentID = -1;
};

#endif // BMSCELLVOLTAGEWGT_H
