
include ( dialogs/dialogs.pri)
include ( controls/controls.pri)
include ( auxiliary/auxiliary.pri)

SOURCES += \
    $$PWD/DapServiceClient.cpp\
    $$PWD/DapServiceNativeAbstract.cpp\
    $$PWD/ServiceCtl.cpp\
    $$PWD/usrmsg.cpp

HEADERS  += \
    $$PWD/DapServiceClient.h\
    $$PWD/DapServiceNativeAbstract.h\
    $$PWD/ServiceCtl.h\
    $$PWD/usrmsg.h

FORMS    +=  \
    $$PWD/ui/$$BRAND/AccountScreen.ui \
    $$PWD/ui/$$BRAND/BugReportScreen.ui \
    $$PWD/ui/ComboBoxPopup.ui \
    $$PWD/ui/FAQScreen.ui \
    $$PWD/ui/MainScreenLayout.ui \
    $$PWD/ui/$$BRAND/NavigationPanelBack.ui \
    $$PWD/ui/PasswordRecoveryScreen.ui


INCLUDEPATH += $$PWD

android{
    DEFINES += DAP_PLATFORM_MOBILE DAP_SERVICE_CONNECT_TCP
    DEFINES += DAP_PLATFORM=\\\"mobile\\\"
    #QT +=  androidextras
    SOURCES += $$PWD/DapServiceNativeAndroid.cpp
    HEADERS += $$PWD/DapServiceNativeAndroid.h
}

ios{
    DEFINES += DAP_PLATFORM_MOBILE
    DEFINES += DAP_PLATFORM=\\\"mobile\\\"
    SOURCES += $$PWD/DapServiceNativeIOS.cpp
    HEADERS += $$PWD/DapServiceNativeIOS.h
}

linux-* {
        DEFINES += DAP_PLATFORM=\\\"desktop\\\"
        DEFINES += DAP_PLATFORM_DESKTOP
        SOURCES += $$PWD/DapServiceNativeLinux.cpp
        HEADERS += $$PWD/DapServiceNativeLinux.h
}

win32 {
        DEFINES += DAP_PLATFORM=\\\"desktop\\\"
        DEFINES += DAP_PLATFORM_DESKTOP
        SOURCES += $$PWD/DapServiceNativeWindows.cpp
        HEADERS += $$PWD/DapServiceNativeWindows.h
        DEFINES += NTDDI_VERSION=0x06000000
        DEFINES += _WIN32_WINNT=0x0600
        RC_ICONS = $$PWD/../../../brand/$${BRAND}/DapChainVpnGui/resources/pics/icon_app.ico
}
macos {
        DEFINES += DAP_PLATFORM=\\\"desktop\\\"
        DEFINES += DAP_PLATFORM_DESKTOP DAP_SERVICE_CONNECT_TCP
        SOURCES += $$PWD/DapServiceNativeMacOS.cpp
        HEADERS += $$PWD/DapServiceNativeMacOS.h
        ICON = resources/$$BRAND/pics/$${BRAND}.icns
}




android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../../brand/$$BRAND/os/android

    DISTFILES += \
        $$PWD/../../../os/android/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/../../../os/android/gradlew \
        $$PWD/../../../os/android/res/values/libs.xml \
        $$PWD/../../../os/android/build.gradle \
        $$PWD/../../../os/android/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/../../../os/android/src/com/demlabs/dapchain/MainActivity.java \
        $$PWD/../../../os/android/src/com/demlabs/dapchain/DapChainVpnService.java \
        $$PWD/../../../os/android/src/com/demlabs/dapchain/DapChainVpnServiceNative.java \
        $$PWD/../../../os/android/gradlew.bat
}


contains(DEFINES, DAP_PLATFORM_MOBILE) {
    FORMS += \
        $$PWD/ui/mobile/MainScreen.ui \
        $$PWD/ui/mobile/SignInScreen.ui \
        $$PWD/ui/mobile/StartScreen.ui \
        $$PWD/ui/mobile/SettingsScreen.ui\
        $$PWD/ui/mobile/SignUpScreen.ui \
        $$PWD/ui/mobile/NavigationPanelMain.ui
}
else {
    FORMS +=  \
        $$PWD/ui/$$BRAND/desktop/StartScreen.ui \
        $$PWD/ui/$$BRAND/desktop/SignInScreen.ui \
        $$PWD/ui/$$BRAND/desktop/SettingsScreen.ui \
        $$PWD/ui/$$BRAND/desktop/SignUpScreen.ui\
        $$PWD/ui/$$BRAND/desktop/MainScreen.ui\
        $$PWD/ui/desktop/LoginScreenLayout.ui \
        $$PWD/ui/$$BRAND/desktop/NavigationPanelMain.ui
}

