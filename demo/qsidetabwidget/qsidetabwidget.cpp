#include "qsidetabwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QStyle>
#include <QVariant>

QSideTabWidget::QSideTabWidget(QWidget *parent)
    : QWidget(parent), isCollapsed(true)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    sidebar = new QWidget(this);
    sidebar->setFixedWidth(60);

    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(0,0,0,0);
    sideLayout->setSpacing(5);

    expandButton = new QPushButton(">>>", sidebar);
    expandButton->setFixedSize(40, 40);
    sideLayout->addWidget(expandButton, 1, Qt::AlignCenter);
    sideLayout->addStretch();

    mainLayout->addWidget(sidebar);

    stack = new QStackedWidget(this);
    mainLayout->addWidget(stack, 1);

    connect(expandButton, &QPushButton::clicked, this, &QSideTabWidget::expandSidebar);

    sidebar->installEventFilter(this);
    stack->installEventFilter(this);

    applyDefaultStyleSheet();
}

int QSideTabWidget::addTab(QWidget *widget, const QString &label)
{
    return insertTab(count(), widget, label);
}

int QSideTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
    return insertTab(count(), widget, icon, label);
}

int QSideTabWidget::insertTab(int index, QWidget *widget, const QString &label)
{
    return insertTab(index, widget, QIcon(), label);
}

int QSideTabWidget::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label)
{
    QPushButton *btn = new QPushButton(label, sidebar);
    btn->setIcon(icon);
    btn->setFixedHeight(40);
    btn->hide();

    menuButtons.insert(index, btn);
    widgets.insert(index, widget);
    tabLabels.insert(index, label);
    tabIcons.insert(index, icon);

    stack->insertWidget(index, widget);

    QVBoxLayout *sideLayout = qobject_cast<QVBoxLayout*>(sidebar->layout());
    sideLayout->insertWidget(index, btn);

    connect(btn, &QPushButton::clicked, [=]() { changePage(index); });

    setQStyleSheet(customStyleSheet);
    return index;
}

void QSideTabWidget::removeTab(int index)
{
    if (index < 0 || index >= menuButtons.size()) return;

    QWidget *w = widgets.takeAt(index);
    stack->removeWidget(w);

    QPushButton *btn = menuButtons.takeAt(index);
    delete btn;

    tabLabels.removeAt(index);
    tabIcons.removeAt(index);

    delete w;
}

int QSideTabWidget::currentIndex() const
{
    return stack->currentIndex();
}

QWidget* QSideTabWidget::currentWidget() const
{
    return stack->currentWidget();
}

QWidget* QSideTabWidget::widget(int index) const
{
    return stack->widget(index);
}

int QSideTabWidget::count() const
{
    return widgets.count();
}

void QSideTabWidget::setCurrentIndex(int index)
{
    if (index >= 0 && index < widgets.size()) {
        stack->setCurrentIndex(index);
        emit currentChanged(index);
        updateTabStyles();
    }
}

void QSideTabWidget::setCurrentWidget(QWidget *widget)
{
    int idx = stack->indexOf(widget);
    if (idx != -1) setCurrentIndex(idx);
}

bool QSideTabWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == sidebar && event->type() == QEvent::MouseButtonPress && isCollapsed) {
        toggleSidebar(true);
        return true;
    }
    if (obj == stack && event->type() == QEvent::MouseButtonPress && !isCollapsed) {
        toggleSidebar(false);
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void QSideTabWidget::expandSidebar()
{
    toggleSidebar(true);
}

void QSideTabWidget::changePage(int index)
{
    setCurrentIndex(index);
    updateTabStyles();
    toggleSidebar(false);
}

void QSideTabWidget::setQStyleSheet(const QString &style)
{
    customStyleSheet = style;
    applyDefaultStyleSheet();
}

void QSideTabWidget::applyDefaultStyleSheet()
{
    QString style = customStyleSheet.isEmpty() ? QString(
        "QWidget { background-color:#2c3e50; }"
        "QPushButton { color:white; text-align:left; padding:6px; background-color:#2c3e50; border:none; }"
        "QPushButton[selected=\"true\"] { background-color:#34495e; font-weight:bold; }"
    ) : customStyleSheet;

    sidebar->setStyleSheet(style);
    expandButton->setStyleSheet(style);

    for (QPushButton *btn : menuButtons) {
        btn->setStyleSheet(style);
    }
}

void QSideTabWidget::updateTabStyles()
{
    for (int i = 0; i < menuButtons.size(); ++i) {
        QPushButton *btn = menuButtons[i];
        btn->setProperty("selected", QVariant(i == currentIndex()));
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
        btn->update();
    }
}

// 新增：展开/收起逻辑封装
void QSideTabWidget::toggleSidebar(bool expand)
{
    if (expand) {
        sidebar->setFixedWidth(200);
        for (QPushButton *btn : menuButtons) btn->show();
        expandButton->hide();
        isCollapsed = false;
    } else {
        sidebar->setFixedWidth(60);
        for (QPushButton *btn : menuButtons) btn->hide();
        expandButton->show();
        isCollapsed = true;
    }
    updateTabStyles();
}
