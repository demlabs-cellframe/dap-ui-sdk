#ifndef DAPSERVERSELECTOR_H
#define DAPSERVERSELECTOR_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QJsonObject>
#include <QHash>
#include <QQueue>
#include "DapServerInfo.h"

struct ServerMetrics {
    QString serverId;
    QString address;
    uint16_t port;
    
    // Performance metrics
    uint32_t averagePing = 0;
    uint32_t minPing = UINT32_MAX;
    uint32_t maxPing = 0;
    double pingStdDev = 0.0;
    
    // Reliability metrics
    uint32_t successfulConnections = 0;
    uint32_t totalAttempts = 0;
    double successRate = 0.0;
    uint32_t consecutiveFailures = 0;
    
    // Timing information
    QDateTime lastAttempt;
    QDateTime lastSuccess;
    QDateTime lastFailure;
    uint32_t lastConnectionDuration = 0;
    
    // Quality scores
    double pingScore = 0.0;        // 0-100, higher is better
    double reliabilityScore = 0.0; // 0-100, higher is better
    double recencyScore = 0.0;     // 0-100, higher is better (based on recent performance)
    double overallScore = 0.0;     // 0-100, weighted combination
    
    // User preferences
    bool userPreferred = false;
    int userPriority = 0;         // Higher number = higher priority
    
    // Location information
    QString country;
    QString city;
    double latitude = 0.0;
    double longitude = 0.0;
    uint32_t distanceKm = 0;      // Distance from user
    
    // Load information
    uint32_t currentLoad = 0;     // 0-100 percentage
    uint32_t maxCapacity = 0;
    
    // Availability
    bool isOnline = true;
    bool isMaintenanceMode = false;
    QDateTime maintenanceUntil;
};

struct ConnectionResult {
    bool successful;
    uint32_t duration;
    uint32_t ping;
    QString errorMessage;
    QDateTime timestamp;
};

struct UserPreferences {
    QStringList preferredCountries;
    QStringList blockedCountries;
    QStringList preferredServers;
    QStringList blockedServers;
    bool preferLowLatency = true;
    bool preferHighReliability = true;
    bool preferLowLoad = false;
    uint32_t maxAcceptablePing = 300; // ms
    double minAcceptableSuccessRate = 0.8; // 80%
    bool allowMaintenanceServers = false;
};

class DapServerSelector : public QObject
{
    Q_OBJECT

public:
    explicit DapServerSelector(QObject *parent = nullptr);
    ~DapServerSelector();

    // Server management  
    void updateServerList(const DapServerInfoList& servers);
    void updateServerMetrics(const QString& serverId, const ConnectionResult& result);
    void updateServerPing(const QString& serverId, uint32_t ping);
    void updateServerLoad(const QString& serverId, uint32_t load);
    void updateServerStatus(const QString& serverId, bool online, bool maintenance = false);
    void setCurrentServer(const QString& address, quint16 port);
    void reportConnectionSuccess(const QString& address, quint16 port);
    void reportConnectionFailure(const QString& address, quint16 port, const QString& error);
    
    // Selection methods
    DapServerInfo selectBestServer() const;
    DapServerInfo selectBestServer(const QStringList& excludeServers) const;
    DapServerInfoList selectTopServers(int count = 5) const;
    DapServerInfo selectServerByLocation(const QString& country, const QString& city = QString()) const;
    DapServerInfo selectFastestServer() const;
    DapServerInfo selectMostReliableServer() const;
    
    // Preferences
    void setUserPreferences(const UserPreferences& preferences);
    UserPreferences getUserPreferences() const { return m_userPreferences; }
    
    // Information queries
    QList<ServerMetrics> getAllServerMetrics() const;
    ServerMetrics getServerMetrics(const QString& serverId) const;
    QStringList getAvailableCountries() const;
    QStringList getServersInCountry(const QString& country) const;
    
    // Statistics and diagnostics
    QJsonObject getStatistics() const;
    QJsonObject getServerRankings() const;
    void exportMetrics(const QString& filename) const;
    void importMetrics(const QString& filename);
    
    // Configuration
    void setScoreWeights(double pingWeight, double reliabilityWeight, double recencyWeight);
    void setUserLocation(double latitude, double longitude);
    void enableAutomaticQualityUpdates(bool enable, uint32_t intervalMs = 300000); // 5 minutes default

signals:
    void serverMetricsUpdated(const QString& serverId);
    void serverRankingChanged();
    void serverBecameUnavailable(const QString& serverId);
    void serverBecameAvailable(const QString& serverId);
    void qualityUpdateCompleted();

public slots:
    void onPeriodicQualityUpdate();
    void onServerPingCompleted(const QString& serverId, uint32_t ping, bool success);

private:
    // Core logic
    void calculateServerScores();
    void calculatePingScore(ServerMetrics& metrics) const;
    void calculateReliabilityScore(ServerMetrics& metrics) const;
    void calculateRecencyScore(ServerMetrics& metrics) const;
    void calculateOverallScore(ServerMetrics& metrics) const;
    void calculateDistance(ServerMetrics& metrics) const;
    
    // Filtering
    QList<QString> filterAvailableServers() const;
    QList<QString> applyUserPreferences(const QList<QString>& serverIds) const;
    bool isServerAcceptable(const ServerMetrics& metrics) const;
    
    // Utilities
    void cleanupOldMetrics();
    void initializeDefaultServers();
    void logServerSelection(const QString& serverId, const QString& reason) const;
    
    // Ping management
    void startPingTests();
    void stopPingTests();
    void pingServer(const QString& serverId);

private:
    QHash<QString, ServerMetrics> m_serverMetrics;
    DapServerInfoList m_serverList;
    UserPreferences m_userPreferences;
    
    // Scoring weights
    double m_pingWeight = 0.4;       // 40% weight for ping
    double m_reliabilityWeight = 0.4; // 40% weight for reliability
    double m_recencyWeight = 0.2;    // 20% weight for recent performance
    
    // User location for distance calculations
    double m_userLatitude = 0.0;
    double m_userLongitude = 0.0;
    bool m_userLocationSet = false;
    
    // Quality monitoring
    QTimer* m_qualityUpdateTimer;
    bool m_automaticQualityUpdates = false;
    uint32_t m_qualityUpdateInterval = 300000; // 5 minutes
    
    // Performance tracking
    QDateTime m_lastRankingUpdate;
    uint32_t m_totalSelections = 0;
    QHash<QString, uint32_t> m_selectionCount;
    
    // Constants
    static constexpr uint32_t MAX_METRICS_AGE_DAYS = 30;
    static constexpr uint32_t PING_TIMEOUT_MS = 5000;
    static constexpr uint32_t MIN_ATTEMPTS_FOR_RELIABLE_SCORE = 5;
    static constexpr double DISTANCE_WEIGHT_FACTOR = 0.1; // Reduced importance of distance
    static constexpr double RECENCY_DECAY_HOURS = 24; // How quickly old data loses relevance
};

// Utility functions for server analysis
double calculateDistance(double lat1, double lon1, double lat2, double lon2);
QString getCountryFromServerId(const QString& serverId);
QJsonObject serverMetricsToJson(const ServerMetrics& metrics);
ServerMetrics serverMetricsFromJson(const QJsonObject& json);

#endif // DAPSERVERSELECTOR_H 