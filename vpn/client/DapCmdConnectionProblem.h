#ifndef DAPCMDCONNECTIONPROBLEM_H
#define DAPCMDCONNECTIONPROBLEM_H

#include "DapCmdAbstract.h"

/**
 * @brief The DapCmdConnectionProblem class
 * @details Handles connection problem dialog commands
 */
class DapCmdConnectionProblem : public DapCmdAbstract
{
    Q_OBJECT

public:
    explicit DapCmdConnectionProblem(QObject *parent = nullptr);

    // Command types
    enum CommandType {
        ShowDialog,
        RetryCurrentServer,
        SelectNewServer,
        DialogClosed
    };

    void sendShowDialog(const QString& currentServer, const QString& errorMessage);
    void sendDialogClosed();

public slots:
    void slotRetryCurrentServer();
    void slotSelectNewServer();
    void slotDialogClosed();

protected:
    void handle(const QJsonObject *rcvdObj) override;

signals:
    void sigShowConnectionProblemDialog(const QString& currentServer, const QString& errorMessage);
    void sigRetryCurrentServer();
    void sigSelectNewServer();
    void sigDialogClosed();

private:
    void handleShowDialog(const QJsonObject& obj);
    void handleRetryCurrentServer(const QJsonObject& obj);
    void handleSelectNewServer(const QJsonObject& obj);
    void handleDialogClosed(const QJsonObject& obj);
};

#endif // DAPCMDCONNECTIONPROBLEM_H 