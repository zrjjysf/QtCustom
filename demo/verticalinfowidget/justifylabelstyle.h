#ifndef JUSTIFYLABELSTYLE_H
#define JUSTIFYLABELSTYLE_H
#include <QProxyStyle>
#include <QDebug>

class JustifyLabelStyle : public QProxyStyle
{
    Q_OBJECT   // 必须加上这个
public:
    explicit JustifyLabelStyle(QStyle *base = nullptr)
        : QProxyStyle(base) {}

    void drawItemText(QPainter *painter, const QRect &rect, int flags,
                      const QPalette &pal, bool enabled, const QString &text,
                      QPalette::ColorRole textRole = QPalette::NoRole) const override
    {
        int newflags = flags;
        if (flags & Qt::AlignJustify) {
            newflags |= Qt::TextJustificationForced;
        }
        QProxyStyle::drawItemText(painter, rect, newflags, pal, enabled, text, textRole);
    }
};


#endif