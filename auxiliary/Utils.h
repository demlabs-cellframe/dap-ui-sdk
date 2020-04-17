#ifndef UTILS_H
#define UTILS_H

#include <QColor>
#include <QWidget>
#include <QVariant>

namespace Utils
{
    QColor toColor(const QString &strRGBA);
    ///The function returns the first integer in the string.
    /// 100%=>100 100px=>100 100**=>100 100=>100
    int toIntValue(const QString &a_text);

    template <class T>
    inline T* findObjectChild(QObject* a_object, const QString& a_objectName, T* a_foundWidget = nullptr);

    void setPropertyAndUpdateStyle(QWidget* a_widget, const QString& a_property, const QVariant& a_value = true);
    void setPropertyAndUpdateStyle(QWidget* a_widget, const char*    a_property, const QVariant& a_value = true);
    void updateStyle(QWidget* a_widget);
};

template <class T>
inline T* Utils::findObjectChild(QObject* a_parentObj, const QString &a_objectName, T* a_outFoundObject /*= nullptr*/)
{
    Q_ASSERT_X(a_parentObj,  "Utils::findObjectChild", "parentObj can't be nullptr");

    a_outFoundObject = a_parentObj->findChild<T*>(a_objectName);

    if (!a_outFoundObject)
    {
        QString errorMsg = QString("Can't find \"%1\" object in \"%2\"")
                .arg(a_objectName)
                .arg(a_parentObj->objectName());
        qFatal(errorMsg.toLatin1().data());
    }

    return a_outFoundObject;
}

#endif // UTILS_H


