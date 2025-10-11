#ifndef BATTERYCELLWIDGET_H
#define BATTERYCELLWIDGET_H

#include <QWidget>
#include <QString>

class BatteryCellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BatteryCellWidget(QWidget *parent = nullptr);

    // 设置编号
    void setNumber(int number);

    // 设置中间文本
    void setCenterText(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_number;
    QString m_centerText;
};

#endif // BATTERYCELLWIDGET_H
