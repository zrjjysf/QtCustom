#include "customscrollbar.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStyle>

CustomScrollBar::CustomScrollBar(QWidget *parent)
    : QScrollBar(Qt::Vertical, parent)
{
}

QRect CustomScrollBar::topButtonRect() const {
    return QRect(0, 0, width(), width()*2);
}

QRect CustomScrollBar::pageUpButtonRect() const {
    return QRect(0, width()*2, width(), width()*2);
}

QRect CustomScrollBar::pageDownButtonRect() const {
    return QRect(0, height() - 2 * width()*2, width(), width()*2);
}

QRect CustomScrollBar::bottomButtonRect() const {
    return QRect(0, height() - width()*2, width(), width()*2);
}

void CustomScrollBar::paintEvent(QPaintEvent *event)
{
    // 先画默认的滚动条
    // {
    //     setFixedHeight(int h)
    // }
    QScrollBar::paintEvent(event);

    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(palette().button());

    // 按钮区域
    QRect rTop = topButtonRect();
    QRect rPgUp = pageUpButtonRect();
    QRect rPgDn = pageDownButtonRect();
    QRect rBottom = bottomButtonRect();

    p.fillRect(rTop, palette().button());
    p.fillRect(rPgUp, palette().button());
    p.fillRect(rPgDn, palette().button());
    p.fillRect(rBottom, palette().button());

    p.drawRect(rTop);
    p.drawRect(rPgUp);
    p.drawRect(rPgDn);
    p.drawRect(rBottom);

    // 文本符号
    p.drawText(rTop, Qt::AlignCenter, "⤒");
    p.drawText(rPgUp, Qt::AlignCenter, "▲");
    p.drawText(rPgDn, Qt::AlignCenter, "▼");
    p.drawText(rBottom, Qt::AlignCenter, "⤓");
}

void CustomScrollBar::mousePressEvent(QMouseEvent *event)
{
    if (topButtonRect().contains(event->pos())) {
        setValue(minimum()); // 滚动到顶部
        return;
    }
    if (pageUpButtonRect().contains(event->pos())) {
        setValue(value() - pageStep()); // 上翻一页
        return;
    }
    if (pageDownButtonRect().contains(event->pos())) {
        setValue(value() + pageStep()); // 下翻一页
        return;
    }
    if (bottomButtonRect().contains(event->pos())) {
        setValue(maximum()); // 滚动到底部
        return;
    }

    // 其他情况仍用默认行为
    QScrollBar::mousePressEvent(event);
}
