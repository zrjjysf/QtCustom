#ifndef QSIDETABWIDGET_H
#define QSIDETABWIDGET_H

#include <QWidget>
#include <QList>
#include <QIcon>

class QPushButton;
class QStackedWidget;

class QSideTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QSideTabWidget(QWidget *parent = nullptr);

    int addTab(QWidget *widget, const QString &label);
    int addTab(QWidget *widget, const QIcon &icon, const QString &label);

    int insertTab(int index, QWidget *widget, const QString &label);
    int insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label);

    void removeTab(int index);

    int currentIndex() const;
    QWidget* currentWidget() const;
    QWidget* widget(int index) const;
    int count() const;

    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);

    void setQStyleSheet(const QString &style);

signals:
    void currentChanged(int index);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void expandSidebar();
    void changePage(int index);

private:
    void applyDefaultStyleSheet();
    void updateTabStyles();
    void toggleSidebar(bool expand); // 新增：展开/收起封装函数

private:
    QWidget *sidebar;
    QPushButton *expandButton;
    QStackedWidget *stack;
    QList<QPushButton*> menuButtons;
    QList<QWidget*> widgets;
    QList<QString> tabLabels;
    QList<QIcon> tabIcons;
    bool isCollapsed;
    QString customStyleSheet;
};

#endif // QSIDETABWIDGET_H
