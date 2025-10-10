#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include "cardframe.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Card Demo");

    QHBoxLayout *hbox = new QHBoxLayout(&window);
    hbox->setSpacing(20);
    hbox->setContentsMargins(20, 20, 20, 20);

    hbox->addWidget(new CardFrame("枪 A"));
    hbox->addWidget(new CardFrame("枪 B"));
    hbox->addWidget(new CardFrame("枪 C"));
    hbox->addWidget(new CardFrame("枪 D"));

    window.show();
    return app.exec();
}
