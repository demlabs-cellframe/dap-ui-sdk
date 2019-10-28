SOURCES += \
    $$PWD/DapServiceClient.cpp\
    $$PWD/DapServiceNativeAbstract.cpp\
    $$PWD/DapUIAnimationScreenAbstract.cpp\
    $$PWD/DapUIAnimationScreen.cpp\
    $$PWD/DapUiDialog.cpp\
    $$PWD/DapUiMainWindow.cpp\
    $$PWD/DapUiScreen.cpp\
    $$PWD/DapUiVpnStateIndicator.cpp\
    $$PWD/schedule.cpp\
    $$PWD/schedules.cpp\
    $$PWD/ServiceCtl.cpp\
    $$PWD/SideBar.cpp\
    $$PWD/usrmsg.cpp\
    $$PWD/Utils.cpp


HEADERS  += \
    $$PWD/DapServiceClient.h\
    $$PWD/DapServiceNativeAbstract.h\
    $$PWD/DapUIAnimationScreenAbstract.h\
    $$PWD/DapUIAnimationScreen.h\
    $$PWD/DapUiDialog.h\
    $$PWD/DapUiMainWindow.h\
    $$PWD/DapUiScreen.h\
    $$PWD/DapUiVpnStateIndicator.h\
    $$PWD/schedule.h\
    $$PWD/schedules.h\
    $$PWD/ServiceCtl.h\
    $$PWD/SideBar.h\
    $$PWD/usrmsg.h\
    $$PWD/Utils.h

android{
    DEFINES += DAP_PLATFORM_MOBILE DAP_SERVICE_CONNECT_TCP
    DEFINES += DAP_PLATFORM=\\\"mobile\\\"
    QT +=  androidextras
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
        RC_ICONS =  resources/$${BRAND}/pics/icon_app.ico
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
        $$PWD/android/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/android/gradlew \
        $$PWD/android/res/values/libs.xml \
        $$PWD/android/build.gradle \
        $$PWD/android/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/android/src/com/demlabs/dap/DapChainVpnMainActivity.java \
        $$PWD/android/src/com/demlabs/dap/DapChainVpnService.java \
        $$PWD/android/src/com/demlabs/dap/DapChainVpnServiceNative.java
        $$PWD/android/gradlew.bat

        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
        QT += androidextras
        #ANDROID_ETRA_LIBS+= $$PWD/../../libDapClient/android/arm/lib/libssl.so

     #   CONFIG += link_prl
    #    ANDROID_EXTRA_LIBS = $$PWD/../../libDapClient/android/arm/lib/libssl.so $$PWD/../libDapClient/android/arm/lib/libcrypto.so
}
