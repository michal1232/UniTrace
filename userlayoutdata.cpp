#include "userlayoutdata.h"
#include <QDebug>
#include <QXmlStreamReader>


qint32 UserLayoutData::_currMaxPage = 0;
QStringList UserLayoutData::_namePage;
QString UserLayoutData::bckImagePage[10];
/**
 * @brief UserLayoutData::UserLayoutData
 * @param parent
 */
UserLayoutData::UserLayoutData(QObject *parent) : QObject(parent)
{

}

qint32 UserLayoutData::getCurrMaxPage()
{
    return _currMaxPage;
}

QStringList UserLayoutData::getNamesOfPages()
{
    return _namePage;
}

QStringList UserLayoutData::getListPathImage()
{
    QStringList listPath;

    for(qint32 i = 0; i < 10; i++) {
        if(bckImagePage[i] == "")
            break;

        listPath.append(bckImagePage[i]);
    }

    return listPath;
}

/**
 * @brief UserLayoutData::addFromLayoutList
 * @param typeOf
 * @param x
 * @param y
 * @param width
 * @param regName
 * @param label
 */
void UserLayoutData::addFromLayoutList(QString typeOf, qint32 x, qint32 y, qint32 width, qint32 height, QString regName, QString label, QString function, QString value)
{
    UserLayoutList list;
    DataViewItem item;
    UserLayoutSeq seq;

    list.typeOf = typeOf;
    list.x = x;
    list.y = y;
    list.width = width;
    list.height = height;
    list.regName = regName;
    list.label = label;
    list.function = function;
    list.value = value;


    mList[0].append(list);
    mItems[0].append(item);

    if(typeOf == "seqView") {
        mSeqDt[0].append(seq);
        seqPoint[0].append(mList[0].size()-1);
    }

}

void UserLayoutData::deleteFromLayoutList(qint32 point)
{
    mList[0].removeAt(point);
    mItems[0].removeAt(point);

    emit deleteToLayoutList(point);
}

/**
 * @brief UserLayoutData::changeXYCoordinateList
 * @param point
 * @param typeOf
 * @param x
 * @param y
 */
void UserLayoutData::changeXYCoordinateList(qint32 point, QString typeOf, qint32 x, qint32 y)
{
    mList[0][point].typeOf = typeOf;
    mList[0][point].x = x;
    mList[0][point].y = y;

    emit changeXYproperty(x, y, mList[0][point].height, mList[0][point].width);
}

/**
 * @brief UserLayoutData::changeWidth
 * @param point
 * @param typeOf
 * @param width
 */
void UserLayoutData::changeWidth(qint32 point, QString typeOf, qint32 width)
{
    if(point < mList[0].size()) {
        if(typeOf == mList[0][point].typeOf)
            mList[0][point].width = width;
    }
}

void UserLayoutData::changeHeight(qint32 point, QString typeOf, qint32 height)
{
    if(point < mList[0].size()) {
        if(typeOf == mList[0][point].typeOf)
            mList[0][point].height = height;
    }
}

void UserLayoutData::changeLabel(qint32 point, QString typeOf, QString label)
{
    if(point < mList[0].size()) {
        if(typeOf == mList[0][point].typeOf)
            mList[0][point].label = label;
    }
}

void UserLayoutData::changeValue(qint32 point, QString typeOf, QString value)
{
    if(point < mList[0].size()) {
        if(typeOf == mList[0][point].typeOf)
            mList[0][point].value = value;
    }
}

void UserLayoutData::changeFunction(qint32 point, QString typeOf, QString function)
{
    if(point < mList[0].size()) {
        if(typeOf == mList[0][point].typeOf)
            mList[0][point].function = function;
    }
}

void UserLayoutData::changeRegName(qint32 point, QString typeOf, QString regName)
{
    if(point < mList[0].size()) {
        if(typeOf == mList[0][point].typeOf) {
            mList[0][point].regName = regName;
            emit getSelReg(0, point, regName);
        }
    }
}

void UserLayoutData::changeBckImage(qint32 page, QString path)
{
    bckImagePage[page] = path;
}

void UserLayoutData::changeSeqReg(qint32 page, qint32 point, QString regName)
{
    for(qint32 i = 0; i < seqPoint[page].size(); i++) {
        if(seqPoint[page][i] == point){
            QVector<float> zeroVal;
            for(qint32 j = 0; j < mItems[page].at(seqPoint[page][i]).len.toInt(); j++) {
                zeroVal.append(0);
            }
            mSeqDt[page][i].seqData.clear();
            mSeqDt[page][i].seqData.append(zeroVal);
            mSeqDt[page][i].regName = regName;
        }
    }
}

void UserLayoutData::addSeqRow(qint32 page, qint32 point, QString regName)
{
    for(qint32 i = 0; i < seqPoint[page].size(); i++) {
        if((seqPoint[page][i] == point) && (mList[page].at(seqPoint[page][i]).regName == regName)){
            QVector<float> zeroVal;
            for(qint32 j = 0; j < mItems[page].at(seqPoint[page][i]).len.toInt(); j++) {
                zeroVal.append(0);
            }
            mSeqDt[page][i].seqData.append(zeroVal);
        }
    }
}

void UserLayoutData::delSeqRow(qint32 page, qint32 point, QString regName)
{
    for(qint32 i = 0; i < seqPoint[page].size(); i++) {
        if((seqPoint[page][i] == point) && (mList[page].at(seqPoint[page][i]).regName == regName)){
            mSeqDt[page][i].seqData.removeLast();
        }
    }
}

void UserLayoutData::delSeqAll(qint32 page, qint32 point, QString regName)
{
    for(qint32 i = 0; i < seqPoint[page].size(); i++) {
        if((seqPoint[page][i] == point) && (mList[page].at(seqPoint[page][i]).regName == regName)){
            mSeqDt[page].removeAt(i);
            seqPoint[page].removeAt(i);
        }
    }
}

void UserLayoutData::changeSeqValue(qint32 page, qint32 point, QString regName, qint32 row, qint32 col, QString data)
{
    for(qint32 i = 0; i < seqPoint[page].size(); i++) {
        if((seqPoint[page][i] == point) && (mList[page].at(seqPoint[page][i]).regName == regName)){
            mSeqDt[page][i].seqData[row][col] = data.toFloat();
        }
    }
}

void UserLayoutData::restoreSeqValue(qint32 page, qint32 point, QString regName)
{
    for(qint32 i = 0; i < seqPoint[page].size(); i++) {
        if((seqPoint[page][i] == point) && (mList[page].at(seqPoint[page][i]).regName == regName)){
            for(qint32 j = 0; j<mSeqDt[page][i].seqData.size(); j++) {
                for(qint32 k = 0; k<mSeqDt[page][i].seqData[page].size(); k++) {
                   emit changeSeqValueSignal(point, regName, j, k, QString::number(mSeqDt[page][i].seqData[j][k]));
                }
            }
        }
    }
}

void UserLayoutData::saveSeqToFile(qint32 page, qint32 point, QString regName, QString fileSource)
{
    for(qint32 i = 0; i < seqPoint[page].size(); i++) {
        if((seqPoint[page][i] == point) && (mList[page].at(seqPoint[page][i]).regName == regName)){

            QFile file(fileSource);
            if (file.open(QIODevice::ReadWrite)) {
                QTextStream stream(&file);

                stream << mSeqDt[page][i].regName << ";" << mSeqDt[page][i].seqData.size() << ";" << mSeqDt[page][i].seqData[0].size() << ";" <<  "\n";
               for (int c=0; c < mSeqDt[page][i].seqData.size(); c++ ) {
                  for (int k=0; k < mSeqDt[page][i].seqData[0].size(); k++ ) {
                    stream << mSeqDt[page][i].seqData[c][k] << ";";
                  }
                  stream <<  "\n";
               }
            }
        }
    }

}

void UserLayoutData::loadSeqToFile(qint32 page, qint32 point, QString regName, QString fileSource)
{
    for(qint32 i = 0; i < seqPoint[page].size(); i++) {
        if((seqPoint[page][i] == point) && (mList[page].at(seqPoint[page][i]).regName == regName)){

            QFile file(fileSource);
            if (file.open(QIODevice::ReadWrite)) {

                QString lineFrst = file.readLine();
                QStringList listFrst = lineFrst.split(QLatin1Char(';'));
                qint32 k = 0;

                if((listFrst.at(0) == regName) && (listFrst.at(1).toInt() == mSeqDt[page][i].seqData.size()) && (listFrst.at(2).toInt() == mSeqDt[page][i].seqData[0].size())) {
                    while (!file.atEnd()) {
                        QString line = file.readLine();
                        QStringList list = line.split(QLatin1Char(';'));

                        for(qint32 j = 0; j < mSeqDt[page][i].seqData[0].size(); j++) {
                            mSeqDt[page][i].seqData[k][j] = list.at(j).toFloat();
                        }
                        k++;
                    }
                }

            }
        }
    }
}

void UserLayoutData::loadImageBck(qint32 page, QString path)
{
    bckImagePage[page] = path;
}

/**
 * @brief UserLayoutData::loadPage
 */
void UserLayoutData::loadPage(qint32 page)
{
    for(qint32 i = 0 ; i < mList[page].size(); i++) {
        emit addToLayoutList(mList[page][i].typeOf, mList[page][i].x, mList[page][i].y, mList[page][i].width, mList[page][i].height, mList[page][i].regName, mList[page][i].function, mList[page][i].label, mList[page][i].value, i+1);
    }
    emit layoutComplited();
}

/**
 * @brief UserLayoutData::retSelReg
 * @param point
 * @param item
 */
void UserLayoutData::retSelReg(qint32 page, qint32 point, DataViewItem *item)
{
    if(point < mList[page].size()) {
        mItems[page][point] = *item;
    }
}

/**
 * @brief UserLayoutData::readReg
 * @param point
 */
void UserLayoutData::readReg(qint32 page, qint32 point)
{
    rwFromPage = page;
    if(point < mList[rwFromPage].size()) {
        if(mItems[page].at(point).logicChannel != "") {
            const QRegExp rx(QLatin1Literal("[^0-9]+"));
            const auto&& parts = mItems[page].at(point).logicChannel.split(rx, QString::SkipEmptyParts);
            emit sendRCRS(parts[0].toInt(), mItems[rwFromPage][point].protopNameReg, mItems[rwFromPage][point].regAddress, mItems[rwFromPage][point].nodeId, mItems[rwFromPage][point].uartId, mItems[rwFromPage][point].len, mItems[rwFromPage][point].offset);
        }
    }
}

/**
 * @brief UserLayoutData::writeReg
 * @param point
 * @param data
 */
void UserLayoutData::writeReg(qint32 page, qint32 point, QString data)
{
    rwFromPage = page;
    if(point < mList[rwFromPage].size()) {
        if(mItems[page].at(point).logicChannel != "") {
            if(data == "True" || data == "true" || data == "T")
                data = "1";
            else if(data == "False" || data == "false" || data == "F")
                data = "0";

            mItems[rwFromPage][point].sentValue = data;
            const QRegExp rx(QLatin1Literal("[^0-9]+"));
            const auto&& parts = mItems[page].at(point).logicChannel.split(rx, QString::SkipEmptyParts);
            emit sendSCRS(parts[0].toInt(), mItems[rwFromPage][point].valueType, mItems[rwFromPage][point].protopNameReg, mItems[rwFromPage][point].sentValue, mItems[rwFromPage][point].regAddress, mItems[rwFromPage][point].nodeId, mItems[rwFromPage][point].uartId, mItems[rwFromPage][point].len, mItems[rwFromPage][point].offset, mItems[rwFromPage][point].mulValue);
        }
    }
}

/**
 * @brief UserLayoutData::setAdamReg
 * @param adamPacekt
 */
void UserLayoutData::setAdamReg(DATA_UART_ADAM_PACKET *adamPacekt)
{
    qint32 adamAddrRCRS = adamPacekt->messageCode + ((adamPacekt->serviceCode - 102) << 7U);
    qint32 adamAddrSCRS = adamPacekt->messageCode + ((adamPacekt->serviceCode - 103) << 7U);

    if(adamPacekt->nodeId == 2001)
    {
        for(qint32 i = 0; i < mItems[rwFromPage].size(); i++) {
            if((mItems[rwFromPage][i].regAddress.toInt() == adamAddrRCRS) || (mItems[rwFromPage][i].regAddress.toInt() == adamAddrSCRS))
            {
                //TODO: ALL not working after rapit update.
                emit printReg(i, rwFromPage, adamPacekt->data.toString());
            }
        }
    }
}

/**
 * @brief UserLayoutData::setModBusReg
 * @param modBusPacket
 */
void UserLayoutData::setModBusReg(MODBUS_RX_STRUCT *modBusPacket, QString logicChannel)
{
    QString function = "0";
    float num = 1.0;
    qint32 i32_tumpRegAddr = 0;
    bool status;
    QString tempDataPrintReg = "";


    for(qint32 i = 0; i < mItems[rwFromPage].size(); i++) {

        if(mItems[rwFromPage].at(i).regAddress.contains("x") || mItems[rwFromPage].at(i).regAddress.contains("X")) {
            i32_tumpRegAddr = mItems[rwFromPage].at(i).regAddress.toUInt(&status, 16);
        }
        else {
            i32_tumpRegAddr = (quint16)(mItems[rwFromPage].at(i).regAddress.toUInt());
        }

        if(i32_tumpRegAddr == modBusPacket->regAddr &&
                modBusPacket->regName == mItems[rwFromPage].at(i).protopNameReg &&
                modBusPacket->regOffset == mItems[rwFromPage].at(i).offset) {
            for(qint32 j = 0; j < mList[rwFromPage].size(); j++) {
                if((mItems[rwFromPage][i].protopNameReg == mList[rwFromPage][j].regName) && (mList[rwFromPage][j].typeOf == "ClcIcon")) {
                    function = mList[rwFromPage][j].function;
                    num = mList[rwFromPage][j].value.toFloat();
                    break;
                }
            }
        }
    }

    for(int j = 0; j < modBusPacket->data.size(); j++) {
        if(modBusPacket->dataType == "uint8") {
            quint8 tump = modBusPacket->data.at(j).toUInt();
            tempDataPrintReg += QString::number((quint8)tump);
            tempDataPrintReg += ", ";
        }
        else if(modBusPacket->dataType == "uint16") {
            quint16 tump = modBusPacket->data.at(j).toUInt();
            float tumpF;

            if(function == "0") {
                tempDataPrintReg += QString::number((quint16)tump);
                tempDataPrintReg += ", ";
            }
            else {
                if(function == "/") {
                    tumpF = (float)((float)tump / num);
                    tempDataPrintReg += QString::number((float)tumpF);
                    tempDataPrintReg += ", ";
                }
                else if(function == "*") {
                    tumpF = (float)(tump * num);
                    tempDataPrintReg += QString::number((float)tumpF);
                    tempDataPrintReg += ", ";
                }
                else if(function == "+") {
                    tumpF = (float)(tump + num);
                    tempDataPrintReg += QString::number((float)tumpF);
                    tempDataPrintReg += ", ";
                }
                else if(function == "-") {
                    tumpF = (float)(tump - num);
                    tempDataPrintReg += QString::number((float)tumpF);
                    tempDataPrintReg += ", ";
                }
            }
        }
        else if(modBusPacket->dataType == "uint32") {
            quint32 tump = modBusPacket->data.at(j).toUInt();
            float tumpF;

            if(function == "0") {
                tempDataPrintReg += QString::number((quint32)tump);
                tempDataPrintReg += ", ";
            }
            else {
                if(function == "/") {
                    tumpF = (float)((float)tump / num);
                    tempDataPrintReg += QString::number((float)tumpF);
                    tempDataPrintReg += ", ";
                }
                else if(function == "*") {
                    tumpF = (float)(tump * num);
                    tempDataPrintReg += QString::number((float)tumpF);
                    tempDataPrintReg += ", ";
                }
                else if(function == "+") {
                    tumpF = (float)(tump + num);
                    tempDataPrintReg += QString::number((float)tumpF);
                    tempDataPrintReg += ", ";
                }
                else if(function == "-") {
                    tumpF = (float)(tump - num);
                    tempDataPrintReg += QString::number((float)tumpF);
                    tempDataPrintReg += ", ";
                }
            }
        }
        else if(modBusPacket->dataType == "int8") {
            qint8 tump = modBusPacket->data.at(j).toUInt();
            tempDataPrintReg += QString::number((qint8)tump);
            tempDataPrintReg += ", ";

        }
        else if(modBusPacket->dataType == "int16") {
            qint16 tump = modBusPacket->data.at(j).toUInt();
            tempDataPrintReg += QString::number((qint16)tump);
            tempDataPrintReg += ", ";
        }
        else if(modBusPacket->dataType == "int32") {
            qint32 tump = modBusPacket->data.at(j).toUInt();
            tempDataPrintReg += QString::number((qint32)tump);
            tempDataPrintReg += ", ";
        }
        else if(modBusPacket->dataType == "bitfield") {
            qint32 tump = modBusPacket->data.at(j).toUInt();
            tempDataPrintReg += QString("%1").arg(tump,8, 2,QLatin1Char('0'));
            tempDataPrintReg += ", ";
        }
        else if(modBusPacket->dataType == "bool") {
            qint32 tump = modBusPacket->data.at(j).toUInt();
            if(tump != 0) {
                tempDataPrintReg += "True";
            }
            else {
                tempDataPrintReg += "False";
            }
            tempDataPrintReg += ", ";
        }
        else {
            quint8 tump = modBusPacket->data.at(j).toUInt();
            tempDataPrintReg += QString::number((quint8)tump);
            tempDataPrintReg += ", ";
        }
    }

    for(qint32 i = 0; i < mItems[rwFromPage].size(); i++) {

        if(mItems[rwFromPage].at(i).regAddress.contains("x") || mItems[rwFromPage].at(i).regAddress.contains("X")) {
            i32_tumpRegAddr = mItems[rwFromPage].at(i).regAddress.toUInt(&status, 16);
        }
        else {
            i32_tumpRegAddr = (quint16)(mItems[rwFromPage].at(i).regAddress.toUInt());
        }

        if(i32_tumpRegAddr == modBusPacket->regAddr &&
                modBusPacket->regName == mItems[rwFromPage].at(i).protopNameReg &&
                modBusPacket->regOffset == mItems[rwFromPage].at(i).offset )
        {
            mItems[rwFromPage][i].readValue.clear();
            mItems[rwFromPage][i].readValue = tempDataPrintReg;

            emit printReg(i, rwFromPage, mItems[rwFromPage][i].readValue);

            if(mList[rwFromPage][i].typeOf == "terminal")
                emit printRegName(rwFromPage, modBusPacket->regName);
        }

    }
}

void UserLayoutData::setSCPIReg(QString SCPIdata, QString logicChannel, QString regName)
{

}
//<p style="background-color:tomato;">This is a paragraph.</p>
void UserLayoutData::setDataToTerminal(QByteArray inData, qint32 src)
{
    QString sendString;

    switch (src) {
    case RX: sendString.append("<font color=\"lightgreen\">");
        break;
    case TX: sendString.append("<font color=\"lightblue\">");
        break;
    }

    QString time_format = "HH:mm:ss.zzz";
    QDateTime time = QDateTime::currentDateTime();
    QString timeString = time.toString(time_format);

    sendString.append(timeString + ": ");

    for(qint32 i = 0; i < inData.size(); i++) {

        sendString.append("0x");
        sendString.append(QString("%1").arg((quint8)(inData.at(i)), 2, 16, QLatin1Char( '0' )));
        sendString.append(" ");
    }

    sendString.append("</font>");
    emit printToTerminal(sendString, src);
}

void UserLayoutData::getFunctSlot(qint32 point, qint32 page, QString regName)
{
    emit getFunctSignal(page, point, mList[page].at(point).function, regName);
}

void UserLayoutData::getFunctRegSlot(qint32 page, QString regName, qint32 point)
{
    for(qint32 i = 0; i < mList[page].size(); i++)
    {
        if(regName == mList[page].at(i).regName && mList[page].at(i).typeOf == "ClcIcon") {
            emit getFunctRegSignal(regName, page, mList[page].at(i).function, mList[page].at(i).value.toInt(), point);
        }
    }
}

void UserLayoutData::getDataType(qint32 point, qint32 page, QString regName)
{
    if(point < mItems[page].size()) {
        emit getDataTypeSignal(page, point, regName, mItems[page].at(point).valueType);
    }
}

void UserLayoutData::getNoModifLenSlot(qint32 page, qint32 point, QString regName)
{
    bool containReg = false;

    for(qint32 i = 0; i < mItems[page].size(); i++) {
        if(mItems[page].at(i).protopNameReg == regName) {
            containReg = true;
            emit getNoModifLenSignal(regName, page, point, mItems[page].at(i).len.toUInt());
            break;
        }
    }
    if(mItems[page].size() == 0 || containReg == false) {
        emit getNoModifLenSignal(regName, page, point, 1);
    }
}

void UserLayoutData::getLenSlot(qint32 page, QString regName, qint32 point)
{
    bool emitLen = false;
    for(qint32 i = 0; i < mItems[page].size(); i++) {
        if(mItems[page].at(i).protopNameReg == regName) {
            if(mItems[page].at(i).valueType == "char" ||
                    mItems[page].at(i).valueType == "int8" ||
                    mItems[page].at(i).valueType == "uint8" ||
                    mItems[page].at(i).valueType == "bint8" ||
                    mItems[page].at(i).valueType == "xint8")
            {
                emit getLenSignal(regName, page, mItems[page].at(i).len.toInt()*2, point);
            }
            else if(mItems[page].at(i).valueType == "bool" ||
                    mItems[page].at(i).valueType == "int16" ||
                    mItems[page].at(i).valueType == "uint16" ||
                    mItems[page].at(i).valueType == "xint16" ||
                    mItems[page].at(i).valueType == "bint16") {
                emit getLenSignal(regName, page, mItems[page].at(i).len.toInt(), point);
            }
            else if(mItems[page].at(i).valueType == "int32" ||
                    mItems[page].at(i).valueType == "uint32" ||
                    mItems[page].at(i).valueType == "xint32" ||
                    mItems[page].at(i).valueType == "bint32" ||
                    mItems[page].at(i).valueType == "float")
            {
                emit getLenSignal(regName, page, mItems[page].at(i).len.toInt()/2, point);
            }
            else if(mItems[page].at(i).valueType == "bitfield") {
                if((mItems[page].at(i).len.toInt()%8 == 0))
                    emit getLenSignal(regName, page, (mItems[page].at(i).len.toInt()/8), point);
                else
                    emit getLenSignal(regName, page, (mItems[page].at(i).len.toInt()/8)+1, point);
            }

            emitLen = true;
            break;
        }
    }
    if(emitLen == false)
        emit getLenSignal(regName, page, 1, point);
}

void UserLayoutData::getRangeSlot(qint32 page, QString regName, qint32 point)
{
    for(qint32 i = 0; i < mItems[page].size(); i++) {
        if(mItems[page].at(i).protopNameReg == regName) {
            emit getRangeSignal(regName, page, mItems[page].at(i).minRange.toInt(), mItems[page].at(i).maxRange.toInt(), point);
            break;
        }
    }
}

void UserLayoutData::getBckImageSlot(qint32 page)
{
    emit getBckImageSignal(page, bckImagePage[page]);
}

/**
 * @brief UserLayoutData::saveLayoutItemToFile
 * @param fileSource
 *    QString typeOf;
 */
void UserLayoutData::saveLayoutItemToFile(QString fileSource, qint32 page)
{
    QFile file(fileSource);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);

        stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        stream << "<LayoutData>\n";


       for (int c=0; c < mList[page].size(); c++ ) {
          stream <<  "     <element_" << c <<  ">\n";
          stream <<  "          <typeOf>" << mList[page][c].typeOf << "</typeOf>\n";
          stream <<  "          <regName>" << mList[page][c].regName << "</regName>\n";
          stream <<  "          <label>" << mList[page][c].label << "</label>\n";
          stream <<  "          <function>" << mList[page][c].function << "</function>\n";
          stream <<  "          <width>" << mList[page][c].width << "</width>\n";
          stream <<  "          <height>" << mList[page][c].height << "</height>\n";
          stream <<  "          <x>" << mList[page][c].x << "</x>\n";
          stream <<  "          <y>" << mList[page][c].y << "</y>\n";
          stream <<  "          <value>" << mList[page][c].value << "</value>\n";
          stream <<  "     </element_" << c <<  ">";
          stream << "\n";
       }

       stream << "</LayoutData>\n";
    }
}

/**
 * @brief UserLayoutData::loadLayoutItemToFile
 * @param fileSource
 */
void UserLayoutData::loadLayoutItemToFile(QString fileSource, qint32 page)
{
    qint32 cnt = 0;
    QFile file(fileSource);
    DataViewItem item;
    UserLayoutSeq seq;



    QStringList nameList = fileSource.split('/');
    QStringList nameNoCsv = nameList.last().split('.');
    qInfo() << page;

    if (file.open(QIODevice::ReadWrite)) {
        QXmlStreamReader reader(&file);
        if (reader.readNextStartElement()) {
                if (reader.name() == "LayoutData"){
                    if(page > 0) {
                        emit addLayoutPage(nameNoCsv.at(0), page);
                        _currMaxPage++;
                        _namePage.append(nameNoCsv.at(0));
                    }
                    else {
                        mList[0].clear();
                        mItems[0].clear();
                    }
                    while(reader.readNextStartElement()){
                        if(reader.name().contains("element_")){
                            while(reader.readNextStartElement()){
                                UserLayoutList layoutList;
                                if(reader.name() == "nameReg")
                                    layoutList.typeOf = reader.readElementText();
                                else if(reader.name() == "valueType")
                                    layoutList.regName = reader.readElementText();
                                else if(reader.name() == "regAddress")
                                    layoutList.label = reader.readElementText();
                                else if(reader.name() == "nodeId")
                                    layoutList.function = reader.readElementText();
                                else if(reader.name() == "uartId")
                                    layoutList.width = reader.readElementText().toInt();
                                else if(reader.name() == "groupReg")
                                    layoutList.height = reader.readElementText().toInt();
                                else if(reader.name() == "len")
                                    layoutList.x = reader.readElementText().toInt();
                                else if(reader.name() == "offset")
                                    layoutList.y = reader.readElementText().toInt();
                                else if(reader.name() == "mulValue")
                                    layoutList.value = reader.readElementText();
                                else
                                    reader.skipCurrentElement();

                                mList[page].append(layoutList);
                                mItems[page].append(item);
                                emit getSelReg(page, cnt, layoutList.regName);
                                if(layoutList.typeOf == "seqView") {
                                    seqPoint[page].append(mList[page].size()-1);
                                    mSeqDt[page].append(seq);

                                    changeSeqReg(page, cnt, layoutList.regName);

                                    for(qint32 i = 0; i < layoutList.value.toInt()-1; i++) {
                                        addSeqRow(page, cnt, layoutList.regName);
                                    }
                                }
                                cnt++;
                            }
                        }
                        else
                            reader.skipCurrentElement();
                        }
                    }
                }
                else
                    reader.raiseError(QObject::tr("Incorrect file"));
    }
}

void UserLayoutData::closeAll()
{
    for(qint32 j = 0; j < MAX_NUM_LAYOUT; j++) {
            mList[j].clear();
            mItems[j].clear();
            mSeqDt[j].clear();
            seqPoint[j].clear();
            bckImagePage[j] = "";
            rwFromPage = 0;
    }
}

void UserLayoutData::clearPage0()
{
    mList[0].clear();
    mItems[0].clear();
    mSeqDt[0].clear();
    seqPoint[0].clear();
    bckImagePage[0] = "";
}
