import QtQuick 2.0
import QtQml 2.2

Item
{
    ///@details dapFontPath Project path to the font folder
    property string dapFontPath
    ///@details dapFontNames Array of names of font files (for example roboto_thin.ttf)
    property var dapFontNames
    ///@details dapProjectFonts Array of loaded fonts
    property var dapProjectFonts

    //Create non-visual items
    Instantiator
    {
        id: fontRepeater
        model: dapFontNames.length
        FontLoader
        {
            source: dapFontPath + dapFontNames[index]
            onStatusChanged: {
                if (status === FontLoader.Ready) {
                    console.debug("Loaded:", name, "weight:", font.weight)
                }
            }
        }
        Component.onCompleted: dapProjectFonts = getFonts(fontRepeater)
    }

    //To associate dapProjectFonts property with FontLoader objects
    function getFonts(fontRepeater)
    {
        var fonts = [];
        var allLoaded = true;

        for (var i = 0; i < fontRepeater.count; i++) {
            var f = fontRepeater.objectAt(i)
            fonts[i] = f;
            if (f.status !== FontLoader.Ready) {
                allLoaded = false;
                console.warn("Font not ready:", f.source, "status:", f.status)
            } else {
                console.log("Font loaded:", f.name)
            }
        }

        if (allLoaded) {
            console.log("All fonts loaded successfully.")
        }

        return fonts;
    }

}
