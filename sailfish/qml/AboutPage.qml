/*
 * Copyright (C) 2018 Alexander Seibel.
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

import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    id: aboutPage

    SilicaFlickable {
        id: aboutPageFlickable
        anchors.fill: parent
        contentHeight: column.height

        Column {
            PageHeader {
                title: "About"
            }

            id: column
            width: parent.width
            anchors { left: parent.left; right: parent.right }
            height: childrenRect.height

            Item {
                anchors { left: parent.left; right: parent.right }
                height: appName.height + 2 * appName.anchors.margins
                Text {
                    id: appName
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left; right: parent.right
                        margins: Theme.paddingMedium
                    }
                    font.pixelSize: Theme.fontSizeExtraLarge
                    horizontalAlignment: Text.AlignHCenter
                    color: Theme.highlightColor
                    font.bold: true
                    text: "GagBook"
                }
            }

            Item {
                anchors { left: parent.left; right: parent.right }
                height: versionText.height + 2 * versionText.anchors.margins

                Text {
                    id: versionText
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left; right: parent.right
                        margins: Theme.paddingMedium
                    }
                    wrapMode: Text.Wrap
                    font.pixelSize: Theme.fontSizeSmall
                    horizontalAlignment: Text.AlignHCenter
                    color: Theme.highlightColor
                    text: "v. " + APP_VERSION
                }
            }

            SectionHeader {
                text: "What is GagBook?"
            }

            Item {
                anchors { left: parent.left; right: parent.right }
                height: aboutText.height + 2 * aboutText.anchors.margins

                Text {
                    id: aboutText
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left; right: parent.right
                        margins: Theme.paddingMedium
                    }
                    wrapMode: Text.Wrap
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.highlightColor
                    text: "GagBook is a mobile app for the 9GAG website that allows you to easily " +
                          "view posts from 9GAG with a simple, smooth and native user interface."
                }
            }

            SectionHeader {
                text: "Download counter"
            }

            Item {
                anchors { left: parent.left; right: parent.right }
                height: counterText.height + 2 * counterText.anchors.margins

                Text {
                    id: counterText
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left; right: parent.right
                        margins: Theme.paddingMedium
                    }
                    font.pixelSize: Theme.fontSizeSmall
                    //horizontalAlignment: Text.AlignHCenter
                    color: Theme.highlightColor
                    wrapMode: Text.Wrap
                    //textFormat: Text.PlainText
                    text: "Downloaded: ~ " + gagbookManager.downloadCounter + " MB"
                }
            }

            SectionHeader {
                text: "Legal"
            }

            Item {
                anchors { left: parent.left; right: parent.right }
                height: legalText.height + 2 * legalText.anchors.margins

                Text {
                    id: legalText
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left; right: parent.right
                        margins: Theme.paddingMedium
                    }
                    font.pixelSize: Theme.fontSizeSmall
                    //horizontalAlignment: Text.AlignHCenter
                    color: Theme.highlightColor
                    wrapMode: Text.Wrap
                    textFormat: Text.RichText
                    text: "Copyright (C) 2018 Alexander Seibel.<br>Copyright (c) 2017 Leszek Lesner.<br>" +
                          "Copyright (c) 2014 Dickson Leong.<br>" +
                          "Thanks to <a href=\"https://twitter.com/b0bben\">b0bben</a> for the initial Sailfish port.<br>" +
                          "<br>By using this app you automatically agree to 9GAG's <a href=\"https://9gag.com/tos\">Terms " +
                          "of Service</a> and <a href=\"https://9gag.com/privacy\">Privacy Policy</a>."
                    onLinkActivated: Qt.openUrlExternally(link)
                }
            }

            SectionHeader {
                text: "Donations"
            }

            Item {
                anchors { left: parent.left; right: parent.right }
                height: donationButton.height + 2 * donationButton.anchors.margins

                Button {
                    id: donationButton
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                        margins: Theme.paddingMedium
                    }

                    text: "Donate via PayPal"
                    onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/" +
                                                    "webscr?cmd=_s-xclick&hosted_button_id=M9W2EN2ZELT48")
                }
            }

            SectionHeader {
                text: "Source code"
            }

            Item {
                anchors { left: parent.left; right: parent.right }
                height: sourceButton.height + 2 * sourceButton.anchors.margins

                Button {
                    id: sourceButton
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                        margins: Theme.paddingMedium
                    }

                    text: "GitHub Repository"
                    onClicked: Qt.openUrlExternally(QMLUtils.REPO_WEBSITE)
                }
            }
        }

        VerticalScrollDecorator {}
    }
}
