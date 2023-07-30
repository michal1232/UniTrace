#ifndef JAVASCRIPTING_H
#define JAVASCRIPTING_H

#include <QObject>
#include <QtScript/QScriptable>
#include <QScriptEngine>
#include <QDateTime>
#include <QtDebug>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QDir>
#include "DataViewList.h"

QScriptValue printOut(QScriptContext *context, QScriptEngine *engine);

class UniTrace : public QObject, public QScriptable
{
    Q_OBJECT
public:
    UniTrace();

    bool error = false;
    QString textError = "";

    bool warning = false;
    QString textWarning = "";

    bool waitReg = false;
    QString textWaitReg = "";

    QVector<QString> filename;
    QVector<QString> filedata;
    QString sourceFile;

signals:
    void printSignal(QString text);
    void sendRcrsSignal(qint32 logicChannel, QString regName, QString regAddress, QString nodeId, QString uartId, QString len, QString offset);
    void sendScrsSignal(qint32 logicChannel, QString valueType, QString nameReg, QString sentValue, QString regAddress, QString nodeId, QString uartId, QString len, QString offset, QString mulValue);

    void readHandler(QString regName, QString data, bool status);

public slots:
    void print(QString text);
    void wait(int milisec);
    void waitLast(int milisec);
    void write(QString regName, QString data);
    void read(QString regName);
    QString readMem(QString regName);
    void retError(QString text);
    void retWarning(QString text);
    void setSource(QString source);
    void saveData(QString name, QString data);

    void readDataToMem(QString regName, QString data, bool status);
    void timeoutData();

private:


};



class JavaScripting: public QObject
{
    Q_OBJECT
public:
    JavaScripting();
    void callEmit_printReport(QString text);
    void saveDataToFile();

signals:
    void printReport(QString text);
    void sendRcrsSignal(qint32 logicChannel, QString regName, QString regAddress, QString nodeId, QString uartId, QString len, QString offset);
    void sendScrsSignal(qint32 logicChannel, QString valueType, QString nameReg, QString sentValue, QString regAddress, QString nodeId, QString uartId, QString len, QString offset, QString mulValue);

    void readHandler(QString regName, QString data, bool status);

    void endScriptSuccess(qint32 name, qint32 page, bool status);

public slots:
    void runJScriptSlot(QString name, QString page, QString script);
    void print(QString text);
    void sendRcrsSlot(qint32 logicChannel, QString regName, QString regAddress, QString nodeId, QString uartId, QString len, QString offset);
    void sendScrsSlot(qint32 logicChannel, QString valueType, QString nameReg, QString sentValue, QString regAddress, QString nodeId, QString uartId, QString len, QString offset, QString mulValue);


    void readAdamData(struct DATA_UART_ADAM_PACKET *adamPacekt);
    void readModbusData(struct MODBUS_RX_STRUCT *modBusPacket, QString logicChannel);
    void readSCPIData(QString SCPIdata, QString logicChannel, QString regName);
private:
    UniTrace *uniTrace;
    QScriptEngine myEngine;
};



#endif // JAVASCRIPTING_H
