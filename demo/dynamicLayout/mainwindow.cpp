#include "mainwindow.h"
#include <QDebug>

ColorArea::ColorArea(const QString &color, const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    setStyleSheet(QString("background:%1; color:white; font-size:24px; border:none;").arg(color));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(1024, 600);
    setFixedSize(1024, 600); // ✅ 锁定窗口大小，防止被布局撑大

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);

    QStringList colors = {"#f66", "#6f6", "#66f", "#fc6"};
    QStringList names  = {"区域 A", "区域 B", "区域 C", "区域 D"};

    for (int i = 0; i < colors.size(); ++i) {
        ColorArea *area = new ColorArea(colors[i], names[i], this);
        areas << area;
        mainLayout->addWidget(area);
        connect(area, &QPushButton::clicked, this, [=](){
            onAreaClicked(i);
        });
    }

    // 初始平均分配宽度
    int initWidth = width() / areas.size();
    for (auto *a : areas)
        a->setFixedWidth(initWidth);
}

void MainWindow::onAreaClicked(int index)
{
    if (index == currentIndex) return;
    animateTo(index);
    currentIndex = index;
}

void MainWindow::animateTo(int index)
{
    int totalWidth = width();
    int smallWidth = totalWidth * 0.12;   // 其他区域占比
    int largeWidth = totalWidth - smallWidth * (areas.size() - 1);

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    for (int i = 0; i < areas.size(); ++i) {
        QWidget *area = areas[i];
        int startW = area->width();
        int endW = (i == index ? largeWidth : smallWidth);

        // ✅ 改为动画 fixedWidth
        QPropertyAnimation *ani = new QPropertyAnimation(area, "maximumWidth");
        ani->setDuration(300);
        ani->setStartValue(startW);
        ani->setEndValue(endW);
        ani->setEasingCurve(QEasingCurve::InOutCubic);

        // 动画过程中同步 fixedWidth
        connect(ani, &QPropertyAnimation::valueChanged, this, [area](const QVariant &v){
            area->setFixedWidth(v.toInt());
        });

        group->addAnimation(ani);
    }

    connect(group, &QParallelAnimationGroup::finished, group, &QObject::deleteLater);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}
