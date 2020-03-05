
include ( dialogs/dialogs.pri)
include ( controls/controls.pri)
include ( auxiliary/auxiliary.pri)


SOURCES += \
    $$PWD/DapServiceClient.cpp\
    $$PWD/DapServiceNativeAbstract.cpp\
    $$PWD/DapUiVpnStateIndicator.cpp\
    $$PWD/ServiceCtl.cpp\
    $$PWD/usrmsg.cpp\


HEADERS  += \
    $$PWD/DapServiceClient.h\
    $$PWD/DapServiceNativeAbstract.h\
    $$PWD/DapUiVpnStateIndicator.h\
    $$PWD/ServiceCtl.h\
    $$PWD/usrmsg.h

FORMS    +=  \
    $$PWD/ui/AccountScreen.ui \
    $$PWD/ui/BugReportScreen.ui \
    $$PWD/ui/ComboBoxPopup.ui \
    $$PWD/ui/FAQScreen.ui \
    $$PWD/ui/MainScreen.ui \
    $$PWD/ui/PasswordRecoveryScreen.ui \
    $$PWD/ui/MainScreenLayout.ui \
    $$PWD/ui/NavigationPanel.ui \
    $$PWD/ui/SettingsScreen.ui \
    $$PWD/ui/SignInScreen.ui \
    $$PWD/ui/SignUpScreen.ui \
    $$PWD/ui/StartScreen.ui \
    $$PWD/ui/dashboard_desktop_big.ui \
    $$PWD/ui/dashboard_desktop_small.ui \
    $$PWD/ui/dashboard_desktop.ui \
    $$PWD/ui/dashboard_mobile_hor_big.ui \
    $$PWD/ui/dashboard_mobile_hor_small.ui \
    $$PWD/ui/dashboard_mobile_hor.ui \
    $$PWD/ui/dashboard_mobile_ver_big.ui \
    $$PWD/ui/dashboard_mobile_ver_small.ui \
    $$PWD/ui/dashboard_mobile_ver.ui \
    $$PWD/ui/dlg_settings.ui \
    $$PWD/ui/dlg_about.ui \
    $$PWD/ui/forgot_password_desktop.ui \
    $$PWD/ui/sign_up_desktop.ui \
    $$PWD/ui/sign_up_desktop_small.ui \
    $$PWD/ui/sign_up_desktop_big.ui \
    $$PWD/ui/forgot_password_desktop_big.ui \
    $$PWD/ui/forgot_password_desktop_small.ui \
    $$PWD/ui/settings.ui \
    $$PWD/ui/settings_more_menu.ui \
    $$PWD/ui/sidebar_desktop.ui \
    $$PWD/ui/dlg_edit_server.ui


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
        RC_ICONS = $$PWD/../brand/$${BRAND}/DapChainVpnGui/resources/pics/icon_app.ico
}
macos {
        DEFINES += DAP_PLATFORM=\\\"desktop\\\"
        DEFINES += DAP_PLATFORM_DESKTOP DAP_SERVICE_CONNECT_TCP
        SOURCES += $$PWD/DapServiceNativeMacOS.cpp
        HEADERS += $$PWD/DapServiceNativeMacOS.h
        ICON = resources/$$BRAND/pics/$${BRAND}.icns

}




android {
    DISTFILES += \
        $$PWD/../os/android/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/../os/android/gradlew \
        $$PWD/../os/android/res/values/libs.xml \
        $$PWD/../os/android/build.gradle \
        $$PWD/../os/android/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/../os/android/src/com/demlabs/dapchain/MainActivity.java \
        $$PWD/../os/android/src/com/demlabs/dapchain/DapChainVpnService.java \
        $$PWD/../os/android/src/com/demlabs/dapchain/DapChainVpnServiceNative.java \
        $$PWD/../os/android/gradlew.bat \
        $$PWD/../os/android/AndroidManifest.xml


        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../os/android
        #QT += androidextras
        #ANDROID_ETRA_LIBS+= $$PWD/../../libDapClient/android/arm/lib/libssl.so

     #   CONFIG += link_prl
    #    ANDROID_EXTRA_LIBS = $$PWD/../../libDapClient/android/arm/lib/libssl.so $$PWD/../libDapClient/android/arm/lib/libcrypto.so
}

