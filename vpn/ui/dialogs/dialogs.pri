HEADERS  += \
    $$PWD/AccountScreen.h \
    $$PWD/BugReportLoadingScreen.h \
    $$PWD/BugReportResultScreen.h \
    $$PWD/BugReportScreenBase.h \
    $$PWD/BugReportWithEmailScreen.h \
    $$PWD/BugReportWithoutEmailScreen.h \
    $$PWD/MainScreen.h \
    $$PWD/MainScreenLayout.h \
    $$PWD/PasswordRecoveryScreen.h \
    $$PWD/StartScreen.h \
    $$PWD/NavigationPanel.h \
    $$PWD/SignUpScreen.h \
    $$PWD/TermsOfUseScreen.h \
    $$PWD/PrivacyPolicyScreen.h

SOURCES += \
    $$PWD/AccountScreen.cpp \
    $$PWD/BugReportLoadingScreen.cpp \
    $$PWD/BugReportResultScreen.cpp \
    $$PWD/BugReportScreenBase.cpp \
    $$PWD/BugReportWithEmailScreen.cpp \
    $$PWD/BugReportWithoutEmailScreen.cpp \
    $$PWD/MainScreen.cpp \
    $$PWD/MainScreenLayout.cpp \
    $$PWD/PasswordRecoveryScreen.cpp \
    $$PWD/NavigationPanel.cpp \
    $$PWD/StartScreen.cpp \
    $$PWD/SignUpScreen.cpp \
    $$PWD/TermsOfUseScreen.cpp \
    $$PWD/PrivacyPolicyScreen.cpp

!android {
    HEADERS  += \
        $$PWD/desktop/ScreenWithComboBoxesAbstract.h \
        $$PWD/desktop/LoginScreenLayout.h \
        $$PWD/desktop/ComboBoxPopupScreenBase.h \
        $$PWD/desktop/ComboBoxPopupScreen.h

    SOURCES += \
        $$PWD/desktop/ScreenWithComboBoxesAbstract.cpp \
        $$PWD/desktop/LoginScreenLayout.cpp \
        $$PWD/desktop/ComboBoxPopupScreenBase.cpp \
        $$PWD/desktop/ComboBoxPopupScreen.cpp

    FORMS += \
        $$PWD/desktop/LoginScreenLayout.ui

    INCLUDEPATH += $$PWD/desktop
}

INCLUDEPATH += $$PWD

FORMS += \
    $$PWD/BugReportLoadingScreen.ui \
    $$PWD/BugReportResultScreen.ui \
    $$PWD/BugReportScreen.ui
