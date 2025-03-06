#ifndef UTILS_H
#define UTILS_H

#include <QVariant>
#include <QDebug>
#include <QRegularExpression>
#include <QtMath>
#include <QDateTime>

class QMainWindow;

namespace Utils
{
    ///Returns a regular expression for the email match.
    QRegularExpression regularEmail();

    ///Returns the required version type
    enum TypeVersion{MAJOR,MINOR,PAT};
    int getNumberVersion(const QString &version,TypeVersion type);
    bool isNewerVersion(const QString& availableVer, const QString currVer);
    QString getOSName();

    ///The function returns the first integer in the string.
    /// 100%=>100 100px=>100 100**=>100 100=>100
    int toIntValue(const QString &a_text);

    QString convertByte(const quint64 &byte);

    QString toNativeLanguageName(QLocale::Language a_language);

    bool isVersionLessThan(const QString& version1, const QString& version2);

    template <class T>
    inline T findChild(QObject* a_object, const QString& a_objectName, T &a_foundObjectOUT);

    template <class T>
    inline T findChild(QObject* a_object, const QString& a_objectName);

    template <class T>
    T findParent(QObject* a_object);

    QString getTextFromFile(const QString &a_fname);
};

template <class T>
inline T Utils::findChild(QObject* a_parentObj, const QString &a_objectName)
{
    Q_ASSERT_X(a_parentObj,  "Utils::findObjectChild", "parentObj can't be nullptr");

    T l_foundObject = a_parentObj->findChild<T>(a_objectName);

    if (!l_foundObject)
    {
        QString errorMsg = QString("Can't find \"%1\" object in \"%2\"")
                .arg(a_objectName)
                .arg(a_parentObj->objectName());
        qDebug() << errorMsg;
        QByteArray tmpString = errorMsg.toLatin1();
        qFatal("%s", tmpString.data());
    }

    return l_foundObject;
}

template <class T>
inline T Utils::findChild(QObject* a_parentObj, const QString &a_objectName, T &a_foundObjectOUT)
{
    return a_foundObjectOUT = findChild<T>(a_parentObj, a_objectName);
}

template <class T>
T Utils::findParent(QObject* a_object)
{
    if (!a_object)
            return nullptr;

    QMainWindow *mainWindow = qobject_cast<T>(a_object);
    if (mainWindow)
        return mainWindow;

    return Utils::findParent<T>(a_object->parent());
}

#endif // UTILS_H


