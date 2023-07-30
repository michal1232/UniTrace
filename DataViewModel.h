#ifndef DATAVIEWMODEL_H
#define DATAVIEWMODEL_H

#include <QAbstractListModel>
#include "DataViewList.h"
#include "serialthread.h"

class DataViewList;

class DataViewModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(DataViewList *list READ list WRITE setList)

public:
    explicit DataViewModel(QObject *parent = nullptr);

    enum MODEL_ROLE {
        SetValueRole = Qt::UserRole,
        NameRegRole,
        ValueTypeRole,
        ReadValueRole,
        SentValueRole,
        RegAddressRole,
        NodeIdRole,
        UartIdRole,
        GroupRegRole,
        LenRole,
        OffsetRole,
        MulValueRole,
        MinRangeRole,
        MaxRangeRole,
        RWStatusRole,
        ProtocolRole,
        LCRole,
        ProtopNameRegRole,
        ProtopGroupRole,
        DefaultValueRole,
        DescriptionRole,
        SelGroupRole,
    } ;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    DataViewList *list() const;
    void setList(DataViewList *list);

public slots:
    void changeData(qint32 index, QVariant value, quint32 role);

private:
    DataViewList *mList;
};

#endif // DATAVIEWMODEL_H
