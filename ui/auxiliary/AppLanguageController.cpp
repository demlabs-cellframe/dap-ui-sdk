#include "AppLanguageController.h"

#include <QApplication>
#include <QDebug>

AppLanguageController::AppLanguageController(QObject *a_parent)
    :QObject()
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
    QVariant local = instance()->m_settings.value(instance()->SETTING_NAME);
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
        a_language = AppLanguageController::stringToLanguage(strLanguage);
    }
    else
    {
        strLanguage = AppLanguageController::languageToString(a_language);
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
    this->m_settings.setValue(this->SETTING_NAME, a_language);
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
