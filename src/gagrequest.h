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

#ifndef GAGREQUEST_H
#define GAGREQUEST_H

#include <QtNetwork/QNetworkReply>
#include <QtCore/QObject>
#include <QtCore/QList>

#include "networkmanager.h"
#include "gagobject.h"
#include "gagmodel.h"

class GagRequest : public QObject
{
    Q_OBJECT

public:
    explicit GagRequest(NetworkManager *networkManager, QObject *parent = 0);

    void initiateGagsRequest();
    void fetchGags(int groupId, QString &section, QString &lastId);

signals:
    /*! Emit this if the network request succeeds on fetching the gags data and
     *  the content has been parsed successful.
     *  \p gagList Contains the parsed GagObjects. */
    void fetchGagsSuccess(const QList<GagObject> &gagList);

    /*! Emit this if the network request failed on fetching the gags data.
     *  \p errorMessage Contains the reason of the failure. */
    void fetchGagsFailure(const QString &errorMessage);

    /*! Emit this to initiate the request to fetch the data for the gags/posts.
     *  \sa fetchGags, fetchGagsImpl */
    void readyToRequestGags();

    /*! Emit this if there are no further gags/posts available for the current
     *  section/groupId and therefore the end of the list has been reached. */
    void reachedEndOfList();

protected:
    /*! Get the global instance of NetworkManager. */
    NetworkManager *networkManager() const;

    /*! Implement this to start/send the request by emitting the readyToRequestGags
     *  signal. This function is useful if some preparation is needed prior to
     *  the request or to achieve a specific state of the derived GagRequest
     *  object (e.g. a login).
     *  \sa readyToRequestGags */
    virtual void startGagsRequest() = 0;

    /*! Implement this to make your own network request.\p groupId The id to select
     * between the different sections/groups. \p section Specifies the section from
     * which the gags should be fetched, eg. 'hot'. \p lastId is the id of the last
     * gag (it has to be an empty QString object if the latest gags that should be
     * fetched). */
    virtual QNetworkReply *fetchGagsImpl(const int groupId, const QString &section,
                                         const QString &lastId) = 0;

    /*! Implement this to parse the network response to a list of GagObjects.
     *  \p response This is the content of the network reply. */
    virtual QList<GagObject> parseGags(const QByteArray &response) = 0;

private slots:
    void onFetchGagsFinished();

private:
    NetworkManager *m_networkManager;
    QNetworkReply *m_gagsReply;
    QList<GagObject> m_gagList;
};

#endif // GAGREQUEST_H
