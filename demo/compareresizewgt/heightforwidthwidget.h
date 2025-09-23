#ifndef HEIGHTFORWIDTHWIDGET_H
#define HEIGHTFORWIDTHWIDGET_H

#include <QWidget>
#include <QPainter>

class HeightForWidthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeightForWidthWidget(double ratio, QWidget *parent = nullptr);

    bool hasHeightForWidth() const override { return true; }
    int heightForWidth(int w) const override { return int(w / m_ratio); }
    QSize sizeHint() const override { return QSize(200, int(200 / m_ratio)); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_ratio; // width / height
};

#endif // HEIGHTFORWIDTHWIDGET_H
