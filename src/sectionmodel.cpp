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

#include "sectionmodel.h"

Section::Section(const QString &name, const QString &urlPath, const int &groupId, const QString &listType,
                 const QString &listType2, bool isSensitive)
    : m_name(name), m_urlPath(urlPath), m_groupId(groupId), m_listType(listType), m_listType2(listType2),
      m_isSensitive(isSensitive)
{

}

Section::Section(const QString &name, const QString &urlPath, const int &groupId, bool isSensitive)
    : m_name(name), m_urlPath(urlPath), m_groupId(groupId), m_listType("hot"), m_listType2("hot"),
      m_isSensitive(isSensitive)
{

}

Section::Section(const QString &name, const QString &urlPath, bool isSensitive)
    : m_name(name), m_urlPath(urlPath), m_groupId(-1), m_listType(QString()), m_listType2(QString()),
      m_isSensitive(isSensitive)
{

}

QString Section::name() const
{
    return m_name;
}

QString Section::urlPath() const
{
    return m_urlPath;
}

int Section::groupId() const
{
    return m_groupId;
}

QString Section::listType() const
{
    return m_listType;
}

QString Section::listType2() const
{
    return m_listType2;
}

bool Section::isSensitive() const
{
    return m_isSensitive;
}


SectionModel::SectionModel(QObject *parent) : QAbstractListModel(parent)
{

}

void SectionModel::addSection(const Section &section)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_sections.append(section);
    endInsertRows();
}

int SectionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_sections.count();
}

QVariant SectionModel::data(const QModelIndex &index, int role) const
{
    if ((index.row() < 0) || (index.row() >= m_sections.count())) {
        return QVariant();
    }

    const Section &section = m_sections[index.row()];
    QVariant reqData;

    switch (role) {
        case NameRole:
            reqData = section.name(); break;
        case UrlPathRole:
            reqData = section.urlPath(); break;
        case GroupIdRole:
            reqData = section.groupId(); break;
        case ListTypeRole:
            reqData = section.listType(); break;
        case ListType2Role:
            reqData = section.listType2(); break;
        case IsSensitiveRole:
            reqData = section.isSensitive(); break;
        default:
            reqData = QVariant(); break;
    }

    return reqData;
}

QHash<int, QByteArray> SectionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[UrlPathRole] = "urlPath";
    roles[GroupIdRole] = "groupId";
    roles[ListTypeRole] = "listType";
    roles[ListType2Role] = "listType2";
    roles[IsSensitiveRole] = "isSensitive";

    return roles;
}

QVariantMap SectionModel::getSection(int row)
{
    QHash<int, QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    QModelIndex modelIndex = index(row, 0);

    while (i.hasNext()) {
        i.next();
        QVariant data = modelIndex.data(i.key());
        res[i.value()] = data;
    }

    return res;
}

bool SectionModel::isEmpty()
{
    if (m_sections.size() == 0)
        return true;
    else
        return false;
}

//void SectionModel::setSectionsList(const QList<Section> &sectionsList)
//{
//    beginResetModel();
//    m_sections = sectionsList;
//    endResetModel();
//}

void SectionModel::setDefaultSections()
{
    beginResetModel();
    m_sections.clear();
    m_sections.reserve(57);

    m_sections.append(Section("Hot", "hot", 1, QString(), QString(), false));
    m_sections.append(Section("Trending", "trending", 1, QString(), QString(), false));
    m_sections.append(Section("Fresh", "vote", 1, QString(), QString(), false));
    m_sections.append(Section("Animals", "cute", 3, "hot", "hot", false));
    m_sections.append(Section("Anime & Manga", "anime-manga", 32, "hot", "hot", false));
    m_sections.append(Section("Ask 9GAG", "ask9gag", 31, "hot", "hot", false));
    m_sections.append(Section("Awesome", "awesome", 7, "hot", "hot", false));
    m_sections.append(Section("Basketball", "basketball", 76, "hot", "hot", false));
    m_sections.append(Section("Car", "car", 62, "hot", "hot", false));
    m_sections.append(Section("Comics", "comic", 17, "hot", "hot", false));
    m_sections.append(Section("Cosplay", "cosplay", 11, "hot", "hot", false));
    m_sections.append(Section("Countryballs", "country", 38, "hot", "hot", false));
    m_sections.append(Section("Classical Art Memes", "classicalartmemes", 72, "hot", "hot", false));
    m_sections.append(Section("DIY & Crafts", "imadedis", 41, "hot", "hot", false));
    m_sections.append(Section("Drawing & Illustration", "drawing", 84, "hot", "hot", false));
    m_sections.append(Section("Fan Art", "animefanart", 85, "hot", "hot", false));
    m_sections.append(Section("Food & Drinks", "food", 8, "hot", "hot", false));
    m_sections.append(Section("Football", "football", 77, "hot", "hot", false));
    m_sections.append(Section("Fortnite", "fortnite", 79, "hot", "hot", false));
    m_sections.append(Section("Funny", "funny", 27, "hot", "hot", false));
    m_sections.append(Section("Gaming", "gaming", 5, "hot", "hot", false));
    m_sections.append(Section("GIF", "gif", 10, "hot", "hot", false));
    m_sections.append(Section("Girl", "girl", 16, "hot", "hot", false));
    m_sections.append(Section("Girly Things", "girly", 47, "hot", "hot", false));
    m_sections.append(Section("Guy", "guy", 20, "hot", "hot", false));
    m_sections.append(Section("History", "history", 66, "hot", "hot", false));
    m_sections.append(Section("Horror", "horror", 40, "hot", "hot", false));
    m_sections.append(Section("Home Design", "home", 74, "hot", "hot", false));
    m_sections.append(Section("K-Pop", "kpop", 34, "hot", "hot", false));
    m_sections.append(Section("League Of Legends", "leagueoflegends", 81, "hot", "hot", false));
    m_sections.append(Section("LEGO", "lego", 83, "hot", "hot", false));
    m_sections.append(Section("Movie & TV", "movie-tv", 33, "hot", "hot", false));
    m_sections.append(Section("Music", "music", 63, "hot", "hot", false));
    m_sections.append(Section("NFK - Not For Kids", "nsfw", 12, "hot", "hot", true));
    m_sections.append(Section("Overwatch", "overwatch", 60, "hot", "hot", false));
    m_sections.append(Section("PC Master Race", "pcmr", 64, "hot", "hot", false));
    m_sections.append(Section("Pic Of The Day", "photography", 73, "hot", "hot", false));
    m_sections.append(Section("Pokémon", "pokemon", 82, "hot", "hot", false));
    m_sections.append(Section("Politics", "politics", 43, "hot", "hot", false));
    m_sections.append(Section("Relationship", "relationship", 44, "hot", "hot", false));
    m_sections.append(Section("PUBG", "pubg", 78, "hot", "hot", false));
    m_sections.append(Section("Roast Me", "roastme", 75, "hot", "hot", false));
    m_sections.append(Section("Savage", "savage", 45, "hot", "hot", false));
    m_sections.append(Section("Satisfying", "satisfying", 61, "hot", "hot", false));
    m_sections.append(Section("School", "school", 36, "hot", "hot", false));
    m_sections.append(Section("Sci-Tech", "science", 42, "hot", "hot", false));
    m_sections.append(Section("Star Wars", "starwars", 70, "hot", "hot", false));
    m_sections.append(Section("Superhero", "superhero", 49, "hot", "hot", false));
    m_sections.append(Section("Surreal Memes", "surrealmemes", 71, "hot", "hot", false));
    m_sections.append(Section("Sport", "sport", 35, "hot", "hot", false));
    m_sections.append(Section("Travel", "travel", 9, "hot", "hot", false));
    m_sections.append(Section("Timely", "timely", 14, "hot", "hot", false));
    m_sections.append(Section("Video", "video", 57, "hot", "hot", false));
    m_sections.append(Section("Warhammer", "warhammer", 80, "hot", "hot", false));
    m_sections.append(Section("Wallpaper", "wallpaper", 65, "hot", "hot", false));
    m_sections.append(Section("WTF", "wtf", 4, "hot", "hot", false));
    m_sections.append(Section("Dark Humor", "darkhumor", 37, "hot", "hot", true));

    endResetModel();
}

void SectionModel::save(QSettings *settings, const QString &key) const
{
    settings->beginWriteArray(key, m_sections.size());

    for (int i = 0; i < m_sections.size(); ++i) {
        settings->setArrayIndex(i);
        settings->setValue("name", m_sections.at(i).name());
        settings->setValue("urlPath", m_sections.at(i).urlPath());
        settings->setValue("groupId", m_sections.at(i).groupId());
        //settings->setValue("listType", m_sections.at(i).listType());
        //settings->setValue("listType2", m_sections.at(i).listType2());
        settings->setValue("isSensitive", m_sections.at(i).isSensitive());
    }

    settings->endArray();
}

void SectionModel::restore(QSettings *settings, const QString &key)
{
    beginResetModel();
    m_sections.clear();
    int size = settings->beginReadArray(key);
    QVariant name, urlPath, groupId, isSensitive;

    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        name = settings->value("name");
        urlPath = settings->value("urlPath");
        groupId = settings->value("groupId");
        isSensitive = settings->value("isSensitive");

        Section section(name.toString(), urlPath.toString(), groupId.toInt(), isSensitive.toBool());
        m_sections.append(section);
    }

    settings->endArray();
    endResetModel();
}
