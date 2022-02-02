!CONFIG(neuromorphic) {
include ( controls/controls.pri)
include ( dialogs/dialogs.pri)
}
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

INCLUDEPATH += $$PWD

android{
    DEFINES += DAP_PLATFORM_MOBILE DAP_SERVICE_CONNECT_TCP
    DEFINES += DAP_PLATFORM=\\\"mobile\\\"
    #QT +=  androidextras
    SOURCES += $$PWD/DapServiceNativeAndroid.cpp
    HEADERS += $$PWD/DapServiceNativeAndroid.h
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
        RC_ICONS = $$PWD/../../../DapChainVpnGui/resources/pics/icon_app.ico
}

darwin {
        SOURCES += $$PWD/DapServiceNativeDarwin.cpp
        HEADERS += $$PWD/DapServiceNativeDarwin.h
        ICON = resources/$$BRAND/pics/$${BRAND}.icns
}

ios{
    DEFINES += DAP_PLATFORM_MOBILE
    DEFINES += DAP_PLATFORM=\\\"mobile\\\"
}

macos {
        DEFINES += DAP_PLATFORM=\\\"desktop\\\"
        DEFINES += DAP_PLATFORM_DESKTOP DAP_SERVICE_CONNECT_TCP
}



android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../../brand/$$BRAND/os/android

    DISTFILES += \
        $$PWD/../../../brand/$$BRAND/os/android/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/../../../brand/$$BRAND/os/android/gradlew \
        $$PWD/../../../brand/$$BRAND/os/android/res/values/libs.xml \
        $$PWD/../../../brand/$$BRAND/os/android/build.gradle \
        $$PWD/../../../brand/$$BRAND/os/android/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/../../../brand/$$BRAND/os/android/src/com/$${BRAND}/MainActivity.java \
        $$PWD/../../../brand/$$BRAND/os/android/src/com/$${BRAND}/$${BRAND}Service.java \
        $$PWD/../../../brand/$$BRAND/os/android/src/com/$${BRAND}/$${BRAND}ServiceNative.java \
        $$PWD/../../../brand/$$BRAND/os/android/gradlew.bat
        #equals(BUILD_VARIANT, "GooglePlay") {
        #        DISTFILES += $$PWD/../../../brand/$$BRAND/os/android/src/com/$${BRAND}/InAppShop.java
        #}
}



