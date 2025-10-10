#include "cardframe.h"
#include <QPainter>
#include <QLinearGradient>
#include <QFontMetrics>

CardFrame::CardFrame(const QString &title, QWidget *parent)
    : QFrame(parent), m_title(title)
{
    setFixedSize(200, 150);
    setAttribute(Qt::WA_StyledBackground);
}

void CardFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect().adjusted(0, 0, -1, -1);

    // 背景（白色+圆角）
    p.setBrush(Qt::white);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(r, 10, 10);

    // 右侧阴影
    QLinearGradient gradRight(r.right()-10, r.top(), r.right(), r.top());
    gradRight.setColorAt(0, QColor(0,0,0,40));
    gradRight.setColorAt(1, QColor(0,0,0,0));
    p.fillRect(QRect(r.right()-10, r.top()+10, 10, r.height()-20), gradRight);

    // 底部阴影
    QLinearGradient gradBottom(r.left(), r.bottom()-10, r.left(), r.bottom());
    gradBottom.setColorAt(0, QColor(0,0,0,40));
    gradBottom.setColorAt(1, QColor(0,0,0,0));
    p.fillRect(QRect(r.left()+10, r.bottom()-10, r.width()-20, 10), gradBottom);

    // 标题文字居中
    p.setPen(Qt::black);
    QFont f = font();
    f.setPointSize(14);
    f.setBold(true);
    p.setFont(f);

    QFontMetrics fm(f);
    QRect textRect = fm.boundingRect(m_title);
    QPoint center = r.center() - QPoint(textRect.width()/2, textRect.height()/2);
    p.drawText(center, m_title);
}
