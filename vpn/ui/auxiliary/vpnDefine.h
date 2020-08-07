#ifndef VPNDEFINE_H
#define VPNDEFINE_H



enum ConnectionState {
    ConnectingToService,
    Disconnected,
    Connecting,
    Connected,
    Reconnecting,
    Disconnecting,
    ServerChanging
};

#endif // VPNDEFINE_H
