#include "terminal.h"

terminalT::terminalT()
{

}

void terminalT::setRadialCol(qint32 colVal)
{
    riadalCol = colVal;
}

void terminalT::setTime(bool time)
{
    this->time = time;
}

void terminalT::setLctx(bool lctx)
{
    this->lctx = lctx;
}

void terminalT::setEcho(bool echo)
{
   this->echo = echo;
}

void terminalT::setNl(bool nl)
{
    this->nl = nl;
}

void terminalT::setCr(bool cr)
{
    this->cr = cr;
}

void terminalT::setLf(bool lf)
{
    this->lf = lf;
}

void terminalT::setLcrx(qint32 lcrx)
{
    this->lcrx = lcrx;
}

void terminalT::setDataInTerm(QString dataIn)
{
    QString noTimeData = dataIn;
    QString noNlData;

    if(riadalCol == 0) {
        dataInTerm += dataIn;
    }
    else if(riadalCol == 1) {

        if(time == true) {
            noTimeData = "";
            noTimeData = dataIn.mid(14,dataIn.size());
            dataInTerm += dataIn.mid(0,14);
        }

        if(lctx == true) {
            dataInTerm += noTimeData.mid(0, 6);
            noTimeData = noTimeData.mid(6, noTimeData.size());
        }

        if(nl == true) {
            noNlData = noTimeData.remove(0, -1);
            noTimeData = noNlData;
        }

        QByteArray numText = noTimeData.toLatin1();
        dataInTerm += numText.toHex(' ');
        dataInTerm += " ";

        if(nl == true) {
            dataInTerm += "\n";
        }
    }
    else if(riadalCol == 2) {

        if(time == true) {
            noTimeData = "";
            noTimeData = dataIn.mid(14,dataIn.size());
            dataInTerm += dataIn.mid(0,14);
        }

        if(lctx == true) {
            dataInTerm += noTimeData.mid(0, 6);
            noTimeData = noTimeData.mid(6, noTimeData.size());
        }

        if(nl == true) {
            noNlData = noTimeData.remove(0, -1);
            noTimeData = noNlData;
        }

        QByteArray numText = noTimeData.toLatin1();
        for(qint32 i = 0; i < numText.size() ; i++) {
            dataInTerm += QString::number(numText[i]);
            dataInTerm += " ";
        }

        if(nl == true) {
            dataInTerm += "\n";
        }
    }

    emit getDataInTerm(dataInTerm);
}

void terminalT::clearDataInTerm()
{
    dataInTerm = "";
}

void terminalT::onTerminalComplete()
{
    emit getRadialCol(riadalCol);
    emit getTogle(time, lctx, nl, echo, cr, lf, lcrx);
    emit getDataInTerm(dataInTerm);
}
