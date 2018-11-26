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

#include "gagbookmanager.h"

#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QUrlQuery>
#include <QtCore/QDateTime>

#include "networkmanager.h"
#include "gagimagedownloader.h"
#include "appsettings.h"

GagBookManager::GagBookManager(QObject *parent) :
    QObject(parent), m_isBusy(false), m_settings(0),
    m_netManager(new NetworkManager(this)), m_loginReply(0)
{
    GagImageDownloader::initializeCache();
    connect(m_netManager, SIGNAL(downloadCounterChanged()), SIGNAL(downloadCounterChanged()));
}

bool GagBookManager::isBusy() const
{
    return m_isBusy;
}

QString GagBookManager::downloadCounter() const
{
    return m_netManager->downloadCounter();
}

AppSettings *GagBookManager::settings() const
{
    return m_settings;
}

void GagBookManager::setSettings(AppSettings *settings)
{
    m_settings = settings;
}

NetworkManager *GagBookManager::networkManager() const
{
    return m_netManager;
}

void GagBookManager::login(const QString &username, const QString &password)
{
    Q_ASSERT(m_netManager);

    if (m_loginReply != 0) {
        m_loginReply->disconnect();
        m_loginReply->deleteLater();
        m_loginReply = 0;
    }

    if (m_isBusy != true) {
        m_isBusy = true;
        emit busyChanged();
    }

    QByteArray postData;
    QUrlQuery postDataQuery;
    postDataQuery.addQueryItem("username", username);
    postDataQuery.addQueryItem("password", password);
    postData = postDataQuery.toString(QUrl::FullyEncoded).toUtf8();

    m_loginReply = m_netManager->createPostRequest(QUrl("https://9gag.com/login"), postData);
    connect(m_loginReply, SIGNAL(finished()), SLOT(onLoginFinished()));
}

void GagBookManager::logout()
{
    //we log out by removing the loggedin cookie
    m_netManager->clearCookies();
    m_settings->setLoggedIn(false);
}

void GagBookManager::onLoginFinished()
{
    if(m_loginReply->error() == QNetworkReply::NoError) {
        const int statusCode = m_loginReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QUrl redirectUrl = m_loginReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        // if redirect to 9gag.com, means login success
        if (statusCode >= 300 && statusCode < 400 && redirectUrl.host() == "9gag.com") {
            m_settings->setLoggedIn(true);
            emit loginSuccess();
        } else {
            emit loginFailure("Wrong username or password. Please try again.");
        }
    } else {
        emit loginFailure(m_loginReply->errorString());
    }

    m_loginReply->deleteLater();
    m_loginReply = 0;

    if (m_isBusy != false) {
        m_isBusy = false;
        emit busyChanged();
    }
}
