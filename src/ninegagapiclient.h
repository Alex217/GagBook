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

/*!
 * \brief The NineGagApiClient class accesses the JSON database via the undisclosed official 9GAG API.
 */
class NineGagApiClient : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief NineGagApiClient Constructor.
     * \param parent The parent object.
     */
    NineGagApiClient(QObject *parent = 0);

    /*!
     * \brief login Performs a login to the 9GAG API server.
     * \param netMan Pointer to the global NetworkManager instance.
     * \param guest Determines whether to perform a login as user or guest (true for guest login).
     * \param username The username e-mail string (obsolete if guest login is used).
     * \param password The password string (obsolete if guest login is used).
     */
    void login(NetworkManager *netMan, bool guest, const QString &username = QString(),
               const QString &password = QString());

    /*!
     * \brief getPosts Performs an API request and returns the response as QNetworkReply.
     *        Be aware that first a login has to be performed to access the servers.
     * \param netMan Pointer to the global NetworkManager instance.
     * \param section The 9GAG section string as shown in the url of a browser session (e.g. 'hot', 'fresh').
     * \param lastId The id of the last GagObject in the list.
     * \return Returns the JSON reply as QNetworkReply.
     */
    QNetworkReply *getPosts(NetworkManager *netMan, const QString &section, const QString &lastId);

    /*!
     * \brief sessionIsValid Checks if the login/session is still valid.
     * \return Returns true if the session is still valid and a re-login is not needed.
     */
    bool sessionIsValid();

    /*!
     * \brief isGuestSession Returns the current login state (logged in as guest or user).
     * \return Returns true if the API client has been logged in as guest.
     */
    bool isGuestSession();

signals:
    void loggedIn();//bool successfully, QString &error);

private slots:
    void guestLoginFinished();
    void userLoginFinished();

private:
    QByteArray m_appToken;
    QByteArray m_deviceUUID;
    QNetworkReply *m_loginReply;
    quint32 m_tokenExpiry;
    bool m_isGuestSession;

    /*!
     * \brief getTimestamp Generates the current system time in milliseconds.
     * \return Returns the current (POSIX) system time in milliseconds.
     */
    QByteArray getTimestamp();

    /*!
     * \brief createSHA1 Generates a SHA-1 hash sum based on the current system time.
     * \return Returns the QByteArray representation formatted as a hex digit.
     */
    QByteArray createSHA1();

    /*!
     * \brief createMD5 Generates a MD5 hash sum for the given QString.
     * \param str The data to generate the MD5 hash sum.
     * \return Returns a hex encoded MD5 hash for the given QString.
     */
    QByteArray createMD5(const QString &str);

    /*!
     * \brief createUUID Generates a QUuid based on random numbers and returns it as a QString.
     * \return Returns the QByteArray representation formatted as five hex digits.
     */
    QByteArray createUUID();

    /*!
     * \brief createRequestSig Generates a specific hash sum to sign the HTTP request.
     * \param timestamp The timestamp needed for the signature generation.
     * \return Returns the generated request signature as a QByteArray representation.
     */
    QByteArray createRequestSig(const QByteArray &timestamp);

    /*!
     * \brief request Sends the API request with the given data.
     * \param netMan Pointer to the global NetworkManager instance.
     * \param url The url to which the request is directed to.
     * \param sign Signs the request if true.
     * \return Returns the pointer to the QNetworkReply object of the request.
     */
    QNetworkReply *request(NetworkManager *netMan, const QUrl &url, bool sign = true);

    /*!
     * \brief guestLogin Performs a guest login on the 9GAG API server.
     * \param netMan Pointer to the global NetworkManager instance.
     */
    void guestLogin(NetworkManager *netMan);

    /*!
     * \brief userLogin Performs a user login on the 9GAG API server.
     * \param netMan Pointer to the global NetworkManager instance.
     * \param username The username string (e-mail).
     * \param password The password string.
     */
    void userLogin(NetworkManager *netMan, const QString &username, const QString &password);
};

#endif // NINEGAGAPICLIENT_H
