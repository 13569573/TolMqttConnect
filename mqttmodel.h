#ifndef MQTTMODEL_H
#define MQTTMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QString>
#include <QStringList>

struct MqttItem
{
    int registerAddress;
    int registerType;
    QString mqttName;
    int writeType;
    bool enabled;
};

class MqttModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MqttModel(QObject *parent = nullptr);
    static QString registerTypeText(int registerType);
    static QString writeTypeText(int writeType);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    
    bool isAddRow(const QModelIndex &index) const;

    void addItem();
    void removeRow(int row);
    MqttItem getItem(int row) const;
    void setItem(int row, const MqttItem &item);
    QVector<MqttItem> items() const;
    QVector<MqttItem> enabledItems() const;
    bool saveToFile(const QString &filePath);
    bool loadFromFile(const QString &filePath);
    bool importFromExcel(const QString &filePath);

signals:
    void importProgress(int current, int total);

private:
    QVector<MqttItem> m_items;
    static const QStringList s_registerTypes;
    static const QStringList s_writeTypes;
};

#endif // MQTTMODEL_H
