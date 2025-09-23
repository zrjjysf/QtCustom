#include <QApplication>
#include <QFile>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QUiLoader>
#include <QDebug>

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        reloadButton = new QPushButton("Reload UI", this);
        layout->addWidget(reloadButton);

        uiContainer = new QWidget(this);
        layout->addWidget(uiContainer);

        connect(reloadButton, &QPushButton::clicked, this, &MainWindow::reloadUi);

        reloadUi(); // 初始加载
    }

public slots:
    void reloadUi() {
        QFile file("mainwindow.ui"); // 从磁盘读取
        if (!file.open(QFile::ReadOnly)) {
            qWarning() << "Failed to open UI file";
            return;
        }

        QUiLoader loader;
        QWidget *newUi = loader.load(&file, this);
        file.close();

        if (!newUi) {
            qWarning() << "Failed to load UI:" << loader.errorString();
            return;
        }

        // 删除旧的 UI
        delete uiContainer->layout();
        QVBoxLayout *containerLayout = new QVBoxLayout(uiContainer);
        containerLayout->addWidget(newUi);
    }

private:
    QPushButton *reloadButton;
    QWidget *uiContainer;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
