#include "DataViewModel.h"

/**
 * @brief DataViewModel::DataViewModel
 * @param parent
 */
DataViewModel::DataViewModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{
}

/**
 * @brief DataViewModel::rowCount
 * @param parent
 * @return
 */
int DataViewModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

/**
 * @brief DataViewModel::data
 * @param index
 * @param role
 * @return
 */
QVariant DataViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const DataViewItem item = mList->items().at(index.row());
    switch (role) {
    case SetValueRole:
        return QVariant(item.setValue);
    case NameRegRole:
        return QVariant(item.nameReg);
    case ValueTypeRole:
        return QVariant(item.valueType);
    case ReadValueRole:
        return QVariant(item.readValue);
    case SentValueRole:
        return QVariant(item.sentValue);
    case RegAddressRole:
        return QVariant(item.regAddress);
    case NodeIdRole:
        return QVariant(item.nodeId);
    case UartIdRole:
        return QVariant(item.uartId);
    case GroupRegRole:
        return QVariant(item.groupReg);
    case LenRole:
        return QVariant(item.len);
    case OffsetRole:
        return QVariant(item.offset);
    case MulValueRole:
        return QVariant(item.mulValue);
    case MinRangeRole:
        return QVariant(item.minRange);
    case MaxRangeRole:
        return QVariant(item.maxRange);
    case RWStatusRole:
        return QVariant(item.rwStatus);
    case ProtocolRole:
        return QVariant(item.protocol);
    case LCRole:
        return QVariant(item.logicChannel);
    case ProtopNameRegRole:
        return QVariant(item.protopNameReg);
    case ProtopGroupRole:
        return QVariant(item.protopGroup);
    case DefaultValueRole:
        return QVariant(item.defaultValue);
    case DescriptionRole:
        return QVariant(item.description);
    case SelGroupRole:
        return QVariant(item.selGroup);
    }

    return QVariant();
}

/**
 * @brief DataViewModel::setData
 * @param index
 * @param value
 * @param role
 * @return
 */
bool DataViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    DataViewItem item = mList->items().at(index.row());
    switch (role) {
    case SetValueRole:
        item.setValue = value.toBool();
        break;
    case NameRegRole:
        item.nameReg = value.toString();
        break;
    case ValueTypeRole:
        item.valueType = value.toString();
        break;
    case ReadValueRole:
        item.readValue = value.toString();
        break;
    case SentValueRole:
        item.sentValue = value.toString();
        break;
    case RegAddressRole:
        item.regAddress = value.toString();
        break;
    case NodeIdRole:
        item.nodeId = value.toString();
        break;
    case UartIdRole:
        item.uartId = value.toString();
        break;
    case GroupRegRole:
        item.groupReg = value.toString();
        break;
    case LenRole:
        item.len = value.toString();
        break;
    case OffsetRole:
        item.offset = value.toString();
        break;
    case MulValueRole:
        item.mulValue = value.toString();
        break;
    case MinRangeRole:
        item.minRange = value.toString();
        break;
    case MaxRangeRole:
        item.maxRange = value.toString();
        break;
    case RWStatusRole:
        item.rwStatus = value.toString();
        break;
    case ProtocolRole:
        item.protocol = value.toString();
        break;
    case LCRole:
        item.logicChannel = value.toString();
        break;
    case ProtopNameRegRole:
        item.protopNameReg = value.toString();
        break;
    case ProtopGroupRole:
        item.protopGroup = value.toString();
        break;
    case DefaultValueRole:
        item.defaultValue = value.toString();
        break;
    case DescriptionRole:
        item.description = value.toString();
        break;
    case SelGroupRole:
        item.selGroup = value.toBool();
        break;
    }

    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

/**
 * @brief DataViewModel::flags
 * @param index
 * @return
 */
Qt::ItemFlags DataViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

/**
 * @brief DataViewModel::roleNames
 * @return
 */
QHash<int, QByteArray> DataViewModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[SetValueRole] = "setValue";
    names[NameRegRole] = "nameReg";
    names[ValueTypeRole] = "valueType";
    names[ReadValueRole] = "readValue";
    names[SentValueRole] = "sentValue";
    names[RegAddressRole] = "regAddress";
    names[NodeIdRole] = "nodeId";
    names[UartIdRole] = "uartId";
    names[GroupRegRole] = "groupReg";
    names[LenRole] = "len";
    names[OffsetRole] = "offset";
    names[MulValueRole] = "mulValue";
    names[MinRangeRole] = "minRange";
    names[MaxRangeRole] = "maxRange";
    names[RWStatusRole] = "rwStatus";
    names[ProtocolRole] = "protocol";
    names[LCRole] = "logicChannel";
    names[ProtopNameRegRole] = "protopNameReg";
    names[ProtopGroupRole] = "protopGroup";
    names[DefaultValueRole] = "defaultVaule";
    names[DescriptionRole] = "descripton";
    names[SelGroupRole] = "selGroup";
    return names;
}

/**
 * @brief DataViewModel::list
 * @return
 */
DataViewList *DataViewModel::list() const
{
    return mList;
}

/**
 * @brief DataViewModel::setList
 * @param list
 */
void DataViewModel::setList(DataViewList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &DataViewList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &DataViewList::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(mList, &DataViewList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &DataViewList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
        connect(mList, &DataViewList::setItemData, this, &DataViewModel::changeData);
    }
    endResetModel();
}

/**
 * @brief DataViewModel::changeData
 * @param index
 * @param value
 */
void DataViewModel::changeData(qint32 index, QVariant value, quint32 role)
{
    QModelIndex modelIndex = createIndex(index, (role-Qt::UserRole), &value);
    this->setData(modelIndex, value, role);
}

