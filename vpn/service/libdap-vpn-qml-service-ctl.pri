SOURCES += \
    $$PWD/DapServiceClient.cpp\
    $$PWD/DapServiceNativeAbstract.cpp\
    $$PWD/ServiceCtl.cpp\
    $$PWD/singlerunapplication.cpp\
    #$$PWD/usrmsg.cpp

HEADERS  += \
    $$PWD/DapServiceClient.h\
    $$PWD/DapServiceNativeAbstract.h\
    $$PWD/ServiceCtl.h\
    $$PWD/singlerunapplication.h\
    #$$PWD/usrmsg.h

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
	RC_ICONS = $$PWD/../../../brand/$$BRAND/DapChainVpnGui/resources/pics/icon_app.ico
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

    !lessThan(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 14) {
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../../brand/$$BRAND/os/android
    } else:equals(QT_MAJOR_VERSION, 5):equals(QT_MINOR_VERSION, 12) {
        message("Legacy build")
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../../brand/$$BRAND/os/android/legacy
    }

DISTFILES += \
        $$ANDROID_PACKAGE_SOURCE_DIR/*.* \
        $$ANDROID_PACKAGE_SOURCE_DIR/src/com/$${BRAND}/*.java \
        $$ANDROID_PACKAGE_SOURCE_DIR/gradle/wrapper/gradle-wrapper.jar \
        $$ANDROID_PACKAGE_SOURCE_DIR/gradlew \
        $$ANDROID_PACKAGE_SOURCE_DIR/res/values/libs.xml \
        $$ANDROID_PACKAGE_SOURCE_DIR/gradle/wrapper/gradle-wrapper.properties \
}



