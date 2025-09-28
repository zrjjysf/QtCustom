#include "iconlabel.h"
#include <QLabel>
#include <QHBoxLayout>
#include "themesvgwgt.h"
#include <QDebug>


IconLabel::IconLabel(QWidget *parent):m_label(new QLabel),m_icon(new ThemeSvgWgt)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_icon);
    layout->addWidget(m_label);
}

void IconLabel::setText(const QString &text)
{
    m_label->setText(text);
}

void IconLabel::resizeEvent(QResizeEvent *event)
{
    qDebug()<<m_icon->size();
    qDebug()<<m_label->size();
    QFrame::resizeEvent(event);
}