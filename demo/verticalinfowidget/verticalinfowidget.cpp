#include "verticalinfowidget.h"
#include <QProxyStyle>
#include <QPainter>
#include <QVBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QDebug>
#include "ccapplication.h"
#include "justifylabelstyle.h"
#include "ui_verticalinfowidget.h"


VerticalInfoWidget::VerticalInfoWidget(QWidget *parent)
    : QFrame(parent),
      m_topLabel(new QLabel),
      m_bottomLabel(new QLabel)
{
    wrapStyleProxy();
    connect(CCApplication::instance(), &CCApplication::freshQSS,this,&VerticalInfoWidget::wrapStyleProxy);
    m_topLabel->setObjectName("topLabel");
    m_bottomLabel->setObjectName("bottomLabel");
    QVBoxLayout *m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_topLabel);
    m_layout->addWidget(m_bottomLabel);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
}

void VerticalInfoWidget::setTopText(const QString &text)
{
    m_topLabel->setText(text);
}

void VerticalInfoWidget::setBottomText(const QString &text)
{
    m_bottomLabel->setText(text);
}

void VerticalInfoWidget::wrapStyleProxy()
{
    if (qobject_cast<JustifyLabelStyle*>(this->style())) {
    } else {
        QStyle *proxy = new JustifyLabelStyle(this->style());
        this->setStyle(proxy);
    }
}