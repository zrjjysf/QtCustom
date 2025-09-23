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
        int parentWidth = rect.width();
        int parentHeight = rect.height();

        int targetWidth = parentWidth;
        int targetHeight = int(targetWidth / m_ratio);

        if (targetHeight > parentHeight) {
            targetHeight = parentHeight;
            targetWidth = int(targetHeight * m_ratio);
        }

        QRect targetRect(
            rect.x() + (parentWidth - targetWidth) / 2,
            rect.y() + (parentHeight - targetHeight) / 2,
            targetWidth,
            targetHeight
        );

        for (int i = 0; i < count(); ++i) {
            if (auto *item = itemAt(i)) {
                item->setGeometry(targetRect);
            }
        }
    }
}

QSize AspectRatioLayout::sizeHint() const {
    if (m_items.isEmpty()) {
        return QSize(160, 90);  // 默认16:9
    }
    return m_items.first()->sizeHint();
}

QSize AspectRatioLayout::minimumSize() const {
    if (m_items.isEmpty()) {
        return QSize(16, 9);
    }
    return m_items.first()->minimumSize();
}

Qt::Orientations AspectRatioLayout::expandingDirections() const {
    return Qt::Horizontal | Qt::Vertical;
}
