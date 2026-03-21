#include "mqttmodel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAxObject>
#include <QAxWidget>
#include <QVariantList>
#include <QCoreApplication>

const QStringList MqttModel::s_registerTypes = {"00_X", "1REG", "2IEEE", "3LONG", "4STRING"};
const QStringList MqttModel::s_writeTypes = {"0只读", "1读写"};

MqttModel::MqttModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int MqttModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.count() + 1;
}

int MqttModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 6;
}

bool MqttModel::isAddRow(const QModelIndex &index) const
{
    return index.isValid() && index.row() == m_items.count();
}

QVariant MqttModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (isAddRow(index)) {
        return QVariant();
    }

    if (index.row() >= m_items.count())
        return QVariant();

    const MqttItem &item = m_items.at(index.row());

    if (index.column() == 4) {
        if (role == Qt::CheckStateRole) {
            return item.enabled ? Qt::Checked : Qt::Unchecked;
        }
        if (role == Qt::DisplayRole) {
            return QVariant();
        }
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return item.registerAddress;
        case 1: return s_registerTypes.value(item.registerType, "00_X");
        case 2: return item.mqttName;
        case 3: return s_writeTypes.value(item.writeType, "0只读");
        }
    }

    return QVariant();
}

QVariant MqttModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return QStringLiteral("寄存器地址");
        case 1: return QStringLiteral("寄存器类型");
        case 2: return QStringLiteral("MQTT名称");
        case 3: return QStringLiteral("读写类型");
        case 4: return QStringLiteral("是否启用");
        case 5: return QStringLiteral("操作");
        }
    }
    return QVariant();
}

Qt::ItemFlags MqttModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (isAddRow(index)) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() < 5) {
        flags |= Qt::ItemIsEditable;
        if (index.column() == 4) {
            flags |= Qt::ItemIsUserCheckable;
        }
    }

    return flags;
}

bool MqttModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || isAddRow(index) || index.row() >= m_items.count())
        return false;

    if (index.column() == 4 && role == Qt::CheckStateRole) {
        m_items[index.row()].enabled = (value.toInt() == Qt::Checked);
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole, Qt::CheckStateRole});
        return true;
    }

    if (role != Qt::EditRole)
        return false;

    switch (index.column()) {
    case 0:
        m_items[index.row()].registerAddress = value.toInt();
        break;
    case 1: {
        QString strValue = value.toString();
        int typeIndex = s_registerTypes.indexOf(strValue);
        if (typeIndex >= 0) {
            m_items[index.row()].registerType = typeIndex;
        }
        break;
    }
    case 2:
        m_items[index.row()].mqttName = value.toString();
        break;
    case 3: {
        QString strValue = value.toString();
        int typeIndex = s_writeTypes.indexOf(strValue);
        if (typeIndex >= 0) {
            m_items[index.row()].writeType = typeIndex;
        }
        break;
    }
    case 4:
        m_items[index.row()].enabled = value.toBool();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
}

void MqttModel::addItem()
{
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    MqttItem item;
    item.registerAddress = 0;
    item.registerType = 0;
    item.mqttName = QStringLiteral("");
    item.writeType = 0;
    item.enabled = true;
    m_items.append(item);
    endInsertRows();
}

void MqttModel::removeRow(int row)
{
    if (row < 0 || row >= m_items.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

MqttItem MqttModel::getItem(int row) const
{
    if (row < 0 || row >= m_items.count())
        return MqttItem();
    return m_items.at(row);
}

void MqttModel::setItem(int row, const MqttItem &item)
{
    if (row < 0 || row >= m_items.count())
        return;
    m_items[row] = item;
}

bool MqttModel::loadFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray())
        return false;

    QJsonArray array = doc.array();
    beginResetModel();
    m_items.clear();

    for (const auto &item : array) {
        if (!item.isObject())
            continue;

        QJsonObject obj = item.toObject();
        MqttItem newItem;
        newItem.registerAddress = obj.value(QStringLiteral("registerAddress")).toInt(0);
        newItem.registerType = obj.value(QStringLiteral("registerType")).toInt(0);
        newItem.mqttName = obj.value(QStringLiteral("mqttName")).toString();
        newItem.writeType = obj.value(QStringLiteral("writeType")).toInt(0);
        newItem.enabled = obj.value(QStringLiteral("enabled")).toBool(true);
        m_items.append(newItem);
    }

    endResetModel();
    return true;
}

bool MqttModel::saveToFile(const QString &filePath)
{
    QJsonArray array;

    for (const auto &item : m_items) {
        QJsonObject obj;
        obj[QStringLiteral("registerAddress")] = item.registerAddress;
        obj[QStringLiteral("registerType")] = item.registerType;
        obj[QStringLiteral("mqttName")] = item.mqttName;
        obj[QStringLiteral("writeType")] = item.writeType;
        obj[QStringLiteral("enabled")] = item.enabled;
        array.append(obj);
    }

    QJsonDocument doc;
    doc.setArray(array);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool MqttModel::importFromExcel(const QString &filePath)
{
    emit importProgress(1, 100);
    QCoreApplication::processEvents();

    QAxObject excel("Excel.Application");
    if (excel.isNull())
        return false;

    emit importProgress(10, 100);
    QCoreApplication::processEvents();

    excel.setProperty("Visible", false);
    QAxObject *workbooks = excel.querySubObject("Workbooks");
    QAxObject *workbook = workbooks->querySubObject("Open(const QString&)", filePath);
    if (!workbook) {
        excel.dynamicCall("Quit()");
        return false;
    }

    emit importProgress(30, 100);
    QCoreApplication::processEvents();

    QAxObject *sheets = workbook->querySubObject("Worksheets");
    QAxObject *sheet = sheets->querySubObject("Item(int)", 1);
    if (!sheet) {
        workbook->dynamicCall("Close()");
        excel.dynamicCall("Quit()");
        return false;
    }

    QAxObject *usedRange = sheet->querySubObject("UsedRange");
    QVariant varValues = usedRange->dynamicCall("Value");
    QVariantList rowList = varValues.toList();
    if (rowList.isEmpty()) {
        workbook->dynamicCall("Close()");
        excel.dynamicCall("Quit()");
        return false;
    }

    emit importProgress(60, 100);
    QCoreApplication::processEvents();

    beginResetModel();
    m_items.clear();

    int totalRows = rowList.count();
    // 假设第一行是表头，从第二行开始读
    for (int i = 1; i < totalRows; ++i) {
        // 进度映射到 60-100 之间
        int progressValue = 60 + (i * 40 / (totalRows - 1));
        emit importProgress(progressValue, 100);
        QCoreApplication::processEvents();

        QVariantList colList = rowList.at(i).toList();
        if (colList.count() < 5) continue;

        MqttItem item;
        item.registerAddress = colList.at(0).toInt();
        
        item.registerType = colList.at(1).toInt();
        if (item.registerType < 0 || item.registerType >= s_registerTypes.count()) 
            item.registerType = 0;

        item.mqttName = colList.at(2).toString();

        item.writeType = colList.at(3).toInt();
        if (item.writeType < 0 || item.writeType >= s_writeTypes.count()) 
            item.writeType = 0;

        item.enabled = colList.at(4).toBool();
        
        m_items.append(item);
    }
    emit importProgress(100, 100);

    endResetModel();

    workbook->dynamicCall("Close()");
    excel.dynamicCall("Quit()");
    return true;
}
