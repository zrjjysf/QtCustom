#ifndef VERTICALINFOWIDGET_H
#define VERTICALINFOWIDGET_H

#include <QFrame>

class QLabel;
class VerticalInfoWidget : public QFrame
{
    Q_OBJECT
public:
    explicit VerticalInfoWidget(QWidget *parent = nullptr);

    void setTopText(const QString &text);
    void setBottomText(const QString &text);
private:
    QLabel *m_topLabel;
    QLabel *m_bottomLabel;
private slots:
    void wrapStyleProxy();
};

#endif // VERTICALINFOWIDGET_H
