#include "adamprotocol.h"

/**
 * @brief AdamProtocol::AdamProtocol
 */
AdamProtocol::AdamProtocol()
{

}

/**
 * @brief AdamProtocol::sendAdamSCRS
 * @param valueType
 * @param sentValue
 * @param regAddress
 * @param nodeId
 * @param uartId
 * @return
 */
QString AdamProtocol::sendAdamSCRS(QString valueType, QString sentValue, QString regAddress, QString nodeId, QString uartId)
{
    QString len = "00";

    qint32 nodeIdRecalc = nodeId.toInt() * 32;
    QString nodeIdResult = QString("%1").arg(nodeIdRecalc, 4, 16, QLatin1Char( '0' ));
    nodeIdResult = nodeIdResult.toUpper();

    qint32 uartIdRecalc = uartId.toInt();
    QString uartIdResult = QString("%1").arg(uartIdRecalc, 2, 16, QLatin1Char( '0' ));
    uartIdResult = uartIdResult.toUpper();

    QString valueTypeResult = "00";

    qint32 i32_send_value;
    quint32 ui32_send_value;
    float f_send_value;
    QString sendValueResult;

    if(valueType == "uint8") {
        valueTypeResult = "0A";
        len = "05";
        i32_send_value = sentValue.toInt();
        sendValueResult = QString("%1").arg(i32_send_value, 2, 16, QLatin1Char( '0' ));
    }
    else if(valueType == "uint16") {
        valueTypeResult = "07";
        len = "06";
        i32_send_value = sentValue.toInt();
        sendValueResult = QString("%1").arg(i32_send_value, 4, 16, QLatin1Char( '0' ));
    }
    else if(valueType == "uint32") {
        valueTypeResult = "04";
        len = "08";
        i32_send_value = sentValue.toInt();
        sendValueResult = QString("%1").arg(i32_send_value, 8, 16, QLatin1Char( '0' ));
    }
    else if(valueType == "int8") {
        valueTypeResult = "09";
        len = "05";
        ui32_send_value = sentValue.toUInt();
        sendValueResult = QString("%1").arg(ui32_send_value, 2, 16, QLatin1Char( '0' ));
    }
    else if(valueType == "int16") {
        valueTypeResult = "06";
        len = "06";
        ui32_send_value = sentValue.toUInt();
        sendValueResult = QString("%1").arg(ui32_send_value, 4, 16, QLatin1Char( '0' ));
    }
    else if(valueType == "int32") {
        valueTypeResult = "03";
        len = "08";
        ui32_send_value = sentValue.toUInt();
        sendValueResult = QString("%1").arg(ui32_send_value, 8, 16, QLatin1Char( '0' ));
    }
    else if(valueType == "float") {
        valueTypeResult = "02";
        len = "08";
        f_send_value = sentValue.toFloat();
//TODO: Not implemented
    }

    qint32 regAddressRecalc = regAddress.toInt();
    qint32 serviceRecalc = 103 + ((regAddressRecalc & 0xff00U) >> 7);
    qint32 msgRecalc = regAddressRecalc & 0x00ffU;
    QString serviceResult = QString("%1").arg(serviceRecalc, 2, 16, QLatin1Char( '0' ));
    QString regAddressResult = QString("%1").arg(msgRecalc, 2, 16, QLatin1Char( '0' ));
    serviceResult = serviceResult.toUpper();
    regAddressResult = regAddressResult.toUpper();

    QString sendString = len + nodeIdResult + uartIdResult + valueTypeResult + serviceResult + regAddressResult + sendValueResult;

    return sendString;
}

/**
 * @brief AdamProtocol::sendAdamRCRS
 * @param regAddress
 * @param nodeId
 * @param uartId
 * @return
 */
QString AdamProtocol::sendAdamRCRS(QString regAddress, QString nodeId, QString uartId)
{
    QString len = "04";

    qint32 nodeIdRecalc = nodeId.toInt() * 32;
    QString nodeIdResult = QString("%1").arg(nodeIdRecalc, 4, 16, QLatin1Char( '0' ));
    nodeIdResult = nodeIdResult.toUpper();

    qint32 uartIdRecalc = uartId.toInt();
    QString uartIdResult = QString("%1").arg(uartIdRecalc, 2, 16, QLatin1Char( '0' ));
    uartIdResult = uartIdResult.toUpper();

    QString valueTypeResult = "00";

    qint32 regAddressRecalc = regAddress.toInt();
    qint32 serviceRecalc = 102 + ((regAddressRecalc & 0xff00U) >> 7);
    qint32 msgRecalc = regAddressRecalc & 0x00ffU;
    QString serviceResult = QString("%1").arg(serviceRecalc, 2, 16, QLatin1Char( '0' ));
    QString regAddressResult = QString("%1").arg(msgRecalc, 2, 16, QLatin1Char( '0' ));
    serviceResult = serviceResult.toUpper();
    regAddressResult = regAddressResult.toUpper();

    QString sendString = len + nodeIdResult + uartIdResult + valueTypeResult + serviceResult + regAddressResult;

    return sendString;
}

/**
 * @brief AdamProtocol::decodeAdamRxData
 * @param rxSerialData
 * @return
 */
DATA_UART_ADAM_PACKET AdamProtocol::decodeAdamRxData(QString rxSerialData)
{
    bool status;

    adamString.s_len = rxSerialData.at(0);
    adamString.s_len.append(rxSerialData.at(1));

    adamData.len = adamString.s_len.toUInt(&status, 16);

    adamString.s_nodeId = rxSerialData.at(2);
    adamString.s_nodeId .append(rxSerialData.at(3));
    adamString.s_nodeId .append(rxSerialData.at(4));
    adamString.s_nodeId .append(rxSerialData.at(5));

    adamData.nodeId = adamString.s_nodeId.toUInt(&status, 16) / 32;

    /* Emergency Event Data (EED) */
    if (adamData.nodeId < 128)
       receivedEED(adamData.nodeId, adamData.len, rxSerialData.mid(6));
    else
       /* High Priority Node Service Data (NSH) */
       if (adamData.nodeId < 200)
          receivedNSH(adamData.nodeId, adamData.len, rxSerialData.mid(6));
       else
          /* High Priority User-Defined Data Types (UDH) */
          if (adamData.nodeId < 300)
             receivedUDH(adamData.nodeId, adamData.len, rxSerialData.mid(6));
          else
             /* Normal Operation Data (NOD) */
             if (adamData.nodeId < 1800)
                receivedNOD(adamData.nodeId, adamData.len, rxSerialData.mid(6));
             else
                /* Low Priority User-Defined Data Types (UDL) */
                if (adamData.nodeId < 1900)
                   receivedUDL(adamData.nodeId, adamData.len, rxSerialData.mid(6));
                else
                   /* Debug Service Data (DSD) */
                   if (adamData.nodeId < 2000)
                      receivedDSD(adamData.nodeId, adamData.len, rxSerialData.mid(6));
                   else
                      /* Low Priority Node Service Data (NSL) */
                      receivedNSL(adamData.nodeId, adamData.len, rxSerialData.mid(6));

    adamString.s_uartId.clear();
    adamString.s_data.clear();
    adamString.s_dataType.clear();
    adamString.s_len.clear();
    adamString.s_nodeId.clear();
    adamString.s_serviceCode.clear();
    adamString.s_messageCode.clear();

    return adamData;
}

/**
 * @brief AdamProtocol::receivedEED
 * @param id
 * @param len
 * @param frame
 */
void AdamProtocol::receivedEED(qint32 id, qint32 len, QString frame)
{

}

/**
 * @brief AdamProtocol::receivedNSH
 * @param id
 * @param len
 * @param frame
 */
void AdamProtocol::receivedNSH(qint32 id, qint32 len, QString frame)
{

}

/**
 * @brief AdamProtocol::receivedUDH
 * @param id
 * @param len
 * @param frame
 */
void AdamProtocol::receivedUDH(qint32 id, qint32 len, QString frame)
{

}

/**
 * @brief AdamProtocol::receivedNOD
 * @param id
 * @param len
 * @param frame
 */
void AdamProtocol::receivedNOD(qint32 id, qint32 len, QString frame)
{

}

/**
 * @brief AdamProtocol::receivedUDL
 * @param id
 * @param len
 * @param frame
 */
void AdamProtocol::receivedUDL(qint32 id, qint32 len, QString frame)
{

}

/**
 * @brief AdamProtocol::receivedDSD
 * @param id
 * @param len
 * @param frame
 */
void AdamProtocol::receivedDSD(qint32 id, qint32 len, QString frame)
{

}

/**
 * @brief AdamProtocol::receivedNSL
 * @param id
 * @param len
 * @param frame
 */
void AdamProtocol::receivedNSL(qint32 id, qint32 len, QString frame)
{
    bool status;

    adamString.s_uartId = frame.at(0);
    adamString.s_uartId.append(frame.at(1));
    adamData.uartId = adamString.s_uartId.toUInt(&status, 16);

    adamString.s_dataType = frame.at(2);
    adamString.s_dataType.append(frame.at(3));
    adamData.dataType = adamString.s_dataType.toUInt(&status, 16);

    adamString.s_serviceCode = frame.at(4);
    adamString.s_serviceCode.append(frame.at(5));
    adamData.serviceCode = adamString.s_serviceCode.toUInt(&status, 16);

    adamString.s_messageCode = frame.at(6);
    adamString.s_messageCode.append(frame.at(7));
    adamData.messageCode = adamString.s_messageCode.toUInt(&status, 16);

    qint32 cnt = 0;

    for(qint32 i = 0; i < (adamData.len -4); i++) {
        adamString.s_data.append(frame.at(8+cnt));
        cnt++;
        adamString.s_data.append(frame.at(8+cnt));
        cnt++;
    }

    quint32 word;
    float *f;

    switch (adamData.dataType) {
    case AS_FLOAT:
        word = adamString.s_data.toUInt(&status,16);
        f = reinterpret_cast<float *>(&word);
        adamData.data = *f;
        break;
    case AS_LONG:
    case AS_SHORT:
    case AS_CHAR:
        adamData.data = adamString.s_data.toInt(&status, 16);
        break;
    case AS_ULONG:
    case AS_USHORT:
    case AS_UCHAR:
        adamData.data = adamString.s_data.toUInt(&status, 16);
        break;
    }
}
