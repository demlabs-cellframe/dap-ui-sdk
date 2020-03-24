#include "Utils.h"

#include <QVector>
#include <QStyle>
#include <QVariant>
#include <QDebug>

namespace Utils
{
    QColor toColor(const QString &strRGBA)
    {
        QString strColor(strRGBA.simplified());
        if (!strColor.isEmpty() && strColor[0] == '#')
            return QColor(strRGBA);

        QVector<int> result;
        QString temp;
        for(auto it(strColor.begin()); it != strColor.end(); ++it)
        {
            if((*it).isDigit() || (*it) == '.')
                temp += *it;
            else if(!(temp.isEmpty() || temp.isNull()))
            {
                result.append(temp.contains(".") ? int(temp.toDouble() * 255) : temp.toInt());
                temp.clear();
            }
        }
        return (result.size() == 4) ? QColor(result.at(0), result.at(1), result.at(2), result.at(3)) : QColor();
    }

    int toIntValue(const QString &a_text)
    {
        QRegExp regString("(\\d+)");
        regString.indexIn(a_text);
        return regString.cap(0).toInt();
    }

    void setPropertyAndUpdateStyle(QWidget *a_widget, const QString &a_property, const QVariant &a_value /*= true*/)
    {
        const char* property = a_property.toStdString().c_str();

        Utils::setPropertyAndUpdateStyle(a_widget, property, a_value);
    }

    void setPropertyAndUpdateStyle(QWidget *a_widget, const char* a_property, const QVariant &a_value /*= true*/)
    {
        if (a_widget->property(a_property) == a_value)
            return;

        a_widget->setProperty(a_property, a_value);

        Utils::updateStyle(a_widget);
    }

    void updateStyle(QWidget *a_widget)
    {
        a_widget->style()->unpolish(a_widget);
        a_widget->style()->polish(a_widget);
    }


}
