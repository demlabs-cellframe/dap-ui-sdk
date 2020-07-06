SOURCES += \
    $$PWD/AccountScreen.cpp \
    $$PWD/BugReportScreen.cpp \
    $$PWD/MainScreen.cpp \
    $$PWD/MainScreenLayout.cpp \
    $$PWD/PasswordRecoveryScreen.cpp \
    $$PWD/StartScreen.cpp \
    $$PWD/NavigationPanel.cpp \
    $$PWD/SignUpScreen.cpp \
    $$PWD/TermsOfUseScreen.cpp \
    $$PWD/PrivacyPolicyScreen.cpp

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

!android {
    HEADERS  += \
        $$PWD/desktop/ScreenWithComboBoxesAbstract.h \
        $$PWD/desktop/ComboBoxPopupScreen.h \
        $$PWD/desktop/LoginScreenLayout.h

    SOURCES += \
        $$PWD/desktop/ScreenWithComboBoxesAbstract.cpp \
        $$PWD/desktop/ComboBoxPopupScreen.cpp \
        $$PWD/desktop/LoginScreenLayout.cpp


    INCLUDEPATH += $$PWD/desktop
}


INCLUDEPATH += $$PWD $$PWD/$$BRAND
