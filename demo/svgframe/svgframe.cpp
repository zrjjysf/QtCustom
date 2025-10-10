#include "svgframe.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QMouseEvent>

SvgFrame::SvgFrame(QWidget *parent)
    : QFrame(parent)
{
    setFixedSize(200, 100);

    // 加载 normal.svg
    {
        QSvgRenderer renderer(QStringLiteral("./normal.svg"));
        m_normal = QPixmap(size());
        m_normal.fill(Qt::transparent);
        QPainter p(&m_normal);
        renderer.render(&p);
    }

    // 加载 pressed.svg
    {
        QSvgRenderer renderer(QStringLiteral("./pressed.svg"));
        m_pressed = QPixmap(size());
        m_pressed.fill(Qt::transparent);
        QPainter p(&m_pressed);
        renderer.render(&p);
    }
}

void SvgFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(0, 0, m_isPressed ? m_pressed : m_normal);
}

void SvgFrame::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    update();
    QFrame::mousePressEvent(event);
}

void SvgFrame::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    update();
    QFrame::mouseReleaseEvent(event);
}
