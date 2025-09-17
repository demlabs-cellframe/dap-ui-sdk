import QtQuick
import QtQuick.Window

Image {
    // Scale factor from app theme (passed by parents)
    property real scaleFactor: 1.0
    // Device pixel ratio for crisp rasterization on HiDPI
    readonly property real dpr: (Screen.devicePixelRatio || 1)

    // Render vector at the actual device pixel size to avoid pixelation
    // We avoid over-scaling (no extra scaleFactor here) to keep edges crisp
    function _updateSourceSize() {
        if (width > 0 && height > 0) {
            sourceSize = Qt.size(Math.max(1, Math.round(width * dpr)), Math.max(1, Math.round(height * dpr)));
        }
    }
    Component.onCompleted: _updateSourceSize()
    onWidthChanged: _updateSourceSize()
    onHeightChanged: _updateSourceSize()

    // Visual quality hints
    fillMode: Image.PreserveAspectFit
    antialiasing: false
    smooth: false
    mipmap: false
    cache: true
}
