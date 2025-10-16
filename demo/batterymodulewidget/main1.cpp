#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <QScrollArea>
#include <QRandomGenerator>
#include <QLabel>
#include <QVector>
#include "batterycellwidget.h"
#include "batterymodulewidget.h"
#include <QDebug>

struct ABSCellVoltage {
    float voltage; ///< 电压值，单位：V，-1表示无效
    ABSCellVoltage(float vol)
    {
        voltage = vol;
    }
    operator float() const {
        return voltage;
    }
};

int main(int argc, char *argv[])
{
    QVector<ABSCellVoltage> a;
    a.append(ABSCellVoltage{1.0});
    a.append(ABSCellVoltage{2.0});
    a.append(ABSCellVoltage{3.0});
    QVector<float> value(a.begin(),a.end());
    qDebug()<<value;
}