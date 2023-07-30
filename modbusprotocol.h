#ifndef MODBUSPROTOCOL_H
#define MODBUSPROTOCOL_H

#include <QObject>
#include "DataViewList.h"

struct MODBUS_DATA_STRUCT {
    QString regName;
    quint8 slaveID;
    quint8 functionCode;
    quint16 registerAddr;
    quint16 numRegister;
    quint8 numBytes;
    quint16 crc;
    QVector<QVariant> data;
};

struct MODBUS_RX_STRUCT{
    QVector<QVariant> data;
    QString dataType;
    quint16 regAddr;
    QString regName;
    QString regOffset;
    QString regNode;
    bool status;
};

class ModBusProtocol: public QObject
{
    Q_OBJECT
public:
    ModBusProtocol();

    QByteArray sendModBusRCRS(QString regName, QString regAddress, QString uartId, QString len, QString offset);
    QByteArray sendModBusSCRS(QString regName, QString sentValue, QString regAddress, QString uartId, QString len, QString offset, QString mulValue);
    MODBUS_RX_STRUCT rxModBus(QByteArray byteArray);

private:
    static MODBUS_DATA_STRUCT modBusData;

    quint16 crc16Update(quint16 crc, quint8 dataIn);
};

#endif // MODBUSPROTOCOL_H
