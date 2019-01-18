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

#include "qmlutils.h"

#include <QImage>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtGui/QClipboard>
#include <QtCore/QStandardPaths>
#include <QtGui/QGuiApplication>

QMLUtils::QMLUtils(QObject *parent) :
    QObject(parent)
{
}

int QMLUtils::imageMaxHeight() const
{
    return 4096;
    // TODO: macro seems to be not working, gives the value 3379 for different devices
    //return GL_MAX_TEXTURE_SIZE;
}

QUrl QMLUtils::repoWebsite() const
{
    return QUrl("https://github.com/Alex217/GagBook");
}

void QMLUtils::copyToClipboard(const QString &text)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text, QClipboard::Clipboard);

#ifdef Q_WS_SIMULATOR
    qDebug("Copied to clipboard: %s", qPrintable(text));
#endif
}

QString QMLUtils::saveImage(const QUrl &imageUrl, bool isLongImage)
{
    // if the url is not a local file, return
    if (!imageUrl.isLocalFile())
        return QString("");

    // create the image saving directory if not existent
    QDir fileSavingDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/GagBook");

    if (!fileSavingDir.exists())
        fileSavingDir.mkpath(".");

    const QString imagePath = imageUrl.toLocalFile();
    const QString copyPath = fileSavingDir.absoluteFilePath(imagePath.mid(imagePath.lastIndexOf("/") + 1));
    bool success;

    if (isLongImage) {
        // scale the long image to a supported height so that the gallery is able to open/render it without downscaling
        QImage longImg(imagePath);
        if (!longImg.isNull()) {
            if (longImg.height() > imageMaxHeight()) {
                // using 'SmoothTransformation' may take a few seconds on very long images (e.g. height > 10000)
                QImage scaledImg(longImg.scaledToHeight(imageMaxHeight(), Qt::SmoothTransformation));
                success = scaledImg.save(copyPath);
            }
            else
                success = QFile::copy(imagePath, copyPath);
        }
        else
            success = false;
    }
    else {
        success = QFile::copy(imagePath, copyPath);
    }

    // use QUrl to get the 'file://' scheme so that it can be opened using Qt.openUrlExternally() in QML
    return ((success || QFile::exists(copyPath)) ? QUrl::fromLocalFile(copyPath).toString() : QString(""));
}

bool QMLUtils::fileExists(const QUrl &url)
{
    return QFile::exists(url.toLocalFile());
}

/*!
 * \brief QMLUtils::toMobileUrl Utility method to convert a given URL to the mobile URL.
 * \param url The URL which should be converted to the mobile version. Note that the
 *  input URL at least has to contain the substring 'http://'.
 * \param convertToHttps Set this to true to convert a HTTP link to a HTTPS link.
 * \return Returns the mobile representation of the given URL. If the URL is wrongly
 *  formatted the passed URL will be returned.
 */
QUrl QMLUtils::toMobileUrl(const QUrl &url, bool convertToHttps)
{
    QString urlStr = url.toString();
    int colonPos = urlStr.indexOf(":");

    if (colonPos == -1) {
        qWarning("QMLUtils::toMobileUrl(): Wrongly formatted input URL!");
        return url;
    }

    urlStr = urlStr.insert(colonPos + 3, QString("m."));

    if (convertToHttps && (urlStr.at(colonPos - 1) == QChar('p'))) {
        urlStr = urlStr.insert(colonPos, QString("s"));
    }

    return QUrl(urlStr);
}

/*!
 * \brief QMLUtils::fromMobileUrl Utility method to convert a given URL from the mobile
 *  representation to the desktop URL.
 * \param url The URL which should be converted to the desktop version. Note that the
 *  input URL at least has to contain the substring 'http://'.
 * \param convertToHttps Set this to true to convert a HTTP link to a HTTPS link.
 * \return Returns the desktop representation of the given URL. If the URL is wrongly
 *  formatted the passed URL will be returned.
 */
QUrl QMLUtils::fromMobileUrl(const QUrl &url, bool convertToHttps)
{
    QString urlStr = url.toString();
    int colonPos = urlStr.indexOf(":");

    if (colonPos == -1) {
        qWarning("QMLUtils::fromMobileUrl(): Wrongly formatted input URL!");
        return url;
    }

    if (urlStr.at(colonPos + 3) == QChar('m')) {
        urlStr = urlStr.remove(colonPos + 3, 2);
    }

    if (convertToHttps && (urlStr.at(colonPos - 1) == QChar('p'))) {
        urlStr = urlStr.insert(colonPos, QString("s"));
    }

    return QUrl(urlStr);
}

void QMLUtils::shareLink(const QString &link, const QString &title)
{
    qWarning("QMLUtils::shareLink(): This function is not implemented yet.");
    Q_UNUSED(title)
    Q_UNUSED(link)
}
