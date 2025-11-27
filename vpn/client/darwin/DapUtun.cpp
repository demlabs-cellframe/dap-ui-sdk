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

DapUtun::DapUtun()
    : DapTunUnixAbstract()
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
    process.start("/bin/sh", {"-c", cmd});

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
    qDebug() << "[DapUtun::tunDeviceCreate] - DEPRECATED: Using NetworkExtension instead";
    
    // This method is deprecated in favor of NetworkExtension
    // NetworkExtension handles UTUN creation automatically
    emit error("Direct UTUN creation is deprecated. Use NetworkExtension instead.");
    return;
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
    qDebug() << "DapUtun::onWorkerStarted - DEPRECATED: Using NetworkExtension instead";
    
    // This method is deprecated in favor of NetworkExtension
    // NetworkExtension handles all network configuration automatically
    emit error("Direct network configuration is deprecated. Use NetworkExtension instead.");
    return;
}

QString DapUtun::getCurrentNetworkInterface()
{
    qDebug() << "Fetching current active network interface...";

    QProcess routeProcess;
    routeProcess.start("route", {"get", "default"});
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
    serviceOrderProcess.start("/bin/sh", {"-c", "networksetup -listnetworkserviceorder"});
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
    qDebug() << "DapUtun::tunDeviceDestroy - DEPRECATED: Using NetworkExtension instead";
    
    // This method is deprecated in favor of NetworkExtension
    // NetworkExtension handles cleanup automatically
    DapTunUnixAbstract::tunDeviceDestroy();
    return;
}
