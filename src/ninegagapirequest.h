/*
 * Copyright (C) 2018 Alexander Seibel.
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

#ifndef NINEGAGAPIREQUEST_H
#define NINEGAGAPIREQUEST_H

#include "gagrequest.h"
#include "ninegagapiclient.h"

/*!
 * \brief The NineGagApiRequest class accesses the 9GAG API via the NineGagApiClient and parses the response to
 *        a list of gags. \sa NineGagApiClient
 */
class NineGagApiRequest : public GagRequest
{
    Q_OBJECT

public:
    /*!
     * \brief NineGagApiRequest Constructor.
     * \param networkManager Pointer to the global NetworkManager instance.
     * \param groupId The id to select between the different 9GAG sections/groups.
     * \param section Specifies the 9GAG section from which the gags should be fetched.
     * \param parent The parent object.
     */
    explicit NineGagApiRequest(NetworkManager *networkManager, const int groupId, const QString &section,
                               QObject *parent = 0);
    ~NineGagApiRequest();

protected:
    void startRequest();
    QNetworkReply *createRequest(const int groupId, const QString &section, const QString &lastId);
    QList<GagObject> parseResponse(const QByteArray &response);

private slots:
    void onLogin();

private:
    NineGagApiClient *m_apiClient;
    bool m_loginOngoing;
};

#endif // NINEGAGAPIREQUEST_H
