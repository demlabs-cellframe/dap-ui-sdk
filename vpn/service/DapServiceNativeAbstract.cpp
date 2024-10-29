#include <QtDebug>
#include "DapServiceNativeAbstract.h"

/**
 * @brief DapServiceNativeAbstract::DapServiceNativeAbstract
 */
DapServiceNativeAbstract::DapServiceNativeAbstract(){}

/**
 * @brief DapServiceNativeAbstract::init
 */
void DapServiceNativeAbstract::init()
{
    qInfo() << "DapServiceNativeAbstract :: init()";
    checkInstallation();
}


