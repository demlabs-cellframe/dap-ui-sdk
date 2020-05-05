SOURCES += \
    $$PWD/ComboBox.cpp \
    $$PWD/ComboBoxListView.cpp \
    $$PWD/ComboBoxPopup.cpp \
    $$PWD/NavigationPanel.cpp \

HEADERS  += \
    $$PWD/ComboBox.h \
    $$PWD/ComboBoxListView.h \
    $$PWD/ComboBoxPopup.h \
    $$PWD/NavigationPanel.h

android {
    HEADERS  += \
        $$PWD/mobile/NavigationPanelBack.h \
        $$PWD/mobile/NavigationPanelMain.h

    SOURCES += \
        $$PWD/mobile/NavigationPanelMain.cpp \
        $$PWD/mobile/NavigationPanelBack.cpp
        
    INCLUDEPATH += $$PWD/mobile
}


INCLUDEPATH += $$PWD
