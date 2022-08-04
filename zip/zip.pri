LIBS += -lz

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        $$PWD/packzip.cpp \
        $$PWD/zipbase.cpp

HEADERS += \
    $$PWD/packzip.h \
    $$PWD/zipbase.h \
    $$PWD/zipdef.h

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
