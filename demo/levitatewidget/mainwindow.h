#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

class GunIdBadge;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;
    GunIdBadge *badge;
};

#endif // MAINWINDOW_H
