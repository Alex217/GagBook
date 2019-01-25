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

#ifndef COMMENTOBJECT_H
#define COMMENTOBJECT_H

#include <QUrl>
#include <QList>
#include <QObject>
#include <QDateTime>
#include <QExplicitlySharedDataPointer>

#include "commentmediaobject.h"
#include "userobject.h"

enum ContentType {
    Text,
    Media,
    UserMedia
};

Q_DECLARE_METATYPE(ContentType)

class CommentObjectData;

class CommentObject
{
    Q_GADGET
    Q_ENUM(ContentType)

public:
    explicit CommentObject(CommentObject *parent = 0);
    CommentObject(const CommentObject &obj);
    CommentObject &operator=(const CommentObject &obj);
    ~CommentObject();

    void insertChild(int pos, CommentObject *child);
    void appendChild(CommentObject *child);
    void appendChildren(QList<CommentObject *> commentList);
    void removeChild(int pos);
    void removeAllChildren();

    CommentObject *parentComment();
    CommentObject *child(int row);
    bool isRootComment();
    int childIndex(CommentObject *comment);
    int currentChildCount() const;
    int row() const;

    /* Getter and setter methods */
    QString id() const;
    void setId(const QString &id);

    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &timestamp);

    QUrl permalink() const;
    void setPermalink(const QUrl &url);

    QString text() const;
    void setText(const QString &text);

    ContentType textType() const;
    void setTextType(ContentType contentType);

    CommentMediaObject media() const;
    void setMedia(const CommentMediaObject &mediaObj);

    QString orderKey() const;
    void setOrderKey(const QString &orderKey);

    UserObject user() const;
    void setUser(const UserObject &obj);

    int upvotes() const;
    void setUpvotes(int upvotes);

    int totalChildCount() const;
    void setTotalChildCount(int childCount);

    bool hasMoreTopLvlComments() const;
    void setHasMoreTopLvlComments(bool hasMore);

private:
    QExplicitlySharedDataPointer<CommentObjectData> data;

    QList<CommentObject *> m_childComments;
    CommentObject *m_parentComment;
};

#endif // COMMENTOBJECT_H
