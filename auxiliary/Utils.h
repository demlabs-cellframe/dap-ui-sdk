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



    void setPropertyAndUpdateStyle(QWidget* a_widget, const QString& a_property, const QVariant& a_value = true);
    void setPropertyAndUpdateStyle(QWidget* a_widget, const char*    a_property, const QVariant& a_value = true);
    void updateStyle(QWidget* a_widget);
};

#endif // UTILS_H
