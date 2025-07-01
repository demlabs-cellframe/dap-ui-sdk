import QtQuick

Image {
    property double scaleFactor: 1.0
    sourceSize: Qt.size( img.sourceSize.width * scaleFactor, img.sourceSize.height* scaleFactor )
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
