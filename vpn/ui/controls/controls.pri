SOURCES += \
    $$PWD/ComboBox.cpp \
    $$PWD/ComboBoxListView.cpp \
    $$PWD/ComboBoxPopup.cpp \
    $$PWD/CustomLicenseLineEdit.cpp \
    $$PWD/IndicatorsFrame.cpp \
    $$PWD/NavigationPanelBackBase.cpp \
    $$PWD/ScreenComboBox.cpp \
    $$PWD/ScreenComboBoxPopup.cpp \
    $$PWD/SerialRemovalConfirmationMessage.cpp \
    $$PWD/CustomTariffButton.cpp \
    $$PWD/CustomGraphicsView.cpp \
    $$PWD/StatisticWidgetDownloadTab.cpp \
    $$PWD/StatisticWidgetUploadTab.cpp \
    $$PWD/StatisticWidgetWithTabs.cpp \
    $$PWD/TariffDelegate.cpp


HEADERS  += \
    $$PWD/ComboBox.h \
    $$PWD/ComboBoxListView.h \
    $$PWD/ComboBoxPopup.h \
    $$PWD/CustomLicenseLineEdit.h \
    $$PWD/IndicatorsFrame.h \
    $$PWD/NavigationPanelBackBase.h \
    $$PWD/ScreenComboBox.h \
    $$PWD/ScreenComboBoxPopup.h \
    $$PWD/SerialRemovalConfirmationMessage.h \
    $$PWD/CustomTariffButton.h \
    $$PWD/CustomGraphicsView.h \
    $$PWD/StatisticWidgetBase.h \
    $$PWD/StatisticWidgetDownloadTab.h \
    $$PWD/StatisticWidgetUploadTab.h \
    $$PWD/StatisticWidgetWithTabs.h \
    $$PWD/TariffDelegate.h


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

FORMS += \
    $$PWD/NavigationPanelBack.ui \
    $$PWD/StatisticWidgetTab.ui \
    $$PWD/StatisticWidgetWithTabs.ui
