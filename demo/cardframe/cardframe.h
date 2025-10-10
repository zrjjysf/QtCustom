#ifndef CARDFRAME_H
#define CARDFRAME_H

#include <QFrame>

class CardFrame : public QFrame
{
    Q_OBJECT
public:
    explicit CardFrame(const QString &title, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_title;
};

#endif // CARDFRAME_H
