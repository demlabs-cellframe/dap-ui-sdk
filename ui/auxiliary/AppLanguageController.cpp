#include "AppLanguageController.h"

#include <QApplication>
#include <QDebug>
#include <DapDataLocal.h>

AppLanguageController::AppLanguageController(QObject *a_parent)
    :QObject(a_parent)
{}

AppLanguageController *AppLanguageController::instance()
{
    static AppLanguageController s_instance;

    return &s_instance;
}

Language AppLanguageController::appLanguage()
{
    return AppLanguageController::stringToLanguage(AppLanguageController::appLanguageString());
}

QString AppLanguageController::appLanguageString()
{
    QVariant local = DapDataLocal::instance()->getEncriptedSetting(instance()->SETTING_NAME);
    QString strLocal;

    if (!local.isValid())
    {
        strLocal = QLocale::system().name();
        instance()->setAppLanguage(strLocal);
    }
    else
        strLocal = local.toString();

    return strLocal;
}

void AppLanguageController::retranslateApp(Language a_language)
{
    QString strLanguage;
    if (a_language == Language::Undefined)
    {
        strLanguage = AppLanguageController::appLanguageString();
    }
    else
    {
        strLanguage = AppLanguageController::languageToString(a_language);
        if (strLanguage == AppLanguageController::instance()->appLanguageString())
            return;
    }

    if (!DapDataLocal::instance()->getEncriptedSetting(instance()->SETTING_SYS_LANGUAGE).isValid() ||
                QLocale().language()!=DapDataLocal::instance()->getEncriptedSetting(instance()->SETTING_SYS_LANGUAGE))
        {
            DapDataLocal::instance()->saveEncriptedSetting(instance()->SETTING_SYS_LANGUAGE,QLocale().language());
            if (QLocale().language()==QLocale::Chinese)
                strLanguage=AppLanguageController::languageToString(Language::Zh);
            else
                strLanguage=AppLanguageController::languageToString(Language::En);
        }

    if (strLanguage.isEmpty())
        return;

    instance()->setAppLanguage(strLanguage);

    instance()->m_qtLanguageTranslator.load(":/translations/dapChainVPNClient_" + strLanguage);
    QApplication::instance()->installTranslator(&instance()->m_qtLanguageTranslator);

    emit instance()->appRetranslated();
}

void AppLanguageController::setAppLanguage(QString a_language)
{
    DapDataLocal::instance()->saveEncriptedSetting(this->SETTING_NAME, a_language);
}

QString AppLanguageController::languageToString(Language a_language)
{
    if (instance()->m_languageStrings.contains(a_language))
        return instance()->m_languageStrings[a_language];

    return "";
}

Language AppLanguageController::stringToLanguage(const QString& a_languageStr)
{
    Language lang = instance()->m_languageStrings.key(a_languageStr, Language::Undefined);

    if (lang == Language::Undefined)
        qWarning()<< "Language string is not found";

    return lang;
}
