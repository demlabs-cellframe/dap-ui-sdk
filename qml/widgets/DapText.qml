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
        if(textMetric.elideWidth < fontMetric.tightBoundingRect(textMetric.text).width)
        {
            switch(textElide)
            {
                case Text.ElideRight:
                    elText = textMetric.elidedText.substring(0, textMetric.elidedText.length - 1) +
                                ((fontMetric.tightBoundingRect(textMetric.elidedText.substring(0, textMetric.elidedText.length - 1)).width +
                                  fontMetric.tightBoundingRect(textMetric.text.charAt(textMetric.elidedText.length - 1) + '..').width) < elText.elideWidth ?
                                     (textMetric.text.charAt(textMetric.elidedText.length - 3) + '..'):
                                   '..');
                    break;

                case Text.ElideLeft:
                    elText = '..' + ((fontMetric.tightBoundingRect(textMetric.elidedText.substring(1, textMetric.elidedText.length - 1)).width +
                              fontMetric.tightBoundingRect('..' + textMetric.text.charAt(textMetric.text.length - textMetric.elidedText.length)).width) < textMetric.elideWidth ?
                                 (textMetric.text.charAt(textMetric.text.length - textMetric.elidedText.length)):
                                 '') + textMetric.elidedText.substring(1, textMetric.elidedText.length - 1);

                    break;

                case Text.ElideMiddle:
                    elText = textMetric.elidedText.substring(0, textMetric.elidedText.indexOf('…')) +
                            ((fontMetric.tightBoundingRect(textMetric.elidedText.substring(0, textMetric.elidedText.indexOf('…'))).width +
                              fontMetric.tightBoundingRect(textMetric.text.charAt(textMetric.elidedText.indexOf('…')) + '..').width +
                              fontMetric.tightBoundingRect(textMetric.elidedText.substring(textMetric.elidedText.indexOf('…') + 1, textMetric.elidedText.length)).width) < textMetric.elideWidth ?
                                 (textMetric.text.charAt(textMetric.elidedText.indexOf('…')) + '..'):
                                 '..') +
                            textMetric.elidedText.substring(textMetric.elidedText.indexOf('…') + 1, textMetric.elidedText.length);
                    break;

                case Text.ElideNone:
                    elText = fullText;
            }

        }
        else
            elText = textMetric.elidedText.replace('…', '..');
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
}
