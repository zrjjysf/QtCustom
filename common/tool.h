#ifndef TOOL
#define TOOL
#include <QString>

class QPalette;

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

#endif