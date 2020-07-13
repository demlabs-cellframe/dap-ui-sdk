#ifndef VPNDEFINE_H
#define VPNDEFINE_H


enum ConnectionStates {
    Disconnected = 0,
    Connecting   = 1,
    Connected    = 2,
    Disconnecting = 3,
    ServerChanging = 4
};

#endif // VPNDEFINE_H
