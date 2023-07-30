#ifndef SCPI_PROTOCOL_H
#define SCPI_PROTOCOL_H

#include <QObject>

class SCPI_Protocol: public QObject
{
    Q_OBJECT
public:
    SCPI_Protocol();

    QString sendSCPImsg(QString regNameProtop, QString sendData);
};

#endif // SCPI_PROTOCOL_H
