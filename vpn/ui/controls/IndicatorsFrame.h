#ifndef INDICATORSFRAME_H
#define INDICATORSFRAME_H

#include <QFrame>
#include <QVBoxLayout>
#include <QCheckBox>

class IndicatorsFrame : public QFrame
{
    Q_OBJECT
public:
    IndicatorsFrame(QWidget* a_parent = nullptr);

public slots:
    void setAuthorized(bool a_authorized = true);
    void setStreamOpened(bool a_streamOpened = true);
    void setVirtualNetwork(bool a_virtualNetwork = true);
private:
    QVBoxLayout*    m_layout{Q_NULLPTR};
    QCheckBox*      m_authorized{Q_NULLPTR};
    QCheckBox*      m_streamOpened{Q_NULLPTR};
    QCheckBox*      m_virtualNetwork{Q_NULLPTR};
};

#endif // INDICATORSFRAME_H
