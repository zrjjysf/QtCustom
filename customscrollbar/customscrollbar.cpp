#include "customscrollbar.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOptionSlider>
#include <QStyle>
#include "tool.h"

CustomScrollBar::CustomScrollBar(QWidget *parent)
    : QScrollBar(Qt::Vertical, parent), m_dragging(false)
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
    
    // 1. 绘制背景
    p.fillRect(rect(), palette().window());
    
    // 2. 使用QStyle绘制轨道
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    
    // 设置轨道的矩形区域
    QRect groove = grooveRect();
    opt.rect = groove;
    opt.subControls = QStyle::SC_ScrollBarGroove;
    
    // 绘制轨道
    style()->drawComplexControl(QStyle::CC_ScrollBar, &opt, &p, this);
    
    // 3. 使用QStyle绘制滑块
    if (maximum() != minimum()) {
        // 创建一个新的选项用于滑块
        QStyleOptionSlider sliderOpt;
        initStyleOption(&sliderOpt);
        
        // 设置滑块的矩形区域
        QRect slider = sliderRect();
        sliderOpt.rect = slider;
        sliderOpt.subControls = QStyle::SC_ScrollBarSlider;
        
        // 设置滑块状态
        if (m_dragging) {
            sliderOpt.state |= QStyle::State_Sunken;
        }
        
        // 绘制滑块
        style()->drawComplexControl(QStyle::CC_ScrollBar, &sliderOpt, &p, this);
    }
    
    // 4. 绘制按钮
    p.setPen(Qt::black);
    p.setBrush(palette().button());
    
    QRect buttons[] = {
        topButtonRect(), 
        pageUpButtonRect(), 
        pageDownButtonRect(), 
        bottomButtonRect()
    };
    
    for (const QRect &rect : buttons) {
        p.fillRect(rect, palette().button());
        p.drawRect(rect);
    }
    
    // 5. 绘制按钮符号
    p.drawText(buttons[0], Qt::AlignCenter, "⤒");
    p.drawText(buttons[1], Qt::AlignCenter, "▲");
    p.drawText(buttons[2], Qt::AlignCenter, "▼");
    p.drawText(buttons[3], Qt::AlignCenter, "⤓");
}

void CustomScrollBar::mousePressEvent(QMouseEvent *event)
{
    if (topButtonRect().contains(event->pos())) {
        setValue(minimum());
        return;
    }
    if (pageUpButtonRect().contains(event->pos())) {
        triggerAction(QAbstractSlider::SliderPageStepSub);
        return;
    }
    if (pageDownButtonRect().contains(event->pos())) {
        triggerAction(QAbstractSlider::SliderPageStepAdd);
        return;
    }
    if (bottomButtonRect().contains(event->pos())) {
        setValue(maximum());
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
        update(); // 重绘以更新滑块状态
        return;
    }
    
    QScrollBar::mouseReleaseEvent(event);
}