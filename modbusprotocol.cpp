#include "modbusprotocol.h"

MODBUS_DATA_STRUCT ModBusProtocol::modBusData;

ModBusProtocol::ModBusProtocol()
{

}

QByteArray ModBusProtocol::sendModBusRCRS(QString regName, QString regAddress, QString uartId, QString len, QString offset)
{
    QByteArray retVal;

    quint8 offsetVal;

    if(offset == "DO")
        offsetVal = 1;
    else if(offset == "DI")
        offsetVal = 2;
    else if(offset == "AO")
        offsetVal = 3;
    else if(offset == "AI")
        offsetVal = 4;
    else
        offsetVal = 255;


    modBusData.regName = regName;
    modBusData.slaveID = (quint8)(uartId.toUInt());
    modBusData.functionCode = offsetVal;

    bool status;

    if(regAddress.contains("x") || regAddress.contains("X")) {
        modBusData.registerAddr = regAddress.toUInt(&status, 16);
    }
    else {
        modBusData.registerAddr = (quint16)(regAddress.toUInt());
    }

    modBusData.numRegister = (quint16)(len.toUInt());

    modBusData.crc = 0xFFFF;
    modBusData.crc = crc16Update(modBusData.crc, modBusData.slaveID);

    modBusData.crc = crc16Update(modBusData.crc, modBusData.functionCode);

    modBusData.crc = crc16Update(modBusData.crc, ((modBusData.registerAddr>>8)&0xFF));
    modBusData.crc = crc16Update(modBusData.crc, (modBusData.registerAddr&0xFF));

    modBusData.crc = crc16Update(modBusData.crc, ((modBusData.numRegister>>8)&0xFF));
    modBusData.crc = crc16Update(modBusData.crc, (modBusData.numRegister&0xFF));

    retVal.append((char)modBusData.slaveID);
    retVal.append((char)modBusData.functionCode);
    retVal.append((char)((modBusData.registerAddr>>8)&0xFF));
    retVal.append((char)(modBusData.registerAddr&0xFF));
    retVal.append((char)((modBusData.numRegister>>8)&0xFF));
    retVal.append((char)(modBusData.numRegister&0xFF));
    retVal.append((char)(modBusData.crc&0xFF));
    retVal.append((char)((modBusData.crc>>8)&0xFF));

    return retVal;
}

MODBUS_RX_STRUCT ModBusProtocol::rxModBus(QByteArray byteArray)
{
    MODBUS_DATA_STRUCT rxData;
    MODBUS_RX_STRUCT retData;
    DataViewList list;

    QString valueType;
    quint16 crc = 0xFFFF;
    quint16 rot_crr = 0;

    retData.status = true;

    for(qint32 i = 0; i < byteArray.size()-2; i++) {
        crc = crc16Update(crc, byteArray[i]);
    }
    rot_crr = ((crc>>8)&0xFF) | ((crc<<8)&0xFF00);

    quint8 crcHL;
    quint8 crcLL;
    quint8 len;
    quint8 offset = 0;

    rxData.slaveID = byteArray[0];
    rxData.functionCode = byteArray[1];

    if((rxData.functionCode == 5) || (rxData.functionCode == 6)) {
        crcHL = byteArray[6];
        crcLL = byteArray[7];
        offset = 4;
        rxData.numRegister = 1;
    }
    else {
        len = byteArray[2];
        crcHL = byteArray[3+len];
        crcLL = byteArray[4+len];
        offset = 3;
        rxData.numRegister = byteArray[2];
    }

    if((((rot_crr>>8)&0xFF) == crcHL) && ((rot_crr&0xFF) == crcLL))
    {
        if(rxData.functionCode == 1 || rxData.functionCode == 5 || rxData.functionCode == 15)
            retData.regOffset = "DO";
        else if(rxData.functionCode == 2)
            retData.regOffset = "DI";
        else if(rxData.functionCode == 3 || rxData.functionCode == 6 || rxData.functionCode == 16)
            retData.regOffset = "AO";
        else if(rxData.functionCode == 4)
            retData.regOffset = "AI";

        valueType = list.getValueTypeP(modBusData.registerAddr, modBusData.regName, retData.regOffset);

        if(modBusData.slaveID == byteArray[0])
        {
            if(valueType != "ERROR_MISS_DATA")
            {
                if(valueType == "uint8" || valueType == "int8" || valueType == "bint8" || valueType == "xint8"|| valueType == "char"|| valueType == "bitfield" || (valueType == "bool" && (rxData.functionCode == 1 || rxData.functionCode == 2 || rxData.functionCode == 5))) {
                    for(qint32 i = 0; i < rxData.numRegister; i++) {
                       rxData.data.append(byteArray.at(offset+i));
                    }
                }
                else if(valueType == "uint16" || valueType == "int16"|| valueType == "bint16"|| valueType == "xint16" || (valueType == "bool" && rxData.functionCode == 5)) {
                    for(qint32 i = 0; i < (rxData.numRegister); i+=2) {
                       rxData.data.append(((byteArray.at(offset+i)<<8)&0xFF00) | (byteArray.at(offset+i+1)&0xFF));
                    }
                }
                else if(valueType == "uint32" || valueType == "int32"|| valueType == "bint32"|| valueType == "xint32" || valueType == "float") {
                    for(qint32 i = 0; i < (rxData.numRegister); i+=4) {
                       rxData.data.append(((byteArray.at(offset+i)<<24)&0xFF000000) |
                                           ((byteArray.at(offset+i)<<16)&0xFF0000) |
                                           ((byteArray.at(offset+i)<<8)&0xFF00) |
                                           (byteArray.at(offset+i+1)&0xFF));
                    }
                }
            }
            else {
                retData.status = false;
            }
        }
    }
    else {
        retData.status = false;
    }

    retData.regNode = modBusData.slaveID;
    retData.regName = modBusData.regName;
    retData.data = rxData.data;
    retData.dataType = valueType;
    retData.regAddr = modBusData.registerAddr;

    return retData;
}

QByteArray ModBusProtocol::sendModBusSCRS(QString regName, QString sentValue, QString regAddress, QString uartId, QString len, QString offset, QString mulValue)
{
    QByteArray retVal;
    bool status;

    modBusData.data.clear();
    modBusData.numBytes = 0;

    modBusData.regName = regName;
    modBusData.slaveID = (quint8)(uartId.toUInt());

    modBusData.slaveID = (quint8)(uartId.toUInt());

    if(regAddress.contains("x") || regAddress.contains("X")) {
        modBusData.registerAddr = regAddress.toUInt(&status, 16);
    }
    else {
        modBusData.registerAddr = (quint16)(regAddress.toUInt());
    }

    modBusData.numRegister = (quint16)(len.toUInt());

    if(offset == "DO") {
        if(len.toUInt() == 1) {
            modBusData.functionCode = 5;

            if(sentValue.toUInt() == 0)
                modBusData.data.append(0x0000);
            else
                modBusData.data.append(0xFF00);
        }
        else {
            modBusData.functionCode = 0xF;
            QStringList sentList = sentValue.split(",");

            for(qint32 i = 0; i< sentList.size(); i++) {
                modBusData.numBytes++;
                modBusData.data.append(sentList.at(i).toUInt());
            }
        }
    }
    else if(offset == "AO") {
        if(len.toUInt() == 1) {
            modBusData.functionCode = 0x06;
            modBusData.data.append((quint16)(sentValue.toFloat() *mulValue.toFloat()));
        }
        else {
            modBusData.functionCode = 0x10;

            QStringList sentList = sentValue.split(",");

            float iMulValue = mulValue.toFloat();

            for(qint32 i = 0; i< sentList.size(); i++) {
                modBusData.numBytes += 2;
                modBusData.data.append((quint16)(sentList.at(i).toFloat()*iMulValue));
            }
        }
    }

    modBusData.crc = 0xFFFF;
    modBusData.crc = crc16Update(modBusData.crc, modBusData.slaveID);

    modBusData.crc = crc16Update(modBusData.crc, modBusData.functionCode);

    modBusData.crc = crc16Update(modBusData.crc, ((modBusData.registerAddr>>8)&0xFF));
    modBusData.crc = crc16Update(modBusData.crc, (modBusData.registerAddr&0xFF));

    if(len.toUInt() > 1) {
        modBusData.crc = crc16Update(modBusData.crc, ((modBusData.numRegister>>8)&0xFF));
        modBusData.crc = crc16Update(modBusData.crc, (modBusData.numRegister&0xFF));

        modBusData.crc = crc16Update(modBusData.crc, modBusData.numBytes);
    }

    for(qint32 i = 0; i< modBusData.data.size(); i++) {
        modBusData.crc = crc16Update(modBusData.crc, ((modBusData.data.at(i).toUInt()>>8)&0xFF));
        modBusData.crc = crc16Update(modBusData.crc, (modBusData.data.at(i).toUInt()&0xFF));
    }

    retVal.append((char)modBusData.slaveID);
    retVal.append((char)modBusData.functionCode);
    retVal.append((char)((modBusData.registerAddr>>8)&0xFF));
    retVal.append((char)(modBusData.registerAddr&0xFF));

    if(len.toUInt() > 1) {
        retVal.append((char)((modBusData.numRegister>>8)&0xFF));
        retVal.append((char)(modBusData.numRegister&0xFF));
        retVal.append((char)modBusData.numBytes);
    }

    for(qint32 i = 0; i< modBusData.data.size(); i++) {
        retVal.append((char)((modBusData.data.at(i).toUInt()>>8)&0xFF));
        retVal.append((char)(modBusData.data.at(i).toUInt()&0xFF));
    }
    retVal.append((char)(modBusData.crc&0xFF));
    retVal.append((char)((modBusData.crc>>8)&0xFF));

    return  retVal;
}

quint16 ModBusProtocol::crc16Update(quint16 crc, quint8 dataIn) {
    int i;

    crc ^= (quint16)dataIn;
    for (i = 0; i < 8; ++i) {
        if (crc & 1)
            crc = (crc >> 1) ^ 0xA001;
        else
            crc = (crc >> 1);
    }

    return crc;
}
