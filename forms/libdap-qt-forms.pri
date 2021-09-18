INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

SOURCES     += \
    $$PWD/ui/baseform.cpp \
    $$PWD/ui/bugreports.cpp \
    $$PWD/ui/chooseserver.cpp \
    $$PWD/ui/connection.cpp \
    $$PWD/ui/documentpopup.cpp \
    $$PWD/ui/helper/auxiliary/AppStyleSheetHandler.cpp \
    $$PWD/ui/helper/auxiliary/UiScaling.cpp \
    $$PWD/ui/helper/schedule/schedule.cpp \
    $$PWD/ui/helper/schedule/schedules.cpp \
    $$PWD/ui/helper/trafficstringhelper.cpp \
    $$PWD/ui/helper/uptimestringhelper.cpp \
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
    $$PWD/ui/resetkeydialog.cpp \
    $$PWD/ui/settings.cpp \
    $$PWD/ui/splashscreen.cpp \
    $$PWD/ui/statistics.cpp

HEADERS     += \
    $$PWD/ui/baseform.h \
    $$PWD/ui/bugreports.h \
    $$PWD/ui/chooseserver.h \
    $$PWD/ui/connection.h \
    $$PWD/ui/docs.h \
    $$PWD/ui/documentpopup.h \
    $$PWD/ui/helper/auxiliary/AppStyleSheetHandler.h \
    $$PWD/ui/helper/auxiliary/UiScaling.h \
    $$PWD/ui/helper/schedule/SheduleElement.h \
    $$PWD/ui/helper/schedule/schedule.h \
    $$PWD/ui/helper/schedule/schedules.h \
    $$PWD/ui/helper/trafficstringhelper.h \
    $$PWD/ui/helper/uptimestringhelper.h \
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
    $$PWD/ui/resetkeydialog.h \
    $$PWD/ui/settings.h \
    $$PWD/ui/splashscreen.h \
    $$PWD/ui/statistics.h \
    $$PWD/vpnDefine.h

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
    $$PWD/ui/resetkeydialog.ui \
    $$PWD/ui/settings.ui \
    $$PWD/ui/splashscreen.ui \
    $$PWD/ui/statistics.ui

RESOURCES += \
    $$PWD/appRes.qrc

DISTFILES += \
    $$PWD/README.md
