#include "templatesvgwidget.h"
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include "templatesvgrenderer.h"

TemplateSvgWidget::TemplateSvgWidget(QWidget *parent) 
    : QSvgWidget(parent)
    , m_renderer(new TemplateSvgRenderer(this))
    , m_rendererOwned(true)
{
    setAttribute(Qt::WA_StyledBackground, true);
}

TemplateSvgWidget::TemplateSvgWidget(const QString &file, QWidget *parent)
    : QSvgWidget(parent)
    , m_path(file)
    , m_renderer(new TemplateSvgRenderer(this))
    , m_rendererOwned(true)
{
    setAttribute(Qt::WA_StyledBackground, true);
    m_renderer->load(file);
}

TemplateSvgWidget::TemplateSvgWidget(TemplateSvgRenderer* renderer, bool rendererOwned, QWidget *parent)
    : QSvgWidget(parent)
    , m_renderer(renderer)
    , m_rendererOwned(rendererOwned)
{
    setAttribute(Qt::WA_StyledBackground, true);
}

TemplateSvgWidget::~TemplateSvgWidget()
{
    // 只有自己创建的渲染器才需要处理
    if (m_rendererOwned && m_renderer) {
        delete m_renderer;
    }
}

void TemplateSvgWidget::setSvgPath(const QString &path)
{
    m_path = path;
    QString realPath = path;

    if (realPath.startsWith("url(") && realPath.endsWith(")"))
        realPath = realPath.mid(4, realPath.size() - 5);

    if (m_renderer) {
        m_renderer->load(realPath);
        update();
    }
}

TemplateSvgRenderer* TemplateSvgWidget::renderer()
{
    return m_renderer;
}

void TemplateSvgWidget::setRenderer(TemplateSvgRenderer* renderer, bool rendererOwned)
{
    // 如果之前有自己创建的渲染器，先清理
    if (m_rendererOwned && m_renderer) {
        delete m_renderer;
    }
    
    m_renderer = renderer;
    m_rendererOwned = rendererOwned;
    
    reload();
}

void TemplateSvgWidget::reload()
{
    if (m_path.isEmpty() || !m_renderer)
        return;

    QString realPath = m_path;
    if (realPath.startsWith("url(") && realPath.endsWith(")"))
        realPath = realPath.mid(4, realPath.size() - 5);

    m_renderer->load(realPath);
}

void TemplateSvgWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if (!m_renderer) return;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    m_renderer->render(&p, rect());
}

void TemplateSvgWidget::changeEvent(QEvent *e)
{
    QSvgWidget::changeEvent(e);
    if (e->type() == QEvent::PaletteChange || e->type() == QEvent::StyleChange) {
        reload();
    }
}