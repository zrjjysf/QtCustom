#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void toggleSidebar();
    void changePage(int index);

private:
    QWidget *sidebar = nullptr;
    QStackedWidget *stack = nullptr;
    bool isCollapsed = true;

    QList<QPushButton*> menuButtons;
};

#endif // MAINWINDOW_H
