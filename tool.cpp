#include "tool.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include <QPalette>
#include <QMetaEnum>

QString getQSS()
{
    QString result;
    QFile qssFile(":/style.qss"); // 如果你放在资源文件中
    if (qssFile.open(QFile::ReadOnly)) {
        result = qssFile.readAll();
        qssFile.close();
    }
    qDebug()<<result;
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
