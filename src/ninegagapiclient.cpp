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


// Defining API URLs:
const QByteArray API_URL = "https://api.9gag.com";
const QByteArray COMMENT_URL = "https://comment.9gag.com";
const QByteArray COMMENT_CDN_URL = "https://comment-cdn.9gag.com";
const QByteArray NOTIFY_URL = "https://notify.9gag.com";
const QByteArray AD_URL = "https://ad.9gag.com";
const QByteArray ADMIN_URL = "https://admin.9gag.com";

// Defining API paths:
const QByteArray POSTS_PATH = "/v2/post-list";
const QByteArray GUEST_PATH = "/v2/guest-token";
const QByteArray LOGIN_PATH = "/v2/user-token";
// "/v1/topComments.json"
// "/v1/comment.json"

// Defining App constants:
const QByteArray APP_ID = "com.ninegag.android.app";
const QByteArray DEVICE_TYPE = "android";
const QByteArray BUCKET_NAME = "__DEFAULT__";   // "MAIN_RELEASE";
const QByteArray COMMENT_CDN = "a_dd8f2b7d304a10edaf6f29517ea0ca4100a43d1b";


NineGagApiClient::NineGagApiClient(QObject *parent) :
    QObject(parent), m_appToken(createSHA1()), m_deviceUUID(createUUID()), m_loginReply(0),
    m_tokenExpiry(0), m_isGuestSession(true)
{

}

QByteArray NineGagApiClient::getTimestamp()
{
    return QByteArray::number(QDateTime::currentMSecsSinceEpoch());
}

QByteArray NineGagApiClient::createSHA1()
{
    return QCryptographicHash::hash(getTimestamp(), QCryptographicHash::Sha1).toHex();
}

QByteArray NineGagApiClient::createMD5(const QString &str)
{
    return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Md5).toHex();
}

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

QByteArray NineGagApiClient::createRequestSig(const QByteArray &timestamp)
{
    QString str = QString("*%1_._%2._.%3").arg(QString(timestamp)).arg(QString(APP_ID)).arg(QString(m_deviceUUID));
    str.append("9GAG");     // since QString::arg() would replace the 9 in '9GAG'

    return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha1).toHex();
}

QNetworkReply *NineGagApiClient::request(NetworkManager *netMan, const QUrl &url, bool sign)
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

    return netMan->createGetRequest(netReq);
}

void NineGagApiClient::guestLogin(NetworkManager *netMan)
{
    if (!m_isGuestSession) {
        m_isGuestSession = true;
    }

    QUrl url(API_URL + GUEST_PATH);

    Q_ASSERT(m_loginReply == 0);

    m_loginReply = this->request(netMan, url);
    connect(m_loginReply, SIGNAL(finished()), this, SLOT(guestLoginFinished()), Qt::UniqueConnection);
}

void NineGagApiClient::userLogin(NetworkManager *netMan, const QString &username, const QString &password)
{
    if (m_isGuestSession) {
        m_isGuestSession = false;
    }

    QUrl url(API_URL + LOGIN_PATH);
    QUrlQuery query;

    query.addQueryItem("loginMethod", "9gag");  // ToDo: "email"?
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

    m_loginReply = this->request(netMan, url);
    connect(m_loginReply, SIGNAL(finished()), this, SLOT(userLoginFinished()), Qt::UniqueConnection);
}

void NineGagApiClient::login(NetworkManager *netMan, bool guest, const QString &username, const QString &password)
{
    if (guest) {
        this->guestLogin(netMan);
    }
    else {
        this->userLogin(netMan, username, password);
    }
}

// first a login is required to access the posts
QNetworkReply *NineGagApiClient::getPosts(NetworkManager *netMan, const QString &section, const QString &lastId)
{
    QUrl url(API_URL + POSTS_PATH);
    QUrlQuery query;

    // set query arguments
    query.addQueryItem("group", "1");       // posts category
    query.addQueryItem("type", section);
    query.addQueryItem("itemCount", "10");  // count of posts
    // ToDo: disabled album and video posts until support is added | "animated,photo,video,album"
    query.addQueryItem("entryTypes", "animated,photo");
    query.addQueryItem("offset", "10");

    if (!lastId.isEmpty()) {
        query.addQueryItem("olderThan", lastId);
    }

    url.setQuery(query.query());

    // convert url query to the strange 9GAG specific scheme
    //QString tmp = url.toString().replace(QChar('?'), "/").replace(QChar('&'), "/").replace(QChar('='), "/");
    //url = QUrl(tmp);
    //qDebug() << "Constructed request URL: " << url;

    return this->request(netMan, url);
}

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

bool NineGagApiClient::isGuestSession()
{
    if (m_isGuestSession) {
        return true;
    }

    return false;
}

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
