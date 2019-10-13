#include <QtDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>
#include "DapServiceNativeMacOS.h"

DapServiceNativeMacOS::DapServiceNativeMacOS()
{

}

/**
 * @brief DapServiceNativeMacOS::checkInstallation
 */
void DapServiceNativeMacOS::checkInstallation()
{
    qInfo() << "checkInstallation()";
    if( ::system(QString("netstat -anv | grep LISTEN | grep '127.0.0.1.22143' >/dev/null").toLatin1().constData() ) == 0 ){
        qInfo() << "22143 port is open on 127.0.0.1";
        isNeedInstallOnlyService = false;
    }else{
        qWarning() << "Service is not running";
        isNeedInstallOnlyService = true;
    }

    QFileInfo check_file(QString("/dev/tun5"));
    if(check_file.exists() ){
        qInfo() << "tun driver exists";
        isNeedInstallOnlyTun = false;
    } else {
        qWarning() << "tun driver was not installed";
        isNeedInstallOnlyTun = true;
    }

    if (isNeedInstallOnlyService && isNeedInstallOnlyTun)
        installTunAndService();
    else if (isNeedInstallOnlyService)
        installService();
    else if (isNeedInstallOnlyTun)
        installTun();
    deepTunCheck();
}

/**
 * @brief DapServiceNativeMacOS::restartService
 */
void DapServiceNativeMacOS::restartService()
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


void DapServiceNativeMacOS::installService()
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

void DapServiceNativeMacOS::installTun()
{
    qInfo() << "installOnlyTun()";
    ::system(QString("/usr/bin/osascript -e 'set windowTitle to \"UNSW requires priveledge access to initialize the secure channel\"\n do shell script \"%1\" with administrator privileges'")
             .arg(QString("sudo launchctl unload /Library/LaunchDaemons/net.sf.tuntaposx.tun.plist;"
                          "sudo cp -fr /Applications/%1.app/Contents/Resources/tun.kext /Library/Extensions/;"
                          "sudo cp -f /Applications/%1.app/Contents/Resources/net.sf.tuntaposx.tun.plist /Library/LaunchDaemons/;"
                          "sudo chown root /Library/LaunchDaemons/net.sf.tuntaposx.tun.plist;"
                          "sudo chmod 600 /Library/LaunchDaemons/net.sf.tuntaposx.tun.plist;"
                          "sudo launchctl load -w /Library/LaunchDaemons/net.sf.tuntaposx.tun.plist;"
                          //"sudo kextunload /Library/Extensions/tun.kext;"
                          "sudo kextload /Library/Extensions/tun.kext;"
                          "sudo echo $? > /tmp/tun.code.txt;"
                          )
                  .arg(DAP_BRAND)).toLatin1().constData() );
}

void DapServiceNativeMacOS::installTunAndService()
{
    qInfo() << "installTunAndService()";
    ::system(QString("/usr/bin/osascript -e 'set windowTitle to \"UNSW requires priveledge access to initialize the secure channel\"\n do shell script \"%1\" with administrator privileges'")
             .arg(QString("sudo launchctl stop com.ncoded.DapVpnService;"
                          "sudo launchctl unload -w /Library/LaunchDaemons/com.ncoded.DapVpnService.plist;"
                          "sudo launchctl unload /Library/LaunchDaemons/net.sf.tuntaposx.tun.plist;"
                          "sudo cp -fr /Applications/%1.app/Contents/Resources/tun.kext /Library/Extensions/;"
                          "sudo cp -f /Applications/%1.app/Contents/Resources/net.sf.tuntaposx.tun.plist /Library/LaunchDaemons/;"
                          "sudo cp -f /Applications/%1.app/Contents/Resources/com.ncoded.DapVpnService.plist /Library/LaunchDaemons/;"
                          "sudo chown root /Library/LaunchDaemons/com.ncoded.DapVpnService.plist;"
                          "sudo chmod 600 /Library/LaunchDaemons/com.ncoded.DapVpnService.plist;"
                          "sudo chown root /Library/LaunchDaemons/net.sf.tuntaposx.tun.plist;"
                          "sudo chmod 600 /Library/LaunchDaemons/net.sf.tuntaposx.tun.plist;"
                          "sudo launchctl load -w /Library/LaunchDaemons/net.sf.tuntaposx.tun.plist;"
                          "sudo launchctl load -w /Library/LaunchDaemons/com.ncoded.DapVpnService.plist;"
                          //"sudo kextunload /Library/Extensions/tun.kext;"
                          "sudo kextload /Library/Extensions/tun.kext;"
                          "sudo echo $? > /tmp/tun.code.txt;"
                          "sudo launchctl start com.ncoded.DapVpnService;"
                          )
                  .arg(DAP_BRAND)).toLatin1().constData() );
}

void DapServiceNativeMacOS::deepTunCheck()
{
    // check tun again
    QFileInfo l_check_tun(QString("/dev/tun5"));
    if(!l_check_tun.exists() ){
        int l_retCode = 0;
        QFileInfo l_tun_code_file_info(QString("/tmp/tun.code.txt"));
        if(l_tun_code_file_info.exists() ){
            QFile tun_code_file("/tmp/tun.code.txt");
            tun_code_file.open(QIODevice::ReadOnly);

            QString l_line = tun_code_file.readLine();
            bool l_isOk = true;
            l_retCode = l_line.split(" ")[0].toInt(&l_isOk);
            if (!l_isOk)
                l_retCode = 255;
        }

        if (27 == l_retCode)
            QMessageBox::information(0, "Failed to load tun kext", "You need to allow system to load tun driver. Go to \"System Preferences > Security & Privacy > General\", you will see \"System software from developer xxx was blocked from loading.\" with an Allow button. Push it.");
        else
            QMessageBox::information(0, "Failed to load tun kext","Tun driver not loaded. Unknown error. Application will be closed!");

        QApplication::quit();
    }
    qInfo() << "deepTunCheck() tun driver exists";
}
