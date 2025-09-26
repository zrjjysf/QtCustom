#include "gunidbadge.h"
#include <QPainter>

GunIdBadge::GunIdBadge(const QString &text, QWidget *parent)
    : QWidget(parent), m_text(text) {
    // setAttribute(Qt::WA_TransparentForMouseEvents); // 点击事件透传
    // setAttribute(Qt::WA_TranslucentBackground);     // 背景透明
    setFixedSize(50, 50); // 默认大小
}

void GunIdBadge::setText(const QString &text) {
    m_text = text;
    update();
}

QSize GunIdBadge::sizeHint() const {
    return QSize(50, 50);
}

void GunIdBadge::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制圆形背景
    painter.setBrush(QColor(255, 100, 100));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(rect());

    // 绘制文字
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, m_text);
}
