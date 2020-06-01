#include "musicimageprovider.h"

#include <QDebug>

MusicImageProvider::MusicImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

void MusicImageProvider::setMusicImages(const std::vector<QPixmap> &images)
{
    m_pixmaps = images;
}

QPixmap MusicImageProvider::requestPixmap(const QString &id, QSize *size,
                                          const QSize &requestedSize)
{
    int index = id.toInt();

    if (index >= static_cast<int>(m_pixmaps.size()))
        return QPixmap();

    if (size)
       *size = m_pixmaps.at(index).size();

    return m_pixmaps.at(index).scaled(requestedSize,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
}
