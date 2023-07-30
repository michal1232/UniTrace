import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

import DataView 1.0

Item {
    id: root

    width: 150
    height: 50

    property string regType: "sliderView"
    property var widthMove: 150
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

    property bool settingVisile: true
    property bool sizeVisile: true

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
        LayoutData.getLenSlot(root.page, root.regName, root.point);
        LayoutData.getRangeSlot(root.page, root.regName, root.point);

        LayoutData.getFunctRegSlot(root.page, root.regName, root.point);
    }

    Connections {
        target: LayoutData

        function onGetRangeSignal(reg, page, min, max, point) {
            if(root.regName === reg && root.page === page && root.point === point) {
                for(let i = 0; i < root.regLen; i++){
                    root.objectArray[i].sliderMinRange = min;
                    root.objectArray[i].sliderMaxRange = max;
                }
            }
        }

        function onGetLenSignal(reg, page, len, point) {
            if(root.regName === reg && root.page === page && root.point === point) {
                if(root.enResize === true) {
                    root.enResize = false;
                    root.regLen = len;
                    root.height = (len+1)*25

                    var y = 0

                    for(let i = 0; i < len; i++){
                        createSliderWindow(y);
                        y += 25
                    }
                }
            }
        }

        function onGetFunctRegSignal(reg, page, funct, num, point) {
            if(reg === root.regName && root.page === page  && root.point === point) {
                for(let i = 0; i < root.regLen; i++) {
                    root.objectArray[i].functVal = funct;
                    root.objectArray[i].numVal = num;
                }
            }
        }

        function onPrintReg(point, page, regValue)
        {

            var splitRegValue;

            if(root.point === point && root.page === page) {
                splitRegValue = regValue.split(",");

                for(let i = 0; i < root.regLen; i++) {
                    if(objectArray[i].functVal === "/") {
                        objectArray[i].sliderAlias.value = (splitRegValue[i]*objectArray[i].numVal)
                    }
                    //                else if(root.functVal === "*") {
                    //                    sliderText.text = Math.round((control.value*root.numVal) * 100 ) / 100
                    //                }
                    //                else if(root.functVal === "+") {
                    //                    sliderText.text = Math.round((control.value+root.numVal) * 100 ) / 100
                    //                }
                    //                else if(root.functVal === "-") {
                    //                    sliderText.text = Math.round((control.value-root.numVal) * 100 ) / 100
                    //                }
                    else {
                        objectArray[i].sliderAlias.value = splitRegValue[i];
                    }
                    //objectArray[i].sliderAlias.value = splitRegValue[i];
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

            LayoutData.changeLabel(point, regType, labelT);
            LayoutData.changeRegName(point, regType, labelT);

            for(let i = 0; i < root.regLen; i++){
                objectArray[i].destroy(0);
            }
            root.objectArray.splice(0,root.regLen);

            root.enResize = true;
            LayoutData.getLenSlot(root.page, root.regName, root.point);
            LayoutData.getRangeSlot(root.page, root.regName, root.point);
        }
    }

    WidgetBck {
        id:bck
        height: root.height
        width: root.width

        bcgMouse.onClicked: {
            if (mouse.button === Qt.RightButton) {

                zoomScrs.start();
                var textValue = "";

                for(let i = 0; i < root.regLen; i++) {
                    textValue +=root.objectArray[i].sliderAlias.value;

                    if(i < root.regLen-1)
                        textValue += ","
                }

                LayoutData.writeReg(root.page, root.point, textValue);
                outZoomScrs.start();

            }
            else if(mouse.button === Qt.MiddleButton) {
                zoomRcrs.start();
                LayoutData.readReg(root.page, point);
                outZoomRcrs.start();
            }
        }

        resizeXMouse.visible: root.sizeVisile
        resizeXMouse.onReleased: {
            root.width = bck.widthMove
            LayoutData.changeWidth(point, regType, bck.widthMove);
            for(let i = 0; i < root.regLen; i++){
                objectArray[i].width = root.width;
            }
        }

        resizeYMouse.visible: false

        rcrsMouse.onClicked: {
            LayoutData.readReg(root.page, point);
        }

        scrsMouse.onClicked: {

            var textValue = "";

            for(let i = 0; i < root.regLen; i++) {
                textValue +=root.objectArray[i].sliderAlias.value;

                if(i < root.regLen-1)
                    textValue += ","
            }

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
        anchors.left: parent.left
        anchors.leftMargin: 15

        label.onTextChanged: {
            root.labelT = labelText.text;
            LayoutData.changeLabel(root.point, root.regType, root.labelT);
        }
    }

    Rectangle {
        id:sliceRect
        anchors.top: labelText.bottom


    }


    function createSliderWindow(y) {
        var component;
        var textWindow;
        component = Qt.createComponent("SliderWindow.qml")
        textWindow = component.createObject(sliceRect, {width:root.width, y: y});

        objectArray.push(textWindow);

        if (textWindow === null) {
            // Error Handling
            console.log("Error creating object");
        }
    }
}

