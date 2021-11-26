INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

SOURCES     += \
    $$PWD/ui/baseform.cpp \
    $$PWD/ui/bugreports.cpp \
    $$PWD/ui/chooseserver.cpp \
    $$PWD/ui/choosetheme.cpp \
    $$PWD/ui/dashboard.cpp \
    $$PWD/ui/documentpopup.cpp \
    $$PWD/ui/helper/auxiliary/AppStyleSheetHandler.cpp \
    $$PWD/ui/helper/auxiliary/UiScaling.cpp \
    $$PWD/ui/helper/gradientwidget.cpp \
    $$PWD/ui/helper/schedule/schedule.cpp \
    $$PWD/ui/helper/schedule/schedules.cpp \
    $$PWD/ui/helper/serialinputfield.cpp \
    $$PWD/ui/helper/trafficstringhelper.cpp \
    $$PWD/ui/helper/uptimestringhelper.cpp \
    $$PWD/ui/history.cpp \
    $$PWD/ui/language.cpp \
    $$PWD/ui/login.cpp \
    $$PWD/ui/menufooter.cpp \
    $$PWD/ui/model/bugreportsmodel.cpp \
    $$PWD/ui/model/chooseservermodel.cpp \
    $$PWD/ui/model/choosethememodel.cpp \
    $$PWD/ui/model/historymodel.cpp \
    $$PWD/ui/model/languagemodel.cpp \
    $$PWD/ui/model/modelbase.cpp \
    $$PWD/ui/model/purchasemodel.cpp \
    $$PWD/ui/model/settingsmodel.cpp \
    $$PWD/ui/popupdialog.cpp \
    $$PWD/ui/purchase.cpp \
    $$PWD/ui/serialinput.cpp \
    $$PWD/ui/settings.cpp \
    $$PWD/ui/splashscreen.cpp \
    $$PWD/ui/statistics.cpp \
    $$PWD/ui/versionupdate.cpp

HEADERS     += \
    $$PWD/ui/baseform.h \
    $$PWD/ui/bugreports.h \
    $$PWD/ui/chooseserver.h \
    $$PWD/ui/choosetheme.h \
    $$PWD/ui/dashboard.h \
    $$PWD/ui/docs.h \
    $$PWD/ui/documentpopup.h \
    $$PWD/ui/helper/auxiliary/AppStyleSheetHandler.h \
    $$PWD/ui/helper/auxiliary/UiScaling.h \
    $$PWD/ui/helper/gradientwidget.h \
    $$PWD/ui/helper/schedule/SheduleElement.h \
    $$PWD/ui/helper/schedule/schedule.h \
    $$PWD/ui/helper/schedule/schedules.h \
    $$PWD/ui/helper/serialinputfield.h \
    $$PWD/ui/helper/trafficstringhelper.h \
    $$PWD/ui/helper/uptimestringhelper.h \
    $$PWD/ui/history.h \
    $$PWD/ui/language.h \
    $$PWD/ui/login.h \
    $$PWD/ui/menufooter.h \
    $$PWD/ui/model/bugreportsmodel.h \
    $$PWD/ui/model/chooseservermodel.h \
    $$PWD/ui/model/choosethememodel.h \
    $$PWD/ui/model/historymodel.h \
    $$PWD/ui/model/languagemodel.h \
    $$PWD/ui/model/modelbase.h \
    $$PWD/ui/model/purchasemodel.h \
    $$PWD/ui/model/settingsmodel.h \
    $$PWD/ui/popupdialog.h \
    $$PWD/ui/purchase.h \
    $$PWD/ui/serialinput.h \
    $$PWD/ui/settings.h \
    $$PWD/ui/splashscreen.h \
    $$PWD/ui/statistics.h \
    $$PWD/ui/versionupdate.h \
    $$PWD/vpnDefine.h

FORMS += \
    $$PWD/ui/brand_ui/$$BRAND/bugreports.ui \
    $$PWD/ui/brand_ui/$$BRAND/chooseserver.ui \
    $$PWD/ui/brand_ui/$$BRAND/dashboard.ui \
    $$PWD/ui/brand_ui/$$BRAND/documentpopup.ui \
    $$PWD/ui/brand_ui/$$BRAND/history.ui \
    $$PWD/ui/brand_ui/$$BRAND/language.ui \
    $$PWD/ui/brand_ui/$$BRAND/login.ui \
    $$PWD/ui/brand_ui/$$BRAND/menufooter.ui \
    $$PWD/ui/brand_ui/$$BRAND/popupdialog.ui \
    $$PWD/ui/brand_ui/$$BRAND/purchase.ui \
    $$PWD/ui/brand_ui/$$BRAND/serialinput.ui \
    $$PWD/ui/brand_ui/$$BRAND/settings.ui \
    $$PWD/ui/brand_ui/$$BRAND/splashscreen.ui \
    $$PWD/ui/brand_ui/$$BRAND/statistics.ui \
    $$PWD/ui/brand_ui/$$BRAND/versionupdate.ui \
    $$PWD/ui/brand_ui/$$BRAND/choosetheme.ui

RESOURCES += \
    $$PWD/ui/brand_ui/$$BRAND/appRes.qrc

DISTFILES += \
    $$PWD/DoxyLayout.xml \
    $$PWD/README.md
