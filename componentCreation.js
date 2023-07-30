var component;
var sprite;

var rectString = "import QtQuick 2.0; Item { id: root; property var nameId:\'+id+\';property var xa:\'+xAxe+\'; property var ya:\'+yAxe+\'; Rectangle {width: 20; height: 20; x: \' +xa+ \' ; MouseArea {id:dragArea; anchors.fill: parent; drag.target: parent; onReleased: {root.xa = mouseX; parent.Drag.drop()}}}}";

// 'import QtQuick 2.0; Item { id: root; property var nameId:'+id+';property var xa:'+xAxe+'; property var ya:'+yAxe+';  Rectangle {id: rect ;color: "red"; width: 20; height: 20; x:'+xAxe+';y: '+yAxe+'; Drag.active: dragArea.drag.active; MouseArea {id:dragArea; anchors.fill: parent; drag.target: parent; onReleased: {root.xa = mouseX; parent.Drag.drop()}}}}'

function createSpriteObjects() {
    component = Qt.createComponent("RegisterIcon.qml")
    sprite = component.createObject(appWindow, {x: 100, y: 100});

    if (sprite === null) {
        // Error Handling
        console.log("Error creating object");
    }
}
