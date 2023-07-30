#ifndef OWNPROTOCOL_H
#define OWNPROTOCOL_H

    #include <QObject>

struct DATA_TYPE_TABLE {
    quint8 uint8Type;
    quint8 uint16Type;
    quint8 uint32Type;
    quint8 int8Type;
    quint8 int16Type;
    quint8 int32Type;
    quint8 xint8Type;
    quint8 xint16Type;
    quint8 xint32Type;
    quint8 bint8Type;
    quint8 bint16Type;
    quint8 bint32Type;
    quint8 floatType;
    quint8 boolType;
    quint8 bitfieldType;
    quint8 charType;
};

struct MSG_TYPE_TABLE {
    quint8 reqRead;
    quint8 reqWrite;
    quint8 regReadRes;
    quint8 regWriteRes;
    quint8 perMsg;
};

struct PACKET_STRUCTURE {
    quint8 *startByte;
    quint8 uartIdSize;
    quint8 nodeIdSize;
    quint8 regSize;
    quint8 lenSize;
    DATA_TYPE_TABLE typeTabel;
    MSG_TYPE_TABLE msgTable;
};

struct PACKET_SETTING {
    bool stringOrRaw;
    bool enReg;
    bool enMessage;
    bool enDataDel;
    char dataDel;
};

class ownProtocol: public QObject
{
    Q_OBJECT
public:
    ownProtocol();
};

#endif // OWNPROTOCOL_H
