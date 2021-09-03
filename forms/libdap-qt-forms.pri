INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

SOURCES     += \
    $$PWD/ui/bugreports.cpp \
    $$PWD/ui/chooseserver.cpp \
    $$PWD/ui/connection.cpp \
    $$PWD/ui/documentpopup.cpp \
    $$PWD/ui/history.cpp \
    $$PWD/ui/language.cpp \
    $$PWD/ui/login.cpp \
    $$PWD/ui/menufooter.cpp \
    $$PWD/ui/model/chooseservermodel.cpp \
    $$PWD/ui/model/historymodel.cpp \
    $$PWD/ui/model/languagemodel.cpp \
    $$PWD/ui/model/modelbase.cpp \
    $$PWD/ui/model/purchasemodel.cpp \
    $$PWD/ui/model/settingsmodel.cpp \
    $$PWD/ui/purchase.cpp \
    $$PWD/ui/settings.cpp \
    $$PWD/ui/splashscreen.cpp \
    $$PWD/ui/statistics.cpp

HEADERS     += \
    $$PWD/ui/bugreports.h \
    $$PWD/ui/chooseserver.h \
    $$PWD/ui/connection.h \
    $$PWD/ui/docs.h \
    $$PWD/ui/documentpopup.h \
    $$PWD/ui/history.h \
    $$PWD/ui/language.h \
    $$PWD/ui/login.h \
    $$PWD/ui/menufooter.h \
    $$PWD/ui/model/chooseservermodel.h \
    $$PWD/ui/model/historymodel.h \
    $$PWD/ui/model/languagemodel.h \
    $$PWD/ui/model/modelbase.h \
    $$PWD/ui/model/purchasemodel.h \
    $$PWD/ui/model/settingsmodel.h \
    $$PWD/ui/purchase.h \
    $$PWD/ui/settings.h \
    $$PWD/ui/splashscreen.h \
    $$PWD/ui/statistics.h

FORMS += \
    $$PWD/ui/bugreports.ui \
    $$PWD/ui/chooseserver.ui \
    $$PWD/ui/connection.ui \
    $$PWD/ui/documentpopup.ui \
    $$PWD/ui/history.ui \
    $$PWD/ui/language.ui \
    $$PWD/ui/login.ui \
    $$PWD/ui/menufooter.ui \
    $$PWD/ui/purchase.ui \
    $$PWD/ui/settings.ui \
    $$PWD/ui/splashscreen.ui \
    $$PWD/ui/statistics.ui

RESOURCES += \
    $$PWD/appRes.qrc

DISTFILES += \
    $$PWD/README.md
