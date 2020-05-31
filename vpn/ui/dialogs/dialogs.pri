SOURCES += \
    $$PWD/AccountScreen.cpp \
    $$PWD/BugReportScreen.cpp \
    $$PWD/FAQScreen.cpp \
    $$PWD/MainScreen.cpp \
    $$PWD/MainScreenLayout.cpp \
    $$PWD/PasswordRecoveryScreen.cpp \
    $$PWD/SettingsScreen.cpp \
    $$PWD/$$BRAND/SignInScreen.cpp \
    $$PWD/SignUpScreen.cpp \
    $$PWD/StartScreen.cpp

HEADERS  += \
    $$PWD/AccountScreen.h \
    $$PWD/BugReportScreen.h \
    $$PWD/FAQScreen.h \
    $$PWD/MainScreen.h \
    $$PWD/MainScreenLayout.h \
    $$PWD/PasswordRecoveryScreen.h \
    $$PWD/SettingsScreen.h \
    $$PWD/$$BRAND/SignInScreen.h \
    $$PWD/SignUpScreen.h \
    $$PWD/StartScreen.h

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
