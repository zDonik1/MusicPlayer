#include "metadatascanner.h"

#include <QMediaMetaData>

MetaDataScanner::MetaDataScanner()
{
    connect(&m_mediaPlayer, QOverload<>::of(&QMediaPlayer::metaDataChanged),
            this, &MetaDataScanner::onMetaDataChanged,
            Qt::QueuedConnection);
}

void MetaDataScanner::getMetaData(const std::vector<Music *> &music)
{
    m_music = music;

    if (m_music.size() == 0)
        return;

    m_mediaPlayer.setMedia(QUrl::fromLocalFile(m_music.at(0)->path.toString()));
    m_counter = 0;
}

void MetaDataScanner::getMetaData(Music *music)
{
    m_music = { music };
    getMetaData(std::move(m_music));
}

void MetaDataScanner::onMetaDataChanged()
{
    if (!m_mediaPlayer.isMetaDataAvailable())
        return;

    auto &metaData = m_music.at(m_counter)->metaData;
    metaData.title = m_mediaPlayer.metaData(QMediaMetaData::Title).toString();
    metaData.duration = m_mediaPlayer.metaData
            (QMediaMetaData::Duration).toLongLong();
    metaData.image = m_mediaPlayer.metaData
            (QMediaMetaData::CoverArtImage).value<QImage>();

    if (m_mediaPlayer.metaData(QMediaMetaData::CoverArtImage).value<QImage>().isNull())
        qDebug() << "no image";
    else
        qDebug() << "some image";
    qDebug() << m_mediaPlayer.metaData(QMediaMetaData::CoverArtUrlLarge).toUrl();
    qDebug() << m_mediaPlayer.metaData(QMediaMetaData::CoverArtUrlSmall).toUrl();

    if (m_counter == static_cast<int>(m_music.size()) - 1) {
        emit metaDataReady();
        return;
    }

    m_mediaPlayer.setMedia(QUrl::fromLocalFile(
                               m_music.at(++m_counter)->path.toString()));
}
