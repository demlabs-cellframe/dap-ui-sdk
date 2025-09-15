import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import qmlclipboard 1.0

Item {
    id: root
    
    // Main properties
    property int fieldsNumber: 12  // Number of seed phrase words (12 or 24)
    property bool restoreMode: false  // true for restore/import, false for new generation
    property var wordsModel: []  // Array of words
    property int buttonsAreaHeight: 96  // Height of the buttons area (reduced)
    property bool wordsCopied: false  // Track if words were copied in create mode
    
    // Button aliases for external access
    property alias copyButton: copyActionButton
    property alias nextButton: nextActionButton
    
    // Legacy compatibility properties  
    property alias textButton: copyActionButton.textButton
    property alias enabled: copyActionButton.enabled
    property alias text: copyActionButton.text
    
    // Styling properties (all configurable)
    property color fieldBackgroundColor: "#0D0E16"
    property color fieldBorderColor: "#404040"
    property color fieldTextColor: "#FFFFFF"
    property color fieldNumberColor: "#4DAF50"
    property color clearButtonColor: "#FF6B6B"
    property color clearButtonHoverColor: "#FF5555"
    
    property int fieldRadius: 4
    property int buttonRadius: 4
    property int scaleFactor: 1
    property string fontFamily: "Quicksand"
    property int fontSize: 16
    property int buttonFontSize: 14
    
    // Icon paths (configurable)
    property string clearIconPath: "qrc:/icons/other/x_mark_white.svg"
    property string clearIconHoverPath: "qrc:/icons/other/x_mark.svg"
    property string clearAllIconPath: "qrc:/icons/other/icon_reload.svg"
    property string clearAllIconHoverPath: "qrc:/icons/other/icon_reload_hover.svg"
    property color clearAllTextColor: "#FFFFFF"
    property color clearAllTextHoverColor: "#4DAF50"
    
    // Signals
    signal copyClicked()
    signal nextClicked()
    signal clearAllClicked()
    signal wordChanged(int index, string word)

    signal pasteWordsRequested()  // Request parent to handle paste
    signal copyWordsRequested()   // Request parent to handle copy
    
    QMLClipboard {
        id: clipboard
    }
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 16 * scaleFactor
        
        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            
            Grid {
                id: wordsGrid
                width: scrollView.width
                columns: 2
                columnSpacing: 8 * scaleFactor
                rowSpacing: 8 * scaleFactor
                
                Repeater {
                    model: fieldsNumber
                    
                    Item {
                        width: (wordsGrid.width - wordsGrid.columnSpacing) / 2
                        height: 42 * scaleFactor
                        
                        TextField {
                            id: wordField
                            anchors.fill: parent
                            leftPadding: 22 * scaleFactor
                            rightPadding: restoreMode ? 26 * scaleFactor : 8 * scaleFactor
                            font.family: fontFamily
                            font.pixelSize: fontSize
                            color: fieldTextColor
                            readOnly: !restoreMode
                            selectByMouse: true
                            
                            validator: RegularExpressionValidator {
                                regularExpression: /[A-Za-z\-]+/
                            }
                            
                            background: Rectangle {
                                color: fieldBackgroundColor
                                border.width: 1 * scaleFactor
                                border.color: fieldBorderColor
                                radius: fieldRadius * scaleFactor
                            }
                            
                            // Initialize text without binding to avoid loops
                            Component.onCompleted: {
                                if(index < wordsModel.length) 
                                {
                                    text = wordsModel[index];
                                }
                            }
                            
                            onTextChanged: {
                                // Only update if the value actually changed to prevent binding loops
                                if(index < wordsModel.length && wordsModel[index] !== text) 
                                {
                                    var newWordsModel = wordsModel.slice();
                                    newWordsModel[index] = text;
                                    wordsModel = newWordsModel;
                                    
                                    root.wordChanged(index, text);
                                    
                                    if(restoreMode) 
                                    {
                                        setEnableNext(true);
                                    }
                                } 
                                else if(index >= wordsModel.length) 
                                {
                                    root.wordChanged(index, text);
                                    
                                    if(restoreMode) 
                                    {
                                        var allFieldsFilled = true;
                                        for(var j = 0; j < wordsGrid.children.length && j < fieldsNumber; j++) 
                                        {
                                            var item = wordsGrid.children[j];
                                            if(item && item.children && item.children.length > 0) 
                                            {
                                                var textField = item.children[0];
                                                if(textField && textField.hasOwnProperty('text')) 
                                                {
                                                    if(!textField.text || textField.text.trim() === "") 
                                                    {
                                                        allFieldsFilled = false;
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        setEnableNext(allFieldsFilled);
                                    }
                                }
                            }

                            Keys.onPressed: function (event) {
                                if(event.key === Qt.Key_V && (event.modifiers & Qt.ControlModifier)) 
                                {
                                    event.accepted = true;
                                    if(restoreMode) 
                                    {
                                        root.pasteWordsRequested();
                                    }
                                }
                            }
                        }
                        
                        Text {
                            z: 1
                            x: 6 * scaleFactor
                            y: 5 * scaleFactor
                            text: (index + 1).toString()
                            color: fieldNumberColor
                            font.family: fontFamily
                            font.pixelSize: 12
                        }
                        
                        Image {
                            id: clearFieldButton
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 8 * scaleFactor
                            sourceSize.width: 18 * scaleFactor
                            sourceSize.height: 18 * scaleFactor
                            source: clearArea.containsMouse ? clearIconHoverPath : clearIconPath
                            visible: restoreMode
                            
                            MouseArea {
                                id: clearArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    var newWordsModel = wordsModel.slice();
                                    newWordsModel[index] = "";
                                    wordsModel = newWordsModel;
                                    wordField.text = "";
                                    root.wordChanged(index, "");
                                    setEnableNext(false);
                                }
                            }
                        }
                    }
                }
            }
        }
        
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: buttonsAreaHeight
            
            ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16 
                spacing: 16 
                
                Item {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: 90
                    Layout.preferredHeight: restoreMode ? 28 : 0
                    visible: restoreMode
                    
                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Clear input")
                        color: clearAllArea.containsMouse ? clearAllTextHoverColor : clearAllTextColor
                        font.family: fontFamily
                        font.pixelSize: 12
                    }
                    
                    Image {
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        rotation: 180
                        sourceSize.width: 28
                        sourceSize.height: 28
                        source: clearAllArea.containsMouse ? clearAllIconHoverPath : clearAllIconPath
                    }
                    
                    MouseArea {
                        id: clearAllArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            root.clearAllWords();
                        }
                    }
                }
                
                // Action buttons
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36
                    spacing: 8
                    
                    DapButton {
                        id: copyActionButton
                        Layout.preferredWidth: 155
                        Layout.preferredHeight: 36
                        textButton: restoreMode ? qsTr("Paste") : qsTr("Copy")
                        enabled: true
                        
                        implicitWidth: 155
                        implicitHeight: 36
                        horizontalAligmentText: Text.AlignHCenter
                        fontButton.family: fontFamily
                        fontButton.pixelSize: buttonFontSize
                        
                        onClicked: {
                            if(restoreMode) {

                                var clipboardText = clipboard.getText()
                                if(clipboardText && clipboardText.length > 0) 
                                {
                                    console.log("Direct paste from clipboard:", clipboardText);
                                    var words = clipboardText.trim().split(/\s+/)
                                    if(words.length >= fieldsNumber) 
                                    {

                                        words = words.slice(0, fieldsNumber)
                                        root.pasteAllWords(words);
                                    } 
                                    else 
                                    {
                                        console.log("Not enough words in clipboard:", words.length, "required:", fieldsNumber);

                                        root.pasteWordsRequested()
                                    }
                                } 
                                else 
                                {
                                    console.log("Clipboard is empty, requesting parent to handle")
                                    root.pasteWordsRequested()
                                }
                            } 
                            else 
                            {
                                root.wordsCopied = true
                                root.copyWordsRequested()

                            }
                            
                            root.copyClicked()
                        }
                    }
                    
                    DapButton {
                        id: nextActionButton
                        Layout.preferredWidth: 155
                        Layout.preferredHeight: 36
                        textButton: qsTr("Next")
                        enabled: false
                        
                        implicitWidth: 155
                        implicitHeight: 36
                        horizontalAligmentText: Text.AlignHCenter
                        fontButton.family: fontFamily
                        fontButton.pixelSize: buttonFontSize
                        
                        onClicked: {
                            root.nextClicked();
                        }
                    }
                }
            }
        }
    }
    
    function pasteAllWords(words) 
    {
        var newWordsModel = [];
        for(var i = 0; i < fieldsNumber; i++) 
        {
            newWordsModel[i] = i < words.length ? words[i].trim() : "";
        }
        wordsModel = newWordsModel;
        
        for(var j = 0; j < wordsGrid.children.length && j < fieldsNumber; j++) 
        {
            var item = wordsGrid.children[j];
            if(item && item.children && item.children.length > 0) 
            {
                var textField = item.children[0];
                if(textField && textField.hasOwnProperty('text')) 
                {
                    var newText = newWordsModel[j] || "";
                    if(textField.text !== newText) 
                    {
                        textField.text = newText;
                    }
                }
            }
        }
        setEnableNext(true);
    }
    
    function clearAllWords() 
    {
        var newWordsModel = [];
        for(var i = 0; i < fieldsNumber; i++) 
        {
            newWordsModel[i] = "";
        }
        wordsModel = newWordsModel;

        for(var j = 0; j < wordsGrid.children.length && j < fieldsNumber; j++) 
        {
            var item = wordsGrid.children[j];
            if(item && item.children && item.children.length > 0) 
            {
                var textField = item.children[0];
                if(textField && textField.hasOwnProperty('text') && textField.text !== "") 
                {
                    textField.text = "";
                }
            }
        }
        
        root.clearAllClicked();
        setEnableNext(false);
    }
    
    function checkAllWordsFilled() 
    {
        if(!restoreMode) return true;
        
        for(var i = 0; i < fieldsNumber; i++) 
        {
            if(i >= wordsModel.length || !wordsModel[i] || wordsModel[i].trim() === "") 
            {
                return false;
            }
        }
        return true;
    }
    
    function setWordsModel(words) 
    {
        console.log("DapSeedPhraseWidget: setWordsModel called with", words.length, "words:", words);

        var newWordsModel = [];
        for(var i = 0; i < fieldsNumber; i++) 
        {
            newWordsModel[i] = (i < words.length) ? words[i] : "";
        }
        wordsModel = newWordsModel;

        for(var i = 0; i < wordsGrid.children.length && i < fieldsNumber; i++) 
        {
            var item = wordsGrid.children[i];
            if(item && item.children && item.children.length > 0) 
            {
                var textField = item.children[0];
                if(textField && textField.hasOwnProperty('text')) 
                {
                    var newText = newWordsModel[i] || "";
                    if(textField.text !== newText) 
                    {
                        textField.text = newText;
                    }
                }
            }
        }
        
        setEnableNext(checkAllWordsFilled());
    }
    
    function getWordsModel() 
    {
        return wordsModel;
    }

    function setEnableNext(isEnable)
    {
        if(isEnable)
        {
            if(restoreMode)
            {
                nextActionButton.enabled = checkAllWordsFilled()
            }
            else
            {
                nextActionButton.enabled = checkAllWordsFilled() && wordsCopied
            }
        }
        else
        {
            nextActionButton.enabled = isEnable
        }
    }

    Component.onCompleted: {
        console.log("DapSeedPhraseWidget: Component completed. Fields:", fieldsNumber, "RestoreMode:", restoreMode);
        console.log("DapSeedPhraseWidget: Initial wordsCopied:", wordsCopied);
        if(restoreMode)
        {
            console.log("DapSeedPhraseWidget: Restore mode - checking fields");
            setEnableNext(true);
        } 
        else 
        {
            console.log("DapSeedPhraseWidget: Create mode - disabling Next until copy");
            root.wordsCopied = false;
            setEnableNext(false);  
            console.log("DapSeedPhraseWidget: Set wordsCopied to false, Next should be disabled");
        }
    }
    
    onWordsCopiedChanged: {
        console.log("DapSeedPhraseWidget: wordsCopied changed to:", wordsCopied);
        console.log("DapSeedPhraseWidget: Next button should be enabled:", wordsCopied);
    }
}
