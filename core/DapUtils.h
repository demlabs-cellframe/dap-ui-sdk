#ifndef DAPUTILS_H
#define DAPUTILS_H

#include <QByteArray>
#include <QIODevice>
#include <QDataStream>
#include <QDebug>

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
        // stream >> value;
        // return value;
        try {
            stream >> value;

            if (stream.status() != QDataStream::Ok) {
                throw std::runtime_error("Failed to read data from QByteArray.");
            }
        } catch (const std::bad_alloc&) {
            qWarning() << "[DapUtils] MEMORY ALLOCATION FAILED while reading from QByteArray.";
            return T();
        } catch (const std::exception& e) {
            qWarning() << "[DapUtils] EXCEPTION OCCURRED:" << e.what();
            return T();
        }
        return value;
    }

    QString shellCmd(const QString& cmd, int waitMsecs = -1);
    void shellCmdNoWait(const QString& cmd);

};

#endif // DAPUTILS_H
