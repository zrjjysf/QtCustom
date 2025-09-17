#include "customscrollbar.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOptionSlider>
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <QTimer>
#include "tool.h"
#include <QString>

constexpr const char* turn_top = R"(<svg width="64" height="64" viewBox="0 0 64 64" xmlns="http://www.w3.org/2000/svg">
  <!-- 箭杆 -->
  <line x1="32" y1="20" x2="32" y2="56" stroke="black" stroke-width="3"/>
  <!-- 箭头 -->
  <polyline points="20,32 32,20 44,32" fill="none" stroke="black" stroke-width="3"/>
  <!-- 上方横线 -->
  <line x1="16" y1="16" x2="48" y2="16" stroke="black" stroke-width="3"/>
</svg>
)";

constexpr const char* turn_bottom = R"(<svg width="64" height="64" viewBox="0 0 64 64" xmlns="http://www.w3.org/2000/svg">
  <!-- 箭杆 -->
  <line x1="32" y1="44" x2="32" y2="8" stroke="black" stroke-width="3"/>
  <!-- 箭头 -->
  <polyline points="20,32 32,44 44,32" fill="none" stroke="black" stroke-width="3"/>
  <!-- 下方横线 -->
  <line x1="16" y1="48" x2="48" y2="48" stroke="black" stroke-width="3"/>
</svg>
)";

constexpr const char* turn_up = R"(<svg width="64" height="64" viewBox="0 0 64 64" xmlns="http://www.w3.org/2000/svg">
  <polygon points="12,52 32,12 52,52" fill="black"/>
</svg>
)";
constexpr const char* turn_down = R"(<svg width="64" height="64" viewBox="0 0 64 64" xmlns="http://www.w3.org/2000/svg">
  <polygon points="12,12 52,12 32,52" fill="black"/>
</svg>
)";

CustomScrollBar::CustomScrollBar(QWidget *parent)
    : QScrollBar(Qt::Vertical, parent), 
      m_dragging(false),
      m_topButtonPressed(false),
      m_pageUpButtonPressed(false),
      m_pageDownButtonPressed(false),
      m_bottomButtonPressed(false)
{
    dumpPaletteColors(palette());
}

QRect CustomScrollBar::topButtonRect() const {
    return QRect(0, 0, width(), width() * 2);
}

QRect CustomScrollBar::pageUpButtonRect() const {
    return QRect(0, width() * 2, width(), width() * 2);
}

QRect CustomScrollBar::pageDownButtonRect() const {
    return QRect(0, height() - 4 * width(), width(), width() * 2);
}

QRect CustomScrollBar::bottomButtonRect() const {
    return QRect(0, height() - 2 * width(), width(), width() * 2);
}

// 计算轨道区域（去除了按钮占用的空间）
QRect CustomScrollBar::grooveRect() const {
    int buttonHeight = width() * 2;
    return QRect(0, 2 * buttonHeight, width(), height() - 4 * buttonHeight);
}

// 计算滑块区域
QRect CustomScrollBar::sliderRect() const {
    if (maximum() == minimum() || pageStep() == 0) 
        return QRect();
    
    QRect groove = grooveRect();
    
    // 计算滑块高度
    double pageRatio = static_cast<double>(pageStep()) / (maximum() - minimum() + pageStep());
    int sliderHeight = qMax(10, static_cast<int>(groove.height() * pageRatio));
    
    // 计算滑块位置
    double valueRange = maximum() - minimum();
    if (valueRange == 0) return QRect();
    
    double sliderPos = groove.top() + (groove.height() - sliderHeight) * 
                      (value() - minimum()) / valueRange;
    
    return QRect(0, static_cast<int>(sliderPos), width(), sliderHeight);
}

void CustomScrollBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
    // 1. 绘制背景
    p.fillRect(rect(), palette().window());
    
    // 2. 绘制轨道
    QRect groove = grooveRect();
    p.fillRect(groove, palette().base());
    p.setPen(palette().mid().color());
    p.drawRect(groove.adjusted(0, 0, -1, -1));
    
    // 3. 绘制滑块
    QRect slider = sliderRect();
    if (slider.isValid()) {
        // 绘制3D效果的滑块
        QLinearGradient gradient(slider.topLeft(), slider.bottomRight());
        gradient.setColorAt(0, palette().light().color());
        gradient.setColorAt(1, palette().dark().color());
        p.fillRect(slider, gradient);
        
        // 绘制边框
        p.setPen(palette().shadow().color());
        p.drawRect(slider.adjusted(0, 0, -1, -1));
    }
    
    // 4. 绘制按钮 - 使用3D效果
    draw3DButton(p, topButtonRect(), turn_top, m_topButtonPressed);
    draw3DButton(p, pageUpButtonRect(), turn_up, m_pageUpButtonPressed);
    draw3DButton(p, pageDownButtonRect(), turn_down, m_pageDownButtonPressed);
    draw3DButton(p, bottomButtonRect(), turn_bottom, m_bottomButtonPressed);
}

void CustomScrollBar::draw3DButton(QPainter &p, const QRect &rect, const QString &svgString, bool pressed)
{
    // 绘制3D效果的按钮
    QColor buttonColor = palette().button().color();
    
    // 根据按下状态调整颜色
    if (pressed) {
        buttonColor = buttonColor.darker(120); // 按下时变暗
    }
    
    // 绘制按钮主体
    p.fillRect(rect, buttonColor);
    
    // 绘制3D边框效果
    if (pressed) {
        // 按下状态 - 内凹效果
        p.setPen(palette().dark().color());
        p.drawLine(rect.topLeft(), rect.topRight());
        p.drawLine(rect.topLeft(), rect.bottomLeft());
        
        p.setPen(palette().light().color());
        p.drawLine(rect.topRight(), rect.bottomRight());
        p.drawLine(rect.bottomLeft(), rect.bottomRight());
    } else {
        // 正常状态 - 凸起效果
        p.setPen(palette().light().color());
        p.drawLine(rect.topLeft(), rect.topRight());
        p.drawLine(rect.topLeft(), rect.bottomLeft());
        
        p.setPen(palette().dark().color());
        p.drawLine(rect.topRight(), rect.bottomRight());
        p.drawLine(rect.bottomLeft(), rect.bottomRight());
    }
    
    // 绘制文本
    p.setPen(palette().buttonText().color());
    // p.drawText(rect, Qt::AlignCenter, text);
    // QByteArray::fromStdString(svgString.toStdString());
    QSvgRenderer svg(QByteArray::fromStdString(svgString.toStdString())); // svgPath 可以是 ":/icons/up_arrow.svg"
    if (svg.isValid()) {
        // QRectF targetRect = rect.adjusted(4, 4, -4, -4); // 留点边距
        svg.render(&p, rect);
    }
}

void CustomScrollBar::mousePressEvent(QMouseEvent *event)
{
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
        // 点击了滑块，开始拖动
        m_dragging = true;
        m_dragStartPos = event->pos();
        m_dragStartValue = value();
        return;
    }
    
    if (groove.contains(event->pos())) {
        // 点击了轨道，跳转到相应位置
        int newValue = minimum() + (maximum() - minimum()) * 
                      (event->pos().y() - groove.top() - slider.height() / 2) / 
                      (groove.height() - slider.height());
        setValue(qBound(minimum(), newValue, maximum()));
        return;
    }
    
    QScrollBar::mousePressEvent(event);
}

void CustomScrollBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        QRect groove = grooveRect();
        int delta = event->pos().y() - m_dragStartPos.y();
        int valueRange = maximum() - minimum();
        int grooveHeight = groove.height() - sliderRect().height();
        
        if (grooveHeight > 0) {
            int newValue = m_dragStartValue + valueRange * delta / grooveHeight;
            setValue(qBound(minimum(), newValue, maximum()));
        }
        return;
    }
    
    QScrollBar::mouseMoveEvent(event);
}

void CustomScrollBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging) {
        m_dragging = false;
        return;
    }
    
    QScrollBar::mouseReleaseEvent(event);
}