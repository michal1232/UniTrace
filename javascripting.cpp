#include "javascripting.h"

UniTrace::UniTrace()
{
    QDir dir(".");

    QStringList appFileList = dir.absolutePath().split("uni-trace.app/");
    sourceFile = appFileList.at(0);
}

void UniTrace::print(QString text)
{
    emit printSignal(text);
}

void UniTrace::wait(int milisec)
{
    QTime dieTime = QTime::currentTime().addMSecs(milisec);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void UniTrace::waitLast(int milisec)
{
    QTime dieTime = QTime::currentTime().addMSecs(milisec);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        if(waitReg == true) {
            waitReg = false;
            break;
        }
    }
}

void UniTrace::write(QString regName, QString data)
{
    DataViewItem *item = DataViewList::getmItems(regName);
    int logicChannel = item->logicChannel.split('C')[1].toInt();
    textWaitReg = regName;
    emit sendScrsSignal(logicChannel, item->valueType, item->protopNameReg, data, item->regAddress, item->nodeId, item->uartId, item->len, item->offset, item->mulValue);
}

void UniTrace::read(QString regName)
{
    DataViewItem *item = DataViewList::getmItems(regName);
    int logicChannel = item->logicChannel.split('C')[1].toInt();
    textWaitReg = regName;
    emit sendRcrsSignal(logicChannel, item->protopNameReg, item->regAddress, item->nodeId, item->uartId, item->len, item->offset);
    //QTimer::singleShot(200, this, &UniTrace::timeoutData);
}

QString UniTrace::readMem(QString regName)
{
    DataViewItem *item = DataViewList::getmItems(regName);
    return item->readValue;
}

void UniTrace::retError(QString text)
{
    error = true;
    textError = text;
}

void UniTrace::retWarning(QString text)
{
    warning = true;
    textWarning = text;
}

void UniTrace::setSource(QString source)
{
    sourceFile =  source;
}

void UniTrace::saveData(QString name, QString data)
{
    filename.append(name);
    filedata.append(data);
}


void UniTrace::readDataToMem(QString regName, QString data, bool status)
{
    if(regName == textWaitReg) {
        waitReg = true;
    }
    emit readHandler(regName, data, status);
}

void UniTrace::timeoutData()
{
    readDataToMem("","", false);
}

JavaScripting::JavaScripting()
{
    uniTrace = new UniTrace;
    myEngine.globalObject().setProperty("uniTrace",
                                      myEngine.newQObject(uniTrace));

    QObject::connect(uniTrace, &UniTrace::printSignal, this, &JavaScripting::print);
    QObject::connect(uniTrace, &UniTrace::sendRcrsSignal, this, &JavaScripting::sendRcrsSlot);
    QObject::connect(uniTrace, &UniTrace::sendScrsSignal, this, &JavaScripting::sendScrsSlot);
    QObject::connect(this, &JavaScripting::readHandler, uniTrace, &UniTrace::readDataToMem);
}

void JavaScripting::callEmit_printReport(QString text)
{
    emit printReport(text);
}

void JavaScripting::saveDataToFile()
{
    for(qint32 i = 0; i < uniTrace->filename.size(); i++) {
        QFile file(uniTrace->sourceFile + uniTrace->filename[i]);
        QStringList lineFile;

        if(file.open(QIODevice::ReadWrite)) {

            while (!file.atEnd()) {
                lineFile.append(file.readLine());
            }

            file.resize(0);

            QTextStream stream(&file);

            for(qint32 i = 0; i < lineFile.size(); i++) {
                stream << lineFile.at(i);
            }

            stream << uniTrace->filedata[i] << "\n";

            file.close();
        }
    }

}

void JavaScripting::runJScriptSlot(QString name, QString page, QString script)
{
    QString time_format = "HH:mm:ss.zzz";
    QDateTime time = QDateTime::currentDateTime();
    QString timeString = time.toString(time_format);

    emit printReport("Runnig [" + name + "] " + timeString + " >>");

   QScriptValue result = myEngine.evaluate("uniTrace.readHandler.connect(readHandler)\n"+script+"\nuniTrace.readHandler.disconnect(readHandler)");
   time = QDateTime::currentDateTime();
   timeString = time.toString(time_format);

   if (myEngine.hasUncaughtException()) {
       int line = myEngine.uncaughtExceptionLineNumber();
       qDebug() << "uncaught exception at line" << line << ":" << result.toString();
       emit printReport("<font color=\"red\">Uncaught exception at line " + QString::number(line) + " : " + result.toString() + "</font>");

       emit printReport(">> End [" + name + "] " + timeString + " ;");
       emit endScriptSuccess(name.toInt(), page.toInt(), false);
   }
   else if(uniTrace->error == true) {
        emit printReport("<font color=\"red\">User define error: " + uniTrace->textError + "</font>");
        emit printReport(">> End [" + name + "] " + timeString + " ;");
        emit endScriptSuccess(name.toInt(), page.toInt(), false);
        uniTrace->error = false;
        uniTrace->textError = "";
    }
    else if(uniTrace->warning == true) {
        emit printReport("<font color=\"orange\">User define warning: " + uniTrace->textWarning + "</font>");
        emit printReport(">> End [" + name + "] " + timeString + " ;");
        emit endScriptSuccess(name.toInt(), page.toInt(), false);
        uniTrace->warning = false;
        uniTrace->textWarning = "";
    }
    else {
         emit printReport(">> End [" + name + "] " + timeString + " ;");
         emit endScriptSuccess(name.toInt(), page.toInt(), true);
    }

   saveDataToFile();
}

void JavaScripting::print(QString text)
{
    emit printReport(text);
}

void JavaScripting::sendRcrsSlot(qint32 logicChannel, QString regName, QString regAddress, QString nodeId, QString uartId, QString len, QString offset)
{
    emit sendRcrsSignal(logicChannel, regName, regAddress, nodeId, uartId, len, offset);
}

void JavaScripting::sendScrsSlot(qint32 logicChannel, QString valueType, QString nameReg, QString sentValue, QString regAddress, QString nodeId, QString uartId, QString len, QString offset, QString mulValue)
{
    emit sendScrsSignal(logicChannel, valueType, nameReg, sentValue, regAddress, nodeId, uartId, len, offset, mulValue);
}

void JavaScripting::readAdamData(DATA_UART_ADAM_PACKET *adamPacekt)
{

}

void JavaScripting::readModbusData(MODBUS_RX_STRUCT *modBusPacket, QString logicChannel)
{
    QString dataOut;
    for(qint32 i = 0; i < modBusPacket->data.size(); i++) {
        dataOut.append(modBusPacket->data[i].toString());
        dataOut.append(",");
    }
    emit readHandler(modBusPacket->regName, dataOut, true);
}

void JavaScripting::readSCPIData(QString SCPIdata, QString logicChannel, QString regName)
{
    emit readHandler(regName, SCPIdata, true);
}

QScriptValue printOut(QScriptContext *context, QScriptEngine *engine)
{
   QScriptValue text = context->argument(0);
   QString stringText = text.toString();
   JavaScripting subClass;

   subClass.callEmit_printReport(stringText);

   return 1;
}

