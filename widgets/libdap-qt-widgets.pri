INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

SOURCES     += \
    $$PWD/kelguicommon.cpp

# add plugin definitions for QtDesigner
CONFIG(plugin) {
HEADERS     += \
    $$PWD/kelguibuttonplugin.h \
    $$PWD/kelguicheckboxplugin.h \
    $$PWD/kelguilabelplugin.h \
    $$PWD/kelguiplaintexteditplugin.h \
    $$PWD/kelguipushbuttonplugin.h \
    $$PWD/kelguiradioplugin.h \
    $$PWD/kelguiseparatorplugin.h \
    $$PWD/kelguiwidgetplugin.h \
    $$PWD/kelvpnneuromorphic.h

SOURCES     += \
    $$PWD/kelguibuttonplugin.cpp \
    $$PWD/kelguicheckboxplugin.cpp \
    $$PWD/kelguilabelplugin.cpp \
    $$PWD/kelguiplaintexteditplugin.cpp \
    $$PWD/kelguipushbuttonplugin.cpp \
    $$PWD/kelguiradioplugin.cpp \
    $$PWD/kelguiseparatorplugin.cpp \
    $$PWD/kelguiwidgetplugin.cpp \
    $$PWD/kelvpnneuromorphic.cpp
# message ($$HEADERS)
}

RESOURCES += \
    $$PWD/widgetsRes.qrc

DISTFILES += \
    $$PWD/README.md

INCLUDEPATH += $$PWD

include(kelguibutton.pri)
include(kelguicheckbox.pri)
include(kelguiplaintextedit.pri)
include(kelguipushbutton.pri)
include(kelguiradio.pri)
include(kelguiseparator.pri)
include(kelguilabel.pri)
include(kelguiwidget.pri)
include(style/kelguistylemanager.pri)

HEADERS += \
    $$PWD/kelguicommon.h
