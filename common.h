#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>

#include "DataViewList.h"
#include "adamprotocol.h"
#include "modbusprotocol.h"

#define INFO_TIMEOUT 2000

class common : public QObject
{
    Q_OBJECT
public:
    common(QObject *parent = nullptr);

signals:
    void saveDataModel(QString fileSource);
    void saveDataLayout(QString fileSource, qint32 page);
    void saveDataLC(QString fileSource);

    void loadDataModel(QString fileSource, QString logicChannel, QString protocol, bool openProject);
    void loadDataLayout(QString fileSource, qint32 page);
    void loadLC(QString fileSource);
    void loadImageBck(qint32 page, QString path);

    void closeDataModel(bool deleteAll);
    void closeLayout();
    void closeAllButton();
    void closeAllSerial();

    void recentLoadProj(QString fileSource, QString name);

    void printInfoSignal(QString info);
    void printProjectSignal(QString info);

    void connectionSettingClickSignal();
    void createProtocolClickSignal();
    void modelSettingClickSignal();
    void newProjectClickSignal();

public slots:
    void saveProject(QString fileSource);
    void loadProject(QString fileSource);
    void closeProject();

    void getLastLoadedProject();

    void printInfo(QString info);
    void printProject(QString project);

    void connectionSettingClickSlot();
    void createProtocolClickSlot();
    void modelSettingClickSlot();
    void newProjectClickSlot();

private:
    void timeoutCleanInfo();

    QTimer *timer;
};

#endif // COMMON_H
