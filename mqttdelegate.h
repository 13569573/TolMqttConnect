#ifndef MQTTDELEGATE_H
#define MQTTDELEGATE_H

#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

class MqttDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit MqttDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // MQTTDELEGATE_H
