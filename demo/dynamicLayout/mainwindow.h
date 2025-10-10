#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

class ColorArea : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorArea(const QString &color, const QString &text, QWidget *parent = nullptr);
};

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onAreaClicked(int index);

private:
    QList<ColorArea*> areas;
    QHBoxLayout *mainLayout;
    int currentIndex = -1;
    QList<int> targetWidths;

    void animateTo(int index);
};

#endif // MAINWINDOW_H
