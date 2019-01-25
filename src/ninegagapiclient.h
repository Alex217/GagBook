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

#ifndef NINEGAGAPICLIENT_H
#define NINEGAGAPICLIENT_H

#include <QObject>

#include "networkmanager.h"

class NineGagApiClient : public QObject
{
    Q_OBJECT

public:
    enum SortOrder {
        Score,
        Time
    };

    enum SortDirection {
        Ascending,
        Descending
    };

    NineGagApiClient(NetworkManager *netMan, QObject *parent = 0);
    void login(bool guest, const QString &username = QString(), const QString &password = QString());
    bool sessionIsValid();
    bool isGuestSession();
    QNetworkReply *getPosts(const int groupId, const QString &section, const QString &lastId);
    QNetworkReply *getComments(const QUrl &gagUrl, const int count, const int level, QString refComment,
                               SortOrder sortOrder, SortDirection sortDirection,
                               const QString &auth = QString());
    QNetworkReply *retrieveSections();

signals:
    void loggedIn();

private slots:
    void guestLoginFinished();
    void userLoginFinished();

private:
    QByteArray getTimestamp();
    QByteArray createSHA1();
    QByteArray createMD5(const QString &str);
    QByteArray createUUID();
    QByteArray createRequestSig(const QByteArray &timestamp);
    QNetworkReply *request(QNetworkRequest netReq);
    QNetworkReply *request(const QUrl &url, bool sign = true);
    void guestLogin();
    void userLogin(const QString &username, const QString &password);

    NetworkManager *m_netMan;
    QByteArray m_appToken;
    QByteArray m_deviceUUID;
    QNetworkReply *m_loginReply;
    quint32 m_tokenExpiry;
    bool m_isGuestSession;
};

#endif // NINEGAGAPICLIENT_H
