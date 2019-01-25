/*
 * Copyright (C) 2019 Alexander Seibel.
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
    property int currentlySelectedIndex: 0
    signal selectedSortTypeChanged(string sortType)

    function getSortingText(sortType) {
        if (sortType === "hot")
            //: The 'Hot' sorting lists the best rated items at the top (e.g. comments with the most upvotes).
            //% Synonyms: Popular, Favoured
            return qsTr("Hot")
        else if (sortType === "fresh")
            //: The 'Fresh' sorting lists the newest items (by the elapsed time) at the top.
            //% Synonyms: New, Recent
            return qsTr("Fresh")
    }

    SilicaListView {
        anchors.fill: parent
        model: sortModel

        header: PageHeader {
            title: qsTr("Sorting")
        }

        delegate: SimpleListItem {

            text: getSortingText(sortType)
            selected: currentlySelectedIndex == index

            onClicked: {
                currentlySelectedIndex = index
                selectedSortTypeChanged(sortType)
                pageStack.pop()
            }
        }

        VerticalScrollDecorator { }
    }

    ListModel {
        id: sortModel

        ListElement {
            sortType: "hot"
        }

        ListElement {
            sortType: "fresh"
        }
    }
}
