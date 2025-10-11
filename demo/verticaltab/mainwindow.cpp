#include "mainwindow.h"
#include <QEvent>
#include <QMouseEvent>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    // ===== 左侧侧边栏 =====
    sidebar = new QWidget(this);
    sidebar->setFixedWidth(60);
    sidebar->setStyleSheet("background-color:#2c3e50;");

    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(0,0,0,0);
    sideLayout->setSpacing(5);

    // 展开提示按钮（只是提示）
    expandButton = new QPushButton(">>>", sidebar);
    expandButton->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
    expandButton->setFixedSize(40, 40);
    expandButton->setStyleSheet(
        "color:white; background-color:#2c3e50; border:none;"
    );
    sideLayout->addWidget(expandButton, 1, Qt::AlignCenter);

    // 菜单按钮
    QPushButton *btn1 = new QPushButton(" 首页", sidebar);
    btn1->setIcon(QIcon::fromTheme("go-home"));
    QPushButton *btn2 = new QPushButton(" 设置", sidebar);
    btn2->setIcon(QIcon::fromTheme("preferences-system"));
    QPushButton *btn3 = new QPushButton(" 关于", sidebar);
    btn3->setIcon(QIcon::fromTheme("help-about"));

    menuButtons = {btn1, btn2, btn3};
    for (QPushButton *btn : menuButtons) {
        btn->setStyleSheet(
            "QPushButton {color: white; text-align: left; padding: 6px;"
            "background-color: #2c3e50; border: none;}"
            "QPushButton:hover {background-color: #3b4b61;}"
        );
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(40);
        btn->hide(); // 初始隐藏
        sideLayout->addWidget(btn);
    }

    sideLayout->addStretch(); // 保持按钮从上到下排列

    // ===== 主内容区 =====
    stack = new QStackedWidget(this);
    stack->addWidget(new QLabel("这里是首页内容"));
    stack->addWidget(new QLabel("这里是设置页面"));
    stack->addWidget(new QLabel("这里是关于页面"));
    QFont f; f.setPointSize(16);
    for (int i = 0; i < stack->count(); ++i)
        stack->widget(i)->setFont(f);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack, 1);

    // 信号连接
    connect(expandButton, &QPushButton::clicked, this, &MainWindow::expandSidebar);
    connect(btn1, &QPushButton::clicked, [=]() { changePage(0); });
    connect(btn2, &QPushButton::clicked, [=]() { changePage(1); });
    connect(btn3, &QPushButton::clicked, [=]() { changePage(2); });

    // 点击展开/收起
    sidebar->installEventFilter(this);
    stack->installEventFilter(this);
}

void MainWindow::changePage(int index)
{
    stack->setCurrentIndex(index);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == sidebar && event->type() == QEvent::MouseButtonPress && isCollapsed) {
        expandSidebar();
        return true;
    }
    if (obj == stack && event->type() == QEvent::MouseButtonPress && !isCollapsed) {
        sidebar->setFixedWidth(60);
        for (QPushButton *btn : menuButtons) btn->hide();
        expandButton->show();
        isCollapsed = true;
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::expandSidebar()
{
    sidebar->setFixedWidth(200);
    for (QPushButton *btn : menuButtons) btn->show();
    expandButton->hide();
    isCollapsed = false;
}
