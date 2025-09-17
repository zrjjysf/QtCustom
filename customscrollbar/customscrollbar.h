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
    bool m_dragging;
    QPoint m_dragStartPos;
    int m_dragStartValue;
};

#endif // CUSTOMSCROLLBAR_H
