#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>

#include "DataViewList.h"
#include "adamprotocol.h"
#include "modbusprotocol.h"
#include "scpi_protocol.h"

#define MAX_COUNT_SERIAL_PORT 10U

enum PROTOCOL_NUM{
    ADAM    = 1,
    MODBUS  = 2,
    SCPI    = 3
};

enum SRC_DATA {
    RX = 0,
    TX = 1,
};

enum MSG_TYPE {
    MOD_RCRS = 0,
    MOD_SCRS
};

struct LOCAL_SERIALPORT_SETTING{
    QString fileName;
    qint32 baudRate;
    qint32 dataLenght;
    qint32 parity;
    qint32 stopBit;
    qint32 bufferSize;
    qint32 protocol;
    qint32 timeout;
};

struct MODBUS_QUEUE {
    MSG_TYPE type;
    QString regName;
    QString sentValue;
    QString regAddress;
    QString uartId;
    QString len;
    QString offset;
    QString mulValue;
};

struct SCPI_QUEUE {
    QString regName;
    QString data;
};

class SerialThread : public QObject
{
    Q_OBJECT



public:
    explicit SerialThread(QObject *parent = nullptr);

public slots:
    void addSlots(qint32 logicChannel, QString portName, qint32 baudRate, qint32 dataLenght, qint32 parity, qint32 stopBit, qint32 bufferSize);
    void startSlots(qint32 logicChannel, QString fileName, qint32 baudRate, qint32 dataLenght, qint32 parity, qint32 stopBit, qint32 bufferSize);
    void stopSlots(qint32 logicChannel);
    void restartSlot(qint32 logicChannel);
    void readSerialSlots();
    void writeSerialSlots(qint32 logicChannel, QString sendData);
    void getPortNameSlot();
    void getProtocol(qint32 logicChannel);
    void setProtocol(qint32 logicChannel, qint32 protocol);
    void getSerialStatus(qint32 logicChannel);
    void getStringDataInSlot();
    void clrStringDataInSlot();
    void setNlTime(bool nlTimeEn);
    void setTermTime(bool termTimeEn);
    void setTermLC(bool termLCEn);
    void setTermPaused(bool termPausedEn);
    void getTermLastMsgSlot(bool incCnt);

    void sendRCRS(qint32 logicChannel, QString nameReg, QString regAddress, QString nodeId, QString uartId, QString len, QString offset);
    void sendSCRS(qint32 logicChannel, QString valueType, QString nameReg, QString sentValue, QString regAddress, QString nodeId, QString uartId, QString len, QString offset, QString mulValue);

    void setTimeOut(qint32 logicChannel, QString timeoutStr);
    void timeOutModBus(qint32 logicChannelNum);
    void timeOutSCPI(qint32 logicChannelNum);

    void setNumOfSerial(qint32 logicChannel);
    void getNumOfSerial();
    void getInfoOfSerial(qint32 logicChannel);

    void saveSerialLc(QString fileName);
    void loadSerialLc(QString fileName);
    void closeAllSerial();

signals:    
    void printPortNameSignal(QStringList outString);
    void printPortAvaible(qint32 logicChannel, bool avaiblePort);
    void addSerialLCSignal(QString logicChannel);

    void cleanRxTerminalSignal();
    void printDataSignal(QString inString);
    void printDataByte(QByteArray inData, qint32 src);

    void getProtocolSignal(qint32 protocol);
    void recievedAdamMsg(struct DATA_UART_ADAM_PACKET *rcv_data);
    void recievedModBusMsg(struct MODBUS_RX_STRUCT *rcv_data, QString logicChannel);
    void recievedSCPIMsg(QString SCPIdata, QString logicChannel, QString regName);

    void getNumOfSerialSignal(qint32 numOfSerial);
    void getInfoOfSerialSignal(qint32 numOfSerial, QString fileName, qint32 baudRate, qint32 dataLenght, qint32 parity, qint32 stopBit, qint32 bufferSize, qint32 protocol, qint32 modbusTimeout);

    void editItemProtocolChanged(QString logicChannel, QString protocol);

    void getStringDataInSignal(QString dataIn);
    void getTermLastMsgSignal(QString lastData);

private:
    static QByteArray serial_port_data[MAX_COUNT_SERIAL_PORT];
    static qint32 timeout[MAX_COUNT_SERIAL_PORT];
    static QString stringInData;

    bool serial_port_avaible[MAX_COUNT_SERIAL_PORT];
    bool data_are_store[MAX_COUNT_SERIAL_PORT];
    QSerialPort *serial_port[MAX_COUNT_SERIAL_PORT];
    QString serial_port_buffer[MAX_COUNT_SERIAL_PORT];

    qint32 selProtocol[MAX_COUNT_SERIAL_PORT];    
    qint32 numOfSerial = 0;

    bool termTime = false;
    bool nlTime = false;
    bool termLC = false;
    bool termPaused = false;
    QVector<QString> termLastMsg;
    qint32 termLastMsgCnt = 0;

    LOCAL_SERIALPORT_SETTING serial_port_setting[MAX_COUNT_SERIAL_PORT];
    STRING_UART_ADAM_PACKET uartAdamString[MAX_COUNT_SERIAL_PORT];
    DATA_UART_ADAM_PACKET uartAdamData[MAX_COUNT_SERIAL_PORT];

    QTimer *timers[MAX_COUNT_SERIAL_PORT];
    bool mobBud_Busy[MAX_COUNT_SERIAL_PORT];
    QVector<MODBUS_QUEUE> modbus_queue[MAX_COUNT_SERIAL_PORT];
    QByteArray    modbus_currSend[MAX_COUNT_SERIAL_PORT];

    QTimer *SCPItimers[MAX_COUNT_SERIAL_PORT];
    bool SCPI_Busy[MAX_COUNT_SERIAL_PORT];
    QVector<SCPI_QUEUE> SCPI_queue[MAX_COUNT_SERIAL_PORT];
    SCPI_QUEUE SCPI_currSend[MAX_COUNT_SERIAL_PORT];

    void createQueuePacket(qint32 logicChannel);
};

#endif // SERIALTHREAD_H
