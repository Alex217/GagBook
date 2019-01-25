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

#include "commentmediaobject.h"

class CommentMediaObjectData : public QSharedData
{
public:
    CommentMediaObjectData() : mediaType(CommentMediaObject::CommentMediaType::Invalid), imageUrl(QUrl()),
        imageSize(QSize()), gifUrl(QUrl()), gifSize(QSize()), videoUrl(QUrl()), videoSize(QSize())
    {

    }

    ~CommentMediaObjectData()
    {
        // TODO delete cached files
    }

    CommentMediaObject::CommentMediaType mediaType;

    QUrl imageUrl;
    QSize imageSize;

    QUrl gifUrl;
    QSize gifSize;

    QUrl videoUrl;
    QSize videoSize;

private:
    Q_DISABLE_COPY(CommentMediaObjectData)
};

CommentMediaObject::CommentMediaObject()
    : data(new CommentMediaObjectData)
{

}

CommentMediaObject::CommentMediaObject(const CommentMediaObject &obj)
    : data(obj.data)
{

}

CommentMediaObject &CommentMediaObject::operator=(const CommentMediaObject &obj)
{
    if (this != &obj)
        data = obj.data;

    return *this;
}

CommentMediaObject::~CommentMediaObject()
{

}

CommentMediaObject::CommentMediaType CommentMediaObject::mediaType()
{
    return data->mediaType;
}

void CommentMediaObject::setMediaType(CommentMediaType type)
{
    data->mediaType = type;
}

QUrl CommentMediaObject::imageUrl() const
{
    return data->imageUrl;
}

void CommentMediaObject::setImageUrl(const QUrl &url)
{
    data->imageUrl = url;
}

QSize CommentMediaObject::imageSize() const
{
    return data->imageSize;
}

void CommentMediaObject::setImageSize(const QSize &size)
{
    data->imageSize = size;
}

QUrl CommentMediaObject::gifUrl() const
{
    return data->gifUrl;
}

void CommentMediaObject::setGifUrl(const QUrl &url)
{
    data->gifUrl = url;
}

QSize CommentMediaObject::gifSize() const
{
    return data->gifSize;
}

void CommentMediaObject::setGifSize(const QSize &size)
{
    data->gifSize = size;
}

QUrl CommentMediaObject::videoUrl() const
{
    return data->videoUrl;
}

void CommentMediaObject::setVideoUrl(const QUrl &url)
{
    data->videoUrl = url;
}

QSize CommentMediaObject::videoSize() const
{
    return data->videoSize;
}

void CommentMediaObject::setVideoSize(const QSize &size)
{
    data->videoSize = size;
}
