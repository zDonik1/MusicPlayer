import QtQuick 2.0

Item {
    signal play()
    signal next()
    signal previous()
    signal seek(real position)
    signal shuffle()
    signal repeat()
    signal musicChanged(int index)

    signal dirToggled(int index)
}
