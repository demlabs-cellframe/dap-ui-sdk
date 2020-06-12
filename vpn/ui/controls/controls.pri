SOURCES += \
    $$PWD/ComboBox.cpp \
    $$PWD/ComboBoxListView.cpp \
    $$PWD/ComboBoxPopup.cpp \
    $$PWD/CustomLicenseLineEdit.cpp


HEADERS  += \
    $$PWD/ComboBox.h \
    $$PWD/ComboBoxListView.h \
    $$PWD/ComboBoxPopup.h \
    $$PWD/CustomLicenseLineEdit.h


android {
    SOURCES += \
        $$PWD/mobile/ScreenComboBox.cpp

    HEADERS  += \
        $$PWD/mobile/ScreenComboBox.h
}
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
