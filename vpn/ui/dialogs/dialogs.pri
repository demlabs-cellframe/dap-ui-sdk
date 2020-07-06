HEADERS  += \
    $$PWD/AccountScreen.h \
    $$PWD/BugReportScreen.h \
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
    $$PWD/BugReportScreen.cpp \
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

    INCLUDEPATH += $$PWD/desktop
}


INCLUDEPATH += $$PWD $$PWD/$$BRAND
