import QtQuick.VirtualKeyboard 2.1
import com.powertune 1.0
import QtQuick 2.8
import QtQuick.Controls 2.1
import Qt.labs.settings 1.0
import QtQuick.Controls 1.4 as Quick1
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.10 //compatibility with QT 5.10

ApplicationWindow {
    id: window
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    minimumWidth: 800
    minimumHeight: 480
    visible: true
    title: qsTr("RECOVERY MENU")
    color: "black"

    Connections{
        target: PTrecovery
/*
        function onProcoutChanged(Procout) {
                consoleText.append(Procout)
                console.log("Procout in qml");
                scrollBar.increase()
            }*/
        onProcoutChanged:{
            consoleText.append(procout)
            scrollBar.increase()
        }

    }
    Image {
        id: name
        width: 400; height: 480
        fillMode: Image.PreserveAspectFit
        source: "qrc:/Logo.png"
        Component.onCompleted: {
            if(window.width == 1600){
                name.width == 800
                name.height == 630
            }
        }
    }
    WifiCountryList {
        id: wificountrynames
    }

    InputPanel {
            id: inputPanel
            z: 99
            x: 0
            y: window.height
            width: window.width

            states: State {
                name: "visible"
                when: inputPanel.active
                PropertyChanges {
                    target: inputPanel
                    y: window.height - inputPanel.height
                }
            }
            transitions: Transition {
                from: ""
                to: "visible"
                reversible: true
                ParallelAnimation {
                    NumberAnimation {
                        properties: "y"
                        duration: 250
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    Flickable {
        id: flickable
        width: 450
        height: 400
        visible:false

        Component.onCompleted: {
            if(window.width == 1600){
                flickable.width == 900
                flickable.height == 600
            }
        }

        TextArea.flickable: TextArea {
            id: consoleText
            wrapMode: TextArea.Wrap
            readOnly: true
            color: "green"
            font.pixelSize: 15
            background: Rectangle {
                height: flickable.height
                width: flickable.width
                color: "black"
            }
        }

        ScrollBar.vertical: ScrollBar {
            id: scrollBar
            policy: ScrollBar.AlwaysOn
        }
    }

    Grid {
        id: extragrid
        anchors.top: parent.top
        anchors.topMargin: parent.height / 20
        anchors.leftMargin: parent.width / 80
        anchors.left: flickable.right
        rows: 14
        columns: 2
        spacing: 2

        Text {
            text: " "
            font.pixelSize: window.width / 55
        }
        Button {
            id: btnScanNetwork
            // visible: false
            text: "Scan WIFI"
            width: window.width / 4
            height: window.height / 15
            font.pixelSize: window.width / 55
            onClicked: {
                consoleText.clear()
                PTrecovery.getavailablenetworks()
                //btnScanNetwork.enabled =false;
            }
        }
        Component.onCompleted: {
            PTrecovery.initializeWifiscanner()
        }
        Text {
            text: "WIFI Country :"
            font.pixelSize: window.width / 50
            color: "white"
        }
        ComboBox {
            id: wificountrycbx
            //visible: false
            width: window.width / 4
            height: window.height / 15
            font.pixelSize: window.width / 55
            model: wificountrynames
            textRole: "name"
            property bool initialized: true
        }
        Text {
            text: "Wifi:"
            font.pixelSize: window.width / 50
            color: "white"
        }
        ComboBox {
            id: wifilistbox
            //visible: false
            width: window.width / 4
            height: window.height / 15
            font.pixelSize: window.width / 55
            model: PTrecovery.wifilist
            onCountChanged: btnScanNetwork.enabled = true
            property bool initialized: false
            delegate: ItemDelegate {
                width: wifilistbox.width
                text: wifilistbox.textRole ? (Array.isArray(
                                                  control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
                font.weight: wifilistbox.currentIndex == index ? Font.DemiBold : Font.Normal
                font.family: wifilistbox.font.family
                font.pixelSize: wifilistbox.font.pixelSize
                highlighted: wifilistbox.highlightedIndex == index
                hoverEnabled: wifilistbox.hoverEnabled
            }
        }
        Text {

            text: "Password:"
            font.pixelSize: window.width / 50
            color: "white"
        }
        TextField {
            id: pw1
            placeholderText: qsTr("Passphrase")
            width: window.width / 4
            font.pixelSize: window.width / 55
        }

        Text {
            text: " "
            font.pixelSize: window.width / 50
        }

        Button {
            id: applyWifiSettings
            text: "Connect WIFI"
            width: window.width / 4
            height: window.height / 15
            font.pixelSize: window.width / 55
            onClicked: {
               // updateBtn.enabled = false
                applyWifiSettings.enabled = false
                btnScanNetwork.enabled = false
                PTrecovery.setwifi(
                            wificountrynames.get(
                                wificountrycbx.currentIndex).countryname,
                            wifilistbox.textAt(wifilistbox.currentIndex),
                            pw1.text)
            }
        }



        Text {
            text: " "
            font.pixelSize: window.width / 55
        }

        Text {
            text: " "
            font.pixelSize: window.width / 55
        }

        Text {
            text: " "
            font.pixelSize: window.width / 55
        }
        Text {
            text: " "
            font.pixelSize: window.width / 55
        }
        Text {
            id: ethernetip
            text: "Ethernet IP :"
            font.pixelSize: window.width / 50
            visible: true
            color: "white"
        }

        Quick1.Button {
            id: ethernetstatus
            text: PTrecovery.eth0ipstat
            width: window.width / 4
            height: window.height / 15
            style: ButtonStyle {
                label: Label {
                    text: ethernetstatus.text
                    color: "black"
                    font.pixelSize: window.width / 55
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: {
                        (ethernetstatus.text == "NOT CONNECTED") ? "red" : "green"
                    }
                    border.width: control.activeFocus ? 2 : 1
                    border.color: "#888"
                }
            }
        }

        Text {
            id: wlanip
            text: "WLAN IP :"
            font.pixelSize: window.width / 50
            visible: true
            color: "white"
        }
        Quick1.Button {
            id: wifistatus
            text: PTrecovery.wlanipstat
            width: window.width / 4
            height: window.height / 15
            style: ButtonStyle {
                label: Label {
                    text: wifistatus.text
                    color: "black"
                    font.pixelSize: window.width / 55
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: {
                        (wifistatus.text == "NOT CONNECTED") ? "red" : "green"
                    }
                    border.width: control.activeFocus ? 2 : 1
                    border.color: "#888"
                }
            }
        }
        /*
        Text {
            text: " "
            font.pixelSize: window.width / 50
        }
        Button {
            id: updateBtn
            text: "Update"
            width: window.width / 4
            height: window.height / 15
            font.pixelSize: window.width / 55
            onClicked: {
                flickable.visible = true;
              //  Connect.update()
                updateBtn.enabled = false
                applyWifiSettings.enabled = false
                btnScanNetwork.enabled = false
                PTrecovery.update()
            }
        }*/
        Text {
            text: " "
            font.pixelSize: window.width / 50
        }
        Text {
            text: " "
            font.pixelSize: window.width / 50
        }
        Text {
            text: " "
            font.pixelSize: window.width / 50
        }
        Button {
            id: quitbuttn
            text: "Quit to commandline"
            width: window.width / 4
            height: window.height / 15
            font.pixelSize: window.width / 55
            onClicked: {
                onClicked: { Qt.quit();}
                flickable.visible = true;
               // updateBtn.enabled = false
                applyWifiSettings.enabled = false
                btnScanNetwork.enabled = false

            }
        }

        Text {
            text: " "
            font.pixelSize: window.width / 50
        }
        Text {
            text: " "
            font.pixelSize: window.width / 50
        }
        Text {
            text: " "
            font.pixelSize: window.width / 50
        }
        Button {
            id: downloadsource
            text: "Update"
            width: window.width / 4
            height: window.height / 15
            font.pixelSize: window.width / 55
            onClicked: {
                flickable.visible = true;
                downloadsource.enabled = false;
                // updateBtn.enabled = false
                applyWifiSettings.enabled = false
                btnScanNetwork.enabled = false
                PTrecovery.deletesource();
            }
        }

    }
}
