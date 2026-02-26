#ifndef VPNDEFINE_H
#define VPNDEFINE_H

enum ConnectionState {
    ConnectingToService,
    ServersListLoading,
    Disconnected,
    Connecting,
    Connected,
    Reconnecting,
    Disconnecting,
    ServerChanging,
    NoCDBConnecting,
    ConnectionError
};

#endif // VPNDEFINE_H
