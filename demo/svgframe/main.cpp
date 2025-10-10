#include <QApplication>
#include "svgframe.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SvgFrame frame;
    frame.show();

    return app.exec();
}
