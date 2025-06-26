/* Authors:
* Dmitriy A. Gearasimov <gerasimov.dmitriy@demlabs.net>
* Demlabs Ltd   https://demlabs.net
* DAP UI SDK  https://gitlab.demlabs.net/dap/dap-ui-sdk
* Copyright  (c) 2022
* All rights reserved.

This file is part of DAP UI SDK the open source project

   DAP UI SDK is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   DAP SDK is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with any DAP based project.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DapUtils.h"
#include "darwin/DapUtun.h"
#include "DapDNSController.h"

DapUtun::DapUtun()
    : DapTunUnixAbstract()
    , m_dnsController(new DapDNSController(this))
{
    // List of Apple addresses needed to be routed directly through the tunnel
    appleAdditionalRoutes = QStringList({
        "18.203.89.116","23.56.184.20","35.190.80.1","52.12.45.81","52.215.192.131","54.64.248.78",
        "74.125.140.188","74.125.206.189", "91.105.192.100", "104.17.244.204","104.18.28.208",
        "104.18.29.120","104.18.157.26","108.138.217.3","108.138.233.14","108.156.28.57","108.156.32.59",
        "108.177.15.189","136.143.189.136","136.143.190.84","136.143.191.197","142.250.178.10",
        "142.250.179.234","142.250.187.196","142.250.200.37","142.250.200.46","149.154.167.41",
        "157.240.240.17","157.240.240.18","172.67.167.199","172.217.169.34","172.217.169.35",
        "172.217.169.67","172.217.169.78","185.20.209.147","185.64.190.78","195.93.246.49",
        "199.60.103.225","216.239.32.116",
        "224.0.0.0 240.0.0.0",
        "255.255.255.255 255.255.255.255" });

    connect(SigUnixHandler::getInstance(), &SigUnixHandler::sigKill,
            this, &DapUtun::tunDeviceDestroy, Qt::DirectConnection);
}


void DapUtun::executeCommand(const QString &cmd)
{
    qDebug() << "Executing command:" << cmd;

    QProcess process;
    process.start(cmd);

    if (!process.waitForFinished(2000)) {
        qWarning() << "Command failed to execute:" << process.errorString();
        return;
    }

    int ret = process.exitCode();
    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    qDebug() << "Command output:" << output.trimmed();
    if (!errorOutput.isEmpty()) {
        qWarning() << "Command error output:" << errorOutput.trimmed();
    }

    qDebug() << "Command returned exit code:" << ret;
    if (ret != 0) {
        qWarning() << "Command failed with exit code:" << ret;
    }
}

void DapUtun::tunDeviceCreate()
{
    qDebug() << "[DapUtun::tunDeviceCreate]";

    if (m_tunSocket > 0) {
        qInfo() << "Socket already open";
        return;
    }

    // Lambda function to retrieve error messages
    auto getErrorString = []() -> QString {
        int err = errno;
        char errbuf[256];
        ::strerror_r(err, errbuf, sizeof(errbuf));
        return QString("%1 (code %2)").arg(errbuf).arg(err);
    };

    // Initialize the control structure
    struct ctl_info l_ctl_info = {0};

    // Copy utun control name
    if (::strlcpy(l_ctl_info.ctl_name, UTUN_CONTROL_NAME, sizeof(l_ctl_info.ctl_name))
        >= sizeof(l_ctl_info.ctl_name)) {
        emit error(QString("UTUN_CONTROL_NAME %1 is too long").arg(UTUN_CONTROL_NAME));
        return;
    }

    // Create utun socket
    int l_tun_fd = ::socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (l_tun_fd < 0) {
        emit error(QString("Opening utun device control (SYSPROTO_CONTROL) error: %1")
                       .arg(getErrorString()));
        return;
    }
    qInfo() << "Utun SYSPROTO_CONTROL descriptor obtained";

    // Retrieve control ID via ioctl
    if (::ioctl(l_tun_fd, CTLIOCGINFO, &l_ctl_info) < 0) {
        ::close(l_tun_fd);
        emit error(QString("Can't execute ioctl(CTLIOCGINFO): %1").arg(getErrorString()));
        return;
    }
    qInfo() << "Utun CTLIOCGINFO structure passed through ioctl";

    // Trying to connect with one of utunX devices
    int l_ret = -1;
    for (int l_unit = 0; l_unit < 256; l_unit++) {
        struct sockaddr_ctl l_sa_ctl = {0};
        l_sa_ctl.sc_id = l_ctl_info.ctl_id;
        l_sa_ctl.sc_len = sizeof(l_sa_ctl);
        l_sa_ctl.sc_family = AF_SYSTEM;
        l_sa_ctl.ss_sysaddr = AF_SYS_CONTROL;
        l_sa_ctl.sc_unit = l_unit + 1;

        l_ret = ::connect(l_tun_fd, reinterpret_cast<struct sockaddr*>(&l_sa_ctl), sizeof(l_sa_ctl));
        if (l_ret == 0)
            break;
    }

    if (l_ret < 0) {
        ::close(l_tun_fd);
        emit error(QString("Can't create utun device: %1").arg(getErrorString()));
        return;
    }
    // Get iface name of newly created utun dev.
    qInfo() << "Utun device created";

    // Retrieve the name of the utun network interface
    char l_utunname[20] = {0};
    socklen_t l_utunname_len = sizeof(l_utunname);
    if (::getsockopt(l_tun_fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, l_utunname, &l_utunname_len)) {
        ::close(l_tun_fd);
        emit error(QString("Can't get utun device name: %1").arg(getErrorString()));
        return;
    }

    
    /* this is the special file descriptor that the caller will use to talk
    * with the virtual interface */
    m_tunDeviceName = QString::fromLatin1(l_utunname, l_utunname_len - 1);
    qInfo() << "Created utun" << m_tunDeviceName << "network interface";

    m_tunSocket = l_tun_fd;
}

QString DapUtun::getInternetInterface()
{
    return DapUtils::shellCmd("route get 8.8.8.8 | grep interface | awk '{print $2;}'");
}
void DapUtun::saveCurrentConnectionInterfaceData() {
    m_lastUsedConnectionDevice = getInternetInterface();
    qDebug() << "Current internet interface:" << m_lastUsedConnectionDevice;

    QString cmdList = QString("networksetup -listnetworkserviceorder | grep 'Hardware Port' | grep %1")
                          .arg(m_lastUsedConnectionDevice);
    QString result = DapUtils::shellCmd(cmdList);

    QStringList parts = result.split(":");
    if (parts.length() < 3) {
        qWarning() << "Unable to retrieve connection interface name! Command output:" << result;
        return;
    }

    QStringList interfaceParts = parts[1].split(",");
    if (interfaceParts.length() < 2) {
        qWarning() << "Unexpected format from networksetup command! Command output:" << result;
        return;
    }

    m_lastUsedConnectionName = interfaceParts[0].trimmed();
    qDebug() << "Current internet connection name:" << m_lastUsedConnectionName;

    QString cmdGetInfo = QString("networksetup -getinfo \"%1\" | grep Router")
                             .arg(m_lastUsedConnectionName);
    result = DapUtils::shellCmd(cmdGetInfo);

    QStringList infoLines = result.split("\n", Qt::SkipEmptyParts);
    if (infoLines.isEmpty()) {
        qWarning() << "No default router found.";
    } else {
        QStringList routerParts = infoLines.first().split(":");
        if (routerParts.length() != 2) {
            qWarning() << "Unexpected default router line format:" << infoLines.first();
        } else {
            QString routerValue = routerParts[1].trimmed();
            m_defaultGwOld = (routerValue == "none") ? QString() : routerValue;
        }
    }

    qInfo() << "Device Name:" << m_lastUsedConnectionDevice
            << "Interface Name:" << m_lastUsedConnectionName
            << "Router:" << m_defaultGwOld;
}

QStringList DapUtun::getNetworkServices() {
    QProcess process;
    process.start("networksetup", QStringList() << "-listallnetworkservices");

    if (!process.waitForFinished(5000)) {
        qWarning() << "Failed to get list of network services:" << process.errorString();
        return QStringList();
    }

    QStringList lines = QString(process.readAllStandardOutput()).split("\n", Qt::SkipEmptyParts);

    if (lines.size() < 2) {
        qWarning() << "No valid network services found.";
        return QStringList();
    }

    return lines.mid(1);
}

QString DapUtun::getDNSServers(const QString &service) {
    QProcess process;
    process.start("networksetup", QStringList() << "-getdnsservers" << service);
    if (!process.waitForFinished(5000)) {
        qWarning() << "Timeout while getting DNS for service" << service << ":" << process.errorString();
        return QString();
    }

    return process.readAllStandardOutput().trimmed();
}

bool DapUtun::setDNS(const QString &service, const QString &dnsServer) {
    QProcess process;
    QStringList arguments;

    if (dnsServer.isEmpty()) {
        arguments << "-setdnsservers" << service << "empty";
    } else {
        arguments << "-setdnsservers" << service << dnsServer;
    }

    process.start("networksetup", arguments);
    if (!process.waitForFinished(5000)) {
        qWarning() << "Timeout while configuring DNS for service" << service << ":" << process.errorString();
        return false;
    }

    if (process.exitCode() != 0) {
        qWarning() << "Command failed for service" << service << "with exit code:" << process.exitCode();
        qWarning() << "Error output: " << process.readAllStandardError();
        return false;
    }

    return true;
}

//Not used because breaks other interfaces. At the moment, only the utun interface is being modified.
void DapUtun::configureDNS(const QMap<QString, QString> &dnsMap) {
    QStringList services = getNetworkServices();
    if (services.isEmpty()) {
        qWarning() << "No network services detected.";
        return;
    }

    for (const QString &service : services) {
        QString dnsServer = dnsMap.value(service, "");
        if (setDNS(service, dnsServer)) {
            qDebug() << "DNS servers successfully " << (dnsServer.isEmpty() ? "cleared" : "configured") << " for service:" << service;
        } else {
            qWarning() << "Failed to " << (dnsServer.isEmpty() ? "clear" : "configure") << " DNS servers for service:" << service;
        }
    }
}

void DapUtun::clearAllDNS() {
    configureDNS(QMap<QString, QString>());
}

void DapUtun::onWorkerStarted()
{
    qDebug() << "Starting tunnel creation";

    if (m_tunSocket <= 0) {
        qCritical() << "Failed to bring up network interface: invalid tunnel socket";
        return;
    }

    saveCurrentConnectionInterfaceData();

    m_currentInterface = getCurrentNetworkInterface();
    if (m_currentInterface.isEmpty()) {
        qCritical() << "No active network interface found!";
        return;
    }

    if (!isLocalAddress(upstreamAddress())) {
        QString run = QString("route add -host %2 %1")
                          .arg(m_defaultGwOld)
                          .arg(upstreamAddress());
        executeCommand(run);
    }

    for (const auto &address : m_routingExceptionAddrs) {
        QString run = QString("route add -host %2 %1")
                          .arg(m_defaultGwOld)
                          .arg(address);
        executeCommand(run);
    }

    // QString cmdConnAdd = QString(
    //                          "networksetup -createnetworkservice %1 %2 ;"
    //                          "networksetup -setnetworkserviceenabled %1 on;"
    //                          "networksetup -setmanual %1 %3 255.255.255.255 %4;"
    //                          ).arg(DAP_BRAND)
    //                          .arg(tunDeviceName())
    //                          .arg(addr())
    //                          .arg(gw());

    // qDebug() << "Network service creation command:" << cmdConnAdd;
    // DapUtils::shellCmd(cmdConnAdd);

    QString cmdSetDNS = QString("sudo networksetup -setdnsservers %1 %2")
                            .arg(m_currentInterface)
                            .arg(gw());

    qDebug() << "Setting DNS for" << m_currentInterface << "to:" << gw();
    DapUtils::shellCmd(cmdSetDNS);
    
    // Start DNS monitoring to prevent manual changes
    qInfo() << "[DapUtun] Starting DNS monitoring after setting VPN DNS";
    m_dnsController->setExpectedDNSServers(QStringList{gw()});
    m_dnsController->setVPNConnectionState(true);
    m_dnsController->startDNSMonitoring(3000); // Check every 3 seconds

    QString ifconfigCmd = QString("ifconfig %1 %2 %3")
                              .arg(tunDeviceName())
                              .arg(addr())
                              .arg(gw());
    executeCommand(ifconfigCmd);
    qDebug() << "Configured interface" << tunDeviceName() << "with IP" << addr() << "and gateway" << gw();

    executeCommand("route delete default");
    qDebug() << "Removed default route:" << m_defaultGwOld;

    QString gateway = gw();
    qDebug() << "Gateway obtained: " << gateway;
    if (!gateway.isEmpty()) {
        executeCommand(QString("route add default %1").arg(gateway));
        qDebug() << "Added default route:" << gateway;

        executeCommand(QString("route add -net 224.0.0.0/4 %1").arg(gateway));
        qDebug() << "Added multicast route via gateway:" << gateway;

        executeCommand(QString("route add -net 255.255.255.255/32 %1").arg(gateway));
        qDebug() << "Added broadcast route via gateway:" << gateway;
    } else {
        qWarning() << "Gateway is empty, cannot set up default routes";
    }

    QString cmdAddAdditionalRoutes = QString("networksetup -setadditionalroutes %1").arg(DAP_BRAND);
    foreach (const QString &additionalRoute, appleAdditionalRoutes) {
        cmdAddAdditionalRoutes += " " + additionalRoute;
    }
    qDebug() << "Additional Apple routes command:" << cmdAddAdditionalRoutes;
    DapUtils::shellCmd(cmdAddAdditionalRoutes);

    m_isCreated = true;
    emit created();
}

QString DapUtun::getCurrentNetworkInterface()
{
    qDebug() << "Fetching current active network interface...";

    QProcess routeProcess;
    routeProcess.start("route get default");
    routeProcess.waitForFinished();
    QString routeOutput = routeProcess.readAllStandardOutput();

    qDebug() << "Raw route get default output:" << routeOutput;

    QString activeInterface;
    QStringList routeLines = routeOutput.split("\n");
    for (const QString &line : routeLines) {
        if (line.contains("interface:")) {
            activeInterface = line.split(":").last().trimmed();
            qDebug() << "Detected active interface:" << activeInterface;
            break;
        }
    }

    if (activeInterface.isEmpty()) {
        qWarning() << "No active internet interface found!";
        return QString();
    }

    QProcess serviceOrderProcess;
    serviceOrderProcess.start("networksetup -listnetworkserviceorder");
    serviceOrderProcess.waitForFinished();
    QString serviceOrderOutput = serviceOrderProcess.readAllStandardOutput();

    qDebug() << "Raw network service order output:" << serviceOrderOutput;

    QString regexPattern = R"(\(Hardware Port: (.+?), Device: )" + QRegularExpression::escape(activeInterface) + R"(\))";
    QRegularExpression regex(regexPattern);
    QRegularExpressionMatch match = regex.match(serviceOrderOutput);

    if (match.hasMatch()) {
        QString matchedService = match.captured(1).trimmed();
        qDebug() << "Matched active network service via service order:" << matchedService;
        return matchedService;
    }

    qWarning() << "Could not match active interface to a network service!";
    return QString();
}

void DapUtun::tunDeviceDestroy()
{
    // Stop DNS monitoring and restore original DNS settings
    qInfo() << "[DapUtun] Stopping DNS monitoring and restoring original DNS";
    m_dnsController->setVPNConnectionState(false);
    m_dnsController->stopDNSMonitoring();
    
    if (!m_dnsController->restoreDefaultDNS()) {
        qWarning() << "[DapUtun] Failed to restore original DNS settings via DapDNSController";
    }

    DapTunUnixAbstract::tunDeviceDestroy();

    if (!isLocalAddress(upstreamAddress())){
        QString cmdDeleteUpstream = QString("route delete -host %1")
                                        .arg(upstreamAddress());
        executeCommand(cmdDeleteUpstream);
    }

    for (const auto &route : m_routingExceptionAddrs) {
        QString cmdDeleteException = QString("route delete -host %1")
                                         .arg(route);
        executeCommand(cmdDeleteException);
    }

    // Other routes connected with tunnel should be destroyed autimaticly

    // Restore default gateway
    QString cmdRestoreDefault = QString("route add default %1")
                                    .arg(m_defaultGwOld);
    executeCommand(cmdRestoreDefault);
    qInfo() << "Restored default route:" << m_defaultGwOld;

    qInfo() << "DNS restoration completed via DapDNSController";
}
