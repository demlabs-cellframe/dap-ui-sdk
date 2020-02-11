#include "Utils.h"

#include <QVector>

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
}
