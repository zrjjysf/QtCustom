#ifndef CUSTOMSCROLLBAR_H
#define CUSTOMSCROLLBAR_H

#include <QScrollBar>
#include <QRect>

class CustomScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit CustomScrollBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QRect topButtonRect() const;
    QRect pageUpButtonRect() const;
    QRect pageDownButtonRect() const;
    QRect bottomButtonRect() const;
};

#endif // CUSTOMSCROLLBAR_H
