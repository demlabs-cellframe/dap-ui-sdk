HEADERS  += \
    $$PWD/AccountScreen.h \
    $$PWD/BugReportLoadingScreen.h \
    $$PWD/BugReportResultScreen.h \
    $$PWD/BugReportScreenBase.h \
    $$PWD/BugReportWithEmailScreen.h \
    $$PWD/BugReportWithoutEmailScreen.h \
    $$PWD/MainScreenBase.h \
    $$PWD/MainScreenLayout.h \
    $$PWD/PasswordRecoveryScreen.h \
    $$PWD/ScreenWithCustomPopupsAbstract.h \
    $$PWD/StartScreen.h \
    $$PWD/NavigationPanel.h \
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
    $$PWD/MainScreenBase.cpp \
    $$PWD/MainScreenLayout.cpp \
    $$PWD/PasswordRecoveryScreen.cpp \
    $$PWD/NavigationPanel.cpp \
    $$PWD/ScreenWithCustomPopupsAbstract.cpp \
    $$PWD/StartScreen.cpp \
    $$PWD/SignUpScreen.cpp \
    $$PWD/TermsOfUseScreen.cpp \
    $$PWD/PrivacyPolicyScreen.cpp

!android {
    HEADERS  += \
        $$PWD/desktop/LoginScreenLayout.h \
        $$PWD/desktop/ComboBoxPopupScreen.h \
        $$PWD/desktop/PopupScreenBase.h \
        $$PWD/desktop/ScreenWithScreenPopupsAbstract.h

    SOURCES += \
        $$PWD/desktop/LoginScreenLayout.cpp \
        $$PWD/desktop/ComboBoxPopupScreen.cpp \
        $$PWD/desktop/PopupScreenBase.cpp \
        $$PWD/desktop/ScreenWithScreenPopupsAbstract.cpp

    FORMS += \
        $$PWD/desktop/LoginScreenLayout.ui

    INCLUDEPATH += $$PWD/desktop
}

INCLUDEPATH += $$PWD

FORMS += \
    $$PWD/BugReportLoadingScreen.ui \
    $$PWD/BugReportResultScreen.ui \
    $$PWD/BugReportScreen.ui
