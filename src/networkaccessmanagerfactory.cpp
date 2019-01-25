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

#include "networkaccessmanagerfactory.h"

#include <QNetworkDiskCache>
#include <QStandardPaths>

static const QString NETWORK_CACHE_PATH = QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
        .append("/harbour-gagbook/network");

class MyNetworkAccessManager : public QNetworkAccessManager
{
public:
    MyNetworkAccessManager(QObject *parent) : QNetworkAccessManager(parent)
    {
    }

protected:
    QNetworkReply *createRequest(Operation operation, const QNetworkRequest &request,
                                 QIODevice *outgoingData = nullptr) override
    {
        QNetworkRequest cacheRequest(request);
        cacheRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, //QNetworkRequest::PreferCache);
                                  (networkAccessible() == QNetworkAccessManager::Accessible) ?
                                      QNetworkRequest::PreferCache : QNetworkRequest::AlwaysCache);

        return QNetworkAccessManager::createRequest(operation, cacheRequest, outgoingData);
    }
};

QNetworkAccessManager *NetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *manager = new MyNetworkAccessManager(parent);
    QNetworkDiskCache *diskCache = new QNetworkDiskCache(manager);
    diskCache->setCacheDirectory(NETWORK_CACHE_PATH);

    manager->setCache(diskCache);
    // TODO set a cookie?
    return manager;
}
