#ifndef ICONLABEL_H
#define ICONLABEL_H

#include <QFrame>


class QLabel;
class ThemeSvgWgt;
class IconLabel : public QFrame
{
    Q_OBJECT
public:
    explicit IconLabel(QWidget *parent = nullptr);

    void setText(const QString &text);
    void resizeEvent(QResizeEvent *event) override;
private:
    QLabel *m_label;
    ThemeSvgWgt* m_icon;
};

#endif // ICONLABEL_H
