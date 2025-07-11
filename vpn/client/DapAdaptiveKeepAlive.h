#pragma once

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

/**
 * @brief Adaptive Keep-Alive Management System
 * 
 * Dynamically adjusts keep-alive intervals based on network conditions,
 * connection stability, and response times to optimize connection maintenance
 * while minimizing unnecessary network traffic.
 */
class DapAdaptiveKeepAlive : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Network quality levels for adaptive timing
     */
    enum class NetworkQuality {
        Excellent,  // Very stable, high bandwidth, low latency
        Good,       // Stable connection, normal performance
        Fair,       // Some instability, moderate performance
        Poor        // Unstable connection, high latency
    };

    /**
     * @brief Configuration for adaptive keep-alive behavior
     */
    struct Config {
        int minInterval = 3000;     // Minimum keep-alive interval (3 seconds)
        int maxInterval = 30000;    // Maximum keep-alive interval (30 seconds)
        int baseInterval = 6000;    // Base keep-alive interval (6 seconds - current default)
        int responseTimeout = 5000; // Timeout waiting for keep-alive response (5 seconds)
        int maxMissedResponses = 3; // Maximum missed responses before quality downgrade
        bool enableAdaptation = true; // Enable/disable adaptive behavior
        
        // Quality-specific intervals
        int excellentInterval = 15000; // 15 seconds for excellent quality
        int goodInterval = 8000;       // 8 seconds for good quality  
        int fairInterval = 5000;       // 5 seconds for fair quality
        int poorInterval = 3000;       // 3 seconds for poor quality
    };

    explicit DapAdaptiveKeepAlive(QObject *parent = nullptr);
    virtual ~DapAdaptiveKeepAlive();

    /**
     * @brief Configure adaptive keep-alive parameters
     */
    void setConfig(const Config &config);
    Config getConfig() const { return m_config; }

    /**
     * @brief Start adaptive keep-alive monitoring
     */
    void start();
    
    /**
     * @brief Stop keep-alive monitoring
     */
    void stop();
    
    /**
     * @brief Check if keep-alive is currently active
     */
    bool isActive() const { return m_timer->isActive(); }

    /**
     * @brief Update network quality assessment
     */
    void updateNetworkQuality(NetworkQuality quality);
    
    /**
     * @brief Report successful keep-alive response
     * @param responseTime Response time in milliseconds
     */
    void onKeepAliveResponse(int responseTime);
    
    /**
     * @brief Report missed/failed keep-alive response
     */
    void onKeepAliveTimeout();
    
    /**
     * @brief Reset adaptive state (on reconnection)
     */
    void reset();

    /**
     * @brief Get current statistics
     */
    struct Statistics {
        int currentInterval;
        int successfulResponses;
        int missedResponses;
        int averageResponseTime;
        NetworkQuality currentQuality;
        QDateTime lastResponse;
        bool adaptationActive;
    };
    Statistics getStatistics() const;

signals:
    /**
     * @brief Emitted when it's time to send a keep-alive packet
     */
    void sendKeepAlive();
    
    /**
     * @brief Emitted when keep-alive interval changes
     */
    void intervalChanged(int newInterval);
    
    /**
     * @brief Emitted when network quality assessment changes
     */
    void qualityChanged(NetworkQuality newQuality);
    
    /**
     * @brief Emitted when connection appears unhealthy
     */
    void connectionUnhealthy(const QString &reason);

private slots:
    void onTimerTimeout();

private:
    void updateInterval();
    void calculateAdaptiveInterval();
    int getQualityBasedInterval(NetworkQuality quality) const;
    void recordResponseTime(int responseTime);
    
    Config m_config;
    QTimer *m_timer;
    
    // State tracking
    NetworkQuality m_currentQuality;
    int m_currentInterval;
    int m_missedResponses;
    int m_successfulResponses;
    
    // Response time tracking
    QList<int> m_responseTimes;
    static const int MAX_RESPONSE_HISTORY = 20;
    int m_averageResponseTime;
    
    // Timestamps
    QDateTime m_lastKeepAliveSent;
    QDateTime m_lastResponse;
    
    // Adaptation state
    bool m_adaptationEnabled;
    int m_qualityDegradationCount;
};

Q_DECLARE_METATYPE(DapAdaptiveKeepAlive::NetworkQuality) 