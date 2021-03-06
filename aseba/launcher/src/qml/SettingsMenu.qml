import QtQuick 2.11

Rectangle {
    color: "#535353"
    id:pane
    width: 350
    ListModel {
        id: entries
        ListElement {
            name: qsTr("Launch a Simulator")
            action: "playground"
        }
        ListElement {
            name: qsTr("Download maps for the simulator")
            action: "playground-faq"
        }
    }



    function onMenuEntryClicked(action) {
        if(action === "playground") {
            Utils.launchPlayground()
        }
        else if(action === "playground-faq") {
            Qt.openUrlExternally(qsTr("https://www.thymio.org/thymio-simulator"))
        }
    }

    Item {
        anchors.fill: parent
        anchors.margins: 10
        Item {
            id:top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20

            SvgButton {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/assets/launcher-icon-close.svg"
                height: 16
                width: height
                onClicked: {
                    pane.visible = false
                }
            }

            Text {
                text: qsTr("Tools")
                color: "white"
                font.bold: true
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        ListView {
            anchors.topMargin: 20
            orientation: ListView.Vertical
            anchors.top: top.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 350
            model: entries
            delegate: Item {
                height: 30
                width: 350
                Text {
                    anchors.right: parent.right
                    width: contentWidth
                    color: ma.containsMouse? "#009FE3" : "white"
                    text: name
                }
                MouseArea {
                    anchors.fill: parent
                    id:ma
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        pane.onMenuEntryClicked(action)
                    }
                }
            }
        }

        PropertyAnimation {
            id: showAnimation
            property: "x"
            target: pane
            from: pane.parent.width
            to: pane.parent.width - 350
            duration: 200
            easing.type: Easing.InOutQuad
        }
        onVisibleChanged: {
            x = pane.parent.width
            showAnimation.running = true
        }
    }
    InverseMouseArea {
        enabled: pane.visible
        onPressed:  {
            pane.visible = false
        }
        anchors.fill: parent
    }
}
