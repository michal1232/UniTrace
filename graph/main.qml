/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
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

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4
import Graph 1.0


ApplicationWindow {
    id: root
    width: 1160
    height: 800
    visible: true
    title: qsTr("Stack")
    property var cnt: 0


    Item {
        anchors.fill: parent

        Graph {
            id: graph2
            height: 400
            width: 600

            function newSample(i) {
                var ret = (Math.cos(i / 100.0 * Math.PI * 2) + 1) * 0.4
                return ret;
            }

            Component.onCompleted: {
                setMaxLine(3);
                graph2.samp[0] = graph2.newSample(0);
                graph2.samp[1] = graph2.newSample(0 + 20);
                graph2.samp[2] = graph2.newSample(0 + 40);
                appendSample(samp);
//                for (var i=0; i<100; ++i) {
//                    graph2.samp[0] = graph2.newSample(i);
//                    graph2.samp[1] = graph2.newSample(i + 20);
//                    graph2.samp[2] = graph2.newSample(i + 40);
//                    appendSample(samp);
//                }
            }

            property int offset: 100;
            property var samp: [3]
        }

        Timer {
            id: timer
            interval: 100
            repeat: true
            running: true

            property var samp: [3]
            onTriggered: {
                timer.samp[0] = graph2.newSample(++graph2.offset);
                timer.samp[1] = graph2.newSample(++graph2.offset + 20);
                timer.samp[2] = graph2.newSample(++graph2.offset + 40);

                if(root.cnt >= 100) {
                    graph2.removeFirstSample();
                }
                else {
                    root.cnt++;
                }

                graph2.appendSample(samp);
            }

        }

        Rectangle {
            anchors.fill: graph2
            color: "transparent"
            border.color: "black"
            border.width: 2
        }

    }
}
