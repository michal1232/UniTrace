import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

import DataView 1.0

Item {
    id: root

    width: 50
    height: 45

    property string regType: "switchView"
    property var widthMove: 50
    property var point: 0
    property var page: 0
    property string labelT: "label"
    property string regName: "label"
    property string value: "---"

    property var objectArray: []
    property var regLen: 1
    property bool enResize: true
    property var functVal: ""
    property var numVal: 1
    property string dataType: ""

    property bool settingVisile: true
    property bool sizeVisile: true

    property bool enSetDataType: true

    property var oldX: 0
    property var oldY: 0
    property bool ctrlPressed: false

    onXChanged: {
        if((Math.abs(root.x - root.oldX) < 5))
            root.x = root.oldX
        else  {
            root.x =Math.round(Math.floor(root.x) / 5) * 5
            console.log(root.x);
            root.oldX = root.x
        }
    }

    onYChanged: {
        if((Math.abs(root.y - root.oldY) < 5))
            root.y = root.oldY
        else  {
            root.y =Math.round(Math.floor(root.y) / 5) * 5
            console.log(root.y);
            root.oldY = root.y
        }
    }

    Component.onCompleted:{
        LayoutData.getNoModifLenSlot(root.page, root.point, root.regName);
        LayoutData.getDataType(root.point, root.page, root.regName);

    }

    Connections {
        target: LayoutData

        function onGetNoModifLenSignal(reg, page, point, len) {
            if(root.regName === reg && root.page === page && root.point === point) {
                if(root.enResize === true) {
                    root.enResize = false;
                    root.regLen = len;

                    var modHeight = Math.floor((root.height-20)/25);
                    bck.height = root.height;
                    ledGrid.columns = Math.ceil(root.regLen/modHeight);

                    for(let i = 0; i < len; i++){
                        createSwitch(i);
                    }
                }
            }
        }

        function onPrintReg(point, page, regValue)
        {
            if(root.point === point && root.page === page) {

                var splitRegValue;
                splitRegValue = regValue.split(",");

                if(dataType === "bitfield") {

                    let j = 0;
                    for(let i = 0; i < root.regLen; i++) {
                        j= Math.floor(i/8);

                        if(splitRegValue[j].charAt((j+1)*8-i-1) === "0")
                            root.objectArray[i].control.checked = false;
                        else
                            root.objectArray[i].control.checked = true;
                    }
                }
                else if(dataType === "bool") {
                    if(splitRegValue[0] === "True" || splitRegValue[0] === "T"  || splitRegValue[0] === "true"  || splitRegValue[0] === "1") {
                        root.objectArray[0].control.checked = true;
                    }
                    else {
                        root.objectArray[0].control.checked = false;
                    }
                }
            }
        }

        function onGetDataTypeSignal(page, point, regName, dataType) {
            if(root.enSetDataType === true) {
                if(root.point === point && root.page === page  && root.regName === regName ) {
                    root.dataType = dataType;
                    root.enSetDataType = false;
                }
            }
        }
    }

    WidgetMenu {
        id:widgetMenu

        widgetWidth: root.width

        onMenuTriggered: {
            root.labelT = widgetMenu.menuLC +"."+widgetMenu.regName + "." + widgetMenu.menuId;
            root.regName = root.labelT;
            labelText.text = root.labelT;

            LayoutData.changeLabel(root.point, root.regType, root.labelT);
            LayoutData.changeRegName(root.point, root.regType, root.labelT);

            for(let i = 0; i < root.regLen; i++){
                root.objectArray[i].destroy(0);
            }
            root.objectArray.splice(0,root.regLen);

            root.enResize = true;
            LayoutData.getNoModifLenSlot(root.page, root.regName);

            root.enSetDataType = true;
            LayoutData.getDataType(root.point, root.page, root.regName);
        }
    }

    WidgetBck {
        id:bck
        height: root.height
        width: root.width

        bcgMouse.onClicked: {
            if(mouse.button === Qt.RightButton) {
                zoomScrs.start();
                var textValue = "";
                var temp = 0;

                for(let i = 0; i < root.regLen; i++) {
                    temp |= (root.objectArray[i].control.checked << i);
                    if((i+1)%8 === 0) {
                        textValue += temp +",";
                        temp = 0;
                    }
                }
                textValue += temp;
                LayoutData.writeReg(root.page, root.point, textValue);
                outZoomScrs.start();
            }
            if(mouse.button === Qt.MiddleButton) {
                zoomRcrs.start();
                LayoutData.readReg(root.page, root.point);
                outZoomRcrs.start();
            }

        }

        resizeXMouse.visible: root.sizeVisile
        resizeXMouse.onReleased: {
            root.width = bck.widthMove
            LayoutData.changeWidth(point, regType, root.width)
        }

        resizeYMouse.visible: root.sizeVisile
        resizeYMouse.onReleased: {
            //root.height = bck.heightMove
            LayoutData.changeHeight(point, regType, root.height)
        }
        resizeYMouse.onMouseYChanged: {
            var modHeight = Math.floor((bck.heightMove-20)/25);
            root.height = Math.floor(modHeight) * 25+20;
            bck.height = root.height;
            ledGrid.columns = Math.ceil(root.regLen/modHeight);
        }

        rcrsMouse.onClicked: {
            LayoutData.readReg(root.page, root.point);
        }

        scrsMouse.onClicked: {

            var textValue = "";
            var temp = 0;

            for(let i = 0; i < root.regLen; i++) {
                temp |= (root.objectArray[i].control.checked << i);
                if((i+1)%8 === 0) {
                    textValue += temp +",";
                    temp = 0;
                }
            }
            textValue += temp;
            LayoutData.writeReg(root.page, root.point, textValue);
        }

        settingRow.visible: root.settingVisile

        deleteMouse.onClicked: {
            LayoutData.deleteFromLayoutList(point);
        }

        menuMouse.onClicked: {
            widgetMenu.channelConMenu.popup()
        }

        dragMouse.drag.target: root
        dragMouse.onPressed:  {
            root.focus = true;
        }

        dragMouse.onReleased: {
            root.focus = false;
            root.Drag.drop();
            LayoutData.changeXYCoordinateList(root.point,regType, root.x, root.y);
        }


    }

    DcTextEdit {
        id:labelText
        text: root.labelT
        anchors.top: root.top
        anchors.topMargin: 4
        anchors.left: root.left
        anchors.leftMargin: 10
        height: 20

        label.onTextChanged: {
            root.labelT = labelText.text;
            LayoutData.changeLabel(root.point, root.regType, root.labelT);
        }
    }

    Item {
        height: root.height-labelText.height
        width: root.width
        anchors.top: labelText.bottom
        anchors.topMargin: -5
        anchors.left: root.left
        clip: true

        Grid {
            id: ledGrid
            anchors.centerIn: parent
            columns: root.regLen

            spacing: 5
        }
    }



    function createSwitch(num) {
        var component;
        var led;
        component = Qt.createComponent("SwitchWindow.qml")
        led = component.createObject(ledGrid, {});

        objectArray.push(led);
        led.text = num + ": "

        if (led === null) {
            // Error Handling
            console.log("Error creating object");
        }
    }

}
