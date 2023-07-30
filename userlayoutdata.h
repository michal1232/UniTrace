#ifndef USERLAYOUTDATA_H
#define USERLAYOUTDATA_H

#include <QObject>
#include <QDateTime>
#include "DataViewList.h"
#include "serialthread.h"

#define MAX_NUM_LAYOUT 10

struct UserLayoutList {
    QString typeOf;
    qint32 x;
    qint32 y;
    qint32 width;
    qint32 height;
    QString regName;
    QString label;
    QString function;
    QString value;
};

struct UserLayoutSeq {
    QString regName;
    QVector<QVector <float>> seqData;
};

class UserLayoutData: public QObject
{
    Q_OBJECT
public:
    explicit UserLayoutData(QObject *parent = nullptr);

    static qint32 getCurrMaxPage();
    static QStringList getNamesOfPages();
    static QStringList getListPathImage();

public slots:
    void addFromLayoutList(QString typeOf, qint32 x, qint32 y, qint32 width, qint32 height, QString regName, QString label, QString function, QString value);
    void deleteFromLayoutList(qint32 point);
    void changeXYCoordinateList(qint32 point, QString typeOf, qint32 x, qint32 y);
    void changeWidth(qint32 point, QString typeOf, qint32 width);
    void changeHeight(qint32 point, QString typeOf, qint32 height);
    void changeLabel(qint32 point, QString typeOf, QString label);
    void changeValue(qint32 point, QString typeOf, QString value);
    void changeFunction(qint32 point, QString typeOf, QString function);
    void changeRegName(qint32 point, QString typeOf, QString regName);
    void changeBckImage(qint32 page, QString path);

    void changeSeqReg(qint32 page, qint32 point, QString regName);
    void addSeqRow(qint32 page, qint32 point, QString regName);
    void delSeqRow(qint32 page, qint32 point, QString regName);
    void delSeqAll(qint32 page, qint32 point, QString regName);
    void changeSeqValue(qint32 page, qint32 point, QString regName, qint32 row, qint32 col, QString data);
    void restoreSeqValue(qint32 page, qint32 point, QString regName);
    void saveSeqToFile(qint32 page, qint32 point, QString regName, QString fileSource);
    void loadSeqToFile(qint32 page, qint32 point, QString regName, QString fileSource);
    void loadImageBck(qint32 page, QString path);

    void loadPage(qint32 page);

    void retSelReg(qint32 page, qint32 point ,DataViewItem *item);

    void readReg(qint32 page, qint32 point);
    void writeReg(qint32 page, qint32 point, QString data);
    void setAdamReg(struct DATA_UART_ADAM_PACKET *adamPacekt);
    void setModBusReg(struct MODBUS_RX_STRUCT * modBusPacket, QString logicChannel);
    void setSCPIReg(QString SCPIdata, QString logicChannel, QString regName);

    void setDataToTerminal(QByteArray inData, qint32 src);

    void getFunctSlot(qint32 point, qint32 page, QString regName);
    void getFunctRegSlot(qint32 page, QString regName, qint32 point);
    void getDataType(qint32 point, qint32 page, QString regName);
    void getNoModifLenSlot(qint32 page, qint32 point, QString regName);
    void getLenSlot(qint32 page, QString regName, qint32 point);
    void getRangeSlot(qint32 page, QString regName, qint32 point);
    void getBckImageSlot(qint32 page);

    void saveLayoutItemToFile(QString fileSource, qint32 page);
    void loadLayoutItemToFile(QString fileSource, qint32 page);
    void closeAll();
    void clearPage0();

signals:
    void addToLayoutList(QString typeOf, qint32 x, qint32 y, qint32 width, qint32 height, QString regName, QString function, QString label, QString value, qint32 cnt);
    void layoutComplited();
    void deleteToLayoutList(qint32 point);
    void refreshObj(qint32 point, QString label, qint32 width);
    void changeXYproperty(qint32 x, qint32 y, qint32 height, qint32 width);

    void changeSeqValueSignal(qint32 point, QString regName, qint32 row, qint32 col, QString data);

    void getSelReg(qint32 page, qint32 point, QString regName);
    void sendRCRS(qint32 logicChannel, QString regName, QString regAddress, QString nodeId, QString uartId, QString len, QString offset);
    void sendSCRS(qint32 logicChannel, QString valueType, QString nameReg, QString sentValue, QString regAddress, QString nodeId, QString uartId, QString len, QString offset, QString mulValue);

    void printReg(qint32 point, qint32 page, QString regValue);
    void printRegName(qint32 page, QString regValue);
    void printToTerminal(QString txtData, qint32 src);

    void getFunctSignal(qint32 page, qint32 point, QString funct, QString regName);
    void getFunctRegSignal(QString reg, qint32 page, QString funct, qint32 num, qint32 point);
    void getDataTypeSignal(qint32 page, qint32 point, QString regName, QString dataType);
    void getNoModifLenSignal(QString reg, qint32 page, qint32 point, qint32 len);
    void getLenSignal(QString reg, qint32 page, qint32 len, qint32 point);
    void getRangeSignal(QString reg, qint32 page, qint32 min, qint32 max, qint32 point);
    void getBckImageSignal(qint32 page, QString path);

    void addLayoutPage(QString name, qint32 page);

private:
    UserLayoutData *userLayoutData;

    QVector<UserLayoutList> mList[10];
    QVector<DataViewItem>  mItems[10];   

    QVector<UserLayoutSeq> mSeqDt[10];
    QVector<int> seqPoint[10];

    qint32 rwFromPage = 0;

    static qint32 _currMaxPage;
    static QStringList _namePage;
    static QString bckImagePage[10];
};

#endif // USERLAYOUTDATA_H
