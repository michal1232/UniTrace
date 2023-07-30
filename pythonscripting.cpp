#include "pythonscripting.h"
#include "QDebug"

pythonScripting::pythonScripting()
{

}

void pythonScripting::saveToLocalSlot(QString name, QString page, QString script)
{
    QDir dir(".");

    QStringList appFileList = dir.absolutePath().split("uni-trace.app/");
    QString appFile = appFileList.at(0);
    QStringList scriptPath;
    scriptPath.append( (appFile + page + "_" + name + ".py"));

    QFile file(scriptPath[0]);

    file.open(QIODevice::WriteOnly);
    file.resize(0);

    QTextStream stream(&file);
    stream << script;
    file.close();

    QProcess p;
    p.start("python",scriptPath);
    bool status = p.waitForFinished();

    qInfo() << p.readAll();

    emit endScriptSuccess(name.toInt(), page.toInt(), status);
}
