#include "DapServiceNativeIOS.h"



#include <QtDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>
#include "DapServiceNativeIOS.h"
#include "DapClientDefinitions.h"
#include "Foundation/Foundation.h"

DapServiceNativeIOS::DapServiceNativeIOS()
{

}

/**
 * @brief DapServiceNativeIOS::checkInstallation
 */
void DapServiceNativeIOS::checkInstallation()
{
    qInfo() << "checkInstallation()";

//    NSURL *url = [NSURL URLWithString:@"http://127.0.0.1:%d", S(SERVICE_LOCAL_PORT)];
//    NSURLRequest *request = [NSURLRequest requestWithURL:url];
//    NSURLSession *session = [NSURLSession sharedSession];
//    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
//        if (!error) {
//            // Service is running
//            qInfo() << "Service is running on 127.0.0.1%d", SERVICE_LOCAL_PORT;
//            isNeedInstallService = false;
//        } else {
//            // Service is not running
//            qWarning() << "Service is not running";
//            isNeedInstallService = true;
//        }
//    }];
//    [task resume];
}

/**
 * @brief DapServiceNativeIOS::restartService
 */
void DapServiceNativeIOS::restartService()
{
    qInfo() << "restartService()";
}

void DapServiceNativeIOS::installService()
{
    qInfo() << "installService()";
}
