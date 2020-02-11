#ifndef UTILS_H
#define UTILS_H

#include <QColor>

namespace Utils
{
    QColor toColor(const QString &strRGBA);
    ///The function returns the first integer in the string.
    /// 100%=>100 100px=>100 100**=>100 100=>100
    int toIntValue(const QString &a_text);
};

#endif // UTILS_H
