#pragma once

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QElapsedTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

/**
 * @brief Real-time Network Quality Monitoring System
 * 
 * Continuously monitors network conditions including latency, throughput,
 * packet loss, and stability to provide quality assessments for adaptive
 * VPN behavior and reconnection strategies.
 */
class DapNetworkQualityMonitor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Network quality assessment levels
     */
    enum class Quality {
        Excellent,  // <50ms latency, >90% success rate, stable
        Good,       // <150ms latency, >80% success rate, mostly stable  
        Fair,       // <500ms latency, >60% success rate, some instability
        Poor,       // >500ms latency, <60% success rate, unstable
        Unknown     // Insufficient data for assessment
    };

    /**
     * @brief Configuration for network monitoring
     */
    struct Config {
        int pingInterval = 30000;           // Ping test interval (30 seconds)
        int rapidTestInterval = 5000;       // Rapid test interval during issues (5 seconds)
        int monitoringEnabled = true;       // Enable/disable monitoring
        int pingTimeout = 5000;             // Ping timeout (5 seconds)
        int sampleCount = 10;               // Number of samples for quality assessment
        int stabilityWindow = 60000;        // Time window for stability analysis (1 minute)
        
        // Quality thresholds
        struct Thresholds {
            int excellentLatency = 50;      // ms
            int goodLatency = 150;          // ms
            int fairLatency = 500;          // ms
            
            double excellentSuccessRate = 0.90;  // 90%
            double goodSuccessRate = 0.80;       // 80%
            double fairSuccessRate = 0.60;       // 60%
            
            int excellentJitter = 10;       // ms
            int goodJitter = 30;            // ms
            int fairJitter = 100;           // ms
        } thresholds;
        
        // Test endpoints for connectivity checks
        QStringList testEndpoints = {
            "8.8.8.8",          // Google DNS
            "1.1.1.1",          // Cloudflare DNS
            "208.67.222.222"    // OpenDNS
        };
    };

    explicit DapNetworkQualityMonitor(QObject *parent = nullptr);
    virtual ~DapNetworkQualityMonitor();

    /**
     * @brief Configure network monitoring parameters
     */
    void setConfig(const Config &config);
    Config getConfig() const { return m_config; }

    /**
     * @brief Start network quality monitoring
     */
    void startMonitoring();
    
    /**
     * @brief Stop network quality monitoring
     */
    void stopMonitoring();
    
    /**
     * @brief Check if monitoring is active
     */
    bool isMonitoring() const { return m_monitorTimer->isActive(); }

    /**
     * @brief Get current network quality assessment
     */
    Quality getCurrentQuality() const { return m_currentQuality; }
    
    /**
     * @brief Trigger immediate quality assessment
     */
    void performImmediateAssessment();

    /**
     * @brief Report external network event (e.g., connection drop)
     */
    void reportNetworkEvent(const QString &event, int severity = 1);

    /**
     * @brief Get comprehensive network statistics
     */
    struct Statistics {
        Quality currentQuality;
        double averageLatency;
        double currentJitter;
        double packetLossRate;
        int successfulTests;
        int failedTests;
        QDateTime lastSuccessfulTest;
        QDateTime lastQualityChange;
        bool isStable;
        QString qualityReason;
        
        // Historical data
        QList<double> latencyHistory;
        QList<Quality> qualityHistory;
    };
    Statistics getStatistics() const;

signals:
    /**
     * @brief Emitted when network quality changes
     */
    void qualityChanged(Quality newQuality, Quality oldQuality);
    
    /**
     * @brief Emitted when quality significantly degrades
     */
    void qualityDegraded(const QString &reason);
    
    /**
     * @brief Emitted when quality improves
     */
    void qualityImproved(const QString &reason);
    
    /**
     * @brief Emitted for each completed network test
     */
    void testCompleted(bool success, double latency, const QString &endpoint);
    
    /**
     * @brief Emitted when network appears unstable
     */
    void networkUnstable(const QString &details);
    
    /**
     * @brief Emitted with periodic statistics updates
     */
    void statisticsUpdated(const Statistics &stats);

private slots:
    void performQualityTest();
    void onPingReplyFinished();
    void onPingTimeout();

private:
    void calculateQuality();
    void analyzeLatencyPattern();
    void updateStabilityMetrics();
    void recordTestResult(bool success, double latency, const QString &endpoint);
    QString getQualityReason() const;
    void switchToRapidTesting();
    void switchToNormalTesting();
    
    Config m_config;
    QTimer *m_monitorTimer;
    QTimer *m_pingTimeoutTimer;
    QNetworkAccessManager *m_networkManager;
    
    // Current state
    Quality m_currentQuality;
    bool m_isRapidTesting;
    int m_currentEndpointIndex;
    QElapsedTimer m_pingStartTime;
    
    // Test results storage
    struct TestResult {
        QDateTime timestamp;
        bool success;
        double latency;
        QString endpoint;
    };
    QList<TestResult> m_testResults;
    static const int MAX_TEST_HISTORY = 100;
    
    // Statistics
    double m_averageLatency;
    double m_currentJitter;
    double m_packetLossRate;
    int m_successfulTests;
    int m_failedTests;
    QDateTime m_lastSuccessfulTest;
    QDateTime m_lastQualityChange;
    bool m_isStable;
    
    // Quality analysis
    QList<double> m_latencyHistory;
    QList<Quality> m_qualityHistory;
    static const int MAX_LATENCY_HISTORY = 50;
    static const int MAX_QUALITY_HISTORY = 20;
    
    // Network events
    struct NetworkEvent {
        QDateTime timestamp;
        QString event;
        int severity;
    };
    QList<NetworkEvent> m_networkEvents;
    static const int MAX_EVENT_HISTORY = 50;
    
    // Internal helpers
    void cleanupOldData();
    double calculateJitter() const;
    bool isNetworkStable() const;
    Quality assessQualityFromMetrics() const;
};

Q_DECLARE_METATYPE(DapNetworkQualityMonitor::Quality) 