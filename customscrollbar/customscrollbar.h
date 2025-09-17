#ifndef CUSTOMSCROLLBAR_H
#define CUSTOMSCROLLBAR_H

#include <QScrollBar>
#include <QRect>
#include <QPoint>

class CustomScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit CustomScrollBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    
private:
    QRect topButtonRect() const;
    QRect pageUpButtonRect() const;
    QRect pageDownButtonRect() const;
    QRect bottomButtonRect() const;
    QRect grooveRect() const;
    QRect sliderRect() const;
    
    void draw3DButton(QPainter &p, const QRect &rect, const QString &svg, bool pressed);
    
    bool m_dragging;
    QPoint m_dragStartPos;
    int m_dragStartValue;
    
    // 按钮按下状态
    bool m_topButtonPressed;
    bool m_pageUpButtonPressed;
    bool m_pageDownButtonPressed;
    bool m_bottomButtonPressed;
};

#endif // CUSTOMSCROLLBAR_H