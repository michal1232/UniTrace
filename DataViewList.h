#ifndef DATAVIEWLIST_H
#define DATAVIEWLIST_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QAbstractListModel>
#include <QFile>
#include <QTime>
#include <QCoreApplication>
#include <QStandardItemModel>

#include "DataViewModel.h"
#include "serialthread.h"
#include "adamprotocol.h"

/**
 * @brief The DataViewItem struct
 */
struct DataViewItem
{
    bool setValue;
    QString nameReg;
    QString valueType;
    QString readValue;
    QString sentValue;
    QString regAddress;
    QString nodeId;
    QString uartId;
    QString groupReg;
    QString len;
    QString offset;
    QString mulValue;
    QString maxRange;
    QString minRange;
    QString rwStatus;
    QString protocol;
    QString logicChannel;
    QString protopNameReg;
    QString protopGroup;
    QString defaultValue;
    QString description;
    bool selGroup;
};

struct DataViewEn
{
    bool protocolEn;
    bool LCEn;
    bool nameRegEn;
    bool valueTypeEn;
    bool minRangeEn;
    bool maxRangeEn;
    bool rwStatusEn;
    bool uartIdEn;
    bool lenEn;
    bool offsetEn;
    bool nodeIdEn;
    bool regAddressEn;
};

/**
 * @brief The GroupViewItem struct
 */
struct GroupViewItem
{
    bool selGroup;
    QStringList groupLC;
    QStringList groupVal;
    QStringList groupReg;
};

/**
 * @brief The DataViewList class
 */
class DataViewList : public QObject
{
    Q_OBJECT
public:
    explicit DataViewList(QObject *parent = nullptr);

    QVector<DataViewItem> items() const;
    QVector<GroupViewItem> group() const;

    bool setItemAt(int index, const DataViewItem &item);

    void delay( int millisecondsToWait );

    QString getValueTypeP(qint32 inNameReg, QString nameReg, QString offset);
    void setAllRowData(qint32 i);

    static void getSetmItemsViewEn(DataViewEn *viewEn, bool get);

    static DataViewItem *getmItems(QString propRegName);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void setItemData(int index, QVariant value, quint32 role);

    void itemQmlRemoved(int index);

    void sendRcrsSignal(qint32 logicChannel, QString regName, QString regAddress, QString nodeId, QString uartId, QString len, QString offset);
    void sendScrsSignal(qint32 logicChannel, QString valueType, QString nameReg, QString sentValue, QString regAddress, QString nodeId, QString uartId, QString len, QString offset, QString mulValue);

    void refreshItem();
	void setGroup(QVector<GroupViewItem> *group);

    void getmItemsEnViewSignal(bool protocolEn, bool LCEn, bool nameRegEn, bool valueTypeEn, bool minRangeEn, bool maxRangeEn, bool rwStatusEn, bool uartIdEn, bool lenEn, bool offsetEn,  bool nodeIdEn,  bool regAddressEn);
    void getSelItem(qint32 page, qint32 point, DataViewItem *item);
    void setVauleType(qint32 currentIndex, quint32 row);
    void setOffset(quint32 currentIndex, quint32 row);
    void setRwStatus(quint32 currentIndex, quint32 row);
    void setProtocol(quint32 currentIndex, quint32 row);
    void setLC(quint32 currentIndex, quint32 row);
    void toggleSetSignal(qint32 row, bool value);
    void getDefaultValueSignal(qint32 row, QString defValue);
    void getDescriptionSignal(qint32 row, QString desc);

    void refreshRow(qint32 row);

    void selRow(qint32 row);
    void deselRow(qint32 row);

    void getLenSignal(QString reg, qint32 page, qint32 len, qint32 point);
    void getRangeSignal(QString reg, qint32 page, qint32 min, qint32 max, qint32 point);

    void setNewDataToLC(QString logicChannel, QString protocol);
    void changeProtocolToLC(qint32 logicChannel, QString protocol);

    void getIdRegSignal(QStringList idList);

public slots:
    void appendItem(QString lc, qint32 row);
    void removeCompletedItems(bool deleteAll);
    void upSelItem();
    void downSelItem();
    void regroupItem();

    void editItemViewEn(bool enVal, qint32 num);

    void editItemSent(qint32 row, QString input);
    void editItemSet(qint32 row, bool input);
    void editItemSetAll(bool input);
    void editItemMulValue(qint32 row, QString input);
    void editItemNewProject(qint32 row, qint32 column, QString input);
    void editItemProtocolChanged(QString logicChannel, QString protocol);

    void saveItemToFile(QString fileSource);
    void loadItemToFile(QString fileSource, QString logicChannel, QString protocol, bool openProject);
    void saveReadData(QString fileSource);
    void loadSendData(QString fileSource);

    void sendSelected();
    void sendAll();
    void writeSelected();
    void writeAll();

    void saveRequestAdam(struct DATA_UART_ADAM_PACKET *adamPacekt);
    void saveRequestModBus(struct MODBUS_RX_STRUCT *modBusPacket, QString logicChannel);
    void saveRequestSCPI(QString SCPIdata, QString logicChannel, QString regName);

    void setSelItem(qint32 page, qint32 point, QString regName);
    void getIndexOfReg(QString regAddress);

    void getValueType(quint32 row);
    void getOffset(quint32 row);
    void getRwStatus(qint32 row);
    void getProtocol(qint32 row);
    void getLc(qint32 row);
    void setPropRegName(qint32 row);
    void setGroupRegName(qint32 row);
    void setSetReg(qint32 row, bool set);
    void toggleSet(qint32 row);
    void setNewDataToLCSlot(QString logicChannel, QString protocol);
    void changeProtocolToLCSlot(qint32 logicChannel, QString protocol);
    void getDefaultValueSlot(qint32 row);
    void getDescriptionSlot(qint32 row);

    void getLenSlot(qint32 page, QString regName, qint32 point);
    void getRangeSlot(qint32 page, QString regName, qint32 point);
    void getmItemsEnViewSlot();

    void rowChanged(qint32 position, qint32 value);
    void rowDeleted(qint32 position);

    void getIdReg(QString logicChannel, QString nameReg);

private:
    static QVector<DataViewItem>  mItems;
    static QVector<GroupViewItem> mGroup;
    static DataViewEn mItemsViewEn;
    QVector<qint32> inReg;

    void presetDataToWrite(qint32 num);
};

#endif // DATAVIEWLIST_H
