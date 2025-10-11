#include <QApplication>
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QDebug>

void hideFormRow(QWidget *widget,QFormLayout *layout = nullptr)
{
    qDebug()<<"parentWidget:"<<widget->parentWidget();
    if (!widget||!widget->parentWidget()) return;


    QLayout *tmplayout = widget->parentWidget()->layout();
    layout = qobject_cast<QFormLayout*>(tmplayout);
    qDebug()<<"layout:"<<layout;

    // 如果传入的是 field
    QWidget *labelWidget = layout->labelForField(widget);
    if (labelWidget) {
        labelWidget->hide();
        widget->hide();
        return;
    }

    // 如果传入的是 label
    for (int row = 0; row < layout->rowCount(); ++row) {
        QLayoutItem *labelItem = layout->itemAt(row, QFormLayout::LabelRole);
        QLayoutItem *fieldItem = layout->itemAt(row, QFormLayout::FieldRole);

        if (labelItem && labelItem->widget() == widget) {
            labelItem->widget()->hide();
            if (fieldItem && fieldItem->widget())
                fieldItem->widget()->hide();
            return;
        }
    }

    // 如果是 addRow(widget) 这种单控件行
    for (int row = 0; row < layout->rowCount(); ++row) {
        QLayoutItem *fieldItem = layout->itemAt(row, QFormLayout::SpanningRole);
        if (fieldItem && fieldItem->widget() == widget) {
            fieldItem->widget()->hide();
            return;
        }
    }
}
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("QFormLayout 示例");

    // 创建表单布局
    QFormLayout *formLayout = new QFormLayout(&window);

    // 添加行：标签 + 控件
    formLayout->addRow("姓名:", new QLineEdit);
    formLayout->addRow("年龄:", new QSpinBox);
    formLayout->addRow("性别:", new QComboBox);

    // 也可以直接传 QLabel
    QLabel *noteLabel = new QLabel("备注:");
    QLabel *noteLabel1 = new QLabel("备注1");
    QTextEdit *noteEdit = new QTextEdit;
    formLayout->addRow(noteLabel, noteLabel1);

    // 添加按钮行（只放一个控件）
    QPushButton *submitBtn = new QPushButton("提交");
    formLayout->addRow(submitBtn);
    hideFormRow(noteLabel1);
    // noteLabel->hide();
    // noteLabel1->hide();
    // formLayout->itemAt(3, QFormLayout::LabelRole)->widget()->hide();
    // formLayout->itemAt(3, QFormLayout::FieldRole)->widget()->hide();

    // 设置布局到窗口
    // window.setLayout(formLayout);
    window.show();

    return app.exec();
}
