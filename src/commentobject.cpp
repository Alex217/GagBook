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

#include <QDebug>

#include "commentobject.h"

class CommentObjectData : public QSharedData
{
public:
    CommentObjectData() : id(QString()), timestamp(QDateTime()), url(QUrl()), text(QString()),
        textType(ContentType::Text), media(CommentMediaObject()), orderKey(QString()),
        user(UserObject()), upvotes(0), childCommentCount(0), hasMoreTopLvlComments(false)
    {

    }

    ~CommentObjectData()
    {
        // TODO: delete cached media files
    }

    // variables
    QString id;
    QDateTime timestamp;
    QUrl url;
    QString text;
    ContentType textType;
    CommentMediaObject media;
    QString orderKey;
    UserObject user;
    int upvotes;
    int childCommentCount;
    bool hasMoreTopLvlComments; // needed as a workaround if NineGagApiRequest is used as data source

private:
    Q_DISABLE_COPY(CommentObjectData)
};


/*!
    \class CommentObject
    \since 1.4.0
    \brief The CommentObject class encapsulates an user comment.

    CommentObject contains the comment text, the UserObject, a CommentMediaObject if available and a
    list of child comments which correspond to the answers of a comment.
    CommentObject is an explicitly shared data object.
 */

/*!
 * \brief CommentObject::CommentObject Constructs a CommentObject.
 * \param parent Pointer to the parent object.
 */
CommentObject::CommentObject(CommentObject *parent)
    : data(new CommentObjectData), m_parentComment(parent)
{

}

CommentObject::CommentObject(const CommentObject &obj)
    : data(obj.data)
{

}

CommentObject &CommentObject::operator=(const CommentObject &obj)
{
    data = obj.data;
    return *this;
}

/*!
 * \brief CommentObject::~CommentObject Destroys the CommentObject.
 */
CommentObject::~CommentObject()
{
    qDeleteAll(m_childComments);
}

/*!
 * \brief CommentObject::insertChild Inserts a CommentObject as a child at the given position.
 * \param pos The index position in the list of children. If \a pos <= 0, the value is prepended to
 *  the list and if \a pos >= size(), the value is appended to the list.
 * \param child The CommentObject that should be inserted.
 * \sa appendChild, appendChildren
 */
void CommentObject::insertChild(int pos, CommentObject *child)
{
    m_childComments.insert(pos, child);
}

/*!
 * \brief CommentObject::appendChild Appends a CommentObject as a child at the end of the list
 *  of children.
 * \param child The CommentObject that should be appended.
 * \sa appendChildren, insertChild
 */
void CommentObject::appendChild(CommentObject *child)
{
    m_childComments.append(child);
}

/*!
 * \brief CommentObject::appendChildren Appends the given list of CommentObjects as children to
 *  the end of the list of children.
 * \param commentList The list of CommentObjects that should be appended.
 * \sa appendChild, insertChild
 */
void CommentObject::appendChildren(QList<CommentObject *> commentList)
{
    m_childComments.reserve(commentList.count());
    m_childComments.append(commentList);
}

/*!
 * \brief CommentObject::removeChild Removes the CommentObject at the given index position from
 *  the list of children.
 * \param pos The index position in the list of children. \a pos must be a valid index position in
 *  the list of children (i.e. 0 <= \a pos < size())
 * \sa removeAllChildren
 */
void CommentObject::removeChild(int pos)
{
    if ((pos < 0) || (pos >= m_childComments.count())) {
        qWarning() << "CommentObject::removeChild(): Invalid index position!";
        return;
    }

    CommentObject *ptr = m_childComments.takeAt(pos);
    delete ptr;
}

/*!
 * \brief CommentObject::removeAllChildren Removes all children.
 */
void CommentObject::removeAllChildren()
{
    qDeleteAll(m_childComments);
    m_childComments.clear();
}

/*!
 * \brief CommentObject::parentComment Returns the parent CommentObject.
 * \return Returns the parent CommentObject. Note: The return value could be a nullptr!
 */
CommentObject *CommentObject::parentComment()
{
    return m_parentComment;
}

/*!
 * \brief CommentObject::child Returns the CommentObject at the given index in the list of
 *  children.
 * \param row The index position in the list of children. \a row must be a valid index
 *  position in  the list of children (i.e. 0 <= pos < size()).
 * \return Returns the CommentObject at the given index in the list of children. Note: The
 *  return value could be a nullptr!
 */
CommentObject *CommentObject::child(int row)
{
    if ((row < 0) || (row >= m_childComments.count())) {
        qWarning("CommentObject::child(): Index is out of bounds!");
        return nullptr;
    }

    return m_childComments.at(row);
}

/*!
 * \brief CommentObject::isRootComment Returns \c true the CommentObject object is the
 *  root object (i.e. has no parent).
 * \return Returns \c true if the CommentObject is the root object; \c false otherwise.
 */
bool CommentObject::isRootComment()
{
    if (m_parentComment == 0)
        return true;
    else
        return false;
}

/*!
 * \brief CommentObject::childIndex Returns the index position of the first occurence of
 *  \a comment at the list of children.
 * \param comment The CommentObject for that the index position should be determined.
 * \return Returns the index position of the first occurence of \a comment at the list
 *  of children. -1 is returned if no item matched with \a comment.
 */
int CommentObject::childIndex(CommentObject *comment)
{
    int index = m_childComments.indexOf(comment);

    // no comment matched
    if (index == -1)
        qWarning("CommentObject::childIndex(): Didn't find the requested CommentObject!");

    return index;
}

/*!
 * \brief CommentObject::currentChildCount Returns the number of children.
 * \return Returns the number of the currently instantiated children.
 */
int CommentObject::currentChildCount() const
{
    return m_childComments.count();
}

/*!
 * \brief CommentObject::row Returns the item's location within its parent's list of items.
 * \return Returns the item's index position within its parent's list of children. For the
 *  root CommentObject (i.e. has no parent) 0 is returned.
 */
int CommentObject::row() const
{
    if (m_parentComment) {
        return m_parentComment->m_childComments.indexOf(const_cast<CommentObject*>(this));
    }

    return 0;   // the rootComment has no parent
}

QString CommentObject::id() const
{
    return data->id;
}

void CommentObject::setId(const QString &id)
{
    data->id = id;
}

QDateTime CommentObject::timestamp() const
{
    return data->timestamp;
}

void CommentObject::setTimestamp(const QDateTime &timestamp)
{
    data->timestamp = timestamp;
}

QUrl CommentObject::permalink() const
{
    return data->url;
}

void CommentObject::setPermalink(const QUrl &url)
{
    data->url = url;
}

QString CommentObject::text() const
{
    return data->text;
}

void CommentObject::setText(const QString &text)
{
    data->text = text;
}

ContentType CommentObject::textType() const
{
    return data->textType;
}

void CommentObject::setTextType(ContentType contentType)
{
    data->textType = contentType;
}

CommentMediaObject CommentObject::media() const
{
    return data->media;
}

void CommentObject::setMedia(const CommentMediaObject &mediaObj)
{
    data->media = mediaObj;
}

QString CommentObject::orderKey() const
{
    return data->orderKey;
}

void CommentObject::setOrderKey(const QString &orderKey)
{
    data->orderKey = orderKey;
}

UserObject CommentObject::user() const
{
    return data->user;
}

void CommentObject::setUser(const UserObject &obj)
{
    data->user = obj;
}

int CommentObject::upvotes() const
{
    return data->upvotes;
}

void CommentObject::setUpvotes(int upvotes)
{
    data->upvotes = upvotes;
}

int CommentObject::totalChildCount() const
{
    return data->childCommentCount;
}

void CommentObject::setTotalChildCount(int childCount)
{
    data->childCommentCount = childCount;
}

bool CommentObject::hasMoreTopLvlComments() const
{
    return data->hasMoreTopLvlComments;
}

void CommentObject::setHasMoreTopLvlComments(bool hasMore)
{
    data->hasMoreTopLvlComments = hasMore;
}
