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

#ifndef COMMENTMEDIAOBJECT_H
#define COMMENTMEDIAOBJECT_H

#include <QUrl>
#include <QSize>
#include <QObject>
#include <QExplicitlySharedDataPointer>

class CommentMediaObjectData;

class CommentMediaObject
{
    Q_GADGET

public:
    enum CommentMediaType {
      Static,
      Animated,
      Invalid
    };

    Q_ENUM(CommentMediaType)

    explicit CommentMediaObject();
    CommentMediaObject(const CommentMediaObject &obj);
    CommentMediaObject &operator=(const CommentMediaObject &obj);
    ~CommentMediaObject();

    Q_INVOKABLE CommentMediaType mediaType();
    void setMediaType(CommentMediaType type);

    Q_INVOKABLE QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);
    Q_INVOKABLE QSize imageSize() const;
    void setImageSize(const QSize &size);

    Q_INVOKABLE QUrl gifUrl() const;
    void setGifUrl(const QUrl &url);
    Q_INVOKABLE QSize gifSize() const;
    void setGifSize(const QSize &size);

    Q_INVOKABLE QUrl videoUrl() const;
    void setVideoUrl(const QUrl &url);
    Q_INVOKABLE QSize videoSize() const;
    void setVideoSize(const QSize &size);

private:
    QExplicitlySharedDataPointer<CommentMediaObjectData> data;
};

Q_DECLARE_METATYPE(CommentMediaObject)

#endif // COMMENTMEDIAOBJECT_H
