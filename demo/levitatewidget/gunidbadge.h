#ifndef GUNIDBADGE_H
#define GUNIDBADGE_H

#include <QWidget>

class GunIdBadge : public QWidget {
    Q_OBJECT
public:
    explicit GunIdBadge(const QString &text, QWidget *parent = nullptr);
    void setText(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QString m_text;
};

#endif // GUNIDBADGE_H
