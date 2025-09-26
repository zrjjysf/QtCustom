#include "ccapplication.h"



CCApplication::CCApplication(int &argc, char **argv, bool GUIenabled) : QApplication(argc, argv, GUIenabled)
{

}

CCApplication *CCApplication::instance()
{
    return qobject_cast<CCApplication *>(QCoreApplication::instance());
}

void CCApplication::setQSS(QString qss)
{
    instance()->setStyleSheet(qss);
    emit freshQSS();
}