#include "metadatascanner.h"

#include <QMediaMetaData>

MetaDataScanner::MetaDataScanner()
{
    connect(&m_mediaPlayer, QOverload<>::of(&QMediaPlayer::metaDataChanged),
            this, &MetaDataScanner::onMetaDataChanged,
            Qt::QueuedConnection);
}

void MetaDataScanner::getMetaData(std::vector<Music> *music)
{
    if (!music)
        return;

    m_music = music;

    if (music->size() == 0)
        return;

    m_mediaPlayer.setMedia(QUrl::fromLocalFile(m_music->at(0).path.toString()));
    m_counter = 0;
}

void MetaDataScanner::onMetaDataChanged()
{
    if (!m_mediaPlayer.isMetaDataAvailable())
        return;

    qDebug() << "we here" << m_counter << "of" << m_music->size() - 1;
    auto &metaData = m_music->at(m_counter).metaData;
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

    if (m_counter == static_cast<int>(m_music->size()) - 1) {
        emit metaDataReady();
        return;
    }

    m_mediaPlayer.setMedia(QUrl::fromLocalFile(
                               m_music->at(++m_counter).path.toString()));
}
