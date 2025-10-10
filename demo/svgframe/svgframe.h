#ifndef SVGFRAME_H
#define SVGFRAME_H

#include <QFrame>
#include <QPixmap>

class SvgFrame : public QFrame
{
    Q_OBJECT
public:
    explicit SvgFrame(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPixmap m_normal;
    QPixmap m_pressed;
    bool m_isPressed = false;
};

#endif // SVGFRAME_H
