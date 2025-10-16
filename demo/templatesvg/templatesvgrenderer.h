#ifndef TEMPLATESVGRENDERER_H
#define TEMPLATESVGRENDERER_H

#include <QPalette>
#include <QString>
#include <QSvgRenderer>
#include <QHash>

/**
 * @brief TemplateSvgRenderer
 *
 * @note 可以添加替换规则，在渲染svg之前对svg文本进行替换，从而达到定制svg的效果
 * @note 具有QSvgRenderer的功能，QSvgRenderer在构造时或者后续调用load()来加载数据，并缓存，后续绘制就拿这个缓存数据
 */
class TemplateSvgRenderer : public QSvgRenderer {
    Q_OBJECT
public:
    explicit TemplateSvgRenderer(QObject *parent = nullptr);

    /**
     * @brief load 从字节数据加载 SVG
     * @param contents 原始 SVG 数据
     * @return 是否加载成功
     */
    bool load(const QByteArray &contents);

    /**
     * @brief load 从文件加载 SVG
     * @param filename SVG 文件路径
     * @return 是否加载成功
     */
    bool load(const QString &filename);


    /**
     * @brief setReplacement 设置替换键值对，没有的话会新增
     * @param key 要替换的字符串
     * @param value 替换后的值
     */
    void setReplacement(const QString &key, const QString &value);

    /**
     * @brief removeReplacement 移除指定的替换键
     * @param key 要移除的键
     */
    void removeReplacement(const QString &key);

    /**
     * @brief clearReplacements 清空所有替换规则
     */
    void clearReplacements();

    /**
     * @brief getReplacement 获取指定键的替换值
     * @param key 键名
     * @return 对应的值，如果不存在则返回空字符串
     */
    QString getReplacement(const QString &key) const;

    /**
     * @brief getAllReplacements 获取所有替换规则
     * @return 替换规则哈希表
     */
    QHash<QString, QString> getAllReplacements() const;

private:
    QByteArray preprocessSvg(const QByteArray &contents) const;
    bool refreshCache();

private:
    QByteArray m_origin_contents;
    QHash<QString, QString> m_replacements; // 变量替换表
};

#endif   // TEMPLATESVGRENDERER_H