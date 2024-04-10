INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
macos:LIBS += -framework AppKit

INCLUDEPATH += $$PWD

include(../../../../config.pri)

HEADERS += \
    $$PWD/dapqml-abstract/abstractcdbmanager.h \
    $$PWD/dapqml-abstract/abstractcertificate.h \
    $$PWD/dapqml-abstract/abstractserverlistmodelbridge.h \
    $$PWD/dapqml-abstract/abstractservermanager.h \
    $$PWD/dapqml-model/dapqmlmodelautoserverlist.h \
    $$PWD/dapqml-model/dapqmlmodelbugreports.h \
    $$PWD/dapqml-model/dapqmlmodelchoosecert.h \
    $$PWD/dapqml-model/dapqmlmodelchoosecountry.h \
    $$PWD/dapqml-model/dapqmlmodelchooseserver.h \
    $$PWD/dapqml-model/dapqmlmodelfullserverlist.h \
    $$PWD/dapqml-model/dapqmlmodelfaq.h \
    $$PWD/dapqml-model/dapqmlmodelorderlist.h \
    $$PWD/dapqml-model/dapqmlmodelroutingexceptions.h \
    $$PWD/dapqml-model/dapqmlmodelserialhistory.h \
    $$PWD/dapqml-model/dapqmlservermanager.h \
    $$PWD/dapqml-model/doc/doc-server-lists-and-models.h \
    $$PWD/dapqml-model/modules/orderlistmodules.h

SOURCES += \
    $$PWD/dapqml-abstract/abstractserverlistmodelbridge.cpp \
    $$PWD/dapqml-model/dapqmlmodelautoserverlist.cpp \
    $$PWD/dapqml-model/dapqmlmodelbugreports.cpp \
    $$PWD/dapqml-model/dapqmlmodelchoosecert.cpp \
    $$PWD/dapqml-model/dapqmlmodelchoosecountry.cpp \
    $$PWD/dapqml-model/dapqmlmodelchooseserver.cpp \
    $$PWD/dapqml-model/dapqmlmodelfullserverlist.cpp \
    $$PWD/dapqml-model/dapqmlmodelfaq.cpp \
    $$PWD/dapqml-model/dapqmlmodelorderlist.cpp \
    $$PWD/dapqml-model/dapqmlmodelroutingexceptions.cpp \
    $$PWD/dapqml-model/dapqmlmodelserialhistory.cpp \
    $$PWD/dapqml-model/dapqmlservermanager.cpp \
    $$PWD/dapqml-model/modules/orderlistmodules.cpp

guiapp {
HEADERS += \
    $$PWD/dapqml-model/dapqmlmodelmanagecdb.h \
    #$$PWD/dapqml-model/dapqmlmodelmanageservers.h \
    $$PWD/dapqml-model/dapqmlmodelsettings.h

SOURCES += \
    $$PWD/dapqml-model/dapqmlmodelmanagecdb.cpp \
    #$$PWD/dapqml-model/dapqmlmodelmanageservers.cpp \
    $$PWD/dapqml-model/dapqmlmodelsettings.cpp

}
