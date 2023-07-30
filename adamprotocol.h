#ifndef ADAMPROTOCOL_H
#define ADAMPROTOCOL_H

#include <QObject>
#include <QVariant>

#define  AS_NODATA   0     /* No data */
#define  AS_ERROR    1     /* Emergency event */
#define  AS_FLOAT    2     /* IEE754 single presision floating point */
#define  AS_LONG     3     /* 2's complement 32 bit integer */
#define  AS_ULONG    4     /* unsigned 32 bit integer */
#define  AS_BLONG    5     /* bit coded 32 bit integer */
#define  AS_SHORT    6
#define  AS_USHORT   7
#define  AS_BSHORT   8
#define  AS_CHAR     9
#define  AS_UCHAR    10
#define  AS_BCHAR    11

/** User define service*/
#define RCRS 102
#define SCRS 103

/**
 * @brief The STRING_UART_ADAM_PACKET struct
 */
struct STRING_UART_ADAM_PACKET{
    QString s_len;
    QString s_nodeId;
    QString s_uartId;
    QString s_dataType;
    QString s_serviceCode;
    QString s_messageCode;
    QString s_data;
};

/**
 * @brief The DATA_UART_ADAM_PACKET struct
 */
struct DATA_UART_ADAM_PACKET{
    quint8 len;
    quint16 nodeId;
    quint8 uartId;
    quint8 dataType;
    quint8 serviceCode;
    quint8 messageCode;
    QVariant data;
};

/**
 * @brief The AdamProtocol class
 */
class AdamProtocol: public QObject
{
    Q_OBJECT
public:
    AdamProtocol();

    QString sendAdamSCRS(QString valueType, QString sentValue, QString regAddress, QString nodeId, QString uartId);
    QString sendAdamRCRS(QString regAddress, QString nodeId, QString uartId);

    DATA_UART_ADAM_PACKET decodeAdamRxData(QString rxSerialData);

private:
    STRING_UART_ADAM_PACKET adamString;
    DATA_UART_ADAM_PACKET adamData;

    void receivedEED(qint32 id, qint32 len, QString frame);
    void receivedNSH(qint32 id, qint32 len, QString frame);
    void receivedUDH(qint32 id, qint32 len, QString frame);
    void receivedNOD(qint32 id, qint32 len, QString frame);
    void receivedUDL(qint32 id, qint32 len, QString frame);
    void receivedDSD(qint32 id, qint32 len, QString frame);
    void receivedNSL(qint32 id, qint32 len, QString frame);
};

#endif // ADAMPROTOCOL_H
