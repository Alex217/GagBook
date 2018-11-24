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
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtGui/QClipboard>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QtGui/QDesktopServices>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QStandardPaths>
#include <QtGui/QGuiApplication>
#else
#include <QtGui/QDesktopServices>
#include <QtGui/QApplication>
#endif

QMLUtils::QMLUtils(QObject *parent) :
    QObject(parent)
{
}

int QMLUtils::imageMaxHeight() const
{
    return 4096;
    // macro seems to be not working, gives the value 3379 for different devices
    //return GL_MAX_TEXTURE_SIZE;
}

QUrl QMLUtils::repoWebsite() const
{
    return QUrl("https://github.com/Alex217/GagBook");
}

void QMLUtils::copyToClipboard(const QString &text)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QClipboard *clipboard = QGuiApplication::clipboard();
#else
    QClipboard *clipboard = QApplication::clipboard();
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QDir fileSavingDir(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation));
#else
    QDir fileSavingDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/GagBook");
#endif
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

bool QMLUtils::fileExists(const QString &fileName)
{
    if (QFile::exists(QUrl(fileName).toLocalFile())) {
        return true;
    }
    else {
        return false;
    }
}

void QMLUtils::shareLink(const QString &link, const QString &title)
{
    qWarning("QMLUtils::shareLink(): This function is not implemented yet.");
    Q_UNUSED(title)
    Q_UNUSED(link)
}
