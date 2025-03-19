HEADERS += \
      
        $$PWD/DapFs.h \
        
SOURCES += \
        $$PWD/DapFs.cpp

android {
    DEFINES += USE_QT_FILESYSTEM
}


INCLUDEPATH += $$PWD
