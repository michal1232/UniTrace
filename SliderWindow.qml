import QtQuick 2.0
import QtQuick.Controls 2.15


import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

Item {
    id: root
    width: 150
    height: 25

    property alias sliderAlias: control
    property var point: 0
    property var sliderValue: 0.0
    property var sliderMaxRange: 255.0
    property var sliderMinRange: 0
    property var functVal: ""
    property var numVal: 1

    Component.onCompleted:{
        LayoutData.getFunctRegSlot(root.page, root.regName, root.point);
    }

    Connections {
        target: LayoutData

        function onGetFunctRegSignal(reg, page, funct, num, point) {
            if(reg === regName && root.page === page  && root.point === point) {
                root.functVal = funct;
                root.numVal = num;
            }
        }
    }


        Slider {
            id: control
            //height: parent.height
            //width: root.width-50

            background: Rectangle {
                x: control.leftPadding
                y: control.topPadding + control.availableHeight / 2 - height / 2
                implicitWidth: root.width - 50
                implicitHeight: 4
                width: control.availableWidth
                height: implicitHeight
                radius: 2
                color: "#bdbebf"

                Rectangle {
                    width: control.visualPosition * parent.width
                    height: parent.height
                    color: "#21be2b"
                    radius: 2
                }
            }

            handle: Rectangle {
                x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
                y: control.topPadding + control.availableHeight / 2 - height / 2
                implicitWidth: 16
                implicitHeight: 16
                radius: 8
                color: control.pressed ? "#f0f0f0" : "#f6f6f6"
                border.color: "#bdbebf"
            }

            from: root.sliderMinRange
            to: root.sliderMaxRange
            stepSize: 1

            onValueChanged: {

                if(root.functVal === "/") {
                   sliderText.text = Math.round((control.value/root.numVal) * 100 ) / 100
                }
                else if(root.functVal === "*") {
                    sliderText.text = Math.round((control.value*root.numVal) * 100 ) / 100
                }
                else if(root.functVal === "+") {
                    sliderText.text = Math.round((control.value+root.numVal) * 100 ) / 100
                }
                else if(root.functVal === "-") {
                    sliderText.text = Math.round((control.value-root.numVal) * 100 ) / 100
                }
                else {
                    sliderText.text = Math.round(control.value * 100 ) / 100
                }
            }
        }

        Item {
            id: itemText
            width: 50
            height: control.height
            anchors.right: control.right
            anchors.rightMargin:-40

            Text {
                id: sliderText
                anchors.centerIn: parent


            }
        }

}
