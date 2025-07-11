#include "DapAdaptiveKeepAlive.h"
#include <QDebug>
#include <algorithm>

DapAdaptiveKeepAlive::DapAdaptiveKeepAlive(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_currentQuality(NetworkQuality::Good)
    , m_currentInterval(6000)  // Default 6 seconds
    , m_missedResponses(0)
    , m_successfulResponses(0)
    , m_averageResponseTime(0)
    , m_adaptationEnabled(true)
    , m_qualityDegradationCount(0)
{
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &DapAdaptiveKeepAlive::onTimerTimeout);
    
    // Initialize with default configuration
    Config defaultConfig;
    setConfig(defaultConfig);
    
    qDebug() << "DapAdaptiveKeepAlive initialized with interval:" << m_currentInterval << "ms";
}

DapAdaptiveKeepAlive::~DapAdaptiveKeepAlive()
{
    stop();
}

void DapAdaptiveKeepAlive::setConfig(const Config &config)
{
    m_config = config;
    m_adaptationEnabled = config.enableAdaptation;
    
    // Validate configuration
    m_config.minInterval = std::max(1000, m_config.minInterval);  // At least 1 second
    m_config.maxInterval = std::max(m_config.minInterval, m_config.maxInterval);
    m_config.baseInterval = std::max(m_config.minInterval, std::min(m_config.baseInterval, m_config.maxInterval));
    
    qDebug() << "DapAdaptiveKeepAlive configuration updated:"
             << "min:" << m_config.minInterval
             << "max:" << m_config.maxInterval  
             << "base:" << m_config.baseInterval
             << "adaptation:" << m_adaptationEnabled;
             
    // Update current interval if changed
    updateInterval();
}

void DapAdaptiveKeepAlive::start()
{
    if (m_timer->isActive()) {
        qDebug() << "DapAdaptiveKeepAlive already active";
        return;
    }
    
    qInfo() << "Starting adaptive keep-alive with interval:" << m_currentInterval << "ms";
    m_timer->start(m_currentInterval);
    
    // Reset state for new session
    m_lastKeepAliveSent = QDateTime::currentDateTime();
}

void DapAdaptiveKeepAlive::stop()
{
    if (!m_timer->isActive()) {
        return;
    }
    
    qInfo() << "Stopping adaptive keep-alive";
    m_timer->stop();
}

void DapAdaptiveKeepAlive::updateNetworkQuality(NetworkQuality quality)
{
    if (m_currentQuality == quality) {
        return;
    }
    
    NetworkQuality oldQuality = m_currentQuality;
    m_currentQuality = quality;
    
    qInfo() << "Network quality changed from" << static_cast<int>(oldQuality) 
            << "to" << static_cast<int>(quality);
    
    emit qualityChanged(quality);
    
    // Update interval based on new quality
    if (m_adaptationEnabled) {
        updateInterval();
    }
}

void DapAdaptiveKeepAlive::onKeepAliveResponse(int responseTime)
{
    m_lastResponse = QDateTime::currentDateTime();
    m_successfulResponses++;
    m_missedResponses = 0;  // Reset missed counter on successful response
    
    // Record response time for analysis
    recordResponseTime(responseTime);
    
    qDebug() << "Keep-alive response received, time:" << responseTime << "ms"
             << "average:" << m_averageResponseTime << "ms"
             << "successful:" << m_successfulResponses;
    
    // Analyze response time for quality assessment
    if (m_adaptationEnabled) {
        // If response time is consistently good, consider upgrading quality
        if (responseTime < 100 && m_averageResponseTime < 150) {
            if (m_currentQuality == NetworkQuality::Good && m_successfulResponses > 5) {
                updateNetworkQuality(NetworkQuality::Excellent);
            } else if (m_currentQuality == NetworkQuality::Fair && m_successfulResponses > 3) {
                updateNetworkQuality(NetworkQuality::Good);
            } else if (m_currentQuality == NetworkQuality::Poor && m_successfulResponses > 2) {
                updateNetworkQuality(NetworkQuality::Fair);
            }
        }
        // If response time is getting worse, consider downgrading
        else if (responseTime > 2000 || m_averageResponseTime > 1500) {
            m_qualityDegradationCount++;
            if (m_qualityDegradationCount >= 3) {
                if (m_currentQuality == NetworkQuality::Excellent) {
                    updateNetworkQuality(NetworkQuality::Good);
                } else if (m_currentQuality == NetworkQuality::Good) {
                    updateNetworkQuality(NetworkQuality::Fair);
                } else if (m_currentQuality == NetworkQuality::Fair) {
                    updateNetworkQuality(NetworkQuality::Poor);
                }
                m_qualityDegradationCount = 0;
            }
        } else {
            m_qualityDegradationCount = 0;  // Reset on normal response
        }
    }
}

void DapAdaptiveKeepAlive::onKeepAliveTimeout()
{
    m_missedResponses++;
    
    qWarning() << "Keep-alive timeout, missed responses:" << m_missedResponses;
    
    if (m_missedResponses >= m_config.maxMissedResponses) {
        emit connectionUnhealthy(QString("Missed %1 consecutive keep-alive responses")
                                .arg(m_missedResponses));
        
        // Degrade network quality
        if (m_adaptationEnabled) {
            if (m_currentQuality == NetworkQuality::Excellent) {
                updateNetworkQuality(NetworkQuality::Good);
            } else if (m_currentQuality == NetworkQuality::Good) {
                updateNetworkQuality(NetworkQuality::Fair);
            } else if (m_currentQuality == NetworkQuality::Fair) {
                updateNetworkQuality(NetworkQuality::Poor);
            }
        }
    }
}

void DapAdaptiveKeepAlive::reset()
{
    qInfo() << "Resetting adaptive keep-alive state";
    
    m_missedResponses = 0;
    m_successfulResponses = 0;
    m_responseTimes.clear();
    m_averageResponseTime = 0;
    m_qualityDegradationCount = 0;
    m_currentQuality = NetworkQuality::Good;  // Reset to default
    
    updateInterval();
}

DapAdaptiveKeepAlive::Statistics DapAdaptiveKeepAlive::getStatistics() const
{
    Statistics stats;
    stats.currentInterval = m_currentInterval;
    stats.successfulResponses = m_successfulResponses;
    stats.missedResponses = m_missedResponses;
    stats.averageResponseTime = m_averageResponseTime;
    stats.currentQuality = m_currentQuality;
    stats.lastResponse = m_lastResponse;
    stats.adaptationActive = m_adaptationEnabled;
    
    return stats;
}

void DapAdaptiveKeepAlive::onTimerTimeout()
{
    m_lastKeepAliveSent = QDateTime::currentDateTime();
    emit sendKeepAlive();
    
    // Restart timer for next keep-alive
    if (m_timer->isActive()) {  // Safety check
        m_timer->start(m_currentInterval);
    }
}

void DapAdaptiveKeepAlive::updateInterval()
{
    int newInterval;
    
    if (m_adaptationEnabled) {
        newInterval = getQualityBasedInterval(m_currentQuality);
    } else {
        newInterval = m_config.baseInterval;
    }
    
    // Ensure interval is within bounds
    newInterval = std::max(m_config.minInterval, std::min(newInterval, m_config.maxInterval));
    
    if (newInterval != m_currentInterval) {
        int oldInterval = m_currentInterval;
        m_currentInterval = newInterval;
        
        qInfo() << "Keep-alive interval changed from" << oldInterval 
                << "to" << m_currentInterval << "ms"
                << "quality:" << static_cast<int>(m_currentQuality);
        
        emit intervalChanged(m_currentInterval);
        
        // Update timer if it's running
        if (m_timer->isActive()) {
            m_timer->stop();
            m_timer->start(m_currentInterval);
        }
    }
}

void DapAdaptiveKeepAlive::calculateAdaptiveInterval()
{
    // This method can be extended for more sophisticated interval calculation
    // based on historical data, time of day, network patterns, etc.
    
    int baseInterval = getQualityBasedInterval(m_currentQuality);
    
    // Adjust based on recent performance
    if (m_averageResponseTime > 0) {
        if (m_averageResponseTime > 1000) {
            // High latency - reduce interval
            baseInterval = static_cast<int>(baseInterval * 0.8);
        } else if (m_averageResponseTime < 200) {
            // Low latency - can increase interval
            baseInterval = static_cast<int>(baseInterval * 1.2);
        }
    }
    
    // Adjust based on success rate
    if (m_successfulResponses > 0) {
        int totalAttempts = m_successfulResponses + m_missedResponses;
        double successRate = static_cast<double>(m_successfulResponses) / totalAttempts;
        
        if (successRate < 0.8) {
            // Poor success rate - reduce interval
            baseInterval = static_cast<int>(baseInterval * 0.7);
        } else if (successRate > 0.95) {
            // Excellent success rate - can increase interval
            baseInterval = static_cast<int>(baseInterval * 1.1);
        }
    }
    
    m_currentInterval = std::max(m_config.minInterval, std::min(baseInterval, m_config.maxInterval));
}

int DapAdaptiveKeepAlive::getQualityBasedInterval(NetworkQuality quality) const
{
    switch (quality) {
        case NetworkQuality::Excellent:
            return m_config.excellentInterval;
        case NetworkQuality::Good:
            return m_config.goodInterval;
        case NetworkQuality::Fair:
            return m_config.fairInterval;
        case NetworkQuality::Poor:
            return m_config.poorInterval;
        default:
            return m_config.baseInterval;
    }
}

void DapAdaptiveKeepAlive::recordResponseTime(int responseTime)
{
    m_responseTimes.append(responseTime);
    
    // Keep only recent history
    while (m_responseTimes.size() > MAX_RESPONSE_HISTORY) {
        m_responseTimes.removeFirst();
    }
    
    // Calculate average response time
    if (!m_responseTimes.isEmpty()) {
        int sum = 0;
        for (int time : m_responseTimes) {
            sum += time;
        }
        m_averageResponseTime = sum / m_responseTimes.size();
    }
} 