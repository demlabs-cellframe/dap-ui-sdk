import QtQuick 2.9

Image {
    sourceSize: Qt.size( img.sourceSize.width, img.sourceSize.height )
//    mipmap: true
//    smooth: false

    Image {
        id: img
        source: parent.source
        width: 0
        height: 0
    }
}

/*Image {
}*/
