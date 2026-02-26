#include "TunnelStateManager.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDebug>
#include <QProcess>
#include <QThread>

#ifdef Q_OS_UNIX
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#endif

static const int MAX_ROUTE_RETRIES = 3;
static const int ROUTE_RETRY_DELAY_MS = 100;

TunnelStateManager& TunnelStateManager::instance()
{
    static TunnelStateManager s_instance;
    return s_instance;
}

TunnelStateManager::TunnelStateManager()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QDir dir(dataDir + "/Demlabs/" + DAP_BRAND);
    if(!dir.exists())
    {
        dir.mkpath(".");
    }
    m_stateFilePath = dir.absoluteFilePath("tunnel_state.json");
}

QString TunnelStateManager::stateFilePath() const
{
    return m_stateFilePath;
}

void TunnelStateManager::saveTunnelState(
    const QString& tunDevice,
    const QString& vpnGateway,
    const QString& oldDefaultGw,
    const QString& upstreamAddr,
    const QString& networkInterface)
{
    QJsonObject state;
    state["version"] = 1;
    state["tunDevice"] = tunDevice;
    state["vpnGateway"] = vpnGateway;
    state["oldDefaultGw"] = oldDefaultGw;
    state["upstreamAddr"] = upstreamAddr;
    state["networkInterface"] = networkInterface;
    state["pid"] = static_cast<qint64>(getpid());

    qInfo() << "[TunnelStateManager] Saving state:"
            << "tunDevice:" << tunDevice
            << "vpnGateway:" << vpnGateway
            << "oldDefaultGw:" << oldDefaultGw
            << "networkInterface:" << networkInterface;

    QFile file(m_stateFilePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        file.write(QJsonDocument(state).toJson(QJsonDocument::Compact));
        file.close();
        qInfo() << "[TunnelStateManager] Saved tunnel state to:" << m_stateFilePath;
    }
    else
    {
        qWarning() << "[TunnelStateManager] Failed to save state:" << file.errorString();
    }
}

void TunnelStateManager::clearTunnelState()
{
    if(QFile::exists(m_stateFilePath))
    {
        if(QFile::remove(m_stateFilePath))
        {
            qInfo() << "[TunnelStateManager] Cleared tunnel state file";
        }
        else
        {
            qWarning() << "[TunnelStateManager] Failed to remove state file";
        }
    }
}

bool TunnelStateManager::hasCrashedState() const
{
    if(!QFile::exists(m_stateFilePath))
    {
        return false;
    }

    QFile file(m_stateFilePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        return true;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(!doc.isObject())
    {
        return true;
    }

    qint64 savedPid = doc.object()["pid"].toVariant().toLongLong();

    if(savedPid > 0)
    {
#ifdef Q_OS_MACOS
        if(::kill(static_cast<pid_t>(savedPid), 0) == 0)
        {
            qDebug() << "[TunnelStateManager] Process" << savedPid << "still running";
            return false;
        }
#endif
    }

    qWarning() << "[TunnelStateManager] Detected crashed state from PID:" << savedPid;
    return true;
}

void TunnelStateManager::cleanupCrashedState()
{
    if(!hasCrashedState())
    {
        return;
    }

    qWarning() << "[TunnelStateManager] ===== CLEANING UP CRASHED TUNNEL STATE =====";

    QFile file(m_stateFilePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "[TunnelStateManager] Cannot read state file";
        clearTunnelState();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(!doc.isObject())
    {
        qWarning() << "[TunnelStateManager] Invalid state file format";
        clearTunnelState();
        return;
    }

    QJsonObject state = doc.object();
    QString tunDevice = state["tunDevice"].toString();
    QString vpnGateway = state["vpnGateway"].toString();
    QString oldDefaultGw = state["oldDefaultGw"].toString();
    QString upstreamAddr = state["upstreamAddr"].toString();
    QString networkInterface = state["networkInterface"].toString();

    qInfo() << "[TunnelStateManager] Restoring from crashed state:"
            << "tunDevice:" << tunDevice
            << "vpnGateway:" << vpnGateway
            << "oldDefaultGw:" << oldDefaultGw
            << "upstreamAddr:" << upstreamAddr
            << "networkInterface:" << networkInterface;

#ifdef Q_OS_MACOS
    // Step 1: Bring down TUN interface
    if(!tunDevice.isEmpty())
    {
        QString cmd = QString("ifconfig %1 down 2>/dev/null").arg(tunDevice);
        qDebug() << "[TunnelStateManager] cmd:" << cmd;
        ::system(cmd.toLatin1().constData());
    }

    // Step 2: Delete VPN-related routes
    if(!vpnGateway.isEmpty())
    {
        QString cmd = QString("route delete default %1 2>/dev/null").arg(vpnGateway);
        qDebug() << "[TunnelStateManager] cmd:" << cmd;
        ::system(cmd.toLatin1().constData());

        if(!tunDevice.isEmpty())
        {
            cmd = QString("route delete default -interface %1 2>/dev/null").arg(tunDevice);
            ::system(cmd.toLatin1().constData());
        }
    }

    if(!vpnGateway.isEmpty())
    {
        QString cmd = QString("route delete -host %1 2>/dev/null").arg(vpnGateway);
        qDebug() << "[TunnelStateManager] cmd:" << cmd;
        ::system(cmd.toLatin1().constData());
    }

    if(!upstreamAddr.isEmpty())
    {
        QString cmd = QString("route delete -host %1 2>/dev/null").arg(upstreamAddr);
        qDebug() << "[TunnelStateManager] cmd:" << cmd;
        ::system(cmd.toLatin1().constData());
    }

    // Step 3: Check current routing state
    QProcess process;
    process.start("bash", QStringList() << "-c"
                  << "netstat -rn -f inet | grep '^default' | head -1 | awk '{print $2}'");
    process.waitForFinished(3000);
    QString currentDefault = QString::fromUtf8(process.readAllStandardOutput()).trimmed();

    qDebug() << "[TunnelStateManager] Current default gateway:" << currentDefault;

    // Step 4: Restore default route
    bool needsRestoration = currentDefault.isEmpty() ||
                           currentDefault == vpnGateway ||
                           currentDefault.startsWith("link#");

    if(needsRestoration)
    {
        qInfo() << "[TunnelStateManager] Restoring default routes...";

        ::system("route delete default 2>/dev/null");

        bool restored = false;

        if(!oldDefaultGw.isEmpty())
        {
            QString cmd = QString("route add default %1").arg(oldDefaultGw);
            qDebug() << "[TunnelStateManager] Restore route cmd:" << cmd;

            for(int retry = 0; retry < MAX_ROUTE_RETRIES; ++retry)
            {
                int result = ::system(cmd.toLatin1().constData());
                if(result == 0)
                {
                    qInfo() << "[TunnelStateManager] Restored default gateway:" << oldDefaultGw;
                    restored = true;
                    break;
                }
                QThread::msleep(ROUTE_RETRY_DELAY_MS);
            }
        }

        // Last resort: auto-detect gateway
        if(!restored)
        {
            qWarning() << "[TunnelStateManager] Could not restore default route, trying auto-detection...";

            process.start("bash", QStringList() << "-c"
                << "networksetup -listallnetworkservices | grep -v '*' | while read svc; do "
                   "networksetup -getinfo \"$svc\" 2>/dev/null | grep 'Router:' | awk '{print $2}' | head -1; "
                   "done | grep -v '^$' | head -1");
            process.waitForFinished(5000);
            QString detectedGw = QString::fromUtf8(process.readAllStandardOutput()).trimmed();

            if(!detectedGw.isEmpty())
            {
                QString cmd = QString("route add default %1").arg(detectedGw);
                qDebug() << "[TunnelStateManager] Auto-detected gateway cmd:" << cmd;
                int result = ::system(cmd.toLatin1().constData());
                if(result == 0)
                {
                    qInfo() << "[TunnelStateManager] Restored auto-detected gateway:" << detectedGw;
                }
            }
        }
    }
    else
    {
        qDebug() << "[TunnelStateManager] Default route already exists:" << currentDefault;
    }

    // Step 5: Restore DNS settings
    qInfo() << "[TunnelStateManager] Restoring DNS settings...";

    if(!networkInterface.isEmpty())
    {
        QString cmd = QString("networksetup -setdnsservers \"%1\" empty").arg(networkInterface);
        qDebug() << "[TunnelStateManager] Restore DNS cmd:" << cmd;
        ::system(cmd.toLatin1().constData());
    }

    ::system("rm -f /etc/resolver/default 2>/dev/null");
    ::system("echo 'remove State:/Network/Service/VPN_DNS/DNS' | scutil 2>/dev/null");
    ::system("dscacheutil -flushcache 2>/dev/null");
    ::system("killall -HUP mDNSResponder 2>/dev/null");

    qInfo() << "[TunnelStateManager] DNS settings restored";

#endif

    clearTunnelState();

    qWarning() << "[TunnelStateManager] ===== CLEANUP COMPLETE =====";
}
