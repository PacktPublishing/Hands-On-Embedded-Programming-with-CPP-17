/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Scene3D 2.0
import QtQuick.Layouts 1.2
import QtMultimedia 5.0

Item {
    id: mainview
    width: 1215
    height: 720
    visible: true
    property bool isHoverEnabled: false
    property int mediaLatencyOffset: 68
        

    state: "stopped"
    states: [
        State {
            name: "playing"
            PropertyChanges {
                target: playButtonImage
                source: {
                    if (playButtonMouseArea.containsMouse)
                        "qrc:/images/pausehoverpressed.png"
                    else
                        "qrc:/images/pausenormal.png"
                }
            }
            PropertyChanges {
                target: stopButtonImage
                source: "qrc:/images/stopnormal.png"
            }
        },
        State {
            name: "paused"
            PropertyChanges {
                target: playButtonImage
                source: {
                    if (playButtonMouseArea.containsMouse)
                        "qrc:/images/playhoverpressed.png"
                    else
                        "qrc:/images/playnormal.png"
                }
            }
            PropertyChanges {
                target: stopButtonImage
                source: "qrc:/images/stopnormal.png"
            }
        },
        State {
            name: "stopped"
            PropertyChanges {
                target: playButtonImage
                source: "qrc:/images/playnormal.png"
            }
            PropertyChanges {
                target: stopButtonImage
                source: "qrc:/images/stopdisabled.png"
            }
        }
    ]   
    
    Connections {
        target: qmlinterface
        onStopped: mainview.state = "stopped"
        onPaused: mainview.state = "paused"
        onPlaying: mainview.state = "started"
        onStart: visualizer.startVisualization()
    }

    Component.onCompleted: isHoverEnabled = qmlinterface.isHoverEnabled()

    Image {
        id: coverImage
        anchors.fill: parent
        source: "qrc:/images/albumcover.png"
    }

    //![1]
    Scene3D {
        anchors.fill: parent

        Visualizer {
            id: visualizer
            animationState: mainview.state
            numberOfBars: 120
            barRotationTimeMs: 8160 // 68 ms per bar
        }
    }
    //![1]

    Rectangle {
        id: blackBottomRect
        color: "black"
        width: parent.width
        height: 0.14 * mainview.height
        anchors.bottom: parent.bottom
    }

    // Duration of played content
    Text {
        text: qmlinterface.durationTotal
        color: "#80C342"
        x: parent.width / 6
        y: mainview.height - mainview.height / 8
        font.pixelSize: 12
    }

    // Duration of the content left
    Text {
        text: qmlinterface.durationLeft
        color: "#80C342"
        x: parent.width - parent.width / 6
        y: mainview.height - mainview.height / 8
        font.pixelSize: 12
    }

    property int buttonHorizontalMargin: 10
    Rectangle {
        id: playButton
        height: 54
        width: 54
        anchors.bottom: parent.bottom
        anchors.bottomMargin: width
        x: parent.width / 2 - width - buttonHorizontalMargin
        color: "transparent"

        Image {
            id: playButtonImage
            source: "qrc:/images/pausenormal.png"
        }

        MouseArea {
            id: playButtonMouseArea
            anchors.fill: parent
            hoverEnabled: isHoverEnabled
            onClicked: {
                if (mainview.state == 'paused' || mainview.state == 'stopped')
                    mainview.state = 'playing'
                else
                    mainview.state = 'paused'
            }
            onEntered: {
                if (mainview.state == 'playing')
                    playButtonImage.source = "qrc:/images/pausehoverpressed.png"
                else
                    playButtonImage.source = "qrc:/images/playhoverpressed.png"
            }
            onExited: {
                if (mainview.state == 'playing')
                    playButtonImage.source = "qrc:/images/pausenormal.png"
                else
                    playButtonImage.source = "qrc:/images/playnormal.png"
            }
        }
    }

    Rectangle {
        id: stopButton
        height: 54
        width: 54
        anchors.bottom: parent.bottom
        anchors.bottomMargin: width
        x: parent.width / 2 + buttonHorizontalMargin
        color: "transparent"

        Image {
            id: stopButtonImage
            source: "qrc:/images/stopnormal.png"
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: isHoverEnabled
            onClicked: mainview.state = 'stopped'
            onEntered: {
                if (mainview.state != 'stopped')
                    stopButtonImage.source = "qrc:/images/stophoverpressed.png"
            }
            onExited: {
                if (mainview.state != 'stopped')
                    stopButtonImage.source = "qrc:/images/stopnormal.png"
            }
        }
    }
}
