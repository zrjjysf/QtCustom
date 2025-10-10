#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ===== 左侧侧边栏 =====
    sidebar = new QWidget(this);
    sidebar->setFixedWidth(60); // 初始收起状态
    sidebar->setStyleSheet("background-color:#2c3e50;");

    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(5, 5, 5, 5);
    sideLayout->setSpacing(10);

    // 折叠展开按钮
    QPushButton *toggleBtn = new QPushButton("≡", sidebar);
    toggleBtn->setFixedHeight(40);
    toggleBtn->setStyleSheet("font-size:18px; color:white; background-color:#34495e;");

    // 菜单按钮
    QPushButton *btn1 = new QPushButton("", sidebar);
    btn1->setIcon(QIcon::fromTheme("go-home"));
    QPushButton *btn2 = new QPushButton("", sidebar);
    btn2->setIcon(QIcon::fromTheme("preferences-system"));
    QPushButton *btn3 = new QPushButton("", sidebar);
    btn3->setIcon(QIcon::fromTheme("help-about"));

    menuButtons = {btn1, btn2, btn3};
    for (QPushButton *btn : menuButtons) {
        btn->setStyleSheet(R"(
            QPushButton {
                color: white;
                background-color: #2c3e50;
                border: none;
                padding: 6px;
            }
            QPushButton:hover {
                background-color: #3b4b61;
            }
        )");
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(40);
        sideLayout->addWidget(btn);
    }

    sideLayout->insertWidget(0, toggleBtn);
    sideLayout->addStretch();

    // ===== 右侧内容区 =====
    stack = new QStackedWidget(this);
    stack->addWidget(new QLabel("这里是首页内容"));
    stack->addWidget(new QLabel("这里是设置页面"));
    stack->addWidget(new QLabel("这里是关于页面"));

    QFont f; f.setPointSize(16);
    for (int i = 0; i < stack->count(); ++i)
        stack->widget(i)->setFont(f);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack, 1);

    // ===== 信号连接 =====
    connect(toggleBtn, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(btn1, &QPushButton::clicked, [=]() { changePage(0); });
    connect(btn2, &QPushButton::clicked, [=]() { changePage(1); });
    connect(btn3, &QPushButton::clicked, [=]() { changePage(2); });
}

void MainWindow::changePage(int index)
{
    stack->setCurrentIndex(index);
}

void MainWindow::toggleSidebar()
{
    if (isCollapsed) {
        sidebar->setFixedWidth(200);
        menuButtons[0]->setText(" 首页");
        menuButtons[1]->setText(" 设置");
        menuButtons[2]->setText(" 关于");
    } else {
        sidebar->setFixedWidth(60);
        for (QPushButton *btn : menuButtons) {
            btn->setText("");
        }
    }
    isCollapsed = !isCollapsed;
}
