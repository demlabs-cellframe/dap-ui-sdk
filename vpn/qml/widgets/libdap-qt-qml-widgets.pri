INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

INCLUDEPATH += $$PWD

include(../config.pri)

RESOURCES += \
    $$PWD/dapqml-widgets-asset/brand/$$BRAND/dapqml-widgets-asset-brand.qrc \
    $$PWD/dapqml-widgets-asset/brand/$$BRAND/dapqml-widgets-style-brand.qrc \
    $$PWD/dapqml-widgets-asset/nonbrand/nonbrandAsset.qrc \
    $$PWD/dapqml-widgets.qrc

HEADERS += \
    $$PWD/dapqml-widgets/dapqmlimage.h \
    $$PWD/dapqml-widgets/dapqmlserialkeyinput.h \
    $$PWD/dapqml-widgets/dapqmlstyle.h \
    $$PWD/dapqml-widgets/dapqmlthememodel.h \
    $$PWD/dapqml-widgets/dapqmlwindow.h \
    $$PWD/dapqml-widgets/doc-group-dapqmlstyle.h \
    $$PWD/dapqml-widgets/doc-group-qml-widgets.h \
    $$PWD/dapqml-widgets/style/debug/styledebugitemdescriptor.h \
    $$PWD/dapqml-widgets/style/qssitem.h \
    $$PWD/dapqml-widgets/style/qsslink.h \
    $$PWD/dapqml-widgets/style/qssmap.h \
    $$PWD/dapqml-widgets/style/qssparser.h \
    $$PWD/dapqml-widgets/style/qssvalue.h \
    $$PWD/dapqml-widgets/style/scaled.h \
    $$PWD/dapqml-widgets/style/debug/styledebugtree.h

SOURCES += \
    $$PWD/dapqml-widgets/dapqmlimage.cpp \
    $$PWD/dapqml-widgets/dapqmlserialkeyinput.cpp \
    $$PWD/dapqml-widgets/dapqmlstyle.cpp \
    $$PWD/dapqml-widgets/dapqmlthememodel.cpp \
    $$PWD/dapqml-widgets/dapqmlwindow.cpp \
    $$PWD/dapqml-widgets/style/debug/styledebugitemdescriptor.cpp \
    $$PWD/dapqml-widgets/style/qssitem.cpp \
    $$PWD/dapqml-widgets/style/qsslink.cpp \
    $$PWD/dapqml-widgets/style/qssmap.cpp \
    $$PWD/dapqml-widgets/style/qssparser.cpp \
    $$PWD/dapqml-widgets/style/qssvalue.cpp \
    $$PWD/dapqml-widgets/style/scaled.cpp \
    $$PWD/dapqml-widgets/style/debug/styledebugtree.cpp
