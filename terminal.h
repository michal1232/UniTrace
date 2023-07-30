#ifndef TERMINAL_H
#define TERMINAL_H

#include <QObject>

class terminalT: public QObject
{
    Q_OBJECT
public:
    terminalT();

signals:
    void getRadialCol(qint32 colVal);
    void getTogle(bool time, bool lctx, bool nl, bool echo, bool cr, bool lf, qint32 lcrx);
    void getDataInTerm(QString dataIn);

public slots:
    void setRadialCol(qint32 colVal);
    void setTime(bool time);
    void setLctx(bool lctx);
    void setEcho(bool echo);
    void setNl(bool nl);
    void setCr(bool cr);
    void setLf(bool lf);
    void setLcrx(qint32 lcrx);
    void setDataInTerm(QString dataIn);
    void clearDataInTerm();

    void onTerminalComplete();

private:
   qint32 riadalCol;
   bool time;
   bool lctx;
   bool nl;
   bool echo;
   bool cr;
   bool lf;
   qint32 lcrx;
   QString dataInTerm;
};

#endif // TERMINAL_H
