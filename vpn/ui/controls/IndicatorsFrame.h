#ifndef INDICATORSFRAME_H
#define INDICATORSFRAME_H

#include <QFrame>

class IndicatorsFrame : public QFrame
{
    Q_OBJECT
public:
    IndicatorsFrame();

public slots:
    void setAuthorized(bool a_authorized = true);
    void setStreamOpened(bool a_streamOpened = true);
    void setVirtualNetwork(bool a_virtualNetwork = true);
};

#endif // INDICATORSFRAME_H
