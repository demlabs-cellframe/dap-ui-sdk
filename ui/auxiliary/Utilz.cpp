#include "Utilz.h"

#include <QVector>
#include <QVariant>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QRegularExpression>


namespace Utils
{
    QRegularExpression regularEmail()
    {
        return QRegularExpression("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", QRegularExpression::CaseInsensitiveOption) ;
    }


    int getNumberVersion(const QString &version,TypeVersion type)
    {
        int minVersion(0);
        int majVersion(0);
        int patVersion(0);

        QStringList versionList = version.split(".");
        if(!versionList[0].isEmpty())
            majVersion = versionList[0].toInt();
        if(versionList.count()>1)
        {
            QStringList minVersionList = versionList[1].split("-");
            if(!minVersionList[0].isEmpty())
                minVersion = minVersionList[0].toInt();
            if(minVersionList.count()>1)
                if(!minVersionList[1].isEmpty())
                    patVersion = minVersionList[1].toInt();
        }

        if(type == TypeVersion::MAJOR)
            return majVersion;
        if(type == TypeVersion::MINOR)
            return minVersion;
        if(type == TypeVersion::PAT)
            return patVersion;

        return 0;
    }

    bool isNewerVersion(const QString& availableVer, const QString currVer)
    {
        auto verTypes = {TypeVersion::MAJOR, TypeVersion::MINOR, TypeVersion::PAT};
        for (auto verType : verTypes)
        {
            if (getNumberVersion(availableVer, verType) > getNumberVersion(currVer, verType))
                return true;
            else if (getNumberVersion(availableVer, verType) < getNumberVersion(currVer, verType))
                break;
        }
        return false;
    }

    QString getOSName()
    {
        #if defined(Q_OS_ANDROID)
        return QLatin1String("android");
        #elif defined(Q_OS_MACOS)
        return QLatin1String("mac");
        #elif defined(Q_OS_WIN)
        return QLatin1String("windows");
        #elif defined(Q_OS_IOS)
        return QLatin1String("ios");
        #elif defined(Q_OS_LINUX)
        return QLatin1String("linux");
        #else
        return QLatin1String("unknown");
        #endif
    }

    int toIntValue(const QString &a_text)
    {
        QRegularExpression regString("(\\d+)");
        QRegularExpressionMatch match = regString.match(a_text);
        if (match.hasMatch()) {
            return match.captured(0).toInt();
        }
        return 0;
    }

    QString convertByte(const quint64 &byte)
    {
        if (byte < 0){
            return QString("error");
        } else if (byte >= pow(2,40)){
            return QString("%1 %2").arg(QString::number(byte/pow(2,40), 'f', 3)).arg("Tb");
        } else if (byte >= pow(2,30)){
            return QString("%1 %2").arg(QString::number(byte/pow(2,30), 'f', 3)).arg("Gb");
        } else if (byte >= pow(2,20)){
            return QString("%1 %2").arg(QString::number(byte/pow(2,20), 'f', 3)).arg("Mb");
        } else if (byte >= pow(2,10)){
            return QString("%1 %2").arg(QString::number(byte/pow(2,10), 'f', 3)).arg("Kb");
        } else return QString("%1 %2").arg(QString::number(byte)).arg("bytes");
    }

    QString getTextFromFile(const QString &a_fname){
        QString fileText;
        QFile file(a_fname);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            fileText = QString(file.readAll());
            file.close();
        }
        return fileText;
    }

    QString toNativeLanguageName(QLocale::Language a_language)
    {
        switch (a_language)
        {
            case QLocale::English   : return "English";
            case QLocale::Chinese   : return "中文";
            case QLocale::Russian   : return "Русский";
            case QLocale::Ukrainian : return "Український";
            case QLocale::French    : return "Française";
            case QLocale::Korean    : return "한국어";
            case QLocale::Thai      : return "ไทย";
            case QLocale::Khmer     : return "ខ្មែរ";
            default: return "";
        }
    }
}
