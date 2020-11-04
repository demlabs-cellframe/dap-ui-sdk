#ifndef DAPSYSTEM_H
#define DAPSYSTEM_H

#include <QObject>

class DapSystem : public QObject
{
    Q_OBJECT
private:
    DapSystem();
public:
    static QString dirAppFiles(const QString a_brand);
};

#endif // DAPSYSTEM_H
