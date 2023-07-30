#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSortFilterProxyModel>
#include <QtQml/QQmlContext>

#include <QQmlContext>
#include <QDebug>
#include "serialthread.h"
#include "DataViewList.h"
#include "DataViewModel.h"
#include "treeviewmodel.h"
#include "userlayoutdata.h"
#include "ProxyDataModelView.h"
#include "common.h"
#include "graph.h"
#include "radialbar.h"
#include "pythonscripting.h"
#include "javascripting.h"
#include "codelayer.h"
#include "terminal.h"


/**
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    //QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

    qmlRegisterType<DataViewModel>("DataView", 1, 0, "DataViewModel");
    qmlRegisterUncreatableType<DataViewList>("DataView", 1, 0, "DataViewList",
        QStringLiteral("ToDoList should not be created in QML"));

    qmlRegisterType<Graph>("Graph", 1, 0, "Graph");
    qmlRegisterType<RadialBar>("CustomControls", 1, 0, "RadialBar");

    TreeViewModel mymodel;
    engine.rootContext()->setContextProperty("mymodel", &mymodel);

    DataViewList *dataViewList;
    dataViewList = new DataViewList;

    DataViewModel *model;
    model = new DataViewModel;
    model->setList(dataViewList);
    ProxyDataModelView *proxyModel = new ProxyDataModelView;
    proxyModel->setSourceModel(model);
    //proxyModel->setSource(model);

    engine.rootContext()->setContextProperty("testModel", model);
    engine.rootContext()->setContextProperty("testProxyModel", proxyModel);

    SerialThread *serialThread;
    serialThread    = new SerialThread;
    engine.rootContext()->setContextProperty("serial", serialThread);

    UserLayoutData * userLayoutData;
    userLayoutData = new UserLayoutData;
    engine.rootContext()->setContextProperty("LayoutData", userLayoutData);

    common * usedCommon;
    usedCommon = new common;
    engine.rootContext()->setContextProperty("common", usedCommon);

    pythonScripting *script;
    script = new pythonScripting;
    engine.rootContext()->setContextProperty("script", script);

    JavaScripting *jScript;
    jScript = new JavaScripting;
    engine.rootContext()->setContextProperty("jScript", jScript);

    codeLayer *codeL;
    codeL = new codeLayer;
    engine.rootContext()->setContextProperty("codeLayer", codeL);

    terminalT *termi;
    termi = new terminalT;
    engine.rootContext()->setContextProperty("term", termi);

    qmlRegisterType< SyntaxHighlighter >( "StephenQuan", 1, 0, "SyntaxHighlighter" );
    qmlRegisterType< TextCharFormat >( "StephenQuan", 1, 0, "TextCharFormat" );

    engine.rootContext()->setContextProperty("dataViewList", dataViewList);

    QObject::connect(dataViewList, &DataViewList::sendRcrsSignal,serialThread, &SerialThread::sendRCRS );
    QObject::connect(dataViewList, &DataViewList::sendScrsSignal,serialThread, &SerialThread::sendSCRS );
    QObject::connect(serialThread, &SerialThread::recievedAdamMsg, dataViewList, &DataViewList::saveRequestAdam);
    QObject::connect(serialThread, &SerialThread::recievedModBusMsg, dataViewList, &DataViewList::saveRequestModBus);  
    QObject::connect(serialThread, &SerialThread::recievedSCPIMsg, dataViewList, &DataViewList::saveRequestSCPI);
    QObject::connect(dataViewList, &DataViewList::setGroup, &mymodel, &TreeViewModel::insertGroup);
    QObject::connect(serialThread, &SerialThread::editItemProtocolChanged, dataViewList, &DataViewList::editItemProtocolChanged);
    QObject::connect(jScript, &JavaScripting::sendRcrsSignal,serialThread, &SerialThread::sendRCRS );
    QObject::connect(jScript, &JavaScripting::sendScrsSignal,serialThread, &SerialThread::sendSCRS );

    QObject::connect(dataViewList, &DataViewList::getSelItem, userLayoutData, &UserLayoutData::retSelReg);
    QObject::connect(userLayoutData, &UserLayoutData::getSelReg, dataViewList, &DataViewList::setSelItem);
    QObject::connect(userLayoutData, &UserLayoutData::sendRCRS, serialThread, &SerialThread::sendRCRS );
    QObject::connect(userLayoutData, &UserLayoutData::sendSCRS, serialThread, &SerialThread::sendSCRS );
    QObject::connect(serialThread, &SerialThread::recievedAdamMsg, userLayoutData, &UserLayoutData::setAdamReg);
    QObject::connect(serialThread, &SerialThread::recievedModBusMsg, userLayoutData, &UserLayoutData::setModBusReg);
    QObject::connect(serialThread, &SerialThread::recievedSCPIMsg, userLayoutData, &UserLayoutData::setSCPIReg);
    QObject::connect(serialThread, &SerialThread::recievedAdamMsg, jScript, &JavaScripting::readAdamData);
    QObject::connect(serialThread, &SerialThread::recievedModBusMsg, jScript, &JavaScripting::readModbusData);
    QObject::connect(serialThread, &SerialThread::recievedSCPIMsg, jScript, &JavaScripting::readSCPIData);

    QObject::connect(serialThread, &SerialThread::printDataByte, userLayoutData, &UserLayoutData::setDataToTerminal);

    QObject::connect(proxyModel, &ProxyDataModelView::rowChanged, dataViewList, &DataViewList::rowChanged);
    QObject::connect(proxyModel, &ProxyDataModelView::rowDeleted, dataViewList, &DataViewList::rowDeleted);

     QObject::connect(usedCommon, &common::saveDataModel, dataViewList, &DataViewList::saveItemToFile);
     QObject::connect(usedCommon, &common::loadDataModel, dataViewList, &DataViewList::loadItemToFile);
     QObject::connect(usedCommon, &common::closeDataModel, dataViewList, &DataViewList::removeCompletedItems);
     QObject::connect(usedCommon, &common::saveDataLayout, userLayoutData, &UserLayoutData::saveLayoutItemToFile);
     QObject::connect(usedCommon, &common::loadDataLayout, userLayoutData, &UserLayoutData::loadLayoutItemToFile);
     QObject::connect(usedCommon, &common::closeLayout, userLayoutData, &UserLayoutData::closeAll);
     QObject::connect(usedCommon, &common::saveDataLC, serialThread, &SerialThread::saveSerialLc);
     QObject::connect(usedCommon, &common::loadLC, serialThread, &SerialThread::loadSerialLc);
     QObject::connect(usedCommon, &common::closeAllSerial, serialThread, &SerialThread::closeAllSerial);
     QObject::connect(usedCommon, &common::loadImageBck, userLayoutData, &UserLayoutData::loadImageBck);

     QObject::connect(serialThread, &SerialThread::printDataSignal, termi, &terminalT::setDataInTerm);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
