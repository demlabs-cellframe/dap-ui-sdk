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
    ServerChanging
};

#endif // VPNDEFINE_H
