#include "DapNetworkQualityMonitor.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QHostInfo>
#include <algorithm>
#include <numeric>

DapNetworkQualityMonitor::DapNetworkQualityMonitor(QObject *parent)
    : QObject(parent)
    , m_monitorTimer(new QTimer(this))
    , m_pingTimeoutTimer(new QTimer(this))
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentQuality(Quality::Unknown)
    , m_isRapidTesting(false)
    , m_currentEndpointIndex(0)
    , m_averageLatency(0.0)
    , m_currentJitter(0.0)
    , m_packetLossRate(0.0)
    , m_successfulTests(0)
    , m_failedTests(0)
    , m_isStable(true)
{
    // Configure timers
    m_monitorTimer->setSingleShot(false);
    m_pingTimeoutTimer->setSingleShot(true);
    
    connect(m_monitorTimer, &QTimer::timeout, this, &DapNetworkQualityMonitor::performQualityTest);
    connect(m_pingTimeoutTimer, &QTimer::timeout, this, &DapNetworkQualityMonitor::onPingTimeout);
    
    // Initialize with default configuration
    Config defaultConfig;
    setConfig(defaultConfig);
    
    qDebug() << "DapNetworkQualityMonitor initialized";
}

DapNetworkQualityMonitor::~DapNetworkQualityMonitor()
{
    stopMonitoring();
}

void DapNetworkQualityMonitor::setConfig(const Config &config)
{
    m_config = config;
    
    // Validate configuration
    m_config.pingInterval = std::max(5000, m_config.pingInterval);  // At least 5 seconds
    m_config.rapidTestInterval = std::max(1000, m_config.rapidTestInterval);  // At least 1 second
    m_config.pingTimeout = std::max(1000, m_config.pingTimeout);  // At least 1 second
    
    if (m_config.testEndpoints.isEmpty()) {
        // Fallback to default endpoints
        m_config.testEndpoints = QStringList() << "8.8.8.8" << "1.1.1.1" << "208.67.222.222";
    }
    
    qDebug() << "DapNetworkQualityMonitor configuration updated:"
             << "interval:" << m_config.pingInterval
             << "rapid:" << m_config.rapidTestInterval
             << "timeout:" << m_config.pingTimeout
             << "endpoints:" << m_config.testEndpoints.size();
}

void DapNetworkQualityMonitor::startMonitoring()
{
    if (!m_config.monitoringEnabled) {
        qDebug() << "Network quality monitoring is disabled";
        return;
    }
    
    if (m_monitorTimer->isActive()) {
        qDebug() << "Network quality monitoring already active";
        return;
    }
    
    qInfo() << "Starting network quality monitoring, interval:" << m_config.pingInterval << "ms";
    
    // Reset state
    m_currentQuality = Quality::Unknown;
    m_isRapidTesting = false;
    m_currentEndpointIndex = 0;
    
    // Start monitoring
    m_monitorTimer->start(m_config.pingInterval);
    
    // Perform immediate test
    performImmediateAssessment();
}

void DapNetworkQualityMonitor::stopMonitoring()
{
    if (!m_monitorTimer->isActive()) {
        return;
    }
    
    qInfo() << "Stopping network quality monitoring";
    
    m_monitorTimer->stop();
    m_pingTimeoutTimer->stop();
}

void DapNetworkQualityMonitor::performImmediateAssessment()
{
    qDebug() << "Performing immediate network quality assessment";
    performQualityTest();
}

void DapNetworkQualityMonitor::reportNetworkEvent(const QString &event, int severity)
{
    NetworkEvent netEvent;
    netEvent.timestamp = QDateTime::currentDateTime();
    netEvent.event = event;
    netEvent.severity = severity;
    
    m_networkEvents.append(netEvent);
    
    // Keep event history manageable
    while (m_networkEvents.size() > MAX_EVENT_HISTORY) {
        m_networkEvents.removeFirst();
    }
    
    qInfo() << "Network event reported:" << event << "severity:" << severity;
    
    // High severity events trigger rapid testing
    if (severity >= 3) {
        switchToRapidTesting();
    }
    
    // Recalculate quality with new event data
    calculateQuality();
}

DapNetworkQualityMonitor::Statistics DapNetworkQualityMonitor::getStatistics() const
{
    Statistics stats;
    stats.currentQuality = m_currentQuality;
    stats.averageLatency = m_averageLatency;
    stats.currentJitter = m_currentJitter;
    stats.packetLossRate = m_packetLossRate;
    stats.successfulTests = m_successfulTests;
    stats.failedTests = m_failedTests;
    stats.lastSuccessfulTest = m_lastSuccessfulTest;
    stats.lastQualityChange = m_lastQualityChange;
    stats.isStable = m_isStable;
    stats.qualityReason = getQualityReason();
    stats.latencyHistory = m_latencyHistory;
    stats.qualityHistory = m_qualityHistory;
    
    return stats;
}

void DapNetworkQualityMonitor::performQualityTest()
{
    if (m_config.testEndpoints.isEmpty()) {
        qWarning() << "No test endpoints configured";
        return;
    }
    
    // Select endpoint for testing (round-robin)
    QString endpoint = m_config.testEndpoints[m_currentEndpointIndex];
    m_currentEndpointIndex = (m_currentEndpointIndex + 1) % m_config.testEndpoints.size();
    
    qDebug() << "Starting ping test to" << endpoint;
    
    // Start ping timer
    m_pingStartTime.start();
    
    // Create HTTP request for connectivity test
    // Note: In a real implementation, you might want to use ICMP ping or a dedicated ping library
    QUrl url(QString("http://%1").arg(endpoint));
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "DapChainVPN NetworkMonitor");
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, false);
    
    // Start timeout timer
    m_pingTimeoutTimer->start(m_config.pingTimeout);
    
    // Make the request
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &DapNetworkQualityMonitor::onPingReplyFinished);
    
    // Store endpoint for this test
    reply->setProperty("endpoint", endpoint);
}

void DapNetworkQualityMonitor::onPingReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }
    
    m_pingTimeoutTimer->stop();
    
    QString endpoint = reply->property("endpoint").toString();
    double latency = m_pingStartTime.elapsed();
    bool success = (reply->error() == QNetworkReply::NoError || 
                   reply->error() == QNetworkReply::ContentNotFoundError);  // 404 is OK for connectivity test
    
    qDebug() << "Ping test completed to" << endpoint 
             << "success:" << success 
             << "latency:" << latency << "ms"
             << "error:" << reply->errorString();
    
    recordTestResult(success, latency, endpoint);
    emit testCompleted(success, latency, endpoint);
    
    reply->deleteLater();
    
    // Update quality assessment
    calculateQuality();
}

void DapNetworkQualityMonitor::onPingTimeout()
{
    qWarning() << "Ping test timed out";
    
    QString endpoint = m_config.testEndpoints[m_currentEndpointIndex];
    recordTestResult(false, m_config.pingTimeout, endpoint);
    emit testCompleted(false, m_config.pingTimeout, endpoint);
    
    calculateQuality();
}

void DapNetworkQualityMonitor::calculateQuality()
{
    if (m_testResults.size() < 3) {
        // Insufficient data
        if (m_currentQuality != Quality::Unknown) {
            Quality oldQuality = m_currentQuality;
            m_currentQuality = Quality::Unknown;
            emit qualityChanged(m_currentQuality, oldQuality);
        }
        return;
    }
    
    // Calculate metrics from recent results
    updateStabilityMetrics();
    analyzeLatencyPattern();
    
    // Assess quality based on metrics
    Quality newQuality = assessQualityFromMetrics();
    
    if (newQuality != m_currentQuality) {
        Quality oldQuality = m_currentQuality;
        m_currentQuality = newQuality;
        m_lastQualityChange = QDateTime::currentDateTime();
        
        // Add to quality history
        m_qualityHistory.append(newQuality);
        while (m_qualityHistory.size() > MAX_QUALITY_HISTORY) {
            m_qualityHistory.removeFirst();
        }
        
        qInfo() << "Network quality changed from" << static_cast<int>(oldQuality)
                << "to" << static_cast<int>(newQuality)
                << "reason:" << getQualityReason();
        
        emit qualityChanged(newQuality, oldQuality);
        
        if (newQuality < oldQuality) {
            emit qualityDegraded(getQualityReason());
            switchToRapidTesting();
        } else if (newQuality > oldQuality) {
            emit qualityImproved(getQualityReason());
            if (newQuality >= Quality::Good) {
                switchToNormalTesting();
            }
        }
    }
    
    // Check for instability
    if (!m_isStable) {
        emit networkUnstable(QString("Network instability detected: jitter=%1ms, loss=%2%")
                           .arg(m_currentJitter, 0, 'f', 1)
                           .arg(m_packetLossRate * 100, 0, 'f', 1));
    }
    
    // Emit statistics update
    emit statisticsUpdated(getStatistics());
    
    // Cleanup old data
    cleanupOldData();
}

void DapNetworkQualityMonitor::analyzeLatencyPattern()
{
    // Get recent successful results
    QList<double> recentLatencies;
    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-m_config.stabilityWindow / 1000);
    
    for (const auto &result : m_testResults) {
        if (result.timestamp >= cutoff && result.success) {
            recentLatencies.append(result.latency);
        }
    }
    
    if (recentLatencies.isEmpty()) {
        m_averageLatency = 0.0;
        m_currentJitter = 0.0;
        return;
    }
    
    // Calculate average latency
    double sum = std::accumulate(recentLatencies.begin(), recentLatencies.end(), 0.0);
    m_averageLatency = sum / recentLatencies.size();
    
    // Update latency history
    m_latencyHistory.append(m_averageLatency);
    while (m_latencyHistory.size() > MAX_LATENCY_HISTORY) {
        m_latencyHistory.removeFirst();
    }
    
    // Calculate jitter
    m_currentJitter = calculateJitter();
    
    qDebug() << "Latency analysis: avg=" << m_averageLatency 
             << "jitter=" << m_currentJitter 
             << "samples=" << recentLatencies.size();
}

void DapNetworkQualityMonitor::updateStabilityMetrics()
{
    // Calculate packet loss rate from recent results
    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-m_config.stabilityWindow / 1000);
    int totalTests = 0;
    int failedTests = 0;
    
    for (const auto &result : m_testResults) {
        if (result.timestamp >= cutoff) {
            totalTests++;
            if (!result.success) {
                failedTests++;
            }
        }
    }
    
    m_packetLossRate = (totalTests > 0) ? static_cast<double>(failedTests) / totalTests : 0.0;
    
    // Update total counters
    m_successfulTests = totalTests - failedTests;
    m_failedTests = failedTests;
    
    // Update last successful test timestamp
    for (auto it = m_testResults.rbegin(); it != m_testResults.rend(); ++it) {
        if (it->success) {
            m_lastSuccessfulTest = it->timestamp;
            break;
        }
    }
    
    // Assess stability
    m_isStable = isNetworkStable();
    
    qDebug() << "Stability metrics: loss=" << (m_packetLossRate * 100) << "%" 
             << "stable=" << m_isStable 
             << "tests=" << totalTests;
}

void DapNetworkQualityMonitor::recordTestResult(bool success, double latency, const QString &endpoint)
{
    TestResult result;
    result.timestamp = QDateTime::currentDateTime();
    result.success = success;
    result.latency = latency;
    result.endpoint = endpoint;
    
    m_testResults.append(result);
    
    // Keep history manageable
    while (m_testResults.size() > MAX_TEST_HISTORY) {
        m_testResults.removeFirst();
    }
}

QString DapNetworkQualityMonitor::getQualityReason() const
{
    switch (m_currentQuality) {
        case Quality::Excellent:
            return QString("Low latency (%.0fms), stable connection").arg(m_averageLatency);
        case Quality::Good:
            return QString("Moderate latency (%.0fms), mostly stable").arg(m_averageLatency);
        case Quality::Fair:
            return QString("High latency (%.0fms) or some packet loss (%.1f%%)").arg(m_averageLatency).arg(m_packetLossRate * 100);
        case Quality::Poor:
            return QString("Very high latency (%.0fms) or significant packet loss (%.1f%%)").arg(m_averageLatency).arg(m_packetLossRate * 100);
        case Quality::Unknown:
            return "Insufficient data for quality assessment";
        default:
            return "Unknown quality status";
    }
}

void DapNetworkQualityMonitor::switchToRapidTesting()
{
    if (!m_isRapidTesting) {
        m_isRapidTesting = true;
        m_monitorTimer->setInterval(m_config.rapidTestInterval);
        qInfo() << "Switched to rapid testing mode, interval:" << m_config.rapidTestInterval << "ms";
    }
}

void DapNetworkQualityMonitor::switchToNormalTesting()
{
    if (m_isRapidTesting) {
        m_isRapidTesting = false;
        m_monitorTimer->setInterval(m_config.pingInterval);
        qInfo() << "Switched to normal testing mode, interval:" << m_config.pingInterval << "ms";
    }
}

void DapNetworkQualityMonitor::cleanupOldData()
{
    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-300);  // Keep 5 minutes of data
    
    // Cleanup test results
    auto it = std::remove_if(m_testResults.begin(), m_testResults.end(),
                            [cutoff](const TestResult &result) {
                                return result.timestamp < cutoff;
                            });
    m_testResults.erase(it, m_testResults.end());
    
    // Cleanup network events  
    auto eventIt = std::remove_if(m_networkEvents.begin(), m_networkEvents.end(),
                                 [cutoff](const NetworkEvent &event) {
                                     return event.timestamp < cutoff;
                                 });
    m_networkEvents.erase(eventIt, m_networkEvents.end());
}

double DapNetworkQualityMonitor::calculateJitter() const
{
    if (m_latencyHistory.size() < 2) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (int i = 1; i < m_latencyHistory.size(); ++i) {
        sum += std::abs(m_latencyHistory[i] - m_latencyHistory[i-1]);
    }
    
    return sum / (m_latencyHistory.size() - 1);
}

bool DapNetworkQualityMonitor::isNetworkStable() const
{
    // Network is considered stable if:
    // 1. Packet loss rate is low
    // 2. Jitter is reasonable
    // 3. No recent high-severity events
    
    if (m_packetLossRate > 0.20) {  // More than 20% packet loss
        return false;
    }
    
    if (m_currentJitter > m_config.thresholds.fairJitter) {  // High jitter
        return false;
    }
    
    // Check for recent high-severity events
    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-60);  // Last minute
    for (const auto &event : m_networkEvents) {
        if (event.timestamp >= cutoff && event.severity >= 3) {
            return false;
        }
    }
    
    return true;
}

DapNetworkQualityMonitor::Quality DapNetworkQualityMonitor::assessQualityFromMetrics() const
{
    double successRate = 1.0 - m_packetLossRate;
    
    // Excellent quality criteria
    if (m_averageLatency <= m_config.thresholds.excellentLatency &&
        successRate >= m_config.thresholds.excellentSuccessRate &&
        m_currentJitter <= m_config.thresholds.excellentJitter &&
        m_isStable) {
        return Quality::Excellent;
    }
    
    // Good quality criteria
    if (m_averageLatency <= m_config.thresholds.goodLatency &&
        successRate >= m_config.thresholds.goodSuccessRate &&
        m_currentJitter <= m_config.thresholds.goodJitter) {
        return Quality::Good;
    }
    
    // Fair quality criteria
    if (m_averageLatency <= m_config.thresholds.fairLatency &&
        successRate >= m_config.thresholds.fairSuccessRate &&
        m_currentJitter <= m_config.thresholds.fairJitter) {
        return Quality::Fair;
    }
    
    // Everything else is poor quality
    return Quality::Poor;
} 