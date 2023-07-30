#include "serialthread.h"

QByteArray SerialThread::serial_port_data[MAX_COUNT_SERIAL_PORT];
qint32 SerialThread::timeout[MAX_COUNT_SERIAL_PORT];
QString SerialThread::stringInData;


/**
 * @brief SerialThread::SerialThread
 * @param parent
 */
SerialThread::SerialThread(QObject *parent) : QObject(parent)
{
    for(qint32 i = 0; i < MAX_COUNT_SERIAL_PORT; i++) {
        serial_port_avaible[i] = false;
        mobBud_Busy[i] = false;
        timers[i] = new QTimer(this);
        SCPI_Busy[i] = false;
        SCPItimers[i] = new QTimer(this);

    }

    connect(timers[0], &QTimer::timeout, this, [this] {timeOutModBus(0); });
    connect(timers[1], &QTimer::timeout, this, [this] {timeOutModBus(1); });
    connect(timers[2], &QTimer::timeout, this, [this] {timeOutModBus(2); });
    connect(timers[3], &QTimer::timeout, this, [this] {timeOutModBus(3); });
    connect(timers[4], &QTimer::timeout, this, [this] {timeOutModBus(4); });
    connect(timers[5], &QTimer::timeout, this, [this] {timeOutModBus(5); });
    connect(timers[6], &QTimer::timeout, this, [this] {timeOutModBus(6); });
    connect(timers[7], &QTimer::timeout, this, [this] {timeOutModBus(7); });
    connect(timers[8], &QTimer::timeout, this, [this] {timeOutModBus(8); });
    connect(timers[9], &QTimer::timeout, this, [this] {timeOutModBus(9); });

    connect(SCPItimers[0], &QTimer::timeout, this, [this] {timeOutSCPI(0); });
    connect(SCPItimers[1], &QTimer::timeout, this, [this] {timeOutSCPI(1); });
    connect(SCPItimers[2], &QTimer::timeout, this, [this] {timeOutSCPI(2); });
    connect(SCPItimers[3], &QTimer::timeout, this, [this] {timeOutSCPI(3); });
    connect(SCPItimers[4], &QTimer::timeout, this, [this] {timeOutSCPI(4); });
    connect(SCPItimers[5], &QTimer::timeout, this, [this] {timeOutSCPI(5); });
    connect(SCPItimers[6], &QTimer::timeout, this, [this] {timeOutSCPI(6); });
    connect(SCPItimers[7], &QTimer::timeout, this, [this] {timeOutSCPI(7); });
    connect(SCPItimers[8], &QTimer::timeout, this, [this] {timeOutSCPI(8); });
    connect(SCPItimers[9], &QTimer::timeout, this, [this] {timeOutSCPI(9); });


}

void SerialThread::addSlots(qint32 logicChannel, QString portName, qint32 baudRate, qint32 dataLenght, qint32 parity, qint32 stopBit, qint32 bufferSize)
{
    serial_port[logicChannel] = new QSerialPort(this);

    serial_port_setting[logicChannel].fileName = portName;
    serial_port_setting[logicChannel].baudRate = baudRate;
    serial_port_setting[logicChannel].dataLenght = dataLenght;
    serial_port_setting[logicChannel].parity = parity;
    serial_port_setting[logicChannel].stopBit = stopBit;
    serial_port_setting[logicChannel].bufferSize = bufferSize;

    emit addSerialLCSignal("LC"+QString::number(logicChannel));
}

/**
 * @brief SerialThread::startSlots
 * @param portName
 * @param baudRate
 * @param dataLenght
 * @param parity
 * @param stopBit
 * @param bufferSize
 * @param protocol
 */
void SerialThread::startSlots(qint32 logicChannel, QString portName, qint32 baudRate, qint32 dataLenght, qint32 parity, qint32 stopBit, qint32 bufferSize)
{
    QSerialPortInfo spInfo;

    foreach(spInfo, QSerialPortInfo::availablePorts())
    {
        if(spInfo.portName() == portName)
        {
            serial_port[logicChannel]->setPortName(portName);
            serial_port[logicChannel]->open(QSerialPort::ReadWrite);
            serial_port[logicChannel]->setBaudRate(baudRate);
            serial_port[logicChannel]->setDataBits((QSerialPort::DataBits)dataLenght);
            serial_port[logicChannel]->setParity((QSerialPort::Parity)parity);
            serial_port[logicChannel]->setStopBits((QSerialPort::StopBits)stopBit);
            serial_port[logicChannel]->setReadBufferSize(bufferSize);
            serial_port[logicChannel]->setFlowControl(QSerialPort::NoFlowControl);


            serial_port_setting[logicChannel].fileName = portName;
            serial_port_setting[logicChannel].baudRate = baudRate;
            serial_port_setting[logicChannel].dataLenght = dataLenght;
            serial_port_setting[logicChannel].parity = parity;
            serial_port_setting[logicChannel].stopBit = stopBit;
            serial_port_setting[logicChannel].bufferSize = bufferSize;

            serial_port_avaible[logicChannel] = true;
            QObject::connect(serial_port[logicChannel], SIGNAL(readyRead()), this, SLOT(readSerialSlots()));

            emit printPortAvaible(logicChannel, true);
            break;
        }
        else
            serial_port_avaible[logicChannel] = false;
    }
    if(serial_port_avaible[logicChannel] != true)
        emit printPortAvaible(logicChannel, false);
}

/**
 * @brief SerialThread::stopSlots
 */
void SerialThread::stopSlots(qint32 logicChannel)
{
    serial_port[logicChannel]->close();
    emit printPortAvaible(logicChannel, false);
}

void SerialThread::restartSlot(qint32 logicChannel)
{
    QSerialPortInfo spInfo;
    foreach(spInfo, QSerialPortInfo::availablePorts())
    {
        if(spInfo.portName() == serial_port_setting[logicChannel].fileName)
        {
            serial_port[logicChannel]->open(QSerialPort::ReadWrite);
            serial_port_avaible[logicChannel] = true;
            emit printPortAvaible(logicChannel, true);
        }
    }

}

/**
 * @brief SerialThread::readSerialSlots
 */
void SerialThread::readSerialSlots()
{
    for(quint32 i=0; i < MAX_COUNT_SERIAL_PORT; i++) {
        if(serial_port_avaible[i] == true) {
            while (serial_port[i]->bytesAvailable())
            {
                QByteArray inData;
                inData.append(serial_port[i]->readAll());

                serial_port_data[i].append(inData);
                data_are_store[i] = true;

                emit printDataByte(inData, RX);

                QString termString = "";
//                termString.append("<font color=\"lightgreen\">");
//                stringInData.append("<font color=\"lightgreen\">");

                if(termTime == true) {
                    QString time_format = "HH:mm:ss.zzz";
                    QDateTime time = QDateTime::currentDateTime();
                    QString timeString = time.toString(time_format);

                    stringInData.append(timeString + ": ");
                    termString.append(timeString + ": ");
                }

                if(termLC == true) {
                    stringInData.append("LC");
                    stringInData.append(QString::number(i));
                    stringInData.append(":> ");
                    termString.append("LC");
                    termString.append(QString::number(i));
                    termString.append(":> ");
                }

                for(int i=0; i<inData.size(); i++) {
                    stringInData += inData[i];
                    termString += inData[i];
                }

                if(nlTime == true) {
                    stringInData += "\n";
                    termString += "\n";
                }
//                termString.append("</font>");
//                stringInData.append("</font>");

                if(termPaused == false) {
                    emit printDataSignal(termString);
                   }

                inData.clear();

                if(selProtocol[i] == ADAM) {
                    if(serial_port_data[i].contains("\n"))
                    {
                        serial_port_buffer[i] += QString::fromStdString(serial_port_data[i].toStdString());


                        AdamProtocol adamProtocol;
                        uartAdamData[i] = adamProtocol.decodeAdamRxData(serial_port_buffer[i]);
                        emit recievedAdamMsg(&uartAdamData[i]);



                        serial_port_buffer[i].clear();
                        serial_port_data[i].clear();
                        data_are_store[i] = false;

                        break;
                    }
                }
            }
        }
    }

}

/**
 * @brief SerialThread::writeSerialSlots
 * @param sendData
 */
void SerialThread::writeSerialSlots(qint32 logicChannel, QString sendData)
{
    if(sendData.contains("\n"))
    {
        sendData.remove(sendData.size()-1,1);

        emit cleanRxTerminalSignal();

        termLastMsg.insert(0, sendData);
        termLastMsgCnt = 0;

        if(serial_port_avaible[logicChannel] == true) {
            if(serial_port[logicChannel]->isWritable())
            {
                serial_port[logicChannel]->write(sendData.toStdString().c_str());
            }
        }
    }
}

/**
 * @brief SerialThread::getPortNameSlot
 */
void SerialThread::getPortNameSlot()
{
    QSerialPortInfo spInfo;
    QStringList stringOfPorts;

    foreach(spInfo, QSerialPortInfo::availablePorts())
    {
        if(spInfo.hasVendorIdentifier() && spInfo.hasVendorIdentifier())
        {
            stringOfPorts.append(spInfo.portName());
        }
    }
    qInfo() << stringOfPorts;
    emit printPortNameSignal(stringOfPorts);
}

void SerialThread::getProtocol(qint32 logicChannel)
{
    emit getProtocolSignal(selProtocol[logicChannel]);
}

void SerialThread::setProtocol(qint32 logicChannel, qint32 protocol)
{
    selProtocol[logicChannel] = protocol;
    serial_port_setting[logicChannel].protocol = protocol;

    if(protocol == 1)
        emit editItemProtocolChanged("LC"+QString::number(logicChannel), "ADAM");
    else if(protocol == 2)
        emit editItemProtocolChanged("LC"+QString::number(logicChannel), "ModBus");
    else if(protocol == 3)
        emit editItemProtocolChanged("LC"+QString::number(logicChannel), "SCPI");
}

void SerialThread::getSerialStatus(qint32 logicChannel)
{
    if(serial_port_avaible[logicChannel] == true) {
        emit printPortAvaible(logicChannel, true);
    }
    else {
        emit printPortAvaible(logicChannel, false);
    }
}

void SerialThread::getStringDataInSlot()
{
    emit getStringDataInSignal(stringInData);
}

void SerialThread::clrStringDataInSlot()
{
    stringInData.clear();
}

void SerialThread::setNlTime(bool nlTimeEn)
{
   nlTime = nlTimeEn;
}

void SerialThread::setTermTime(bool termTimeEn)
{
    termTime = termTimeEn;
}

void SerialThread::setTermLC(bool termLCEn)
{
    termLC = termLCEn;
}

void SerialThread::setTermPaused(bool termPausedEn)
{
    termPaused = termPausedEn;
}

void SerialThread::getTermLastMsgSlot(bool incCnt)
{
    emit getTermLastMsgSignal(termLastMsg[termLastMsgCnt]);

    if(incCnt) {
        if(termLastMsgCnt < (termLastMsg.size()-1))
            termLastMsgCnt++;
    }
    else {
        if(termLastMsgCnt != 0)
            termLastMsgCnt--;
    }
}

/**
 * @brief SerialThread::sendRCRS
 * @param valueType
 * @param sentValue
 * @param regAddress
 * @param nodeId
 * @param uartId
 */
void SerialThread::sendRCRS(qint32 logicChannel, QString nameReg, QString regAddress, QString nodeId, QString uartId, QString len, QString offset)
{
    QString sendString;
    QByteArray sendQuint8;

    if(selProtocol[logicChannel] == ADAM) {
        AdamProtocol adamProtocol;
        //TODO:Correct ADAM protocol with saving nameReg
        sendString = adamProtocol.sendAdamRCRS(regAddress, nodeId, uartId);

        if(serial_port[logicChannel]->isWritable())
        {
            serial_port[logicChannel]->write(sendString.toStdString().c_str());
        }
    }
    else if(selProtocol[logicChannel] == MODBUS) {
        if(mobBud_Busy[logicChannel] == false) {
            mobBud_Busy[logicChannel] = true;
            serial_port_data[logicChannel] = "";

            ModBusProtocol modBusProtocol;
            sendQuint8 = modBusProtocol.sendModBusRCRS(nameReg, regAddress,uartId,len,offset);

            emit printDataByte(sendQuint8, TX);

            if(serial_port[logicChannel]->isWritable())
            {
                serial_port[logicChannel]->write(sendQuint8);
                //QTimer::singleShot(timeout[logicChannel], this, &SerialThread::timeOutModBus);
                timers[logicChannel]->setInterval(timeout[logicChannel]);
                timers[logicChannel]->setSingleShot(true);
                timers[logicChannel]->start();
            }
        }
        else {
            MODBUS_QUEUE new_rsrc_data;

            new_rsrc_data.type = MOD_RCRS;
            new_rsrc_data.len = len;
            new_rsrc_data.mulValue = 1;
            new_rsrc_data.offset = offset;
            new_rsrc_data.regAddress = regAddress;
            new_rsrc_data.regName = nameReg;
            new_rsrc_data.sentValue = "";
            new_rsrc_data.uartId = uartId;

            modbus_queue[logicChannel].append(new_rsrc_data);
        }
        qInfo() << modbus_queue[logicChannel].size();
    }


}

/**
 * @brief SerialThread::sendSCRS
 * @param valueType
 * @param sentValue
 * @param regAddress
 * @param nodeId
 * @param uartId
 */
void SerialThread::sendSCRS(qint32 logicChannel,QString valueType, QString nameReg, QString sentValue, QString regAddress, QString nodeId, QString uartId, QString len, QString offset, QString mulValue)
{
    QString sendString;

    if(selProtocol[logicChannel] == ADAM) {
        AdamProtocol adamProtocol;
        sendString = adamProtocol.sendAdamSCRS(valueType, sentValue, regAddress, nodeId, uartId);
        if(serial_port[logicChannel]->isWritable())
        {
            serial_port[logicChannel]->write(sendString.toStdString().c_str());
        }
    }
    else if(selProtocol[logicChannel] == MODBUS) {
        if(mobBud_Busy[logicChannel] == false) {
            mobBud_Busy[logicChannel] = true;
            serial_port_data[logicChannel] = "";

            ModBusProtocol modBusProtocol;
            modbus_currSend[logicChannel]  = modBusProtocol.sendModBusSCRS(nameReg, sentValue, regAddress, uartId, len, offset, mulValue);
            emit printDataByte(modbus_currSend[logicChannel] , TX);

            if(serial_port[logicChannel]->isWritable())
            {
                serial_port[logicChannel]->write(modbus_currSend[logicChannel] );
                //QTimer::singleShot(timeout[logicChannel], this, &SerialThread::timeOutModBus);
                timers[logicChannel]->setInterval(timeout[logicChannel]);
                timers[logicChannel]->setSingleShot(true);
                timers[logicChannel]->start();
            }
        }
        else {
            MODBUS_QUEUE new_scrs_data;

            new_scrs_data.type = MOD_SCRS;
            new_scrs_data.len = len;
            new_scrs_data.mulValue = mulValue;
            new_scrs_data.offset = offset;
            new_scrs_data.regAddress = regAddress;
            new_scrs_data.regName = nameReg;
            new_scrs_data.sentValue = sentValue;
            new_scrs_data.uartId = uartId;

            modbus_queue[logicChannel].append(new_scrs_data);
        }
        qInfo() << modbus_queue[logicChannel].size();
    }
    else if(selProtocol[logicChannel] == SCPI) {
        SCPI_Protocol scpiProtocol;

        if(SCPI_Busy[logicChannel] == false) {
            SCPI_Busy[logicChannel] = true;
            serial_port_data[logicChannel] = "";

            SCPI_currSend[logicChannel].data = scpiProtocol.sendSCPImsg(nameReg, sentValue);
            SCPI_currSend[logicChannel].regName = nameReg;
            if(serial_port[logicChannel]->isWritable())
            {
                emit printDataByte(SCPI_currSend[logicChannel].data.toUtf8() , TX);
                serial_port[logicChannel]->write(SCPI_currSend[logicChannel].data.toStdString().c_str());
                SCPItimers[logicChannel]->setInterval(timeout[logicChannel]);
                SCPItimers[logicChannel]->setSingleShot(true);
                SCPItimers[logicChannel]->start();
            }

        }
        else {
            SCPI_queue[logicChannel].append({nameReg, scpiProtocol.sendSCPImsg(nameReg, sentValue)});
        }
        qInfo() << modbus_queue[logicChannel].size();


    }
}

void SerialThread::setTimeOut(qint32 logicChannel, QString timeoutStr)
{
    timeout[logicChannel] = timeoutStr.toInt();
    serial_port_setting[logicChannel].timeout = timeoutStr.toInt();
}

void SerialThread::timeOutModBus(qint32 logicChannelNum)
{
    static quint32 error_frame_cnt = 0;
    ModBusProtocol modBusProtocol;
    struct MODBUS_RX_STRUCT data;
    QString logicChannel;

    data.status = false;

    if(serial_port_avaible[logicChannelNum] == true) {
        if(data_are_store[logicChannelNum] == true) {
            data = modBusProtocol.rxModBus(serial_port_data[logicChannelNum]);
            serial_port_data[logicChannelNum].clear();
            data_are_store[logicChannelNum] = false;
            logicChannel = "LC" + QString::number(logicChannelNum);
        }
    }

    if(data.status == true) {
        emit recievedModBusMsg(&data, logicChannel);

        if(modbus_queue[logicChannelNum].isEmpty() == false) {
            createQueuePacket(logicChannelNum);

            modbus_queue[logicChannelNum].removeFirst();

            if(modbus_queue[logicChannelNum].isEmpty()) {
                mobBud_Busy[logicChannelNum] = false;
            }

        }
        else {
            mobBud_Busy[logicChannelNum] = false;
        }
    }
    else {
        if(error_frame_cnt == 4) {
            error_frame_cnt = 0;

            if(modbus_queue[logicChannelNum].isEmpty() == false) {
                modbus_queue[logicChannelNum].removeFirst();;

                if(modbus_queue[logicChannelNum].isEmpty()) {
                    mobBud_Busy[logicChannelNum] = false;
                }
                else {
                    createQueuePacket(logicChannelNum);
                }
            }
            else {
                mobBud_Busy[logicChannelNum] = false;
            }
        }
        else {
            if(modbus_queue[logicChannelNum].isEmpty() == false) {
                createQueuePacket(logicChannelNum);
                error_frame_cnt++;
            }
            else {
                if(serial_port[logicChannelNum]->isWritable())
                {
                    serial_port[logicChannelNum]->write(modbus_currSend[logicChannelNum] );
                    //QTimer::singleShot(timeout[logicChannelNum], this, &SerialThread::timeOutModBus);
                    timers[logicChannelNum]->setInterval(timeout[logicChannelNum]);
                    timers[logicChannelNum]->setSingleShot(true);
                    timers[logicChannelNum]->start();
                }
                error_frame_cnt++;
            }
        }
    }
}

void SerialThread::timeOutSCPI(qint32 logicChannelNum)
{
    static quint32 error_frame_cnt = 0;
    QString logicChannel;
    QString inData;

    if(serial_port_avaible[logicChannelNum] == true) {
        if(data_are_store[logicChannelNum] == true) {
            inData = serial_port_data[logicChannelNum];
            serial_port_data[logicChannelNum].clear();
            data_are_store[logicChannelNum] = false;
            logicChannel = "LC" + QString::number(logicChannelNum);
        }
    }

    if(inData != "") {
        emit recievedSCPIMsg(inData, logicChannel, SCPI_currSend[logicChannelNum].regName.toUtf8());

        if(SCPI_queue[logicChannelNum].isEmpty() == false) {
            if(serial_port[logicChannelNum]->isWritable())
            {
                emit printDataByte(SCPI_queue[logicChannelNum].at(0).data.toUtf8() , TX);
                SCPI_currSend[logicChannelNum].regName = SCPI_queue[logicChannelNum].at(0).regName;
                serial_port[logicChannelNum]->write(SCPI_queue[logicChannelNum].at(0).data.toStdString().c_str());
                SCPItimers[logicChannelNum]->setInterval(timeout[logicChannelNum]);
                SCPItimers[logicChannelNum]->setSingleShot(true);
                SCPItimers[logicChannelNum]->start();
            }

            SCPI_queue[logicChannelNum].removeFirst();

            if(SCPI_queue[logicChannelNum].isEmpty()) {
                SCPI_Busy[logicChannelNum] = false;
            }

        }
        else {
            SCPI_Busy[logicChannelNum] = false;
        }
    }
    else {
        if(error_frame_cnt == 4) {
            error_frame_cnt = 0;

            if(SCPI_queue[logicChannelNum].isEmpty() == false) {
                SCPI_queue[logicChannelNum].removeFirst();;

                if(SCPI_queue[logicChannelNum].isEmpty()) {
                    SCPI_Busy[logicChannelNum] = false;
                }
                else {
                    if(serial_port[logicChannelNum]->isWritable())
                    {
                        emit printDataByte(SCPI_queue[logicChannelNum].at(0).data.toUtf8() , TX);
                        SCPI_currSend[logicChannelNum].regName = SCPI_queue[logicChannelNum].at(0).regName;
                        serial_port[logicChannelNum]->write(SCPI_queue[logicChannelNum].at(0).data.toStdString().c_str());
                        SCPItimers[logicChannelNum]->setInterval(timeout[logicChannelNum]);
                        SCPItimers[logicChannelNum]->setSingleShot(true);
                        SCPItimers[logicChannelNum]->start();
                    }
                }
            }
            else {
                SCPI_Busy[logicChannelNum] = false;
            }
        }
        else {
            if(SCPI_queue[logicChannelNum].isEmpty() == false) {
                if(serial_port[logicChannelNum]->isWritable())
                {
                    emit printDataByte(SCPI_queue[logicChannelNum].at(0).data.toUtf8() , TX);
                    SCPI_currSend[logicChannelNum].regName = SCPI_queue[logicChannelNum].at(0).regName;
                    serial_port[logicChannelNum]->write(SCPI_queue[logicChannelNum].at(0).data.toStdString().c_str());
                    SCPItimers[logicChannelNum]->setInterval(timeout[logicChannelNum]);
                    SCPItimers[logicChannelNum]->setSingleShot(true);
                    SCPItimers[logicChannelNum]->start();
                }
                error_frame_cnt++;
            }
            else {
                if(serial_port[logicChannelNum]->isWritable())
                {
                    serial_port[logicChannelNum]->write(SCPI_currSend[logicChannelNum].data.toStdString().c_str() );
                    SCPItimers[logicChannelNum]->setInterval(timeout[logicChannelNum]);
                    SCPItimers[logicChannelNum]->setSingleShot(true);
                    SCPItimers[logicChannelNum]->start();
                }
                error_frame_cnt++;
            }
        }
    }
}

void SerialThread::setNumOfSerial(qint32 logicChannel)
{
    numOfSerial = logicChannel;
}

void SerialThread::getNumOfSerial()
{
    emit getNumOfSerialSignal(numOfSerial);
}

void SerialThread::getInfoOfSerial(qint32 logicChannel)
{
    emit getInfoOfSerialSignal(logicChannel,
                               serial_port_setting[logicChannel].fileName,
                               serial_port_setting[logicChannel].baudRate,
                               serial_port_setting[logicChannel].dataLenght,
                               serial_port_setting[logicChannel].parity,
                               serial_port_setting[logicChannel].stopBit,
                               serial_port_setting[logicChannel].bufferSize,
                               serial_port_setting[logicChannel].protocol,
                               serial_port_setting[logicChannel].timeout);
}

void SerialThread::saveSerialLc(QString fileName)
{
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "SerialLcData" << ";"
                   << "\n";

            stream << "LogicChannel" << ";"
                   << "fileName" << ";"
                   << "baudRate" << ";"
                   << "dataLenght" << ";"
                   << "parity" << ";"
                   << "stopBit" << ";"
                   << "bufferSize" << ";"
                   << "protocol" << ";"
                   << "timeout" << ";"
                   << "\n";

            for (int c=0; c < numOfSerial; c++ )
                stream << (QString::number(c)) << ";"
                   << serial_port_setting[c].fileName << ";"
                   << serial_port_setting[c].baudRate << ";"
                   << serial_port_setting[c].dataLenght << ";"
                   << serial_port_setting[c].parity << ";"
                   << serial_port_setting[c].stopBit << ";"
                   << serial_port_setting[c].bufferSize << ";"
                   << serial_port_setting[c].protocol << ";"
                   << serial_port_setting[c].timeout << ";"
                   << "\n";
        }
    }
}

/**
 * @brief SerialThread::loadSerialLc
 * @param fileName
 */
void SerialThread::loadSerialLc(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite)) {

        QString source = file.readLine();
        if(source.contains("SerialLcData")) {

            QString header = file.readLine();
            qInfo() << header;

            while (!file.atEnd()) {
                QString line = file.readLine();
                QStringList list = line.split(QLatin1Char(';'));

                if(list.size() >= 8) {
                    numOfSerial++;
                    addSlots(list.at(0).toUInt(), list.at(1), list.at(2).toUInt(), list.at(3).toUInt(),list.at(4).toUInt(),list.at(5).toUInt(),list.at(6).toUInt());
                    setTimeOut(list.at(0).toUInt(), list.at(8));
                    setProtocol(list.at(0).toUInt(), list.at(7).toUInt());
                }
            }
        }
    }
}

void SerialThread::closeAllSerial()
{
    for(qint32 i = 0; i < numOfSerial; i++) {
        serial_port[i]->close();
        QObject::disconnect(serial_port[i], SIGNAL(readyRead()), this, SLOT(readSerialSlots()));

        serial_port_data[i].clear();
        timeout[i] = 0;

        serial_port_avaible[i] = false;
        data_are_store[i] = false;
        serial_port[i] = 0;
        serial_port_buffer[i].clear();

        selProtocol[i] = 0;
    }
    numOfSerial = 0;
}

void SerialThread::createQueuePacket(qint32 logicChannel)
{
    QByteArray sendQuint8;

    ModBusProtocol modBusProtocol;

    if(modbus_queue[logicChannel].at(0).type == MOD_SCRS) {
        sendQuint8 = modBusProtocol.sendModBusSCRS(modbus_queue[logicChannel].at(0).regName,
                                                   modbus_queue[logicChannel].at(0).sentValue,
                                                   modbus_queue[logicChannel].at(0).regAddress,
                                                   modbus_queue[logicChannel].at(0).uartId,
                                                   modbus_queue[logicChannel].at(0).len,
                                                   modbus_queue[logicChannel].at(0).offset,
                                                   modbus_queue[logicChannel].at(0).mulValue);

    }
    else {
        sendQuint8 = modBusProtocol.sendModBusRCRS(modbus_queue[logicChannel].at(0).regName,
                                                   modbus_queue[logicChannel].at(0).regAddress,
                                                   modbus_queue[logicChannel].at(0).uartId,
                                                   modbus_queue[logicChannel].at(0).len,
                                                   modbus_queue[logicChannel].at(0).offset);
    }

    emit printDataByte(sendQuint8, TX);

    if(serial_port[logicChannel]->isWritable())
    {
        serial_port[logicChannel]->write(sendQuint8);
        //QTimer::singleShot(timeout[logicChannel], this, &SerialThread::timeOutModBus);
        timers[logicChannel]->setInterval(timeout[logicChannel]);
        timers[logicChannel]->setSingleShot(true);
        timers[logicChannel]->start();
    }
}
