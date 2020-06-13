SOURCES += \
    $$PWD/ComboBox.cpp \
    $$PWD/ComboBoxListView.cpp \
    $$PWD/ComboBoxPopup.cpp \
    $$PWD/CustomLicenseLineEdit.cpp \
    $$PWD/ScreenComboBox.cpp \
    $$PWD/ScreenComboBoxPopup.cpp


HEADERS  += \
    $$PWD/ComboBox.h \
    $$PWD/ComboBoxListView.h \
    $$PWD/ComboBoxPopup.h \
    $$PWD/CustomLicenseLineEdit.h \
    $$PWD/ScreenComboBox.h \
    $$PWD/ScreenComboBoxPopup.h


android {
    SOURCES +=

    HEADERS  +=
}
!android {
    SOURCES +=

    HEADERS  +=
}


INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/desktop
