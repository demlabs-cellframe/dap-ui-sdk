#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>

/**
 * Singleton that persists tunnel state to tunnel_state.json.
 * Used to detect and recover from crashes where routes/DNS
 * were left modified, leaving the system without connectivity.
 */
class TunnelStateManager : public QObject
{
    Q_OBJECT
public:
    static TunnelStateManager &instance();

    void saveTunnelState(const QString &a_tunDevice,
                         const QString &a_vpnGateway,
                         const QString &a_oldDefaultGw,
                         const QString &a_upstreamAddr,
                         const QString &a_networkInterface);

    void clearTunnelState();

    bool hasCrashedState() const;

    void cleanupCrashedState();
    void resetDnsOnly();

    QJsonObject loadTunnelState() const;

signals:
    void crashRecoveryCompleted();

private:
    explicit TunnelStateManager(QObject *parent = nullptr);
    Q_DISABLE_COPY(TunnelStateManager)

    QString stateFilePath() const;
};
