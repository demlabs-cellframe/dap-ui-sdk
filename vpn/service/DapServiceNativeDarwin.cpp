#include <QtDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>
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
    ::system(QString("/usr/bin/osascript -e 'set windowTitle to \"UltraPad requires priveledge access to initialize the secure channel\"\n do shell script \"%1\" with administrator privileges'")
             .arg(QString(
                          "sudo launchctl stop com.ncoded.UltraPadService"
                          )
                  ).toLatin1().constData() );
    //normally we don't have to launch service manually
    checkInstallation();
}


void DapServiceNativeDarwin::installService()
{
    qInfo() << "installOnlyService()";
    ::system(QString("/usr/bin/osascript -e 'set windowTitle to \"UNSW requires priveledge access to initialize the secure channel\"\n do shell script \"%1\" with administrator privileges'")
             .arg(QString("sudo launchctl stop com.ncoded.DapVpnService;"
                          "sudo launchctl unload -w /Library/LaunchDaemons/com.ncoded.DapVpnService.plist;"
                          "sudo cp -f /Applications/%1.app/Contents/Resources/com.ncoded.DapVpnService.plist /Library/LaunchDaemons/;"
                          "sudo chown root /Library/LaunchDaemons/com.ncoded.DapVpnService.plist;"
                          "sudo chmod 600 /Library/LaunchDaemons/com.ncoded.DapVpnService.plist;"
                          "sudo launchctl load -w /Library/LaunchDaemons/com.ncoded.DapVpnService.plist;"
                          "sudo launchctl start com.ncoded.DapVpnService;"
                          )
                  .arg(DAP_BRAND)).toLatin1().constData() );
}

