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

#include "gagrequest.h"

#include <QtNetwork/QNetworkReply>

#include "networkmanager.h"

GagRequest::GagRequest(NetworkManager *networkManager, const QString &section, QObject *parent) :
    QObject(parent), m_networkManager(networkManager), m_reply(0), m_groupId(-1), m_section(section),
    m_noMorePosts(false)
{
}

GagRequest::GagRequest(NetworkManager *networkManager, const int groupId, const QString &section,
                       QObject *parent)
    : QObject(parent), m_networkManager(networkManager), m_reply(0), m_groupId(groupId), m_section(section),
      m_noMorePosts(false)
{
}

void GagRequest::setLastId(const QString &lastId)
{
    m_lastId = lastId;
}

void GagRequest::setNoMorePosts(bool noMorePosts)
{
    m_noMorePosts = noMorePosts;
    // ToDo: emit signal/change state in corresponding view
}

void GagRequest::initiateRequest()
{
    startRequest();
}

void GagRequest::send()
{
    Q_ASSERT(m_reply == 0);

    m_reply = createRequest(m_groupId, m_section, m_lastId);

    // make sure the QNetworkReply will be destroyed when this object is destroyed
    m_reply->setParent(this);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()), Qt::UniqueConnection);
}

void GagRequest::onFinished()
{
    if (m_reply->error()) {
        qDebug() << "QNetworkReply error: " << m_reply->error() << "\nQNetworkReply object: " << m_reply->readAll();
        QString errorStr = m_reply->errorString();
        m_reply->disconnect();
        m_reply->deleteLater();
        m_reply = 0;
        emit failure(errorStr);
        return;
    }

    QByteArray response = m_reply->readAll();
    m_reply->disconnect();
    m_reply->deleteLater();
    m_reply = 0;

    m_gagList = parseResponse(response);
    if (m_gagList.isEmpty()) {
        if (m_noMorePosts)
            emit failure("Reached end of the list. There are no further posts available");
        else
            emit failure("Unable to parse response");
    }
    else
        emit success(m_gagList);
}

NetworkManager *GagRequest::networkManager() const
{
    return m_networkManager;
}
