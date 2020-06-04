SOURCES += \
    $$PWD/ComboBox.cpp \
    $$PWD/ComboBoxListView.cpp \
    $$PWD/ComboBoxPopup.cpp \
    $$PWD/NavigationPanel.cpp \
    $$PWD/NavigationPanelBack.cpp \
    $$PWD/NavigationPanelMain.cpp \

HEADERS  += \
    $$PWD/ComboBox.h \
    $$PWD/ComboBoxListView.h \
    $$PWD/ComboBoxPopup.h \
    $$PWD/NavigationPanel.h \
    $$PWD/NavigationPanelBack.h \
    $$PWD/NavigationPanelMain.h \

!android {
    SOURCES += \
        $$PWD/desktop/ScreenComboBox.cpp \
        $$PWD/desktop/ScreenComboBoxPopup.cpp

    HEADERS  += \
        $$PWD/desktop/ScreenComboBox.h \
        $$PWD/desktop/ScreenComboBoxPopup.h
}


INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/desktop
