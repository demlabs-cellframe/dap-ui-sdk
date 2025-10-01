QT += core network

include (../../cellframe-ui-sdk/Core/libdap-qt-helpers.pri)
SOURCES += \
    $$PWD/DapPluginsPathControll.cpp \
    $$PWD/DapUtils.cpp

HEADERS += \
    $$PWD/DapLocationInfo.h \
    $$PWD/DapPluginsPathControll.h \
    $$PWD/DapUtils.h 


INCLUDEPATH += $$PWD
# INCLUDEPATH += $$PWD/../quazip
