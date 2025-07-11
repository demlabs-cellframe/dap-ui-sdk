#include "DapIntelligentReconnector.h"
#include "DapServerSelector.h"
#include "DapAdaptiveKeepAlive.h"
#include "DapNetworkQualityMonitor.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtMath>

DapIntelligentReconnector::DapIntelligentReconnector(QObject *parent)
    : QObject(parent)
    , m_isReconnecting(false)
    , m_currentAttempt(0)
    , m_consecutiveFailures(0)
    , m_lastError(ConnectionError::UnknownError)
    , m_retryTimer(new QTimer(this))
    , m_networkConditionsKnown(false)
    , m_totalReconnections(0)
    , m_successfulReconnections(0)
{
    qDebug() << "[DapIntelligentReconnector] Initializing intelligent reconnection engine";
    
    // Initialize components
    m_serverSelector.reset(new DapServerSelector(this));
    m_keepAlive.reset(new DapAdaptiveKeepAlive(this));
    m_networkMonitor.reset(new DapNetworkQualityMonitor(this));
    
    // Setup timer
    m_retryTimer->setSingleShot(true);
    connect(m_retryTimer, &QTimer::timeout, this, &DapIntelligentReconnector::onRetryTimerTimeout);
    
    // Connect network monitor with type conversion
    connect(m_networkMonitor.get(), &DapNetworkQualityMonitor::qualityChanged,
            this, [this](DapNetworkQualityMonitor::Quality newQuality, DapNetworkQualityMonitor::Quality /*oldQuality*/) {
                // Convert DapNetworkQualityMonitor::Quality to NetworkQuality
                NetworkQuality networkQuality;
                switch (newQuality) {
                    case DapNetworkQualityMonitor::Quality::Excellent:
                        networkQuality = NetworkQuality::Excellent;
                        break;
                    case DapNetworkQualityMonitor::Quality::Good:
                        networkQuality = NetworkQuality::Good;
                        break;
                    case DapNetworkQualityMonitor::Quality::Fair:
                        networkQuality = NetworkQuality::Fair;
                        break;
                    case DapNetworkQualityMonitor::Quality::Poor:
                        networkQuality = NetworkQuality::Poor;
                        break;
                    case DapNetworkQualityMonitor::Quality::Unknown:
                    default:
                        networkQuality = NetworkQuality::Poor; // Default to Poor for Unknown
                        break;
                }
                this->onNetworkQualityChanged(networkQuality);
            });
    
    // Initialize default network conditions
    m_currentNetworkConditions = {
        NetworkQuality::Good,
        100,    // 100ms latency
        1.0,    // 1% packet loss
        1000,   // 1Mbps bandwidth
        true    // stable
    };
    
    qDebug() << "[DapIntelligentReconnector] Initialization complete";
}

DapIntelligentReconnector::~DapIntelligentReconnector()
{
    qDebug() << "[DapIntelligentReconnector] Destroying reconnection engine";
    stopReconnection();
}

void DapIntelligentReconnector::setConfig(const ReconnectionConfig& config)
{
    qDebug() << "[DapIntelligentReconnector] Updating configuration"
             << "maxAttempts:" << config.maxAttempts
             << "baseDelay:" << config.baseDelay
             << "maxDelay:" << config.maxDelay;
    
    m_config = config;
    
    // Validate configuration
    if (m_config.maxAttempts == 0) {
        qWarning() << "[DapIntelligentReconnector] Invalid maxAttempts, setting to default (10)";
        m_config.maxAttempts = 10;
    }
    
    if (m_config.baseDelay == 0) {
        qWarning() << "[DapIntelligentReconnector] Invalid baseDelay, setting to default (1000ms)";
        m_config.baseDelay = 1000;
    }
    
    if (m_config.maxDelay < m_config.baseDelay) {
        qWarning() << "[DapIntelligentReconnector] maxDelay less than baseDelay, adjusting";
        m_config.maxDelay = m_config.baseDelay * 60; // 60x base delay
    }
}

void DapIntelligentReconnector::startReconnection(const QString& lastServerId, ConnectionError error)
{
    qInfo() << "[DapIntelligentReconnector] Starting reconnection process"
            << "lastServer:" << lastServerId
            << "error:" << connectionErrorToString(error);
    
    if (m_isReconnecting) {
        qDebug() << "[DapIntelligentReconnector] Already reconnecting, resetting state";
        stopReconnection();
    }
    
    // Initialize reconnection state
    m_isReconnecting = true;
    m_currentAttempt = 0;
    m_reconnectionStartTime = QDateTime::currentDateTime();
    m_currentServerId = lastServerId;
    m_lastError = error;
    
    // Update statistics
    m_totalReconnections++;
    
    // Clear old attempt history if needed
    while (m_attemptHistory.size() >= MAX_ATTEMPT_HISTORY) {
        m_attemptHistory.removeFirst();
    }
    
    // Start network monitoring if not already active
    if (!m_networkConditionsKnown) {
        m_networkMonitor->startMonitoring();
    }
    
    // Schedule first retry attempt
    scheduleNextAttempt();
    
    emit statusChanged("Starting reconnection process");
}

void DapIntelligentReconnector::stopReconnection()
{
    if (!m_isReconnecting) {
        return;
    }
    
    qInfo() << "[DapIntelligentReconnector] Stopping reconnection process";
    
    m_isReconnecting = false;
    m_retryTimer->stop();
    
    emit statusChanged("Reconnection stopped");
}

void DapIntelligentReconnector::onConnectionSuccess()
{
    if (!m_isReconnecting) {
        return;
    }
    
    uint32_t totalTime = m_reconnectionStartTime.msecsTo(QDateTime::currentDateTime());
    
    qInfo() << "[DapIntelligentReconnector] Reconnection successful"
            << "attempts:" << m_currentAttempt
            << "totalTime:" << totalTime << "ms";
    
    // Update statistics
    m_successfulReconnections++;
    m_lastSuccessTime = QDateTime::currentDateTime();
    m_consecutiveFailures = 0;
    
    updateStatistics(true);
    logSuccess(m_currentAttempt, totalTime);
    
    // Update server metrics
    if (m_config.enableIntelligentServerSelection) {
        ConnectionResult result;
        result.successful = true;
        result.duration = totalTime;
        result.ping = 0;
        result.errorMessage = "";
        result.timestamp = QDateTime::currentDateTime();
        m_serverSelector->updateServerMetrics(m_currentServerId, result);
    }
    
    // Reset state
    m_isReconnecting = false;
    m_retryTimer->stop();
    
    emit reconnectionSuccess();
    emit statusChanged("Connected successfully");
}

void DapIntelligentReconnector::onConnectionFailure(ConnectionError error)
{
    if (!m_isReconnecting) {
        return;
    }
    
    qWarning() << "[DapIntelligentReconnector] Connection attempt failed"
             << "attempt:" << m_currentAttempt
             << "error:" << connectionErrorToString(error);
    
    m_lastError = error;
    m_consecutiveFailures++;
    
    updateStatistics(false, error);
    
    // Check if we should continue retrying
    if (!shouldRetry(error) || m_currentAttempt >= m_config.maxAttempts) {
        QString reason = QString("Max attempts reached (%1) or non-retryable error (%2)")
                        .arg(m_config.maxAttempts)
                        .arg(connectionErrorToString(error));
        
        logFailure(reason);
        
        m_isReconnecting = false;
        m_retryTimer->stop();
        
        emit reconnectionFailed(reason);
        emit statusChanged(QString("Reconnection failed: %1").arg(reason));
        return;
    }
    
    // Check if we should switch servers
    if (shouldSwitchServer()) {
        DapServerInfo newServer = m_serverSelector->selectBestServer();
        if (newServer.isValid()) {
            QString newServerId = QString("%1:%2").arg(newServer.address()).arg(newServer.port());
            if (newServerId != m_currentServerId) {
                qInfo() << "[DapIntelligentReconnector] Switching server from" << m_currentServerId << "to" << newServerId;
                m_currentServerId = newServerId;
                m_consecutiveFailures = 0; // Reset for new server
                emit serverSwitchRecommended(newServer.address(), newServer.port());
            }
        }
    }
    
    // Schedule next attempt
    scheduleNextAttempt();
}

void DapIntelligentReconnector::startReconnection(NetworkQuality networkQuality)
{
    qInfo() << "[DapIntelligentReconnector] Starting reconnection with network quality:" << networkQualityToString(networkQuality);
    
    // Update network conditions
    m_currentNetworkConditions.quality = networkQuality;
    m_networkConditionsKnown = true;
    
    // Convert network quality to general error for existing logic
    ConnectionError error = ConnectionError::NetworkUnreachable;
    
    startReconnection("", error);
}

void DapIntelligentReconnector::handleNetworkError(NetworkQuality networkQuality)
{
    qInfo() << "[DapIntelligentReconnector] Handling network error with quality:" << networkQualityToString(networkQuality);
    
    // Update network conditions
    m_currentNetworkConditions.quality = networkQuality;
    m_networkConditionsKnown = true;
    
    // Convert network quality to connection error
    ConnectionError error = ConnectionError::NetworkUnreachable;
    
    onConnectionFailure(error);
}

void DapIntelligentReconnector::scheduleNextAttempt()
{
    if (!m_isReconnecting) {
        return;
    }
    
    m_currentAttempt++;
    
    // Calculate delay with exponential backoff and jitter
    uint32_t delay = calculateBackoffDelay(m_currentAttempt);
    delay = addJitter(delay);
    
    // Apply network-based adjustments
    if (m_config.enableAdaptiveRetry && m_networkConditionsKnown) {
        adaptRetryStrategy();
        
        // Adjust delay based on network quality
        switch (m_currentNetworkConditions.quality) {
            case NetworkQuality::Poor:
                delay *= 2; // Double delay for poor networks
                break;
            case NetworkQuality::Fair:
                delay = delay * 3 / 2; // 1.5x delay for fair networks
                break;
            case NetworkQuality::Good:
                // Keep calculated delay
                break;
            case NetworkQuality::Excellent:
                delay = delay * 2 / 3; // Reduce delay for excellent networks
                break;
        }
    }
    
    // Ensure delay is within bounds
    delay = qMax(m_config.baseDelay, qMin(delay, m_config.maxDelay));
    
    qInfo() << "[DapIntelligentReconnector] Scheduling attempt" << m_currentAttempt
            << "in" << delay << "ms"
            << "server:" << m_currentServerId;
    
    logAttempt(m_currentAttempt, m_currentServerId, QString("Delay: %1ms").arg(delay));
    
    m_retryTimer->start(delay);
    m_lastAttemptTime = QDateTime::currentDateTime();
    
    emit nextAttemptScheduled(delay);
    emit statusChanged(QString("Retrying in %1 seconds (attempt %2/%3)")
                      .arg(delay / 1000.0, 0, 'f', 1)
                      .arg(m_currentAttempt)
                      .arg(m_config.maxAttempts));
}

void DapIntelligentReconnector::onRetryTimerTimeout()
{
    if (!m_isReconnecting) {
        return;
    }
    
    qInfo() << "[DapIntelligentReconnector] Executing retry attempt" << m_currentAttempt;
    
    emit retryAttempt(m_currentAttempt, m_currentServerId);
    emit statusChanged(QString("Attempting to connect (attempt %1/%2)")
                      .arg(m_currentAttempt)
                      .arg(m_config.maxAttempts));
}

uint32_t DapIntelligentReconnector::calculateBackoffDelay(uint32_t attemptNumber) const
{
    if (attemptNumber == 0) {
        return m_config.baseDelay;
    }
    
    // Calculate exponential backoff: baseDelay * (multiplier ^ (attempt - 1))
    double delay = m_config.baseDelay * qPow(m_config.backoffMultiplier, attemptNumber - 1);
    
    // Ensure we don't overflow
    return static_cast<uint32_t>(qMin(delay, static_cast<double>(m_config.maxDelay)));
}

uint32_t DapIntelligentReconnector::addJitter(uint32_t delay) const
{
    if (m_config.jitterPercent == 0) {
        return delay;
    }
    
    // Add random jitter: ±jitterPercent of the delay
    uint32_t jitterRange = (delay * m_config.jitterPercent) / 100;
    uint32_t jitter = QRandomGenerator::global()->bounded(jitterRange * 2 + 1) - jitterRange;
    
    return qMax(static_cast<uint32_t>(delay + jitter), m_config.baseDelay / 2);
}

bool DapIntelligentReconnector::shouldRetry(ConnectionError error) const
{
    switch (error) {
        case ConnectionError::AuthenticationFailed:
            // Don't retry authentication failures immediately
            return false;
            
        case ConnectionError::NetworkUnreachable:
        case ConnectionError::ServerUnavailable:
        case ConnectionError::Timeout:
        case ConnectionError::ProtocolError:
        case ConnectionError::UnknownError:
            return true;
    }
    
    return true;
}

bool DapIntelligentReconnector::shouldSwitchServer() const
{
    if (!m_config.enableIntelligentServerSelection) {
        return false;
    }
    
    // Switch server after consecutive failures
    return m_consecutiveFailures >= MAX_CONSECUTIVE_FAILURES_BEFORE_SERVER_SWITCH;
}

void DapIntelligentReconnector::adaptRetryStrategy()
{
    // Adapt based on current network conditions
    if (!m_networkConditionsKnown) {
        return;
    }
    
    // For poor network conditions, be more conservative
    if (m_currentNetworkConditions.quality == NetworkQuality::Poor) {
        // Increase max attempts for poor networks
        m_config.maxAttempts = qMax(m_config.maxAttempts, static_cast<uint32_t>(15));
    }
    
    // For unstable networks, use longer delays
    if (!m_currentNetworkConditions.isStable) {
        m_config.backoffMultiplier = qMax(m_config.backoffMultiplier, 2.0);
    }
}

void DapIntelligentReconnector::updateNetworkConditions(const NetworkConditions& conditions)
{
    qDebug() << "[DapIntelligentReconnector] Network conditions updated"
             << "quality:" << networkQualityToString(conditions.quality)
             << "latency:" << conditions.latency
             << "packetLoss:" << conditions.packetLoss
             << "stable:" << conditions.isStable;
    
    m_currentNetworkConditions = conditions;
    m_networkConditionsKnown = true;
    
    if (m_config.enableAdaptiveRetry && m_isReconnecting) {
        adaptRetryStrategy();
    }
}

void DapIntelligentReconnector::onNetworkQualityChanged(NetworkQuality quality)
{
    qDebug() << "[DapIntelligentReconnector] Network quality changed to" << networkQualityToString(quality);
    
    m_currentNetworkConditions.quality = quality;
    
    if (m_config.enableAdaptiveRetry && m_isReconnecting) {
        adaptRetryStrategy();
    }
}

void DapIntelligentReconnector::onNetworkChange()
{
    qInfo() << "[DapIntelligentReconnector] Network change detected";
    
    // Reset network conditions
    m_networkConditionsKnown = false;
    
    // Start monitoring new network
    m_networkMonitor->startMonitoring();
    
    // If we're currently reconnecting, this might be good news
    if (m_isReconnecting && m_retryTimer->isActive()) {
        // Reduce the current retry delay to attempt connection sooner
        int currentRemaining = m_retryTimer->remainingTime();
        int newDelay = qMax(1000, currentRemaining / 3); // Try in 1/3 of remaining time, min 1 second
        
        qInfo() << "[DapIntelligentReconnector] Network changed during reconnection, reducing delay from"
                << currentRemaining << "to" << newDelay;
        
        m_retryTimer->start(newDelay);
    }
}

uint32_t DapIntelligentReconnector::getNextRetryDelay() const
{
    if (!m_isReconnecting || !m_retryTimer->isActive()) {
        return 0;
    }
    
    return m_retryTimer->remainingTime();
}

QString DapIntelligentReconnector::getCurrentStatus() const
{
    if (!m_isReconnecting) {
        return "Not reconnecting";
    }
    
    if (m_retryTimer->isActive()) {
        return QString("Waiting for retry (attempt %1/%2, %3s remaining)")
               .arg(m_currentAttempt)
               .arg(m_config.maxAttempts)
               .arg(m_retryTimer->remainingTime() / 1000);
    }
    
    return QString("Attempting to connect (attempt %1/%2)")
           .arg(m_currentAttempt)
           .arg(m_config.maxAttempts);
}

QJsonObject DapIntelligentReconnector::getStatistics() const
{
    QJsonObject stats;
    
    stats["totalReconnections"] = static_cast<int>(m_totalReconnections);
    stats["successfulReconnections"] = static_cast<int>(m_successfulReconnections);
    stats["successRate"] = m_totalReconnections > 0 ? 
                          static_cast<double>(m_successfulReconnections) / m_totalReconnections : 0.0;
    
    stats["isReconnecting"] = m_isReconnecting;
    stats["currentAttempt"] = static_cast<int>(m_currentAttempt);
    stats["consecutiveFailures"] = static_cast<int>(m_consecutiveFailures);
    
    if (m_lastSuccessTime.isValid()) {
        stats["lastSuccessTime"] = m_lastSuccessTime.toString(Qt::ISODate);
    }
    
    // Network conditions
    QJsonObject networkInfo;
    networkInfo["quality"] = networkQualityToString(m_currentNetworkConditions.quality);
    networkInfo["latency"] = static_cast<int>(m_currentNetworkConditions.latency);
    networkInfo["packetLoss"] = m_currentNetworkConditions.packetLoss;
    networkInfo["bandwidth"] = static_cast<int>(m_currentNetworkConditions.bandwidth);
    networkInfo["stable"] = m_currentNetworkConditions.isStable;
    networkInfo["conditionsKnown"] = m_networkConditionsKnown;
    stats["networkConditions"] = networkInfo;
    
    // Recent attempts
    QJsonArray attemptsArray;
    for (const auto& attempt : m_attemptHistory) {
        QJsonObject attemptObj;
        attemptObj["timestamp"] = attempt.timestamp.toString(Qt::ISODate);
        attemptObj["serverId"] = attempt.serverId;
        attemptObj["error"] = connectionErrorToString(attempt.error);
        attemptObj["duration"] = static_cast<int>(attempt.duration);
        attemptObj["successful"] = attempt.successful;
        attemptsArray.append(attemptObj);
    }
    stats["recentAttempts"] = attemptsArray;
    
    return stats;
}

QList<ConnectionAttempt> DapIntelligentReconnector::getRecentAttempts() const
{
    return m_attemptHistory;
}

void DapIntelligentReconnector::clearStatistics()
{
    qInfo() << "[DapIntelligentReconnector] Clearing statistics";
    
    m_attemptHistory.clear();
    m_totalReconnections = 0;
    m_successfulReconnections = 0;
    m_lastSuccessTime = QDateTime();
}

void DapIntelligentReconnector::updateStatistics(bool success, ConnectionError error)
{
    ConnectionAttempt attempt;
    attempt.timestamp = QDateTime::currentDateTime();
    attempt.serverId = m_currentServerId;
    attempt.error = error;
    attempt.duration = m_lastAttemptTime.msecsTo(attempt.timestamp);
    attempt.successful = success;
    
    m_attemptHistory.append(attempt);
    
    // Keep history size manageable
    while (m_attemptHistory.size() > MAX_ATTEMPT_HISTORY) {
        m_attemptHistory.removeFirst();
    }
}

void DapIntelligentReconnector::logAttempt(uint32_t attempt, const QString& serverId, const QString& reason)
{
    qInfo() << QString("[DapIntelligentReconnector] Attempt %1 to server %2: %3")
               .arg(attempt)
               .arg(serverId)
               .arg(reason);
}

void DapIntelligentReconnector::logSuccess(uint32_t totalAttempts, uint32_t totalTime)
{
    qInfo() << QString("[DapIntelligentReconnector] Reconnection successful after %1 attempts in %2ms")
               .arg(totalAttempts)
               .arg(totalTime);
}

void DapIntelligentReconnector::logFailure(const QString& reason)
{
    qCritical() << QString("[DapIntelligentReconnector] Reconnection failed: %1").arg(reason);
}

// Utility functions
QString connectionErrorToString(ConnectionError error)
{
    switch (error) {
        case ConnectionError::NetworkUnreachable: return "NetworkUnreachable";
        case ConnectionError::ServerUnavailable: return "ServerUnavailable";
        case ConnectionError::AuthenticationFailed: return "AuthenticationFailed";
        case ConnectionError::Timeout: return "Timeout";
        case ConnectionError::ProtocolError: return "ProtocolError";
        case ConnectionError::UnknownError: return "UnknownError";
    }
    return "UnknownError";
}

ConnectionError stringToConnectionError(const QString& str)
{
    if (str == "NetworkUnreachable") return ConnectionError::NetworkUnreachable;
    if (str == "ServerUnavailable") return ConnectionError::ServerUnavailable;
    if (str == "AuthenticationFailed") return ConnectionError::AuthenticationFailed;
    if (str == "Timeout") return ConnectionError::Timeout;
    if (str == "ProtocolError") return ConnectionError::ProtocolError;
    return ConnectionError::UnknownError;
}

QString networkQualityToString(NetworkQuality quality)
{
    switch (quality) {
        case NetworkQuality::Excellent: return "Excellent";
        case NetworkQuality::Good: return "Good";
        case NetworkQuality::Fair: return "Fair";
        case NetworkQuality::Poor: return "Poor";
    }
    return "Unknown";
}

NetworkQuality stringToNetworkQuality(const QString& str)
{
    if (str == "Excellent") return NetworkQuality::Excellent;
    if (str == "Good") return NetworkQuality::Good;
    if (str == "Fair") return NetworkQuality::Fair;
    if (str == "Poor") return NetworkQuality::Poor;
    return NetworkQuality::Good;
} 