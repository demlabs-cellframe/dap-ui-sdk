#include "DapServerSelector.h"
#include <QDebug>

DapServerSelector::DapServerSelector(QObject *parent)
    : QObject(parent)
    , m_qualityUpdateTimer(new QTimer(this))
{
    qDebug() << "DapServerSelector initialized";
}

DapServerSelector::~DapServerSelector()
{
    stopPingTests();
}

void DapServerSelector::updateServerList(const DapServerInfoList& servers)
{
    m_serverList = servers;
    qDebug() << "Updated server list with" << servers.size() << "servers";
    
    // Initialize metrics for new servers
    for (const auto& server : servers) {
        QString serverId = QString("%1:%2").arg(server.address()).arg(server.port());
        if (!m_serverMetrics.contains(serverId)) {
            ServerMetrics metrics;
            metrics.serverId = serverId;
            metrics.address = server.address();
            metrics.port = server.port();
            m_serverMetrics[serverId] = metrics;
        }
    }
    
    calculateServerScores();
}

void DapServerSelector::updateServerMetrics(const QString& serverId, const ConnectionResult& result)
{
    if (!m_serverMetrics.contains(serverId)) {
        qWarning() << "Server metrics not found for" << serverId;
        return;
    }
    
    ServerMetrics& metrics = m_serverMetrics[serverId];
    metrics.totalAttempts++;
    
    if (result.successful) {
        metrics.successfulConnections++;
        metrics.lastSuccess = result.timestamp;
        metrics.lastConnectionDuration = result.duration;
        metrics.consecutiveFailures = 0;
    } else {
        metrics.consecutiveFailures++;
        metrics.lastFailure = result.timestamp;
    }
    
    metrics.lastAttempt = result.timestamp;
    metrics.successRate = static_cast<double>(metrics.successfulConnections) / metrics.totalAttempts;
    
    calculateServerScores();
    emit serverMetricsUpdated(serverId);
}

void DapServerSelector::updateServerPing(const QString& serverId, uint32_t ping)
{
    if (!m_serverMetrics.contains(serverId)) {
        qWarning() << "Server metrics not found for" << serverId;
        return;
    }
    
    ServerMetrics& metrics = m_serverMetrics[serverId];
    
    if (metrics.minPing == UINT32_MAX) {
        metrics.minPing = ping;
        metrics.maxPing = ping;
        metrics.averagePing = ping;
    } else {
        metrics.minPing = std::min(metrics.minPing, ping);
        metrics.maxPing = std::max(metrics.maxPing, ping);
        // Simple moving average for now
        metrics.averagePing = (metrics.averagePing + ping) / 2;
    }
    
    calculateServerScores();
}

void DapServerSelector::updateServerLoad(const QString& serverId, uint32_t load)
{
    if (!m_serverMetrics.contains(serverId)) {
        qWarning() << "Server metrics not found for" << serverId;
        return;
    }
    
    m_serverMetrics[serverId].currentLoad = load;
    calculateServerScores();
}

void DapServerSelector::updateServerStatus(const QString& serverId, bool online, bool maintenance)
{
    if (!m_serverMetrics.contains(serverId)) {
        qWarning() << "Server metrics not found for" << serverId;
        return;
    }
    
    ServerMetrics& metrics = m_serverMetrics[serverId];
    bool wasOnline = metrics.isOnline;
    
    metrics.isOnline = online;
    metrics.isMaintenanceMode = maintenance;
    
    if (wasOnline != online) {
        if (online) {
            emit serverBecameAvailable(serverId);
        } else {
            emit serverBecameUnavailable(serverId);
        }
    }
    
    calculateServerScores();
}

void DapServerSelector::setCurrentServer(const QString& address, quint16 port)
{
    QString serverId = QString("%1:%2").arg(address).arg(port);
    qDebug() << "Setting current server to" << serverId;
    
    // This is just tracking, no special action needed for now
}

void DapServerSelector::reportConnectionSuccess(const QString& address, quint16 port)
{
    QString serverId = QString("%1:%2").arg(address).arg(port);
    qDebug() << "Reporting connection success for" << serverId;
    
    ConnectionResult result;
    result.successful = true;
    result.timestamp = QDateTime::currentDateTime();
    result.duration = 0; // Unknown for now
    result.ping = 0; // Unknown for now
    
    updateServerMetrics(serverId, result);
}

void DapServerSelector::reportConnectionFailure(const QString& address, quint16 port, const QString& error)
{
    QString serverId = QString("%1:%2").arg(address).arg(port);
    qDebug() << "Reporting connection failure for" << serverId << ":" << error;
    
    ConnectionResult result;
    result.successful = false;
    result.timestamp = QDateTime::currentDateTime();
    result.errorMessage = error;
    
    updateServerMetrics(serverId, result);
}

DapServerInfo DapServerSelector::selectBestServer() const
{
    return selectBestServer(QStringList());
}

DapServerInfo DapServerSelector::selectBestServer(const QStringList& excludeServers) const
{
    if (m_serverList.isEmpty()) {
        qWarning() << "No servers available for selection";
        return DapServerInfo();
    }
    
    double bestScore = -1.0;
    DapServerInfo bestServer;
    
    for (const auto& server : m_serverList) {
        QString serverId = QString("%1:%2").arg(server.address()).arg(server.port());
        
        if (excludeServers.contains(serverId)) {
            continue;
        }
        
        if (m_serverMetrics.contains(serverId)) {
            const ServerMetrics& metrics = m_serverMetrics[serverId];
            if (!metrics.isOnline || metrics.isMaintenanceMode) {
                continue;
            }
            
            if (metrics.overallScore > bestScore) {
                bestScore = metrics.overallScore;
                bestServer = server;
            }
        } else {
            // If no metrics, consider it as a potential candidate
            if (bestScore < 0) {
                bestServer = server;
                bestScore = 0;
            }
        }
    }
    
    if (bestServer.isValid()) {
        QString serverId = QString("%1:%2").arg(bestServer.address()).arg(bestServer.port());
        qInfo() << "Selected best server:" << serverId << "with score:" << bestScore;
        logServerSelection(serverId, "Best overall score");
    }
    
    return bestServer;
}

DapServerInfoList DapServerSelector::selectTopServers(int count) const
{
    // Simple implementation - just return first 'count' servers for now
    DapServerInfoList result;
    int added = 0;
    
    for (const auto& server : m_serverList) {
        if (added >= count) break;
        
        QString serverId = QString("%1:%2").arg(server.address()).arg(server.port());
        if (m_serverMetrics.contains(serverId)) {
            const ServerMetrics& metrics = m_serverMetrics[serverId];
            if (metrics.isOnline && !metrics.isMaintenanceMode) {
                result.append(server);
                added++;
            }
        } else {
            result.append(server);
            added++;
        }
    }
    
    return result;
}

DapServerInfo DapServerSelector::selectServerByLocation(const QString& country, const QString& city) const
{
    // Simple implementation - just return first available server for now
    for (const auto& server : m_serverList) {
        if (server.location().contains(country, Qt::CaseInsensitive)) {
            if (city.isEmpty() || server.location().contains(city, Qt::CaseInsensitive)) {
                return server;
            }
        }
    }
    
    return DapServerInfo();
}

DapServerInfo DapServerSelector::selectFastestServer() const
{
    if (m_serverList.isEmpty()) {
        return DapServerInfo();
    }
    
    uint32_t bestPing = UINT32_MAX;
    DapServerInfo fastestServer;
    
    for (const auto& server : m_serverList) {
        QString serverId = QString("%1:%2").arg(server.address()).arg(server.port());
        if (m_serverMetrics.contains(serverId)) {
            const ServerMetrics& metrics = m_serverMetrics[serverId];
            if (metrics.isOnline && !metrics.isMaintenanceMode && metrics.averagePing < bestPing) {
                bestPing = metrics.averagePing;
                fastestServer = server;
            }
        }
    }
    
    if (fastestServer.isValid()) {
        logServerSelection(QString("%1:%2").arg(fastestServer.address()).arg(fastestServer.port()), 
                          QString("Fastest ping: %1ms").arg(bestPing));
    }
    
    return fastestServer;
}

DapServerInfo DapServerSelector::selectMostReliableServer() const
{
    if (m_serverList.isEmpty()) {
        return DapServerInfo();
    }
    
    double bestReliability = -1.0;
    DapServerInfo mostReliableServer;
    
    for (const auto& server : m_serverList) {
        QString serverId = QString("%1:%2").arg(server.address()).arg(server.port());
        if (m_serverMetrics.contains(serverId)) {
            const ServerMetrics& metrics = m_serverMetrics[serverId];
            if (metrics.isOnline && !metrics.isMaintenanceMode && 
                metrics.totalAttempts >= MIN_ATTEMPTS_FOR_RELIABLE_SCORE &&
                metrics.successRate > bestReliability) {
                bestReliability = metrics.successRate;
                mostReliableServer = server;
            }
        }
    }
    
    if (mostReliableServer.isValid()) {
        logServerSelection(QString("%1:%2").arg(mostReliableServer.address()).arg(mostReliableServer.port()), 
                          QString("Most reliable: %1%").arg(bestReliability * 100, 0, 'f', 1));
    }
    
    return mostReliableServer;
}

void DapServerSelector::setUserPreferences(const UserPreferences& preferences)
{
    m_userPreferences = preferences;
    calculateServerScores();
}

QList<ServerMetrics> DapServerSelector::getAllServerMetrics() const
{
    return m_serverMetrics.values();
}

ServerMetrics DapServerSelector::getServerMetrics(const QString& serverId) const
{
    return m_serverMetrics.value(serverId);
}

QStringList DapServerSelector::getAvailableCountries() const
{
    QStringList countries;
    for (const auto& server : m_serverList) {
        QString location = server.location();
        if (!location.isEmpty() && !countries.contains(location)) {
            countries.append(location);
        }
    }
    return countries;
}

QStringList DapServerSelector::getServersInCountry(const QString& country) const
{
    QStringList servers;
    for (const auto& server : m_serverList) {
        if (server.location().contains(country, Qt::CaseInsensitive)) {
            servers.append(QString("%1:%2").arg(server.address()).arg(server.port()));
        }
    }
    return servers;
}

QJsonObject DapServerSelector::getStatistics() const
{
    QJsonObject stats;
    stats["total_servers"] = m_serverList.size();
    stats["total_selections"] = static_cast<int>(m_totalSelections);
    
    int onlineServers = 0;
    for (const auto& metrics : m_serverMetrics) {
        if (metrics.isOnline) onlineServers++;
    }
    stats["online_servers"] = onlineServers;
    
    return stats;
}

QJsonObject DapServerSelector::getServerRankings() const
{
    QJsonObject rankings;
    // Simple implementation for now
    return rankings;
}

void DapServerSelector::exportMetrics(const QString& filename) const
{
    qDebug() << "Exporting metrics to" << filename << "(not implemented)";
}

void DapServerSelector::importMetrics(const QString& filename)
{
    qDebug() << "Importing metrics from" << filename << "(not implemented)";
}

void DapServerSelector::setScoreWeights(double pingWeight, double reliabilityWeight, double recencyWeight)
{
    m_pingWeight = pingWeight;
    m_reliabilityWeight = reliabilityWeight;
    m_recencyWeight = recencyWeight;
    
    // Normalize weights
    double total = m_pingWeight + m_reliabilityWeight + m_recencyWeight;
    if (total > 0) {
        m_pingWeight /= total;
        m_reliabilityWeight /= total;
        m_recencyWeight /= total;
    }
    
    calculateServerScores();
}

void DapServerSelector::setUserLocation(double latitude, double longitude)
{
    m_userLatitude = latitude;
    m_userLongitude = longitude;
    m_userLocationSet = true;
    
    calculateServerScores();
}

void DapServerSelector::enableAutomaticQualityUpdates(bool enable, uint32_t intervalMs)
{
    m_automaticQualityUpdates = enable;
    m_qualityUpdateInterval = intervalMs;
    
    if (enable) {
        m_qualityUpdateTimer->start(intervalMs);
    } else {
        m_qualityUpdateTimer->stop();
    }
}

void DapServerSelector::onPeriodicQualityUpdate()
{
    qDebug() << "Performing periodic quality update";
    calculateServerScores();
    emit qualityUpdateCompleted();
}

void DapServerSelector::onServerPingCompleted(const QString& serverId, uint32_t ping, bool success)
{
    if (success) {
        updateServerPing(serverId, ping);
    }
}

void DapServerSelector::calculateServerScores()
{
    for (auto& metrics : m_serverMetrics) {
        calculatePingScore(metrics);
        calculateReliabilityScore(metrics);
        calculateRecencyScore(metrics);
        calculateOverallScore(metrics);
    }
    
    m_lastRankingUpdate = QDateTime::currentDateTime();
    emit serverRankingChanged();
}

void DapServerSelector::calculatePingScore(ServerMetrics& metrics) const
{
    if (metrics.averagePing == 0) {
        metrics.pingScore = 50.0; // Default score for unknown ping
        return;
    }
    
    // Score based on ping: 0-50ms = 100, 50-150ms = 80-50, 150ms+ = 50-0
    if (metrics.averagePing <= 50) {
        metrics.pingScore = 100.0;
    } else if (metrics.averagePing <= 150) {
        metrics.pingScore = 100.0 - ((metrics.averagePing - 50) * 0.5);
    } else {
        metrics.pingScore = std::max(0.0, 50.0 - ((metrics.averagePing - 150) * 0.1));
    }
}

void DapServerSelector::calculateReliabilityScore(ServerMetrics& metrics) const
{
    if (metrics.totalAttempts == 0) {
        metrics.reliabilityScore = 50.0; // Default score for no data
        return;
    }
    
    // Base score on success rate
    metrics.reliabilityScore = metrics.successRate * 100.0;
    
    // Penalize consecutive failures
    if (metrics.consecutiveFailures > 0) {
        metrics.reliabilityScore *= (1.0 - (metrics.consecutiveFailures * 0.1));
        metrics.reliabilityScore = std::max(0.0, metrics.reliabilityScore);
    }
}

void DapServerSelector::calculateRecencyScore(ServerMetrics& metrics) const
{
    QDateTime now = QDateTime::currentDateTime();
    
    if (!metrics.lastAttempt.isValid()) {
        metrics.recencyScore = 0.0;
        return;
    }
    
    // Score based on how recent the last attempt was
    qint64 hoursAgo = metrics.lastAttempt.secsTo(now) / 3600;
    
    if (hoursAgo <= 1) {
        metrics.recencyScore = 100.0;
    } else if (hoursAgo <= 24) {
        metrics.recencyScore = 100.0 - ((hoursAgo - 1) * 4.0); // Decrease 4 points per hour
    } else {
        metrics.recencyScore = std::max(0.0, 8.0 - (hoursAgo - 24) * 0.3); // Slow decrease after 24h
    }
}

void DapServerSelector::calculateOverallScore(ServerMetrics& metrics) const
{
    metrics.overallScore = (metrics.pingScore * m_pingWeight) +
                          (metrics.reliabilityScore * m_reliabilityWeight) +
                          (metrics.recencyScore * m_recencyWeight);
    
    // Apply user preferences
    if (metrics.userPreferred) {
        metrics.overallScore += 10.0; // Bonus for user preferred servers
    }
    
    // Penalize high load
    if (metrics.currentLoad > 80) {
        metrics.overallScore *= 0.8;
    }
    
    // Ensure score is within bounds (C++11 compatible clamp)
    metrics.overallScore = std::max(0.0, std::min(metrics.overallScore, 100.0));
}

void DapServerSelector::calculateDistance(ServerMetrics& metrics) const
{
    // Simple implementation - not calculating actual distance for now
    Q_UNUSED(metrics)
}

QList<QString> DapServerSelector::filterAvailableServers() const
{
    QList<QString> available;
    
    for (const auto& server : m_serverList) {
        QString serverId = QString("%1:%2").arg(server.address()).arg(server.port());
        if (m_serverMetrics.contains(serverId)) {
            const ServerMetrics& metrics = m_serverMetrics[serverId];
            if (metrics.isOnline && !metrics.isMaintenanceMode) {
                available.append(serverId);
            }
        } else {
            // If no metrics, assume available
            available.append(serverId);
        }
    }
    
    return available;
}

QList<QString> DapServerSelector::applyUserPreferences(const QList<QString>& serverIds) const
{
    // Simple implementation - just return the input list for now
    return serverIds;
}

bool DapServerSelector::isServerAcceptable(const ServerMetrics& metrics) const
{
    if (!metrics.isOnline || metrics.isMaintenanceMode) {
        return false;
    }
    
    if (metrics.averagePing > m_userPreferences.maxAcceptablePing) {
        return false;
    }
    
    if (metrics.successRate < m_userPreferences.minAcceptableSuccessRate) {
        return false;
    }
    
    return true;
}

void DapServerSelector::cleanupOldMetrics()
{
    QDateTime cutoff = QDateTime::currentDateTime().addDays(-MAX_METRICS_AGE_DAYS);
    
    for (auto it = m_serverMetrics.begin(); it != m_serverMetrics.end();) {
        if (it->lastAttempt.isValid() && it->lastAttempt < cutoff) {
            it = m_serverMetrics.erase(it);
        } else {
            ++it;
        }
    }
}

void DapServerSelector::initializeDefaultServers()
{
    // Nothing to do - servers come from updateServerList
}

void DapServerSelector::logServerSelection(const QString& serverId, const QString& reason) const
{
    qInfo() << "Server selected:" << serverId << "- Reason:" << reason;
    
    // Update selection count
    const_cast<DapServerSelector*>(this)->m_selectionCount[serverId]++;
    const_cast<DapServerSelector*>(this)->m_totalSelections++;
}

void DapServerSelector::startPingTests()
{
    qDebug() << "Starting ping tests (not implemented)";
}

void DapServerSelector::stopPingTests()
{
    qDebug() << "Stopping ping tests";
}

void DapServerSelector::pingServer(const QString& serverId)
{
    qDebug() << "Pinging server" << serverId << "(not implemented)";
}

// Utility functions
double calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    // Simple implementation - return 0 for now
    Q_UNUSED(lat1)
    Q_UNUSED(lon1)
    Q_UNUSED(lat2)
    Q_UNUSED(lon2)
    return 0.0;
}

QString getCountryFromServerId(const QString& serverId)
{
    // Simple implementation - extract from server ID if possible
    Q_UNUSED(serverId)
    return QString();
}

QJsonObject serverMetricsToJson(const ServerMetrics& metrics)
{
    QJsonObject json;
    json["serverId"] = metrics.serverId;
    json["address"] = metrics.address;
    json["port"] = static_cast<int>(metrics.port);
    json["averagePing"] = static_cast<int>(metrics.averagePing);
    json["successRate"] = metrics.successRate;
    json["overallScore"] = metrics.overallScore;
    return json;
}

ServerMetrics serverMetricsFromJson(const QJsonObject& json)
{
    ServerMetrics metrics;
    metrics.serverId = json["serverId"].toString();
    metrics.address = json["address"].toString();
    metrics.port = static_cast<uint16_t>(json["port"].toInt());
    metrics.averagePing = static_cast<uint32_t>(json["averagePing"].toInt());
    metrics.successRate = json["successRate"].toDouble();
    metrics.overallScore = json["overallScore"].toDouble();
    return metrics;
} 