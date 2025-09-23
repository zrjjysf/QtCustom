#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QTableWidget>
#include <QPainter>

class CustomTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CustomTableWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MYTABLEWIDGET_H
