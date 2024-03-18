/*
 * Copyright (C) PrivateWeb Software (https://github.com/PrivateWebSoftware) - All Rights Reserved
 *
 * Licensed under the Apache License 2.0 (the "License"). You may not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution
 */

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import PrivateWeb.PWLottie
import PrivateWeb.PWLottie.Controllers

ApplicationWindow {
    id: applicationWindow

    width: 640
    height: 480

    visible: true

    title: qsTr("PWLottie Demo")

    Pane {
        id: informationBar

        height: 80

        Material.elevation: 3
        Material.background: "#ff7c4d"
        Material.roundedScale: Material.NotRounded

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Text {
            id: lottieItemsCountText

            color: "#ffffff"

            text: "Lottie Items count: " + (lottieItemsListView.count * 3)

            font.bold: true
            font.pointSize: 14

            verticalAlignment: Text.AlignVCenter

            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }
        }

        ComboBox {
            id: controllerTypeComboBox

            height: 80
            width: 200

            currentIndex: 1

            Material.accent: "#ff571a"

            model: [ "NoController", "BaseController" ]

            anchors {
                top: parent.top
                left: lottieItemsCountText.right
                right: removeLastLottieButton.left
                bottom: parent.bottom


                leftMargin: 10
                rightMargin: 10
            }

            onActivated: (index) => {
                             if (index === 0) {
                                 lottieItemsListView.changeControllerChanged(ControllerType.NoController)
                             } else if (index === 1) {
                                 lottieItemsListView.changeControllerChanged(ControllerType.BaseController)
                             }
                         }
        }

        RoundButton {
            id: removeLastLottieButton

            width: height

            contentItem: Text {
                text: "-"

                font.bold: true
                font.pointSize: 16

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            anchors {
                top: parent.top
                right: addNewLottieButton.left
                bottom: parent.bottom

                rightMargin: 5
            }

            onClicked: {
                lottieItemsListView.removeLastLotties()
            }
        }

        RoundButton {
            id: addNewLottieButton

            width: height

            contentItem: Text {
                text: "+"

                font.bold: true
                font.pointSize: 16

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            anchors {
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }

            onClicked: {
                lottieItemsListView.addNewLotties()
            }
        }
    }

    ListView {
        id: lottieItemsListView

        cacheBuffer: (Qt.platform.os === "ios" || Qt.platform.os === "android") ? 360 : 5000 /// NOTE: Setting this value will prevent killing lotties, if u need to test optimiztion on not powerful devices, set lower value.

        clip: true

        model: ListModel {}

        /***********/
        /* Signals */
        /***********/

        signal changeControllerChanged(int newController)

        /*************/
        /* Functions */
        /*************/

        function addNewLotties() {
            lottieItemsListView.model.append({
                                                 "lottieSource": ":/lotties/Lotties/normal-prefomance.json"
                                             })
        }

        function removeLastLotties() {
            if (lottieItemsListView.count > 0) {
                lottieItemsListView.model.remove(lottieItemsListView.count - 1);
            }
        }

        anchors {
            top: informationBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        delegate: RowLayout {
            id: lottiesRowLayout

            width: applicationWindow.width
            height: lottiesRowLayout.lottieSize

            readonly property int lottiesCount: 3

            readonly property real lottieSize: (Qt.platform.os === "ios" || Qt.platform.os === "android") ? 256 : 512
            readonly property real minimumSourceSize: (Qt.platform.os === "ios" || Qt.platform.os === "android") ? 512 : 512

            Repeater {
                model: lottiesRowLayout.lottiesCount

                PWLottieItem {
                    id: pwLottieItem

                    width: lottiesRowLayout.lottieSize
                    height: lottiesRowLayout.lottieSize

                    antialiasing: true
                    smooth: true

                    source: lottieSource
                    controller: (controllerTypeComboBox.currentIndex === 1) ? ControllerType.BaseController : ControllerType.NoController

                    frameRate: 60
                    loops: 0

                    sourceSize.width: lottiesRowLayout.minimumSourceSize
                    sourceSize.height: lottiesRowLayout.minimumSourceSize

                    Layout.alignment: Qt.AlignCenter

                    Connections {
                        target: lottieItemsListView

                        function onChangeControllerChanged(newController) {
                            pwLottieItem.controller = newController
                        }
                    }
                }
            }
        }
    }
}
