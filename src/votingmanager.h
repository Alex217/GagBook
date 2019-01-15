/*
 * Copyright (C) 2018 Alexander Seibel.
 * Copyright (c) 2014 Bob Jelica
 * Copyright (c) 2014 Dickson Leong
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

#ifndef VOTINGMANAGER_H
#define VOTINGMANAGER_H

#include <QtCore/QObject>

class QNetworkReply;
class GagBookManager;

/*! Handle gag upvote/downvote action */
class VotingManager : public QObject
{
    Q_OBJECT

    /*! True if there is active network request. Voting should be disable when busy. */
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)

    /*! Set the global instance of GagBookManager. Must be set before component completed
        and can not be change afterward */
    Q_PROPERTY(GagBookManager *manager READ manager WRITE setManager)
public:
    explicit VotingManager(QObject *parent = 0);

    enum VoteType {
        Like,
        Unlike,
        Dislike
    };

    Q_ENUM(VoteType)

    bool isBusy() const;

    GagBookManager *manager() const;
    void setManager(GagBookManager *manager);

    /*! Vote for a gag. \p id is the id of the gag. */
    Q_INVOKABLE void vote(const QString &id, VoteType voteType);

signals:
    void busyChanged();

    /*! Emit when vote is succeeded. \p id is the id of the gag and \p likes is
        new likes of the gag after voting. */
    void voteSuccess(const QString &id, int likes);

    /*! Emit when vote is failed, \p errorMessage contains the reason for the failure
        and should show to user. */
    void failure(const QString &errorMessage);

private slots:
    void onReplyFinished();

private:
    bool m_isBusy;
    GagBookManager *m_manager;
    QNetworkReply *m_reply;

    QString enumToString(VoteType aElement);
};

#endif // VOTINGMANAGER_H
