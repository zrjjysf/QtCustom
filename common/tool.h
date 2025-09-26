#ifndef TOOL
#define TOOL
#include <QString>
#include <QApplication>

class QPalette;
class QLayout;

void dumpPaletteColors(const QPalette& palette);

/**
 * @brief get qss string from style.qss in applicationDirPath
 * 
 * @return QString 
 */
QString getQSS();

/**
 * @brief clean up current QProxyStyle
 * 
 */
void cleanQSS();


void cleanlayoutDefaultMargins(QLayout* layout);
#endif