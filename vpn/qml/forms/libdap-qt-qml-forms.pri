INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

INCLUDEPATH += $$PWD

include(../../../../config.pri)

RESOURCES += \
    $$PWD/dapqml-forms-asset/$$BRAND/dapqml-forms-asset.qrc \
    $$PWD/dapqml-forms.qrc

HEADERS += \
    $$PWD/dapqml-abstract/abstractcdbmanager.h \
    $$PWD/dapqml-abstract/abstractcertificate.h \
    $$PWD/dapqml-abstract/abstractservermanager.h \
    $$PWD/dapqml-forms/doc-group-qml-forms.h \
    $$PWD/dapqml-model/chooseserver/autoservermodel.h \
    $$PWD/dapqml-model/dapqmlmodelbugreports.h \
    $$PWD/dapqml-model/dapqmlmodelchoosecert.h \
    $$PWD/dapqml-model/dapqmlmodelchooseserver.h \
    $$PWD/dapqml-model/dapqmlmodelmanagecdb.h \
    $$PWD/dapqml-model/dapqmlmodelmanageservers.h \
    $$PWD/dapqml-model/dapqmlmodelserialhistory.h \
    $$PWD/dapqml-model/dapqmlmodelsettings.h

SOURCES += \
    $$PWD/dapqml-model/chooseserver/autoservermodel.cpp \
    $$PWD/dapqml-model/dapqmlmodelbugreports.cpp \
    $$PWD/dapqml-model/dapqmlmodelchoosecert.cpp \
    $$PWD/dapqml-model/dapqmlmodelchooseserver.cpp \
    $$PWD/dapqml-model/dapqmlmodelmanagecdb.cpp \
    $$PWD/dapqml-model/dapqmlmodelmanageservers.cpp \
    $$PWD/dapqml-model/dapqmlmodelserialhistory.cpp \
    $$PWD/dapqml-model/dapqmlmodelsettings.cpp
