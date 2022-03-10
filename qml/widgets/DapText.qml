import QtQuick 2.4

DapTextForm 
{
    textMetric.elideWidth: width

    //Text eliding is available only after full item boot
    //with all metrics - to delete binding loop
    Component.onCompleted:
    {
        checkTextElide();
    }
    //If it needed the dynamic width change
    onWidthChanged: checkTextElide()

    //Function to elide text and check result
    function checkTextElide()
    {
        textMetric.elideWidth = width;
        if(textMetric.elideWidth < fontMetric.tightBoundingRect(textMetric.text).width)
        {
            //Index of left or right or index of cut
            var indexOfChar = 0;
            //Temp string without cutting part
            var firstString = "";
            //Width of '..' for current font
            var twoPointWidth = fontMetric.tightBoundingRect('..').width;
            if(textMetric.elideWidth <= twoPointWidth + fontMetric.averageCharacterWidth)
            {
                elText = "";
                return;
            }
            if(textMetric.elideWidth <= 2 * twoPointWidth + 5)
            {
                elText = '..';
                return;
            }
            switch(textElide)
            {
                case Text.ElideRight:
                    indexOfChar = textMetric.elidedText.length - 1;
                    firstString = textMetric.elidedText.substring(0, indexOfChar);
                    if(fontMetric.tightBoundingRect(firstString + '..').width < elText.elideWidth)
                    {
                        if(fontMetric.tightBoundingRect(firstString + textMetric.text.charAt(indexOfChar) + '..').width < elText.elideWidth)
                            elText = firstString + textMetric.text.charAt(indexOfChar) + '..';
                        else
                            elText = firstString + '..';
                    }
                    else
                    {
                        indexOfChar--;
                        elText = textMetric.elidedText.substring(0, indexOfChar) + '..';
                    }
                    break;

                case Text.ElideLeft:
                    indexOfChar = textMetric.elidedText.length;
                    firstString = textMetric.elidedText.substring(1, indexOfChar);
                    if(fontMetric.tightBoundingRect('..' + firstString).width < textMetric.elideWidth)
                    {
                        if(fontMetric.tightBoundingRect('..' + textMetric.text.charAt(textMetric.text.length - textMetric.elidedText.length) + firstString).width < textMetric.elideWidth)
                            elText = '..' + textMetric.text.charAt(textMetric.text.length - textMetric.elidedText.length) + firstString;
                        else
                            elText = '..' + firstString;
                    }
                    else
                        elText = '..' + textMetric.elidedText.substring(2, indexOfChar);
                    break;

                case Text.ElideMiddle:
                    indexOfChar = textMetric.elidedText.indexOf('…');
                    //Index of char after '…'
                    var indexOfNextChar = indexOfChar + 1;
                    if(indexOfChar !== -1)
                    {
                        firstString = textMetric.elidedText.substring(0, indexOfChar);
                        if(fontMetric.tightBoundingRect(firstString + '..' + textMetric.elidedText.substring(indexOfNextChar, textMetric.elidedText.length)).width < textMetric.elideWidth)
                            elText = firstString + '..' + textMetric.elidedText.substring(indexOfNextChar, textMetric.elidedText.length);
                        else
                        {
                            indexOfChar--;
                            firstString = textMetric.elidedText.substring(0, indexOfChar);
                            if(fontMetric.tightBoundingRect(firstString + '..' + textMetric.elidedText.substring(indexOfNextChar, textMetric.elidedText.length)).width < textMetric.elideWidth)
                                elText = firstString + '..' + textMetric.elidedText.substring(indexOfNextChar, textMetric.elidedText.length);
                            else
                                elText = firstString + '..' + textMetric.elidedText.substring(indexOfNextChar, textMetric.elidedText.length);
                        }
                    }
                    else elText = fullText;
                    break;

                case Text.ElideNone:
                    elText = fullText;
                    break;
            }

        }
        else
            elText = fullText;
    }

    //Function to copy full text to the clipboard
    function copyFullText()
    {
        //Replace elide text by full text and select it
        text = fullText;
        selectAll();
        //Because this method can copy only selected text
        copy();
        // User should not see it!
        deselect();
        text = elText;
    }

    function updateText()
    {
        //Replace elide text by full text and select it
        text = fullText;
        text = elText;
    }
}
