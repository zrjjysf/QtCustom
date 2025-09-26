#include "tool.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include <QPalette>
#include <QMetaEnum>
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include <QWidget>
#include <QAbstractScrollArea>
#include <QLayout>

QString getQSS()
{
    QString result;
    QString appDir = QCoreApplication::applicationDirPath();
    QFile qssFile(appDir+"/style.qss"); // 如果你放在资源文件中
    if (qssFile.open(QFile::ReadOnly)) {
        result = qssFile.readAll();
        qssFile.close();
    }
    qDebug().noquote()<<result;
    return result;
}

QString enumToString(QPalette::ColorGroup group) {
    switch (group) {
        case QPalette::Active: return "Active";
        case QPalette::Inactive: return "Inactive";
        case QPalette::Disabled: return "Disabled";
        default: return "UnknownGroup";
    }
}

void dumpPaletteColors(const QPalette& palette)
{
    QString filePath = QDir::currentPath() + "/palette_dump.css";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法写入文件：" << filePath;
        return;
    }

    QTextStream out(&file);

    const QMetaObject &metaObj = QPalette::staticMetaObject;

    // 获取 ColorGroup 枚举
    int groupIndex = metaObj.indexOfEnumerator("ColorGroup");
    QMetaEnum groupEnum = metaObj.enumerator(groupIndex);

    // 获取 ColorRole 枚举
    int roleIndex = metaObj.indexOfEnumerator("ColorRole");
    QMetaEnum roleEnum = metaObj.enumerator(roleIndex);

    out << "*\n";
    out << "{\n";

    for (int g = 0; g < groupEnum.keyCount(); ++g) {
        int groupValue = groupEnum.value(g);
        if (groupValue >= QPalette::NColorGroups) continue;

        out << "/* Color Group: " << groupEnum.key(g) << " */\n";

        for (int r = 0; r < roleEnum.keyCount(); ++r) {
            int roleValue = roleEnum.value(r);
            if (roleValue >= QPalette::NColorRoles || roleValue == QPalette::NoRole) continue;

            QColor color = palette.color(static_cast<QPalette::ColorGroup>(groupValue),
                                          static_cast<QPalette::ColorRole>(roleValue));
            out << QString("    color : %2; /* %1 */\n")
                       .arg(roleEnum.key(r))
                       .arg(color.name());
        }
        out << "\n";
    }
    out << "}\n";

    file.close();
    qDebug() << "颜色信息已写入：" << filePath;
}

void cleanQSS()
{
    qApp->setStyleSheet(QString());

    QStyle *fresh = QStyleFactory::create(QStringLiteral("Fusion"));
    qApp->setStyle(fresh);

    qApp->setPalette(qApp->style()->standardPalette());
    // 遍历所有控件进行重置
    for (QWidget *w : QApplication::allWidgets()) {
        // 清理局部 QSS
        w->setStyleSheet(QString());
        // 调色板恢复为应用标准
        w->setPalette(qApp->style()->standardPalette());
        // 恢复字体为默认
        w->setFont(QFont());

        // 常被 QSS 污染的 viewport 清理
        if (auto sa = qobject_cast<QAbstractScrollArea *>(w)) {
            if (QWidget *vp = sa->viewport()) {
                vp->setStyleSheet(QString());
                vp->setPalette(qApp->style()->standardPalette());
                vp->setFont(QFont());
                // vp->setAttribute(Qt::WA_StyledBackground, false);
                // vp->setAutoFillBackground(false);

                qApp->style()->unpolish(vp);
                qApp->style()->polish(vp);
                vp->update();
            }
        }

        // 重新应用 style
        qApp->style()->unpolish(w);
        qApp->style()->polish(w);
        w->update();
    }
}

void cleanlayoutDefaultMargins(QLayout* layout)
{
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
}