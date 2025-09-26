#include "themedsvgrenderer.h"
#include <QColor>
#include <QFile>

ThemedSvgRenderer::ThemedSvgRenderer(QObject *parent) : QSvgRenderer(parent) {}

bool ThemedSvgRenderer::load(const QByteArray &contents, const QPalette &palette)
{
    QByteArray modified = preprocessSvg(contents, palette);
    return QSvgRenderer::load(modified);
}

bool ThemedSvgRenderer::load(const QString &filename, const QPalette &palette)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QByteArray raw = f.readAll();
    QByteArray modified = preprocessSvg(raw, palette);
    return QSvgRenderer::load(modified);
}

QByteArray ThemedSvgRenderer::preprocessSvg(const QByteArray &contents, const QPalette &palette) const
{
    QByteArray modified = contents;

    // 默认：替换 currentColor 为调色板 WindowText 颜色
    if (!palette.isCopyOf(QPalette())) {
        QColor c = palette.color(QPalette::WindowText);
        modified.replace("currentColor", c.name(QColor::HexRgb).toUtf8());
    }

    // 未来可扩展：更多颜色占位符替换
    // modified.replace("{{primary}}", "#ff0000");
    // modified.replace("{{secondary}}", "#00ff00");

    return modified;
}
