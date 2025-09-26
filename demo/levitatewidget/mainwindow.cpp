#include "mainwindow.h"
#include "gunidbadge.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("枪界面 Demo");
    resize(600, 400);

    tabWidget = new QTabWidget(this);

    // 页面1
    QWidget *page1 = new QWidget;
    QVBoxLayout *layout1 = new QVBoxLayout(page1);
    layout1->addWidget(new QLabel("这是 枪A 的界面内容"));
    tabWidget->addTab(page1, "枪A");

    // 页面2
    QWidget *page2 = new QWidget;
    QVBoxLayout *layout2 = new QVBoxLayout(page2);
    layout2->addWidget(new QLabel("这是 枪B 的界面内容"));
    tabWidget->addTab(page2, "枪B");

    // 页面3
    QWidget *page3 = new QWidget;
    QVBoxLayout *layout3 = new QVBoxLayout(page3);
    layout3->addWidget(new QLabel("这是 枪C 的界面内容"));
    tabWidget->addTab(page3, "枪C");

    setCentralWidget(tabWidget);

    // 在 tabWidget 之上叠加标识控件
    badge = new GunIdBadge("枪A", tabWidget);
    badge->move(10, 20); // 左上角

    // 监听 tab 切换，更新标识文字
    connect(tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
        QString label = tabWidget->tabText(index);
        badge->setText(label);
    });
}