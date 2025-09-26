#ifndef THEMEDSVGRENDERER_H
#define THEMEDSVGRENDERER_H

#include <QPalette>
#include <QString>
#include <QSvgRenderer>

/**
 * @brief ThemedSvgRenderer
 *
 * @note 一个支持自动颜色占位符替换的 QSvgRenderer 子类。
 * 默认会把 SVG 内的 "currentColor" 替换成调色板中的 WindowText 颜色。
 * 目前仅单色替换后续可扩展支持更多占位符，例如 {{primary}}, {{secondary}}。
 */
class ThemedSvgRenderer : public QSvgRenderer {
    Q_OBJECT
public:
    explicit ThemedSvgRenderer(QObject *parent = nullptr);

    /**
     * @brief load 从字节数据加载 SVG
     * @param contents 原始 SVG 数据
     * @param palette  颜色来源调色板（可选）,不使用调色板中的 WindowText 颜色
     * @return 是否加载成功
     */
    bool load(const QByteArray &contents, const QPalette &palette = QPalette());

    /**
     * @brief load 从文件加载 SVG
     * @param filename SVG 文件路径
     * @param palette  颜色来源调色板（可选）,不使用调色板中的 WindowText 颜色
     * @return 是否加载成功
     */
    bool load(const QString &filename, const QPalette &palette = QPalette());
private:
    QByteArray preprocessSvg(const QByteArray &contents, const QPalette &palette) const;
};

#endif   // THEMEDSVGRENDERER_H
