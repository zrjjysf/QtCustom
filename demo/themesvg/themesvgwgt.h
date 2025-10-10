#ifndef THEMESVGWGT_H
#define THEMESVGWGT_H

#include <QString>
#include <QSvgWidget>

class ThemedSvgRenderer;
class ThemeSvgWgt : public QSvgWidget {
    Q_OBJECT
    Q_PROPERTY(QString svgPath READ svgPath WRITE setSvgPath)
public:
    explicit ThemeSvgWgt(QWidget *parent = nullptr);
    explicit ThemeSvgWgt(const QString &file, QWidget *parent = nullptr);

    QString svgPath() const
    {
        return m_path;
    }
    void setSvgPath(const QString &path);

    // 用当前 palette 重新预处理并加载
    void reload();
protected:
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *e) override;

private:
    QString m_path;
    ThemedSvgRenderer *m_renderer;
};

#endif   // THEMESVGWGT_H
