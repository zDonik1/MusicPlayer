// https://stackoverflow.com/questions/20691414/
// qt-qml-send-qimage-from-c-to-qml-and-display-the-qimage-on-gui
// 1st answer

#pragma once

#include <QQuickImageProvider>

class MusicImageProvider : public QQuickImageProvider
{
public:
    MusicImageProvider();

    void setMusicImages(const std::vector<QPixmap> &images);

    QPixmap requestPixmap(const QString &id, QSize *size,
                          const QSize &requestedSize) override;

private:
    std::vector<QPixmap> m_pixmaps;
};
