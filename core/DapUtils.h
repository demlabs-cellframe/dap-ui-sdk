#ifndef DAPUTILS_H
#define DAPUTILS_H

#include <QByteArray>
#include <QIODevice>
#include <QDataStream>


namespace DapUtils
{
    template<typename T>
    QByteArray toByteArray(const T& a_value)
    {
        QByteArray arr;
        QDataStream stream(&arr, QIODevice::WriteOnly);
        stream << a_value;
        return arr;
    }

    template<typename T>
    T fromByteArray(QByteArray a_arr)
    {
        T value;
        QDataStream stream(&a_arr, QIODevice::ReadOnly);
        stream >> value;
        return value;
    }
};

#endif // DAPUTILS_H
