SOURCES += \
    $$PWD/ComboBox.cpp \
    $$PWD/ComboBoxListView.cpp \
    $$PWD/ComboBoxPopup.cpp \


HEADERS  += \
    $$PWD/ComboBox.h \
    $$PWD/ComboBoxListView.h \
    $$PWD/ComboBoxPopup.h \


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
