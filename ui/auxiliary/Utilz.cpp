#include "Utilz.h"

#include <QVector>
#include <QStyle>
#include <QVariant>
#include <QDebug>
#include <QMainWindow>


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
        QByteArray l_tempStr = a_property.toLatin1();
        const char* l_property = l_tempStr.constData();
        Utils::setPropertyAndUpdateStyle(a_widget, l_property, a_value);
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

    Qt::LayoutDirection toQtLayoutDirection(QBoxLayout::Direction a_direction)
    {
        switch (a_direction) {
            case QBoxLayout::Direction::LeftToRight:
                return Qt::LeftToRight;
            case QBoxLayout::Direction::RightToLeft:
                return Qt::RightToLeft;
            default:
                return Qt::LayoutDirectionAuto;
        }
    }

    QBoxLayout::Direction toQBoxLayoutDirection(Qt::LayoutDirection a_direction)
    {
        switch (a_direction) {
            case Qt::LeftToRight:
                return QBoxLayout::Direction::LeftToRight;
            case Qt::RightToLeft:
                return QBoxLayout::Direction::RightToLeft;
            default:
                return QBoxLayout::Direction::LeftToRight;
        }
    }
}
