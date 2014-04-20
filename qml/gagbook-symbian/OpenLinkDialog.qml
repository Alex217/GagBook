/*
 * Copyright (c) 2014 Dickson Leong.
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

ContextMenu {
    id: openLinkDialog

    property string link

    platformInverted: appSettings.whiteTheme

    MenuLayout {
        Text {
            anchors { left: parent.left; right: parent.right }
            horizontalAlignment: Text.AlignHCenter
            text: link
            font.italic: true
            font.pixelSize: constant.fontSizeMedium
            color: constant.colorLight
            elide: Text.ElideRight
            maximumLineCount: 3
            wrapMode: Text.WrapAnywhere
        }
        MenuItem {
            text: "Open link in web browser"
            platformInverted: openLinkDialog.platformInverted
            onClicked: {
                QMLUtils.openDefaultBrowser(link)
                infoBanner.alert("Launching web browser...")
            }
        }
        MenuItem {
            text: "Copy link"
            platformInverted: openLinkDialog.platformInverted
            onClicked: {
                QMLUtils.copyToClipboard(link)
                infoBanner.alert("Link copied to clipboard")
            }
        }
    }
}
