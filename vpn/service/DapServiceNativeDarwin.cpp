#include <QtDebug>
#include <QFileInfo>
#include "DapServiceNativeDarwin.h"
#include "DapClientDefinitions.h"

DapServiceNativeDarwin::DapServiceNativeDarwin()
{

}

/**
 * @brief DapServiceNativeDarwin::checkInstallation
 */
void DapServiceNativeDarwin::checkInstallation()
{
    qInfo() << "checkInstallation()";
    if( ::system(QString("netstat -anv | grep LISTEN | grep '127.0.0.1." S(SERVICE_LOCAL_PORT) "' >/dev/null").toLatin1().constData() ) == 0 ){
        qInfo() << SERVICE_LOCAL_PORT << " port is open on 127.0.0.1";
        isNeedInstallService = false;
    }else{
        qWarning() << "Service is not running";
        isNeedInstallService = true;
    }

    if (isNeedInstallService )
        installService();
}

/**
 * @brief DapServiceNativeDarwin::restartService
 */
void DapServiceNativeDarwin::restartService()
{
    qInfo() << "restartService()";
    ::system(QString("/usr/bin/osascript -e 'set windowTitle to \"%2 requires priveledge access to initialize the secure channel\"\n do shell script \"%1\" with administrator privileges'")
             .arg(QString(
                          "sudo launchctl stop com.%3"
                          )
                  ).arg(DAP_BRAND).arg(DAP_BRAND"Service").toLatin1().constData() );
    //normally we don't have to launch service manually
    checkInstallation();
}


void DapServiceNativeDarwin::installService()
{
    qInfo() << "installOnlyService()";
    ::system(QString("/usr/bin/osascript -e 'set windowTitle to \"%1 requires priveledge access to initialize the secure channel\"\n do shell script \"%1\" with administrator privileges'")
             .arg(QString("sudo launchctl stop com.%2;"
                          "sudo launchctl unload -w /Library/LaunchDaemons/com.%2.plist;"
                          "sudo rm /Library/LaunchDaemons/com.%2.plist;"
                          "sudo cp -f /Applications/%1.app/Contents/Resources/com.%2.plist /Library/LaunchDaemons/;"
                          "sudo chown root /Library/LaunchDaemons/com.%2.plist;"
                          "sudo chmod 600 /Library/LaunchDaemons/com.%2.plist;"
                          "sudo launchctl load -w /Library/LaunchDaemons/com.%2.plist;"
                          "sudo launchctl start com.%2;"
                          )
                  .arg(DAP_BRAND).arg(DAP_BRAND"Service")).toLatin1().constData() );
}

