#include "aspectratiolayout.h"

AspectRatioLayout::AspectRatioLayout(double ratio, QWidget *parent)
    : QLayout(parent), m_ratio(ratio) {}

AspectRatioLayout::~AspectRatioLayout() {
    while (!m_items.isEmpty()) {
        delete m_items.takeFirst();
    }
}

void AspectRatioLayout::addItem(QLayoutItem *item) {
    m_items.append(item);
}

QLayoutItem *AspectRatioLayout::itemAt(int index) const {
    return m_items.value(index);
}

QLayoutItem *AspectRatioLayout::takeAt(int index) {
    if (index >= 0 && index < m_items.size()) {
        return m_items.takeAt(index);
    }
    return nullptr;
}

int AspectRatioLayout::count() const {
    return m_items.size();
}

void AspectRatioLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);

    if (count() > 0) {
        // 获取父控件的宽高
        int parentWidth = rect.width();
        int parentHeight = rect.height();

        // 根据宽高比计算出目标宽度和高度
        int targetWidth = parentWidth;
        int targetHeight = int(targetWidth / m_ratio);

        if (targetHeight > parentHeight) {
            targetHeight = parentHeight;
            targetWidth = int(targetHeight * m_ratio);
        }

        // 将目标宽高分配给所有子控件
        for (int i = 0; i < count(); ++i) {
            QLayoutItem *item = itemAt(i);
            if (item) {
                item->setGeometry(QRect(rect.topLeft(), QSize(targetWidth, targetHeight)));
            }
        }
    }
}
