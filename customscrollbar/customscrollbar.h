#ifndef CUSTOMSCROLLBAR_H
#define CUSTOMSCROLLBAR_H

#include <QScrollBar>
#include <QSvgRenderer>
#include <QHash>
#include <QPixmap>

class CustomScrollBar : public QScrollBar
{
    Q_OBJECT

public:
    explicit CustomScrollBar(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 渲染按钮，带缓存
    void draw3DButton(QPainter &p, const QRect &rect, QSvgRenderer *renderer, bool pressed);
    void draw3DButton(QPainter &p, const QRect &rect, const QString &text, bool pressed);

    int indicatorThickness() const;
    QRect topButtonRect() const;
    QRect pageUpButtonRect() const;
    QRect pageDownButtonRect() const;
    QRect bottomButtonRect() const;
    QRect grooveRect() const;
    QRect sliderRect() const;

    bool m_dragging;
    bool m_topButtonPressed, m_pageUpButtonPressed, m_pageDownButtonPressed, m_bottomButtonPressed;
    QPoint m_dragStartPos;
    int m_dragStartValue;

    QSvgRenderer *m_turnTopRenderer;
    QSvgRenderer *m_turnUpRenderer;
    QSvgRenderer *m_turnDownRenderer;
    QSvgRenderer *m_turnBottomRenderer;

    // 缓存 key = QSize
    mutable QHash<QSize, QPixmap> m_topCache;
    mutable QHash<QSize, QPixmap> m_upCache;
    mutable QHash<QSize, QPixmap> m_downCache;
    mutable QHash<QSize, QPixmap> m_bottomCache;

    QPixmap renderSvgToPixmap(QSvgRenderer *renderer, const QSize &size, QHash<QSize, QPixmap> &cache) const;
};

#endif // CUSTOMSCROLLBAR_H
