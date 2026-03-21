#include "mqttdelegate.h"
#include "mqttmodel.h"
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPainter>
#include <QStyleOptionButton>
#include <QStyleOptionViewItem>
#include <QApplication>

MqttDelegate::MqttDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void MqttDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    if (index.column() == 5) {
        painter->save();
        
        QStyleOptionButton buttonOption;
        buttonOption.rect = option.rect.adjusted(2, 2, -2, -2);
        buttonOption.state = QStyle::State_Enabled;
        buttonOption.features = QStyleOptionButton::None;
        
        const MqttModel *model = qobject_cast<const MqttModel *>(index.model());
        if (model && model->isAddRow(index)) {
            buttonOption.text = QStringLiteral("添加一行");
        } else {
            buttonOption.text = QStringLiteral("删除");
        }
        
        QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);
        
        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QWidget *MqttDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    const MqttModel *model = qobject_cast<const MqttModel *>(index.model());
    if (model && model->isAddRow(index)) {
        return nullptr;
    }

    switch (index.column()) {
    case 0: {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setRange(0, 65535);
        return editor;
    }
    case 1: {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"00_X", "1REG", "2IEEE", "3LONG", "4STRING"});
        return editor;
    }
    case 2: {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
    case 3: {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"0只读", "1读写"});
        return editor;
    }
    case 4:
        return nullptr;
    default:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void MqttDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const MqttModel *model = qobject_cast<const MqttModel *>(index.model());
    if (model && model->isAddRow(index)) {
        return;
    }

    switch (index.column()) {
    case 0: {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
        spinBox->setValue(index.data(Qt::EditRole).toInt());
        break;
    }
    case 1: {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        QString text = index.data(Qt::EditRole).toString();
        comboBox->setCurrentText(text);
        break;
    }
    case 2: {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        lineEdit->setText(index.data(Qt::EditRole).toString());
        break;
    }
    case 3: {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        QString text = index.data(Qt::EditRole).toString();
        comboBox->setCurrentText(text);
        break;
    }
    default:
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void MqttDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    const MqttModel *mqttModel = qobject_cast<const MqttModel *>(model);
    if (mqttModel && mqttModel->isAddRow(index)) {
        return;
    }

    switch (index.column()) {
    case 0: {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
        spinBox->interpretText();
        model->setData(index, spinBox->value(), Qt::EditRole);
        break;
    }
    case 1: {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
        break;
    }
    case 2: {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        model->setData(index, lineEdit->text(), Qt::EditRole);
        break;
    }
    case 3: {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
        break;
    }
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void MqttDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}
