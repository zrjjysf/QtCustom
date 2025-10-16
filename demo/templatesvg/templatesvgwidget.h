#ifndef TEMPLATESVGWIDGET_H
#define TEMPLATESVGWIDGET_H

#include <QString>
#include <QSvgWidget>

class TemplateSvgRenderer;
class TemplateSvgWidget : public QSvgWidget {
    Q_OBJECT
    Q_PROPERTY(QString svgPath READ svgPath WRITE setSvgPath)
public:
    explicit TemplateSvgWidget(QWidget *parent = nullptr);
    explicit TemplateSvgWidget(const QString &file, QWidget *parent = nullptr);
    explicit TemplateSvgWidget(TemplateSvgRenderer* renderer, bool rendererOwned = false, QWidget *parent = nullptr);
    ~TemplateSvgWidget();  // 添加析构函数

    QString svgPath() const { return m_path; }
    void setSvgPath(const QString &path);

    void reload();

    TemplateSvgRenderer* renderer();

    /**
     * @brief Set the Renderer object
     * 
     * @param renderer 
     * @param rendererOwned renderer是否交由本类对象管理生命周期，在设置新renderer时会析构旧的
     */
    void setRenderer(TemplateSvgRenderer* renderer,bool rendererOwned = false);
    
    // /**
    //  * @brief setReplacement 设置替换键值对，没有的话会新增
    //  * @param key 要替换的字符串
    //  * @param value 替换后的值
    //  */
    // void setReplacement(const QString &key, const QString &value);

    // /**
    //  * @brief removeReplacement 移除指定的替换键
    //  * @param key 要移除的键
    //  */
    // void removeReplacement(const QString &key);

protected:
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *e) override;

private:
    QString m_path;
    TemplateSvgRenderer *m_renderer;
    bool m_rendererOwned;  // 标记渲染器是否由本对象拥有
};

#endif   // TEMPLATESVGWIDGET_H