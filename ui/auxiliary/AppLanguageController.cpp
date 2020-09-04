#include "AppLanguageController.h"

#include <QApplication>
#include <QDebug>
#include <DapDataLocal.h>

AppLanguageController::AppLanguageController(QObject *a_parent)
    :QObject(a_parent)
{
    QLocale savedLocale;
    if (DapDataLocal::instance()->loadFromSettings(instance()->SETTING_LOCALE, savedLocale))
    {
        QLocale savedSysLocale;
        if (DapDataLocal::instance()->loadFromSettings(SETTING_SYS_LOCALE, savedSysLocale))
        {
            if (QLocale::system() == savedSysLocale) //locale was changed;
                QLocale::setDefault(QLocale(savedLocale));
            else
                DapDataLocal::removeSetting(SETTING_LOCALE);
        }
    }
}

AppLanguageController *AppLanguageController::instance()
{
    static AppLanguageController s_instance;

    return &s_instance;
}

void AppLanguageController::setLocale(QLocale a_locale)
{
    QLocale savedLocale;
    bool localeWasSaved = DapDataLocal::instance()->loadFromSettings(instance()->SETTING_LOCALE, savedLocale);

    if (localeWasSaved && savedLocale == a_locale)
        return;

    DapDataLocal::instance()->saveToSettings(SETTING_LOCALE ,a_locale);
    QLocale::setDefault(a_locale);
    retranslateApp(a_locale);
}

void AppLanguageController::retranslateApp(QLocale a_locale /*= QLocale()*/)
{
//    instance()->m_qtLanguageTranslator.load(a_locale, "dapChainVPNClient", "_", ":/translations/", ".qm");
//    QApplication::instance()->installTranslator(&instance()->m_qtLanguageTranslator);
}
