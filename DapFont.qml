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
        }
        Component.onCompleted: dapProjectFonts = getFonts(fontRepeater)
    }

    //To associate dapProjectFonts property with FontLoader objects
    function getFonts(fontRepeater)
    {
        var fonts = [];
        for(var i = 0; i < fontRepeater.count; i++)
        {
            fonts[i] = fontRepeater.objectAt(i)
        }
        return fonts;
    }

}
