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

#include <QCryptographicHash>
#include <QNetworkReply>
#include <QDateTime>
#include <QUrlQuery>
#include <QUuid>

//#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>

#include "ninegagapiclient.h"

/*!
    \class NineGagApiClient
    \since 1.3.0
    \brief The NineGagApiClient class provides the data via the undisclosed official 9GAG.com API.
*/

// Defining API URLs:
const QByteArray API_URL = "https://api.9gag.com";
const QByteArray COMMENT_URL = "https://comment.9gag.com";
const QByteArray COMMENT_URL_CDN = "https://comment-cdn.9gag.com";
const QByteArray NOTIFY_URL = "https://notify.9gag.com";
const QByteArray ADMIN_URL = "https://admin.9gag.com";

// Defining API paths:
const QByteArray POSTS_PATH = "/v2/post-list";
const QByteArray GUEST_PATH = "/v2/guest-token";
const QByteArray LOGIN_PATH = "/v2/user-token";
const QByteArray SECTIONS_PATH = "/v2/group-list";
const QByteArray COMMENT_PATH = "/v1/comment.json";
const QByteArray COMMENT_PATH_CDN = "/v1/cacheable/comment-list.json";
const QByteArray COMMENTINFO_PATH = "/v1/commentInfo.json";
const QByteArray COMMENTSDATA_PATH = "/v1/commentsData.json";
const QByteArray TOPCOMMENTS_PATH = "/v1/topComments.json";

// Defining App constants:
const QByteArray APP_ID = "com.ninegag.android.app";
const QByteArray DEVICE_TYPE = "android";
const QByteArray BUCKET_NAME = "MAIN_RELEASE";   // "__DEFAULT__";
const QByteArray COMMENT_ID_CDN = "a_dd8f2b7d304a10edaf6f29517ea0ca4100a43d1b";

/*!
 * \brief NineGagApiClient::NineGagApiClient Constructor.
 * \param netMan Pointer to the global NetworkManager instance.
 * \param parent The parent object.
 */
NineGagApiClient::NineGagApiClient(NetworkManager *netMan, QObject *parent) :
    QObject(parent), m_netMan(netMan), m_appToken(createSHA1()), m_deviceUUID(createUUID()),
    m_loginReply(0), m_tokenExpiry(0), m_isGuestSession(true)
{

}

/*!
 * \brief NineGagApiClient::getTimestamp Generates the current system time in milliseconds.
 * \return Returns the current (POSIX) system time in milliseconds.
 */
QByteArray NineGagApiClient::getTimestamp()
{
    return QByteArray::number(QDateTime::currentMSecsSinceEpoch());
}

/*!
 * \brief NineGagApiClient::createSHA1 Generates a SHA-1 hash sum based on the current system time.
 * \return Returns the QByteArray representation formatted as a hex digit.
 */
QByteArray NineGagApiClient::createSHA1()
{
    return QCryptographicHash::hash(getTimestamp(), QCryptographicHash::Sha1).toHex();
}

/*!
 * \brief NineGagApiClient::createMD5 Generates a MD5 hash sum for the given QString.
 * \param str The data to generate the MD5 hash sum.
 * \return  Returns a hex encoded MD5 hash for the given QString.
 */
QByteArray NineGagApiClient::createMD5(const QString &str)
{
    return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Md5).toHex();
}

/*!
 * \brief NineGagApiClient::createUUID Generates a QUuid based on random numbers and returns it as a QString.
 * \return Returns the QByteArray representation formatted as five hex digits.
 */
QByteArray NineGagApiClient::createUUID()
{
    // remove '{', '}' and '-' from the string
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(0, 1).chop(1);
    uuid.remove(QChar('-'));
    return uuid.toUtf8();

    // formatted as five hex digits: '{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}'
    //return QUuid::createUuid().toByteArray();
}

/*!
 * \brief NineGagApiClient::createRequestSig Generates a specific hash sum to sign the HTTP request.
 * \param timestamp The timestamp needed for the signature generation.
 * \return Returns the generated request signature as a QByteArray representation.
 */
QByteArray NineGagApiClient::createRequestSig(const QByteArray &timestamp)
{
    QString str = QString("*%1_._%2._.%3").arg(QString(timestamp)).arg(QString(APP_ID)).arg(QString(m_deviceUUID));
    str.append("9GAG");     // since QString::arg() would replace the 9 in '9GAG'

    return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha1).toHex();
}

/*!
 * \brief NineGagApiClient::request Sends the API request with the given data.
 * \param netReq The QNetworkRequest object that should be used for the request.
 * \return Returns the pointer to the QNetworkReply object of the request.
 */
QNetworkReply *NineGagApiClient::request(QNetworkRequest netReq)
{
    return m_netMan->createGetRequest(netReq);
}

/*!
 * \brief NineGagApiClient::request Sends the API request with the given data.
 * \param url The url to which the request is directed to.
 * \param sign Set this to true to sign the request.
 * \return Returns the pointer to the QNetworkReply object of the request.
 */
QNetworkReply *NineGagApiClient::request(const QUrl &url, bool sign)
{
    QNetworkRequest netReq;

    netReq.setUrl(url);
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //netReq.setRawHeader("Authorization", "Basic " + QByteArray(QString(
    //                    "%1:%2").arg("email").arg("password").toLocal8Bit()).toBase64());
    //netReq.setRawHeader("Accept", "application/json");
    //netReq.setRawHeader("Connection", "keep-alive");
    //netReq.setRawHeader("Accept-Encoding", "gzip, deflate");
    netReq.setRawHeader("9GAG-9GAG_TOKEN", m_appToken);
    netReq.setRawHeader("9GAG-TIMESTAMP", getTimestamp());
    netReq.setRawHeader("9GAG-APP_ID", APP_ID);
    netReq.setRawHeader("X-Package-ID", APP_ID);
    netReq.setRawHeader("9GAG-DEVICE_UUID", m_deviceUUID);
    netReq.setRawHeader("X-Device-UUID", m_deviceUUID);
    netReq.setRawHeader("9GAG-DEVICE_TYPE", DEVICE_TYPE);
    netReq.setRawHeader("9GAG-BUCKET_NAME", BUCKET_NAME);

    if (sign) {
        netReq.setRawHeader("9GAG-REQUEST-SIGNATURE", createRequestSig(netReq.rawHeader("9GAG-TIMESTAMP")));
    }

    // this attribute was introduced with Qt 5.6 and Sailfish OS 2.1.0.x
    //netReq.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    return this->request(netReq);
}

/*!
 * \brief NineGagApiClient::guestLogin Performs a guest login on the API server.
 */
void NineGagApiClient::guestLogin()
{
    if (!m_isGuestSession) {
        m_isGuestSession = true;
    }

    QUrl url(API_URL + GUEST_PATH);

    Q_ASSERT(m_loginReply == 0);

    m_loginReply = this->request(url);
    connect(m_loginReply, &QNetworkReply::finished, this, &NineGagApiClient::guestLoginFinished,
            Qt::UniqueConnection);
}

/*!
 * \brief NineGagApiClient::userLogin Performs a user login on the API server.
 * \param username The username string (e-mail).
 * \param password The password string.
 */
void NineGagApiClient::userLogin(const QString &username, const QString &password)
{
    if (m_isGuestSession) {
        m_isGuestSession = false;
    }

    QUrl url(API_URL + LOGIN_PATH);
    QUrlQuery query;

    query.addQueryItem("loginMethod", "9gag");  // TODO: "email"?
    query.addQueryItem("loginName", username);
    query.addQueryItem("password", createMD5(password));
    query.addQueryItem("pushToken", createSHA1());
    query.addQueryItem("language", "en_US");
    url.setQuery(query.query());

    //url.setUserName("username", QUrl::TolerantMode);
    //url.setPassword(QString(createMD5("password")), QUrl::TolerantMode);

    // convert url query to the strange 9GAG specific scheme
    //QString tmp = url.toString().replace(QChar('?'), "/").replace(QChar('&'), "/").replace(QChar('='), "/");
    //url = QUrl(tmp);
    //qDebug() << "Constructed Login URL: " << url;

    Q_ASSERT(m_loginReply == 0);

    m_loginReply = this->request(url);
    connect(m_loginReply, &QNetworkReply::finished, this, &NineGagApiClient::userLoginFinished,
            Qt::UniqueConnection);
}

/*!
 * \brief NineGagApiClient::login Performs a login on the API server.
 * \param guest Determines whether to perform a login as user or guest (true for guest login).
 * \param username The username e-mail string (obsolete if guest login is used).
 * \param password The password string (obsolete if guest login is used).
 */
void NineGagApiClient::login(bool guest, const QString &username, const QString &password)
{
    if (guest) {
        this->guestLogin();
    }
    else {
        this->userLogin(username, password);
    }
}

/*!
 * \brief NineGagApiClient::sessionIsValid Checks if the login/session is still valid.
 * \return Returns true if the session is still valid and a re-login is not needed.
 */
bool NineGagApiClient::sessionIsValid()
{
    if (m_tokenExpiry == 0) {
        qWarning("First a login is required to fetch the posts!");
        return false;
    }
    else {
        // get current time in seconds
        quint32 currentTime = (quint32) (this->getTimestamp().toULongLong() / (quint32) 1000);

        // check for session expiry
        if (currentTime > m_tokenExpiry) {
            qDebug() << "The session expired. A re-login is required!";
            return false;
        }
    }

    return true;
}

/*!
 * \brief NineGagApiClient::isGuestSession Returns the current login state (logged in as guest or user).
 * \return Returns true if the API client has been logged in as guest.
 */
bool NineGagApiClient::isGuestSession()
{
    return m_isGuestSession;
}

/*!
 * \brief NineGagApiClient::getPosts Performs an API request and returns the response as QNetworkReply.
 *  Be aware that first a login has to be performed to access the servers.
 * \param groupId The id to select between the different sections/groups.
 * \param section Selects the 'subsection' inside the given group/section ('hot', 'trending' or 'vote').
 * \param lastId The id of the last GagObject in the list.
 * \return Returns the post data.
 */
QNetworkReply *NineGagApiClient::getPosts(const int groupId, const QString &section, const QString &lastId)
{
    QUrl url(API_URL + POSTS_PATH);
    QUrlQuery query;

    // set query arguments
    query.addQueryItem("group", QString::number(groupId));       // posts category
    //query.addQueryItem("type", section);

    // ToDo: This is just a quick fix for the sections! Adapt the model and GUI to fix this properly.
    // '-> "type" can be "hot" or "vote" for groups with 'groupId != 1'
    if (groupId == 1) {
        query.addQueryItem("type", section);
    } else {
        query.addQueryItem("type", "hot");
    }

    query.addQueryItem("itemCount", "9");  // count of posts
    // ToDo: disabled album and video posts until support is added | "animated,photo,video,article"
    query.addQueryItem("entryTypes", "animated,photo");
    //query.addQueryItem("offset", "10");

    if (!lastId.isEmpty()) {
        query.addQueryItem("olderThan", lastId);
    }

    url.setQuery(query.query());

    // convert url query to the strange 9GAG specific scheme
    //QString tmp = url.toString().replace(QChar('?'), "/").replace(QChar('&'), "/").replace(QChar('='), "/");
    //url = QUrl(tmp);
    //qDebug() << "Constructed request URL: " << url;

    return this->request(url);
}

/*!
 * \brief NineGagApiClient::getComments Retrieves the comment data for a given post.
 * \param gagUrl The URL of the post for that the comments should be fetched.
 * \param count The number of comments that should be loaded.
 * \param level
 * \param refComment The commentId/orderKey of a comment to load the comments beneath that comment.
 *  A given orderKey value loads top-level comments and a commentId value loads second-level comments.
 * \param sortOrder Note that for SortOrder::Time the posts are ordered by the timestamp-value and
 *  not by the elapsed time!
 * \param sortDirection
 * \param auth
 * \return Returns the retrieved comment data.
 */
QNetworkReply *NineGagApiClient::getComments(const QUrl &gagUrl, const int count, const int level,
                                             QString refComment, SortOrder sortOrder,
                                             SortDirection sortDirection, const QString &auth)
{
    QUrl reqUrl(COMMENT_URL_CDN + COMMENT_PATH_CDN);
    QUrlQuery query;

    query.addQueryItem("appId", COMMENT_ID_CDN);
    query.addQueryItem("url", QUrl::toPercentEncoding(gagUrl.toString()));

    // Set sort order
    switch (sortOrder) {
        default:
            qWarning("NineGagApiClient::getComments(): Using invalid sort order, "
                     "default value will be used!");
            // fall through
        case SortOrder::Score:
            query.addQueryItem("order", "score");
            break;
        case SortOrder::Time:
            query.addQueryItem("order", "ts");
            break;
    }

    // Set sort direction
    switch (sortDirection) {
        default:
        qWarning("NineGagApiClient::getComments(): Using invalid sort direction, "
                 "default value will be used!");
            // fall through
        case SortDirection::Descending:
            query.addQueryItem("direction", "desc");
            break;
        case SortDirection::Ascending:
            query.addQueryItem("direction", "asc");
            break;
    }

    // set refComment
    if (!refComment.isEmpty()) {
        // 'score_*'-value (orderKey) loads further top-level comments
        // 'c_*'-value (commentId) loads further second-level comments
        query.addQueryItem("ref", refComment);
        query.addQueryItem("refCommentId", refComment);
    }

    query.addQueryItem("count", QString::number(count));
    query.addQueryItem("level", QString::number(level));
    query.addQueryItem("auth", (auth.isEmpty() ? "null" : auth));

    reqUrl.setQuery(query.query());

    QNetworkRequest netReq;
    netReq.setUrl(reqUrl);
    netReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    netReq.setRawHeader("appId", COMMENT_ID_CDN);
    // User-Agent
    netReq.setRawHeader("X-Package-ID", APP_ID);
    netReq.setRawHeader("X-Device-UUID", m_deviceUUID);
    // X-Package-Version
    //netReq.setRawHeader("Connection", "keep-alive");
    //netReq.setRawHeader("Accept-Encoding", "gzip");

    return this->request(netReq);
}

// TODO
QNetworkReply *NineGagApiClient::retrieveSections()
{
    QUrl url(API_URL + SECTIONS_PATH);
    QUrlQuery query;

    query.addQueryItem("entryTypes", "animated,photo,video,article");
    //query.addQueryItem("locale", "de_DE");    // TODO: add device specific locale setting
    url.setQuery(query.query());

    return this->request(url);
}

/*!
 * \brief NineGagApiClient::guestLoginFinished This slot handles a finished guest login.
 */
void NineGagApiClient::guestLoginFinished()
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(m_loginReply->readAll());
    QJsonObject rootObj = jsonDoc.object();
    m_appToken = rootObj.value("data").toObject().value("userToken").toString().toUtf8();
    m_tokenExpiry = rootObj.value("data").toObject().value("tokenExpiry").toInt();  // valid for 72 hours

    if (m_loginReply != 0) {
        m_loginReply->disconnect();
        m_loginReply->deleteLater();
        m_loginReply = 0;
    }

    emit loggedIn();
}

/*!
 * \brief NineGagApiClient::userLoginFinished This slot handles a finished user login.
 */
void NineGagApiClient::userLoginFinished()
{
    // ToDo: check Login success

    //QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(m_loginReply->readAll());   //, &error);
    QJsonObject rootObj = jsonDoc.object();
    m_appToken = rootObj.value("data").toObject().value("userToken").toString().toUtf8();
    m_tokenExpiry = rootObj.value("data").toObject().value("tokenExpiry").toInt();  // valid for 12 hours

    // ToDo: check for parsing errors
    // ToDo: retrieve user data?

    if (m_loginReply != 0) {
        m_loginReply->disconnect();
        m_loginReply->deleteLater();
        m_loginReply = 0;
    }

    emit loggedIn();
}
