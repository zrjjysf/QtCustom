#include "themesvgwgt.h"
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include "themedsvgrenderer.h"

ThemeSvgWgt::ThemeSvgWgt(QWidget *parent) : QSvgWidget(parent), m_renderer(new ThemedSvgRenderer(this)) {}

ThemeSvgWgt::ThemeSvgWgt(const QString &file, QWidget *parent)
    : QSvgWidget(parent), m_path(file), m_renderer(new ThemedSvgRenderer(this))
{
    setAttribute(Qt::WA_StyledBackground, true); // 启用样式表背景支持
    m_renderer->load(file, palette());
}

void ThemeSvgWgt::setSvgPath(const QString &path)
{
    m_path = path;
    QString realPath = path;

    if (realPath.startsWith("url(") && realPath.endsWith(")"))
        realPath = realPath.mid(4, realPath.size() - 5);

    m_renderer->load(realPath, palette());
    update();
}

void ThemeSvgWgt::reload()
{
    if (m_path.isEmpty())
        return;

    QString realPath = m_path;
    if (realPath.startsWith("url(") && realPath.endsWith(")"))
        realPath = realPath.mid(4, realPath.size() - 5);

    m_renderer->load(realPath, palette());
}

// void ThemeSvgWgt::paintEvent(QPaintEvent *event)
// {
//     Q_UNUSED(event)
//     QPainter p(this);
//     p.setRenderHint(QPainter::Antialiasing);
//     m_renderer->render(&p, rect());
// }
void ThemeSvgWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QSizeF svgSize = m_renderer->defaultSize();
    if (svgSize.isEmpty()) {
        m_renderer->render(&p, rect()); // fallback
        return;
    }

    QRectF targetRect = rect();
    qreal scale = qMin(
        targetRect.width() / svgSize.width(),
        targetRect.height() / svgSize.height()
    );

    QSizeF scaledSize = svgSize * scale;
    QPointF topLeft(
        targetRect.x() + (targetRect.width() - scaledSize.width()) / 2,
        targetRect.y() + (targetRect.height() - scaledSize.height()) / 2
    );

    QRectF renderRect(topLeft, scaledSize);
    m_renderer->render(&p, renderRect);
}

void ThemeSvgWgt::changeEvent(QEvent *e)
{
    QSvgWidget::changeEvent(e);
    if (e->type() == QEvent::PaletteChange || e->type() == QEvent::StyleChange) {
        // 子控件样式/调色板一变，自动按新颜色重载
        reload();
    }
}