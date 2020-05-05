/****************************************************************************
**
** This file is part of the DapVpn project.
**
** The class implements the command handler creating android tunnel.
**
****************************************************************************/

#ifndef DAPCMDANDROIDTUNNELHANDLER_H
#define DAPCMDANDROIDTUNNELHANDLER_H

#include <QObject>


class DapCmdAndroidTunnelHandler : public QObject
{
    Q_OBJECT

    /// Server address.
    static const QString addressParam;
    /// VPN gateway.
    static const QString gwParam;
    /// File descriptor of socket.
    static const QString upstreamParam;

public:
    /// Standart consructor.
    /// @param parent Parent.
    DapCmdAndroidTunnelHandler(QObject *parent = nullptr) { Q_UNUSED(parent)}

    /// Process command.
    /// @param params Command parameters.
    void handler(const QJsonObject* params);
    /// Process and verify command parameters.
    /// @param address VPN server address.
    /// @param gateway VPN gateway.
    /// @param upstreamSocket File descriptor of socket.
    void tunnelHandler(const QJsonValue& address, const QJsonValue& gateway, const QJsonValue& upstreamSocket);

signals:
    /// The signal emitted in case of successful reception and verification of tunnel params.
    /// @param address VPN server address.
    /// @param gateway VPN gateway.
    /// @param upstreamSocket File descriptor of socket.
    void sigTunnelCreate(const QString& address, const QString& gateway, const int fd);
};

#endif // DAPCMDUSERDATAHANDLER_H
