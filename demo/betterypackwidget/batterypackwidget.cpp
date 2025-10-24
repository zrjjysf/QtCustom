#include "batterypackwidget.h"
#include "templatesvgwidget.h"
#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QStyle>
#include <QMetaEnum>
#include <QMouseEvent>

QSize BatteryPackWidget::staticSize = QSize(100,100);

BatteryPackWidget::BatteryPackWidget(QWidget *parent)
    : QWidget(parent), m_bg(new TemplateSvgWidget) {
  setFixedSize(staticSize);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_bg,1);
  m_bg->setStyleSheet(
      R"(
        TemplateSvgWidget[OpenMode="OPEN"] {
            qproperty-svgPath: url(./package_open.svg);
        }
        TemplateSvgWidget[OpenMode="CLOSE"] {
            qproperty-svgPath: url(./package.svg);
        }
        )");
    setOpenMode(OpenMode::CLOSE);
    setStatus(Status::NORMAL);
}

void BatteryPackWidget::setPackID(int packID) 
{ 
    m_packID = packID;
    m_bg->setReplacement("PACKAGE_ID", "Pack "+QString::number(m_packID));
}

void BatteryPackWidget::setOpenMode(OpenMode mode) 
{ 
    m_openMode = mode; 
    QMetaEnum metaEnum = QMetaEnum::fromType<OpenMode>();
    m_bg->setProperty("OpenMode", metaEnum.valueToKey(m_openMode));
    m_bg->style()->unpolish(m_bg);
    m_bg->style()->polish(m_bg);
    m_bg->update();
}

void BatteryPackWidget::setStatus(Status sta) 
{
    m_status = sta;
    QString currentColor = m_status == ALARM?"red":"#f49c39";
    m_bg->setReplacement("currentColor", currentColor);
}

void BatteryPackWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event); // 保留默认行为（如焦点）

    if(m_openMode == CLOSE)
    {
        setOpenMode(OPEN);
        emit opened(m_packID);
    }
}

void BatteryPackWidget::slotStatusChanged(int status)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Status>();
    if(metaEnum.valueToKey(status) == nullptr)
        return;
    setStatus(static_cast<Status>(status));
}