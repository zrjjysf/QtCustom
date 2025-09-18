#ifndef ASPECTRATIOLAYOUT_H
#define ASPECTRATIOLAYOUT_H

#include <QLayout>
#include <QWidgetItem>

class AspectRatioLayout : public QLayout
{
    Q_OBJECT

public:
    explicit AspectRatioLayout(double ratio, QWidget *parent = nullptr);
    ~AspectRatioLayout();

    void addItem(QLayoutItem *item) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    int count() const override;
    void setGeometry(const QRect &rect) override;

private:
    double m_ratio;
    QList<QLayoutItem *> m_items;
};

#endif // ASPECTRATIOLAYOUT_H
