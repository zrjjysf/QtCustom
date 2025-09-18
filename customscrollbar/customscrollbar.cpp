#include "customscrollbar.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QString>
#include <QStyleOptionSlider>
#include <QSvgRenderer>
#include <QTimer>
#include <QTransform>

constexpr int kMinSliderThickness = 10;

CustomScrollBar::CustomScrollBar(Qt::Orientation orientation, QWidget *parent)
    : QScrollBar(orientation, parent), m_dragging(false),
      m_topButtonPressed(false), m_pageUpButtonPressed(false),
      m_pageDownButtonPressed(false), m_bottomButtonPressed(false),
      m_turnTopRenderer(new QSvgRenderer(QByteArray(
          R"(<svg width="64" height="64" viewBox="0 0 64 64" xmlns="http://www.w3.org/2000/svg">
  <line x1="32" y1="20" x2="32" y2="56" stroke="black" stroke-width="3"/>
  <polyline points="20,32 32,20 44,32" fill="none" stroke="black" stroke-width="3"/>
  <line x1="16" y1="16" x2="48" y2="16" stroke="black" stroke-width="3"/>
</svg>)"))),
      m_turnUpRenderer(new QSvgRenderer(QByteArray(
          R"(<svg width="64" height="64" viewBox="0 0 64 64" xmlns="http://www.w3.org/2000/svg">
  <polygon points="12,52 32,12 52,52" fill="black"/>
</svg>)"))),
      m_turnDownRenderer(new QSvgRenderer(QByteArray(
          R"(<svg width="64" height="64" viewBox="0 0 64 64" xmlns="http://www.w3.org/2000/svg">
  <polygon points="12,12 52,12 32,52" fill="black"/>
</svg>)"))),
      m_turnBottomRenderer(new QSvgRenderer(QByteArray(
          R"(<svg width="64" height="64" viewBox="0 0 64 64" xmlns="http://www.w3.org/2000/svg">
  <line x1="32" y1="44" x2="32" y2="8" stroke="black" stroke-width="3"/>
  <polyline points="20,32 32,44 44,32" fill="none" stroke="black" stroke-width="3"/>
  <line x1="16" y1="48" x2="48" y2="48" stroke="black" stroke-width="3"/>
</svg>)"))) {
}

int CustomScrollBar::indicatorThickness() const {
    return orientation() == Qt::Vertical ? width() * 1.5 : height() * 1.5;
}

QRect CustomScrollBar::topButtonRect() const {
    if (orientation() == Qt::Vertical)
        return QRect(0, 0, width(), indicatorThickness());
    else
        return QRect(0, 0, indicatorThickness(), height());
}

QRect CustomScrollBar::pageUpButtonRect() const {
    if (orientation() == Qt::Vertical)
        return QRect(0, indicatorThickness(), width(), indicatorThickness());
    else
        return QRect(indicatorThickness(), 0, indicatorThickness(), height());
}

QRect CustomScrollBar::pageDownButtonRect() const {
    if (orientation() == Qt::Vertical)
        return QRect(0, height() - indicatorThickness() * 2, width(), indicatorThickness());
    else
        return QRect(width() - indicatorThickness() * 2, 0, indicatorThickness(), height());
}

QRect CustomScrollBar::bottomButtonRect() const {
    if (orientation() == Qt::Vertical)
        return QRect(0, height() - indicatorThickness(), width(), indicatorThickness());
    else
        return QRect(width() - indicatorThickness(), 0, indicatorThickness(), height());
}

QRect CustomScrollBar::grooveRect() const {
    if (orientation() == Qt::Vertical)
        return QRect(0, 2 * indicatorThickness(), width(), height() - 4 * indicatorThickness());
    else
        return QRect(2 * indicatorThickness(), 0, width() - 4 * indicatorThickness(), height());
}

QRect CustomScrollBar::sliderRect() const {
    if (maximum() == minimum() || pageStep() == 0)
        return QRect();

    QRect groove = grooveRect();

    double pageRatio = static_cast<double>(pageStep()) / (maximum() - minimum() + pageStep());
    int sliderSize = qMax(kMinSliderThickness,
                          static_cast<int>((orientation() == Qt::Vertical ? groove.height() : groove.width()) * pageRatio));

    double valueRange = maximum() - minimum();
    if (valueRange == 0)
        return QRect();

    if (orientation() == Qt::Vertical) {
        double sliderPos = groove.top() + (groove.height() - sliderSize) *
                           (value() - minimum()) / valueRange;
        return QRect(0, static_cast<int>(sliderPos), width(), sliderSize);
    } else {
        double sliderPos = groove.left() + (groove.width() - sliderSize) *
                           (value() - minimum()) / valueRange;
        return QRect(static_cast<int>(sliderPos), 0, sliderSize, height());
    }
}

void CustomScrollBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.fillRect(rect(), palette().window());

    QRect groove = grooveRect();
    p.fillRect(groove, palette().base());
    p.setPen(palette().mid().color());
    p.drawRect(groove.adjusted(0, 0, -1, -1));

    QRect slider = sliderRect();
    if (slider.isValid()) {
        QLinearGradient gradient(slider.topLeft(), slider.bottomRight());
        gradient.setColorAt(0, palette().light().color());
        gradient.setColorAt(1, palette().dark().color());
        p.fillRect(slider, gradient);

        p.setPen(palette().shadow().color());
        p.drawRect(slider.adjusted(0, 0, -1, -1));
    }

    draw3DButton(p, topButtonRect(), m_turnTopRenderer, m_topButtonPressed);
    draw3DButton(p, pageUpButtonRect(), m_turnUpRenderer, m_pageUpButtonPressed);
    draw3DButton(p, pageDownButtonRect(), m_turnDownRenderer, m_pageDownButtonPressed);
    draw3DButton(p, bottomButtonRect(), m_turnBottomRenderer, m_bottomButtonPressed);
}

void CustomScrollBar::draw3DButton(QPainter &p, const QRect &rect, QSvgRenderer *renderer, bool pressed) {
    QColor buttonColor = palette().button().color();
    if (pressed) buttonColor = buttonColor.darker(120);
    p.fillRect(rect, buttonColor);

    // 绘制3D边框
    if (pressed) {
        p.setPen(palette().dark().color());
        p.drawLine(rect.topLeft(), rect.topRight());
        p.drawLine(rect.topLeft(), rect.bottomLeft());
        p.setPen(palette().light().color());
        p.drawLine(rect.topRight(), rect.bottomRight());
        p.drawLine(rect.bottomLeft(), rect.bottomRight());
    } else {
        p.setPen(palette().light().color());
        p.drawLine(rect.topLeft(), rect.topRight());
        p.drawLine(rect.topLeft(), rect.bottomLeft());
        p.setPen(palette().dark().color());
        p.drawLine(rect.topRight(), rect.bottomRight());
        p.drawLine(rect.bottomLeft(), rect.bottomRight());
    }

    if (!renderer) return;

    QRectF viewBox = renderer->viewBoxF();
    if (viewBox.isEmpty()) return;

    // 计算缩放因子（保持长宽比）
    qreal scale = qMin(rect.width() / viewBox.width(), rect.height() / viewBox.height());

    QSizeF scaledSize = viewBox.size() * scale;

    // 计算居中位置
    QPointF topLeft(
        rect.center().x() - scaledSize.width() / 2.0,
        rect.center().y() - scaledSize.height() / 2.0
    );
    QRectF targetRect(topLeft, scaledSize);

    p.save();
    if (orientation() == Qt::Horizontal) {
        // 横向按钮图标旋转90度
        QTransform transform;
        transform.translate(rect.center().x(), rect.center().y());
        transform.rotate(-90);
        transform.translate(-rect.center().x(), -rect.center().y());
        p.setTransform(transform, true);
    }
    renderer->render(&p, targetRect);
    p.restore();
}


void CustomScrollBar::draw3DButton(QPainter &p, const QRect &rect, const QString &text, bool pressed) {
    QColor buttonColor = palette().button().color();
    if (pressed) buttonColor = buttonColor.darker(120);
    p.fillRect(rect, buttonColor);

    if (pressed) {
        p.setPen(palette().dark().color());
        p.drawLine(rect.topLeft(), rect.topRight());
        p.drawLine(rect.topLeft(), rect.bottomLeft());
        p.setPen(palette().light().color());
        p.drawLine(rect.topRight(), rect.bottomRight());
        p.drawLine(rect.bottomLeft(), rect.bottomRight());
    } else {
        p.setPen(palette().light().color());
        p.drawLine(rect.topLeft(), rect.topRight());
        p.drawLine(rect.topLeft(), rect.bottomLeft());
        p.setPen(palette().dark().color());
        p.drawLine(rect.topRight(), rect.bottomRight());
        p.drawLine(rect.bottomLeft(), rect.bottomRight());
    }

    p.setPen(palette().buttonText().color());
    p.drawText(rect, Qt::AlignCenter, text);
}

void CustomScrollBar::mousePressEvent(QMouseEvent *event) {
    if (topButtonRect().contains(event->pos())) {
        m_topButtonPressed = true;
        update(topButtonRect());
        QTimer::singleShot(150, this, [this]() {
            m_topButtonPressed = false;
            update(topButtonRect());
            setValue(minimum());
        });
        return;
    }
    if (pageUpButtonRect().contains(event->pos())) {
        m_pageUpButtonPressed = true;
        update(pageUpButtonRect());
        QTimer::singleShot(150, this, [this]() {
            m_pageUpButtonPressed = false;
            update(pageUpButtonRect());
            triggerAction(QAbstractSlider::SliderPageStepSub);
        });
        return;
    }
    if (pageDownButtonRect().contains(event->pos())) {
        m_pageDownButtonPressed = true;
        update(pageDownButtonRect());
        QTimer::singleShot(150, this, [this]() {
            m_pageDownButtonPressed = false;
            update(pageDownButtonRect());
            triggerAction(QAbstractSlider::SliderPageStepAdd);
        });
        return;
    }
    if (bottomButtonRect().contains(event->pos())) {
        m_bottomButtonPressed = true;
        update(bottomButtonRect());
        QTimer::singleShot(150, this, [this]() {
            m_bottomButtonPressed = false;
            update(bottomButtonRect());
            setValue(maximum());
        });
        return;
    }

    QRect slider = sliderRect();
    QRect groove = grooveRect();

    if (slider.contains(event->pos())) {
        m_dragging = true;
        m_dragStartPos = event->pos();
        m_dragStartValue = value();
        return;
    }

    if (groove.contains(event->pos())) {
        if ((orientation() == Qt::Vertical ? groove.height() - slider.height() : groove.width() - slider.width()) <= 0) return;

        int newValue;
        if (orientation() == Qt::Vertical) {
            newValue = minimum() +
                       (maximum() - minimum()) *
                           (event->pos().y() - groove.top() - slider.height() / 2) /
                           (groove.height() - slider.height());
        } else {
            newValue = minimum() +
                       (maximum() - minimum()) *
                           (event->pos().x() - groove.left() - slider.width() / 2) /
                           (groove.width() - slider.width());
        }
        setValue(qBound(minimum(), newValue, maximum()));
        return;
    }

    QScrollBar::mousePressEvent(event);
}

void CustomScrollBar::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragging) {
        QRect groove = grooveRect();
        int delta = orientation() == Qt::Vertical ? event->pos().y() - m_dragStartPos.y()
                                                  : event->pos().x() - m_dragStartPos.x();
        int valueRange = maximum() - minimum();
        int grooveSize = (orientation() == Qt::Vertical ? groove.height() : groove.width()) - (orientation() == Qt::Vertical ? sliderRect().height() : sliderRect().width());

        if (grooveSize > 0) {
            int newValue = m_dragStartValue + valueRange * delta / grooveSize;
            setValue(qBound(minimum(), newValue, maximum()));
        }
        return;
    }

    QScrollBar::mouseMoveEvent(event);
}

void CustomScrollBar::mouseReleaseEvent(QMouseEvent *event) {
    if (m_dragging) {
        m_dragging = false;
        return;
    }

    QScrollBar::mouseReleaseEvent(event);
}
