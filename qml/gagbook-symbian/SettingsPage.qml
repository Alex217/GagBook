/*
 * Copyright (c) 2012-2013 Dickson Leong.
 * All rights reserved.
 *
 * This file is part of GagBook.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 1.1
import com.nokia.symbian 1.1
import GagBook 1.0

Page {
    id: settingsPage

    tools: ToolBarLayout {
        ToolButton {
            platformInverted: gagSettings.whiteTheme
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    Flickable {
        id: settingsFlickable
        anchors { top: pageHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        contentHeight: settingsColumn.height + 2 * settingsColumn.anchors.margins

        Column {
            id: settingsColumn
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left; right: parent.right
                margins: constant.paddingMedium
            }
            height: childrenRect.height
            spacing: constant.paddingLarge

            SettingButtonRow {
                text: "Theme"
                checkedButtonIndex: gagSettings.whiteTheme ? 1 : 0
                buttonsText: ["Dark", "White"]
                onButtonClicked: gagSettings.whiteTheme = index === 1
            }

            SettingButtonRow {
                text: "Source"
                checkedButtonIndex: {
                    switch (gagSettings.source) {
                    case GagSettings.NineGagSource: return 0;
                    case GagSettings.InfiniGagSource: return 1;
                    }
                }
                buttonsText: ["9GAG", "InfiniGAG"]
                onButtonClicked: {
                    switch (index) {
                    case 0: gagSettings.source = GagSettings.NineGagSource; break;
                    case 1: gagSettings.source = GagSettings.InfiniGagSource; break;
                    }
                }
            }

            SettingButtonRow {
                text: "Download GIFs automatically"
                buttonsText: ["On", "Wi-Fi only", "Off"]
                checkedButtonIndex: {
                    switch (gagSettings.gifDownloadMode) {
                    case GagSettings.GifDownloadOn: return 0;
                    case GagSettings.GifDownloadOnWiFiOnly: return 1;
                    case GagSettings.GifDownloadOff: return 2;
                    }
                }
                onButtonClicked: {
                    switch (index) {
                    case 0: gagSettings.gifDownloadMode = GagSettings.GifDownloadOn; break;
                    case 1: gagSettings.gifDownloadMode = GagSettings.GifDownloadOnWiFiOnly; break;
                    case 2: gagSettings.gifDownloadMode = GagSettings.GifDownloadOff; break;
                    }
                }
            }
        }
    }

    ScrollDecorator { platformInverted: gagSettings.whiteTheme; flickableItem: settingsFlickable }

    PageHeader {
        id: pageHeader
        anchors { top: parent.top; left: parent.left; right: parent.right }
        text: "Settings"
        enabled: false
    }
}
