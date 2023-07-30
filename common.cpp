#include "common.h"
#include "userlayoutdata.h"
#include <QDir>

/**
 * @brief common::common
 * @param parent
 */
common::common(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &common::timeoutCleanInfo);
    timer->start(INFO_TIMEOUT);
}

/**
 * @brief common::saveProject
 * @param fileSource
 */
void common::saveProject(QString fileSource)
{
    QDir dir(fileSource);

    if(!dir.exists()) {
        dir.mkpath(".");
    }

    emit saveDataModel(fileSource+"/dataModel.csv");

    qint32 maxPage = 0;
    maxPage =  UserLayoutData::getCurrMaxPage();
    QStringList namePage;
    namePage = UserLayoutData::getNamesOfPages();

    emit saveDataLayout((fileSource+"/newPage.csv"), 0);

    for(qint32 i = 0 ; i < maxPage; i++) {
        emit saveDataLayout((fileSource+"/"+namePage[i]+".csv"), i+1);
    }

    emit saveDataLC(fileSource+"/LogicChannel.csv");

    struct DataViewEn viewEn;
    DataViewList::getSetmItemsViewEn(&viewEn, true);
    QFile dataViewEn(fileSource + "/" + "additionSetting.csv");

    if (dataViewEn.open(QIODevice::ReadWrite)) {
        QTextStream stream(&dataViewEn);
        stream << "additionSetting" << ";"
               << "\n";
        stream << viewEn.protocolEn << ";"
               << viewEn.LCEn << ";"
               << viewEn.nameRegEn << ";"
               << viewEn.valueTypeEn << ";"
               << viewEn.minRangeEn << ";"
               << viewEn.maxRangeEn << ";"
               << viewEn.rwStatusEn << ";"
               << viewEn.uartIdEn << ";"
               << viewEn.lenEn << ";"
               << viewEn.offsetEn << ";"
               << viewEn.nodeIdEn << ";"
               << viewEn.regAddressEn << ";"
               << "\n";

        dataViewEn.close();
    }


    QStringList sepFile = fileSource.split("/");
    QString projFileSrc = fileSource + "/" + sepFile.last() + ".uproj";

    QStringList listPath = UserLayoutData::getListPathImage();

    QFile file(projFileSrc);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << "ProjData" << ";"
               << "\n";

        stream << "dataModel.csv" << ";"
               << "Model" << ";"
               << "\n";

        stream << "newPage.csv" << ";"
               << "Layout_PG0" << ";"
               << "\n";
        for(qint32 i = 0 ; i < maxPage; i++) {
            stream << (namePage[i]+".csv") << ";"
                   << "Layout" << ";"
                   << "\n";
        }

        stream << "LogicChannel.csv" << ";"
               << "LogicChannel" << ";"
               << "\n";

        stream << "additionSetting.csv" << ";"
               << "AdSetting" << ";"
               << "\n";

        for(qint32 i = 0 ; i < listPath.size(); i++) {
            QString path = listPath[i].replace("file://","");
            QStringList nameList =  listPath[i].split("/");
            QString newPath = fileSource+"/"+nameList.last();
            QFile::copy(path, newPath);

            path.clear();
            path += "file://" + fileSource+ "/" + nameList.last();

            stream << path << ";"
                   << "imageBck" << ";"
                   << "\n";
        }

        file.close();
    }
}

/**
 * @brief common::loadProject
 * @param fileSource
 */
void common::loadProject(QString fileSource)
{
    qint32 layoutCnt = 1;
    qint32 imageCnt = 0;
    QStringList fileList = fileSource.split("/");

    QFile file(fileSource+"/"+fileList.last()+".uproj");
    if (file.open(QIODevice::ReadWrite)) {

        QString source = file.readLine();
        if(source.contains("ProjData")) {

            while (!file.atEnd()) {
                QString line = file.readLine();
                QStringList list = line.split(QLatin1Char(';'));

                if(list.size() >= 2) {
                    if(list.at(1) == "Model") {
                        emit loadDataModel((fileSource+"/"+list.at(0)), "", "", true);
                    }
                    else if(list.at(1) == "Layout_PG0") {
                        emit loadDataLayout((fileSource+"/"+list.at(0)), 0);
                    }
                    else if(list.at(1) == "Layout") {
                        emit loadDataLayout((fileSource+"/"+list.at(0)), layoutCnt);
                        layoutCnt++;
                    }
                    else if(list.at(1) == "LogicChannel") {
                        emit loadLC((fileSource+"/"+list.at(0)));
                    }
                    else if(list.at(1) == "imageBck") {
                        emit loadImageBck(imageCnt, list.at(0));
                        imageCnt++;
                    }
                    else if(list.at(1) == "AdSetting") {
                        QFile file((fileSource+"/"+list.at(0)));
                        if (file.open(QIODevice::ReadWrite)) {

                            QString source = file.readLine();
                            if(source.contains("additionSetting")) {

                                QString line = file.readLine();
                                QStringList list = line.split(QLatin1Char(';'));

                                if(list.size() >= 12) {
                                    struct DataViewEn viewEn;
                                    viewEn.protocolEn = list.at(0).toUInt();
                                    viewEn.LCEn = list.at(1).toUInt();
                                    viewEn.nameRegEn = list.at(2).toUInt();
                                    viewEn.valueTypeEn = list.at(3).toUInt();
                                    viewEn.minRangeEn = list.at(4).toUInt();
                                    viewEn.maxRangeEn = list.at(5).toUInt();
                                    viewEn.rwStatusEn = list.at(6).toUInt();
                                    viewEn.uartIdEn = list.at(7).toUInt();
                                    viewEn.lenEn = list.at(8).toUInt();
                                    viewEn.offsetEn = list.at(9).toUInt();
                                    viewEn.nodeIdEn = list.at(10).toUInt();
                                    viewEn.regAddressEn = list.at(11).toUInt();

                                    DataViewList::getSetmItemsViewEn(&viewEn, false);
                                }
                            }
                        }
                    }
                }
            }
        }
        file.close();
    }

    QDir dir(".");

    QStringList appFileList = dir.absolutePath().split("uni-trace.app/");
    QString appFile = appFileList.at(0);
    QStringList lineFile;

    qInfo() << appFile;

    QFile metaFile(appFile + "metadata.txt");
    if(metaFile.exists()) {
        if (metaFile.open(QIODevice::ReadWrite)) {

            while (!metaFile.atEnd()) {
                lineFile.append(metaFile.readLine());
            }

            metaFile.resize(0);

            QTextStream stream(&metaFile);

            if(lineFile.contains(fileSource+"\n") == false) {
                stream <<fileSource << "\n";
            }

            for(qint32 i = 0; i < lineFile.size(); i++) {
                stream << lineFile.at(i) << "\n";
            }

            metaFile.close();
        }
    }

    emit printProjectSignal("Project: " + fileSource);
    emit printInfoSignal("Project loaded succesufly!");
    timer->start(INFO_TIMEOUT);
}

void common::closeProject()
{
    emit closeDataModel(true);
    emit closeLayout();
    emit closeAllButton();
    emit closeAllSerial();
    emit printProjectSignal("Project: Unsaved");
    emit printInfoSignal("Project closed.");
    timer->start(INFO_TIMEOUT);
}

void common::getLastLoadedProject()
{
    QDir dir(".");
    qInfo() << dir.absolutePath();

    QStringList appFileList = dir.absolutePath().split("uni-trace.app/");
    QString appFile = appFileList.at(0);

    qInfo() << appFile;

    QFile file(appFile + "metadata.txt");

//    if(file.exists()) {
//        qInfo() << "file exist";
//        if (file.open(QIODevice::ReadWrite)) {
//            QString prjFileSrcN;
//            QString prjFileSrc;
//            while (!file.atEnd()) {
//                prjFileSrcN = file.readLine();
//                qint32 pos = prjFileSrcN.lastIndexOf("\n");
//                prjFileSrc = prjFileSrcN.left(pos);
//                QStringList listFile = prjFileSrc.split("/");
//                QFile metaFile(prjFileSrc + "/" + listFile.last() + ".uproj");

//                if(metaFile.exists()) {
//                    emit recentLoadProj(prjFileSrc, listFile.last());
//                }
//            }
//            file.close();
//        }
//    }
//    else {
//        qInfo() << "file no exist";
//        file.open(QIODevice::WriteOnly);
//        file.close();
//    }
}

void common::printInfo(QString info)
{
    emit printInfoSignal(info);
    timer->start(INFO_TIMEOUT);
}

void common::printProject(QString project)
{
    emit printProjectSignal(project);
}

void common::connectionSettingClickSlot()
{
    emit connectionSettingClickSignal();
}

void common::createProtocolClickSlot()
{
    emit createProtocolClickSignal();
}

void common::modelSettingClickSlot()
{
    emit modelSettingClickSignal();
}

void common::newProjectClickSlot()
{
    emit newProjectClickSignal();
}

void common::timeoutCleanInfo()
{
    timer->stop();
    emit printInfoSignal("");
}
