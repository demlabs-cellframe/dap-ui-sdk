import QtQuick 2.4

DapTextForm 
{
    onTextChanged:
    {
        elidedText = (width > 2) ? elideByWidth(width) : (maxSymbolCapacity > 2) ? elideByCapacity(maxSymbolCapacity) : text
        width = _metrics.boundingRect(elidedText).width
        height = _metrics.boundingRect(elidedText).height
    }

    // "Copy Button" handler
    function copy()
    {
        _fullText.selectAll()
        _fullText.copy()
    }

    // Elides text by number of symbols
    function elideByCapacity(maxSymbols)
    {
        var res = ""
        if (text.length > maxSymbols)
        {
            switch (elide)
            {
            case Qt.ElideLeft:
                res = ".." + text.slice(-maxSymbols, text.length-1)
                break
            case Qt.ElideMiddle:
                res = text.slice(0, Math.floor(maxSymbols/2)) + ".." + text.slice(Math.floor(maxSymbols/2) + 1, text.length-1)
                break
            case Qt.ElideRight:
                res = text.slice(0, maxSymbols) + ".."
                break
            default:
                res = text
                break
            }
        }
        else
        {
            res = text
        }
        return res
    }

    // Elides text by width
    function elideByWidth(maxWidth)
    {
        if (_metrics.advanceWidth(text) > maxWidth)
        {
            var symbolsNum = (maxWidth / _metrics.averageCharacterWidth)
            return elideByCapacity(Math.floor(symbolsNum))
        }
        else
        {
            return text
        }
    }
}
