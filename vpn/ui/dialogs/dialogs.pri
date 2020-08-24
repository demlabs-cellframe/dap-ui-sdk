HEADERS  += \
    $$PWD/AccountScreen.h \
    $$PWD/BugReportLoadingScreen.h \
    $$PWD/BugReportResultScreen.h \
    $$PWD/BugReportScreenBase.h \
    $$PWD/BugReportWithEmailScreen.h \
    $$PWD/BugReportWithoutEmailScreen.h \
    $$PWD/ComboBoxPopupScreen.h \
    $$PWD/InformationScreen.h \
    $$PWD/MainScreenBase.h \
    $$PWD/MainScreenLayout.h \
    $$PWD/NavigationPanelBase.h \
    $$PWD/PasswordRecoveryScreen.h \
    $$PWD/PopupScreenBase.h \
    $$PWD/ScreenWithCustomPopupsAbstract.h \
    $$PWD/ScreenWithScreenPopupsAbstract.h \
    $$PWD/StartScreen.h \
    $$PWD/SignUpScreen.h \
    $$PWD/TermsOfUseScreen.h \
    $$PWD/PrivacyPolicyScreen.h \
    $$PWD/screens.h

SOURCES += \
    $$PWD/AccountScreen.cpp \
    $$PWD/BugReportLoadingScreen.cpp \
    $$PWD/BugReportResultScreen.cpp \
    $$PWD/BugReportScreenBase.cpp \
    $$PWD/BugReportWithEmailScreen.cpp \
    $$PWD/BugReportWithoutEmailScreen.cpp \
    $$PWD/ComboBoxPopupScreen.cpp \
    $$PWD/InformationScreen.cpp \
    $$PWD/MainScreenBase.cpp \
    $$PWD/MainScreenLayout.cpp \
    $$PWD/NavigationPanelBase.cpp \
    $$PWD/PasswordRecoveryScreen.cpp \
    $$PWD/PopupScreenBase.cpp \
    $$PWD/ScreenWithCustomPopupsAbstract.cpp \
    $$PWD/ScreenWithScreenPopupsAbstract.cpp \
    $$PWD/StartScreen.cpp \
    $$PWD/SignUpScreen.cpp \
    $$PWD/TermsOfUseScreen.cpp \
    $$PWD/PrivacyPolicyScreen.cpp

android {

defined(DAP_USING_SEPARATE_STATISTIC_SCREEN,var){
        HEADERS  += \
            $$PWD/mobile/ConnectionScreenBase.h \
            $$PWD/mobile/StatisticScreenBase.h

        SOURCES += \
            $$PWD/mobile/ConnectionScreenBase.cpp \
            $$PWD/mobile/StatisticScreenBase.cpp
    }

    INCLUDEPATH += $$PWD/mobile
}
else {
    HEADERS  += \
        $$PWD/desktop/LoginScreenLayout.h

    SOURCES += \
        $$PWD/desktop/LoginScreenLayout.cpp

    FORMS += \
        $$PWD/desktop/LoginScreenLayout.ui

    INCLUDEPATH += $$PWD/desktop
}

INCLUDEPATH += $$PWD

FORMS += \
    $$PWD/BugReportLoadingScreen.ui \
    $$PWD/BugReportResultScreen.ui \
    $$PWD/InformationScreen.ui
