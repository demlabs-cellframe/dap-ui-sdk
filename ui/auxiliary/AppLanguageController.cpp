#include "AppLanguageController.h"

#include <QApplication>
#include <QDebug>
#include <DapDataLocal.h>

AppLanguageController::AppLanguageController(QList<QLocale::Language> a_languages, QObject *a_parent)
    :QObject(a_parent)
{
    m_languagesModel.appendLanguages(a_languages);
    QLocale::Language systemLanguage = QLocale::system().language();
    QLocale::Language savedLanguage;

    if (DapDataLocal::instance()->loadFromSettings(SETTING_LOCALE, savedLanguage))
    {
        QLocale::Language savedSystemLanguage;
        if (DapDataLocal::instance()->loadFromSettings(SETTING_SYS_LOCALE, savedSystemLanguage))
        {
            if (systemLanguage == savedSystemLanguage) //locale was changed;
                this->retranslateApp(savedLanguage);
            else
            {
                DapDataLocal::removeSetting(SETTING_LOCALE);
                DapDataLocal::instance()->saveToSettings(SETTING_SYS_LOCALE, systemLanguage);
                m_languagesModel.isExist(systemLanguage) ? this->retranslateApp(systemLanguage)
                                                         : this->retranslateApp(m_defaultLanguage);
            }
        }
        else
        {
            DapDataLocal::instance()->saveToSettings(SETTING_SYS_LOCALE, systemLanguage);
            this->retranslateApp(savedLanguage);
        }
    }
    else
    {
        DapDataLocal::instance()->saveToSettings(SETTING_SYS_LOCALE, systemLanguage);
        m_languagesModel.isExist(systemLanguage) ? this->retranslateApp(systemLanguage)
                                                 : this->retranslateApp(m_defaultLanguage);
    }
}

QLocale::Language AppLanguageController::currentLanguage() const
{
    return m_language;
}

void AppLanguageController::setCurrentLanguage(QLocale::Language a_language)
{
    QLocale savedLocale;
    bool localeWasSaved = DapDataLocal::instance()->loadFromSettings(SETTING_LOCALE, savedLocale);

    if (localeWasSaved && savedLocale == a_language)
        return;

    DapDataLocal::instance()->saveToSettings(SETTING_LOCALE ,a_language);
    this->retranslateApp(a_language);
}

LanguagesModel* AppLanguageController::languagesModel()
{
    return &m_languagesModel;
}

void AppLanguageController::setCurrentLanguageIndex(int a_index)
{
    this->setCurrentLanguage(this->languagesModel()->language(a_index));
}

void AppLanguageController::retranslateApp(QLocale::Language a_language)
{
    if (m_language == a_language)
        return;
    m_language = a_language;

    QLocale l(a_language);

    this->m_qtLanguageTranslator.load(a_language, "dapChainVPNClient", "_", ":/lang/", ".qm");
    QApplication::instance()->installTranslator(&m_qtLanguageTranslator);
    emit this->appRetranslated();
}

/////////////////////////////////////////

QDataStream &operator>>(QDataStream &a_inStream, QLocale::Language &a_language)
{
    QByteArray strLanguage;
    a_inStream >> strLanguage;
    a_language = static_cast<QLocale::Language>(strLanguage.toInt());

    return a_inStream;
}

QDataStream &operator<<(QDataStream &a_outStream, QLocale::Language a_serialKeyData)
{
    a_outStream << QByteArray::number(a_serialKeyData);

    return a_outStream;
}
