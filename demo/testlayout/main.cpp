#include <QApplication>
#include <QWidget>
#include <QFrame>
#include <QLayout>
#include <QPushButton>
#include "tool.h"

// 演示自定义滚动条的使用
int main(int argc, char *argv[])
{
    // 加载 QSS 样式表
    auto cleanInterContentsMargins = [](QLayout* layout){
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
    };
    QApplication app(argc, argv);
    app.setStyleSheet(getQSS());
    QWidget windows;
    windows.resize(1024, 600);
    QVBoxLayout *winlayout=new QVBoxLayout(&windows);
    cleanInterContentsMargins(winlayout);
    QFrame mianWin;
    mianWin.setObjectName("statusBar");
    mianWin.setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
    winlayout->addWidget(&mianWin);
    winlayout->addStretch(1);
    QHBoxLayout *mainlayout=new QHBoxLayout(&mianWin);
    QHBoxLayout *leftlayout=new QHBoxLayout;
    QHBoxLayout *rightlayout=new QHBoxLayout;
    mainlayout->addLayout(leftlayout,1);
    mainlayout->addStretch(1);
    mainlayout->addLayout(rightlayout,1);

    auto createButtom=[&](QString name){
        QPushButton* a=new QPushButton(name);
        a->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        QObject::connect(a, &QPushButton::pressed,[&](){cleanQSS();app.setStyleSheet(getQSS());});
        return a;
    };

    leftlayout->addWidget(createButtom("left1"),1);
    leftlayout->addWidget(createButtom("left2"),1);
    rightlayout->addWidget(createButtom("right1"),1);
    rightlayout->addWidget(createButtom("right2"),1);
    cleanInterContentsMargins(mainlayout);
    cleanInterContentsMargins(leftlayout);
    cleanInterContentsMargins(rightlayout);

    windows.show();

    return app.exec();
}
