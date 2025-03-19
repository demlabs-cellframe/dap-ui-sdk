HEADERS += \
      
        $$PWD/DapFs.h \
        
SOURCES += \
        $$PWD/DapFs.cpp

android {
    DEFINES += USE_QT_FILESYSTEM
}


unix:!android:!macos {
    LIBS += -lstdc++fs

}

INCLUDEPATH += $$PWD
