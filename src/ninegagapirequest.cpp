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

//#include <QJsonParseError>
#include <QTextDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

#include "ninegagapirequest.h"
#include "networkmanager.h"


NineGagApiRequest::NineGagApiRequest(NetworkManager *networkManager, const int groupId, const QString &section,
                                     QObject *parent)
    : GagRequest(networkManager, groupId, section, parent), m_apiClient(new NineGagApiClient(this))
{
    connect(m_apiClient, SIGNAL(loggedIn()), this, SLOT(onLogin()), Qt::UniqueConnection);
    m_loginOngoing = true;

    // ToDo: retrieve login data via QML
    //m_apiClient->login(this->networkManager(), false, "username", "password");

    m_apiClient->login(this->networkManager(), true);
}

NineGagApiRequest::~NineGagApiRequest()
{
    m_apiClient->deleteLater();
}

void NineGagApiRequest::startRequest()
{
    if (!m_loginOngoing) {

        // check if a re-login is required
        if (m_apiClient->sessionIsValid()) {
            emit readyToRequest();
        }
        else {
            qDebug() << "Performing a re-login...";

            // perform a re-login according to previous state
            if (m_apiClient->isGuestSession()) {
                m_loginOngoing = true;
                m_apiClient->login(this->networkManager(), true);
            }
            else {
                // ToDo: perform a user login
            }
        }
    }
}

QNetworkReply *NineGagApiRequest::createRequest(const int groupId, const QString &section, const QString &lastId)
{
    return m_apiClient->getPosts(this->networkManager(), groupId, section, lastId);
}

QList<GagObject> NineGagApiRequest::parseResponse(const QByteArray &response)
{
    QJsonObject rootObj = QJsonDocument::fromJson(response).object();
    QJsonArray postsArr = rootObj.value("data").toObject().value("posts").toArray();
    const QVariantList postsList = postsArr.toVariantList();

    if (postsList.isEmpty()) {
        qWarning("Empty JSON API response!");
        qDebug() << "### API response is: ###\n" << response;
    }

    QList<GagObject> gagList;

    foreach (const QVariant &gagJson, postsList) {
        const QVariantMap gagMap = gagJson.toMap();

        GagObject gag;
        gag.setId(gagMap.value("id").toString());

        // url: change url to mobile website
        QString urlStr = gagMap.value("url").toString();
        urlStr = urlStr.insert((urlStr.indexOf(":") + 3), QString("m."));
        gag.setUrl(QUrl(urlStr));
        //gag.setUrl(gagMap.value("url").toUrl());

        // title: convert included entity numbers
        QString titleStr = gagMap.value("title").toString();
        QTextDocument textDoc;
        textDoc.setHtml(titleStr);
        gag.setTitle(textDoc.toPlainText());

        gag.setVotesCount(gagMap.value("totalVoteCount").toInt());
        gag.setCommentsCount(gagMap.value("commentsCount").toInt());
        bool nsfw = gagMap.value("nsfw").toBool();

        // ToDo: disabled until user login is fully implemented
        if (nsfw) {
            //gag.setIsNSFW(true);

            // ToDo: if set then don't load content if client is not logged in as
            //       user, since the json still contains the urls with a guest login

            //qDebug() << "Following Gag is a NSFW gag: " << gag.title();
        }

        QString gagType = gagMap.value("type").toString();
        QVariantMap imagesMap = gagMap.value("images").toMap();

        // ToDo: set 'imageSize' for each gag

        if (gagType == QString("Photo")) {
            bool longPost = gagMap.value("hasLongPostCover").toBool();

            // Image
            if (!longPost) {
                gag.setImageUrl(imagesMap.value("image700").toMap().value("url").toUrl());
            }
            // Long image
            else {
                gag.setIsPartialImage(true);
                gag.setImageUrl(imagesMap.value("image460c").toMap().value("url").toUrl());
                gag.setFullImageUrl(imagesMap.value("image700").toMap().value("url").toUrl());

                //qDebug() << "Following Gag is a long image: " << gag.title();
            }
        }
        else if (gagType == QString("Animated")) {

            // 9gag provides GIFs only as a video source
            gag.setIsVideo(true);
            gag.setImageUrl(imagesMap.value("image700").toMap().value("url").toUrl());
            gag.setVideoUrl(imagesMap.value("image460sv").toMap().value("url").toUrl());

            /*
            int duration = imagesMap.value("image460sv").toMap().value("duration").toInt();

            // GIF
            if (!duration) {
                gag.setIsGIF(true);
                gag.setImageUrl(imagesMap.value("image700").toMap().value("url").toUrl());
                //gag.setGifImageUrl(imagesMap.value("image700ba").toMap().value("url").toUrl());   // GIF (urls not existend)
                gag.setGifImageUrl(imagesMap.value("image460sv").toMap().value("url").toUrl());   // mp4-Video

                // ToDo: update model to support 'GIFs' with video source
            }
            // mp4-Video
            else {
                gag.setIsVideo(true);
                gag.setImageUrl(imagesMap.value("image700").toMap().value("url").toUrl());
                gag.setVideoUrl(imagesMap.value("image460sv").toMap().value("url").toUrl());
            }
            */
        }
        else if (gagType == QString("Album")) {
            // An Album consists of multiple separate gags with its own id, all specified in the 'article' json object

            gag.setImageUrl(imagesMap.value("image700").toMap().value("url").toUrl());
            qDebug() << "Found an unsupported 'Album' Gag: " << gag.title();

            // ToDo: extend the model to support albums
            // ToDo: for the meantime add a warning message to the user
        }
        else {      // e.g. "Video", which is a Youtube video
            qWarning("Found an unsupported gag type!");
            // ToDo: extend the model to support videos
            // ToDo: for the meantime add a warning message to the user
        }

        gagList.append(gag);
    }

    return gagList;
}

void NineGagApiRequest::onLogin()
{
    m_loginOngoing = false;
    emit readyToRequest();
}
