#ifndef PYTHONSCRIPTING_H
#define PYTHONSCRIPTING_H

#include <QObject>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <QProcess>

class pythonScripting:public QObject
{
    Q_OBJECT
public:
    pythonScripting();

signals:
    void endScriptSuccess(qint32 name, qint32 page, bool status);

public slots:
    void saveToLocalSlot(QString name, QString page, QString script);
};

#endif // PYTHONSCRIPTING_H
