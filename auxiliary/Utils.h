#ifndef UTILS_H
#define UTILS_H

#include <QColor>
#include <QWidget>
#include <QVariant>
#include <QDebug>

namespace Utils
{
    QColor toColor(const QString &strRGBA);
    ///The function returns the first integer in the string.
    /// 100%=>100 100px=>100 100**=>100 100=>100
    int toIntValue(const QString &a_text);

    template <class T>
    inline T findChild(QObject* a_object, const QString& a_objectName, T &a_foundObjectOUT);

    template <class T>
    inline T findChild(QObject* a_object, const QString& a_objectName);

    void setPropertyAndUpdateStyle(QWidget* a_widget, const QString& a_property, const QVariant& a_value = true);
    void setPropertyAndUpdateStyle(QWidget* a_widget, const char*    a_property, const QVariant& a_value = true);
    void updateStyle(QWidget* a_widget);
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
        qFatal(errorMsg.toLatin1().data());
    }

    return l_foundObject;
}

template <class T>
inline T Utils::findChild(QObject* a_parentObj, const QString &a_objectName, T &a_foundObjectOUT)
{
    return a_foundObjectOUT = findChild<T>(a_parentObj, a_objectName);
}


#endif // UTILS_H


