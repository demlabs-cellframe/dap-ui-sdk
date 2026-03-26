#include "TunnelStateManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QProcess>

#include <unistd.h>
#include <signal.h>
#include <errno.h>

static const char *STATE_FILE_NAME = "tunnel_state.json";

TunnelStateManager::TunnelStateManager(QObject *parent)
    : QObject(parent)
{
}

TunnelStateManager &TunnelStateManager::instance()
{
    static TunnelStateManager s_instance;
    return s_instance;
}

QString TunnelStateManager::stateFilePath() const
{
    QString dir = QString("/var/log/%1").arg(DAP_BRAND).toLower();
    return dir + "/" + STATE_FILE_NAME;
}

void TunnelStateManager::saveTunnelState(const QString &a_tunDevice,
                                          const QString &a_vpnGateway,
                                          const QString &a_oldDefaultGw,
                                          const QString &a_upstreamAddr,
                                          const QString &a_networkInterface)
{
    QJsonObject state;
    state["tunDevice"]         = a_tunDevice;
    state["vpnGateway"]        = a_vpnGateway;
    state["oldDefaultGw"]      = a_oldDefaultGw;
    state["upstreamAddr"]      = a_upstreamAddr;
    state["networkInterface"]  = a_networkInterface;
    state["pid"]               = static_cast<qint64>(QCoreApplication::applicationPid());

    QFile file(stateFilePath());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "[TunnelStateManager] Cannot write state file:" << stateFilePath();
        return;
    }

    file.write(QJsonDocument(state).toJson(QJsonDocument::Compact));
    file.close();
    qInfo() << "[TunnelStateManager] Tunnel state saved";
}

void TunnelStateManager::clearTunnelState()
{
    QFile file(stateFilePath());
    if(file.exists())
    {
        file.remove();
        qInfo() << "[TunnelStateManager] Tunnel state file removed";
    }
}

QJsonObject TunnelStateManager::loadTunnelState() const
{
    QFile file(stateFilePath());
    if(!file.open(QIODevice::ReadOnly))
        return {};

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc.object();
}

bool TunnelStateManager::hasCrashedState() const
{
    QString path = stateFilePath();
    qInfo() << "[TunnelStateManager] Checking for crashed state, file:" << path;

    QFile file(path);
    if(!file.exists())
    {
        qInfo() << "[TunnelStateManager] State file does not exist, no crash";
        return false;
    }

    QJsonObject state = loadTunnelState();
    if(state.isEmpty())
    {
        qWarning() << "[TunnelStateManager] State file exists but is empty/invalid";
        return false;
    }

    qint64 savedPid = state["pid"].toInteger();
    qInfo() << "[TunnelStateManager] State file found, saved PID:" << savedPid
            << "current PID:" << QCoreApplication::applicationPid();

    if(savedPid <= 0)
    {
        qWarning() << "[TunnelStateManager] Invalid PID in state file, treating as crash";
        return true;
    }

    if(kill(static_cast<pid_t>(savedPid), 0) == 0)
    {
        qInfo() << "[TunnelStateManager] PID" << savedPid
                << "is still alive, state is current — no crash";
        return false;
    }

    int err = errno;
    qWarning() << "[TunnelStateManager] Crashed state detected: PID" << savedPid
               << "is dead (kill errno:" << err << ")";
    return true;
}

void TunnelStateManager::cleanupCrashedState()
{
    QJsonObject state = loadTunnelState();
    if(state.isEmpty())
    {
        clearTunnelState();
        return;
    }

    QString tunDevice        = state["tunDevice"].toString();
    QString oldDefaultGw     = state["oldDefaultGw"].toString();
    QString upstreamAddr     = state["upstreamAddr"].toString();
    QString networkInterface = state["networkInterface"].toString();

    qWarning() << "[TunnelStateManager] Cleaning up crashed tunnel state:"
               << "tun=" << tunDevice
               << "gw=" << oldDefaultGw
               << "upstream=" << upstreamAddr
               << "iface=" << networkInterface;

    auto runCmd = [](const QString &cmd) -> QString
    {
        QProcess proc;
        proc.start("sh", QStringList() << "-c" << cmd);
        proc.waitForFinished(5000);
        QString out = proc.readAllStandardOutput().trimmed();
        QString err = proc.readAllStandardError().trimmed();
        if(!err.isEmpty())
            qWarning() << "[TunnelStateManager] cmd error:" << err;
        return out;
    };

    // Bring down orphaned TUN interface
    if(!tunDevice.isEmpty())
    {
        runCmd(QString("ifconfig %1 down 2>/dev/null").arg(tunDevice));
        qInfo() << "[TunnelStateManager] Brought down" << tunDevice;
    }

    // Delete VPN upstream host route
    if(!upstreamAddr.isEmpty())
    {
        runCmd(QString("route delete -host %1 2>/dev/null").arg(upstreamAddr));
    }

    // Delete current default route (VPN gateway)
    runCmd("route delete default 2>/dev/null");

    // Restore original default gateway
    if(!oldDefaultGw.isEmpty())
    {
        runCmd(QString("route add default %1").arg(oldDefaultGw));
        qInfo() << "[TunnelStateManager] Restored default gateway:" << oldDefaultGw;
    }

    // Restore DNS
    if(!networkInterface.isEmpty())
    {
        if(!oldDefaultGw.isEmpty())
        {
            runCmd(QString("networksetup -setdnsservers \"%1\" Empty").arg(networkInterface));
            qInfo() << "[TunnelStateManager] Reset DNS for" << networkInterface;
        }
    }

    // Flush DNS cache
    runCmd("dscacheutil -flushcache");
    runCmd("killall -HUP mDNSResponder 2>/dev/null");

    clearTunnelState();

    qInfo() << "[TunnelStateManager] Crash recovery completed";
    emit crashRecoveryCompleted();
}

void TunnelStateManager::resetDnsOnly()
{
    QJsonObject state = loadTunnelState();
    if(state.isEmpty())
        return;

    QString networkInterface = state["networkInterface"].toString();
    if(networkInterface.isEmpty())
        return;

    qInfo() << "[TunnelStateManager] Resetting DNS only for" << networkInterface;

    auto runCmd = [](const QString &cmd) -> QString
    {
        QProcess proc;
        proc.start("sh", QStringList() << "-c" << cmd);
        proc.waitForFinished(5000);
        QString err = proc.readAllStandardError().trimmed();
        if(!err.isEmpty())
            qWarning() << "[TunnelStateManager] cmd error:" << err;
        return proc.readAllStandardOutput().trimmed();
    };

    runCmd(QString("networksetup -setdnsservers \"%1\" Empty").arg(networkInterface));
    runCmd("dscacheutil -flushcache");
    runCmd("killall -HUP mDNSResponder 2>/dev/null");

    qInfo() << "[TunnelStateManager] DNS reset completed, routes unchanged";
}

