#ifndef RESIZEEVENTWIDGET_H
#define RESIZEEVENTWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include <QPainter>

class ResizeEventWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResizeEventWidget(double ratio, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    double m_ratio;
};

#endif // RESIZEEVENTWIDGET_H
