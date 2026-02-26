#ifndef TUNNELSTATEMANAGER_H
#define TUNNELSTATEMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

/**
 * @brief Manages VPN tunnel state persistence for crash recovery
 *
 * Saves tunnel configuration to a file when the tunnel is activated
 * and removes it when properly deactivated. On service startup, if the state
 * file exists and the PID is dead, it means the previous session crashed
 * and we need to cleanup routes/DNS.
 */
class TunnelStateManager
{
public:
    static TunnelStateManager& instance();

    /**
     * @brief Save current tunnel state to file
     * @param tunDevice Tunnel device name (e.g., utun5)
     * @param vpnGateway VPN gateway IP address
     * @param oldDefaultGw Previous default gateway (to restore)
     * @param upstreamAddr VPN server address (to remove route)
     * @param networkInterface Network interface name (to restore DNS)
     */
    void saveTunnelState(
        const QString& tunDevice,
        const QString& vpnGateway,
        const QString& oldDefaultGw,
        const QString& upstreamAddr,
        const QString& networkInterface
    );

    /**
     * @brief Clear saved tunnel state (call on clean shutdown)
     */
    void clearTunnelState();

    /**
     * @brief Check if there's a crashed tunnel state that needs cleanup
     * @return true if state file exists and PID is not running
     */
    bool hasCrashedState() const;

    /**
     * @brief Cleanup routes and DNS from a crashed session
     */
    void cleanupCrashedState();

    QString stateFilePath() const;

private:
    TunnelStateManager();
    ~TunnelStateManager() = default;

    TunnelStateManager(const TunnelStateManager&) = delete;
    TunnelStateManager& operator=(const TunnelStateManager&) = delete;

    QString m_stateFilePath;
};

#endif // TUNNELSTATEMANAGER_H
