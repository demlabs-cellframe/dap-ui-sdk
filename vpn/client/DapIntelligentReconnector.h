#ifndef DAPINTELLIGENTRECONNECTOR_H
#define DAPINTELLIGENTRECONNECTOR_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QJsonObject>
#include <memory>

// Forward declarations
class DapServerSelector;
class DapAdaptiveKeepAlive;
class DapNetworkQualityMonitor;

enum class ConnectionError {
    NetworkUnreachable,
    ServerUnavailable,
    AuthenticationFailed,
    Timeout,
    ProtocolError,
    UnknownError
};

enum class NetworkQuality {
    Excellent,  // <50ms latency, <1% packet loss
    Good,       // 50-150ms latency, 1-3% packet loss
    Fair,       // 150-300ms latency, 3-5% packet loss
    Poor        // >300ms latency, >5% packet loss
};

struct NetworkConditions {
    NetworkQuality quality;
    uint32_t latency;
    double packetLoss;
    uint32_t bandwidth;
    bool isStable;
};

struct ConnectionAttempt {
    QDateTime timestamp;
    QString serverId;
    ConnectionError error;
    uint32_t duration;
    bool successful;
};

struct ReconnectionConfig {
    uint32_t maxAttempts = 10;           // Increased from 3
    uint32_t baseDelay = 1000;           // 1 second base delay
    uint32_t maxDelay = 60000;           // 60 seconds max delay
    double backoffMultiplier = 1.5;      // Exponential backoff factor
    uint32_t jitterPercent = 20;         // Add random jitter
    bool enableAdaptiveRetry = true;     // Adapt based on network conditions
    bool enableIntelligentServerSelection = true;
    bool enablePredictiveReconnection = false;
};

class DapIntelligentReconnector : public QObject
{
    Q_OBJECT

public:
    explicit DapIntelligentReconnector(QObject *parent = nullptr);
    ~DapIntelligentReconnector();

    // Configuration
    void setConfig(const ReconnectionConfig& config);
    ReconnectionConfig getConfig() const { return m_config; }

    // Connection management
    void startReconnection(const QString& lastServerId, ConnectionError error);
    void startReconnection(NetworkQuality networkQuality);
    void handleNetworkError(NetworkQuality networkQuality);
    void stopReconnection();
    void onConnectionSuccess();
    void onConnectionFailure(ConnectionError error);
    
    // State queries
    bool isReconnecting() const { return m_isReconnecting; }
    uint32_t getCurrentAttempt() const { return m_currentAttempt; }
    uint32_t getNextRetryDelay() const;
    QString getCurrentStatus() const;
    
    // Statistics and metrics
    QJsonObject getStatistics() const;
    QList<ConnectionAttempt> getRecentAttempts() const;
    void clearStatistics();

    // Network adaptation
    void updateNetworkConditions(const NetworkConditions& conditions);
    void onNetworkChange();

signals:
    void retryAttempt(uint32_t attemptNumber, const QString& serverId);
    void reconnectionSuccess();
    void reconnectionFailed(const QString& reason);
    void statusChanged(const QString& status);
    void nextAttemptScheduled(uint32_t delayMs);
    void serverSwitchRecommended(const QString& address, quint16 port);

public slots:
    void onRetryTimerTimeout();
    void onNetworkQualityChanged(NetworkQuality quality);

private:
    // Core logic
    uint32_t calculateBackoffDelay(uint32_t attemptNumber) const;
    uint32_t addJitter(uint32_t delay) const;
    bool shouldRetry(ConnectionError error) const;
    bool shouldSwitchServer() const;
    void adaptRetryStrategy();
    void scheduleNextAttempt();
    void updateStatistics(bool success, ConnectionError error = ConnectionError::UnknownError);
    
    // Strategy selection
    QString selectRetryStrategy() const;
    void applyNetworkBasedStrategy();
    
    // Logging and debugging
    void logAttempt(uint32_t attempt, const QString& serverId, const QString& reason);
    void logSuccess(uint32_t totalAttempts, uint32_t totalTime);
    void logFailure(const QString& reason);

private:
    ReconnectionConfig m_config;
    
    // State management
    bool m_isReconnecting;
    uint32_t m_currentAttempt;
    uint32_t m_consecutiveFailures;
    QDateTime m_reconnectionStartTime;
    QString m_currentServerId;
    ConnectionError m_lastError;
    
    // Timing
    QTimer* m_retryTimer;
    QDateTime m_lastAttemptTime;
    
    // Network awareness
    NetworkConditions m_currentNetworkConditions;
    bool m_networkConditionsKnown;
    
    // Statistics
    QList<ConnectionAttempt> m_attemptHistory;
    uint32_t m_totalReconnections;
    uint32_t m_successfulReconnections;
    QDateTime m_lastSuccessTime;
    
    // Components
    std::unique_ptr<DapServerSelector> m_serverSelector;
    std::unique_ptr<DapAdaptiveKeepAlive> m_keepAlive;
    std::unique_ptr<DapNetworkQualityMonitor> m_networkMonitor;
    
    // Constants
    static constexpr uint32_t MAX_ATTEMPT_HISTORY = 100;
    static constexpr uint32_t NETWORK_STABILITY_WINDOW_MS = 30000; // 30 seconds
    static constexpr uint32_t MAX_CONSECUTIVE_FAILURES_BEFORE_SERVER_SWITCH = 5;
};

// Utility functions
QString connectionErrorToString(ConnectionError error);
ConnectionError stringToConnectionError(const QString& str);
QString networkQualityToString(NetworkQuality quality);
NetworkQuality stringToNetworkQuality(const QString& str);

#endif // DAPINTELLIGENTRECONNECTOR_H 