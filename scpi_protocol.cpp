#include "scpi_protocol.h"

SCPI_Protocol::SCPI_Protocol()
{

}

QString SCPI_Protocol::sendSCPImsg(QString regNameProtop, QString sendData)
{
    QStringList regList;
    QStringList splitSendData;

    regList = regNameProtop.split(".");

    if(sendData != "") {
        splitSendData = sendData.split(",");
        for(int i = 0; i < splitSendData.length(); i++) {
            regList[1] = regList[1].left(regList[1].indexOf("<")) + splitSendData[i] + regList[1].right(regList[1].size() - regList[1].indexOf(">")-1);
        }
    }

    for(int i = 0; i < regList[1].size(); i++) {
        if(regList[1][i].isLower()) {
            regList[1].remove(i,1);
            i--;
        }
    }

    return regList[1];
}
