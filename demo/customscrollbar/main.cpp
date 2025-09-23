#include <QApplication>
#include <QTextEdit>
#include "customscrollbar.h"
#include "tool.h"

// 演示自定义滚动条的使用
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 使用工具函数加载全局样式
    app.setStyleSheet(getQSS());

    QTextEdit edit;

    // 设置自定义的滚动条
    CustomScrollBar* vScr = new CustomScrollBar(Qt::Orientation::Vertical,&edit);
    CustomScrollBar* hScr = new CustomScrollBar(Qt::Orientation::Horizontal,&edit);
    vScr->setStyleSheet(R"(CustomScrollBar
    {
        min-width:20px; 
        min-height:20px; 
    })");
    hScr->setStyleSheet(R"(CustomScrollBar
    {
        min-width:20px; 
        min-height:20px; 
    })");
    // vScr->setMinimumHeight(100);
    // hScr->setMinimumWidth(40);
    edit.setVerticalScrollBar(vScr);
    edit.setHorizontalScrollBar(hScr);
    edit.setLineWrapMode(QTextEdit::NoWrap);//禁止换行，这样才能出水平滚动条
    // 填充测试内容
    for (int i = 1; i <= 100; ++i) {
        edit.append(QString::fromUtf8("这是第 %1 行内容，是一个非常非常非常非常非常非常非常非常非常非常非常非常非常非常非常非常非常长的内容，用来测试水平滚动条是否出现").arg(i));
    }

    edit.resize(400, 300);
    edit.show();

    return app.exec();
}
