#ifndef CCAPPLICATION
#define CCAPPLICATION
#include <QApplication>

class CCApplication : public QApplication {
    Q_OBJECT
public:
    CCApplication(int &argc, char **argv, bool GUIenabled = true);

    static CCApplication *instance();
    void setQSS(QString qss);
signals:
    void freshQSS();
};

class StyleAwareWidgetsInterface
{
    public:
    
};
#endif