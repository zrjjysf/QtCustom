#include "templatesvgrenderer.h"
#include <QFile>
#include <QDebug>

TemplateSvgRenderer::TemplateSvgRenderer(QObject *parent) 
    : QSvgRenderer(parent)
{
}

bool TemplateSvgRenderer::load(const QByteArray &contents)
{
    // 保存原始数据
    m_origin_contents = contents;
    
    return refreshCache();
}

bool TemplateSvgRenderer::load(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "TemplateSvgRenderer: Failed to open file:" << filename;
        return false;
    }

    // 读取并保存原始数据
    m_origin_contents = file.readAll();
    file.close();
    
    return refreshCache();
}

void TemplateSvgRenderer::setReplacement(const QString &key, const QString &value)
{
    m_replacements[key] = value;
    
    refreshCache();
}

void TemplateSvgRenderer::removeReplacement(const QString &key)
{
    m_replacements.remove(key);
    
    refreshCache();
}

void TemplateSvgRenderer::clearReplacements()
{
    m_replacements.clear();
    refreshCache();
}

QString TemplateSvgRenderer::getReplacement(const QString &key) const
{
    return m_replacements.value(key, "");
}

QHash<QString, QString> TemplateSvgRenderer::getAllReplacements() const
{
    return m_replacements;
}

QByteArray TemplateSvgRenderer::preprocessSvg(const QByteArray &contents) const
{
    QByteArray modified = contents;

    // 应用所有自定义变量替换
    for (auto it = m_replacements.constBegin(); it != m_replacements.constEnd(); ++it) {
        modified.replace(it.key().toUtf8(), it.value().toUtf8());
    }

    return modified;
}

bool TemplateSvgRenderer::refreshCache()
{
    // 如果有原始数据，预处理并重新加载
    if (!m_origin_contents.isEmpty()) {
        QByteArray modified = preprocessSvg(m_origin_contents);
        return QSvgRenderer::load(modified);
    }
    return false;
}