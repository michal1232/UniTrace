#include <QXmlStreamReader>
#include "DataViewList.h"

QVector<DataViewItem>  DataViewList::mItems;
QVector<GroupViewItem> DataViewList::mGroup;
DataViewEn DataViewList::mItemsViewEn = {true, true, true, true, true, true, true, true, true, true, true, true};

/**
 * @brief DataViewList::DataViewList
 * @param parent
 */
DataViewList::DataViewList(QObject *parent) : QObject(parent)
{
}

/**
 * @brief DataViewList::items
 * @return
 */
QVector<DataViewItem> DataViewList::items() const
{
    return mItems;
}

/**
 * @brief DataViewList::setItemAt
 * @param index
 * @param item
 * @return
 */
bool DataViewList::setItemAt(int index, const DataViewItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    //    const DataViewItem &oldItem = mItems.at(index);
    //    if (item.setValue == oldItem.setValue && item.description == oldItem.description && item.reg == oldItem.reg)
    //        return false;

    mItems[index] = item;
    return true;
}

/**
 * @brief DataViewList::delay
 * @param millisecondsToWait
 */
void DataViewList::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

QString DataViewList::getValueTypeP(qint32 inAddrReg, QString nameReg, QString offset)
{
    qint32 i32_tumpRegAddr;
    bool status;


    for(qint32 i = 0; i < mItems.size(); i++) {

        if(mItems.at(i).regAddress.size() > 2) {
            if(mItems.at(i).regAddress.at(1) == 'x' || mItems.at(i).regAddress.at(1) == 'X') {
                i32_tumpRegAddr =  mItems.at(i).regAddress.toUInt(&status, 16);
            }
            else {
                i32_tumpRegAddr = (quint16)(mItems.at(i).regAddress.toUInt());
            }
        }
        else {
            i32_tumpRegAddr = (quint16)(mItems.at(i).regAddress.toUInt());
        }

        if(i32_tumpRegAddr == inAddrReg && nameReg == mItems.at(i).protopNameReg && offset == mItems.at(i).offset ) {
            return mItems[i].valueType;
        }
    }

    return "ERROR_MISS_DATA";
}

void DataViewList::setAllRowData(qint32 i)
{
    emit setItemData(i, mItems[i].setValue, DataViewModel::SetValueRole);
    emit setItemData(i, mItems[i].nameReg, DataViewModel::NameRegRole);
    emit setItemData(i, mItems[i].valueType, DataViewModel::ValueTypeRole);
    emit setItemData(i, mItems[i].readValue, DataViewModel::ReadValueRole);
    emit setItemData(i, mItems[i].sentValue, DataViewModel::SentValueRole);
    emit setItemData(i, mItems[i].regAddress, DataViewModel::RegAddressRole);
    emit setItemData(i, mItems[i].nodeId, DataViewModel::NodeIdRole);
    emit setItemData(i, mItems[i].uartId, DataViewModel::UartIdRole);
    emit setItemData(i, mItems[i].groupReg, DataViewModel::GroupRegRole);
    emit setItemData(i, mItems[i].len, DataViewModel::LenRole);
    emit setItemData(i, mItems[i].offset, DataViewModel::OffsetRole);
    emit setItemData(i, mItems[i].mulValue, DataViewModel::MulValueRole);
    emit setItemData(i, mItems[i].maxRange, DataViewModel::MaxRangeRole);
    emit setItemData(i, mItems[i].minRange, DataViewModel::MinRangeRole);
    emit setItemData(i, mItems[i].rwStatus, DataViewModel::RWStatusRole);
    emit setItemData(i, mItems[i].protocol, DataViewModel::ProtocolRole);
    emit setItemData(i, mItems[i].logicChannel, DataViewModel::LCRole);
    emit setItemData(i, mItems[i].protopNameReg, DataViewModel::ProtopNameRegRole);
    emit setItemData(i, mItems[i].protopGroup, DataViewModel::ProtopGroupRole);
    emit setItemData(i, mItems[i].protopGroup, DataViewModel::ProtopGroupRole);
    emit setItemData(i, mItems[i].defaultValue, DataViewModel::DefaultValueRole);
    emit setItemData(i, mItems[i].description, DataViewModel::DescriptionRole);
    emit setItemData(i, mItems[i].selGroup, DataViewModel::SelGroupRole);
}

void DataViewList::getSetmItemsViewEn(DataViewEn *viewEn, bool get)
{
    if(get)
        viewEn = &mItemsViewEn;
    else {
        mItemsViewEn.protocolEn = viewEn->protocolEn;
        mItemsViewEn.LCEn = viewEn->LCEn;
        mItemsViewEn.nameRegEn = viewEn->nameRegEn;
        mItemsViewEn.valueTypeEn = viewEn->valueTypeEn;
        mItemsViewEn.minRangeEn = viewEn->minRangeEn;
        mItemsViewEn.maxRangeEn = viewEn->maxRangeEn;
        mItemsViewEn.rwStatusEn = viewEn->rwStatusEn;
        mItemsViewEn.uartIdEn = viewEn->uartIdEn;
        mItemsViewEn.lenEn = viewEn->lenEn;
        mItemsViewEn.offsetEn = viewEn->offsetEn;
        mItemsViewEn.nodeIdEn = viewEn->nodeIdEn;
        mItemsViewEn.regAddressEn = viewEn->regAddressEn;
    }

}

DataViewItem *DataViewList::getmItems(QString propRegName)
{
    for (int i = 0; i < mItems.size(); i++) {
        if (mItems.at(i).protopNameReg == propRegName) {
            return &mItems[i];
        }
    }
    return NULL;
}



/**
 * @brief DataViewList::appendItem
 */
void DataViewList::appendItem(QString lc, qint32 row)
{
    static qint32 point;
    static qint32 regaddr;
    static QString uartid;
    static QString groupreg;
    static QString valueType;
    static QString nodeId;
    static QString len;
    static QString protocol;
    static QString offset;
    static QString logc;
    static QString rwStatus;
    static QString min;
    static QString max;

    emit preItemAppended();

    if(mItems.size() > 0) {
        uartid = mItems[row-1].uartId;
        groupreg = mItems[row-1].groupReg;
        valueType = mItems[row-1].valueType;
        nodeId = mItems[row-1].nodeId;
        len = mItems[row-1].len;
        protocol = mItems[row-1].protocol;
        offset = mItems[row-1].offset;
        logc = mItems[row-1].logicChannel;
        rwStatus = mItems[row-1].rwStatus;
        min = mItems[row-1].minRange;
        max = mItems[row-1].maxRange;
        regaddr = mItems[row-1].regAddress.toInt();
        regaddr++;
    }
    else {
        regaddr = 0;
        point = 0;
        uartid = "1";
        groupreg = "Root";
        valueType = "uint8";
        nodeId = "2000";
        len = "1";
        protocol = "ModBus";
        offset = "DO";
        rwStatus = "RW";
        logc = lc;
        min = "";
        max = "";
    }
    DataViewItem item = {false, "", valueType, "", "", QString::number(regaddr), nodeId, uartid, groupreg, len, offset, "1",max,min,rwStatus,protocol,logc,"","","" ,"" , false};

    mItems.append(item);

    emit postItemAppended();
}

/**
 * @brief DataViewList::removeCompletedItems
 */
void DataViewList::removeCompletedItems(bool deleteAll)
{
    for (int i = 0; i < mItems.size(); ) {
        if (mItems.at(i).setValue || deleteAll == true) {
            emit preItemRemoved(i);

            mItems.removeAt(i);

            emit postItemRemoved();
        } else {
            ++i;
        }
    }
}

void DataViewList::upSelItem()
{
    for (int i = 0; i < mItems.size(); i++) {
        if (mItems.at(i).setValue && i > 0) {
            DataViewItem tumpList = mItems.at(i);

            mItems[i] = mItems[i-1];
            mItems[i-1] = tumpList;

            setAllRowData(i);
            setAllRowData(i-1);

            emit refreshRow(i);
            emit refreshRow(i-1);
        }
    }
}

void DataViewList::downSelItem()
{
    for (int i = mItems.size()-1; i >= 0 ; i--) {
        if (mItems.at(i).setValue) {
            DataViewItem tumpList = mItems.at(i);

            mItems[i] = mItems[i+1];
            mItems[i+1] = tumpList;

            setAllRowData(i);
            setAllRowData(i+1);

            emit refreshRow(i);
            emit refreshRow(i+1);
        }
    }
}

/**
 * @brief DataViewList::regroupItem
 */
void DataViewList::regroupItem()
{
    mGroup.clear();
    for(qint32 j = 0; j <mItems.size(); j++)
    {
        GroupViewItem group;
        QStringList splitGroup;
        splitGroup.append(mItems[j].logicChannel);
        splitGroup.append(mItems[j].groupReg.split('.'));
        QStringList sortSplitGroup = splitGroup;
        QStringList sortmGroup;

        qSort(sortSplitGroup);

        qint32 i;
        bool addFlag = true;
        for(i = 0; i < mGroup.size(); i++)
        {
            sortmGroup = mGroup[i].groupVal;
            qSort(sortmGroup);

            if(sortSplitGroup == sortmGroup)
            {
                mGroup[i].groupReg.append(mItems[j].nameReg);
                addFlag = false;
                break;
            }
        }

        if(addFlag == true)
        {
            group = {false,{mItems[j].logicChannel}, {splitGroup}, {mItems[j].nameReg}};
            mGroup.append(group);
        }

        mItems[j].selGroup = true;
    }
    emit setGroup(&mGroup);
}

void DataViewList::editItemViewEn(bool enVal, qint32 num)
{
    switch (num) {
    case 0: mItemsViewEn.protocolEn = enVal;
        break;
    case 1: mItemsViewEn.LCEn = enVal;
        break;
    case 2: mItemsViewEn.nameRegEn = enVal;
        break;
    case 3: mItemsViewEn.valueTypeEn = enVal;
        break;
    case 4: mItemsViewEn.minRangeEn = enVal;
        break;
    case 5: mItemsViewEn.maxRangeEn = enVal;
        break;
    case 6: mItemsViewEn.rwStatusEn = enVal;
        break;
    case 7: mItemsViewEn.uartIdEn = enVal;
        break;
    case 8: mItemsViewEn.lenEn = enVal;
        break;
    case 9: mItemsViewEn.offsetEn = enVal;
        break;
    case 10: mItemsViewEn.nodeIdEn = enVal;
        break;
    case 11: mItemsViewEn.regAddressEn = enVal;
        break;

    }
}

/**
 * @brief DataViewList::editItemSent
 * @param row
 * @param input
 */
void DataViewList::editItemSent(qint32 row, QString input)
{
    if((row >= 0) && (row < mItems.size())) {
        mItems[inReg[row]].sentValue = input;
    }
}

/**
 * @brief DataViewList::editItemSet
 * @param row
 * @param input
 */
void DataViewList::editItemSet(qint32 row, bool input)
{
    if((row >= 0) && (row < mItems.size())) {
        mItems[inReg[row]].setValue = input;
    }
}

void DataViewList::editItemSetAll(bool input)
{
    for(qint32 i = 0; i < inReg.size(); i++) {
        mItems[inReg[i]].setValue = input;
        emit setItemData(inReg[i], input, DataViewModel::SetValueRole);
    }
}

void DataViewList::editItemMulValue(qint32 row, QString input)
{
    if((row >= 0) && (row < inReg.size())) {
        mItems[inReg[row]].mulValue = input;
    }
}

/**
 * @brief DataViewList::editItemNewProject
 * @param row
 * @param column
 * @param input
 */
void DataViewList::editItemNewProject(qint32 row, qint32 column, QString input)
{
    if((row >= 0) && (row < mItems.size())) {

        switch (column) {
        case 1: mItems[row].protocol = input;
            break;
        case 2: mItems[row].logicChannel = input;
            break;
        case 3: mItems[row].groupReg = input;
            break;
        case 5: mItems[row].nameReg = input;
            break;
        case 6: mItems[row].valueType = input;
            break;
        case 7: mItems[row].minRange = input;
            break;
        case 8: mItems[row].maxRange = input;
            break;
        case 9: mItems[row].rwStatus = input;
            break;
        case 10: mItems[row].nodeId = input;
            break;
        case 11: mItems[row].uartId = input;
            break;
        case 12: mItems[row].offset = input;
            break;
        case 13: mItems[row].regAddress = input;
            break;
        case 14: mItems[row].len = input;
            break;
        case 15: mItems[row].defaultValue = input;
            break;
        case 16: mItems[row].description = input;
            break;
        }
    }
}

void DataViewList::editItemProtocolChanged(QString logicChannel, QString protocol)
{
    for(qint32 i = 0; i < mItems.size(); i++) {
        if(mItems[i].logicChannel == logicChannel) {
            mItems[i].protocol = protocol;
        }
    }
}

/**
 * @brief DataViewList::saveItemToFile
 * @param fileSource
 */
void DataViewList::saveItemToFile(QString fileSource)
{
    QFile file(fileSource);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);

        stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        stream << "<ModelData>\n";


       for (int c=0; c < mItems.size(); c++ ) {
          stream <<  "     <reg_" << c <<  ">\n";
          stream <<  "          <nameReg>" << mItems[c].nameReg << "</nameReg>\n";
          stream <<  "          <valueType>" << mItems[c].valueType << "</valueType>\n";
          stream <<  "          <regAddress>" << mItems[c].regAddress << "</regAddress>\n";
          stream <<  "          <nodeId>" << mItems[c].nodeId << "</nodeId>\n";
          stream <<  "          <uartId>" << mItems[c].uartId << "</uartId>\n";
          stream <<  "          <groupReg>" << mItems[c].groupReg << "</groupReg>\n";
          stream <<  "          <len>" << mItems[c].len << "</len>\n";
          stream <<  "          <offset>" << mItems[c].offset << "</offset>\n";
          stream <<  "          <mulValue>" << mItems[c].mulValue << "</mulValue>\n";
          stream <<  "          <minRange>" << mItems[c].minRange << "</minRange>\n";
          stream <<  "          <maxRange>" << mItems[c].maxRange << "</maxRange>\n";
          stream <<  "          <rwStatus>" << mItems[c].rwStatus << "</rwStatus>\n";
          stream <<  "          <protocol>" << mItems[c].protocol << "</protocol>\n";
          stream <<  "          <logicChannel>" << mItems[c].logicChannel << "</logicChannel>\n";
          stream <<  "          <defaultValue>" << mItems[c].defaultValue << "</defaultValue>\n";
          stream <<  "          <description>" << mItems[c].description << "</description>\n";
          stream <<  "     </reg_" << c <<  ">";
          stream << "\n";
       }

       stream << "</ModelData>\n";
    }
}

/**
 * @brief DataViewList::loadItemToFile
 * @param fileSource
 */
void DataViewList::loadItemToFile(QString fileSource, QString logicChannel, QString protocol, bool openProject)
{   
    //mItems.clear();
    QFile file(fileSource);
    DataViewItem data;

    if (file.open(QIODevice::ReadWrite)) {

        QXmlStreamReader reader(&file);
        if (reader.readNextStartElement()) {
                if (reader.name() == "ModelData"){
                    while(reader.readNextStartElement()){
                        if(reader.name().contains("reg_")){
                            while(reader.readNextStartElement()){
                                if(reader.name() == "nameReg")
                                    data.nameReg = reader.readElementText();
                                else if(reader.name() == "valueType")
                                    data.valueType = reader.readElementText();
                                else if(reader.name() == "regAddress")
                                    data.regAddress = reader.readElementText();
                                else if(reader.name() == "nodeId")
                                    data.nodeId = reader.readElementText();
                                else if(reader.name() == "uartId")
                                    data.uartId = reader.readElementText();
                                else if(reader.name() == "groupReg")
                                    data.groupReg = reader.readElementText();
                                else if(reader.name() == "len")
                                    data.len = reader.readElementText();
                                else if(reader.name() == "offset")
                                    data.offset = reader.readElementText();
                                else if(reader.name() == "mulValue")
                                    data.mulValue = reader.readElementText();
                                else if(reader.name() == "minRange")
                                    data.minRange = reader.readElementText();
                                else if(reader.name() == "maxRange")
                                    data.maxRange = reader.readElementText();
                                else if(reader.name() == "rwStatus")
                                    data.rwStatus = reader.readElementText();
                                else if(reader.name() == "protocol")
                                    if(openProject == false) {
                                        reader.skipCurrentElement();
                                        data.protocol = protocol;
                                    }
                                    else
                                        data.protocol = reader.readElementText();
                                else if(reader.name() == "logicChannel") {
                                    if(openProject == false) {
                                        reader.skipCurrentElement();
                                        data.logicChannel = logicChannel;
                                    }
                                    else
                                        data.logicChannel = reader.readElementText();
                                }
                                else if(reader.name() == "defaultValue")
                                    data.defaultValue = reader.readElementText();
                                else if(reader.name() == "description")
                                    data.description = reader.readElementText();
                                else
                                    reader.skipCurrentElement();
                                }
                            data.setValue = false;
                            data.selGroup = false;
                            data.protopNameReg = data.logicChannel+"."+data.nameReg+"."+data.uartId;
                            data.protopGroup = data.logicChannel+"." + data.groupReg+"."+data.nameReg;

                            emit preItemAppended();
                            mItems.append(data);
                            emit postItemAppended();
                            }
                        else
                            reader.skipCurrentElement();
                        }
                    }
                }
                else
                    reader.raiseError(QObject::tr("Incorrect file"));
            }

    regroupItem();
    if(openProject == false) {
        emit setNewDataToLC(logicChannel, protocol);
    }
}

void DataViewList::saveReadData(QString fileSource)
{
    QFile file(fileSource);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);

        stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        stream << "<ReadedData>\n";


       for (int c=0; c < mItems.size(); c++ ) {
          stream <<  "     <reg_" << c <<  ">\n";
          stream <<  "          <protopNameReg>" << mItems[c].protopNameReg << "</protopNameReg>\n";
          stream <<  "          <readValue>" << mItems[c].readValue << "</readValue>\n";
          stream <<  "          <mulValue>" << mItems[c].mulValue << "</mulValue>\n";
          stream <<  "     </reg_" << c <<  ">";
          stream << "\n";
       }

       stream << "</ReadedData>\n";
    }
}

void DataViewList::loadSendData(QString fileSource)
{
    //mItems.clear();
    QFile file(fileSource);
    if (file.open(QIODevice::ReadWrite)) {

        QXmlStreamReader reader(&file);
        if (reader.readNextStartElement()) {
                if (reader.name() == "ReadedData"){
                    while(reader.readNextStartElement()){
                        if(reader.name().contains("reg_")){
                            while(reader.readNextStartElement()){
                                if(reader.name() == "protopNameReg") {
                                    QString protopName = reader.readElementText();
                                    for(int i = 0; i < mItems.size(); i++) {
                                        if(mItems[i].protopNameReg == protopName && mItems[i].rwStatus.contains("W")) {
                                            if(reader.name() == "readValue")
                                                mItems[i].sentValue = reader.readElementText();
                                            else if(reader.name() == "mulValue")
                                                mItems[i].mulValue = reader.readElementText();
                                            else
                                                reader.skipCurrentElement();
                                            emit setItemData(i, mItems[i].sentValue, DataViewModel::SentValueRole);
                                            emit setItemData(i, mItems[i].mulValue, DataViewModel::MulValueRole);
                                            break;
                                        }
                                        reader.skipCurrentElement();
                                        reader.skipCurrentElement();
                                    }
                                }

                                else
                                    reader.skipCurrentElement();
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

/**
 * @brief DataViewList::sendSelected
 */
void DataViewList::sendSelected()
{
    for (int i = 0; i < mItems.size(); i++) {
        if (mItems.at(i).setValue) {

            if(i < inReg.size()) {
                emit selRow(inReg[i]);
            }
            if(mItems.at(i).rwStatus.contains("R")) {
                const QRegExp rx(QLatin1Literal("[^0-9]+"));
                const auto&& parts = mItems[i].logicChannel.split(rx, QString::SkipEmptyParts);
                emit sendRcrsSignal(parts[0].toInt(), mItems[i].protopNameReg,  mItems[i].regAddress, mItems[i].nodeId, mItems[i].uartId, mItems[i].len, mItems[i].offset);
            }
            delay(100);

            if(i < inReg.size()) {
                emit deselRow(inReg[i]);
            }
        }
    }
}

void DataViewList::sendAll()
{
    for (int i = 0; i < mItems.size(); i++) {

        if(i < inReg.size()) {
            emit selRow(inReg[i]);
        }
        //TODO: Send via 0 connection.
        if(mItems.at(i).rwStatus.contains("R")) {
            const QRegExp rx(QLatin1Literal("[^0-9]+"));
            const auto&& parts = mItems[i].logicChannel.split(rx, QString::SkipEmptyParts);
            emit sendRcrsSignal(parts[0].toInt(), mItems[i].protopNameReg,  mItems[i].regAddress, mItems[i].nodeId, mItems[i].uartId, mItems[i].len, mItems[i].offset);
        }
        delay(100);

        if(i < inReg.size()) {
            emit deselRow(inReg[i]);
        }
    }
}

/**
 * @brief DataViewList::writeSelected
 */
void DataViewList::writeSelected()
{
    for (int i = 0; i < mItems.size(); i++) {
        if (mItems.at(i).setValue) {
            presetDataToWrite(i);
        }
    }
}

void DataViewList::writeAll()
{
    for (int i = 0; i < mItems.size(); i++) {
        presetDataToWrite(i);
    }
}

/**
 * @brief DataViewList::saveRequestAdam
 * @param adamPacekt
 */
void DataViewList::saveRequestAdam(DATA_UART_ADAM_PACKET *adamPacekt)
{
    for(int i = 0; i< mItems.size(); i++)
    {
        qint32 addr;
        if(adamPacekt->serviceCode%2 == 0)
            addr = adamPacekt->messageCode + ((adamPacekt->serviceCode - RCRS) << 7U);
        else
            addr = adamPacekt->messageCode + ((adamPacekt->serviceCode - SCRS) << 7U);

        if(mItems.at(i).regAddress.toInt() == addr)
        {
            mItems[i].readValue = adamPacekt->data.toString();
            emit setItemData(i, adamPacekt->data, DataViewModel::ReadValueRole);
        }
    }
}

/**
 * @brief DataViewList::saveRequestModBus
 * @param modBusPacket
 */
void DataViewList::saveRequestModBus(MODBUS_RX_STRUCT *modBusPacket, QString logicChannel)
{
    qint32 i32_tumpRegAddr = 0;
    bool status;

    for(int i = 0; i< mItems.size(); i++)
    {
        if(mItems.at(i).regAddress.contains("x") || mItems.at(i).regAddress.contains("X")) {
            i32_tumpRegAddr = mItems.at(i).regAddress.toUInt(&status, 16);
        }
        else {
            i32_tumpRegAddr = (quint16)(mItems.at(i).regAddress.toUInt());
        }

        if(i32_tumpRegAddr == modBusPacket->regAddr &&
                modBusPacket->regName == mItems[i].protopNameReg &&
                logicChannel == mItems[i].logicChannel &&
                modBusPacket->regOffset == mItems[i].offset )
        {
            mItems[i].readValue.clear();

            for(int j = 0; j < modBusPacket->data.size(); j++) {
                if(mItems[i].mulValue == "1"){
                    if(modBusPacket->dataType == "uint8") {
                        quint8 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint8)tump);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "bint8") {
                        quint8 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint8)tump, 2);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "xint8") {
                        quint8 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint8)tump, 16);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "char") {
                        quint8 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += (char)tump;
                    }
                    else if(modBusPacket->dataType == "uint16") {
                        quint16 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint16)tump);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "bint16") {
                        quint16 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint16)tump, 2);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "xint16") {
                        quint16 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint16)tump, 16);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "uint32") {
                        quint32 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint32)tump);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "bint32") {
                        quint32 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint32)tump, 2);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "xint32") {
                        quint32 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint32)tump, 16);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "int8") {
                        qint8 tump = modBusPacket->data.at(j).toInt();
                        mItems[i].readValue += QString::number((qint8)tump);
                        mItems[i].readValue += ", ";

                    }
                    else if(modBusPacket->dataType == "int16") {
                        qint16 tump = modBusPacket->data.at(j).toInt();
                        mItems[i].readValue += QString::number((qint16)tump);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "int32") {
                        qint32 tump = modBusPacket->data.at(j).toInt();
                        mItems[i].readValue += QString::number((qint32)tump);
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "bitfield") {
                        qint32 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += "0b";
                        mItems[i].readValue += QString("%1").arg(tump,8, 2,QLatin1Char('0'));
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "bool") {
                        qint32 tump = modBusPacket->data.at(j).toUInt();
                        if(tump != 0) {
                            mItems[i].readValue += "True";
                        }
                        else {
                            mItems[i].readValue += "False";
                        }
                    }
                    else {
                        quint8 tump = modBusPacket->data.at(j).toUInt();
                        mItems[i].readValue += QString::number((quint8)tump);
                        mItems[i].readValue += ", ";
                    }
                }
                else {
                    if(modBusPacket->dataType == "uint8" || modBusPacket->dataType == "int8") {
                        float tump = modBusPacket->data.at(j).toFloat();
                        mItems[i].readValue += QString::number(tump/mItems[i].mulValue.toFloat());
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "uint16" || modBusPacket->dataType == "int16") {
                        float tump = modBusPacket->data.at(j).toFloat();
                        mItems[i].readValue += QString::number(tump/mItems[i].mulValue.toFloat());
                        mItems[i].readValue += ", ";
                    }
                    else if(modBusPacket->dataType == "uint32" || modBusPacket->dataType == "int32") {
                        float tump = modBusPacket->data.at(j).toFloat();
                        mItems[i].readValue += QString::number(tump/mItems[i].mulValue.toFloat());
                        mItems[i].readValue += ", ";
                    }
                }

                //                mItems[i].readValue += modBusPacket->data.at(i).toString();
                //                mItems[i].readValue += ", ";
            }

            emit setItemData(i, mItems[i].readValue, DataViewModel::ReadValueRole);
        }
    }
}

void DataViewList::saveRequestSCPI(QString SCPIdata, QString logicChannel, QString regName)
{
    for(int i = 0; i< mItems.size(); i++)
    {
        if(regName == mItems[i].protopNameReg) {
            mItems[i].readValue = SCPIdata;
            emit setItemData(i, mItems[i].readValue, DataViewModel::ReadValueRole);
        }
    }
}

/**
 * @brief DataViewList::setSelItem
 * @param point
 * @param regName
 */
void DataViewList::setSelItem(qint32 page, qint32 point, QString regName)
{
    for (int i = 0; i < mItems.size(); i++) {
        if (mItems.at(i).protopNameReg == regName) {
            emit getSelItem(page, point, &mItems[i]);
        }
    }
}

void DataViewList::getIndexOfReg(QString regAddress)
{
    for(qint32 i =0 ; i < mItems.size(); i++)
    {
        if(mItems[i].regAddress == regAddress)
        {

        }
    }
}

void DataViewList::getValueType(quint32 row)
{
    qInfo() << "1s";
    qInfo() << row;
    QString valueType = mItems.at(row).valueType;

    if(valueType == "-") emit setVauleType(0, row);
    if(valueType == "bool") emit setVauleType(1, row);
    else if(valueType == "int8") emit setVauleType(2, row);
    else if(valueType == "uint8") emit setVauleType(3, row);
    else if(valueType == "bint8") emit setVauleType(4, row);
    else if(valueType == "xint8") emit setVauleType(5, row);
    else if(valueType == "char") emit setVauleType(6, row);
    else if(valueType == "bitfield") emit setVauleType(7, row);
    else if(valueType == "int16") emit setVauleType(8, row);
    else if(valueType == "uint16") emit setVauleType(9, row);
    else if(valueType == "bint16") emit setVauleType(10, row);
    else if(valueType == "xint16") emit setVauleType(11, row);
    else if(valueType == "int32") emit setVauleType(12, row);
    else if(valueType == "uint32") emit setVauleType(13, row);
    else if(valueType == "bint32") emit setVauleType(14, row);
    else if(valueType == "xint32") emit setVauleType(15, row);
    else if(valueType == "float") emit setVauleType(16, row);

}

void DataViewList::getOffset(quint32 row)
{
    qInfo() << "2s";
    qInfo() << row;
    QString valueType = mItems.at(row).offset;

    if(valueType == "DO") emit setOffset(0, row);
    else if(valueType == "DI") emit setOffset(1, row);
    else if(valueType == "AI") emit setOffset(2, row);
    else if(valueType == "AO") emit setOffset(3, row);
    else if(valueType == "-") emit setOffset(4, row);
}

void DataViewList::getRwStatus(qint32 row)
{
    qInfo() << "3s";
    qInfo() << row;
    QString rwStatus = mItems.at(row).rwStatus;

    if(rwStatus == "R") emit setRwStatus(0, row);
    else if(rwStatus == "W") emit setRwStatus(1, row);
    else if(rwStatus == "RW") emit setRwStatus(2, row);
    else if(rwStatus == "-") emit setRwStatus(3, row);
}

void DataViewList::getProtocol(qint32 row)
{
    qInfo() << "4s";
    qInfo() << row;
    QString protocol = mItems.at(row).protocol;

    if(protocol == "ModBus") emit setProtocol(0, row);
    else if(protocol == "Adam") emit setProtocol(1, row);
    else if(protocol == "SCPI") emit setProtocol(2, row);
}

void DataViewList::getLc(qint32 row)
{
    qInfo() << "5s";
    qInfo() << row;
    if(mItems.at(row).logicChannel.size() > 0) {
        QString lc = mItems.at(row).logicChannel.at(mItems.at(row).logicChannel.size()-1);
        emit setLC(lc.toInt(), row);
    }

}

void DataViewList::setPropRegName(qint32 row)
{
    if((row >= 0) && (row < mItems.size()))
        mItems[row].protopNameReg = mItems.at(row).logicChannel+"."+mItems.at(row).nameReg+"."+mItems.at(row).uartId;
}

void DataViewList::setGroupRegName(qint32 row)
{
    if((row >= 0) && (row < mItems.size()))
        mItems[row].protopGroup = mItems.at(row).logicChannel+"." + mItems.at(row).groupReg+"."+mItems.at(row).nameReg;
}

void DataViewList::setSetReg(qint32 row, bool set)
{
    if((row >= 0) && (row < mItems.size()))
        mItems[row].setValue = set;
}

void DataViewList::toggleSet(qint32 row)
{
    emit toggleSetSignal(row, mItems[row].setValue);
}

void DataViewList::setNewDataToLCSlot(QString logicChannel, QString protocol)
{
    emit setNewDataToLC(logicChannel, protocol);
}

void DataViewList::changeProtocolToLCSlot(qint32 logicChannel, QString protocol)
{
    emit changeProtocolToLC(logicChannel, protocol);
}

void DataViewList::getDefaultValueSlot(qint32 row)
{
    emit getDefaultValueSignal(row, mItems[row].defaultValue);
}

void DataViewList::getDescriptionSlot(qint32 row)
{
    emit getDescriptionSignal(row, mItems[row].description);
}

void DataViewList::getLenSlot(qint32 page, QString regName, qint32 point)
{
    bool emitLen = false;
    for(qint32 i = 0; i < mItems.size(); i++) {
        if(mItems.at(i).protopNameReg == regName) {
            if(mItems.at(i).valueType == "char" ||
                    mItems.at(i).valueType == "int8" ||
                    mItems.at(i).valueType == "uint8" ||
                    mItems.at(i).valueType == "bint8" ||
                    mItems.at(i).valueType == "xint8")
            {
                emit getLenSignal(regName, page, mItems.at(i).len.toInt()*2, point);
            }
            else if(mItems.at(i).valueType == "bool" ||
                    mItems.at(i).valueType == "int16" ||
                    mItems.at(i).valueType == "uint16" ||
                    mItems.at(i).valueType == "xint16" ||
                    mItems.at(i).valueType == "bint16") {
                emit getLenSignal(regName, page, mItems.at(i).len.toInt(), point);
            }
            else if(mItems.at(i).valueType == "int32" ||
                    mItems.at(i).valueType == "uint32" ||
                    mItems.at(i).valueType == "xint32" ||
                    mItems.at(i).valueType == "bint32" ||
                    mItems.at(i).valueType == "float")
            {
                emit getLenSignal(regName, page, mItems.at(i).len.toInt()/2, point);
            }
            else if(mItems.at(i).valueType == "bitfield") {
                if((mItems.at(i).len.toInt()%8 == 0))
                    emit getLenSignal(regName, page, (mItems.at(i).len.toInt()/8), point);
                else
                    emit getLenSignal(regName, page, (mItems.at(i).len.toInt()/8)+1, point);
            }

            emitLen = true;
            break;
        }
    }
    if(emitLen == false)
        emit getLenSignal(regName, page, 1, point);
}

void DataViewList::getRangeSlot(qint32 page, QString regName, qint32 point)
{
    for(qint32 i = 0; i < mItems.size(); i++) {
        if(mItems.at(i).protopNameReg == regName) {
            emit getRangeSignal(regName, page, mItems.at(i).minRange.toInt(), mItems.at(i).maxRange.toInt(), point);
            break;
        }
    }
}

void DataViewList::getmItemsEnViewSlot()
{
    emit getmItemsEnViewSignal(mItemsViewEn.protocolEn,
                               mItemsViewEn.LCEn,
                               mItemsViewEn.nameRegEn,
                               mItemsViewEn.valueTypeEn,
                               mItemsViewEn.minRangeEn,
                               mItemsViewEn.maxRangeEn,
                               mItemsViewEn.rwStatusEn,
                               mItemsViewEn.uartIdEn,
                               mItemsViewEn.lenEn,
                               mItemsViewEn.offsetEn,
                               mItemsViewEn.nodeIdEn,
                               mItemsViewEn.regAddressEn);
}

void DataViewList::rowChanged(qint32 position, qint32 value)
{
    inReg.insert(position, value);
}

void DataViewList::rowDeleted(qint32 position)
{
    inReg.removeAt(position);
}

void DataViewList::getIdReg(QString logicChannel, QString nameReg)
{
    QStringList idList;
    for(qint32 i = 0; i< mItems.size(); i++) {
        if(mItems[i].logicChannel == logicChannel && mItems[i].nameReg == nameReg) {
            idList.append(mItems[i].uartId);
        }
    }

    qInfo() << idList;
    emit getIdRegSignal(idList);
}

void DataViewList::presetDataToWrite(qint32 num)
{
    qInfo() << "6";
    if(num < inReg.size()) {
        emit selRow(inReg[num]);
    }
    if(mItems.at(num).protocol == "ADAM") {
        if(mItems.at(num).rwStatus.contains("W") && mItems[num].sentValue != ""){
            const QRegExp rx(QLatin1Literal("[^0-9]+"));
            const auto&& parts = mItems[num].logicChannel.split(rx, QString::SkipEmptyParts);
            emit sendScrsSignal(parts[0].toInt(),
                    mItems[num].valueType,
                    mItems[num].protopNameReg,
                    mItems[num].sentValue,
                    mItems[num].regAddress,
                    mItems[num].nodeId,
                    mItems[num].uartId,
                    mItems[num].len,
                    mItems[num].offset,
                    mItems[num].mulValue);
        }
    }
    else if(mItems.at(num).protocol == "ModBus") {
        if(mItems.at(num).rwStatus.contains("W") && mItems[num].sentValue != ""){
            QString sentValue;

            while(mItems[num].sentValue[mItems[num].sentValue.size()-1] == " " || mItems[num].sentValue[mItems[num].sentValue.size()-1] == ",") {
                mItems[num].sentValue.remove(mItems[num].sentValue.size()-1,1);
            }

            if(mItems[num].sentValue == "True"|| mItems[num].sentValue == "true"|| mItems[num].sentValue == "T") {
                sentValue = "1";
            }
            else if (mItems[num].sentValue == "False"|| mItems[num].sentValue == "false"|| mItems[num].sentValue == "F") {
                sentValue = "0";
            }
            else if(mItems[num].sentValue.contains("b")) {
                QStringList listAllValue = mItems[num].sentValue.split(",");
                for(qint32 i = 0; i < listAllValue.size(); i++) {
                    QStringList listSentValue = listAllValue[i].split("b");
                    bool status;
                    quint8 uintTump = listSentValue[1].toInt(&status,2);
                    sentValue += QString::number(uintTump);
                    sentValue += ",";
                }

                if(sentValue[sentValue.size()-1] == ",") {
                    sentValue.remove(sentValue.size()-1, 1);
                }

            }
            else {
                sentValue = mItems[num].sentValue;
            }

            QStringList listAllValue = sentValue.split(",");
            bool canBySend = true;
            for(qint32 i = 0; i < listAllValue.size(); i++) {
                if(listAllValue[i] <= mItems[i].minRange && listAllValue[i] >= mItems[i].maxRange) {
                    canBySend = false;
                }
            }

            if(canBySend == true) {
                const QRegExp rx(QLatin1Literal("[^0-9]+"));
                const auto&& parts = mItems[num].logicChannel.split(rx, QString::SkipEmptyParts);
                emit sendScrsSignal(parts[0].toInt(),
                        mItems[num].valueType,
                        mItems[num].protopNameReg,
                        sentValue,
                        mItems[num].regAddress,
                        mItems[num].nodeId,
                        mItems[num].uartId,
                        mItems[num].len,
                        mItems[num].offset,
                        mItems[num].mulValue);
            }
        }
    }
    else if(mItems.at(num).protocol == "SCPI") {
        const QRegExp rx(QLatin1Literal("[^0-9]+"));
        const auto&& parts = mItems[num].logicChannel.split(rx, QString::SkipEmptyParts);
        emit sendScrsSignal(parts[0].toInt(),
                mItems[num].valueType,
                mItems[num].protopNameReg,
                mItems[num].sentValue,
                mItems[num].regAddress,
                mItems[num].nodeId,
                mItems[num].uartId,
                mItems[num].len,
                mItems[num].offset,
                mItems[num].mulValue);
    }
    delay(100);
    if(num < inReg.size()) {
        emit deselRow(inReg[num]);
    }
}




